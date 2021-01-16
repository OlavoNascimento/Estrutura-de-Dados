#include "ponto.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"

typedef struct {
    double x;
    double y;
} PontoImp;

// Cria e inicializa um struct PontoImp com os valores passados.
Ponto ponto_criar(double x, double y) {
    PontoImp *ponImp = malloc(sizeof *ponImp);
    ponImp->x = x;
    ponImp->y = y;
    return ponImp;
}

// Cria e inicializa um struct PontoImp com coordenadas iguais a de uma figura.
Ponto ponto_criar_com_figura(Figura fig) {
    if (fig == NULL) {
        LOG_ERRO("Figura nula passada para ponto_criar_com_figura!\n");
        return NULL;
    }
    return ponto_criar(figura_obter_x(fig), figura_obter_y(fig));
}

// Retorna a coordenada x de um ponto.
double ponto_obter_x(Ponto ponto) {
    PontoImp *ponImp = ponto;
    return ponImp->x;
}

// Retorna a coordenada y de um ponto.
double ponto_obter_y(Ponto ponto) {
    PontoImp *ponImp = ponto;
    return ponImp->y;
}

// Retorna verdadeiro se um ponto está contido dentro de um retângulo.
bool ponto_contido_em_retangulo(Ponto ponto, double x1, double y1, double x2, double y2) {
    PontoImp *ponImp = ponto;
    if (ponImp->x < x1 || ponImp->x > x2)
        return false;
    if (ponImp->y < y1 || ponImp->y > y2)
        return false;
    return true;
}

// Retorna verdadeiro se um ponto está contido dentro de um círculo.
bool ponto_contido_em_circulo(Ponto ponto, double x, double y, double raio) {
    PontoImp *ponImp = ponto;
    return pow((ponImp->x - x), 2) + pow((ponImp->y - y), 2) <= pow(raio, 2);
}

// Libera a memória alocada por um ponto.
void ponto_destruir(Ponto ponto) {
    PontoImp *ponImp = ponto;
    free(ponImp);
}
