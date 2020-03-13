#include <stdio.h>

#include <circulo.h>

Circulo criar_circulo(char *linha) {
    Circulo circ;
    sscanf(linha, "%*c %s %d %d %d %s %s",
        circ.id, &circ.raio, &circ.x, &circ.y, circ.cor_borda, circ.cor_preenchimento);
    return circ;
}

void circulo_para_svg(FILE *arquivo, Circulo circ) {
    fprintf(arquivo,
            "   <circle id='%s' r='%d' cx='%d' cy='%d' stroke='%s' fill='%s'/>\n",
            circ.id,
            circ.raio,
            circ.x,
            circ.y,
            circ.cor_borda,
            circ.cor_preenchimento
    );
}