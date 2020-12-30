#include "linha.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"
#include "../../Utils/matematica.h"

typedef struct {
    FiguraInterface vtable;
    double x1;
    double y1;
    double x2;
    double y2;
    char cor_borda[20];
    char cor_preenchimento[20];
    bool tracejado;
} LinhaImp;

const char *linha_obter_tipo() {
    return "linha";
}

// Retorna um texto vazio. Precisa ser implementado para uma linha poder ser utilizado como uma
// figura genérica.
const char *linha_obter_id() {
    return "";
}

// Conecta as funções do objeto linha com as da interface figura.
static FiguraInterface linha_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_escrever_informacoes(interface, linha_escrever_informacoes);
    figura_registrar_escrever_svg(interface, linha_escrever_svg);

    figura_registrar_obter_tipo(interface, linha_obter_tipo);

    figura_registrar_obter_id(interface, linha_obter_id);

    figura_registrar_obter_x(interface, linha_obter_x);
    figura_registrar_obter_y(interface, linha_obter_y);

    figura_registrar_obter_x_inicio(interface, linha_obter_x);
    figura_registrar_obter_y_inicio(interface, linha_obter_y);

    figura_registrar_obter_x_fim(interface, linha_obter_x_fim);
    figura_registrar_obter_y_fim(interface, linha_obter_y_fim);

    figura_registrar_obter_x_centro(interface, linha_obter_x_centro);
    figura_registrar_obter_y_centro(interface, linha_obter_y_centro);

    figura_registrar_obter_cor_borda(interface, linha_obter_cor_borda);
    figura_registrar_definir_cor_borda(interface, linha_definir_cor_borda);

    figura_registrar_obter_cor_preenchimento(interface, linha_obter_cor_preenchimento);
    figura_registrar_definir_cor_preenchimento(interface, linha_definir_cor_preenchimento);

    figura_registrar_destruir(interface, linha_destruir);
    return interface;
}

// Cria e inicializa um struct Linha com os valores passados.
Linha linha_criar(double x1, double y1, double x2, double y2, const char cor_borda[20],
                  const char cor_preenchimento[20], bool tracejado) {
    LinhaImp *linImp = malloc(sizeof(LinhaImp));
    linImp->x1 = x1;
    linImp->y1 = y1;
    linImp->x2 = x2;
    linImp->y2 = y2;
    strcpy(linImp->cor_borda, cor_borda);
    strcpy(linImp->cor_preenchimento, cor_preenchimento);
    linImp->tracejado = tracejado;

    linImp->vtable = linha_criar_interface_figura();
    return linImp;
}

// Escreve todos os dados de uma linha em um arquivo.
void linha_escrever_informacoes(Linha linha, FILE *arquivo) {
    LinhaImp *linhaImp = (LinhaImp *) linha;
    fprintf(arquivo, "tipo: %s, x1: %lf, y1: %lf, x2: %lf, y2: %lf, corb: %s, corp: %s\n",
            figura_obter_tipo(linha), linhaImp->x1, linhaImp->y1, linhaImp->x2, linhaImp->y2,
            linhaImp->cor_borda, linhaImp->cor_preenchimento);
}

// Escreve o código svg que representa uma linha em um arquivo.
void linha_escrever_svg(Linha linha, FILE *arquivo) {
    LinhaImp *linImp = (LinhaImp *) linha;
    fprintf(arquivo, "\t<line x1='%lf' y1='%lf' x2='%lf' y2='%lf' stroke='%s' fill='%s'",
            linImp->x1, linImp->y1, linImp->x2, linImp->y2, linImp->cor_borda,
            linImp->cor_preenchimento);
    if (linImp->tracejado == true) {
        fprintf(arquivo, " stroke-dasharray=' 10 5'");
    }
    fprintf(arquivo, " />\n");
}

// Retorna a coordenada x de uma linha.
double linha_obter_x(Linha linha) {
    LinhaImp *linImp = (LinhaImp *) linha;
    return min(linImp->x1, linImp->x2);
}

// Retorna a coordenada y de uma linha.
double linha_obter_y(Linha linha) {
    LinhaImp *linImp = (LinhaImp *) linha;
    return min(linImp->y1, linImp->y2);
}

// Retorna a coordenada x onde uma linha termina.
double linha_obter_x_fim(Linha linha) {
    LinhaImp *linImp = (LinhaImp *) linha;
    return max(linImp->x1, linImp->x2);
}

// Retorna a coordenada y onde uma linha termina.
double linha_obter_y_fim(Linha linha) {
    LinhaImp *linImp = (LinhaImp *) linha;
    return max(linImp->y1, linImp->y2);
}

// Retorna a coordenada x do centro de uma linha.
double linha_obter_x_centro(Linha linha) {
    LinhaImp *linImp = (LinhaImp *) linha;
    return (linImp->x1 + linImp->x2) / 2;
}

// Retorna a coordenada y do centro de uma linha.
double linha_obter_y_centro(Linha linha) {
    LinhaImp *linImp = (LinhaImp *) linha;
    return (linImp->y1 + linImp->y2) / 2;
}

// Retorna a cor da borda de uma linha.
const char *linha_obter_cor_borda(Linha linha) {
    LinhaImp *linImp = (LinhaImp *) linha;
    return linImp->cor_borda;
}

// Define a cor da borda de uma linha.
void linha_definir_cor_borda(Linha linha, const char *cor_borda) {
    if (cor_borda == NULL) {
        LOG_ERRO("Não é possível definir NULL como cor da borda de %s!\n",
                 figura_obter_tipo(linha));
        return;
    }
    LinhaImp *linImp = (LinhaImp *) linha;
    strcpy(linImp->cor_borda, cor_borda);
}

// Retorna a cor de preenchimento de uma linha..
const char *linha_obter_cor_preenchimento(Linha linha) {
    LinhaImp *linImp = (LinhaImp *) linha;
    return linImp->cor_preenchimento;
}

// Define a cor de preenchimento de uma linha.
void linha_definir_cor_preenchimento(Linha linha, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        LOG_ERRO("Não é possível definir NULL como cor de preenchimento de %s!\n",
                 figura_obter_tipo(linha));
        return;
    }
    LinhaImp *linImp = (LinhaImp *) linha;
    strcpy(linImp->cor_preenchimento, cor_preenchimento);
}

// Libera a memória alocada por uma linha.
void linha_destruir(Linha linha) {
    LinhaImp *linImp = (LinhaImp *) linha;
    free(linImp->vtable);
    free(linImp);
}
