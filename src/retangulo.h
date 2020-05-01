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

Retangulo ler_retangulo(char *linha);
void escrever_informacoes_retangulo(FILE *arquivo, Retangulo ret);
void escrever_svg_retangulo(FILE *arquivo, Retangulo ret);
bool checar_interseccao_retangulo(Retangulo ret1, Retangulo ret2);
bool checar_ponto_interno_retangulo(Retangulo ret, double ponto_x,
                                    double ponto_y);
