#pragma once

#include <stdio.h>

typedef struct {
    char id[100];
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    char texto[300];
} Texto;

Texto ler_texto(const char *linha);
void escrever_informacoes_texto(FILE *arquivo, Texto tex);
void escrever_svg_texto(FILE *arquivo, Texto tex);
