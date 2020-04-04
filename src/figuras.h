#pragma once

#include <stdbool.h>

#include <retangulo.h>
#include <circulo.h>
#include <texto.h>

typedef enum {
    TipoRetangulo,
    TipoCirculo,
    TipoTexto
} TiposFigura ;

typedef union {
    Retangulo ret;
    Circulo circ;
    Texto tex;
} Figuras;

char* fig_tipo_para_string(TiposFigura tipo);
double max(double a, double b);
double min(double a, double b);
bool interseccao_figuras(Figuras fig1, TiposFigura tipo1, Figuras fig2, TiposFigura tipo2);
Retangulo envolver_figuras(bool intersectam, Figuras fig1, TiposFigura tipo1, Figuras fig2, TiposFigura tipo2);
