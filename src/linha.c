#include <stdio.h>

#include <linha.h>

// Escreve o código svg que representa uma linha em um arquivo.
void escrever_svg_linha(FILE *arquivo, Linha lin) {
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
