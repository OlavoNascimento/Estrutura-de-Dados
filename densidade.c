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
    return denImp;
}

Densidade densidade_ler(const char *linha) {
    double densidade;
    double x;
    double y;
    double largura;
    double altura;
    sscanf(linha, "%lf %lf %lf %lf %lf", &x, &y, &largura, &altura, &densidade);
    return densidade_criar(x, y, largura, altura, densidade);
}

double densidade_obter_y(Densidade dens) {
    DenImp *denImp = (DenImp *) dens;
    return denImp->y;
}

double densidade_obter_y_final(Densidade dens) {
    DenImp *denImp = (DenImp *) dens;
    return denImp->y + denImp->altura;
}

double densidade_obter_x(Densidade dens) {
    DenImp *denImp = (DenImp *) dens;
    return denImp->x;
}

double densidade_obter_x_final(Densidade dens) {
    DenImp *denImp = (DenImp *) dens;
    return denImp->x + denImp->largura;
}

double buscar_densidade_coordenada(Lista lista_densidades, double x, double y) {
    No noAux = lista_get_first(lista_densidades);
    Figura regiao = lista_get_figura(noAux);
    regiao = figura_obter_figura(regiao);
    int tamanho = lista_get_length(lista_densidades);

    for (int i = 0; i < tamanho; i++) {
        DenImp *denImp = (DenImp *) regiao;
        if (checarArea(regiao, x, y)) {
            return denImp->densidade;
            break;
        }
        noAux = lista_get_next(noAux);
        regiao = lista_get_figura(noAux);
        regiao = figura_obter_figura(regiao);
    }

    return 0;
}

bool checarArea(Densidade regiao, double x, double y) {
    DenImp *denImp = (DenImp *) regiao;
    if (x >= denImp->x && y >= denImp->y) {
        if (x <= (denImp->x + denImp->largura) && y <= (denImp->y + denImp->altura)) {
            return true;
        }
    }
    return false;
}

void densidade_destruir(Densidade dens) {
    free(dens);
}
