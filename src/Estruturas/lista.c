#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Utils/logging.h"

typedef struct N {
    ListaInfo info;
    struct N *proximo;
    struct N *anterior;
} NoImp;

typedef struct {
    ObterIdentificadorInfo *obter_identificador_info;
    DestruirInfo *destruir_info;
    int tamanho;
    NoImp *primeiro;
    NoImp *ultimo;
} ListaImp;

Lista lista_criar(ObterIdentificadorInfo obter_identificador_info, DestruirInfo destruir_info) {
    ListaImp *lista = malloc(sizeof *lista);
    if (lista == NULL) {
        LOG_ERRO("Erro ao alocar espaço para uma nova lista!\n");
        return NULL;
    }
    lista->obter_identificador_info = obter_identificador_info;
    lista->destruir_info = destruir_info;
    lista->tamanho = 0;
    lista->primeiro = NULL;
    lista->ultimo = NULL;
    return lista;
}

int lista_obter_tamanho(Lista lista) {
    ListaImp *lista_aux = lista;
    return lista_aux->tamanho;
}

ListaNo lista_inserir_final(Lista lista, ListaInfo info) {
    if (lista == NULL) {
        LOG_ERRO("Lista nula passada para lista_inserir_final!\n");
        return NULL;
    }
    if (info == NULL) {
        LOG_ERRO("Informação nula passada para lista_inserir_final!\n");
        return NULL;
    }

    ListaImp *lista_aux = lista;
    NoImp *novo_no = malloc(sizeof *novo_no);
    novo_no->info = info;
    novo_no->proximo = NULL;

    if (lista_aux->primeiro == NULL) {
        lista_aux->primeiro = novo_no;
        novo_no->anterior = NULL;
    } else {
        lista_aux->ultimo->proximo = novo_no;
        novo_no->anterior = lista_aux->ultimo;
    }

    lista_aux->ultimo = novo_no;
    lista_aux->tamanho++;
    return novo_no;
}

ListaNo lista_inserir_antes(Lista lista, ListaInfo info, ListaNo p) {
    if (p == NULL) {
        LOG_ERRO("Nó nulo passado para lista_inserir_antes!\n");
        return NULL;
    }
    if (info == NULL) {
        LOG_ERRO("Informação nula passada para lista_inserir_antes!\n");
        return NULL;
    }

    ListaImp *lista_aux = lista;
    NoImp *no = p;
    NoImp *no_anterior = NULL;
    NoImp *novo_no = malloc(sizeof *novo_no);

    novo_no->info = info;

    if (no == lista_aux->primeiro) {  // caso seja inserido antes do primeiro
        no->anterior = novo_no;
        novo_no->proximo = no;
        novo_no->anterior = NULL;

        lista_aux->primeiro = novo_no;
    } else {
        no_anterior = no->anterior;
        no->anterior = novo_no;
        no_anterior->proximo = novo_no;

        novo_no->proximo = no;
        novo_no->anterior = no_anterior;
    }
    lista_aux->tamanho++;

    return novo_no;
}

ListaNo lista_inserir_depois(Lista lista, ListaInfo info, ListaNo p) {
    if (p == NULL) {
        LOG_ERRO("Nó nulo passado para lista_inserir_depois!\n");
        return NULL;
    }
    if (info == NULL) {
        LOG_ERRO("Informação nula passada para lista_inserir_depois!\n");
        return NULL;
    }

    ListaImp *lista_aux = lista;
    NoImp *no = p;
    NoImp *no_proximo = NULL;
    NoImp *novo_no = malloc(sizeof *novo_no);

    novo_no->info = info;

    if (no == lista_aux->ultimo) {  // Caso seja inserido depois do último.
        no->proximo = novo_no;
        lista_aux->ultimo = novo_no;

        novo_no->anterior = no;
        novo_no->proximo = NULL;
    } else {
        no_proximo = no->proximo;
        no->proximo = novo_no;
        no_proximo->anterior = novo_no;

        novo_no->proximo = no_proximo;
        novo_no->anterior = no;
    }
    lista_aux->tamanho++;

    return novo_no;
}

