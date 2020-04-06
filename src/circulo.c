#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <circulo.h>

Circulo criar_circulo(char *linha) {
    Circulo circ;
    sscanf(linha, "%*c %s %lf %lf %lf %s %s",
        circ.id, &circ.raio, &circ.x, &circ.y, circ.cor_borda, circ.cor_preenchimento);
    return circ;
}

void circulo_para_svg(FILE *arquivo, Circulo circ) {
    fprintf(arquivo, "\t<circle");
    if(strlen(circ.id) > 0)
        fprintf(arquivo, " id='%s'", circ.id);
    fprintf(arquivo,
            " r='%lf' cx='%lf' cy='%lf' stroke='%s' fill='%s'/>\n",
            circ.raio,
            circ.x,
            circ.y,
            circ.cor_borda,
            circ.cor_preenchimento
    );
}

bool interseccao_circulos(Circulo circ1, Circulo circ2) {
    double dist = (circ1.x - circ2.x) * (circ1.x - circ2.x) +
                  (circ1.y - circ2.y) * (circ1.y - circ2.y);
    double raios = (circ1.raio + circ2.raio) * (circ1.raio + circ2.raio);
    if(dist > raios)
        return false;
    return true;
}

bool ponto_interno_circulo(Circulo circ, double ponto_x, double ponto_y) {
    if(ponto_x <= circ.x - circ.raio || ponto_x >= circ.x + circ.raio)
        return false;
    if(ponto_y <= circ.y - circ.raio || ponto_y >= circ.y + circ.raio)
        return false;
    return true;
}
