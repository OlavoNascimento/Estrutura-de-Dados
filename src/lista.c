#include <stdlib.h>
#include <string.h>

#include <lista.h>

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

    // TODO Remover elementos duplicados
    // TODO Adicionar função comparar_nos
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

struct No* buscar_elemento_lista(Lista *lista, char *id_buscado) {
    struct No *atual = lista->cabeca;
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
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

void lista_para_svg(Lista *lista, char *caminho_svg) {
    FILE *arquivo = fopen(caminho_svg, "w");
    if(arquivo == NULL) {
        fprintf(stderr, "Arquivo %s não pode ser criado!\n", caminho_svg);
        return;
    }

    fprintf(arquivo, "<svg>\n");
    struct No* atual = lista->cabeca;
    while(atual != NULL) {
        switch (atual->tipo) {
            case tipo_circulo:
                circulo_para_svg(arquivo, atual->figura.circ);
                break;
            case tipo_retangulo:
                retangulo_para_svg(arquivo, atual->figura.ret);
                break;
            case tipo_texto:
                texto_para_svg(arquivo, atual->figura.tex);
                break;
        }
        atual = atual->prox;
    }
    fprintf(arquivo, "</svg>\n");
    fclose(arquivo);
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