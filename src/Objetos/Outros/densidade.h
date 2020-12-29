#ifndef DENSIDADE_H
#define DENSIDADE_H

#include <stdbool.h>
#include <stdio.h>

#include "lista.h"

typedef void *Densidade;

/*
Cria e inicializa um struct Densidade com os valores passados.
A densidade não pode ser menor que zero.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Densidade densidade_criar(double x, double y, double largura, double altura, double densidade);

/*
Cria e inicializa um struct Densidade a partir de um texto obtido no arquivo .geo.
O parâmetro linha não pode ser nulo.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Densidade densidade_ler(const char *linha);

/*
Verifica se um ponto está contido em uma densidade.
O parâmetro lista_densidades não pode ser nulo.
Retorna a densidade caso o ponto esteja contido.
*/
double densidade_buscar_coordenada(Lista lista_densidades, double x, double y);

/*
Libera a memória alocada por uma densidade.
O parâmetro densidade não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada pelo struct. O ponteiro não poderá ser utilizado após isso!
*/
void densidade_destruir(Densidade densidade);

#endif
