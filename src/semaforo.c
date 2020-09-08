#include "semaforo.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logging.h"
#include "retangulo.h"

typedef struct {
    Retangulo ret;
} SemaforoImp;

// Cria e inicializa um struct SemaforoImp com os valores passados.
Semaforo semaforo_criar(const char id[100], double largura, double altura, double x, double y,
                        const char cor_borda[20], const char cor_preenchimento[20]) {
    if (id == NULL) {
        LOG_ERROR("Não é possível criar um semaforo com id NULL!\n");
        return NULL;
    }
    if (cor_borda == NULL) {
        LOG_ERROR("Não é possível criar um semaforo com cor de borda NULL!\n");
        return NULL;
    }
    if (cor_preenchimento == NULL) {
        LOG_ERROR("Não é possível criar um semaforo com cor de preenchimento NULL!\n");
        return NULL;
    }
    SemaforoImp *semaImp = malloc(sizeof(SemaforoImp));
    semaImp->ret = retangulo_criar(id, largura, altura, x, y, cor_borda, cor_preenchimento);
    return semaImp;
}

// Cria um semaforo com base em informações de uma linha.
Semaforo semaforo_ler(const char *linha) {
    char id[100];
    double x;
    double y;
    char cor_borda[20] = "red";
    char cor_preenchimento[20] = "green";
    sscanf(linha, "%*s %s %lf %lf", id, &x, &y);
    return semaforo_criar(id, 20, 40, x, y, cor_borda, cor_preenchimento);
}

// Escreve as informações de uma radio base
void semaforo_escrever_informacoes(FILE *arquivo, Semaforo semaforo) {
    SemaforoImp *semaImp = (SemaforoImp *) semaforo;
    retangulo_escrever_informacoes(arquivo, semaImp->ret);
}

// Escreve no svg as informações de uma radio base
void semaforo_escrever_svg(FILE *arquivo, Semaforo semaforo) {
    SemaforoImp *semaImp = (SemaforoImp *) semaforo;
    retangulo_escrever_svg(arquivo, semaImp->ret);
}

// Retorna o id de um semaforo.
const char *semaforo_obter_id(Semaforo sem) {
    SemaforoImp *semImp = (SemaforoImp *) sem;
    return retangulo_obter_id(semImp->ret);
}

double semaforo_obter_x(Semaforo sem) {
    SemaforoImp *semImp = (SemaforoImp *) sem;
    return retangulo_obter_x(semImp->ret);
}

double semaforo_obter_y(Semaforo sem) {
    SemaforoImp *semImp = (SemaforoImp *) sem;
    return retangulo_obter_y(semImp->ret);
}

double semaforo_obter_largura(Semaforo sem) {
    SemaforoImp *semImp = (SemaforoImp *) sem;
    return retangulo_obter_largura(semImp->ret);
}

double semaforo_obter_altura(Semaforo sem) {
    SemaforoImp *semImp = (SemaforoImp *) sem;
    return retangulo_obter_altura(semImp->ret);
}

const char *semaforo_obter_cor_borda(Semaforo sem) {
    SemaforoImp *semImp = (SemaforoImp *) sem;
    return retangulo_obter_cor_borda(semImp->ret);
}

// Define a cor da borda de um semaforo.
void semaforo_definir_cor_borda(Semaforo semaforo, const char *cor_borda) {
    if (cor_borda == NULL) {
        LOG_ERROR("Não é possível definir NULL como cor da borda de um semaforo!\n");
        return;
    }
    SemaforoImp *semaImp = (SemaforoImp *) semaforo;
    retangulo_definir_cor_borda(semaImp->ret, cor_borda);
}

const char *semaforo_obter_cor_preenchimento(Semaforo sem) {
    SemaforoImp *semImp = (SemaforoImp *) sem;
    return retangulo_obter_cor_preenchimento(semImp->ret);
}

// Define a cor de preenchimento de um semaforo.
void semaforo_definir_cor_preenchimento(Semaforo semaforo, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        LOG_ERROR("Não é possível definir NULL como cor de preenchimento de um semaforo!\n");
        return;
    }
    SemaforoImp *semaImp = (SemaforoImp *) semaforo;
    retangulo_definir_cor_preenchimento(semaImp->ret, cor_preenchimento);
}

const char *semaforo_obter_espessura_borda(Semaforo sem) {
    SemaforoImp *semImp = (SemaforoImp *) sem;
    return retangulo_obter_espessura_borda(semImp->ret);
}

void semaforo_definir_espessura_borda(Semaforo sem, const char *espessura_borda) {
    if (espessura_borda == NULL) {
        LOG_ERROR(
            "Não é possível definir NULL como tamanho da espessura da borda de um "
            "semaforo!\n");
        return;
    }
    SemaforoImp *semImp = (SemaforoImp *) sem;
    retangulo_definir_espessura_borda(semImp->ret, espessura_borda);
}

// Libera a memória alocada por um semáforo.
void semaforo_destruir(Semaforo semaforo) {
    SemaforoImp *semaImp = (SemaforoImp *) semaforo;
    retangulo_destruir(semaImp->ret);
    free(semaforo);
}
