#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ler_qry.h>
#include <lista.h>
#include <figuras.h>

#define LINHA_MAX 300

Texto aviso_sobreposicao(Retangulo contorno, char *id1, char *id2) {
    Texto aviso = {
        .id = "\0",
        .x = contorno.x + contorno.largura/8,
        .y = contorno.y + contorno.altura/2,
        .cor_borda = "None",
        .cor_preenchimento = "Black",
        .texto = "sobrepoe",
        .tamanho = 6
    };
    return aviso;
}

void checar_interseccao(Lista *lista, char *linha, FILE *log) {
    char id1[100], id2[100];
    sscanf(linha, "o? %s %s", id1, id2);
    struct No *no1 = buscar_elemento_lista(lista, id1);
    struct No *no2 = buscar_elemento_lista(lista, id2);
    if(no1 == NULL || no2 == NULL)
        return;

    bool intersectam = interseccao_figuras(no1->figura, no1->tipo, no2->figura, no2->tipo);
    Figuras contorno;
    contorno.ret = envolver_figuras(intersectam, no1->figura, no1->tipo, no2->figura, no2->tipo);
    inserir_lista(lista, contorno, TipoRetangulo);
    if(intersectam) {
        Figuras aviso;
        aviso.tex = aviso_sobreposicao(contorno.ret, id1, id2);
        inserir_lista(lista, aviso, TipoTexto);
    }

    fprintf(log, "o? %s %s\n", id1, id2);
    fprintf(log, "%s: %s %s: %s %s\n\n",
            id1, fig_tipo_para_string(no1->tipo),
            id2, fig_tipo_para_string(no2->tipo),
            intersectam ? "SIM" : "NAO"
    );
}

Circulo criar_ponto(bool interno, double ponto_x, double ponto_y) {
    Circulo ponto = {
        .id = "\0",
        .raio = 1,
        .x = ponto_x,
        .y = ponto_y,
    };
    if(interno) {
        strcpy(ponto.cor_borda, "blue");
        strcpy(ponto.cor_preenchimento, "blue");
    } else {
        strcpy(ponto.cor_borda, "magenta");
        strcpy(ponto.cor_preenchimento, "magenta");
    }
    return ponto;
}

void checar_ponto_interno(Lista *lista, char *linha, FILE *log) {
    char id[100];
    double ponto_x = 0, ponto_y = 0;
    sscanf(linha, "i? %s %lf %lf", id, &ponto_x, &ponto_y);
    struct No *no = buscar_elemento_lista(lista, id);
    if(no == NULL)
        return;

    bool interno = ponto_interno_figura(no->figura, no->tipo, ponto_x, ponto_y);
    Figuras ponto, ligacao;
    ponto.circ = criar_ponto(interno, ponto_x, ponto_y);
    inserir_lista(lista, ponto, TipoCirculo);
    ligacao.lin = ligar_ponto_figura(ponto.circ, no->figura, no->tipo);
    inserir_lista(lista, ligacao, TipoLinha);

    fprintf(log, "i? %s %lf %lf\n", id, ponto_x, ponto_y);
    fprintf(log, "%s: %s %s\n\n",
            id, fig_tipo_para_string(no->tipo),
            interno ? "INTERNO" : "NAO INTERNO"
    );
}

void alterar_cor(Lista *lista, char *linha, FILE *log) {
    char id[100], corb[20], corp[20];
    sscanf(linha, "pnt %s %s %s", id, corb, corp);
    struct No *no = buscar_elemento_lista(lista, id);
    if(no == NULL)
        return;
    char *cor_preenchimento = obter_cor_preenchimento_figura(&no->figura, no->tipo);
    char *cor_borda = obter_cor_borda_figura(&no->figura, no->tipo);
    fprintf(log, "pnt %s %s %s\n", id, corb, corp);
    fprintf(log, "corb: %s, corp: %s\n\n", cor_borda, cor_preenchimento);

    alterar_cor_figura(&no->figura, no->tipo, corb, corp);
}

void alterar_cores(Lista *lista, char *linha, FILE *log) {
    char id_inicial[100], id_final[100], corb[20], corp[20];
    sscanf(linha, "pnt* %s %s %s %s", id_inicial, id_final, corb, corp);
    struct No *atual = buscar_elemento_lista(lista, id_inicial);
    if(atual == NULL)
        return;
    while(atual != NULL) {
        char *id_atual = obter_id_figura(&atual->figura, atual->tipo);
        char *atual_corp = obter_cor_preenchimento_figura(&atual->figura, atual->tipo);
        char *atual_corb = obter_cor_borda_figura(&atual->figura, atual->tipo);
        fprintf(log, "pnt* %s %s %s %s\n", id_inicial, id_final, corb, corp);
        fprintf(log, "corb: %s, corp: %s\n\n", atual_corb, atual_corp);

        alterar_cor_figura(&atual->figura, atual->tipo, corb, corp);
        if(strcmp(id_atual, id_final) == 0)
            break;
        atual = atual->prox;
    }
}

void remover_elemento(Lista *lista, char *linha, FILE *log) {
    char id[100];
    sscanf(linha, "delf %s", id);
    struct No *no = buscar_elemento_lista(lista, id);
    if(no == NULL)
        return;

    fprintf(log, "delf %s\n", id);
    escrever_informacoes_figura(log, no->figura, no->tipo);
    fprintf(log, "\n");
    remover_elemento_lista(lista, id);
}

void remover_elementos(Lista *lista, char *linha, FILE *log) {
    char id_inicial[100], id_final[100];
    sscanf(linha, "delf* %s %s", id_inicial, id_final);
    struct No *atual = buscar_elemento_lista(lista, id_inicial);
    while(atual != NULL) {
        char *id_atual = obter_id_figura(&atual->figura, atual->tipo);
        fprintf(log, "delf* %s %s\n", id_inicial, id_final);
        escrever_informacoes_figura(log, atual->figura, atual->tipo);
        fprintf(log, "\n");

        if(strcmp(id_atual, id_final) == 0)
            atual = NULL;
        else
            atual = atual->prox;
        remover_elemento_lista(lista, id_atual);
    }
}

void ler_qry(Lista *lista, char *caminho_qry, FILE* log) {
    FILE *arquivo_consulta = fopen(caminho_qry, "r");
    if(arquivo_consulta == NULL) {
        fprintf(stderr, "Falha ao ler arquivo de consulta %s!\n", caminho_qry);
        return;
    }
    char linha[LINHA_MAX];
    while(fgets(linha, LINHA_MAX, arquivo_consulta) != NULL) {
        char comando[100];
        sscanf(linha, "%s", comando);
        if(strcmp("o?", comando) == 0) {
            checar_interseccao(lista, linha, log);
        } else if(strcmp("i?", comando) == 0) {
            checar_ponto_interno(lista, linha, log);
        } else if(strcmp("pnt", comando) == 0) {
            alterar_cor(lista, linha, log);
        } else if(strcmp("pnt*", comando) == 0) {
            alterar_cores(lista, linha, log);
        } else if(strcmp("delf", comando) == 0) {
            remover_elemento(lista, linha, log);
        } else if(strcmp("delf*", comando) == 0) {
            remover_elementos(lista, linha, log);
        }
    }

    fclose(arquivo_consulta);
}
