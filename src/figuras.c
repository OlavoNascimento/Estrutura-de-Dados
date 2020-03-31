#include <stdbool.h>

#include <figuras.h>
#include <retangulo.h>
#include <circulo.h>

double max(double a, double b) {
    return a > b ? a : b;
}

double min(double a, double b) {
    return a < b ? a : b;
}

bool retangulos_interseccao(Retangulo ret1, Retangulo ret2) {
    if(ret1.x > ret2.x + ret2.largura || ret2.x > ret1.x + ret1.largura)
        return false;
    if(ret1.y + ret1.altura < ret2.y || ret2.y + ret2.altura < ret1.y)
        return false;
    return true;
}

bool circulos_interseccao(Circulo circ1, Circulo circ2) {
    double dist = (circ1.x - circ2.x) * (circ1.x - circ2.x) +
                  (circ1.y - circ2.y) * (circ1.y - circ2.y);
    double raios = (circ1.raio + circ2.raio) * (circ1.raio + circ2.raio);
    if(dist > raios)
        return false;
    return true;
}

bool circulo_retangulo_interseccao(Circulo circ, Retangulo ret) {
    double deltaX = circ.x - max(ret.x, min(circ.x, ret.x + ret.largura));
    double deltaY = circ.y - max(ret.y, min(circ.y, ret.y + ret.altura));
    return (deltaX * deltaX + deltaY * deltaY) <= (circ.raio * circ.raio);
}
