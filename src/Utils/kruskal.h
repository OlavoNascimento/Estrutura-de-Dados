#ifndef KRUSKAL_H
#define KRUSKAL_H

/*
Este módulo implementa o algorítmo de Kruskal para gerar a árvore geradora mínima de um grafo. A
árvore geradora mínima é criada a partir das menores distâncias entre os vértices.
*/

#include "../Estruturas/grafo.h"

/*
Cria a árvore geradora mínima de um grafo.
O parâmetro grafo não pode ser nulo.
Retorna um novo grafo que representa a árvore geradora mínima. A memória deve ser liberada pelo
usuário!
*/
Grafo criar_arvore_geradora_minima(Grafo grafo);

#endif
