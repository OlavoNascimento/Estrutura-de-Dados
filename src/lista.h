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
struct No* buscar_elemento_id_lista(Lista *lista, char *id_buscado);
struct No* buscar_elemento_posicao_lista(Lista *lista, int posicao_buscada);
void remover_elemento_lista(Lista *lista, char *id);
void lista_para_svg(Lista *lista, char *caminho_svg);
void destruir_lista();
