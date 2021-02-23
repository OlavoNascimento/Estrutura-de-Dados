#ifndef SHELLSORT_H
#define SHELLSORT_H

/*
Este módulo define a função shellsort, responsável por ordenar um array em ordem crescente.
*/

#include "../Estruturas/lista.h"
#include "../Interfaces/figura.h"

/*
Ordena uma lista de figuras utilizando o algorítmo shellsort.
A lista não pode ser nula, os valores de x e y que representam as coordenadas de um morador, devem
ser validos. A lista é ordena em ordem crescente.
*/
void shellsort(Figura *postos, int tamanho, int H, double x, double y);

#endif
