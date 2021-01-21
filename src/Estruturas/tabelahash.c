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
typedef struct {
    const char *chave;
    TabelaInfo info;
} Elemento;

typedef struct tabela {
    DestruirInfo *destruir_info;
    Lista *inf;
    int baldes_ocupados;
    int tamanho;
} TabelaImp;

Tabela tabela_criar(DestruirInfo destruir_info) {
    TabelaImp *tabelaImp = malloc(sizeof(TabelaImp));
    if (tabelaImp == NULL) {
        LOG_ERRO("Erro ao alocar espaço para uma nova tabela de espalhamento!\n");
        return NULL;
    }
    tabelaImp->destruir_info = destruir_info;
    tabelaImp->inf = malloc(TAMANHO_INICIAL * sizeof(Lista *));
    tabelaImp->tamanho = TAMANHO_INICIAL;
    tabelaImp->baldes_ocupados = 0;

    for (int i = 0; i < tabelaImp->tamanho; i++) {
        tabelaImp->inf[i] = NULL;
    }
    return tabelaImp;
}

// Funções de hashing
int chave_divisao(int chave, int tamanho_tabela) {
    return (chave & 0x7FFFFFFF) % tamanho_tabela;
}

// Função utilizada para transformar uma string em numero
int chave_string(const char *id) {
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

// Dobra o número de baldes da tabela para diminuir o número de colisões.
void rehash(TabelaImp *tabela) {
    tabela->baldes_ocupados = 0;
    tabela->tamanho *= 2;

    Lista *baldes_antigos = tabela->inf;
    // Novo array de baldes é o dobro do anterior.
    tabela->inf = malloc(tabela->tamanho * sizeof(Lista *));

    // Inicia todos os novos baldes como NULL.
    for (int i = 0; i < tabela->tamanho; i++)
        tabela->inf[i] = NULL;

    // Re-insere os elementos dos baldes antigos nos baldes novos.
    for (int i = 0; i < tabela->tamanho / 2; i++) {
        Lista atual = baldes_antigos[i];
        if (atual == NULL)
            continue;
        for (ListaNo j = lista_obter_primeiro(atual); j != NULL; j = lista_obter_proximo(j)) {
            Elemento *el = lista_obter_info(j);
            tabela_inserir(tabela, el->chave, el->info);
            free(el);
        }
        lista_destruir(atual);
    }
    free(baldes_antigos);
}

// Insere uma informação na tabela que pode ser acessada através do id fornecido.
void tabela_inserir(Tabela tabela, const char *id, TabelaInfo info) {
    if (tabela == NULL) {
        LOG_ERRO("Tabela de espalhamento nula passada para tabela_inserir!\n");
        return;
    } else if (id == NULL) {
        LOG_INFO("Id nulo passado para tabela_inserir!\n");
        return;
    } else if (info == NULL) {
        LOG_INFO("Informação nula passada para tabela_inserir!\n");
        return;
    }
    TabelaImp *tabelaImp = tabela;

    int chave = chave_string(id);
    int pos = chave_divisao(chave, tabelaImp->tamanho);

    if (tabelaImp->inf[pos] == NULL) {
        // Cria uma lista com uma função específica para extrair a chave de um elemento da tabela.
        tabelaImp->inf[pos] = lista_criar(extrair_chave_elemento, NULL);
    }

    Elemento *novo_elemento = malloc(sizeof *novo_elemento);
    novo_elemento->chave = id;
    novo_elemento->info = info;
    lista_inserir_final(tabelaImp->inf[pos], novo_elemento);

    tabelaImp->baldes_ocupados++;
    // Dobra os baldes caso a porcentagem de ocupação da tabela tenha ultrapassado o limite
    // especificado.
    if (tabelaImp->baldes_ocupados == (int) (tabelaImp->tamanho * PORCENTAGEM_REHASH))
        rehash(tabelaImp);
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
    int chave = chave_string(id);
    int pos = chave_divisao(chave, tabelaImp->tamanho);

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

    for (int i = 0; i < tabelaImp->tamanho; i++) {
        Lista atual = tabelaImp->inf[i];
        if (atual == NULL)
            continue;
        // Libera as chaves e informações armazenadas na lista.
        for (ListaNo j = lista_obter_primeiro(atual); j != NULL; j = lista_obter_proximo(j)) {
            Elemento *el = lista_obter_info(j);
            if (tabelaImp->destruir_info != NULL)
                tabelaImp->destruir_info(el->info);
            free(el);
        }
        lista_destruir(atual);
    }
    free(tabelaImp->inf);
    free(tabelaImp);
}
