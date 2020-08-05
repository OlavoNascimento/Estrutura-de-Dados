#pragma once

#include "figuras.h"

// Utilizado para representar as proporções de um arquivo svg.
typedef struct {
    double origem_x;
    double origem_y;
    double largura;
    double altura;
} ExibicaoSVG;

struct No {
    TiposFigura tipo;
    Figuras figura;
    struct No *prox;
};

typedef struct {
    struct No *cabeca;
    struct No *cauda;
    ExibicaoSVG exibicao;
} Lista;

Lista *criar_lista();
void inserir_lista(Lista *lista, Figuras fig, TiposFigura tipo_fig);
struct No *buscar_elemento_lista(Lista *lista, const char *id_buscado);
void remover_elemento_lista(Lista *lista, const char *id_buscado);
void lista_para_svg(Lista *lista, const char *caminho_svg);
void destruir_lista(Lista *lista);
