#include "ler_qry.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "circulo.h"
#include "figuras.h"
#include "linha.h"
#include "lista.h"

// Tamanho maxímo de um comando do arquivo de consulta.
#define LINHA_MAX 300
// Determina o tamanho e espaçamento da borda do retângulo que envolve duas figuras.
#define CONTORNO_TRACEJADO_TAMANHO 2
// Margem entre o retângulo criado pela função envolver_figuras e as figuras que ele envolve.
#define MARGEM_CONTORNO 2
// Tamanho estimado para cada letra de um texto
#define MARGEM_LETRAS_TEXTO 4

// Cria o texto "sobrepoe" no centro de um retângulo.
void criar_texto_sobreposicao(Texto *aviso, Figura contorno) {
    linha_definir_id(aviso, "\0");
    linha_definir_x(aviso, figura_obter_centro_x(contorno) - strlen("sobrepoe"));
    linha_definir_y(aviso, figura_obter_centro_y(contorno));
    linha_definir_cor_borda(aviso, "None");
    linha_definir_cor_preenchimento(aviso, "Black");
    linha_definir_texto(aviso, "sobrepoe");
}

// Cria um retângulo com coordenadas e dimensões necessárias para envolver duas figuras.
Retangulo *criar_delimitacao_figuras(Figura figura1, Figura figura2) {
    Retangulo ret = retangulo_criar(
        "\0",
        // Largura
        0,
        // Altura
        0,
        // Coordenada x do contorno é a menor coordenada x entre as duas figuras.
        min(figura_obter_x_inicio(figura1), figura_obter_x_inicio(figura2)) - MARGEM_CONTORNO,
        // Coordenada y do contorno é a menor coordenada y entre as duas figuras.
        min(figura_obter_y_inicio(figura1), figura_obter_y_inicio(figura2)) - MARGEM_CONTORNO,
        // Cor borda
        "black",
        // Cor Preenchimento
        "none",
        // Tracejado
        0,
        // Espaço Tracejado
        0);

    // Largura do contorno é a distancia entre o x do contorno e a coordenada x onde o fim da figura
    // mais longe se encontra.
    retangulo_definir_largura(ret,
                              max(figura_obter_x_fim(figura1), figura_obter_x_fim(figura2)) -
                                  retangulo_obter_x(ret) + 2 * MARGEM_CONTORNO);
    // Altura do contorno é a distancia entre o y do contorno e a coordenada y onde o fim da figura
    // mais longe se encontra.
    retangulo_definir_altura(ret,
                             max(figura_obter_y_fim(figura1), figura_obter_y_fim(figura2)) -
                                 retangulo_obter_y(ret) + 2 * MARGEM_CONTORNO);
    return ret;
}

// Executa o comando o? especificado no arquivo de consulta, verificando se um ponto é interno a uma
// figura, conecta este ponto e a figura utilizando uma linha.
void checar_interseccao(Lista *lista, const char *linha, FILE *arquivo_log) {
    char id1[100], id2[100];
    sscanf(linha, "o? %s %s", id1, id2);

    No *no1 = lista_get_posic(lista, id1);
    No *no2 = lista_get_posic(lista, id2);
    if (no1 == NULL || no2 == NULL)
        return;

    bool intersectam = figura_checar_interseccao(no1->figura, no2->figura);

    Retangulo ret = criar_delimitacao_figuras(no1->figura, no2->figura);
    Figura contorno = figura_criar(ret, TIPO_RETANGULO);

    if (intersectam) {
        // Adiciona uma mensagem de sobreposição caso as figuras se intersectem.
        Figura aviso;
        criar_texto_sobreposicao(&aviso, contorno);
        lista_insert_final(lista, aviso);
    } else {
        // Adiciona traços a borda do retângulo de contorno caso as figuras não se intersectem.
        retangulo_definir_tracejado_tamanho(figura_obter_figura(contorno), CONTORNO_TRACEJADO_TAMANHO);
        retangulo_definir_tracejado_espaco(figura_obter_figura(contorno), CONTORNO_TRACEJADO_TAMANHO);
    }
    lista_insert_final(lista, contorno);

    fprintf(arquivo_log,
            "o? %s %s\n", id1, id2);
    fprintf(arquivo_log,
            "%s: %s %s: %s %s\n\n",
            id1, figura_tipo_para_string(no1->figura),
            id2, figura_tipo_para_string(no2->figura),
            intersectam ? "SIM" : "NAO");
}

