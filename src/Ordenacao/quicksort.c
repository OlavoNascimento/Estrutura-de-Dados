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
    ListaNo i = inicio;
    ListaNo j = fim;

    double angulo_pivo = angulo_polar(min, lista_obter_info(inicio));
    // Itera até os ponteiros se encontrarem.
    while (true) {
        while (i != NULL && angulo_polar(min, lista_obter_info(i)) < angulo_pivo)
            // Checa se o ângulo polar do ponteiro inferior é menor que o ângulo pivo.
            i = lista_obter_proximo(i);

        while (j != NULL && angulo_polar(min, lista_obter_info(j)) > angulo_pivo)
            // Checa se o ângulo polar do ponteiro superior é maior que o ângulo pivo.
            j = lista_obter_anterior(j);

        if (i >= j)
            return j;

        // Troca as figuras armazenadas nos nós i e j, já que o nó i é maior que o pivô ou o
        // nó j é menor que o pivô.
        lista_trocar_info(i, j);
        i = lista_obter_proximo(i);
        j = lista_obter_anterior(j);
    }
}

// Ordena uma lista de figuras em ordem crescente utilizando o algorítmo quicksort.
void quicksort(Figura min, ListaNo inicio, ListaNo fim) {
    if (inicio == NULL || fim == NULL || inicio >= fim)
        return;

    ListaNo meio = particionar(min, inicio, fim);
    quicksort(min, inicio, meio);
    quicksort(min, lista_obter_proximo(meio), fim);
}
