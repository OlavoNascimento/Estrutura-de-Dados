#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "figuras.h"
#include "logging.h"

typedef struct N {
    Figura figura;
    struct N *proximo;
    struct N *anterior;
} no;

typedef struct lista {
    int tamanho;
    no *primeiro;
    no *ultimo;
} list;

Lista lista_create() {
    list *lista = (list *) malloc(sizeof(list));
    if (lista == NULL) {
        LOG_ERROR("Erro ao alocar espaço para a lista!\n");
        return NULL;
    }

    lista->tamanho = 0;
    lista->primeiro = NULL;
    lista->ultimo = NULL;
    return lista;
}

int lista_get_length(Lista lista) {
    list *listaAux = (list *) lista;
    return listaAux->tamanho;
}

No lista_insert_final(Lista lista, Figura figura) {
    list *listaAux = (list *) lista;
    no *nodeAux;
    no *nodeInsert = (no *) malloc(sizeof(no));
    nodeInsert->figura = figura;

    if (listaAux->primeiro == NULL) {
        listaAux->primeiro = nodeInsert;
        nodeInsert->anterior = NULL;
    } else {
        nodeAux = listaAux->ultimo;
        nodeAux->proximo = nodeInsert;
        nodeInsert->anterior = nodeAux;
    }

    nodeInsert->proximo = NULL;
    listaAux->ultimo = nodeInsert;
    listaAux->tamanho++;
    return nodeInsert;
}

No lista_insert_after(Lista lista, Figura figura, No p) {
    list *lista_aux = (list *) lista;
    no *node_aux;
    no *node_proximo = NULL;
    no *node_insert = (no *) malloc(sizeof(no));
    int tam_aux = lista_aux->tamanho;

    node_insert->figura = figura;
    node_aux = p;

    if (node_aux == lista_aux->ultimo) {  // caso seja inserido depois do último
        node_aux->proximo = node_insert;
        lista_aux->ultimo = node_insert;

        node_insert->anterior = node_aux;
        node_insert->proximo = NULL;
        lista_aux->tamanho++;
    } else {
        node_proximo = node_aux->proximo;
        node_aux->proximo = node_insert;
        node_proximo->anterior = node_insert;

        node_insert->proximo = node_proximo;
        node_insert->anterior = node_aux;
        lista_aux->tamanho++;
    }
    if (tam_aux == lista_aux->tamanho) {
        LOG_ERROR("Erro ao inserir elemento (insert_after)\n");
        return NULL;
    }

    return node_insert;
}

No lista_insert_before(Lista lista, Figura figura, No p) {
    list *lista_aux = (list *) lista;
    no *node_aux;
    no *node_anterior = NULL;
    no *node_insert = (no *) malloc(sizeof(no));
    int tam_aux = lista_aux->tamanho;

    node_insert->figura = figura;
    node_aux = p;

    if (node_aux == lista_aux->primeiro) {  // caso seja inserido antes do primeiro
        node_aux->anterior = node_insert;
        node_insert->proximo = node_aux;
        node_insert->anterior = NULL;

        lista_aux->primeiro = node_insert;
        lista_aux->tamanho++;
    } else {
        node_anterior = node_aux->anterior;
        node_aux->anterior = node_insert;
        node_anterior->proximo = node_insert;

        node_insert->proximo = node_aux;
        node_insert->anterior = node_anterior;
        lista_aux->tamanho++;
    }
    if (tam_aux == lista_aux->tamanho) {
        LOG_ERROR("Erro ao inserir elemento (insert_before)!\n");
        return NULL;
    }

    return node_insert;
}

void lista_remove_no(Lista lista, No no_selecionado) {
    list *lista_auxiliar = (list *) lista;
    no *no_auxiliar = (no *) no_selecionado;
    no *no_anterior = NULL;
    no *no_proximo = NULL;

    if (no_auxiliar == NULL) {
        LOG_ERROR("Nó possui ID inválido\n");
        return;
    }
    if (no_auxiliar == lista_auxiliar->primeiro) {  // se for o primeiro elemento da lista
        no_proximo = no_auxiliar->proximo;
        lista_auxiliar->primeiro = no_proximo;
        no_proximo->anterior = NULL;
    } else if (no_auxiliar == lista_auxiliar->ultimo) {  // se for o último elemento da lista
        no_anterior = no_auxiliar->anterior;
        lista_auxiliar->ultimo = no_anterior;
        no_anterior->proximo = NULL;
    } else {  // se estiver no meio da lista
        no_anterior = no_auxiliar->anterior;
        no_proximo = no_auxiliar->proximo;

        no_anterior->proximo = no_proximo;
        no_proximo->anterior = no_anterior;
    }

    figura_destruir(no_auxiliar->figura);
    free(no_auxiliar);
    lista_auxiliar->tamanho--;
}

No lista_get_no(Lista lista, const char id[100]) {
    list *lista_auxiliar = (list *) lista;
    no *no_auxiliar = lista_auxiliar->primeiro;

    while (no_auxiliar != NULL) {
        const char *id_atual = figura_obter_id(no_auxiliar->figura);

        if (strcmp(id_atual, id) == 0) {
            return no_auxiliar;
        }

        no_auxiliar = no_auxiliar->proximo;
    }
    return NULL;
}

No lista_get_first(Lista lista) {
    list *lista_aux = (list *) lista;
    if (lista_aux->tamanho == 0) {
        return NULL;
    } else {
        return lista_aux->primeiro;
    }
}

No lista_get_last(Lista lista) {
    list *lista_aux = (list *) lista;
    if (lista_aux->tamanho == 0) {
        return NULL;
    }
    return lista_aux->ultimo;
}

Figura lista_get_figura(No p) {
    no *node_auxiliar;
    node_auxiliar = p;
    if (p == NULL) {
        LOG_ERROR("Erro ao obter informações do nó especificado\n");
        return NULL;
    }
    return node_auxiliar->figura;
}

No lista_get_next(Lista lista, No p) {
    list *lista_aux = (list *) lista;
    no *node_aux = (no *) p;
    if (node_aux == lista_aux->ultimo) {
        return NULL;
    }
    return node_aux->proximo;
}

No lista_get_previous(Lista lista, No p) {
    list *lista_aux = (list *) lista;
    no *node_aux = (no *) p;
    if (node_aux == lista_aux->primeiro) {
        return NULL;
    }
    return node_aux->anterior;
}

void lista_libera_lista(Lista lista) {
    list *lista_aux = (list *) lista;
    no *node_atual = lista_aux->primeiro;
    no *node_proximo;

    while (node_atual != NULL) {
        node_proximo = node_atual->proximo;
        figura_destruir(node_atual->figura);
        free(node_atual);
        node_atual = node_proximo;
    }

    lista_aux->primeiro = NULL;
    lista_aux->ultimo = NULL;
    free(lista_aux);
    lista_aux = NULL;

    LOG_INFO("Lista liberada\n");
}
