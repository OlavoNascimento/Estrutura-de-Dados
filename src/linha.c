#include "linha.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    double x1;
    double y1;
    double x2;
    double y2;
    char cor_borda[20];
    char cor_preenchimento[20];
} LinhaImp;

// Cria e inicializa um struct Linha com os valores passados.
Linha linha_criar(double x1, double y1, double x2, double y2, const char cor_borda[20],
                  const char cor_preenchimento[20]) {
    LinhaImp *lin = malloc(sizeof(LinhaImp));
    lin->x1 = x1;
    lin->y1 = y1;
    lin->x2 = x2;
    lin->y2 = y2;
    strcpy(lin->cor_borda, cor_borda);
    strcpy(lin->cor_preenchimento, cor_preenchimento);
    return lin;
}

// Escreve o código svg que representa uma linha em um arquivo.
void linha_escrever_svg(FILE *arquivo, Linha lin) {
    LinhaImp *linImp = (LinhaImp *) lin;
    fprintf(arquivo, "\t<line x1='%lf' y1='%lf' x2='%lf' y2='%lf' stroke='%s' fill='%s'/>\n",
            linImp->x1, linImp->y1, linImp->x2, linImp->y2, linImp->cor_borda,
            linImp->cor_preenchimento);
}

double linha_obter_x1(Linha lin) {
    LinhaImp *linImp = (LinhaImp *) lin;
    return linImp->x1;
}

void linha_definir_x1(Linha lin, double x1) {
    LinhaImp *linImp = (LinhaImp *) lin;
    linImp->x1 = x1;
}

double linha_obter_y1(Linha lin) {
    LinhaImp *linImp = (LinhaImp *) lin;
    return linImp->y1;
}

void linha_definir_y1(Linha lin, double y1) {
    LinhaImp *linImp = (LinhaImp *) lin;
    linImp->y1 = y1;
}

double linha_obter_x2(Linha lin) {
    LinhaImp *linImp = (LinhaImp *) lin;
    return linImp->x2;
}

void linha_definir_x2(Linha lin, double x2) {
    LinhaImp *linImp = (LinhaImp *) lin;
    linImp->x2 = x2;
}

double linha_obter_y2(Linha lin) {
    LinhaImp *linImp = (LinhaImp *) lin;
    return linImp->y2;
}

void linha_definir_y2(Linha lin, double y2) {
    LinhaImp *linImp = (LinhaImp *) lin;
    linImp->y2 = y2;
}

const char *linha_obter_cor_borda(Linha lin) {
    LinhaImp *linImp = (LinhaImp *) lin;
    return linImp->cor_borda;
}

void linha_definir_cor_borda(Linha lin, const char *cor_borda) {
    if (cor_borda == NULL) {
        fprintf(stderr, "ERRO: Não é possível definir null como cor da borda de uma linha!\n");
        return;
    }
    LinhaImp *linImp = (LinhaImp *) lin;
    strcpy(linImp->cor_borda, cor_borda);
}

const char *linha_obter_cor_preenchimento(Linha lin) {
    LinhaImp *linImp = (LinhaImp *) lin;
    return linImp->cor_preenchimento;
}

void linha_definir_cor_preenchimento(Linha lin, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        fprintf(stderr,
                "ERRO: Não é possível definir null como cor de preenchimento de uma linha!\n");
        return;
    }
    LinhaImp *linImp = (LinhaImp *) lin;
    strcpy(linImp->cor_preenchimento, cor_preenchimento);
}

void linha_destruir(Linha lin) {
    free(lin);
}
