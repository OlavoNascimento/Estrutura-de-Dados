#ifndef GRAFO_H
#define GRAFO_H

/*
Implementa uma Grafo, o qual representa a ligação de vértices através de arestas. Um grafo vazio é
criada pela operação grafo_criar(), podendo receber o tamanho máximo de vértices permitidos no
grafo.
Após a criação, vértices podem ser inseridos utilizando a função grafo_inserir_vertice, não é
possível ultrapassar o limite definido ao criar o grafo.
Conexões entre os vértices (arestas) podem ser criadas utilizando a função grafo_criar_aresta.
É possível listar os vértices adjacentes ao um vértice e checar se dois vértices são adjacentes,
através das funções grafo_obter_adjacentes e grafo_checar_adjacente respectivamente.
Para liberar a memória alocada pelo grafo é necessário utilizar a função grafo_destruir.
*/

#include <stdbool.h>

#include "lista.h"

/*
Foi usado 'typedef struct *' em vez de 'typedef void *' para permitir ao compilador diferenciar os
tipos, não afetando o encapsulamento de dados!
O struct *não é definido neste header* e sim em seu arquivo .c correspondente, o ponteiro deste tipo
não pode ser criado sem seu construtor, assim como suas propriedades não podem ser acessadas sem uso
de getters/setters.
*/
typedef struct Grafo_s *Grafo;
typedef struct Aresta_s *Aresta;
typedef struct Vertice_s *Vertice;

/*
Cria um grafo.
É necessário especificar o número máximo de vértices do grafo.
Retorna o endereço para um grafo vazio.
*/
Grafo grafo_criar();

/*
Insere um vértice no grafo.
Os parâmetros grafo e id não podem ser nulos.
*/
Vertice grafo_inserir_vertice(Grafo grafo, const char *id, double x, double y);

/*
Remove um vértice no grafo.
Os parâmetros grafo e id não podem ser nulos.
*/
Vertice grafo_remover_vertice(Grafo grafo, const char *id);

/*
Retorna o tamanho atual de um grafo.
*/
int grafo_obter_tamanho(Grafo grafo);

/*
Insere uma aresta no grafo.
Os parâmetros grafo, origem, destino, quadra_esquerda, quadra_direita e nome não podem ser
nulos.
*/
void grafo_inserir_aresta(Grafo grafo, const char *origem, const char *destino,
                          const char *quadra_esquerda, const char *quadra_direita,
                          double comprimento, double velocidade, const char *nome);

/*
Remove a aresta de um grafo.
Nenhum dos parâmetros podem ser nulos.
*/
void grafo_remover_aresta(Grafo grafo, const char *origem, const char *destino);

/*
Retorna verdadeiro se o vértice id1 é adjacente ao vértice id2.
*/
bool grafo_checar_adjacente(Grafo grafo, const char *id1, const char *id2);

/*
Retorna uma lista contendo os vértices adjacentes ao vértice id1.
Nenhum dos parâmetros podem ser nulos.
O usuário é responsável por liberar a memória!
*/
Lista grafo_obter_adjacentes(Grafo grafo, const char *id1);

/*
Retorna o vértice referente ao id especificado.
O parâmetro grafo não pode ser nulo.
*/
Vertice grafo_obter_vertice_por_id(Grafo grafo, const char *id);

/*
Retorna o vértice referente ao indice "indice".
O parâmetro grafo não pode ser nulo.
*/
Vertice grafo_obter_vertice_por_indice(Grafo grafo, int indice);

/*
Retorna um vetor contendo todas as arestas do grafo.
O parâmetro grafo não pode ser nulo.
*/
Aresta *grafo_obter_arestas(Grafo grafo, int *tamanho_vetor);

/*
Retorna o indice de um vértice de id especificado.
Nenhum dos parâmetros podem ser nulos.
*/
const int *grafo_obter_indice_vertice(Grafo grafo, const char *id);

/*
Cria uma lista com a representações de um grafo. Arestas da arvore geradora mínima são destacadas.
Nenhum dos parâmetros podem ser nulos.
*/
Lista grafo_escrever_svg(Grafo grafo, Grafo mst);

/*
Retorna o nome de uma aresta.
O parâmetro aresta não pode ser nulo.
*/
const char *aresta_obter_nome(Aresta aresta);

/*
Retorna a origem de uma aresta.
O parâmetro aresta não pode ser nulo.
*/
const char *aresta_obter_origem(Aresta aresta);

/*
Retorna o destino de uma aresta.
O parâmetro aresta não pode ser nulo.
*/
const char *aresta_obter_destino(Aresta aresta);

/*
Retorna a quadra a esquerda de uma aresta.
O parâmetro aresta não pode ser nulo.
*/
const char *aresta_obter_quadra_esquerda(Aresta aresta);

/*
Retorna a quadra a direita de uma aresta.
O parâmetro aresta não pode ser nulo.
*/
const char *aresta_obter_quadra_direita(Aresta aresta);

/*
Retorna o comprimento de uma aresta.
O parâmetro aresta não pode ser nulo.
*/
double aresta_obter_comprimento(Aresta aresta);

/*
Retorna a velocidade de uma aresta.
O parâmetro aresta não pode ser nulo.
*/
double aresta_obter_velocidade(Aresta aresta);

/*
Retorna o id de um vértice.
O parâmetro vértice não pode ser nulo.
*/
const char *vertice_obter_id(Vertice vertice);

/*
Retorna a coordenada x de um vértice.
O parâmetro vértice não pode ser nulo.
*/
double vertice_obter_x(Vertice vertice);

/*
Retorna a coordenada y de um vértice.
O parâmetro vértice não pode ser nulo.
*/
double vertice_obter_y(Vertice vertice);

/*
Retorna uma lista com as arestas adjacentes ao vértice passado como parâmetro.
O parâmetro vértice não pode ser nulo.
*/
Lista vertice_obter_arestas(Vertice vertice);

/*
Libera a memória alocada por um vértice.
*/
void vertice_destruir(Vertice vertice);

/*
Libera a memória alocada por um grafo.
*/
void grafo_destruir(Grafo grafo);

#endif
