#include "vias.h"

#include <stdio.h>
#include <string.h>

#include "../Estruturas/grafo.h"
#include "../Estruturas/quadtree.h"
#include "../Utils/logging.h"

// Tamanho maxímo de um comando do arquivo de vias.
#define TAMANHO_COMANDO 300

// Adiciona um vértice ao grafo e a quadtree.
void adicionar_vertice(Grafo vias, QuadTree qt_vias, const char *linha) {
    char id[100];
    double x, y;
    sscanf(linha, "v %s %lf %lf", id, &x, &y);
    Vertice vertice = grafo_inserir_vertice(vias, id, x, y);
    quadtree_inserir(qt_vias, ponto_criar(x, y), vertice);
}

// Adiciona uma aresta ao grafo.
void adicionar_aresta(Grafo vias, const char *linha) {
    char nome[100], origem[100], destino[100], quadra_esquerda[100], quadra_direita[100];
    double comprimento;
    double velocidade;
    sscanf(linha, "e %s %s %s %s %lf %lf %s", origem, destino, quadra_esquerda, quadra_direita,
           &comprimento, &velocidade, nome);
    grafo_inserir_aresta(vias, origem, destino, quadra_esquerda, quadra_direita, comprimento,
                         velocidade, nome);
}

// Lê um arquivo de vias fornecido a função e adiciona os vértices e arestas descritas em suas linha
// como elementos de um grafo/quadtree..
void vias_ler(const char *caminho_vias, Tabela quadtrees, Grafo vias) {
    printf("Lendo vias\n");
    FILE *arquivo_vias = fopen(caminho_vias, "r");
    if (arquivo_vias == NULL) {
        LOG_ERRO("Falha ao ler arquivo de vias: %s!\n", caminho_vias);
        return;
    }

    QuadTree qt_vias = tabela_buscar(quadtrees, "vias");

    char linha[TAMANHO_COMANDO];
    while (fgets(linha, TAMANHO_COMANDO, arquivo_vias) != NULL) {
        char comando[TAMANHO_COMANDO];
        sscanf(linha, "%s", comando);

        if (strcmp("v", comando) == 0) {
            adicionar_vertice(vias, qt_vias, linha);
        } else if (strcmp("e", comando) == 0) {
            adicionar_aresta(vias, linha);
        }
    }

    fclose(arquivo_vias);
}
