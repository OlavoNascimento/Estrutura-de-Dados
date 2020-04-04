#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ler_qry.h>
#include <lista.h>
#include <figuras.h>

#define LINHA_MAX 300

Texto aviso_sobreposicao(Retangulo contorno, char *id1, char *id2) {
    char id[100];
    sprintf(id, "aviso_sobreposição_%s_%s", id1, id2);
    Texto aviso = {
        .x = contorno.x + contorno.largura/8,
        .y = contorno.y + contorno.altura/2,
        .cor_borda = "None",
        .cor_preenchimento = "Black",
        .texto = "sobrepoe",
        .tamanho = 6
    };
    strcpy(aviso.id, id);
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
    if(intersectam) {
        Figuras contorno, aviso;
        contorno.ret = envolver_figuras(no1->figura, no1->tipo, no2->figura, no2->tipo);
        aviso.tex = aviso_sobreposicao(contorno.ret, id1, id2);
        inserir_lista(lista, contorno, TipoRetangulo);
        inserir_lista(lista, aviso, TipoTexto);
    }

    fprintf(log, "o? %s %s\n", id1, id2);
    fprintf(log, "%s: %s %s: %s %s\n",
            id1, fig_tipo_para_string(no1->tipo),
            id2, fig_tipo_para_string(no2->tipo),
            intersectam ? "SIM" : "NAO"
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
            printf("Comando i?\n");
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
