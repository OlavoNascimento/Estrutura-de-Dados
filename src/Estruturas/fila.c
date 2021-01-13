#include "fila.h"

#include <stdbool.h>
#include <stdlib.h>

#include "../Utils/logging.h"

typedef struct N {
    FilaInfo info;
    struct N *proximo;
} No;

typedef struct {
    DestruirInfo *destruir_info;
    int tamanho;
    No *primeiro;
    No *ultimo;
} FilaImp;

Fila fila_criar(DestruirInfo destruir_info) {
    FilaImp *fila = (FilaImp *) malloc(sizeof *fila);
    if (fila == NULL) {
        LOG_ERRO("Erro ao alocar espaço para a fila!\n");
        return NULL;
    }
    fila->destruir_info = destruir_info;
    fila->tamanho = 0;
    fila->primeiro = NULL;
    fila->ultimo = NULL;
    return fila;
}

bool fila_esta_vazia(Fila fila) {
    FilaImp *filaImp = (FilaImp *) fila;
    return filaImp->primeiro == NULL;
}

int fila_obter_tamanho(Fila fila) {
    FilaImp *filaImp = (FilaImp *) fila;
    return filaImp->tamanho;
}

void fila_inserir(Fila fila, FilaInfo info) {
    FilaImp *filaImp = (FilaImp *) fila;
    No *novo_no = malloc(sizeof *novo_no);
    novo_no->info = info;
    novo_no->proximo = NULL;

    if (filaImp->primeiro == NULL) {
        filaImp->primeiro = novo_no;
    } else {
        filaImp->ultimo->proximo = novo_no;
    }
    filaImp->ultimo = novo_no;
    filaImp->tamanho++;
}

FilaInfo fila_remover(Fila fila) {
    FilaImp *filaImp = (FilaImp *) fila;
    if (fila_esta_vazia(fila))
        return NULL;

    FilaInfo info = filaImp->primeiro->info;

    No *primeiro = filaImp->primeiro;
    filaImp->primeiro = primeiro->proximo;
    free(primeiro);
    filaImp->tamanho--;

    return info;
}

FilaInfo fila_obter_info(Fila fila) {
    FilaImp *filaImp = (FilaImp *) fila;
    if (fila_esta_vazia(fila))
        return NULL;
    return filaImp->primeiro->info;
}

void fila_destruir(Fila fila) {
    FilaImp *filaImp = (FilaImp *) fila;

    No *no_atual = filaImp->primeiro;
    while (no_atual != NULL) {
        No *no_proximo = no_atual->proximo;
        if (filaImp->destruir_info != NULL)
            (filaImp->destruir_info(no_atual->info));
        free(no_atual);
        no_atual = no_proximo;
    }
    free(fila);
}
