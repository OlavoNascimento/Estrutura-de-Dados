#ifndef DIJKSTRA_H
#define DIJKSTRA_H

/*
Implementa o algoritmo de Dijkstra, responsável por encontrar o menor caminho entre dois vértices em
um grafo. O caminho pode ser obtido com base no comprimento (dijkstra_distancia) ou na velocidade
(dijkstra_velocidade) das arestas. Retorna uma pilha com os vértices do grafo, formando o caminho da
origem até o destino.
O usuário é responsável por liberar a memória alocada para a pilha.
*/

#include "../Estruturas/grafo.h"
#include "../Estruturas/pilha.h"

/*
Retorna uma pilha com os vértices referentes ao menor caminho entre os vértices de indice "origem" e
"destino".
O menor caminho é obtido com base no comprimentos das arestas do grafo.
O parâmetro grafo não pode ser nulo e os índices "origem" e "destino" devem ser válidos.
*/
Pilha dijkstra_distancia(Grafo grafo, const char *id_origem, const char *id_destino);

/*
Retorna uma pilha com os vértices referentes ao menor caminho entre os vértices de indice "origem" e
"destino".
O menor caminho é obtido com base na velocidade das arestas do grafo.
O parâmetro grafo não pode ser nulo e os índices "origem" e "destino" devem ser válidos.
*/
Pilha dijkstra_velocidade(Grafo grafo, const char *id_origem, const char *id_destino);

#endif
