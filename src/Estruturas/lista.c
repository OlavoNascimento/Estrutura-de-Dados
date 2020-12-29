#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Interfaces/figura.h"
#include "../Utils/logging.h"

typedef struct N {
    Figura figura;
    struct N *proximo;
    struct N *anterior;
} NoImp;

typedef struct {
    int tamanho;
    NoImp *primeiro;
    NoImp *ultimo;
} ListaImp;

Lista lista_criar() {
    ListaImp *lista = (ListaImp *) malloc(sizeof(ListaImp));
    if (lista == NULL) {
        LOG_ERRO("Erro ao alocar espaço para uma nova lista!\n");
        return NULL;
    }

    lista->tamanho = 0;
    lista->primeiro = NULL;
    lista->ultimo = NULL;
    return lista;
}

int lista_obter_tamanho(Lista lista) {
    ListaImp *listaAux = (ListaImp *) lista;
    return listaAux->tamanho;
}

No lista_inserir_final(Lista lista, Figura figura) {
    if (figura == NULL) {
        LOG_ERRO("Figura nula passada para lista_inserir_final!\n");
        return NULL;
    }

    ListaImp *listaAux = (ListaImp *) lista;
    NoImp *nodeAux;
    NoImp *nodeInsert = (NoImp *) malloc(sizeof(NoImp));
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

No lista_inserir_antes(Lista lista, Figura figura, No p) {
    if (p == NULL) {
        LOG_ERRO("Nó nulo passado para lista_inserir_antes!\n");
        return NULL;
    }
    if (figura == NULL) {
        LOG_ERRO("Figura nula passada para lista_inserir_antes!\n");
        return NULL;
    }

    ListaImp *lista_aux = (ListaImp *) lista;
    NoImp *node_aux;
    NoImp *node_anterior = NULL;
    NoImp *node_insert = (NoImp *) malloc(sizeof(NoImp));
    int tam_aux = lista_aux->tamanho;

    node_insert->figura = figura;
    node_aux = p;

    if (node_aux == lista_aux->primeiro) {  // caso seja inserido antes do primeiro
        node_aux->anterior = node_insert;
        node_insert->proximo = node_aux;
        node_insert->anterior = NULL;

        lista_aux->primeiro = node_insert;
    } else {
        node_anterior = node_aux->anterior;
        node_aux->anterior = node_insert;
        node_anterior->proximo = node_insert;

        node_insert->proximo = node_aux;
        node_insert->anterior = node_anterior;
    }
    if (tam_aux == lista_aux->tamanho) {
        LOG_ERRO("Erro ao inserir elemento (lista_inserir_antes)!\n");
        return NULL;
    }
    lista_aux->tamanho++;

    return node_insert;
}

No lista_inserir_depois(Lista lista, Figura figura, No p) {
    if (p == NULL) {
        LOG_ERRO("Nó nulo passado para lista_inserir_depois!\n");
        return NULL;
    }
    if (figura == NULL) {
        LOG_ERRO("Figura nula passada para lista_inserir_depois!\n");
        return NULL;
    }

    ListaImp *lista_aux = (ListaImp *) lista;
    NoImp *node_aux;
    NoImp *node_proximo = NULL;
    NoImp *node_insert = (NoImp *) malloc(sizeof(NoImp));
    int tam_aux = lista_aux->tamanho;

    node_insert->figura = figura;
    node_aux = p;

    if (node_aux == lista_aux->ultimo) {  // Caso seja inserido depois do último.
        node_aux->proximo = node_insert;
        lista_aux->ultimo = node_insert;

        node_insert->anterior = node_aux;
        node_insert->proximo = NULL;
    } else {
        node_proximo = node_aux->proximo;
        node_aux->proximo = node_insert;
        node_proximo->anterior = node_insert;

        node_insert->proximo = node_proximo;
        node_insert->anterior = node_aux;
    }
    if (tam_aux == lista_aux->tamanho) {
        LOG_ERRO("Erro ao inserir elemento (lista_inserir_depois)\n");
        return NULL;
    }
    lista_aux->tamanho++;

    return node_insert;
}

void lista_remover(Lista lista, No no_selecionado) {
    if (no_selecionado == NULL) {
        LOG_ERRO("Nó nulo passado para lista_remover!\n");
        return;
    }
    ListaImp *lista_auxiliar = (ListaImp *) lista;
    NoImp *no_auxiliar = (NoImp *) no_selecionado;
    NoImp *no_anterior = NULL;
    NoImp *no_proximo = NULL;

    if (no_auxiliar == lista_auxiliar->primeiro) {  // Se for o primeiro elemento da lista.
        no_proximo = no_auxiliar->proximo;
        lista_auxiliar->primeiro = no_proximo;
        no_proximo->anterior = NULL;
    } else if (no_auxiliar == lista_auxiliar->ultimo) {  // Se for o último elemento da lista.
        no_anterior = no_auxiliar->anterior;
        lista_auxiliar->ultimo = no_anterior;
        no_anterior->proximo = NULL;
    } else {  // Se estiver no meio da lista.
        no_anterior = no_auxiliar->anterior;
        no_proximo = no_auxiliar->proximo;

        no_anterior->proximo = no_proximo;
        no_proximo->anterior = no_anterior;
    }

    figura_destruir(no_auxiliar->figura);
    free(no_auxiliar);
    lista_auxiliar->tamanho--;
}

No lista_buscar(Lista lista, const char id[100]) {
    ListaImp *lista_auxiliar = (ListaImp *) lista;
    NoImp *no_auxiliar = lista_auxiliar->primeiro;

    while (no_auxiliar != NULL) {
        const char *id_atual = figura_obter_id(no_auxiliar->figura);

        if (strcmp(id_atual, id) == 0) {
            return no_auxiliar;
        }

        no_auxiliar = no_auxiliar->proximo;
    }
    return NULL;
}

No lista_obter_primeiro(Lista lista) {
    ListaImp *lista_aux = (ListaImp *) lista;
    if (lista_aux->tamanho == 0)
        return NULL;
    return lista_aux->primeiro;
}

No lista_obter_ultimo(Lista lista) {
    ListaImp *lista_aux = (ListaImp *) lista;
    if (lista_aux->tamanho == 0) {
        return NULL;
    }
    return lista_aux->ultimo;
}

Figura lista_obter_figura(No p) {
    if (p == NULL) {
        LOG_ERRO("Nó nulo passado para lista_obter_figura!\n");
        return NULL;
    }
    NoImp *node_auxiliar = (NoImp *) p;
    return node_auxiliar->figura;
}

void lista_definir_figura(No p, Figura figura) {
    if (p == NULL) {
        LOG_ERRO("Nó nulo passado para lista_definir_figura!\n");
        return;
    }
    if (figura == NULL) {
        LOG_ERRO("Figura nula passada para lista_definir_figura!\n");
        return;
    }

    NoImp *no_auxiliar = (NoImp *) p;
    no_auxiliar->figura = figura;
}

No lista_obter_proximo(No p) {
    if (p == NULL) {
        LOG_ERRO("Nó nulo passado para lista_obter_proximo!\n");
        return NULL;
    }
    NoImp *node_aux = (NoImp *) p;
    return node_aux->proximo;
}

No lista_obter_anterior(No p) {
    if (p == NULL) {
        LOG_ERRO("Nó nulo passado para lista_obter_anterior!\n");
        return NULL;
    }
    NoImp *node_aux = (NoImp *) p;
    return node_aux->anterior;
}

// Troca as figuras armazenadas em dois nós.
void lista_trocar_figuras(No no1, No no2) {
    Figura temp = lista_obter_figura(no1);
    lista_definir_figura(no1, lista_obter_figura(no2));
    lista_definir_figura(no2, temp);
}

void lista_destruir(Lista lista) {
    ListaImp *lista_aux = (ListaImp *) lista;
    NoImp *node_atual = lista_aux->primeiro;
    NoImp *node_proximo;

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
