#include "grafo.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../Utils/logging.h"
#include "./lista.h"
#include "./tabelahash.h"

struct Aresta_s {
    char nome[100];
    char origem[100];
    char destino[100];
    char quadra_esquerda[100];
    char quadra_direita[100];
    double comprimento;
    double velocidade;
};

struct Vertice_s {
    char id[100];
    double x;
    double y;
    Lista arestas;
};

struct Grafo_s {
    int tamanho_atual;
    int tamanho_maximo;
    Vertice *vertices;
    Tabela id_indice;
};

Grafo grafo_criar(int tamanho_maximo) {
    if (tamanho_maximo <= 0) {
        LOG_ERRO("Tamanho do grafo deve ser maior que 0!\n");
        return NULL;
    }
    Grafo grafo = malloc(sizeof *grafo);
    if (grafo == NULL) {
        LOG_ERRO("Falha ao alocar espaço para um novo grafo!\n");
        return NULL;
    }
    grafo->vertices = malloc(sizeof *grafo->vertices * tamanho_maximo);
    if (grafo->vertices == NULL) {
        free(grafo);
        LOG_ERRO("Falha ao alocar espaço para um novo grafo!\n");
        return NULL;
    }

    grafo->tamanho_atual = 0;
    grafo->tamanho_maximo = tamanho_maximo;
    grafo->id_indice = tabela_criar(free);
    for (int i = 0; i < tamanho_maximo; i++)
        grafo->vertices[i] = NULL;
    return grafo;
}

Vertice grafo_inserir_vertice(Grafo grafo, const char *id, double x, double y) {
    if (grafo->tamanho_atual >= grafo->tamanho_maximo) {
        LOG_ERRO("Tamanho máximo do grafo alcançado!\n");
        return NULL;
    }
    Vertice vertice = malloc(sizeof *vertice);
    if (vertice == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        return NULL;
    }
    strcpy(vertice->id, id);
    vertice->x = x;
    vertice->y = y;
    vertice->arestas =
        lista_criar((ObterIdentificadorLista *) aresta_obter_destino, (ListaDestruirInfo *) free);

    int *indice = malloc(sizeof *indice);
    if (indice == NULL) {
        vertice_destruir(vertice);
        LOG_ERRO("Falha ao alocar memória!\n");
        return NULL;
    }

    *indice = grafo->tamanho_atual;
    grafo->vertices[*indice] = vertice;
    tabela_inserir(grafo->id_indice, id, indice);
    grafo->tamanho_atual++;

    return vertice;
}

Vertice grafo_remover_vertice(Grafo grafo, const char *id) {
    if (id == NULL) {
        LOG_AVISO("Valor nulo passado para grafo_remover_vertice\n");
        return NULL;
    }

    int *indice_id = tabela_remover(grafo->id_indice, id);

    Vertice vertice = grafo->vertices[*indice_id];
    grafo->vertices[*indice_id] = NULL;

    free(indice_id);
    grafo->tamanho_atual--;
    return vertice;
}

void grafo_inserir_aresta(Grafo grafo, const char *origem, const char *destino,
                          const char *quadra_esquerda, const char *quadra_direita,
                          double comprimento, double velocidade, const char *nome) {
    if (origem == NULL || destino == NULL || quadra_esquerda == NULL || quadra_direita == NULL ||
        nome == NULL) {
        LOG_AVISO("Valor nulo passado para grafo_inserir_aresta\n");
        return;
    }

    const int *indice_origem = tabela_buscar(grafo->id_indice, origem);
    if (indice_origem == NULL) {
        LOG_AVISO("Não é possível inserir uma aresta que não tem origem válida\n");
        return;
    }

    Aresta aresta = malloc(sizeof *aresta);
    if (aresta == NULL) {
        LOG_ERRO("Erro ao alocar memória!\n");
        return;
    }
    strcpy(aresta->nome, nome);
    strcpy(aresta->origem, origem);
    strcpy(aresta->destino, destino);
    strcpy(aresta->quadra_direita, quadra_direita);
    strcpy(aresta->quadra_esquerda, quadra_esquerda);
    aresta->comprimento = comprimento;
    aresta->velocidade = velocidade;

    lista_inserir_final(grafo->vertices[*indice_origem]->arestas, aresta);
}

