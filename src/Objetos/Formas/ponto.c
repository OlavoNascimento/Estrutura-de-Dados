#include "ponto.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"

struct Ponto_s {
    double x;
    double y;
};

// Cria e inicializa um Ponto com os valores passados.
Ponto ponto_criar(double x, double y) {
    Ponto ponto = malloc(sizeof *ponto);
    if (ponto == NULL) {
        fprintf(stderr, "Erro ao alocar memória para um novo ponto!\n");
        return NULL;
    }
    ponto->x = x;
    ponto->y = y;
    return ponto;
}

// Cria e inicializa um Ponto com coordenadas iguais a de uma figura.
Ponto ponto_criar_com_figura(Figura fig) {
    if (fig == NULL) {
        LOG_ERRO("Figura nula passada para ponto_criar_com_figura!\n");
        return NULL;
    }
    return ponto_criar(figura_obter_x(fig), figura_obter_y(fig));
}

// Retorna a coordenada x de um ponto.
double ponto_obter_x(Ponto ponto) {
    return ponto->x;
}

// Retorna a coordenada y de um ponto.
double ponto_obter_y(Ponto ponto) {
    return ponto->y;
}

// Retorna verdadeiro se um ponto está contido dentro de um retângulo.
bool ponto_contido_em_retangulo(Ponto ponto, double x1, double y1, double x2, double y2) {
    if (ponto->x < x1 || ponto->x > x2)
        return false;
    if (ponto->y < y1 || ponto->y > y2)
        return false;
    return true;
}

// Retorna verdadeiro se um ponto está contido dentro de um círculo.
bool ponto_contido_em_circulo(Ponto ponto, double x, double y, double raio) {
    return pow((ponto->x - x), 2) + pow((ponto->y - y), 2) <= pow(raio, 2);
}

// Libera a memória alocada por um ponto.
void ponto_destruir(Ponto ponto) {
    free(ponto);
}