void lista_remover(Lista lista, ListaNo no_selecionado) {
    if (no_selecionado == NULL) {
        LOG_ERRO("Nó nulo passado para lista_remover!\n");
        return;
    }
    ListaImp *lista_auxiliar = lista;
    NoImp *no = no_selecionado;
    NoImp *no_anterior = no->anterior;
    NoImp *no_proximo = no->proximo;

    if (no == lista_auxiliar->primeiro) {  // Se for o primeiro elemento da lista.
        lista_auxiliar->primeiro = no_proximo;
        // Caso a lista só tenha um elemento não é necessário mudar o ponteiro do próximo elemento.
        if (no_proximo != NULL)
            no_proximo->anterior = NULL;
    } else if (no == lista_auxiliar->ultimo) {  // Se for o último elemento da lista.
        lista_auxiliar->ultimo = no_anterior;
        no_anterior->proximo = NULL;
    } else {  // Se estiver no meio da lista.
        no_anterior->proximo = no->proximo;
        no_proximo->anterior = no->anterior;
    }

    if (lista_auxiliar->destruir_info != NULL)
        lista_auxiliar->destruir_info(no->info);
    free(no);
    lista_auxiliar->tamanho--;
}

ListaNo lista_buscar(Lista lista, const char *id) {
    if (lista == NULL) {
        LOG_ERRO("Lista nula passada para lista_buscar!\n");
        return NULL;
    }
    ListaImp *lista_auxiliar = lista;
    if (lista_auxiliar->obter_identificador_info == NULL) {
        LOG_ERRO(
            "Não é possível buscar em uma lista que não possui a função obter_identificador_info "
            "definida!\n");
        return NULL;
    }

    NoImp *no = lista_auxiliar->primeiro;
    while (no != NULL) {
        const char *id_atual = lista_auxiliar->obter_identificador_info(no->info);
        if (strcmp(id_atual, id) == 0)
            return no;
        no = no->proximo;
    }
    return NULL;
}

ListaNo lista_obter_primeiro(Lista lista) {
    if (lista == NULL) {
        LOG_ERRO("Lista nula passada para lista_obter_primeiro!\n");
        return NULL;
    }
    ListaImp *lista_aux = lista;
    return lista_aux->primeiro;
}

ListaNo lista_obter_ultimo(Lista lista) {
    if (lista == NULL) {
        LOG_ERRO("Lista nula passada para lista_obter_ultimo!\n");
        return NULL;
    }
    ListaImp *lista_aux = lista;
    return lista_aux->ultimo;
}

ListaInfo lista_obter_info(ListaNo p) {
    if (p == NULL) {
        LOG_ERRO("Nó nulo passado para lista_obter_info!\n");
        return NULL;
    }
    NoImp *no = p;
    return no->info;
}

ListaNo lista_obter_proximo(ListaNo p) {
    if (p == NULL) {
        LOG_ERRO("Nó nulo passado para lista_obter_proximo!\n");
        return NULL;
    }
    NoImp *no = p;
    return no->proximo;
}

ListaNo lista_obter_anterior(ListaNo p) {
    if (p == NULL) {
        LOG_ERRO("Nó nulo passado para lista_obter_anterior!\n");
        return NULL;
    }
    NoImp *no = p;
    return no->anterior;
}

// Troca as informações armazenadas em dois nós.
void lista_trocar_info(ListaNo no1, ListaNo no2) {
    if (no1 == NULL || no2 == NULL) {
        LOG_ERRO("Nó nulo passado para lista_trocar_info!\n");
        return;
    }
    NoImp *noImp1 = no1;
    NoImp *noImp2 = no2;

    ListaInfo temp = lista_obter_info(no1);
    noImp1->info = lista_obter_info(no2);
    noImp2->info = temp;
}

// Aplica uma função a todas as informações de uma lista.
void lista_map(Lista lis, MapInfoLista f, void *extra) {
    for_each_lista(no, lis) {
        f(lista_obter_info(no), extra);
    }
}

void lista_destruir(Lista lista) {
    ListaImp *lista_aux = lista;
    NoImp *no_atual = lista_aux->primeiro;

    while (no_atual != NULL) {
        NoImp *no_proximo = no_atual->proximo;
        if (lista_aux->destruir_info != NULL)
            lista_aux->destruir_info(no_atual->info);
        free(no_atual);
        no_atual = no_proximo;
    }
    free(lista_aux);
}
