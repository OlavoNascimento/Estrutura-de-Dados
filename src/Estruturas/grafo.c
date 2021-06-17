#include "grafo.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../Utils/logging.h"
#include "./Interfaces/figura.h"
#include "./Objetos/Formas/circulo.h"
#include "./Objetos/Formas/linha.h"
#include "./Objetos/Outros/texto.h"
#include "./lista.h"
#include "./tabelahash.h"

#define TAMANHO_INICIAL 16

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

// Remover tamanho do construtor e aumentar o tamanho conforme necessário.
Grafo grafo_criar() {
    Grafo grafo = malloc(sizeof *grafo);
    if (grafo == NULL) {
        LOG_ERRO("Falha ao alocar espaço para um grafo!\n");
        return NULL;
    }
    grafo->tamanho_atual = 0;
    grafo->tamanho_maximo = TAMANHO_INICIAL;

    grafo->vertices = malloc(sizeof *grafo->vertices * grafo->tamanho_maximo);
    if (grafo->vertices == NULL) {
        free(grafo);
        LOG_ERRO("Falha ao alocar espaço para um grafo!\n");
        return NULL;
    }
    for (int i = 0; i < grafo->tamanho_maximo; i++)
        grafo->vertices[i] = NULL;

    grafo->id_indice = tabela_criar(free);
    if (grafo->id_indice == NULL) {
        free(grafo->vertices);
        free(grafo);
        LOG_ERRO("Falha ao alocar espaço para um grafo!\n");
        return NULL;
    }
    return grafo;
}

// Dobra o tamanho do array de vértices de um grafo.
bool aumentar_tamanho_grafo(Grafo grafo) {
    Vertice *temp = realloc(grafo->vertices, sizeof *temp * 2 * grafo->tamanho_maximo);
    if (temp == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        return false;
    }

    grafo->vertices = temp;
    grafo->tamanho_maximo *= 2;
    for (int i = grafo->tamanho_atual; i < grafo->tamanho_maximo; i++)
        grafo->vertices[i] = NULL;

    return true;
}

