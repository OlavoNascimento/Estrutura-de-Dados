#include <float.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../Estruturas/grafo.h"
#include "../Estruturas/lista.h"
#include "../Estruturas/pilha.h"
#include "../Utils/logging.h"

typedef struct {
    double custo;
    int predecessor;
    bool aberto;
} DijkstraInfos;

typedef double ObterValorComparado(Aresta aresta);

DijkstraInfos *inicializa_dijkstra(Grafo grafo, int origem) {
    // Os indices do arranjo DijkstraInfos serão os mesmos indices dos vertices do grafo
    DijkstraInfos *infos = malloc(sizeof *infos * grafo_obter_tamanho(grafo));
    if (infos == NULL) {
        LOG_ERRO("Não foi possível alocar memória para as informações do dijkstra\n");
        return NULL;
    }
    for (int i = 0; i < grafo_obter_tamanho(grafo); i++) {
        infos[i].aberto = true;
        // Dividido por 2 para não ter o risco de somar algum número e passar o limite.
        infos[i].custo = DBL_MAX / 2;
        infos[i].predecessor = -1;
    }
    infos[origem].custo = 0;
    return infos;
}

bool existe_aberto(Grafo grafo, DijkstraInfos *infos) {
    for (int i = 0; i < grafo_obter_tamanho(grafo); i++) {
        if (infos[i].aberto)
            return true;
    }
    return false;
}

int menor_custo(Grafo grafo, DijkstraInfos *infos) {
    int i = 0;
    for (i = 0; i < grafo_obter_tamanho(grafo); i++) {
        if (infos[i].aberto)
            break;
    }
    // Rodou todo o grafo e não encontrou vertices abertos
    if (i == grafo_obter_tamanho(grafo))
        return -1;

    int menor = i;
    for (i = menor + 1; i < grafo_obter_tamanho(grafo); i++) {
        if (infos[i].aberto && infos[menor].custo > infos[i].custo)
            menor = i;
    }
    return menor;
}

// Relaxa todos os vértices adjacentes, definindo o custo para cada vértice adjacente.
void relaxa(Grafo grafo, DijkstraInfos *infos, Vertice vertice, ObterValorComparado obter_valor) {
    const int *indice_origem = grafo_obter_indice_vertice(grafo, vertice_obter_id(vertice));
    if (indice_origem == NULL) {
        LOG_AVISO("Não é possível relaxar vértices adjacentes a uma origem inválida\n");
        return;
    }

    Lista arestas = vertice_obter_arestas(vertice);
    for_each_lista(aresta_no, arestas) {
        const Aresta aresta = lista_obter_info(aresta_no);
        const double custo = obter_valor(aresta);

        const char *id_destino = aresta_obter_destino(aresta);
        const int *indice = grafo_obter_indice_vertice(grafo, id_destino);
        if (indice == NULL) {
            LOG_AVISO("Não é possível relaxar um vértice inválido\n");
            return;
        }

        // Analisa se o custo atualmente setado no indice do DijkstraInfos, que representa o
        // vertice de mesmo indice no grafo, é maior que o custo obtido pelo caminho anterior.
        if (infos[*indice].custo > infos[*indice_origem].custo + custo) {
            infos[*indice].custo = infos[*indice_origem].custo + custo;
            infos[*indice].predecessor = *indice_origem;
        }
    }
}

// Retorna uma pilha com os indices que definem o caminho da origem até o destino
Pilha dijkstra(Grafo grafo, int origem, int destino, ObterValorComparado obter_valor_aresta) {
    // Os indices do arranjo DijkstraInfos serão os mesmos indices dos vertices do grafo
    DijkstraInfos *infos = inicializa_dijkstra(grafo, origem);

    while (existe_aberto(grafo, infos)) {
        int origem_atual = menor_custo(grafo, infos);
        infos[origem_atual].aberto = false;

        const Vertice vertice = grafo_obter_vertice_por_indice(grafo, origem_atual);
        relaxa(grafo, infos, vertice, obter_valor_aresta);
    }

    Pilha pilha_caminho = pilha_criar(NULL);
    if (pilha_caminho == NULL) {
        LOG_ERRO("Não foi possível alocar memória para pilha\n");
        return NULL;
    }

    // Adiciona os indices do menor caminho a pilha.
    for (int i = destino; i != -1; i = infos[i].predecessor)
        pilha_inserir(pilha_caminho, grafo_obter_vertice_por_indice(grafo, i));

    free(infos);
    return pilha_caminho;
}

Pilha dijkstra_distancia(Grafo grafo, int origem, int destino) {
    return dijkstra(grafo, origem, destino, aresta_obter_comprimento);
}

Pilha dijkstra_velocidade(Grafo grafo, int origem, int destino) {
    return dijkstra(grafo, origem, destino, aresta_obter_velocidade);
}