void grafo_remover_aresta(Grafo grafo, const char *origem, const char *destino) {
    if (origem == NULL) {
        LOG_AVISO("Valor nulo passado para grafo_remover_aresta\n");
        return;
    }

    const int *indice_origem = tabela_buscar(grafo->id_indice, origem);
    if (indice_origem == NULL) {
        LOG_AVISO("Não é possível remover uma aresta que não tem origem válida\n");
        return;
    }

    Lista arestas = grafo->vertices[*indice_origem]->arestas;
    ListaNo no = lista_buscar(arestas, destino);
    if (no != NULL) {
        ListaInfo aresta = lista_remover(arestas, no);
        free(aresta);
    }
}

// Verifica se o vértice id1 é adjacente ao vértice id2
bool grafo_checar_adjacente(Grafo grafo, const char *id1, const char *id2) {
    if (id1 == NULL || id2 == NULL) {
        LOG_AVISO("Valor nulo passado para grafo_checar_adjacente\n");
        return false;
    }

    const int *indice_origem = tabela_buscar(grafo->id_indice, id1);
    if (indice_origem == NULL) {
        LOG_AVISO("Não é possível checar a adjacência de um vértice que tem origem inválida\n");
        return false;
    }

    Lista arestas = grafo->vertices[*indice_origem]->arestas;
    ListaNo no = lista_buscar(arestas, id2);
    return no != NULL;
}

// Retorna os vértices adjacentes ao vértice id.
Lista grafo_obter_adjacentes(Grafo grafo, const char *id) {
    Lista adjacentes = lista_criar((ObterIdentificadorLista *) vertice_obter_id, NULL);
    if (id == NULL) {
        LOG_AVISO("Valor nulo passado para grafo_obter_adjacentes\n");
        return adjacentes;
    }

    const int *indice_origem = tabela_buscar(grafo->id_indice, id);
    if (indice_origem == NULL) {
        LOG_AVISO("Não é possível buscar os vértices adjacentes a uma origem inválida\n");
        return adjacentes;
    }

    Lista arestas = grafo->vertices[*indice_origem]->arestas;
    for_each_lista(no, arestas) {
        Aresta aresta = lista_obter_info(no);
        const int *indice_adjacente = tabela_buscar(grafo->id_indice, aresta->destino);
        if (indice_origem == NULL) {
            LOG_AVISO("Vértice do grafo tem destino inválido!\n");
            continue;
        }
        lista_inserir_final(adjacentes, grafo->vertices[*indice_adjacente]);
    }

    return adjacentes;
}

const char *aresta_obter_nome(Aresta aresta) {
    return aresta->nome;
}

const char *aresta_obter_origem(Aresta aresta) {
    return aresta->origem;
}

const char *aresta_obter_destino(Aresta aresta) {
    return aresta->destino;
}

const char *aresta_obter_quadra_esquerda(Aresta aresta) {
    return aresta->quadra_esquerda;
}

const char *aresta_obter_quadra_direita(Aresta aresta) {
    return aresta->quadra_direita;
}

double aresta_obter_comprimento(Aresta aresta) {
    return aresta->comprimento;
}

double aresta_obter_velocidade(Aresta aresta) {
    return aresta->velocidade;
}

const char *vertice_obter_id(Vertice vertice) {
    return vertice->id;
}

double vertice_obter_x(Vertice vertice) {
    return vertice->x;
}

double vertice_obter_y(Vertice vertice) {
    return vertice->y;
}

Lista vertice_obter_arestas(Vertice vertice) {
    return vertice->arestas;
}

void vertice_destruir(Vertice vertice) {
    if (vertice == NULL)
        return;
    lista_destruir(vertice->arestas);
    free(vertice);
}

void grafo_destruir(Grafo grafo) {
    if (grafo == NULL)
        return;
    for (int i = 0; i < grafo->tamanho_maximo; i++)
        vertice_destruir(grafo->vertices[i]);
    tabela_destruir(grafo->id_indice);
    free(grafo->vertices);
    free(grafo);
}
