#pragma once

#include <stdio.h>

typedef struct {
    char id[100];
    double raio;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
} Circulo;

Circulo criar_circulo(char *linha);
void circulo_para_svg(FILE *arquivo, Circulo circ);
bool interseccao_circulos(Circulo circ1, Circulo circ2);
bool ponto_interno_circulo(Circulo circ, double ponto_x, double ponto_y);
