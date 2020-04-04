#include <stdio.h>

#include <linha.h>

void linha_para_svg(FILE *arquivo, Linha lin) {
    fprintf(arquivo,
            "\t<line x1='%lf' y1='%lf' x2='%lf' y2='%lf' stroke='%s' fill='%s'/>\n",
            lin.x1,
            lin.y1,
            lin.x2,
            lin.y2,
            lin.cor_borda,
            lin.cor_preenchimento
    );
}