Vertice grafo_inserir_vertice(Grafo grafo, const char *id, double x, double y) {
    if (grafo->tamanho_atual >= grafo->tamanho_maximo && !aumentar_tamanho_grafo(grafo)) {
        LOG_AVISO("Falha ao adicionar vértice a um grafo!\n");
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
    if (vertice->arestas == NULL) {
        free(vertice->id);
        free(vertice);
        LOG_ERRO("Falha ao alocar memória!\n");
        return NULL;
    }

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

    int *indice_atual = tabela_remover(grafo->id_indice, id);
    if (indice_atual == NULL) {
        LOG_AVISO("Não é possível remover um vértice que não existe no grafo\n");
        return NULL;
    }
    const Vertice vertice_removido = grafo->vertices[*indice_atual];

    int ultimo_indice = grafo->tamanho_atual - 1;
    if (ultimo_indice == *indice_atual)
        ultimo_indice--;

    if (ultimo_indice > 0) {
        // Move o último vértice para a posição atual.
        const Vertice ultimo_vertice = grafo->vertices[ultimo_indice];
        int *indice_removido = tabela_remover(grafo->id_indice, vertice_obter_id(ultimo_vertice));
        grafo->vertices[*indice_removido] = NULL;
        free(indice_removido);

        grafo->vertices[*indice_atual] = ultimo_vertice;
        tabela_inserir(grafo->id_indice, vertice_obter_id(ultimo_vertice), indice_atual);
    } else {
        free(indice_atual);
    }
    grafo->tamanho_atual--;

    return vertice_removido;
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

// Retorna o número de vértices de um grafo.
int grafo_obter_tamanho(Grafo grafo) {
    return grafo->tamanho_atual;
}

// Retorna o vértice de id especificado.
Vertice grafo_obter_vertice_por_id(Grafo grafo, const char *id) {
    if (id == NULL) {
        LOG_AVISO("Id nulo passado para grafo_obter_vertice_por_id\n");
        return NULL;
    }

    const int *indice_origem = tabela_buscar(grafo->id_indice, id);
    if (indice_origem == NULL)
        return NULL;
    return grafo->vertices[*indice_origem];
}

// Retorna o vértice de id indice.
Vertice grafo_obter_vertice_por_indice(Grafo grafo, int indice) {
    if (indice >= grafo->tamanho_maximo) {
        LOG_AVISO("Tentando acessar indice inválido do grafo!\n");
        return NULL;
    }
    return grafo->vertices[indice];
}

// Retorna um array contendo todas as arestas do grafo.
Aresta *grafo_obter_arestas(Grafo grafo, int *tamanho_vetor) {
    Aresta *arestas_grafo = NULL;
    int indice_atual = 0;
    int num_arestas_grafo = 0;

    for (int i = 0; i < grafo->tamanho_atual; i++) {
        Lista arestas = grafo->vertices[i]->arestas;
        if (lista_obter_tamanho(arestas) == 0)
            continue;

        // Aumenta o tamanho do vetor
        num_arestas_grafo += lista_obter_tamanho(arestas);
        Aresta *temp = realloc(arestas_grafo, sizeof *temp * num_arestas_grafo);
        if (temp == NULL) {
            LOG_ERRO("Erro ao alocar memória!\n");
            free(arestas_grafo);
            return NULL;
        }
        arestas_grafo = temp;

        for_each_lista(no, arestas) {
            arestas_grafo[indice_atual] = lista_obter_info(no);
            indice_atual++;
        }
    }
    *tamanho_vetor = num_arestas_grafo;
    return arestas_grafo;
}

// Retorna o indice de um vértice de id especificado.
const int *grafo_obter_indice_vertice(Grafo grafo, const char *id) {
    if (id == NULL) {
        LOG_AVISO("Id nulo passado para grafo_obter_indice_vertice\n");
        return NULL;
    }
    const int *indice_origem = tabela_buscar(grafo->id_indice, id);
    if (indice_origem == NULL)
        return NULL;
    return indice_origem;
}

// Cria uma representação de um grafo.
Lista grafo_escrever_svg(Grafo grafo, Grafo mst) {
    Lista saida = lista_criar(NULL, figura_destruir);
    if (saida == NULL)
        return NULL;
    Lista vertices_formas = lista_criar(NULL, NULL);
    Lista arestas_mst = lista_criar(NULL, NULL);

    for (int i = 0; i < grafo->tamanho_atual; i++) {
        Vertice origem = grafo->vertices[i];
        const char *id_origem = vertice_obter_id(origem);
        if (origem == NULL)
            continue;

        Lista arestas = vertice_obter_arestas(origem);
        for_each_lista(no, arestas) {
            Aresta aresta = lista_obter_info(no);

            const char *id_destino = aresta_obter_destino(aresta);
            Vertice destino = grafo_obter_vertice_por_id(grafo, id_destino);
            if (destino == NULL)
                continue;

            char cor_linha[6] = "black";

            // Se o vértice de origem e destino estão conectados na árvore geradora mínima a aresta
            // atual também faz parte da árvore.
            const bool adjacente = grafo_checar_adjacente(mst, id_origem, id_destino);
            if (adjacente)
                strncpy(cor_linha, "red", 6);

            Linha linha =
                linha_criar(vertice_obter_x(origem), vertice_obter_y(origem),
                            vertice_obter_x(destino), vertice_obter_y(destino), cor_linha);

            // Garante que as arestas da mst fiquem por cima das arestas normais.
            if (adjacente) {
                linha_definir_espessura(linha, 2);
                lista_inserir_final(arestas_mst, linha);
            } else {
                lista_inserir_final(saida, linha);
            }
        }

        Circulo circ = circulo_criar("", 10, vertice_obter_x(origem), vertice_obter_y(origem),
                                     "black", "green");
        lista_inserir_final(vertices_formas, circ);
    }

    for_each_lista(no, arestas_mst) {
        lista_inserir_final(saida, lista_obter_info(no));
    }
    lista_destruir(arestas_mst);

    for_each_lista(no, vertices_formas) {
        lista_inserir_final(saida, lista_obter_info(no));
    }
    lista_destruir(vertices_formas);

    return saida;
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
