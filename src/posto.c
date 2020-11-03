#include "posto.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "circulo.h"
#include "logging.h"

typedef struct {
    Circulo circ;
} PostoImp;

// Cria e inicializa um struct PostoImp com os valores passados.
Posto posto_criar(const char id[100], double raio, double x, double y, const char cor_borda[20],
                  const char cor_preenchimento[20]) {
    if (id == NULL) {
        LOG_ERROR("Não é possível criar um posto com id NULL!\n");
        return NULL;
    }
    if (cor_borda == NULL) {
        LOG_ERROR("Não é possível criar um posto com cor de borda NULL!\n");
        return NULL;
    }
    if (cor_preenchimento == NULL) {
        LOG_ERROR("Não é possível criar um posto com cor de preenchimento NULL!\n");
        return NULL;
    }
    PostoImp *posImp = malloc(sizeof(PostoImp));
    posImp->circ = circulo_criar(id, raio, x, y, cor_borda, cor_preenchimento);
    return posImp;
}

// Cria um posto com base em informações de uma linha.
Posto posto_ler(const char *linha) {
    double raio = 10;  // valor simbólico, padrão para todos os postos
    double x;
    double y;
    char cor_borda[20] = "blue";          // será alterado com outro comando
    char cor_preenchimento[20] = "blue";  // será alterado com outro comando
    sscanf(linha, "%*s %lf %lf", &x, &y);
    return posto_criar("", raio, x, y, cor_borda, cor_preenchimento);
}

// Retorna o id de um posto.
const char *posto_obter_id(Posto posto) {
    PostoImp *posImp = (PostoImp *) posto;
    return circulo_obter_id(posImp->circ);
}

// Escreve as informações de um posto.
void posto_escrever_informacoes(FILE *arquivo, Posto posto) {
    PostoImp *posImp = (PostoImp *) posto;
    circulo_escrever_informacoes(arquivo, posImp->circ);
}

// Escreve no svg as informações de um posto.
void posto_escrever_svg(FILE *arquivo, Posto posto) {
    PostoImp *posImp = (PostoImp *) posto;
    circulo_escrever_svg(arquivo, posImp->circ);
}

// Retorna o raio de um posto. Usado apenas para cálculo interno, já que o posto é considerado
// um ponto.
double posto_obter_raio(Posto posto) {
    PostoImp *posImp = (PostoImp *) posto;
    return circulo_obter_raio(posImp->circ);
}

// Retorna a coordenada y de um posto. O posto é considerado apenas um ponto.
double posto_obter_y(Posto posto) {
    PostoImp *posImp = (PostoImp *) posto;
    return circulo_obter_y(posImp->circ);
}

// Retorna a coordenada x de um posto. O posto é considerado apenas um ponto.
double posto_obter_x(Posto posto) {
    PostoImp *posImp = (PostoImp *) posto;
    return circulo_obter_x(posImp->circ);
}

// Retorna a cor de preenchimento de um posto.
const char *posto_obter_cor_preenchimento(Posto posto) {
    PostoImp *posImp = (PostoImp *) posto;
    return circulo_obter_cor_preenchimento(posImp->circ);
}

// Define a cor de preenchimento de um posto.
void posto_definir_cor_preenchimento(Posto posto, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        LOG_ERROR("Não é possível definir NULL como cor de preenchimento de um posto!\n");
        return;
    }
    PostoImp *posImp = (PostoImp *) posto;
    circulo_definir_cor_preenchimento(posImp->circ, cor_preenchimento);
}

// Retorna a cor da borda de um posto.
const char *posto_obter_cor_borda(Posto posto) {
    PostoImp *posImp = (PostoImp *) posto;
    return circulo_obter_cor_borda(posImp->circ);
}

// Define a cor da borda de um posto.
void posto_definir_cor_borda(Posto posto, const char *cor_borda) {
    if (cor_borda == NULL) {
        LOG_ERROR("Não é possível definir NULL como cor da borda de um posto!\n");
        return;
    }
    PostoImp *posImp = (PostoImp *) posto;
    circulo_definir_cor_borda(posImp->circ, cor_borda);
}

// Define a espessura da borda de um posto.
void posto_definir_espessura_borda(Posto posto, const char *espessura_borda) {
    if (espessura_borda == NULL) {
        LOG_ERROR("Não é possível definir NULL como tamanho da espessura da borda de um posto!\n");
        return;
    }
    PostoImp *posImp = (PostoImp *) posto;
    circulo_definir_espessura_borda(posImp->circ, espessura_borda);
}

// Libera a memória alocada por um posto.
void posto_destruir(Posto posto) {
    PostoImp *posImp = (PostoImp *) posto;
    circulo_destruir(posImp->circ);
    free(posto);
}
