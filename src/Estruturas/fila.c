#include "fila.h"

#include <stdbool.h>
#include <stdlib.h>

#include "../Utils/logging.h"

struct No {
    FilaInfo info;
    struct No *proximo;
};

struct Fila_s {
    FilaDestruirInfo *destruir_info;
    int tamanho;
    struct No *primeiro;
    struct No *ultimo;
};

Fila fila_criar(FilaDestruirInfo destruir_info) {
    Fila fila = malloc(sizeof *fila);
    if (fila == NULL) {
        LOG_ERRO("Falha ao alocar memória\n");
        return NULL;
    }
    fila->destruir_info = destruir_info;
    fila->tamanho = 0;
    fila->primeiro = NULL;
    fila->ultimo = NULL;
    return fila;
}

void fila_inserir(Fila fila, FilaInfo info) {
    struct No *novo_no = malloc(sizeof *novo_no);
    if (novo_no == NULL) {
        LOG_ERRO("Falha ao alocar memória\n");
        return;
    }
    novo_no->info = info;
    novo_no->proximo = NULL;

    if (fila->primeiro == NULL) {
        fila->primeiro = novo_no;
    } else {
        fila->ultimo->proximo = novo_no;
    }
    fila->ultimo = novo_no;
    fila->tamanho++;
}

FilaInfo fila_remover(Fila fila) {
    if (fila_esta_vazia(fila))
        return NULL;
    FilaInfo info = fila->primeiro->info;

    struct No *primeiro = fila->primeiro;
    fila->primeiro = primeiro->proximo;
    free(primeiro);
    fila->tamanho--;

    return info;
}

bool fila_esta_vazia(Fila fila) {
    return fila->primeiro == NULL;
}

int fila_obter_tamanho(Fila fila) {
    return fila->tamanho;
}

FilaInfo fila_obter_info(Fila fila) {
    if (fila_esta_vazia(fila))
        return NULL;
    return fila->primeiro->info;
}

void fila_destruir(Fila fila) {
    struct No *no_atual = fila->primeiro;
    while (no_atual != NULL) {
        struct No *no_proximo = no_atual->proximo;
        if (fila->destruir_info != NULL)
            fila->destruir_info(no_atual->info);
        free(no_atual);
        no_atual = no_proximo;
    }
    free(fila);
}
