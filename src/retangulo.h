#pragma once

#include <stdio.h>

typedef struct {
    char id[100];
    double largura;
    double altura;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    int tracejado_tamanho;
    int tracejado_espaco;
} Retangulo;

Retangulo criar_retangulo(char *linha);
void retangulo_para_svg(FILE *arquivo, Retangulo ret);
bool interseccao_retangulos(Retangulo ret1, Retangulo ret2);
bool ponto_interno_retangulo(Retangulo ret, double ponto_x, double ponto_y);
