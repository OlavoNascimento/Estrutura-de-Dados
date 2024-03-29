#include "linha.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"
#include "../../Utils/matematica.h"

struct Linha_s {
    FiguraInterface vtable;
    double x1;
    double y1;
    double x2;
    double y2;
    char cor[20];
    int espessura;
    bool tracejado;
};

const char *linha_obter_tipo() {
    return "linha";
}

// Retorna um texto vazio. Precisa ser implementado para uma linha poder ser utilizado como uma
// figura genérica.
const char *linha_obter_id() {
    return "";
}

// Registra as funções do objeto linha na interface FiguraInterface.
static FiguraInterface linha_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, linha_obter_tipo);

    figura_registrar_escrever_informacoes(interface,
                                          (EscreverInformacoes *) linha_escrever_informacoes);
    figura_registrar_escrever_svg(interface, (EscreverSvg *) linha_escrever_svg);

    figura_registrar_obter_id(interface, (ObterId *) linha_obter_id);

    figura_registrar_obter_x(interface, (ObterX *) linha_obter_x);
    figura_registrar_obter_y(interface, (ObterY *) linha_obter_y);

    figura_registrar_obter_x_inicio(interface, (ObterXInicio *) linha_obter_x);
    figura_registrar_obter_y_inicio(interface, (ObterYInicio *) linha_obter_y);

    figura_registrar_obter_x_fim(interface, (ObterXFim *) linha_obter_x_fim);
    figura_registrar_obter_y_fim(interface, (ObterYFim *) linha_obter_y_fim);

    figura_registrar_obter_x_centro(interface, (ObterXCentro *) linha_obter_x_centro);
    figura_registrar_obter_y_centro(interface, (ObterYCentro *) linha_obter_y_centro);

    figura_registrar_obter_cor_borda(interface, (ObterCorBorda *) linha_obter_cor);
    figura_registrar_definir_cor_borda(interface, (DefinirCorBorda *) linha_definir_cor);

    figura_registrar_obter_cor_preenchimento(interface, (ObterCorPreenchimento *) linha_obter_cor);
    figura_registrar_definir_cor_preenchimento(interface,
                                               (DefinirCorPreenchimento *) linha_definir_cor);

    figura_registrar_destruir(interface, (Destruir *) linha_destruir);
    return interface;
}

// Cria e inicializa um struct Linha com os valores passados.
Linha linha_criar(double x1, double y1, double x2, double y2, const char cor[20]) {
    Linha linha = malloc(sizeof *linha);
    if (linha == NULL) {
        LOG_ERRO("Falha ao alocar memória\n");
        return NULL;
    }
    linha->x1 = x1;
    linha->y1 = y1;
    linha->x2 = x2;
    linha->y2 = y2;
    strcpy(linha->cor, cor);
    linha->tracejado = false;
    linha->espessura = 1;

    linha->vtable = linha_criar_interface_figura();
    return linha;
}

// Escreve todos os dados de uma linha em um arquivo.
void linha_escrever_informacoes(Linha linha, FILE *arquivo) {
    fprintf(arquivo, "tipo: %s, x1: %lf, y1: %lf, x2: %lf, y2: %lf, cor: %s\n",
            figura_obter_tipo(linha), linha->x1, linha->y1, linha->x2, linha->y2, linha->cor);
}

// Escreve o código svg que representa uma linha em um arquivo.
void linha_escrever_svg(Linha linha, FILE *arquivo) {
    fprintf(arquivo, "\t<line x1='%lf' y1='%lf' x2='%lf' y2='%lf' stroke='%s' stroke-width='%dpx'",
            linha->x1, linha->y1, linha->x2, linha->y2, linha->cor, linha->espessura);
    if (linha->tracejado)
        fprintf(arquivo, " stroke-dasharray=' 10 5'");
    fprintf(arquivo, " />\n");
}

// Retorna a coordenada x de uma linha.
double linha_obter_x(Linha linha) {
    return min(linha->x1, linha->x2);
}

// Retorna a coordenada y de uma linha.
double linha_obter_y(Linha linha) {
    return min(linha->y1, linha->y2);
}

// Retorna a coordenada x onde uma linha termina.
double linha_obter_x_fim(Linha linha) {
    return max(linha->x1, linha->x2);
}

// Retorna a coordenada y onde uma linha termina.
double linha_obter_y_fim(Linha linha) {
    return max(linha->y1, linha->y2);
}

// Retorna a coordenada x do centro de uma linha.
double linha_obter_x_centro(Linha linha) {
    return (linha->x1 + linha->x2) / 2;
}

// Retorna a coordenada y do centro de uma linha.
double linha_obter_y_centro(Linha linha) {
    return (linha->y1 + linha->y2) / 2;
}

// Retorna a cor de uma linha.
const char *linha_obter_cor(Linha linha) {
    return linha->cor;
}

// Define a cor de uma linha.
void linha_definir_cor(Linha linha, const char *cor) {
    if (cor == NULL) {
        LOG_AVISO("Não é possível definir NULL como cor de %s!\n", figura_obter_tipo(linha));
        return;
    }
    strcpy(linha->cor, cor);
}

// Define a espessura da borda de uma linha.
void linha_definir_espessura(Linha linha, int espessura) {
    if (espessura < 0) {
        LOG_AVISO("Não é possível definir uma espessura menor que 0 para uma %s!\n",
                  figura_obter_tipo(linha));
        return;
    }
    linha->espessura = espessura;
}

void linha_definir_tracejado(Linha linha, bool tracejado) {
    linha->tracejado = tracejado;
}

// Libera a memória alocada por uma linha.
void linha_destruir(Linha linha) {
    free(linha->vtable);
    free(linha);
}
