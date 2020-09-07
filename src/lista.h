#ifndef LISTA_H
#define LISTA_H

#include "figuras.h"

typedef void* Lista;
typedef void* No;
/*
Cria o ponteiro inicial de lista dinamica duplamente encadeada.
Não é passado nenhum parâmetro.
Retorna o endereço para uma lista vazia.
*/
Lista lista_create();

/*
Obtém o tamanho da lista (número de nós).
O parâmetro lista não pode ser nulo e deve ser uma lista existente.
retorna um inteiro igual ao tamanho da lista.
*/
int lista_get_length(Lista lista);

/*
Insere um elemento no final da lista.
Ambos os parâmetros não podem ser nulos, o parâmetro lista deve ser uma lista existente.
Retrna o endereço para o elemento adicionado.
*/
No lista_insert_final(Lista lista, Figura figura);

/*
Insere um elemento posterior a um determinado nó "p".
Todos os parâmetros não podem ser nulos e o parâmetro p deve ser um elemento válido da lista.
É retornado o endereço do elemento inserido.
*/
No lista_insert_before(Lista lista, Figura figura, No p);

/*
Insere um elemento anterior a um determinado nó "p".
Todos os parâmetros não podem ser nulos e o parâmetro p deve ser um elemento válido da lista.
É retornado o endereço do elemento inserido.
*/
No lista_insert_after(Lista lista, Figura figura, No p);

/*
Remove um elemento da lista.
Todos os parâmetros não podem ser nulos e o parâmetro lista deve ser uma lista existente.
*/
void lista_remove_no(Lista lista, No no_selecionado);

/*
Acessa  um nó da lista.
O id passado deve ser válido.
Retorna o endereço do nó.
*/
No lista_get_no(Lista lista, char id[100]);

/*
Acessa o último elemento da lista.
O parâmetro lista não pode ser nulo, deve ser uma lista existe e a lista não pode estar vazia.
Retorna o endereço do último nó.
*/
No lista_get_last(Lista lista);

/*
Acessa o primeiro elemento da lista
O parâmetro lista não pode ser nulo, deve ser uma lista existe e a lista não pode estar vazia.
Retorna o endereço do primeiro nó.
*/
No lista_get_first(Lista lista);

/*
Acessa as informações de um nó.
O parâmetro p não pode ser NULL e deve indicar um endereço válido que esteja na lista.
Retorna o endereço para as informações do nó especificado por p.
*/
Figura lista_get_figura(No p);

/*
Acessa o elemento posterior ao elemento p.
Ambos parâmetros não podem ser nulos e o parâmetro p deve indicar um endereço válido que esteja na
lista.
Retorna o endereço do elemento posterior de p.
*/
No lista_get_next(Lista lista, No p);

/*
Acessa o elemento anterior ao elemento p.
Ambos parâmetros não podem ser nulos e o parâmetro p deve indicar um endereço válido que esteja na
lista.
Retorna o endereço do elemento posterior de p.
*/
No lista_get_previous(Lista lista, No p);

/*
Libera a memória alocada em todos os elementos da lista.
O parâmetro lista não pode ser nulo e deve ser uma lista existente.
*/
void lista_libera_lista(Lista lista);

#endif
