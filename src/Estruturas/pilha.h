#ifndef PILHA_H
#define PILHA_H

/*
Este módulo define uma Pilha, responsável por armazenar os diferentes tipos de figuras que existem
no projeto. Disponibiliza funções para modificar criar, modificar e destruir a pilha.
*/

#include <stdbool.h>

typedef void* Pilha;
typedef void* PilhaInfo;

typedef void DestruirInfo(PilhaInfo);

/*
Cria uma pilha.
É possível especificar uma função que libere a memória alocada pelas informações da fila, para que
ao liberar a memória da fila suas informações também sejam liberadas.
Retorna o endereço para uma pilha vazia.
*/
Pilha pilha_criar(DestruirInfo destruir_info);

/*
Verifica se uma pilha está vazia.
O parâmetro pilha não pode ser nulo.
Retorna verdadeiro caso a pilha esteja vazio, falso caso contrário..
*/
bool pilha_esta_vazia(Pilha pilha);

/*
Retorna o tamanho de uma pilha.
O parâmetro pilha não pode ser nulo.
Retorna o tamanho da pilha.
*/
int pilha_obter_tamanho(Pilha pilha);

/*
Adiciona uma informação em uma pilha.
Ambos os parâmetros não podem ser nulos.
A informação é definida como o novo topo da pilha.
*/
void pilha_inserir(Pilha pilha, PilhaInfo info);

/*
Remove a informação no topo de uma pilha.
O parâmetro pilha não pode ser nulo.
A informação é retirada da pilha e o a informação a seguir é definida como o novo topo da pilha.
*/
PilhaInfo pilha_remover(Pilha pilha);

/*
Retorna a informação no topo da pilha sem remove-la.
O parâmetro pilha não pode ser nulo.
A info no topo da pilha é retornada.
*/
PilhaInfo pilha_obter_topo(Pilha pilha);

/*
Libera a memória alocada por uma pilha.
O parâmetro pilha não pode ser nulo.
A memória alocada pela pilha é liberada.
*/
void pilha_destruir(Pilha pilha);

#endif
