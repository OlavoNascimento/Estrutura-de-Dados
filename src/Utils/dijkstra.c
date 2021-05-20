#include <float.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../Estruturas/grafo.h"
#include "../Estruturas/lista.h"
#include "../Estruturas/pilha.h"
#include "../Estruturas/tabelahash.h"
#include "../Utils/logging.h"

typedef struct {
    double distancia;
    double velocidade;
    int predecessor;
    bool aberto;
} dijkstra_infos;

void inicializaDijkstra(Grafo *grafo, dijkstra_infos *infos, int origem) {
    for (int i = 0; i < grafo_obter_tamanho_atual(grafo); i++) {
        infos[i].aberto = true;
        infos[i].distancia =
            DBL_MAX /
            2;  // dividido por 2 para não ter o risco de somar algum numero e passar o limite
        infos[i].predecessor = -1;
    }
    infos[origem].distancia = 0;
}

bool existeAberto(Grafo *grafo, dijkstra_infos *infos) {
    for (int i = 0; i < grafo_obter_tamanho_atual(grafo); i++) {
        if (infos[i].aberto) {
            return true;
        }
    }
    return false;
}

int menorDistancia(Grafo *grafo, dijkstra_infos *infos) {
    int i;
    for (int i = 0; i < grafo_obter_tamanho_atual(grafo); i++) {
        if (infos[i].aberto) {
            break;
        }
    }
    if (i == grafo_obter_tamanho_atual(grafo)) {
        // rodou todo o grafo e não encontrou vertices abertos
        return -1;
    }

    int menor = i;

    for (i = menor + 1; i < grafo_obter_tamanho_atual(grafo); i++) {
        if (infos[i].aberto && infos[menor].distancia > infos[i].distancia) {
            menor = i;
        }
    }
    return menor;
}

int menorVelocidade(Grafo *grafo, dijkstra_infos *infos) {
    int i;
    for (i = 0; i < grafo_obter_tamanho_atual(grafo); i++) {
        if (infos[i].aberto) {
            break;
        }
    }
    if (i == grafo_obter_tamanho_atual(grafo)) {
        // rodou todo o grafo e não encontrou vertices abertos
        return -1;
    }

    int menor = i;

    for (i = menor + 1; i < grafo_obter_tamanho_atual(grafo); i++) {
        if (infos[i].aberto && infos[menor].velocidade > infos[i].velocidade) {
            menor = i;
        }
    }
    return menor;
}

// relaxa todos os vertices adjacentes, definindo as distancias
void relaxa_distancia(Grafo grafo, dijkstra_infos *infos, Tabela id_indice,
                      Vertice vertice_origem_atual) {
    const int *indice_origem = tabela_buscar(id_indice, vertice_obter_id(vertice_origem_atual));

    if (indice_origem == NULL) {
        LOG_AVISO("Não é possível relaxar vértices adjacentes a uma origem inválida\n");
    }

    Lista arestas = vertice_obter_arestas(vertice_origem_atual);
    if (arestas == NULL) {
        LOG_AVISO(
            "Não é possível relaxar vértices adjacentes ao vértice de origem com arestas "
            "inválidas\n");
    }

    char id_destino[100];
    double comprimento;
    Aresta aresta_aux;
    for_each_lista(aresta_no, arestas) {
        aresta_aux = lista_obter_info(aresta_no);
        comprimento = aresta_obter_comprimento(aresta_aux);
        strcpy(id_destino, aresta_obter_destino(aresta_aux));
        const int *indice_aux = tabela_buscar(id_indice, id_destino);

        if (indice_aux == NULL) {
            LOG_AVISO("Não é possível relaxar vértice inválido \n");
        }

        // analisa se a distancia atualmente setada no indice do dikstra_infos, que representa o
        // vertice de mesmo indice no grafo, é maior que a distancia obtida pelo caminho anterior
        if (infos[*indice_aux].distancia > infos[*indice_origem].distancia + comprimento) {
            infos[*indice_aux].distancia = infos[*indice_origem].distancia + comprimento;
            infos[*indice_aux].predecessor = *indice_origem;
        }
    }
}