// Cria um círculo com as coordenadas especificadas e com cores que dependem de
// um valor booleano.
Circulo criar_ponto(double ponto_x, double ponto_y, bool interno) {
    Circulo ponto = circulo_criar(
        "\0",
        1,
        ponto_x,
        ponto_y,
        interno ? "blue" : "magenta",
        interno ? "blue" : "magenta");
    return ponto;
}

// Cria uma linha que conecta um ponto ao centro de uma figura
Circulo criar_ligacao_ponto_figura(Circulo ponto, Figura figura) {
    return linha_criar(
        circulo_obter_x(ponto),
        circulo_obter_y(ponto),
        figura_obter_centro_x(figura),
        figura_obter_centro_y(figura),
        circulo_obter_cor_borda(ponto),
        circulo_obter_cor_preenchimento(ponto));
}

// Executa o comando i? especificado no arquivo de consulta, verificando se um
// ponto é interno a uma figura, conecta este ponto e a figura utilizando uma
// linha.
void checar_ponto_interno(Lista *lista, const char *linha, FILE *arquivo_log) {
    char id[100];
    double ponto_x = 0, ponto_y = 0;
    sscanf(linha, "i? %s %lf %lf", id, &ponto_x, &ponto_y);

    No *no = lista_get_posic(lista, id);
    if (no == NULL)
        return;

    bool interno = figura_checar_ponto_interno(no->figura, ponto_x, ponto_y);

    Circulo circ = criar_ponto(ponto_x, ponto_y, interno);
    Figura ponto = figura_criar(circ, TIPO_CIRCULO);
    lista_insert_final(lista, ponto);

    Linha lin = criar_ligacao_ponto_figura(circ, no->figura);
    Figura ligacao = figura_criar(lin, TIPO_CIRCULO);
    lista_insert_final(lista, ligacao);

    fprintf(arquivo_log, "i? %s %lf %lf\n", id, ponto_x, ponto_y);
    fprintf(arquivo_log,
            "%s: %s %s\n\n",
            id, figura_obter_string_tipo(no->figura),
            interno ? "INTERNO" : "NAO INTERNO");
}

// Executa o comando pnt especificado no arquivo de consulta, alterando a cor de
// preenchimento e borda da figura com id igual ao id que segue o comando pnt.
void alterar_cor(Lista *lista, const char *linha, FILE *arquivo_log) {
    char id[100], nova_corb[20], nova_corp[20];
    sscanf(linha, "pnt %s %s %s", id, nova_corb, nova_corp);

    No *no = lista_get_posic(lista, id);
    if (no == NULL)
        return;

    const char *atual_corp = figura_obter_cor_preenchimento(&no->figura);
    const char *atual_corb = figura_obter_cor_borda(&no->figura);

    fprintf(arquivo_log, "pnt %s %s %s\n", id, nova_corb, nova_corp);
    fprintf(arquivo_log, "corb: %s, corp: %s\n\n", atual_corb, atual_corp);

    alterar_cor_figura_borda(&no->figura, nova_corb);
    alterar_cor_figura_preenchimento(&no->figura, nova_corp);
}

