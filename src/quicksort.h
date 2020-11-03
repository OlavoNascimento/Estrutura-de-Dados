#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <stdbool.h>
#include <stdio.h>

#include "lista.h"

/*
Este módulo define a função quicksort, responsável por ordenar um array em ordem crescente.
*/

/*
Ordena uma lista de figuras utilizando o algorítmo quicksort.
A lista não pode ser nula.
A lista é ordena em ordem crescente.
*/
void quicksort(Figura min, No inicio, No fim);

#endif