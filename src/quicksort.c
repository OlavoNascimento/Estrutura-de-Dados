#include "quicksort.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "lista.h"

// Retorna o angulo entre dois pontos
double angulo_polar(Figura min, Figura ponto) {
    return atan2(figura_obter_y_inicio(ponto) - figura_obter_y_inicio(min),
                 figura_obter_x_inicio(ponto) - figura_obter_x_inicio(min));
}

// Troca duas figuras em um nó de uma lista
void trocar(No no1, No no2) {
    Figura temp = lista_get_figura(no1);
    lista_set_figura(no1, lista_get_figura(no2));
    lista_set_figura(no2, temp);
}

// Particiona uma lista utilizando um pivô. Todos os valores menóres que o pivô são colocando antes
// dele e todos os maiores após ele.
No particionar(Figura min, No inicio, No fim) {
    No i = inicio;
    No j = fim;

    double angulo_pivo = angulo_polar(min, lista_get_figura(inicio));
    // Itera até os ponteiros se encontrarem.
    while (true) {
        while (i != NULL && angulo_polar(min, lista_get_figura(i)) < angulo_pivo)
            // Checa se o ângulo polar do ponteiro inferior é menor que o ângulo pivo.
            i = lista_get_next(i);

        while (j != NULL && angulo_polar(min, lista_get_figura(j)) > angulo_pivo)
            // Checa se o ângulo polar do ponteiro superior é maior que o ângulo pivo.
            j = lista_get_previous(j);

        if (i >= j) {
            return j;
        }
        // Troca as figuras armazenadas nos nós i e j, já que o nó i é maior que o pivô ou o nó j é
        // menor que o pivô.
        trocar(i, j);
        i = lista_get_next(i);
        j = lista_get_previous(j);
    }
}

// Ordena uma lista de figuras em ordem crescente utilizando o algorítmo quicksort.
void quicksort(Figura min, No inicio, No fim) {
    if (inicio == NULL || fim == NULL || inicio >= fim)
        return;

    No meio = particionar(min, inicio, fim);
    quicksort(min, inicio, meio);
    quicksort(min, lista_get_next(meio), fim);
}