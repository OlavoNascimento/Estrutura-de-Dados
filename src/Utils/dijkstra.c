#include <float.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../Estruturas/grafo.h"
#include "../Estruturas/lista.h"
#include "../Estruturas/pilha.h"
#include "../Objetos/Formas/circulo.h"
#include "../Objetos/Outros/animacao.h"
#include "../Objetos/Outros/texto.h"
#include "../Utils/logging.h"

typedef struct {
    double custo;
    int predecessor;
    bool aberto;
} DijkstraInfos;

typedef double ObterCusto(Aresta aresta);

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

int buscar_menor_custo(Grafo grafo, DijkstraInfos *infos) {
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
void relaxar_adjacentes(Grafo grafo, DijkstraInfos *infos, const int indice_origem,
                        ObterCusto obter_custo_aresta) {
    const Vertice vertice = grafo_obter_vertice_por_indice(grafo, indice_origem);
    const Lista arestas = vertice_obter_arestas(vertice);

    for_each_lista(aresta_no, arestas) {
        const Aresta aresta = lista_obter_info(aresta_no);
        const double custo = obter_custo_aresta(aresta);

        const char *id_destino = aresta_obter_destino(aresta);
        const int *indice = grafo_obter_indice_vertice(grafo, id_destino);
        if (indice == NULL) {
            LOG_AVISO("Não é possível relaxar um vértice inválido\n");
            return;
        }

        // Analisa se o custo atualmente setado no indice do DijkstraInfos, que representa o
        // vertice de mesmo indice no grafo, é maior que o custo obtido pelo caminho anterior.
        if (infos[*indice].custo > infos[indice_origem].custo + custo) {
            infos[*indice].custo = infos[indice_origem].custo + custo;
            infos[*indice].predecessor = indice_origem;
        }
    }
}

// Retorna uma pilha com os vértices que definem o caminho da origem até o destino
Pilha dijkstra(Grafo grafo, const char *id_origem, const char *id_destino,
               ObterCusto obter_custo_aresta) {
    const int *origem = grafo_obter_indice_vertice(grafo, id_origem);
    const int *destino = grafo_obter_indice_vertice(grafo, id_destino);
    if (origem == NULL || destino == NULL) {
        LOG_AVISO("Dijkstra recebeu origem ou destino inválido!\n");
        return NULL;
    }

    // Os indices do arranjo DijkstraInfos serão os mesmos indices dos vertices do grafo
    DijkstraInfos *infos = inicializa_dijkstra(grafo, *origem);

    int origem_atual = buscar_menor_custo(grafo, infos);
    while (origem_atual != -1) {
        infos[origem_atual].aberto = false;
        relaxar_adjacentes(grafo, infos, origem_atual, obter_custo_aresta);
        origem_atual = buscar_menor_custo(grafo, infos);
    }

    Pilha pilha_caminho = pilha_criar(NULL);
    if (pilha_caminho == NULL) {
        LOG_ERRO("Não foi possível alocar memória para pilha\n");
        free(infos);
        return NULL;
    }

    // Adiciona os vértices que formam o menor caminho a pilha.
    for (int i = *destino; i != -1; i = infos[i].predecessor)
        pilha_inserir(pilha_caminho, grafo_obter_vertice_por_indice(grafo, i));

    free(infos);
    return pilha_caminho;
}

Pilha dijkstra_distancia(Grafo grafo, const char *id_origem, const char *id_destino) {
    return dijkstra(grafo, id_origem, id_destino, aresta_obter_comprimento);
}

Pilha dijkstra_velocidade(Grafo grafo, const char *id_origem, const char *id_destino) {
    return dijkstra(grafo, id_origem, id_destino, aresta_obter_velocidade);
}

// Cria um ponto animado que percorre um caminho.
// A função retorna a animação criada caso ela precise ser modificada.
Animacao dijkstra_criar_representacao(Pilha caminho, Lista saida, const char *cor_caminho,
                                      const Ponto ponto_origem, const Ponto ponto_destino,
                                      FILE *arquivo_log) {
    const int num_pontos = pilha_obter_tamanho(caminho);
    Ponto *pontos = malloc(sizeof *pontos * num_pontos);

    int i = 0;
    // Utiliza o ponto de origem como primeiro ponto, não o vértice inicial.
    fprintf(arquivo_log, "Inicie no ponto (%lf, %lf)\n", ponto_obter_x(ponto_origem),
            ponto_obter_y(ponto_origem));
    pontos[i++] = ponto_criar(ponto_obter_x(ponto_origem), ponto_obter_y(ponto_origem));

    Vertice anterior = pilha_remover(caminho);
    while (i < num_pontos - 1) {
        Vertice atual = pilha_remover(caminho);
        fprintf(arquivo_log, "Avance do vértice %s para o vértice %s\n", vertice_obter_id(anterior),
                vertice_obter_id(atual));
        pontos[i++] = ponto_criar(vertice_obter_x(atual), vertice_obter_y(atual));
        anterior = atual;
    }

    // Utiliza o ponto de destino como último ponto, não o vértice final.
    fprintf(arquivo_log, "Avance do vértice %s para o ponto de destino (%lf, %lf)\n",
            vertice_obter_id(anterior), ponto_obter_x(ponto_origem), ponto_obter_y(ponto_origem));

    pontos[i++] = ponto_criar(ponto_obter_x(ponto_destino), ponto_obter_y(ponto_destino));
    pilha_destruir(caminho);
    caminho = NULL;

    char id[100];
    id[0] = '\0';
    snprintf(id, 100, "(%.2lf,%.2lf)-(%.2lf,%.2lf)-%d", ponto_obter_x(ponto_origem),
             ponto_obter_y(ponto_origem), ponto_obter_x(ponto_destino),
             ponto_obter_y(ponto_destino), num_pontos);

    // Caminho a ser percorrido.
    Animacao animacao = animacao_criar(id, "blue", "blue", cor_caminho, num_pontos, pontos);
    lista_inserir_final(saida, animacao);

    // Circulo indicando a origem.
    Circulo circ_origem = circulo_criar("", 14, ponto_obter_x(ponto_origem),
                                        ponto_obter_y(ponto_origem), "black", "yellow");
    lista_inserir_final(saida, circ_origem);
    Texto texto_origem = texto_criar("", ponto_obter_x(ponto_origem),
                                     ponto_obter_y(ponto_origem) + 5, "black", "black", "I");
    texto_definir_alinhamento(texto_origem, TEXTO_CENTRO);
    lista_inserir_final(saida, texto_origem);

    // Circulo indicando o destino.
    Circulo circ_destino = circulo_criar("", 14, ponto_obter_x(ponto_destino),
                                         ponto_obter_y(ponto_destino), "black", "red");
    lista_inserir_final(saida, circ_destino);
    Texto texto_destino = texto_criar("", ponto_obter_x(ponto_destino),
                                      ponto_obter_y(ponto_destino) + 5, "black", "black", "F");
    texto_definir_alinhamento(texto_destino, TEXTO_CENTRO);
    lista_inserir_final(saida, texto_destino);

    return animacao;
}
