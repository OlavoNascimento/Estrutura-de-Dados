#include "lista.h"

#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "figuras.h"

// Margem entre o svg e a figuras mais próximas de suas bordas.
#define SVG_MARGEM 14
// Margem entra a figura e seu rótulo.
#define ROTULO_MARGEM 4

typedef struct circulo {
    int id;
} circ;

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
    list *lista = (list *)malloc(sizeof(list));
    if (lista == NULL) {
        printf("Erro ao alocar espaço para a lista\n");
        exit(1);
    }

    lista->tamanho = 0;
    lista->primeiro = NULL;
    lista->ultimo = NULL;
    return lista;
}

int lista_get_length(Lista lista) {
    list *listaAux = (list *)lista;
    return listaAux->tamanho;
}

Posic lista_insert_final(Lista lista, Figura figura) {
    list *listaAux = (list *)lista;
    no *nodeAux;
    no *nodeInsert = (no *)malloc(sizeof(no));
    nodeInsert->figura = figura;
    int tamAux = listaAux->tamanho;

    if (listaAux->primeiro == NULL) {
        listaAux->primeiro = nodeInsert;
        nodeInsert->anterior = NULL;
        nodeInsert->proximo = NULL;

        listaAux->ultimo = nodeInsert;
        listaAux->tamanho++;
        return nodeInsert;
    } else {
        nodeAux = listaAux->ultimo;
        nodeAux->proximo = nodeInsert;
        nodeInsert->anterior = nodeAux;
        nodeInsert->proximo = NULL;
        listaAux->ultimo = nodeInsert;
        listaAux->tamanho++;
        return nodeInsert;
    }

    if (listaAux->tamanho != tamAux) {
        printf("Nó adicionado com sucesso!\n");
    }
}