// relaxa todos os vertices adjacentes, definindo as distancias
void relaxa_velocidade(Grafo grafo, dijkstra_infos *infos, Tabela id_indice,
                       Vertice vertice_origem_atual) {
    const int *indice_origem = tabela_buscar(id_indice, vertice_obter_id(vertice_origem_atual));

    if (indice_origem == NULL) {
        LOG_AVISO("Não é possível relaxar vértices adjacentes a uma origem inválida\n");
    }

    Lista arestas = vertice_obter_arestas(vertice_origem_atual);
    if (arestas == NULL) {
        LOG_AVISO(
            "Não é possível relaxar vértices adjacentes ao vértice de origem com arestas "
            "inválidas\n");
    }

    char id_destino[100];
    double velocidade;
    Aresta aresta_aux;
    for_each_lista(aresta_no, arestas) {
        aresta_aux = lista_obter_info(aresta_no);
        velocidade = aresta_obter_velocidade(aresta_aux);
        strcpy(id_destino, aresta_obter_destino(aresta_aux));
        const int *indice_aux = tabela_buscar(id_indice, id_destino);

        if (indice_aux == NULL) {
            LOG_AVISO("Não é possível relaxar vértice inválido \n");
        }

        // analisa se a distancia atualmente setada no indice do dikstra_infos, que representa o
        // vertice de mesmo indice no grafo, é maior que a distancia obtida pelo caminho anterior
        if (infos[*indice_aux].distancia > infos[*indice_origem].distancia + velocidade) {
            infos[*indice_aux].distancia = infos[*indice_origem].distancia + velocidade;
            infos[*indice_aux].predecessor = *indice_origem;
        }
    }
}

// Retorna uma pilha com os indices que definem o caminho da origem até o destino
Pilha dijkstra_distancia(Grafo *grafo, int origem, int destino) {
    // Os indices do arranjo dijkstra_infos serão os mesmos indices dos vertices do grafo
    dijkstra_infos *infos = malloc(sizeof(dijkstra_infos) * grafo_obter_tamanho(grafo));
    if (infos == NULL) {
        LOG_ERRO("Não foi possivel alocar memória para informações do dijkstra\n");
    }

    inicializaDijkstra(grafo, infos, origem);
    Vertice vertice;

    while (existeAberto(grafo, infos)) {
        int origem_atual = menorDistancia(grafo, infos);
        infos[origem_atual].aberto = false;

        vertice = grafo_obter_vertice_por_indice(grafo, origem_atual);

        relaxa_distancia(grafo, infos, grafo_obter_tabela(grafo), vertice);
    }

    Pilha pilha_caminho = pilha_criar(free);

    if (pilha_caminho == NULL) {
        LOG_ERRO("Não foi possivel alocar memória para pilha\n");
    }

    int diferenca_destino_origem = abs(destino - origem);
    int aux = destino;
    for (int i = 0; i < diferenca_destino_origem; i++) {
        if (aux == origem) {
            break;
        }
        pilha_inserir(pilha_caminho, aux);
        aux = infos[aux].predecessor;
    }
}

Pilha dijkstra_velocidade(Grafo *grafo, int origem, int destino) {
    // Os indices do arranjo dijkstra_infos serão os mesmos indices dos vertices do grafo
    dijkstra_infos *infos = malloc(sizeof(dijkstra_infos) * grafo_obter_tamanho(grafo));
    if (infos == NULL) {
        LOG_ERRO("Não foi possivel alocar memória para informações do dijkstra\n");
    }

    inicializaDijkstra(grafo, infos, origem);
    Vertice vertice;

    while (existeAberto(grafo, infos)) {
        int origem_atual = menorVelocidade(grafo, infos);
        infos[origem_atual].aberto = false;

        vertice = grafo_obter_vertice_por_indice(grafo, origem_atual);

        relaxa_velocidade(grafo, infos, grafo_obter_tabela(grafo), vertice);
    }

    Pilha pilha_caminho = pilha_criar(free);

    if (pilha_caminho == NULL) {
        LOG_ERRO("Não foi possivel alocar memória para pilha\n");
    }

    int diferenca_destino_origem = abs(destino - origem);
    int aux = destino;
    for (int i = 0; i < diferenca_destino_origem; i++) {
        if (aux == origem) {
            break;
        }
        pilha_inserir(pilha_caminho, aux);
        aux = infos[aux].predecessor;
    }
}
