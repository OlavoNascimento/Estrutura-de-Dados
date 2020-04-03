#pragma once

#include <figuras.h>

struct No {
    TiposFigura tipo;
    Figuras figura;
    struct No* prox;
};

typedef struct {
    double origem_x;
    double origem_y;
    double largura;
    double altura;
} Exibicao;

typedef struct {
    struct No* cabeca;
    struct No* cauda;
    Exibicao exibicao;
} Lista;

Lista* criar_lista();
void inserir_lista(Lista *lista, Figuras fig, TiposFigura tipo_fig);
void lista_para_svg(Lista *lista, char *caminho_svg);
struct No* buscar_elemento_lista(Lista *lista, char *id_buscado);
void destruir_lista();
