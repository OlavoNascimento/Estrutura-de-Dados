#include "fase_1.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../../Estruturas/lista.h"
#include "../../Interfaces/figura.h"
#include "../../Objetos/Formas/circulo.h"
#include "../../Objetos/Formas/linha.h"
#include "../../Objetos/Formas/retangulo.h"
#include "../../Objetos/Outros/texto.h"
#include "../../Utils/matematica.h"

// Margem entre o retângulo criado pela função envolver_figuras e as figuras que ele envolve.
#define MARGEM_CONTORNO 2

Retangulo criar_delimitacao_figuras(Figura figura1, Figura figura2) {
    // Coordenada x do contorno é a menor coordenada x entre as duas figuras.
    double x =
        min(figura_obter_x_inicio(figura1), figura_obter_x_inicio(figura2)) - MARGEM_CONTORNO;
    // Coordenada y do contorno é a menor coordenada y entre as duas figuras.
    double y =
        min(figura_obter_y_inicio(figura1), figura_obter_y_inicio(figura2)) - MARGEM_CONTORNO;

    // Largura do contorno é a distância entre o x do contorno e a coordenada x onde o fim da
    // figura mais longe se encontra.
    double largura =
        max(figura_obter_x_fim(figura1), figura_obter_x_fim(figura2)) - x + MARGEM_CONTORNO;
    // Altura do contorno é a distância entre o y do contorno e a coordenada y onde o fim da figura
    // mais longe se encontra.
    double altura =
        max(figura_obter_y_fim(figura1), figura_obter_y_fim(figura2)) - y + MARGEM_CONTORNO;

    return retangulo_criar("", largura, altura, x, y, "black", "none");
}

// Executa o comando o? especificado no arquivo de consulta, verificando se duas figuras se
// sobrepõem.
void checar_interseccao(Lista formas, Tabela id_forma, const char *linha, FILE *arquivo_log) {
    char id1[100], id2[100];
    sscanf(linha, "o? %s %s", id1, id2);

    ListaNo no1 = tabela_buscar(id_forma, id1);
    ListaNo no2 = tabela_buscar(id_forma, id2);
    if (no1 == NULL || no2 == NULL)
        return;
    Figura fig1 = lista_obter_info(no1);
    Figura fig2 = lista_obter_info(no2);

    const char *tipo_fig1 = figura_obter_tipo(fig1);
    const char *tipo_fig2 = figura_obter_tipo(fig2);
    bool intersectam = false;
    if (strcmp(tipo_fig1, "círculo") == 0 && strcmp(tipo_fig1, tipo_fig2) == 0)
        intersectam = circulo_checar_interseccao((Circulo) fig1, (Circulo) fig2);
    else if (strcmp(tipo_fig1, "retângulo") == 0 && strcmp(tipo_fig1, tipo_fig2) == 0)
        intersectam = retangulo_checar_interseccao((Retangulo) fig1, (Retangulo) fig2);
    else if (strcmp(tipo_fig1, "círculo") == 0 && strcmp(tipo_fig2, "retângulo") == 0)
        intersectam = circulo_intersecta_retangulo((Circulo) fig1, (Retangulo) fig2);
    else if (strcmp(tipo_fig1, "retângulo") == 0 && strcmp(tipo_fig2, "círculo") == 0)
        intersectam = circulo_intersecta_retangulo((Circulo) fig2, (Retangulo) fig1);
    else
        return;

    Retangulo contorno = criar_delimitacao_figuras(fig1, fig2);
    if (intersectam) {
        // Adiciona uma mensagem de sobreposição caso as figuras se intersectem.
        Texto aviso = texto_criar("", figura_obter_x_centro(contorno),
                                  figura_obter_y_centro(contorno) + 4, "None", "Black", "sobrepoe");
        texto_definir_alinhamento(aviso, TEXTO_CENTRO);
        lista_inserir_final(formas, aviso);
    } else {
        // Adiciona traços a borda do retângulo de contorno caso as figuras não se intersectem.
        retangulo_definir_borda_tracejada(contorno, true);
    }
    lista_inserir_final(formas, contorno);

    fprintf(arquivo_log, "o? %s %s\n", id1, id2);
    fprintf(arquivo_log, "%s: %s %s: %s %s\n\n", id1, tipo_fig1, id2, tipo_fig2,
            intersectam ? "SIM" : "NAO");
}

