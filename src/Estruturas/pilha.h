#ifndef PILHA_H
#define PILHA_H

/*
Este módulo define uma Pilha, responsável por armazenar os diferentes tipos de figuras que existem
no projeto. Disponibiliza funções para modificar criar, modificar e destruir a pilha.
*/

#include "../Interfaces/figura.h"

typedef void* Pilha;

/*
Cria uma pilha.
Não é passado nenhum parâmetro.
Retorna o endereço para uma pilha vazia.
*/
Pilha pilha_criar();

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
Adiciona uma figura em uma pilha.
Ambos os parâmetros não podem ser nulos.
A figura é definida como o novo topo da pilha.
*/
void pilha_inserir(Pilha pilha, Figura figura);

/*
Remove a figura no topo de uma pilha.
O parâmetro pilha não pode ser nulo.
A figura é retirada da pilha e o a figura a seguir é definida como o novo topo da pilha.
*/
Figura pilha_remover(Pilha pilha);

/*
Retorna a figura no topo da pilha sem remove-la.
O parâmetro pilha não pode ser nulo.
A figura no topo da pilha é retornada.
*/
Figura pilha_obter_topo(Pilha pilha);

/*
Libera a memória alocada por uma pilha.
O parâmetro pilha não pode ser nulo.
A memória alocada pela pilha é liberada.
*/
void pilha_destruir(Pilha pilha);

#endif
