#ifndef DENSIDADE_H
#define DENSIDADE_H

/*
Este módulo define uma Densidade, o qual representa uma área retangular que contem a densidade
populacional em sua área. Possui o número de habitantes por km^2, assim como altura, largura e
cordenadas espaciais.

Suas propriedades são:
    densidade: Número de habitantes por km^2 na área delimitado pela densidade.
    largura
    altura
    x: Coordenada x da densidade no plano.
    y: Coordenada y da densidade no plano.
*/

#include <stdbool.h>
#include <stdio.h>

#include "../../Estruturas/lista.h"

typedef void *Densidade;

/*
Cria e inicializa um Densidade com os valores passados.
A densidade não pode ser menor que zero.
O usuário é responsável por liberar a memória alocada!
*/
Densidade densidade_criar(double x, double y, double largura, double altura, double densidade);

/*
Cria e inicializa um Densidade a partir de um texto obtido no arquivo .geo.
O parâmetro linha não pode ser nulo.
O usuário é responsável por liberar a memória alocada!
*/
Densidade densidade_ler(const char *linha);

/*
Checa se um ponto está contido por as densidades armazenadas em uma lista e retorna o número de
habitantes.
O parâmetro lista_densidades não pode ser nulo.
Retorna o número de habitantes que vivem dentro da densidade.
*/
double densidade_buscar_coordenada(Lista lista_densidades, double x, double y);

/*
Libera a memória alocada por uma densidade.
O parâmetro densidade não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada. O ponteiro não poderá ser utilizado após isso!
*/
void densidade_destruir(Densidade densidade);

#endif
