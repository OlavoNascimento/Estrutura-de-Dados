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
    LinhaImp *linImp = malloc(sizeof(LinhaImp));
    linImp->x1 = x1;
    linImp->y1 = y1;
    linImp->x2 = x2;
    linImp->y2 = y2;
    strcpy(linImp->cor_borda, cor_borda);
    strcpy(linImp->cor_preenchimento, cor_preenchimento);
    return linImp;
}

// Escreve o código svg que representa uma linha em um arquivo.
void linha_escrever_svg(FILE *arquivo, Linha linha) {
    LinhaImp *linImp = (LinhaImp *) linha;
    fprintf(arquivo, "\t<line x1='%lf' y1='%lf' x2='%lf' y2='%lf' stroke='%s' fill='%s'/>\n",
            linImp->x1, linImp->y1, linImp->x2, linImp->y2, linImp->cor_borda,
            linImp->cor_preenchimento);
}

// Retorna a coordenada x1 de uma linha.
double linha_obter_x1(Linha linha) {
    LinhaImp *linImp = (LinhaImp *) linha;
    return linImp->x1;
}

// Define a coordenada x1 de uma linha.
void linha_definir_x1(Linha linha, double x1) {
    LinhaImp *linImp = (LinhaImp *) linha;
    linImp->x1 = x1;
}

// Retorna a coordenada y1 de uma linha.
double linha_obter_y1(Linha linha) {
    LinhaImp *linImp = (LinhaImp *) linha;
    return linImp->y1;
}

// Define a coordenada y1 de uma linha.
void linha_definir_y1(Linha linha, double y1) {
    LinhaImp *linImp = (LinhaImp *) linha;
    linImp->y1 = y1;
}

// Retorna a coordenada x2 de uma linha.
double linha_obter_x2(Linha linha) {
    LinhaImp *linImp = (LinhaImp *) linha;
    return linImp->x2;
}

// Define a coordenada x2 de uma linha.
void linha_definir_x2(Linha linha, double x2) {
    LinhaImp *linImp = (LinhaImp *) linha;
    linImp->x2 = x2;
}

// Retorna a coordenada y2 de uma linha.
double linha_obter_y2(Linha linha) {
    LinhaImp *linImp = (LinhaImp *) linha;
    return linImp->y2;
}

// Define a coordenada y2 de uma linha.
void linha_definir_y2(Linha linha, double y2) {
    LinhaImp *linImp = (LinhaImp *) linha;
    linImp->y2 = y2;
}

// Retorna a cor da borda de uma linha.
const char *linha_obter_cor_borda(Linha linha) {
    LinhaImp *linImp = (LinhaImp *) linha;
    return linImp->cor_borda;
}

// Define a cor da borda de uma linha.
void linha_definir_cor_borda(Linha linha, const char *cor_borda) {
    if (cor_borda == NULL) {
        fprintf(stderr, "ERRO: Não é possível definir NULL como cor da borda de uma linha!\n");
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
        fprintf(stderr,
                "ERRO: Não é possível definir NULL como cor de preenchimento de uma linha!\n");
        return;
    }
    LinhaImp *linImp = (LinhaImp *) linha;
    strcpy(linImp->cor_preenchimento, cor_preenchimento);
}

// Libera a memória alocada por uma linha.
void linha_destruir(Linha linha) {
    free(linha);
}
