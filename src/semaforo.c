
#include "semaforo.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "retangulo.h"

typedef struct {
    Retangulo sema;
} SemaforoImp;

// Cria e inicializa um struct SemaforoImp com os valores passados.
Semaforo semaforo_criar(char id[100], double largura, double altura, double x, double y,
                        char cor_borda[20], char cor_preenchimento[20], int tracejado_tamanho,
                        int tracejado_espaco) {
    SemaforoImp *semaImp = malloc(sizeof(SemaforoImp));
    semaImp->sema = retangulo_criar(id, largura, altura, x, y, cor_borda, cor_preenchimento,
                                    tracejado_tamanho, tracejado_espaco);
    return semaImp;
}

// Cria um semaforo com base em informações de uma linha.
Semaforo semaforo_ler(const char *linha) {
    char id[100];
    double x;
    double y;
    char cor_borda[20] = "red";
    char cor_preenchimento[20] = "green";
    sscanf(linha, "%*c %s %lf %lf", id, &x, &y);
    return semaforo_criar(id, 20, 40, x, y, cor_borda, cor_preenchimento, 10, 0);
}

// Define a cor da borda de um semaforo.
void semaforo_definir_cor_borda(Semaforo sema, const char *cor_borda) {
    SemaforoImp *semaImp = (SemaforoImp *) sema;
    retangulo_definir_cor_borda(semaImp->sema, cor_borda);
}

// Define a cor de preenchimento de um semaforo.
void semaforo_definir_cor_preenchimento(Semaforo sema, const char *cor_preenchimento) {
    SemaforoImp *semaImp = (SemaforoImp *) sema;
    retangulo_definir_cor_preenchimento(semaImp->sema, cor_preenchimento);
}

// Libera a memória alocada por um semáforo.
void semaforo_destruir(Semaforo sema) {
    SemaforoImp *semaImp = (SemaforoImp *) sema;
    retangulo_destruir(semaImp->sema);
    free(sema);
}

// escreve as informações de uma radio base
void semaforo_escrever_informacoes(FILE *arquivo, Semaforo sema) {
    SemaforoImp *semaImp = (SemaforoImp *) sema;
    retangulo_escrever_informacoes(arquivo, semaImp->sema);
}

// escreve no svg as informações de uma radio base
void semaforo_escrever_svg(FILE *arquivo, Semaforo sema) {
    SemaforoImp *semaImp = (SemaforoImp *) sema;
    retangulo_escrever_svg(arquivo, semaImp->sema);
}