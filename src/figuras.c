#include <stdbool.h>
#include <string.h>

#include <figuras.h>
#include <retangulo.h>
#include <circulo.h>

#define MARGEM_CONTORNO 2
#define TRACEJADO_TAMANHO 2

char* fig_tipo_para_string(TiposFigura tipo) {
    char *valores[] = {
        "retângulo", "círculo", "texto"
    };
    return valores[tipo];
}

double max(double a, double b) {
    return a > b ? a : b;
}

double min(double a, double b) {
    return a < b ? a : b;
}

bool interseccao_retangulos(Retangulo ret1, Retangulo ret2) {
    if(ret1.x > ret2.x + ret2.largura || ret2.x > ret1.x + ret1.largura)
        return false;
    if(ret1.y + ret1.altura < ret2.y || ret2.y + ret2.altura < ret1.y)
        return false;
    return true;
}

bool interseccao_circulos(Circulo circ1, Circulo circ2) {
    double dist = (circ1.x - circ2.x) * (circ1.x - circ2.x) +
                  (circ1.y - circ2.y) * (circ1.y - circ2.y);
    double raios = (circ1.raio + circ2.raio) * (circ1.raio + circ2.raio);
    if(dist > raios)
        return false;
    return true;
}

bool interseccao_circulo_retangulo(Circulo circ, Retangulo ret) {
    double deltaX = circ.x - max(ret.x, min(circ.x, ret.x + ret.largura));
    double deltaY = circ.y - max(ret.y, min(circ.y, ret.y + ret.altura));
    return (deltaX * deltaX + deltaY * deltaY) <= (circ.raio * circ.raio);
}

bool interseccao_figuras(Figuras fig1, TiposFigura tipo1, Figuras fig2, TiposFigura tipo2) {
    bool intersectam = false;
    if(tipo1 == TipoRetangulo && tipo1 == tipo2)
        intersectam = interseccao_retangulos(fig1.ret, fig2.ret);
    else if(tipo1 == TipoCirculo && tipo1 == tipo2)
        intersectam = interseccao_circulos(fig1.circ, fig2.circ);
    else if(tipo1 == TipoCirculo && tipo2 == TipoRetangulo)
        intersectam = interseccao_circulo_retangulo(fig1.circ, fig2.ret);
    else if(tipo1 == TipoRetangulo && tipo2 == TipoCirculo)
        intersectam = interseccao_circulo_retangulo(fig2.circ, fig1.ret);

    return intersectam;
}

void envolver_retangulos(Retangulo *contorno, Retangulo ret1, Retangulo ret2) {
    contorno->x = min(ret1.x, ret2.x);
    contorno->y = min(ret1.y, ret2.y);
    contorno->largura = max(ret1.x+ret1.largura, ret2.x+ret2.largura) - contorno->x;
    contorno->altura = max(ret1.y+ret1.altura, ret2.y+ret2.altura) - contorno->y;
}

void envolver_circulos(Retangulo *contorno, Circulo circ1, Circulo circ2) {
    contorno->x = min(circ1.x-circ1.raio, circ2.x-circ2.raio);
    contorno->y = min(circ1.y-circ1.raio, circ2.y-circ2.raio);
    contorno->largura = max(circ1.x+circ1.raio, circ2.x+circ2.raio) - contorno->x;
    contorno->altura = max(circ1.y+circ1.raio, circ2.y+circ2.raio) - contorno->y;
}

void envolver_circulo_retangulo(Retangulo *contorno, Circulo circ, Retangulo ret) {
    contorno->x = min(ret.x, circ.x-circ.raio);
    contorno->y = min(ret.y, circ.y-circ.raio);
    contorno->largura = max(ret.x+ret.largura, circ.x+circ.raio) - contorno->x;
    contorno->altura = max(ret.y+ret.altura, circ.y+circ.raio) - contorno->y;
}

Retangulo envolver_figuras(Figuras fig1, TiposFigura tipo1, Figuras fig2, TiposFigura tipo2) {
    Retangulo contorno = {
        .cor_borda = "black",
        .cor_preenchimento = "none",
        .tracejado_tamanho = TRACEJADO_TAMANHO,
        .tracejado_espaco = TRACEJADO_TAMANHO
    };

    char *id1, *id2;
    if(tipo1 == TipoRetangulo && tipo1 == tipo2) {
        envolver_retangulos(&contorno, fig1.ret, fig2.ret);
        id1 = fig1.ret.id;
        id2 = fig2.ret.id;
    } else if(tipo1 == TipoCirculo && tipo1 == tipo2) {
        envolver_circulos(&contorno, fig1.circ, fig2.circ);
        id1 = fig1.circ.id;
        id2 = fig2.circ.id;
    } else if(tipo1 == TipoCirculo && tipo2 == TipoRetangulo) {
        envolver_circulo_retangulo(&contorno, fig1.circ, fig2.ret);
        id1 = fig1.circ.id;
        id2 = fig2.ret.id;
    } else if(tipo1 == TipoRetangulo && tipo2 == TipoCirculo) {
        envolver_circulo_retangulo(&contorno, fig2.circ, fig1.ret);
        id1 = fig1.ret.id;
        id2 = fig2.circ.id;
    }

    char id[100];
    sprintf(id, "contorno_%s_%s", id1, id2);
    strcpy(contorno.id, id);

    contorno.x -= MARGEM_CONTORNO;
    contorno.y -= MARGEM_CONTORNO;
    contorno.largura += 2 * MARGEM_CONTORNO;
    contorno.altura += 2 * MARGEM_CONTORNO;
    return contorno;
}
