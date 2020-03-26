#include <stdlib.h>
#include <string.h>

#include <lista.h>
#include <retangulo.h>

Lista* criar_lista() {
    Lista *lis = (Lista*) malloc(sizeof(Lista));
    lis->cabeca = NULL;
    lis->cauda= NULL;
    return lis;
}

void inserir_lista(Lista *lista, Figuras fig, int fig_tipo) {
    struct No *no = (struct No*) malloc(sizeof(struct No));
    no->figura = fig;
    no->tipo = fig_tipo;
    no->prox = NULL;

    struct No *atual = lista->cauda;
    if(atual == NULL) {
        // Primeiro elemento da lista
       lista->cabeca = no;
       lista->cauda = no;
    } else {
        // Elementos após o primeiro
        atual->prox = no;
        lista->cauda = no;
    }
}

void remover_elemento_lista(Lista *lista, char *id_buscado) {
    struct No *atual = lista->cabeca;
    struct No *anterior;
    while(atual != NULL) {
        char *id_atual;
        switch (atual->tipo) {
            case tipo_circulo:
                id_atual = atual->figura.circ.id;
                break;
            case tipo_retangulo:
                id_atual = atual->figura.ret.id;
                break;
            case tipo_texto:
                id_atual = atual->figura.tex.id;
                break;
        }

        if(strcmp(id_atual, id_buscado) == 0) {
            anterior->prox = atual->prox;
            free(atual);
            atual = NULL;
        } else {
            anterior = atual;
            atual = atual->prox;
        }
    }
}

void destruir_lista(Lista *lista) {
    struct No *atual = lista->cabeca;
    struct No *prox;
    while(atual != NULL) {
        prox = atual->prox;
        free(atual);
        atual = prox;
    }

    lista->cabeca = NULL;
    lista->cauda = NULL;
    free(lista);
}