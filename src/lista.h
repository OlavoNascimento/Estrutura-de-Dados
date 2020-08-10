#ifndef LISTA_H
#define LISTA_H

#include "figuras.h"

typedef void* Lista;
typedef void* No;

/*
 *Cria a cabeça da lista dinamica duplamente encadeada
 *Sem requerimentos
 *retorna o endereço para uma lista vazia
 */
Lista lista_create();

/*
 *Obtém o tamanho da lista (número de nós)
 *É necessário passar a lista como parâmetro
 *retorna um inteiro igual ao tamanho da lista
 */
int lista_get_length(Lista lista);

/*
 *insere um elemento no final da lista
 *é preciso inicar a lista antes de inserir um elemento
 *não retorna nada
 */
No lista_insert_final(Lista lista, Figura figura);

/*
 *insere um elemento posterior um determinado p
 *p deve ser um elemento válido da lista
 *é retornado o endereço do elemento inserido
 */
No lista_insert_before(Lista lista, Figura figura, No p);

/*
 *insere um elemento anterior um determinado p
 *p deve ser um elemento válido da lista
 *é retornado o endereço do elemento inserido
 */
No lista_insert_after(Lista lista, Figura figura, No p);

/*
 *remove um elemento da lista
 *o id passado tem que ser válido
 *não retorna nada
 */
void lista_remove_no(Lista lista, No no_selecionado);

/*
 *utilzada para ter acesso a um nó da lista
 *o id passado tem q ser válido
 *retorna o endereço do nó
 */
No lista_get_no(Lista lista, char id[20]);

/*
 *pega o último elemento da lista
 *a lista não pode estar vazia
 *retorna o endereço do último nó
 */
No lista_get_last(Lista lista);

/*
 *pega o primeiro elemento da lista
 *a lista não pode estar vazia
 *retorna o endereço do primeiro nó
 */
No lista_get_first(Lista lista);

/*
 *acessa as informações de um nó
 *p não pode ser NULL e deve indicar um endereço válido que esteja na lista
 *retorna o endereço para as informações do nó especificado por p
 */
Figura lista_get_figura(No p);

/*
 *acessa o proximo elemento de p
 *p deve indicar um endereço válido que esteja na lista
 *retorna o endereço do elemento posterior de p
 */
No lista_get_next(Lista lista, No p);

/*
 *acessa o proximo elemento de p
 *p deve indicar um endereço válido que esteja na lista
 *retorna o endereço do elemento anterior de p
 */
No lista_get_previous(Lista lista, No p);

/*
 *libera a memoria alocada em todos os elementos da lista
 *é necessário que a lista seja incializada para que seja possível liberar
 */
void lista_libera_lista(Lista lista);

#endif