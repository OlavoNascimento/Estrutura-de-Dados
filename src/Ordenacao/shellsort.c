#include "shellsort.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "../Estruturas/lista.h"
#include "../Interfaces/figura.h"

static inline void trocar_figuras(Figura *figuras, int i, int j) {
    Figura temp = figuras[i];
    figuras[i] = figuras[j];
    figuras[j] = temp;
}

// Retorna a distância entre o morador de coordenada x,y e o posto armazenado em no1.
double calcular_distancia(Figura figura, double x, double y) {
    double x_no = figura_obter_x(figura);
    double y_no = figura_obter_y(figura);

    double cat1 = fabs(x - x_no);
    double cat2 = fabs(y - y_no);
    return hypot(cat1, cat2);
}

void insertion_sort(Figura *postos, int tamanho, double x, double y) {
    int no1 = 0;
    int no2 = 1;

    for (int i = 0; i < tamanho - 1; i++) {
        if (no1 >= tamanho || no2 >= tamanho)
            break;

        double distancia1 = calcular_distancia(postos[no1], x, y);
        double distancia2 = calcular_distancia(postos[no2], x, y);
        if (distancia1 > distancia2) {
            trocar_figuras(postos, no1, no2);

            // Itera na direção contraria da lista comparando os nós anteriores.
            int aux = no1;
            int aux_prev = no1 - 1;

            while (aux_prev >= 0) {
                distancia1 = calcular_distancia(postos[aux], x, y);
                distancia2 = calcular_distancia(postos[aux_prev], x, y);

                // Se uma distância maior está antes de uma distância menor, realiza a troca.
                if (distancia1 < distancia2)
                    trocar_figuras(postos, aux, aux_prev);

                // Atualiza os nós para os anteriores.
                aux--;
                aux_prev--;
            }
        }

        // Atualiza os nós do loop principal.
        no1++;
        no2++;
    }
}

void shellsort(Figura *postos, int tamanho, int H, double x, double y) {
    if (tamanho <= 0 || H <= 0)
        return;
    // Quando o H chega em 1, é realizado um insertion sort.
    if (H == 1) {
        insertion_sort(postos, tamanho, x, y);
        return;
    }

    int noI = 0;
    int noH = H;

    for (int i = 0; i < tamanho - H; i++) {
        double distanciaI = calcular_distancia(postos[noI], x, y);
        double distanciaH = calcular_distancia(postos[noH], x, y);
        // Se a distancia no noI for maior que a distância no noH, realiza a troca entre os dois
        // nós.
        if (distanciaI > distanciaH)
            trocar_figuras(postos, noI, noH);

        noI++;
        noH++;
    }
    shellsort(postos, tamanho, H - 1, x, y);
}
