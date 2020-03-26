#include <stdio.h>

#include <circulo.h>

Circulo criar_circulo(char *linha) {
    Circulo circ;
    sscanf(linha, "%*c %s %lf %lf %lf %s %s",
        circ.id, &circ.raio, &circ.x, &circ.y, circ.cor_borda, circ.cor_preenchimento);
    return circ;
}

void circulo_para_svg(FILE *arquivo, Circulo circ) {
    fprintf(arquivo,
            "\t<circle id='%s' r='%lf' cx='%lf' cy='%lf' stroke='%s' fill='%s'/>\n",
            circ.id,
            circ.raio,
            circ.x,
            circ.y,
            circ.cor_borda,
            circ.cor_preenchimento
    );
}