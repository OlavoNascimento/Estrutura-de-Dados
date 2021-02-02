#ifndef QUICKSORT_H
#define QUICKSORT_H

/*
Este módulo define a função quicksort, responsável por ordenar um array em ordem crescente.
*/

#include <stdbool.h>
#include <stdio.h>

#include "../Estruturas/lista.h"
#include "../Interfaces/figura.h"

/*
Ordena um array de figuras em ordem crescente utilizando o algorítmo quicksort. As figuras são
comparadas utilizando o ângulo entre elas e a figura mínima.
A figura mínima e o array não podem ser nulos.
O array é ordenado em ordem crescente.
*/
void quicksort(Figura min, Figura *figuras, int inicio, int fim);

#endif
