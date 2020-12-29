#include "shellsort.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "figuras.h"
#include "lista.h"

// Retorna a distância entre o morador de coordenada x,y e o posto armazenado em no1.
double get_distance(No no1, double x, double y) {
    Figura figura = lista_get_figura(no1);
    double x_no = figura_obter_x(figura);
    double y_no = figura_obter_y(figura);

    double cat1 = fabs(x - x_no);
    double cat2 = fabs(y - y_no);

    return sqrt(pow(cat1, 2) + pow(cat2, 2));
}

void insertionSort(Lista lista_postos, double x, double y) {
    No no1 = lista_get_first(lista_postos);
    No no2 = lista_get_next(no1);
    double distance2;
    double distance1;
    No aux;
    No aux_prev;
    for (int i = 0; i < lista_get_length(lista_postos) - 1; i++) {
        if (no1 == NULL || no2 == NULL)
            break;

        distance1 = get_distance(no1, x, y);
        distance2 = get_distance(no2, x, y);
        if (distance1 > distance2) {
            lista_trocar_figuras(no1, no2);

            // Itera na direção contraria da lista comparando os nós anteriores.
            aux = no1;
            aux_prev = lista_get_previous(no1);

            while (aux != NULL && aux_prev != NULL && aux != lista_get_first) {
                distance1 = get_distance(aux, x, y);
                distance2 = get_distance(aux_prev, x, y);

                // Se uma distância maior está antes de uma distância menor, realiza a troca.
                if (distance1 < distance2) {
                    lista_trocar_figuras(aux, aux_prev);
                }

                // Atualiza os nós para os anteriores.
                aux = lista_get_previous(aux);
                aux_prev = lista_get_previous(aux_prev);
            }
        }

        // Atualiza os nós do loop principal.
        no1 = lista_get_next(no1);
        no2 = lista_get_next(no1);
    }
}

void shellSort(Lista lista_postos, int H, double x, double y) {
    // Quando o H chega em 1, é realizado um insertionSort.
    if (H == 1) {
        insertionSort(lista_postos, x, y);
    } else {
        No noI = lista_get_first(lista_postos);
        if (noI == NULL)
            return;
        No noH = noI;
        double distanceI;
        double distanceH;
        // Itera até pegar o nó equivalente ao indice i+H.
        for (int i = 0; i < H; i++) {
            noH = lista_get_next(noH);
        }
        for (int i = 0; i < lista_get_length(lista_postos) - H; i++) {
            distanceI = get_distance(noI, x, y);
            distanceH = get_distance(noH, x, y);
            // Se a distancia no noI for maior que a distância no noH, realiza a troca entre os dois
            // nós.
            if (distanceI > distanceH) {
                lista_trocar_figuras(noI, noH);
            }
            noI = lista_get_next(noI);
            noH = lista_get_next(noH);
        }
        shellSort(lista_postos, H - 1, x, y);
    }
}
