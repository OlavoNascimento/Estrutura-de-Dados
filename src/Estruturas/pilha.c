#include "pilha.h"

#include <stdbool.h>
#include <stdlib.h>

#include "../Interfaces/figura.h"
#include "../Utils/logging.h"

typedef struct N {
    Figura figura;
    struct N *proximo;
} No;

typedef struct {
    int tamanho;
    No *topo;
} PilhaImp;

Pilha pilha_criar() {
    PilhaImp *pilha = (PilhaImp *) malloc(sizeof(PilhaImp));
    if (pilha == NULL) {
        LOG_ERRO("Erro ao alocar espaço para a pilha!\n");
        return NULL;
    }
    pilha->topo = NULL;
    pilha->tamanho = 0;
    return pilha;
}

bool pilha_esta_vazia(Pilha pilha) {
    PilhaImp *pilhaAux = (PilhaImp *) pilha;
    return pilhaAux->topo == NULL;
}

int pilha_obter_tamanho(Pilha pilha) {
    PilhaImp *pilhaAux = (PilhaImp *) pilha;
    return pilhaAux->tamanho;
}

void pilha_inserir(Pilha pilha, Figura figura) {
    PilhaImp *pilhaAux = (PilhaImp *) pilha;
    No *novo_no = malloc(sizeof(No));
    novo_no->figura = figura;
    novo_no->proximo = pilhaAux->topo;
    pilhaAux->topo = novo_no;
    pilhaAux->tamanho++;
}

Figura pilha_remover(Pilha pilha) {
    PilhaImp *pilhaAux = (PilhaImp *) pilha;
    if (pilha_esta_vazia(pilha))
        return NULL;

    Figura fig = pilhaAux->topo->figura;
    No *topo = pilhaAux->topo;
    pilhaAux->topo = topo->proximo;
    free(topo);
    pilhaAux->tamanho--;

    return fig;
}

Figura pilha_obter_topo(Pilha pilha) {
    PilhaImp *pilhaAux = (PilhaImp *) pilha;
    if (pilha_esta_vazia(pilha))
        return NULL;
    return pilhaAux->topo->figura;
}

void pilha_destruir(Pilha pilha) {
    PilhaImp *pilhaAux = (PilhaImp *) pilha;
    for (No *i = pilhaAux->topo; i != NULL; i = i->proximo)
        free(i);
    free(pilha);
}
