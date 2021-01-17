#include "tabelahash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Utils/logging.h"
#include "lista.h"

// Elemento armazenado nas listas da tabela.
typedef struct {
    const char *chave;
    TabelaInfo info;
} Elemento;

typedef struct tabela {
    DestruirInfo *destruir_info;
    Lista **inf;
    int size;
} TabelaImp;

Tabela tabela_criar(int size, DestruirInfo destruir_info) {
    TabelaImp *tabelaImp = malloc(sizeof(TabelaImp));
    if (tabelaImp == NULL) {
        LOG_ERRO("Erro ao alocar espaço para uma nova tabela de espalhamento!\n");
        return NULL;
    }
    tabelaImp->size = size;
    tabelaImp->destruir_info = destruir_info;
    tabelaImp->inf = malloc(size * sizeof(Lista *));

    for (int i = 0; i < size; i++) {
        tabelaImp->inf[i] = NULL;
    }
    return tabelaImp;
}

// Funções de hashing
int chaveDivisao(int chave, int table_size) {
    return (chave & 0x7FFFFFFF) % table_size;
}

// Função utilizada para transformar uma string em numero
int chaveString(const char *id) {
    int numero = 3;
    int tamanho = strlen(id);
    for (int i = 0; i < tamanho; i++) {
        numero = 17 * numero + (int) id[i];
    }
    return numero;
}

// Recebe um elemento e extrai sua chave.
const char *extrair_chave_elemento(ListaInfo elemento) {
    Elemento *el = elemento;
    return el->chave;
}

void tabela_inserir(Tabela tabela, TabelaInfo info, const char *id) {
    if (tabela == NULL) {
        LOG_ERRO("Tabela de espalhamento nula passada para tabela_inserir!\n");
        return;
    } else if (info == NULL) {
        LOG_INFO("Informação nula passada para tabela_inserir!\n");
        return;
    } else if (id == NULL) {
        LOG_INFO("Id nulo passado para tabela_inserir!\n");
        return;
    }
    TabelaImp *tabelaImp = tabela;

    int chave = chaveString(id);
    int pos = chaveDivisao(chave, tabelaImp->size);

    if (tabelaImp->inf[pos] == NULL) {
        // Cria uma lista com uma função específica para extrair a chave de um elemento da tabela.
        tabelaImp->inf[pos] = lista_criar(extrair_chave_elemento, NULL);
    }

    Elemento *novo_elemento = malloc(sizeof *novo_elemento);
    novo_elemento->chave = id;
    novo_elemento->info = info;
    lista_inserir_final(tabelaImp->inf[pos], novo_elemento);
}

TabelaInfo tabela_buscar(Tabela tabela, const char *id) {
    if (tabela == NULL) {
        LOG_ERRO("Tabela de espalhamento nula passada para tabela_buscar!\n");
        return NULL;
    }
    if (id == NULL) {
        LOG_ERRO("Id nulo passado para tabela_buscar!\n");
        return NULL;
    }
    TabelaImp *tabelaImp = tabela;
    int chave = chaveString(id);
    int pos = chaveDivisao(chave, tabelaImp->size);

    if (tabelaImp->inf[pos] == NULL)
        return NULL;
    // Retorna o nó da lista encontrado no indice "pos" da tabela.
    ListaNo no = lista_buscar(tabelaImp->inf[pos], id);
    if (no == NULL)
        return NULL;

    Elemento *elemento = lista_obter_info(no);
    return elemento->info;
}

void tabela_destruir(Tabela tabela) {
    TabelaImp *tabelaImp = tabela;

    for (int i = 0; i < tabelaImp->size; i++) {
        Lista atual = tabelaImp->inf[i];
        if (atual == NULL)
            continue;
        // Libera as chaves e informações armazenadas na lista.
        for (ListaNo j = lista_obter_primeiro(atual); j != NULL; j = lista_obter_proximo(j)) {
            Elemento *el = lista_obter_info(j);
            tabelaImp->destruir_info(el->info);
            free(el);
        }
        lista_destruir(atual);
    }
    free(tabelaImp->inf);
    free(tabelaImp);
}
