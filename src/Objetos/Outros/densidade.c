#include "densidade.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lista.h"
#include "logging.h"

typedef struct {
    double x;
    double y;
    double altura;
    double largura;
    double densidade;
} DenImp;

Densidade densidade_criar(double x, double y, double largura, double altura, double densidade) {
    DenImp *denImp = malloc(sizeof(DenImp));
    denImp->largura = largura;
    denImp->altura = altura;
    denImp->x = x;
    denImp->y = y;
    denImp->densidade = densidade;
    return denImp;
}

Densidade densidade_ler(const char *linha) {
    double densidade;
    double x;
    double y;
    double largura;
    double altura;
    sscanf(linha, "dd %lf %lf %lf %lf %lf", &x, &y, &largura, &altura, &densidade);
    return densidade_criar(x, y, largura, altura, densidade);
}

// Retorna o número de habitantes.
double densidade_calcular_habitantes(DenImp *regiao) {
    return regiao->densidade * (regiao->largura / 1000) * (regiao->altura / 1000);
}

bool checarArea(DenImp *regiao, double x, double y) {
    if (x >= regiao->x && y >= regiao->y) {
        if (x <= (regiao->x + regiao->largura) && y <= (regiao->y + regiao->altura)) {
            return true;
        }
    }
    return false;
}

double densidade_buscar_coordenada(Lista lista_densidades, double x, double y) {
    for (No i = lista_get_first(lista_densidades); i != NULL; i = lista_get_next(i)) {
        DenImp *regiao = (DenImp *) figura_obter_figura(lista_get_figura(i));
        if (checarArea(regiao, x, y))
            return densidade_calcular_habitantes(regiao);
    }
    return 0;
}

void densidade_destruir(Densidade dens) {
    free(dens);
}
