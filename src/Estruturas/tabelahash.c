#include "tabelahash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Utils/logging.h"
#include "lista.h"

typedef struct infos {
    Lista lista;
} InfoImp;

typedef struct tabela {
    ObterIdentificadorInfo *obter_identificador_info;
    DestruirInfo *destruir_info;
    InfoImp **inf;
    int size;
} TabelaImp;

Tabela tabela_criar(int size, ObterIdentificadorInfo obter_identificador_info,
                    DestruirInfo destruir_info) {
    TabelaImp *tabelaImp = malloc(sizeof(TabelaImp));
    if (tabelaImp == NULL) {
        LOG_ERRO("Erro ao alocar espaço para uma nova tabela de espalhamento!\n");
        return NULL;
    }
    tabelaImp->size = size;
    tabelaImp->destruir_info = destruir_info;
    tabelaImp->obter_identificador_info = obter_identificador_info;
    tabelaImp->inf = (InfoImp **) malloc(size * sizeof(InfoImp *));

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

void tabela_inserir(Tabela tabela, ListaInfo info, const char *id) {
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
    InfoImp *infoImp = malloc(sizeof(InfoImp));

    int chave = chaveString(id);
    int pos = chaveDivisao(chave, tabelaImp->size);

    if (tabelaImp->inf[pos] == NULL) {
        // Cria uma lista com o identificador especificado na criação da tabela.
        infoImp->lista = lista_criar(tabelaImp->obter_identificador_info, tabelaImp->destruir_info);
        tabelaImp->inf[pos] = infoImp;
    }
    lista_inserir_final(tabelaImp->inf[pos]->lista, info);
    return;
}

ListaNo tabela_buscar(Tabela tabela, const char *id) {
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

    // Retorna o nó da lista encontrado no indice "pos" da tabela.
    return lista_buscar(tabelaImp->inf[pos]->lista, id);
}

void tabela_destruir(Tabela tabela) {
    TabelaImp *tabelaImp = tabela;

    for (int i = 0; i < tabelaImp->size; i++) {
        if (tabelaImp->inf[i] != NULL) {
            lista_destruir(tabelaImp->inf[i]->lista);
            free(tabelaImp->inf[i]);
        }
    }
    free(tabelaImp);
}
