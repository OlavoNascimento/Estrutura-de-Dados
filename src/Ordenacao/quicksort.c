#include "quicksort.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "../Estruturas/lista.h"
#include "../Interfaces/figura.h"

// Retorna o angulo entre dois pontos
double angulo_polar(Figura min, Figura ponto) {
    double angulo = atan2(figura_obter_y(ponto) - figura_obter_y(min),
                          figura_obter_x(ponto) - figura_obter_x(min));
    return angulo * -1;
}

// Particiona uma lista utilizando um pivô. Todos os valores menóres que o pivô são
// colocando antes dele e todos os maiores após ele.
ListaNo particionar(Figura min, ListaNo inicio, ListaNo fim) {
    // Usa o último elemento como pivo.
    double angulo_pivo = angulo_polar(min, lista_obter_info(fim));
    ListaNo i = lista_obter_anterior(inicio);

    for (ListaNo *j = inicio; j != fim; j = lista_obter_proximo(j)) {
        if (angulo_polar(min, lista_obter_info(j)) <= angulo_pivo) {
            i = (i == NULL) ? inicio : lista_obter_proximo(i);
            lista_trocar_info(i, j);
        }
    }

    i = (i == NULL) ? inicio : lista_obter_proximo(i);
    lista_trocar_info(i, fim);
    return i;
}

// Ordena uma lista de figuras em ordem crescente utilizando o algorítmo quicksort.
void quicksort(Figura min, ListaNo inicio, ListaNo fim) {
    if (fim == NULL || inicio == fim || inicio == lista_obter_proximo(fim))
        return;

    ListaNo *meio = particionar(min, inicio, fim);
    quicksort(min, inicio, lista_obter_anterior(meio));
    quicksort(min, lista_obter_proximo(meio), fim);
}