// Executa o comando i? especificado no arquivo de consulta, verificando se um ponto é interno a uma
// figura, conecta este ponto e a figura utilizando uma linha.
void checar_ponto_interno(Lista formas, Tabela id_forma, const char *linha, FILE *arquivo_log) {
    char id[100];
    double ponto_x = 0, ponto_y = 0;
    sscanf(linha, "i? %s %lf %lf", id, &ponto_x, &ponto_y);

    ListaNo no = tabela_buscar(id_forma, id);
    if (no == NULL)
        return;
    Figura figura = lista_obter_info(no);

    const char *tipo_figura = figura_obter_tipo(figura);
    bool interno = false;
    if (strcmp(tipo_figura, "círculo") == 0)
        interno = circulo_checar_ponto_interno(figura, ponto_x, ponto_y);
    else if (strcmp(tipo_figura, "retângulo") == 0)
        interno = retangulo_checar_ponto_interno(figura, ponto_x, ponto_y);
    else
        return;

    Circulo ponto = circulo_criar("", 1, ponto_x, ponto_y, interno ? "blue" : "magenta",
                                  interno ? "blue" : "magenta");
    lista_inserir_final(formas, ponto);

    Linha ligacao = linha_criar(figura_obter_x_centro(ponto), figura_obter_y_centro(ponto),
                                figura_obter_x_centro(figura), figura_obter_y_centro(figura),
                                figura_obter_cor_preenchimento(ponto));
    lista_inserir_final(formas, ligacao);

    fprintf(arquivo_log, "i? %s %lf %lf\n", id, ponto_x, ponto_y);
    fprintf(arquivo_log, "%s: %s %s\n\n", id, figura_obter_tipo(figura),
            interno ? "INTERNO" : "NAO INTERNO");
}

// Executa o comando pnt especificado no arquivo de consulta, alterando a cor de preenchimento e
// borda da figura com id igual ao id que segue o comando pnt.
void alterar_cor(Tabela id_forma, const char *linha, FILE *arquivo_log) {
    char id[100], nova_corb[20], nova_corp[20];
    sscanf(linha, "pnt %s %s %s", id, nova_corb, nova_corp);

    ListaNo no = tabela_buscar(id_forma, id);
    if (no == NULL)
        return;
    Figura fig = lista_obter_info(no);

    fprintf(arquivo_log, "pnt %s %s %s\n", id, nova_corb, nova_corp);
    fprintf(arquivo_log, "x: %lf, y: %lf\n\n", figura_obter_x(fig), figura_obter_y(fig));

    figura_definir_cor_borda(fig, nova_corb);
    figura_definir_cor_preenchimento(fig, nova_corp);
}

// Executa o comando pnt* especificado no arquivo de consulta, alterando a cor de preenchimento e
// borda de todas as figuras entre id_inicial e id_final (inclusive).
void alterar_cores(Lista formas, const char *linha, FILE *arquivo_log) {
    char id_inicial[100], id_final[100], nova_corb[20], nova_corp[20];
    sscanf(linha, "pnt* %s %s %s %s", id_inicial, id_final, nova_corb, nova_corp);

    bool inicio_encontrado = false;
    for_each_lista(no, formas) {
        Figura fig = lista_obter_info(no);
        const char *id_atual = figura_obter_id(fig);

        if (inicio_encontrado || strcmp(id_atual, id_inicial) == 0) {
            fprintf(arquivo_log, "pnt* %s %s %s %s\n", id_inicial, id_final, nova_corb, nova_corp);
            fprintf(arquivo_log, "x: %lf, y: %lf\n\n", figura_obter_x(fig), figura_obter_y(fig));

            figura_definir_cor_borda(fig, nova_corb);
            figura_definir_cor_preenchimento(fig, nova_corp);
            inicio_encontrado = true;
        }
        if (strcmp(id_atual, id_final) == 0)
            break;
    }
}

// Executa o comando delf especificado no arquivo de consulta, removendo a figura com id igual ao id
// que segue o comando delf.
void remover_elemento(Lista formas, Tabela id_forma, const char *linha, FILE *arquivo_log) {
    char id[100];
    sscanf(linha, "delf %s", id);
    ListaNo no = tabela_buscar(id_forma, id);
    if (no == NULL)
        return;
    Figura fig = lista_obter_info(no);

    fprintf(arquivo_log, "delf %s\n", id);
    figura_escrever_informacoes(fig, arquivo_log);
    fprintf(arquivo_log, "\n");

    lista_remover(formas, no);
    tabela_remover(id_forma, figura_obter_id(fig));
    figura_destruir(fig);
}

// Executa o comando delf* especificado pelo arquivo de consulta, removendo todas as figuras que se
// encontrem entre id_inicial e id_final (inclusive).
void remover_elementos(Lista formas, Tabela id_forma, const char *linha, FILE *arquivo_log) {
    char id_inicial[100], id_final[100];
    sscanf(linha, "delf* %s %s", id_inicial, id_final);

    bool inicio_encontrado = false;
    ListaNo atual = lista_obter_inicio(formas);
    while (atual != NULL) {
        Figura fig = lista_obter_info(atual);
        const char *id_atual = figura_obter_id(fig);
        ListaNo proximo = lista_obter_proximo(atual);

        if (strcmp(id_atual, id_final) == 0)
            proximo = NULL;
        if (inicio_encontrado || strcmp(id_atual, id_inicial) == 0) {
            fprintf(arquivo_log, "delf* %s %s\n", id_inicial, id_final);
            figura_escrever_informacoes(fig, arquivo_log);
            fprintf(arquivo_log, "\n");
            inicio_encontrado = true;
        }

        lista_remover(formas, atual);
        tabela_remover(id_forma, figura_obter_id(fig));
        figura_destruir(fig);
        atual = proximo;
    }
}
