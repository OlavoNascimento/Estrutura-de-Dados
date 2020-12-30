#ifndef SHELLSORT_H
#define SHELLSORT_H

#include "../Estruturas/lista.h"

/*
Este módulo define a função shellsort, responsável por ordenar um array em ordem crescente.
*/

/*
Ordena uma lista de figuras utilizando o algorítmo shellsort.
A lista não pode ser nula, os valores de x e y que representam as coordenadas de um morador, devem
ser validos. A lista é ordena em ordem crescente.
*/
void shellsort(Lista lista_postos, int H, double x, double y);

#endif
