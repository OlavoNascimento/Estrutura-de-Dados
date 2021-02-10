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
    // O eixo y é invertido, então é necessário inverter o angulo.
    return angulo * -1;
}

static inline void trocar_figuras(Figura *figuras, int i, int j) {
    Figura temp = figuras[i];
    figuras[i] = figuras[j];
    figuras[j] = temp;
}

// Particiona uma lista utilizando um pivô. Todos os valores menóres que o pivô são colocados antes
// dele e todos os maiores após ele.
int particionar(Figura min, Figura *figuras, int inicio, int fim) {
    // Usa o último elemento como pivo.
    double angulo_pivo = angulo_polar(min, figuras[fim]);
    int i = inicio - 1;

    for (int j = inicio; j < fim; j++) {
        if (angulo_polar(min, figuras[j]) < angulo_pivo) {
            i++;
            trocar_figuras(figuras, i, j);
        }
    }

    trocar_figuras(figuras, i + 1, fim);
    return i + 1;
}

// Ordena uma lista de figuras em ordem crescente utilizando o algorítmo quicksort.
void quicksort(Figura min, Figura *figuras, int inicio, int fim) {
    if (inicio < fim) {
        int meio = particionar(min, figuras, inicio, fim);
        quicksort(min, figuras, inicio, meio - 1);
        quicksort(min, figuras, meio + 1, fim);
    }
}
