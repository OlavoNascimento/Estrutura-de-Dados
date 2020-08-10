
#include "semaforo.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "retangulo.h"

typedef struct {
    Retangulo sema;
} SemaforoImp;

Semaforo semaforo_criar(char id[100], double largura, double altura, double x, double y,
                        char cor_borda[20], char cor_preenchimento[20], int tracejado_tamanho,
                        int tracejado_espaco) {
    SemaforoImp *semaImp = malloc(sizeof(SemaforoImp));
    semaImp->sema = retangulo_criar(id, largura, altura, x, y, cor_borda, cor_preenchimento, tracejado_tamanho, tracejado_espaco);
    return semaImp;
}

Semaforo semaforo_ler(const char *linha) {
    char id[100];
    double x;
    double y;
    char cor_borda[20] = "red";
    char cor_preenchimento[20] = "green";
    sscanf(linha, "%*c %s %lf %lf", id, &x, &y);
    return semaforo_criar(id, 20, 40, x, y, cor_borda, cor_preenchimento, 10, 0);
}

void semaforo_definir_cor_borda(Semaforo sema, const char *cor_borda) {
    SemaforoImp *semaImp = (SemaforoImp *)sema;
    retangulo_definir_cor_borda(semaImp->sema, cor_borda);
}

void quadra_definir_cor_preenchimento(Semaforo sema, const char *cor_preenchimento) {
    SemaforoImp *semaImp = (SemaforoImp *)sema;
    retangulo_definir_cor_preenchimento(semaImp->sema, cor_preenchimento);
}