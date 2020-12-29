#include "semaforo.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logging.h"
#include "retangulo.h"
#include "texto.h"

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
    SemaforoImp *semImp = malloc(sizeof(SemaforoImp));
    semImp->ret = retangulo_criar(id, largura, altura, x, y, cor_borda, cor_preenchimento);
    return semImp;
}

// Cria um semaforo com base em informações de uma linha.
Semaforo semaforo_ler(const char *linha) {
    char id[100];
    double x;
    double y;
    double largura = 12;
    double altura = 30;
    char cor_borda[20] = "red";
    char cor_preenchimento[20] = "green";
    sscanf(linha, "%*s %s %lf %lf", id, &x, &y);
    return semaforo_criar(id, largura, altura, x - largura / 2, y - altura / 2, cor_borda,
                          cor_preenchimento);
}

// Escreve as informações de uma radio base
void semaforo_escrever_informacoes(FILE *arquivo, Semaforo semaforo) {
    SemaforoImp *semImp = (SemaforoImp *) semaforo;
    retangulo_escrever_informacoes(arquivo, semImp->ret);
}

// Escreve no svg as informações de uma radio base
void semaforo_escrever_svg(FILE *arquivo, Semaforo semaforo) {
    SemaforoImp *semImp = (SemaforoImp *) semaforo;
    retangulo_escrever_svg(arquivo, semImp->ret);
    double x = retangulo_obter_x(semImp->ret) + retangulo_obter_largura(semImp->ret) / 2;
    double y = retangulo_obter_y(semImp->ret) + retangulo_obter_altura(semImp->ret) / 2 + 4;
    Texto texto_semaforo = texto_criar("", x, y, "none", "white", "S", true);
    texto_escrever_svg(arquivo, texto_semaforo);
    texto_destruir(texto_semaforo);
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
    SemaforoImp *semImp = (SemaforoImp *) semaforo;
    retangulo_definir_cor_borda(semImp->ret, cor_borda);
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
    SemaforoImp *semImp = (SemaforoImp *) semaforo;
    retangulo_definir_cor_preenchimento(semImp->ret, cor_preenchimento);
}

const char *semaforo_obter_espessura_borda(Semaforo sem) {
    SemaforoImp *semImp = (SemaforoImp *) sem;
    return retangulo_obter_espessura_borda(semImp->ret);
}

void semaforo_definir_espessura_borda(Semaforo sem, const char *espessura_borda) {
    if (espessura_borda == NULL) {
        LOG_ERROR(
            "Não é possível definir NULL como tamanho da espessura da borda de um semaforo!\n");
        return;
    }
    SemaforoImp *semImp = (SemaforoImp *) sem;
    retangulo_definir_espessura_borda(semImp->ret, espessura_borda);
}

// Libera a memória alocada por um semáforo.
void semaforo_destruir(Semaforo semaforo) {
    SemaforoImp *semImp = (SemaforoImp *) semaforo;
    retangulo_destruir(semImp->ret);
    free(semaforo);
}
