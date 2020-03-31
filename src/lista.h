#pragma once

#include <figuras.h>

struct No {
    TiposFigura tipo;
    Figuras figura;
    struct No* prox;
};

typedef struct {
    struct No* cabeca;
    struct No* cauda;
} Lista;

Lista* criar_lista();
void inserir_lista(Lista *lista, Figuras fig, TiposFigura tipo_fig);
void lista_para_svg(Lista *lista, char *caminho_svg);
struct No* buscar_elemento_lista(Lista *lista, char *id_buscado);
void destruir_lista();
