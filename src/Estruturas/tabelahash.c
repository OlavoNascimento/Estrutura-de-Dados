#include "tabelahash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Utils/logging.h"
#include "lista.h"

#define TAMANHO_INICIAL 16
// Porcentagem do tamanho atual que causa o rehash da tabela.
#define PORCENTAGEM_REHASH 0.75

// Elemento armazenado nas listas da tabela.
struct Elemento {
    char chave[100];
    TabelaInfo info;
};

struct Tabela_s {
    TabelaDestruirInfo *destruir_info;
    Lista *inf;
    int baldes_ocupados;
    int tamanho;
};

Tabela tabela_criar(TabelaDestruirInfo destruir_info) {
    Tabela tabela = malloc(sizeof *tabela);
    if (tabela == NULL) {
        LOG_ERRO("Falha ao alocar espaço para uma Hashmap!\n");
        return NULL;
    }
    tabela->destruir_info = destruir_info;
    tabela->tamanho = TAMANHO_INICIAL;
    tabela->baldes_ocupados = 0;

    tabela->inf = malloc(sizeof *tabela->inf * TAMANHO_INICIAL);
    if (tabela->inf == NULL) {
        LOG_ERRO("Falha ao alocar memória para baldes de um Hashmap!\n");
        free(tabela);
        return NULL;
    }

    for (int i = 0; i < tabela->tamanho; i++)
        tabela->inf[i] = NULL;

    return tabela;
}

// Função utilizada para transformar uma string em número.
int chave_string(const char id[100], int tamanho_tabela) {
    int soma = 3;
    int tamanho = strlen(id);
    for (int i = 0; i < tamanho; i++) {
        soma = 17 * soma + (int) id[i];
    }
    // Retorna o valor absoluto da soma sem ultrapassar o tamanho da tabela.
    return (soma & 0x7FFFFFFF) % tamanho_tabela;
}

// Recebe um elemento e extrai sua chave.
const char *extrair_chave_elemento(ListaInfo elemento) {
    struct Elemento *el = elemento;
    return el->chave;
}

// Dobra o número de baldes da tabela para diminuir o número de colisões.
void rehash(Tabela tabela) {
    // Novo array de baldes é o dobro do anterior.
    Lista *baldes_novos = malloc(sizeof *baldes_novos * 2 * tabela->tamanho);
    if (baldes_novos == NULL) {
        LOG_ERRO("Falha ao alocar espaço para baldes de um Hashmap!\n");
        return;
    }

    Lista *baldes_antigos = tabela->inf;
    tabela->inf = baldes_novos;
    tabela->baldes_ocupados = 0;
    tabela->tamanho *= 2;

    // Inicia todos os novos baldes como NULL.
    for (int i = 0; i < tabela->tamanho; i++)
        tabela->inf[i] = NULL;

    // Re-insere os elementos dos baldes antigos nos baldes novos.
    for (int i = 0; i < tabela->tamanho / 2; i++) {
        Lista atual = baldes_antigos[i];
        if (atual == NULL)
            continue;

        ListaNo no_atual = lista_obter_inicio(atual);
        while (no_atual != NULL) {
            ListaNo no_proximo = lista_obter_proximo(no_atual);

            struct Elemento *el = lista_obter_info(no_atual);
            tabela_inserir(tabela, el->chave, el->info);
            free(el);

            free(no_atual);
            no_atual = no_proximo;
        }
        free(atual);
    }
    free(baldes_antigos);
}

// Insere uma informação na tabela que pode ser acessada através do id fornecido.
void tabela_inserir(Tabela tabela, const char id[100], TabelaInfo info) {
    if (id == NULL) {
        LOG_AVISO("Id nulo passado para tabela_inserir!\n");
        return;
    }

    int pos = chave_string(id, tabela->tamanho);
    if (tabela->inf[pos] == NULL) {
        // Cria uma lista com uma função específica para extrair a chave de um elemento da tabela.
        tabela->inf[pos] = lista_criar(extrair_chave_elemento, NULL);
    }

    struct Elemento *novo_elemento = malloc(sizeof *novo_elemento);
    if (novo_elemento == NULL) {
        LOG_ERRO("Falha ao alocar memória\n");
        return;
    }
    strcpy(novo_elemento->chave, id);
    novo_elemento->info = info;
    lista_inserir_final(tabela->inf[pos], novo_elemento);

    tabela->baldes_ocupados++;
    // Dobra os baldes caso a porcentagem de ocupação da tabela tenha ultrapassado o limite
    // especificado.
    if (tabela->baldes_ocupados >= (int) (tabela->tamanho * PORCENTAGEM_REHASH))
        rehash(tabela);
}

TabelaInfo tabela_buscar(Tabela tabela, const char id[100]) {
    if (tabela == NULL) {
        LOG_AVISO("Tabela de espalhamento nula passada para tabela_buscar!\n");
        return NULL;
    }
    if (id == NULL) {
        LOG_AVISO("Id nulo passado para tabela_buscar!\n");
        return NULL;
    }

    int pos = chave_string(id, tabela->tamanho);
    if (tabela->inf[pos] == NULL)
        return NULL;

    // Retorna o nó da lista encontrado no indice "pos" da tabela.
    ListaNo no = lista_buscar(tabela->inf[pos], id);
    if (no == NULL)
        return NULL;

    struct Elemento *elemento = lista_obter_info(no);
    return elemento->info;
}

TabelaInfo tabela_remover(Tabela tabela, const char id[100]) {
    if (id == NULL) {
        LOG_AVISO("Id nulo passado para tabela_remover!\n");
        return NULL;
    }

    int pos = chave_string(id, tabela->tamanho);
    if (tabela->inf[pos] == NULL)
        return NULL;

    // Retorna o nó da lista encontrado no indice "pos" da tabela.
    ListaNo no = lista_buscar(tabela->inf[pos], id);
    if (no == NULL)
        return NULL;

    struct Elemento *elemento = lista_obter_info(no);
    TabelaInfo info = elemento->info;

    free(elemento);
    lista_remover(tabela->inf[pos], no);
    if (lista_obter_tamanho(tabela->inf[pos]) == 0) {
        lista_destruir(tabela->inf[pos]);
        tabela->inf[pos] = NULL;
        tabela->baldes_ocupados--;
    }

    return info;
}

void tabela_destruir(Tabela tabela) {
    if (tabela == NULL)
        return;
    for (int i = 0; i < tabela->tamanho; i++) {
        Lista atual = tabela->inf[i];
        if (atual == NULL)
            continue;

        // Libera as chaves e informações armazenadas na lista.
        ListaNo no_atual = lista_obter_inicio(atual);
        while (no_atual != NULL) {
            ListaNo no_proximo = lista_obter_proximo(no_atual);

            struct Elemento *el = lista_obter_info(no_atual);
            if (tabela->destruir_info != NULL)
                tabela->destruir_info(el->info);
            free(el);

            free(no_atual);
            no_atual = no_proximo;
        }
        free(atual);
    }
    free(tabela->inf);
    free(tabela);
}