Posic lista_insert_after(Lista lista, Figura figura, Posic p) {
    list *lista_aux = (list *)lista;
    no *node_aux;
    no *node_proximo = NULL;
    no *node_anterior = NULL;
    no *node_insert = (no *)malloc(sizeof(no));
    int tam_aux = lista_aux->tamanho;

    node_insert->figura = figura;
    node_aux = p;

    if (node_aux == lista_aux->ultimo) {  //caso seja inserido depois do último
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
    if (tam_aux != lista_aux->tamanho) {
        printf("Elemento inserido com sucesso (insert_after)\n");
    } else {
        printf("Erro ao inserir elemento (insert_after)\n");
        exit(1);
    }

    return node_insert;
}

Posic lista_insert_before(Lista lista, Figura figura, Posic p) {
    list *lista_aux = (list *)lista;
    no *node_aux;
    no *node_proximo = NULL;
    no *node_anteriror = NULL;
    no *node_insert = (no *)malloc(sizeof(no));
    int tam_aux = lista_aux->tamanho;

    node_insert->figura = figura;
    node_aux = p;

    if (node_aux == lista_aux->primeiro) {  //caso seja inserido antes do primeiro
        node_aux->anterior = node_insert;
        node_insert->proximo = node_aux;
        node_insert->anterior = NULL;

        lista_aux->primeiro = node_insert;
        lista_aux->tamanho++;
    } else {
        node_anteriror = node_aux->anterior;
        node_aux->anterior = node_insert;
        node_anteriror->proximo = node_insert;

        node_insert->proximo = node_aux;
        node_insert->anterior = node_anteriror;
        lista_aux->tamanho++;
    }
    if (tam_aux != lista_aux->tamanho) {
        printf("Elemento inserido com sucesso (insert_before)\n");
    } else {
        printf("Erro ao inserir elemento (insert_before)\n");
        exit(1);
    }

    return node_insert;
}

void lista_printLista(Lista lista) {
    if (lista == NULL) {
        printf("Lista não inicializada\n");
        return;
    }

    list *listaAux = (list *)lista;
    no *node = listaAux->primeiro;
    int i = 0;

    if (listaAux->primeiro == NULL) {
        printf("Lista vazia, impossível printar\n");
        return;
    }

    while (node != NULL) {
        printf("No %d => %f id:%s endereço do nó: %p\n", i, retorna_circuloX(node->figura), retorna_circuloID(node->figura), node);
        node = node->proximo;

        i++;
    }
}

void lista_remove_no(Lista lista, Posic no_selecionado) {
    list *lista_auxiliar = (list *)lista;
    no *no_auxiliar = (no *)no_selecionado;
    no *no_anterior = NULL;
    no *no_proximo = NULL;

    if (no_auxiliar == NULL) {
        printf("ID invalido\n");
        system("pause");
    } else {
        if (no_auxiliar == lista_auxiliar->primeiro) {  //se for o primeiro elemento da lista
            no_proximo = no_auxiliar->proximo;
            lista_auxiliar->primeiro = no_proximo;
            no_proximo->anterior = NULL;

            free(no_auxiliar);
            lista_auxiliar->tamanho--;
        } else if (no_auxiliar == lista_auxiliar->ultimo) {  //se for o último elemento da lista
            no_anterior = no_auxiliar->anterior;
            lista_auxiliar->ultimo = no_anterior;
            no_anterior->proximo = NULL;

            free(no_auxiliar);
            lista_auxiliar->tamanho--;
        } else {  //se estiver no meio da lista
            no_anterior = no_auxiliar->anterior;
            no_proximo = no_auxiliar->proximo;

            no_anterior->proximo = no_proximo;
            no_proximo->anterior = no_anterior;

            free(no_auxiliar);
            lista_auxiliar->tamanho--;
        }

        printf("Elemento removido com sucesso\n");
        return;
    }
}

Posic lista_get_posic(Lista lista, char id[20]) {
    list *lista_auxiliar = (list *)lista;
    no *no_auxiliar = lista_auxiliar->primeiro;
    char id_auxiliar[20];
    bool saida = false;

    while (saida != true) {
        strcpy(id_auxiliar, retorna_circuloID(no_auxiliar->figura));
        if (strcmp(id_auxiliar, id) == 0) {
            return no_auxiliar;
        }
        if (no_auxiliar == lista_auxiliar->ultimo) {
            break;
        }

        no_auxiliar = no_auxiliar->proximo;
    }

    printf("Elemento não encontrando\n");
    exit(1);
}

Posic lista_get_first(Lista lista) {
    list *lista_aux = (list *)lista;
    if (lista_aux->tamanho == 0) {
        printf("Não há elementos na lista\n");
        return NULL;
    } else {
        return lista_aux->primeiro;
    }
}

Posic lista_get_last(Lista lista) {
    list *lista_aux = (list *)lista;
    if (lista_aux->tamanho == 0) {
        printf("Não há elementos na lista\n");
        return NULL;
    } else {
        printf("retornando %p\n", lista_aux->ultimo);
        return lista_aux->ultimo;
    }
}

Info lista_get_infos(Posic p) {
    no *node_auxiliar;
    node_auxiliar = p;
    if (p == NULL) {
        printf("Erro ao obter infromações do nó especificado\n");
        exit(1);
    } else {
        return node_auxiliar->figura;
    }
}

Posic lista_get_next(Lista lista, Posic p) {
    list *lista_aux = (list *)lista;
    no *node_aux = (no *)p;
    if (node_aux == lista_aux->ultimo) {
        return NULL;
    } else {
        return node_aux->proximo;
    }
}

Posic lista_get_previous(Lista lista, Posic p) {
    list *lista_aux = (list *)lista;
    no *node_aux = (no *)p;
    if (node_aux == lista_aux->primeiro) {
        return NULL;
    } else {
        return node_aux->anterior;
    }
}

Lista lista_libera_lista(Lista lista) {
    list *lista_aux = (list *)lista;
    no *node_atual = lista_aux->primeiro;
    no *node_proximo;

    while (node_atual != NULL) {
        node_proximo = node_atual->proximo;
        free(node_atual);
        node_atual = node_proximo;
    }

    lista_aux->primeiro = NULL;
    lista_aux->ultimo = NULL;
    free(lista_aux);
    lista_aux = NULL;

    return NULL;
    printf("Lista liberada\n");
}
