#include <stdlib.h>
#include <string.h>

#include "../Estruturas/grafo.h"
#include "./logging.h"

struct Uniao {
    int rank;
    int raiz;
};

// Checa se dois elementos tem a mesma raiz em um grupo.
int encontrar_raiz(struct Uniao *elementos, int x) {
    if (elementos[x].raiz == x)
        return x;
    // Compacta o caminho.
    elementos[x].raiz = encontrar_raiz(elementos, elementos[x].raiz);
    return elementos[x].raiz;
}

// Coloca dois elementos em um mesmo grupo.
void unir(struct Uniao *elementos, int x, int y) {
    int x_raiz = encontrar_raiz(elementos, x);
    int y_raiz = encontrar_raiz(elementos, y);

    // Já estão unidos.
    if (x_raiz == y_raiz)
        return;

    // Raiz do maior grupo passa a ser a raiz do menor grupo.
    if (elementos[y_raiz].rank > elementos[x_raiz].rank) {
        elementos[y_raiz].raiz = x_raiz;
    } else {
        elementos[x_raiz].raiz = y_raiz;
        if (elementos[x_raiz].rank == elementos[y_raiz].rank)
            elementos[y_raiz].rank++;
    }
}

// Compara duas arestas usando suas distâncias.
int comp(const void *a, const void *b) {
    const Aresta *aresta1 = a;
    const Aresta *aresta2 = b;
    return aresta_obter_comprimento(*aresta1) - aresta_obter_comprimento(*aresta2);
}

// Cria uma árvore geradora mínima de um grafo.
Grafo criar_arvore_geradora_minima(Grafo grafo) {
    int num_arestas = 0;
    Aresta *arestas = grafo_obter_arestas(grafo, &num_arestas);
    if (arestas == NULL) {
        LOG_AVISO("Erro ao criar árvore geradora mínima!\n");
        return NULL;
    }

    qsort(arestas, num_arestas, sizeof(arestas[0]), comp);

    struct Uniao *grupos = malloc(sizeof *grupos * num_arestas);
    for (int i = 0; i < num_arestas; ++i) {
        grupos[i].raiz = i;
        grupos[i].rank = 0;
    }

    Grafo arvore = grafo_criar(100000);

    int indice_atual = 0;
    int arestas_inseridas = 0;
    while (arestas_inseridas < grafo_obter_tamanho(grafo) - 1 && indice_atual < num_arestas) {
        const Aresta aresta = arestas[indice_atual];
        indice_atual++;

        const char *id_origem = aresta_obter_origem(aresta);
        const int *indice_origem = grafo_obter_indice_vertice(grafo, id_origem);
        const int raiz_origem = encontrar_raiz(grupos, *indice_origem);

        const char *id_destino = aresta_obter_destino(aresta);
        const int *indice_destino = grafo_obter_indice_vertice(grafo, id_destino);
        const int raiz_destino = encontrar_raiz(grupos, *indice_destino);

        // Caso a origem e destino tenham a mesma raiz adicionar a aresta atual vai causar um ciclo
        // no grafo.
        if (raiz_origem == raiz_destino)
            continue;

        // Adiciona o vértice de origem se ele não existe na árvore geradora mínima.
        if (grafo_obter_vertice_por_id(arvore, id_origem) == NULL) {
            Vertice origem = grafo_obter_vertice_por_id(grafo, id_origem);
            grafo_inserir_vertice(arvore, vertice_obter_id(origem), vertice_obter_x(origem),
                                  vertice_obter_y(origem));
        }

        // Adiciona o vértice de destino se ele não existe na árvore geradora mínima.
        if (grafo_obter_vertice_por_id(arvore, id_destino) == NULL) {
            Vertice destino = grafo_obter_vertice_por_id(grafo, id_destino);
            grafo_inserir_vertice(arvore, vertice_obter_id(destino), vertice_obter_x(destino),
                                  vertice_obter_y(destino));
        }

        grafo_inserir_aresta(arvore, aresta_obter_origem(aresta), aresta_obter_destino(aresta),
                             aresta_obter_quadra_esquerda(aresta),
                             aresta_obter_quadra_direita(aresta), aresta_obter_comprimento(aresta),
                             aresta_obter_velocidade(aresta), aresta_obter_nome(aresta));
        arestas_inseridas++;

        unir(grupos, raiz_origem, raiz_destino);
    }

    free(arestas);
    free(grupos);

    return arvore;
}
