#ifndef PONTO_H
#define PONTO_H

/*
Este módulo define um Ponto, o qual possui coordenadas espaciais em um plano.

Suas propriedades são:
    x: Coordenada x do ponto no plano.
    y: Coordenada y do ponto no plano.
*/

#include <stdbool.h>

#include "../../Interfaces/figura.h"

typedef void *Ponto;

/*
Cria e inicializa Ponto com os valores passados.
O usuário é responsável por liberar a memória alocada!
*/
Ponto ponto_criar(double x, double y);

/*
Cria e inicializa Ponto com coordenadas iguais a da figura passada.
A figura não pode ser nula.
O usuário é responsável por liberar a memória alocada!
*/
Ponto ponto_criar_com_figura(Figura fig);

/*
Obtém a coordenada x de um ponto.
O parâmetro ponto não pode ser nulo.
Retorna a coordenada x de um ponto.
*/
double ponto_obter_x(Ponto ponto);

/*
Obtém a coordenada y de um ponto.
O parâmetro ponto não pode ser nulo.
Retorna a coordenada y de um ponto.
*/
double ponto_obter_y(Ponto ponto);

/*
Retorna verdadeiro se um ponto está contido dentro de um retângulo.
O parâmetro ponto não pode ser nulo.
*/
bool ponto_contido_em_retangulo(Ponto ponto, double x1, double y1, double x2, double y2);

/*
Retorna verdadeiro se um ponto está contido dentro de um círculo.
O parâmetro ponto não pode ser nulo. Raio deve ser maior que 0.
*/
bool ponto_contido_em_circulo(Ponto ponto, double x, double y, double raio);

/*
Libera a memória alocada por um ponto.
O parâmetro ponto não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada. O ponteiro não poderá ser utilizado após isso!
*/
void ponto_destruir(Ponto ponto);

#endif
