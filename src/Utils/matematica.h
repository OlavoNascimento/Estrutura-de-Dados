#ifndef MATEMATICA_H
#define MATEMATICA_H

/*
Este módulo define funções matemáticas que são utilizadas por mais de um módulo.
*/

#include "../Objetos/Formas/circulo.h"
#include "../Objetos/Formas/retangulo.h"

/*
Retorna o maior valor de dois parâmetros.
*/
double max(double a, double b);

/*
Retorna o menor valor de dois parâmetros.
*/
double min(double a, double b);

/*
Calcula o produto de dois vetores.
*/
double produto_vetorial(double a_x, double a_y, double b_x, double b_y);

/*
Retorna verdadeiro caso um circulo e um retângulo se intersectem.
*/
bool circulo_intersecta_retangulo(Circulo circulo, Retangulo retangulo);

/*
Retorna verdadeiro caso um circulo contem um retângulo.
*/
bool circulo_contem_retangulo(Circulo circulo, Retangulo retangulo);

#endif
