#pragma once

#include <stdbool.h>

#include <retangulo.h>
#include <circulo.h>
#include <texto.h>

typedef enum {
    tipo_retangulo,
    tipo_circulo,
    tipo_texto
} TiposFigura ;

typedef union {
    Retangulo ret;
    Circulo circ;
    Texto tex;
} Figuras;

bool retangulos_interseccao(Retangulo ret1, Retangulo ret2);
bool circulos_interseccao(Circulo circ1, Circulo circ2);
bool circulo_retangulo_interseccao(Circulo circ, Retangulo ret);
