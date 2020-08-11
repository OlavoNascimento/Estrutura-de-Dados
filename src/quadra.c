
#include "quadra.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "retangulo.h"

typedef struct {
    Retangulo ret;
} QuadraImp;

Quadra quadra_criar(char id[100], double largura, double altura, double x, double y,
                    char cor_borda[20], char cor_preenchimento[20], int tracejado_tamanho,
                    int tracejado_espaco) {
    QuadraImp *quadImp = malloc(sizeof(QuadraImp));
    quadImp->ret = retangulo_criar(id, largura, altura, x, y, cor_borda, cor_preenchimento,
                                   tracejado_tamanho, tracejado_espaco);
    return quadImp;
}

Quadra quadra_ler(const char *linha) {
    char id[100];
    double largura;
    double altura;
    double x;
    double y;
    char cor_borda[20] = "none";
    char cor_preenchimento[20] = "none";
    sscanf(linha, "%*c %s %lf %lf %lf %lf", id, &largura, &altura, &x, &y);
    return quadra_criar(id, largura, altura, x, y, cor_borda, cor_preenchimento, 0, 0);
}

void quadra_definir_cor_borda(Quadra quad, const char *cor_borda) {
    QuadraImp *quadImp = (QuadraImp *) quad;
    retangulo_definir_cor_borda(quadImp->ret, cor_borda);
}

void quadra_definir_cor_preenchimento(Quadra quad, const char *cor_preenchimento) {
    QuadraImp *quadImp = (QuadraImp *) quad;
    retangulo_definir_cor_preenchimento(quadImp->ret, cor_preenchimento);
}

void quadra_destruir(Quadra quad) {
    free(quad);
}