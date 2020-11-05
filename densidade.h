#ifndef DENSIDADE_H
#define DENSIDADE_H

#include <stdbool.h>
#include <stdio.h>

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
Obtém a coordenada x de uma região de densidade x.
O parâmetro dens não pode ser nulo.
Retorna a coordenada x de uma região de densidade x.
*/
double densidade_obter_x(Densidade dens);
/*
Obtém a coordenada y de uma região de densidade x.
O parâmetro dens não pode ser nulo.
Retorna a coordenada y de uma região de densidade x.
*/
double densidade_obter_y(Densidade dens);

/*
Obtém a coordenada x_final uma região de densidade x.
O parâmetro dens não pode ser nulo.
Retorna a coordenada x de uma região de densidade x.
*/
double densidade_obter_x_final(Densidade dens);
/*
Obtém a coordenada y de uma região de densidade x.
O parâmetro dens não pode ser nulo.
Retorna a coordenada y de uma região de densidade x.
*/
double densidade_obter_y_final(Densidade dens);

/*
Libera a memória alocada por um dens.
O parâmetro dens não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada pelo struct. O ponteiro não poderá ser utilizado após isso!
*/
void densidade_destruir(Densidade dens);
#endif
