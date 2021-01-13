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
    ListaImp *lista_aux = (ListaImp *) lista;
    return lista_aux->tamanho;
}

ListaNo lista_inserir_final(Lista lista, ListaInfo info) {
    if (info == NULL) {
        LOG_INFO("Informação nula passada para lista_inserir_final!\n");
        return NULL;
    }

    ListaImp *lista_aux = (ListaImp *) lista;
    NoImp *no_aux;
    NoImp *novo_no = malloc(sizeof *novo_no);
    novo_no->info = info;

    if (lista_aux->primeiro == NULL) {
        lista_aux->primeiro = novo_no;
        novo_no->anterior = NULL;
    } else {
        no_aux = lista_aux->ultimo;
        no_aux->proximo = novo_no;
        novo_no->anterior = no_aux;
    }

    novo_no->proximo = NULL;
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

    ListaImp *lista_aux = (ListaImp *) lista;
    NoImp *no_aux;
    NoImp *no_anterior = NULL;
    NoImp *novo_no = malloc(sizeof *novo_no);

    novo_no->info = info;
    no_aux = p;

    if (no_aux == lista_aux->primeiro) {  // caso seja inserido antes do primeiro
        no_aux->anterior = novo_no;
        novo_no->proximo = no_aux;
        novo_no->anterior = NULL;

        lista_aux->primeiro = novo_no;
    } else {
        no_anterior = no_aux->anterior;
        no_aux->anterior = novo_no;
        no_anterior->proximo = novo_no;

        novo_no->proximo = no_aux;
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

    ListaImp *lista_aux = (ListaImp *) lista;
    NoImp *no_aux;
    NoImp *no_proximo = NULL;
    NoImp *novo_no = malloc(sizeof *novo_no);

    novo_no->info = info;
    no_aux = p;

    if (no_aux == lista_aux->ultimo) {  // Caso seja inserido depois do último.
        no_aux->proximo = novo_no;
        lista_aux->ultimo = novo_no;

        novo_no->anterior = no_aux;
        novo_no->proximo = NULL;
    } else {
        no_proximo = no_aux->proximo;
        no_aux->proximo = novo_no;
        no_proximo->anterior = novo_no;

        novo_no->proximo = no_proximo;
        novo_no->anterior = no_aux;
    }
    lista_aux->tamanho++;

    return novo_no;
}

void lista_remover(Lista lista, ListaNo no_selecionado) {
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
        // Caso a lista só tenha um elemento não é necessário mudar o ponteiro do próximo elemento.
        if (no_proximo != NULL)
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

    if (lista_auxiliar->destruir_info != NULL)
        lista_auxiliar->destruir_info(no_auxiliar->info);
    free(no_auxiliar);
    lista_auxiliar->tamanho--;
}

ListaNo lista_buscar(Lista lista, const char id[100]) {
    ListaImp *lista_auxiliar = (ListaImp *) lista;
    if (lista_auxiliar->obter_identificador_info == NULL) {
        LOG_ERRO(
            "Não é possível buscar em uma lista que não possui a função obter_identificador_info "
            "definida!\n");
        return NULL;
    }

    NoImp *no_auxiliar = lista_auxiliar->primeiro;
    while (no_auxiliar != NULL) {
        const char *id_atual = lista_auxiliar->obter_identificador_info(no_auxiliar->info);

        if (strcmp(id_atual, id) == 0) {
            return no_auxiliar;
        }

        no_auxiliar = no_auxiliar->proximo;
    }
    return NULL;
}

ListaNo lista_obter_primeiro(Lista lista) {
    ListaImp *lista_aux = (ListaImp *) lista;
    if (lista_aux->tamanho == 0)
        return NULL;
    return lista_aux->primeiro;
}

ListaNo lista_obter_ultimo(Lista lista) {
    ListaImp *lista_aux = (ListaImp *) lista;
    if (lista_aux->tamanho == 0) {
        return NULL;
    }
    return lista_aux->ultimo;
}

ListaInfo lista_obter_info(ListaNo p) {
    if (p == NULL) {
        LOG_ERRO("Nó nulo passado para lista_obter_info!\n");
        return NULL;
    }
    NoImp *no_auxiliar = (NoImp *) p;
    return no_auxiliar->info;
}

void lista_definir_info(ListaNo p, ListaInfo info) {
    if (p == NULL) {
        LOG_ERRO("Nó nulo passado para lista_definir_info!\n");
        return;
    }
    if (info == NULL) {
        LOG_ERRO("Informação nula passada para lista_definir_info!\n");
        return;
    }

    NoImp *no_auxiliar = (NoImp *) p;
    no_auxiliar->info = info;
}

ListaNo lista_obter_proximo(ListaNo p) {
    if (p == NULL) {
        LOG_ERRO("Nó nulo passado para lista_obter_proximo!\n");
        return NULL;
    }
    NoImp *no_aux = (NoImp *) p;
    return no_aux->proximo;
}

ListaNo lista_obter_anterior(ListaNo p) {
    if (p == NULL) {
        LOG_ERRO("Nó nulo passado para lista_obter_anterior!\n");
        return NULL;
    }
    NoImp *no_aux = (NoImp *) p;
    return no_aux->anterior;
}

// Troca as informações armazenadas em dois nós.
void lista_trocar_info(ListaNo no1, ListaNo no2) {
    ListaInfo temp = lista_obter_info(no1);
    lista_definir_info(no1, lista_obter_info(no2));
    lista_definir_info(no2, temp);
}

void lista_destruir(Lista lista) {
    ListaImp *lista_aux = (ListaImp *) lista;
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
