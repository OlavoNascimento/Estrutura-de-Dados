#ifndef FILA_H
#define FILA_H

/*
Este módulo define uma Fila, responsável por armazenar os diferentes tipos de informações que
existem no projeto. Disponibiliza funções para modificar criar, modificar e destruir a fila.
*/

#include <stdbool.h>

typedef struct Fila_s *Fila;

typedef void *FilaInfo;

typedef void FilaDestruirInfo(FilaInfo info);

/*
Cria uma fila.
É possível especificar uma função que libere a memória alocada pelas informações da fila, para que
ao liberar a memória da fila suas informações também sejam liberadas.
Retorna o endereço para uma fila vazia.
*/
Fila fila_criar(FilaDestruirInfo destruir_info);

/*
Verifica se uma fila está vazia.
O parâmetro fila não pode ser nulo.
Retorna verdadeiro caso a fila esteja vazio, falso caso contrário..
*/
bool fila_esta_vazia(Fila fila);

/*
Retorna o tamanho de uma fila.
O parâmetro fila não pode ser nulo.
Retorna o tamanho da fila.
*/
int fila_obter_tamanho(Fila fila);

/*
Adiciona uma informação em uma fila.
Ambos os parâmetros não podem ser nulos.
A informação é definida como o novo topo da fila.
*/
void fila_inserir(Fila fila, FilaInfo info);

/*
Remove a informação no início de uma fila.
O parâmetro fila não pode ser nulo.
A informação é retirada da fila e o a informação a seguir é definida como o novo topo da fila.
*/
FilaInfo fila_remover(Fila fila);

/*
Retorna a informação no início da fila sem remove-la.
O parâmetro fila não pode ser nulo.
A informação no início da fila é retornada.
*/
FilaInfo fila_obter_info(Fila fila);

/*
Libera a memória alocada por uma fila.
O parâmetro fila não pode ser nulo.
A memória alocada pela fila é liberada.
*/
void fila_destruir(Fila fila);

#endif
