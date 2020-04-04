#pragma once

#include <stdio.h>

typedef struct {
    double x1;
    double y1;
    double x2;
    double y2;
    char cor_borda[20];
    char cor_preenchimento[20];
} Linha;

void linha_para_svg(FILE *arquivo, Linha lin);