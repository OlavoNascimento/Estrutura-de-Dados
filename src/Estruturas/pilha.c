#include "pilha.h"

#include <stdbool.h>
#include <stdlib.h>

#include "../Utils/logging.h"

typedef struct N {
    PilhaInfo info;
    struct N *proximo;
} No;

typedef struct {
    DestruirInfo *destruir_info;
    int tamanho;
    No *topo;
} PilhaImp;

Pilha pilha_criar(DestruirInfo destruir_info) {
    PilhaImp *pilha = (PilhaImp *) malloc(sizeof(PilhaImp));
    if (pilha == NULL) {
        LOG_ERRO("Erro ao alocar espaço para a pilha!\n");
        return NULL;
    }
    pilha->tamanho = 0;
    pilha->topo = NULL;
    pilha->destruir_info = destruir_info;
    return pilha;
}

bool pilha_esta_vazia(Pilha pilha) {
    PilhaImp *pilhaImp = (PilhaImp *) pilha;
    return pilhaImp->topo == NULL;
}

int pilha_obter_tamanho(Pilha pilha) {
    PilhaImp *pilhaImp = (PilhaImp *) pilha;
    return pilhaImp->tamanho;
}

void pilha_inserir(Pilha pilha, PilhaInfo info) {
    PilhaImp *pilhaImp = (PilhaImp *) pilha;
    No *novo_no = malloc(sizeof(No));
    novo_no->info = info;
    novo_no->proximo = pilhaImp->topo;
    pilhaImp->topo = novo_no;
    pilhaImp->tamanho++;
}

PilhaInfo pilha_remover(Pilha pilha) {
    PilhaImp *pilhaImp = (PilhaImp *) pilha;
    if (pilha_esta_vazia(pilha))
        return NULL;

    PilhaInfo info = pilhaImp->topo->info;
    No *topo = pilhaImp->topo;
    pilhaImp->topo = topo->proximo;
    free(topo);
    pilhaImp->tamanho--;

    return info;
}

PilhaInfo pilha_obter_topo(Pilha pilha) {
    PilhaImp *pilhaImp = (PilhaImp *) pilha;
    if (pilha_esta_vazia(pilha))
        return NULL;
    return pilhaImp->topo->info;
}

void pilha_destruir(Pilha pilha) {
    PilhaImp *pilhaImp = (PilhaImp *) pilha;

    No *no_atual = pilhaImp->topo;
    while (no_atual != NULL) {
        No *no_proximo = no_atual->proximo;
        if (pilhaImp->destruir_info != NULL)
            (pilhaImp->destruir_info(no_atual->info));
        free(no_atual);
        no_atual = no_proximo;
    }
    free(pilha);
}
