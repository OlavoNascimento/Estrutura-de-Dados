#pragma once

#include <stdbool.h>

#include <retangulo.h>
#include <circulo.h>
#include <texto.h>
#include <linha.h>

#define SVG_MARGEM 5

typedef enum {
    TipoRetangulo,
    TipoCirculo,
    TipoTexto,
    TipoLinha
} TiposFigura;

typedef union {
    Retangulo ret;
    Circulo circ;
    Texto tex;
    Linha lin;
} Figuras;

typedef struct {
    double origem_x;
    double origem_y;
    double largura;
    double altura;
} Exibicao;

char* fig_tipo_para_string(TiposFigura tipo);
void escrever_informacoes_figura(FILE *arquivo, Figuras figura, TiposFigura tipo);
char* obter_id_figura(Figuras *figura, TiposFigura tipo);
double obter_x_figura(Figuras figura, TiposFigura tipo);
double obter_y_figura(Figuras figura, TiposFigura tipo);
void atualizar_exibicao_svg(Exibicao *exi, Figuras fig, TiposFigura tipo);

bool interseccao_figuras(Figuras fig1, TiposFigura tipo1, Figuras fig2, TiposFigura tipo2);
Retangulo envolver_figuras(bool intersectam, Figuras fig1, TiposFigura tipo1, Figuras fig2, TiposFigura tipo2);
bool ponto_interno_figura(Figuras figura, TiposFigura tipo, double ponto_x, double ponto_y);
Linha ligar_ponto_figura(Circulo ponto, Figuras figura, TiposFigura tipo);
void alterar_cor_figura(Figuras *figura, TiposFigura tipo, char *cor_borda, char *cor_preenchimento);