// Executa o comando pnt* especificado no arquivo de consulta, alterando a cor
// de preenchimento e borda de todas as figuras entre id_inicial e id_final.
// (inclusive).
void alterar_cores(Lista *lista, const char *linha, FILE *arquivo_log) {
    char id_inicial[100], id_final[100], nova_corb[20], nova_corp[20];
    sscanf(linha, "pnt* %s %s %s %s", id_inicial, id_final, nova_corb, nova_corp);
    No *atual = lista_get_posic(lista, id_inicial);
    if (atual == NULL)
        return;

    while (atual != NULL) {
        const char *id_atual = figura_obter_id(&atual);
        const char *atual_corp = figura_obter_cor_preenchimento(&atual);
        const char *atual_corb = figura_obter_cor_borda(&atual);

        fprintf(arquivo_log, "pnt* %s %s %s %s\n", id_inicial, id_final, nova_corb, nova_corp);
        fprintf(arquivo_log, "corb: %s, corp: %s\n\n", atual_corb, atual_corp);

        alterar_cor_figura_borda(&atual, nova_corb);
        alterar_cor_figura_preenchimento(&atual, nova_corp);
        if (strcmp(id_atual, id_final) == 0)
            break;
        atual = atual->prox;
    }
}

// Executa o comando delf especificado no arquivo de consulta, removendo a
// figura com id igual ao id que segue o comando delf.
void remover_elemento(Lista *lista, const char *linha, FILE *arquivo_log) {
    char id[100];
    sscanf(linha, "delf %s", id);
    No *no = lista_get_posic(lista, id);
    if (no == NULL)
        return;

    fprintf(arquivo_log, "delf %s\n", id);
    figura_escrever_informacoes(arquivo_log, no->figura);
    fprintf(arquivo_log, "\n");
    lista_remove_no(lista, no);
}

// Executa o comando delf* especificado pelo arquivo de consulta, removendo
// todas as figuras que se encontrem entre id_inicial e id_final (inclusive).
void remover_elementos(Lista *lista, const char *linha, FILE *arquivo_log) {
    char id_inicial[100], id_final[100];
    sscanf(linha, "delf* %s %s", id_inicial, id_final);

    No *atual = lista_get_posic(lista, id_inicial);
    while (atual != NULL) {
        const char *id_atual = figura_obter_id(&atual->figura);

        fprintf(arquivo_log, "delf* %s %s\n", id_inicial, id_final);
        figura_escrever_informacoes(arquivo_log, atual->figura);
        fprintf(arquivo_log, "\n");

        if (strcmp(id_atual, id_final) == 0) {
            // É necessário marcar a saída do loop antes de remover o elemento,
            // já que após a remoção do elemento seu id será perdido.
            atual = NULL;
        } else {
            atual = atual->prox;
        }
        lista_remove_no(lista, atual);
    }
}

// Ler o arquivo de consulta localizado no caminho fornecido a função e itera
// por todas as suas linhas, executando funções correspondentes aos comandos.
void ler_qry(Lista *lista, const char *caminho_qry, const char *caminho_log) {
    FILE *arquivo_consulta = fopen(caminho_qry, "r");
    if (arquivo_consulta == NULL) {
        fprintf(stderr, "ERRO: Falha ao ler arquivo de consulta: %s!\n", caminho_qry);
        return;
    }
    FILE *arquivo_log = fopen(caminho_log, "w");
    if (arquivo_log == NULL) {
        fprintf(stderr, "ERRO: Falha ao criar arquivo de log: %s!\n", caminho_log);
        return;
    }

    char linha[LINHA_MAX];
    while (fgets(linha, LINHA_MAX, arquivo_consulta) != NULL) {
        char comando[100];
        sscanf(linha, "%s", comando);
        if (strcmp("o?", comando) == 0) {
            checar_interseccao(lista, linha, arquivo_log);
        } else if (strcmp("i?", comando) == 0) {
            checar_ponto_interno(lista, linha, arquivo_log);
        } else if (strcmp("pnt", comando) == 0) {
            alterar_cor(lista, linha, arquivo_log);
        } else if (strcmp("pnt*", comando) == 0) {
            alterar_cores(lista, linha, arquivo_log);
        } else if (strcmp("delf", comando) == 0) {
            remover_elemento(lista, linha, arquivo_log);
        } else if (strcmp("delf*", comando) == 0) {
            remover_elementos(lista, linha, arquivo_log);
        }
    }

    fclose(arquivo_log);
    fclose(arquivo_consulta);
}
