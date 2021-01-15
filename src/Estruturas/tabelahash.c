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
    TabelaImp *tabelaImp = (TabelaImp *) malloc(sizeof(TabelaImp));
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

// funções de hashing
int chaveDivisao(int chave, int Table_size) {
    return (chave & 0x7FFFFFFF) % Table_size;
}

// função utilizada para transformar uma string em numero
int chaveString(const char id[100]) {
    int numero = 3;
    int tamanho = strlen(id);
    for (int i = 0; i < tamanho; i++) {
        numero = 17 * numero + (int) id[i];
    }
    return numero;
}

void tabela_inserir(Tabela tabela, ListaInfo info, const char id[100]) {
    TabelaImp *tabelaImp = (TabelaImp *) tabela;
    InfoImp *infoImp = (InfoImp *) malloc(sizeof(InfoImp));

    if (tabelaImp == NULL) {
        LOG_ERRO("Tabela de espalhamento nula passada para tabela_inserir!\n");
        return;
    } else if (info == NULL) {
        LOG_INFO("Informação nula passada para tabela_inserir!\n");
        return;
    }

    int chave = chaveString(id);
    int pos = chaveDivisao(chave, tabelaImp->size);

    if (tabelaImp->inf[pos] == NULL) {
        // cria uma lista com o identificador especificado na criação da tabela
        infoImp->lista = lista_criar(tabelaImp->obter_identificador_info, tabelaImp->destruir_info);
        tabelaImp->inf[pos] = infoImp;
        lista_inserir_final(tabelaImp->inf[pos]->lista, info);
    } else {
        lista_inserir_final(tabelaImp->inf[pos]->lista, info);
    }
    return;
}

ListaNo tabela_buscar(Tabela tabela, const char id[100]) {
    TabelaImp *tabelaImp = (TabelaImp *) tabela;

    if (tabelaImp == NULL) {
        LOG_ERRO("Tabela de espalhamento nula passada para tabela_buscar!\n");
        return NULL;
    }
    int chave = chaveString(id);
    int pos = chaveDivisao(chave, tabelaImp->size);

    // retorna o nó da lista encontrado no indice "pos" da tabela
    return lista_buscar(tabelaImp->inf[pos]->lista, id);
}

void tabela_destruir(Tabela tabela) {
    TabelaImp *tabelaImp = (TabelaImp *) tabela;

    for (int i = 0; i < tabelaImp->size; i++) {
        if (tabelaImp->inf[i] != NULL) {
            lista_destruir(tabelaImp->inf[i]->lista);
            free(tabelaImp->inf[i]);
        }
    }
    free(tabelaImp);
}
