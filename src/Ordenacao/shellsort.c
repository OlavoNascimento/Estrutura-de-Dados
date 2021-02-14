#include "shellsort.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "../Estruturas/lista.h"
#include "../Interfaces/figura.h"

// Retorna a distância entre o morador de coordenada x,y e o posto armazenado em no1.
double calcular_distancia(ListaNo no1, double x, double y) {
    Figura figura = lista_obter_info(no1);
    double x_no = figura_obter_x(figura);
    double y_no = figura_obter_y(figura);

    double cat1 = fabs(x - x_no);
    double cat2 = fabs(y - y_no);
    return hypot(cat1, cat2);
}

void insertion_sort(Lista lista_postos, double x, double y) {
    ListaNo no1 = lista_obter_primeiro(lista_postos);
    ListaNo no2 = lista_obter_proximo(no1);
    double distance2;
    double distance1;
    ListaNo aux;
    ListaNo aux_prev;
    for (int i = 0; i < lista_obter_tamanho(lista_postos) - 1; i++) {
        if (no1 == NULL || no2 == NULL)
            break;

        distance1 = calcular_distancia(no1, x, y);
        distance2 = calcular_distancia(no2, x, y);
        if (distance1 > distance2) {
            lista_trocar_info(no1, no2);

            // Itera na direção contraria da lista comparando os nós anteriores.
            aux = no1;
            aux_prev = lista_obter_anterior(no1);

            while (aux != NULL && aux_prev != NULL) {
                distance1 = calcular_distancia(aux, x, y);
                distance2 = calcular_distancia(aux_prev, x, y);

                // Se uma distância maior está antes de uma distância menor, realiza a troca.
                if (distance1 < distance2) {
                    lista_trocar_info(aux, aux_prev);
                }

                // Atualiza os nós para os anteriores.
                aux = lista_obter_anterior(aux);
                aux_prev = lista_obter_anterior(aux_prev);
            }
        }

        // Atualiza os nós do loop principal.
        no1 = lista_obter_proximo(no1);
        no2 = lista_obter_proximo(no1);
    }
}

void shellsort(Lista lista_postos, int H, double x, double y) {
    // Quando o H chega em 1, é realizado um insertionSort.
    if (H == 1) {
        insertion_sort(lista_postos, x, y);
    } else {
        ListaNo noI = lista_obter_primeiro(lista_postos);
        if (noI == NULL)
            return;
        ListaNo noH = noI;
        double distanceI;
        double distanceH;
        // Itera até pegar o nó equivalente ao indice i+H.
        for (int i = 0; i < H; i++) {
            noH = lista_obter_proximo(noH);
        }
        for (int i = 0; i < lista_obter_tamanho(lista_postos) - H; i++) {
            distanceI = calcular_distancia(noI, x, y);
            distanceH = calcular_distancia(noH, x, y);
            // Se a distancia no noI for maior que a distância no noH, realiza a troca entre os dois
            // nós.
            if (distanceI > distanceH) {
                lista_trocar_info(noI, noH);
            }
            noI = lista_obter_proximo(noI);
            noH = lista_obter_proximo(noH);
        }
        shellsort(lista_postos, H - 1, x, y);
    }
}
