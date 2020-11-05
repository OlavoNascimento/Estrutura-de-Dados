#include "pilha.h"

#include <stdbool.h>
#include <stdlib.h>

#include "logging.h"

typedef struct N {
    Figura figura;
    struct N *proximo;
} No;

typedef struct {
    int tamanho;
    No *topo;
} PilhaImp;

Pilha pilha_create() {
    PilhaImp *pilha = (PilhaImp *) malloc(sizeof(PilhaImp));
    if (pilha == NULL) {
        LOG_ERROR("Erro ao alocar espaço para a pilha!\n");
        return NULL;
    }
    pilha->topo = NULL;
    pilha->tamanho = 0;
    return pilha;
}

bool pilha_is_empty(Pilha pilha) {
    PilhaImp *pilhaAux = (PilhaImp *) pilha;
    return pilhaAux->topo == NULL;
}

int pilha_get_tamanho(Pilha pilha) {
    PilhaImp *pilhaAux = (PilhaImp *) pilha;
    return pilhaAux->tamanho;
}

void pilha_push(Pilha pilha, Figura figura) {
    PilhaImp *pilhaAux = (PilhaImp *) pilha;
    No *novo_no = malloc(sizeof(No));
    novo_no->figura = figura;
    novo_no->proximo = pilhaAux->topo;
    pilhaAux->topo = novo_no;
    pilhaAux->tamanho++;
}

Figura pilha_pop(Pilha pilha) {
    PilhaImp *pilhaAux = (PilhaImp *) pilha;
    if (pilha_is_empty(pilha))
        return NULL;

    Figura fig = pilhaAux->topo->figura;
    No *topo = pilhaAux->topo;
    pilhaAux->topo = topo->proximo;
    free(topo);
    pilhaAux->tamanho--;

    return fig;
}

Figura pilha_peek(Pilha pilha) {
    PilhaImp *pilhaAux = (PilhaImp *) pilha;
    if (pilha_is_empty(pilha))
        return NULL;
    return pilhaAux->topo->figura;
}

void pilha_destruir(Pilha pilha) {
    PilhaImp *pilhaAux = (PilhaImp *) pilha;
    for (No *i = pilhaAux->topo; i != NULL; i = i->proximo) {
        free(i->figura);
        free(i);
    }
    free(pilha);
}
