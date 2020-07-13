#include <stdio.h>

#include <linha.h>

typedef struct {
    double x1;
    double y1;
    double x2;
    double y2;
    char cor_borda[20];
    char cor_preenchimento[20];
} Linha;

// Escreve o código svg que representa uma linha em um arquivo.
void linha_escrever_svg(FILE *arquivo, Linha lin) {
    fprintf(arquivo,
            "\t<line x1='%lf' y1='%lf' x2='%lf' y2='%lf' stroke='%s' fill='%s'/>\n",
            lin.x1,
            lin.y1,
            lin.x2,
            lin.y2,
            lin.cor_borda,
            lin.cor_preenchimento);
}

double linha_obter_x1(Linha lin) {
    return lin.x1;
}

double linha_obter_y1(Linha lin) {
    return lin.y1;
}

double linha_obter_x2(Linha lin) {
    return lin.x2;
}

double linha_obter_y2(Linha lin) {
    return lin.y2;
}

char linha_obter_cor_borda(Linha lin) {
    return lin.cor_borda;
}

char linha_obter_cor_preenchimento(Linha lin) {
    return lin.cor_preenchimento;
}
