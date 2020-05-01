#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ler_qry.h>
#include <lista.h>
#include <figuras.h>

// Tamanho maxímo de um comando do arquivo de consulta.
#define LINHA_MAX 300
// Determina o tamanho e espaçamento da borda do retângulo que envolve duas
// figuras.
#define TRACEJADO_TAMANHO 2

// Cria o texto "sobrepoe" no centro de um retângulo.
Texto criar_texto_sobreposicao(Retangulo contorno) {
    Texto aviso = {
        .id = "\0",
        .x = contorno.x + contorno.largura/2 - 18,
        .y = contorno.y + contorno.altura/2,
        .cor_borda = "None",
        .cor_preenchimento = "Black",
        .texto = "sobrepoe",
    };
    return aviso;
}

// Executa o comando o? especificado no arquivo de consulta, verificando se um
// ponto é interno a uma figura, conecta este ponto e a figura utilizando uma
// linha.
void checar_interseccao(Lista *lista, const char *linha, FILE *arquivo_log) {
    char id1[100], id2[100];
    sscanf(linha, "o? %s %s", id1, id2);

    struct No *no1 = buscar_elemento_lista(lista, id1);
    struct No *no2 = buscar_elemento_lista(lista, id2);
    if(no1 == NULL || no2 == NULL)
        return;

    bool intersectam = checar_interseccao_figuras(
        no1->figura, no1->tipo,
        no2->figura, no2->tipo
    );
    Figuras contorno;
    contorno.ret = envolver_figuras(
        no1->figura, no1->tipo,
        no2->figura, no2->tipo
    );
    if(intersectam) {
        // Adiciona uma mensagem de sobreposição caso as figuras se intersectem.
        Figuras aviso;
        aviso.tex = criar_texto_sobreposicao(contorno.ret);
        inserir_lista(lista, aviso, TIPO_TEXTO);
    } else {
        // Adiciona traços a borda do retângulo de contorno caso as figuras não
        // se intersectem.
        contorno.ret.tracejado_tamanho = TRACEJADO_TAMANHO;
        contorno.ret.tracejado_espaco = TRACEJADO_TAMANHO;
    }
    inserir_lista(lista, contorno, TIPO_RETANGULO);

    fprintf(arquivo_log,
     "o? %s %s\n", id1, id2);
    fprintf(arquivo_log,
            "%s: %s %s: %s %s\n\n",
            id1, tipo_para_string_figura(no1->tipo),
            id2, tipo_para_string_figura(no2->tipo),
            intersectam ? "SIM" : "NAO"
    );
}

// Cria um círculo com as coordenadas especificadas e com cores que dependem de
// um valor booleano.
Circulo criar_ponto(double ponto_x, double ponto_y, bool interno) {
    Circulo ponto = {
        .id = "\0",
        .raio = 1,
        .x = ponto_x,
        .y = ponto_y,
    };
    strcpy(ponto.cor_preenchimento, interno ? "blue" : "magenta");
    strcpy(ponto.cor_borda, interno ? "blue" : "magenta");
    return ponto;
}

// Executa o comando i? especificado no arquivo de consulta, verificando se um
// ponto é interno a uma figura, conecta este ponto e a figura utilizando uma
// linha.
void checar_ponto_interno(Lista *lista, const char *linha, FILE *arquivo_log) {
    char id[100];
    double ponto_x = 0, ponto_y = 0;
    sscanf(linha, "i? %s %lf %lf", id, &ponto_x, &ponto_y);

    struct No *no = buscar_elemento_lista(lista, id);
    if(no == NULL)
        return;

    bool interno = checar_ponto_interno_figura(
        no->figura, no->tipo,
        ponto_x, ponto_y
    );
    Figuras ponto, ligacao;
    ponto.circ = criar_ponto(ponto_x, ponto_y, interno);
    inserir_lista(lista, ponto, TIPO_CIRCULO);
    ligacao.lin = ligar_ponto_figura(ponto.circ, no->figura, no->tipo);
    inserir_lista(lista, ligacao, TIPO_LINHA);

    fprintf(arquivo_log, "i? %s %lf %lf\n", id, ponto_x, ponto_y);
    fprintf(arquivo_log,
            "%s: %s %s\n\n",
            id, tipo_para_string_figura(no->tipo),
            interno ? "INTERNO" : "NAO INTERNO"
    );
}

// Executa o comando pnt especificado no arquivo de consulta, alterando a cor de
// preenchimento e borda da figura com id igual ao id que segue o comando pnt.
void alterar_cor(Lista *lista, const char *linha, FILE *arquivo_log) {
    char id[100], nova_corb[20], nova_corp[20];
    sscanf(linha, "pnt %s %s %s", id, nova_corb, nova_corp);

    struct No *no = buscar_elemento_lista(lista, id);
    if(no == NULL)
        return;

    const char *atual_corp = obter_cor_preenchimento_figura(
        &no->figura, no->tipo
    );
    const char *atual_corb = obter_cor_borda_figura(&no->figura, no->tipo);

    fprintf(arquivo_log, "pnt %s %s %s\n", id, nova_corb, nova_corp);
    fprintf(arquivo_log, "corb: %s, corp: %s\n\n", atual_corb, atual_corp);

    alterar_cor_figura(&no->figura, no->tipo, nova_corb, nova_corp);
}

