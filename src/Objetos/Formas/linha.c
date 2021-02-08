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
    char cor_borda[20];
    char cor_preenchimento[20];
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

// Conecta as funções do objeto linha com as da interface figura.
static FiguraInterface linha_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, (void *) linha_obter_tipo);

    figura_registrar_escrever_informacoes(interface, (void *) linha_escrever_informacoes);
    figura_registrar_escrever_svg(interface, (void *) linha_escrever_svg);

    figura_registrar_obter_id(interface, (void *) linha_obter_id);

    figura_registrar_obter_x(interface, (void *) linha_obter_x);
    figura_registrar_obter_y(interface, (void *) linha_obter_y);

    figura_registrar_obter_x_inicio(interface, (void *) linha_obter_x);
    figura_registrar_obter_y_inicio(interface, (void *) linha_obter_y);

    figura_registrar_obter_x_fim(interface, (void *) linha_obter_x_fim);
    figura_registrar_obter_y_fim(interface, (void *) linha_obter_y_fim);

    figura_registrar_obter_x_centro(interface, (void *) linha_obter_x_centro);
    figura_registrar_obter_y_centro(interface, (void *) linha_obter_y_centro);

    figura_registrar_obter_cor_borda(interface, (void *) linha_obter_cor_borda);
    figura_registrar_definir_cor_borda(interface, (void *) linha_definir_cor_borda);

    figura_registrar_obter_cor_preenchimento(interface, (void *) linha_obter_cor_preenchimento);
    figura_registrar_definir_cor_preenchimento(interface, (void *) linha_definir_cor_preenchimento);

    figura_registrar_destruir(interface, (void *) linha_destruir);
    return interface;
}

// Cria e inicializa um struct Linha com os valores passados.
Linha linha_criar(double x1, double y1, double x2, double y2, const char cor_borda[20],
                  const char cor_preenchimento[20], bool tracejado) {
    Linha linha = malloc(sizeof *linha);
    if (linha == NULL) {
        fprintf(stderr, "Erro ao alocar memória para uma nova linha!\n");
        return NULL;
    }
    linha->x1 = x1;
    linha->y1 = y1;
    linha->x2 = x2;
    linha->y2 = y2;
    strcpy(linha->cor_borda, cor_borda);
    strcpy(linha->cor_preenchimento, cor_preenchimento);
    linha->tracejado = tracejado;

    linha->vtable = linha_criar_interface_figura();
    return linha;
}

// Escreve todos os dados de uma linha em um arquivo.
void linha_escrever_informacoes(Linha linha, FILE *arquivo) {
    fprintf(arquivo, "tipo: %s, x1: %lf, y1: %lf, x2: %lf, y2: %lf, corb: %s, corp: %s\n",
            figura_obter_tipo(linha), linha->x1, linha->y1, linha->x2, linha->y2, linha->cor_borda,
            linha->cor_preenchimento);
}

// Escreve o código svg que representa uma linha em um arquivo.
void linha_escrever_svg(Linha linha, FILE *arquivo) {
    fprintf(arquivo, "\t<line x1='%lf' y1='%lf' x2='%lf' y2='%lf' stroke='%s' fill='%s'", linha->x1,
            linha->y1, linha->x2, linha->y2, linha->cor_borda, linha->cor_preenchimento);
    if (linha->tracejado == true) {
        fprintf(arquivo, " stroke-dasharray=' 10 5'");
    }
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

// Retorna a cor da borda de uma linha.
const char *linha_obter_cor_borda(Linha linha) {
    return linha->cor_borda;
}

// Define a cor da borda de uma linha.
void linha_definir_cor_borda(Linha linha, const char *cor_borda) {
    if (cor_borda == NULL) {
        LOG_ERRO("Não é possível definir NULL como cor da borda de %s!\n",
                 figura_obter_tipo(linha));
        return;
    }
    strcpy(linha->cor_borda, cor_borda);
}

// Retorna a cor de preenchimento de uma linha..
const char *linha_obter_cor_preenchimento(Linha linha) {
    return linha->cor_preenchimento;
}

// Define a cor de preenchimento de uma linha.
void linha_definir_cor_preenchimento(Linha linha, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        LOG_ERRO("Não é possível definir NULL como cor de preenchimento de %s!\n",
                 figura_obter_tipo(linha));
        return;
    }
    strcpy(linha->cor_preenchimento, cor_preenchimento);
}

// Libera a memória alocada por uma linha.
void linha_destruir(Linha linha) {
    free(linha->vtable);
    free(linha);
}
