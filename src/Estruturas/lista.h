#ifndef LISTA_H
#define LISTA_H

/*
Este módulo define uma Lista, responsável por armazenar os diferentes tipos de figuras que existem
no projeto. Disponibiliza funções para modificar criar, modificar e destruir a lista.
*/

#include "../Interfaces/figura.h"

typedef void* Lista;
typedef void* No;

/*
Cria o ponteiro inicial de lista dinamica duplamente encadeada.
Não é passado nenhum parâmetro.
Retorna o endereço para uma lista vazia.
*/
Lista lista_criar();

/*
Obtém o tamanho da lista (número de nós).
O parâmetro lista não pode ser nulo e deve ser uma lista existente.
retorna um inteiro igual ao tamanho da lista.
*/
int lista_obter_tamanho(Lista lista);

/*
Insere um elemento no final da lista.
Ambos os parâmetros não podem ser nulos, o parâmetro lista deve ser uma lista existente.
Retrna o endereço para o elemento adicionado.
*/
No lista_inserir_final(Lista lista, Figura figura);

/*
Insere um elemento posterior a um determinado nó "p".
Todos os parâmetros não podem ser nulos e o parâmetro p deve ser um elemento válido da lista.
É retornado o endereço do elemento inserido.
*/
No lista_inserir_antes(Lista lista, Figura figura, No p);

/*
Insere um elemento anterior a um determinado nó "p".
Todos os parâmetros não podem ser nulos e o parâmetro p deve ser um elemento válido da lista.
É retornado o endereço do elemento inserido.
*/
No lista_inserir_depois(Lista lista, Figura figura, No p);

/*
Remove um elemento da lista.
Todos os parâmetros não podem ser nulos e o parâmetro lista deve ser uma lista existente.
*/
void lista_remover(Lista lista, No no_selecionado);

/*
Acessa  um nó da lista.
O id passado deve ser válido.
Retorna o endereço do nó.
*/
No lista_buscar(Lista lista, const char id[100]);

/*
Acessa o primeiro elemento da lista
O parâmetro lista não pode ser nulo, deve ser uma lista existe e a lista não pode estar vazia.
Retorna o endereço do primeiro nó.
*/
No lista_obter_primeiro(Lista lista);

/*
Acessa o último elemento da lista.
O parâmetro lista não pode ser nulo, deve ser uma lista existe e a lista não pode estar vazia.
Retorna o endereço do último nó.
*/
No lista_obter_ultimo(Lista lista);

/*
Acessa as informações de um nó.
O parâmetro p não pode ser NULL e deve indicar um endereço válido que esteja na lista.
Retorna o endereço para as informações do nó especificado por p.
*/
Figura lista_obter_figura(No p);

/*
Substitui a figura armazena em um nó
Ambos os parámetros não podem ser nulos.
O nó tem sua figura original substituida pela figura passada como parâmetro para a função.
*/
void lista_definir_figura(No p, Figura figura);

/*
Acessa o elemento posterior ao elemento p.
Ambos parâmetros não podem ser nulos e o parâmetro p deve indicar um endereço válido que esteja na
lista.
Retorna o endereço do elemento posterior de p.
*/
No lista_obter_proximo(No p);

/*
Acessa o elemento anterior ao elemento p.
Ambos parâmetros não podem ser nulos e o parâmetro p deve indicar um endereço válido que esteja na
lista.
Retorna o endereço do elemento posterior de p.
*/
No lista_obter_anterior(No p);

/*
Troca as figuras armazenadas em dois nós.
Ambos os parâmetros não podem ser nulos.
Os nós tem suas figuras trocadas.
*/
void lista_trocar_figuras(No no1, No no2);

/*
Libera a memória alocada em todos os elementos da lista.
O parâmetro lista não pode ser nulo e deve ser uma lista existente.
*/
void lista_destruir(Lista lista);

#endif
