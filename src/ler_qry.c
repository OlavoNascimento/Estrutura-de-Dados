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
    struct No *no1 = buscar_elemento_id_lista(lista, id1);
    struct No *no2 = buscar_elemento_id_lista(lista, id2);
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
    fprintf(log, "%s: %s %s: %s %s\n",
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
    int fig_pos = 0;
    double ponto_x = 0, ponto_y = 0;
    sscanf(linha, "i? %d %lf %lf", &fig_pos, &ponto_x, &ponto_y);
    struct No *no = buscar_elemento_posicao_lista(lista, fig_pos);
    if(no == NULL)
        return;

    bool interno = ponto_interno_figura(no->figura, no->tipo, ponto_x, ponto_y);
    Figuras ponto;
    ponto.circ = criar_ponto(interno, ponto_x, ponto_y);
    inserir_lista(lista, ponto, TipoCirculo);
    Figuras ligacao;
    ligacao.lin = ligar_ponto_figura(ponto.circ, no->figura, no->tipo);
    inserir_lista(lista, ligacao, TipoLinha);

    fprintf(log, "i? %d %lf %lf\n", fig_pos, ponto_x, ponto_y);
    fprintf(log, "%d: %s %s\n",
            fig_pos, fig_tipo_para_string(no->tipo),
            interno ? "INTERNO" : "NAO INTERNO"
    );
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
            printf("Comando pnt\n");
        } else if(strcmp("pnt*", comando) == 0) {
            printf("Comando pnt*\n");
        } else if(strcmp("delf", comando) == 0) {
            printf("Comando delf\n");
        } else if(strcmp("delf*", comando) == 0) {
            printf("Comando delf*\n");
        }
    }

    fclose(arquivo_consulta);
}
