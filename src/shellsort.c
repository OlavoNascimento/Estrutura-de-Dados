#include "shellsort.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "lista.h"
void insertionSort(Lista lista_postos, double x, double y);
void shellSort(Lista lista_postos, int H, double x, double y);

void insertionSort(Lista lista_postos, double x, double y) {
    No no1 = lista_get_first;
    No no2 = lista_get_next(no1);
    double distance2;
    double distance1;
    No aux;
    No aux_prev;
    for (int i = 0; i < lista_get_length(lista_postos) - 1; i++) {
        distance1 = get_distance(no1, x, y);
        distance2 = get_distance(no2, x, y);
        if (distance1 > distance2) {
            trocar(no1, no2);

            // itera na direção contraria da lista comparando os nós anteriores
            aux = no1;
            aux_prev = lista_get_previous;

            while (aux != lista_get_first) {
                distance1 = get_distance(aux, x, y);
                distance2 = get_distance(aux_prev, x, y);

                // se uma distancia maior está antes de uma distância menor, realiza a troca
                if (distance1 < distance2) {
                    trocar(aux, aux_prev);
                }

                // atualiza os nós para os anteriores
                aux = lista_get_previous(aux);
                aux_prev = lista_get_previous(aux_prev);
            }
        }

        // atualiza os nós do loop principal
        no1 = lista_get_next(no1);
        no2 = lista_get_next(no1);
    }
}

// Troca duas figuras em um nó de uma lista
void trocar(No no1, No no2) {
    Figura temp = lista_get_figura(no1);
    lista_set_figura(no1, lista_get_figura(no2));
    lista_set_figura(no2, temp);
}

// retorna a distância entre o morador de coordenada x,y e o posto armazenado em no1
double get_distance(No no1, double x, double y) {
    Figura figura = lista_get_figura(no1);
    double x_no = figura_obter_x_inicio(figura);
    double y_no = figura_obter_y_inicio(figura);

    double cat1 = fabs(x - x_no);
    double cat2 = fabs(y - y_no);

    return sqrt(pow(cat1, 2) + pow(cat2, 2));
}

void shellSort(Lista lista_postos, int H, double x, double y) {
    // quando o H chega em 1, é realizado um insertionSort
    if (H == 1) {
        insertionSort(lista_postos, x, y);
    } else {
        No noI = lista_get_first(lista_postos);
        No noH = noI;
        double distanceI;
        double distanceH;
        // itera até pegar o nó equivalente ao indice i+H
        for (int i = 0; i < H; i++) {
            noH = lista_get_next(noH);
        }
        for (int i = 0; i < lista_get_length(lista_postos) - H; i++) {
            distanceI = get_distance(noI, x, y);
            distanceH = get_distance(noH, x, y);
            // se a distancia no noI for maior que a distância no noH, realiza a troca entre os dois
            // nós
            if (distanceI > distanceH) {
                trocar(noI, noH);
            }
            noI = lista_get_next(noI);
            noH = lista_get_next(noH);
        }
        shellSort(lista_postos, H - 1, x, y);
    }
}
