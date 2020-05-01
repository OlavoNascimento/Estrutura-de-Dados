#pragma once

#include <stdbool.h>

#include <retangulo.h>
#include <circulo.h>
#include <texto.h>
#include <linha.h>

typedef enum {
    TIPOS_FIGURA_MIN,
    TIPO_RETANGULO,
    TIPO_CIRCULO,
    TIPO_TEXTO,
    TIPO_LINHA,
    TIPOS_FIGURA_MAX
} TiposFigura;

typedef union {
    Retangulo ret;
    Circulo circ;
    Texto tex;
    Linha lin;
} Figuras;

double max(double a, double b);
double min(double a, double b);

const char *tipo_para_string_figura(TiposFigura tipo);
double obter_x_inicio_figura(Figuras figura, TiposFigura tipo);
double obter_y_inicio_figura(Figuras figura, TiposFigura tipo);
double obter_x_fim_figura(Figuras figura, TiposFigura tipo);
double obter_y_fim_figura(Figuras figura, TiposFigura tipo);
const char *obter_id_figura(Figuras *figura, TiposFigura tipo);
const char *obter_cor_borda_figura(Figuras *figura, TiposFigura tipo);
const char *obter_cor_preenchimento_figura(Figuras *figura, TiposFigura tipo);

void escrever_informacoes_figura(FILE *arquivo,
                                 Figuras figura, TiposFigura tipo);
void escrever_svg_figura(FILE *arquivo, Figuras figura, TiposFigura tipo);

bool checar_interseccao_figuras(Figuras fig1, TiposFigura tipo1,
                         Figuras fig2, TiposFigura tipo2);
Retangulo envolver_figuras(Figuras fig1, TiposFigura tipo1,
                           Figuras fig2, TiposFigura tipo2);
bool checar_ponto_interno_figura(Figuras figura, TiposFigura tipo,
                          double ponto_x, double ponto_y);
Linha ligar_ponto_figura(Circulo ponto, Figuras figura, TiposFigura tipo);

void alterar_cor_figura(Figuras *figura, TiposFigura tipo,
                        char *cor_borda, char *cor_preenchimento);