// Executa o comando pnt* especificado no arquivo de consulta, alterando a cor
// de preenchimento e borda de todas as figuras entre id_inicial e id_final.
// (inclusive).
void alterar_cores(Lista *lista, const char *linha, FILE *arquivo_log) {
    char id_inicial[100], id_final[100], nova_corb[20], nova_corp[20];
    sscanf(
        linha,
        "pnt* %s %s %s %s",
        id_inicial, id_final, nova_corb, nova_corp
    );
    struct No *atual = buscar_elemento_lista(lista, id_inicial);
    if(atual == NULL)
        return;

    while(atual != NULL) {
        const char *id_atual = obter_id_figura(&atual->figura, atual->tipo);
        const char *atual_corp = obter_cor_preenchimento_figura(
            &atual->figura, atual->tipo
        );
        const char *atual_corb = obter_cor_borda_figura(
            &atual->figura, atual->tipo
        );

        fprintf(
            arquivo_log,
            "pnt* %s %s %s %s\n",
            id_inicial, id_final, nova_corb, nova_corp
        );
        fprintf(arquivo_log, "corb: %s, corp: %s\n\n", atual_corb, atual_corp);

        alterar_cor_figura(&atual->figura, atual->tipo, nova_corb, nova_corp);
        if(strcmp(id_atual, id_final) == 0)
            break;
        atual = atual->prox;
    }
}

// Executa o comando delf especificado no arquivo de consulta, removendo a
// figura com id igual ao id que segue o comando delf.
void remover_elemento(Lista *lista, const char *linha, FILE *arquivo_log) {
    char id[100];
    sscanf(linha, "delf %s", id);
    struct No *no = buscar_elemento_lista(lista, id);
    if(no == NULL)
        return;

    fprintf(arquivo_log, "delf %s\n", id);
    escrever_informacoes_figura(arquivo_log, no->figura, no->tipo);
    fprintf(arquivo_log, "\n");
    remover_elemento_lista(lista, id);
}

// Executa o comando delf* especificado pelo arquivo de consulta, removendo
// todas as figuras que se encontrem entre id_inicial e id_final (inclusive).
void remover_elementos(Lista *lista, const char *linha, FILE *arquivo_log) {
    char id_inicial[100], id_final[100];
    sscanf(linha, "delf* %s %s", id_inicial, id_final);

    struct No *atual = buscar_elemento_lista(lista, id_inicial);
    while(atual != NULL) {
        const char *id_atual = obter_id_figura(&atual->figura, atual->tipo);

        fprintf(arquivo_log, "delf* %s %s\n", id_inicial, id_final);
        escrever_informacoes_figura(arquivo_log, atual->figura, atual->tipo);
        fprintf(arquivo_log, "\n");

        if(strcmp(id_atual, id_final) == 0) {
            // É necessário marcar a saída do loop antes de remover o elemento,
            // já que após a remoção do elemento seu id será perdido.
            atual = NULL;
        } else {
            atual = atual->prox;
        }
        remover_elemento_lista(lista, id_atual);
    }
}

// Ler o arquivo de consulta localizado no caminho fornecido a função e itera
// por todas as suas linhas, executando funções correspondentes aos comandos.
void ler_qry(Lista *lista, const char *caminho_qry, const char *caminho_log) {
    FILE *arquivo_consulta = fopen(caminho_qry, "r");
    if(arquivo_consulta == NULL) {
        fprintf(stderr, "Falha ao ler arquivo de consulta: %s\n", caminho_qry);
        return;
    }
    FILE *arquivo_log = fopen(caminho_log, "w");
    if(arquivo_log == NULL) {
        fprintf(stderr, "Falha ao criar arquivo de log: %s\n", caminho_log);
        return;
    }

    char linha[LINHA_MAX];
    while(fgets(linha, LINHA_MAX, arquivo_consulta) != NULL) {
        char comando[100];
        sscanf(linha, "%s", comando);
        if(strcmp("o?", comando) == 0) {
            checar_interseccao(lista, linha, arquivo_log);
        } else if(strcmp("i?", comando) == 0) {
            checar_ponto_interno(lista, linha, arquivo_log);
        } else if(strcmp("pnt", comando) == 0) {
            alterar_cor(lista, linha, arquivo_log);
        } else if(strcmp("pnt*", comando) == 0) {
            alterar_cores(lista, linha, arquivo_log);
        } else if(strcmp("delf", comando) == 0) {
            remover_elemento(lista, linha, arquivo_log);
        } else if(strcmp("delf*", comando) == 0) {
            remover_elementos(lista, linha, arquivo_log);
        }
    }

    fclose(arquivo_log);
    fclose(arquivo_consulta);
}
