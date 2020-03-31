#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ler_qry.h>
#include <lista.h>
#include <figuras.h>
#include <parametros.h>

#define LINHA_MAX 300

FILE* criar_arquivo_log(char *dir_saida, char *caminho_geo) {
    char *nome_arquivo_log = adicionar_sufixo(caminho_geo, "-arqcons.txt");
    char *caminho_arquivo_log = preparar_caminho(dir_saida, nome_arquivo_log);
    printf("Arquivo de log: %s\n", caminho_arquivo_log);
    FILE *arquivo_log = fopen(nome_arquivo_log, "w");
    if(arquivo_log == NULL) {
        fprintf(stderr, "Falha ao criar arquivo de log!\n");
        return NULL;
    }
    free(nome_arquivo_log);
    free(caminho_arquivo_log);
    return arquivo_log;
}

bool checar_interseccao(Lista *lista, char *linha, FILE *log) {
    char id1[100], id2[100];
    sscanf(linha, "o? %s %s", id1, id2);
    struct No *no1 = buscar_elemento_lista(lista, id1);
    struct No *no2 = buscar_elemento_lista(lista, id2);
    if(no1 == NULL || no2 == NULL)
        return NULL;

    bool intersectam = false;
    if(no1->tipo == tipo_retangulo && no1->tipo == no2->tipo) {
        intersectam = retangulos_interseccao(no1->figura.ret, no2->figura.ret);
    } else if(no1->tipo == tipo_circulo && no1->tipo == no2->tipo) {
        intersectam = circulos_interseccao(no1->figura.circ, no2->figura.circ);
    } else if(no1->tipo == tipo_circulo && no2->tipo == tipo_retangulo) {
        intersectam = circulo_retangulo_interseccao(no1->figura.circ, no2->figura.ret);
    } else if(no1->tipo == tipo_retangulo && no2->tipo == tipo_circulo) {
        intersectam = circulo_retangulo_interseccao(no2->figura.circ, no1->figura.ret);
    }
    return intersectam;
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
            bool intersect = checar_interseccao(lista, linha, log);
            if(intersect) {
                printf("Elementos se intersectam!\n");
            } else {
                printf("Elementos não se intersectam!\n");
            }
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
