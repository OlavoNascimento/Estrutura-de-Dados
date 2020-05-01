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

Circulo ler_circulo(const char *linha);
void escrever_informacoes_circulo(FILE *arquivo, Circulo circ);
void escrever_svg_circulo(FILE *arquivo, Circulo circ);
bool checar_interseccao_circulo(Circulo circ1, Circulo circ2);
bool checar_ponto_interno_circulo(Circulo circ, double ponto_x, double ponto_y);
