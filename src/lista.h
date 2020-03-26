#pragma once

#include <retangulo.h>
#include <circulo.h>
#include <texto.h>

enum tipos {
    tipo_retangulo,
    tipo_circulo,
    tipo_texto
};

typedef union {
    Retangulo ret;
    Circulo circ;
    Texto tex;
} Figuras;

struct No {
    int tipo;
    Figuras figura;
    struct No* prox;
};

typedef struct {
    struct No* cabeca;
    struct No* cauda;
} Lista;

Lista* criar_lista();
void inserir_lista(Lista *lista, Figuras fig, int fig_tipo);
void lista_para_svg(Lista *lista, FILE *arquivo);
void remover_elemento_lista(Lista *lista, char *id);
void destruir_lista();
