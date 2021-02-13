#include "matematica.h"

#include <math.h>

#include "../Interfaces/figura.h"
#include "../Objetos/Formas/circulo.h"
#include "../Objetos/Formas/retangulo.h"

// Retorna o maior valor de dois parâmetros
double max(double a, double b) {
    return a > b ? a : b;
}

// Retorna o menor valor de dois parâmetros
double min(double a, double b) {
    return a < b ? a : b;
}

// Retorna verdadeiro caso um circulo e um retângulo se intersectem.
bool circulo_intersecta_retangulo(Circulo circulo, Retangulo retangulo) {
    const double ret_x_fim = retangulo_obter_x(retangulo) + retangulo_obter_largura(retangulo);
    const double ret_y_fim = retangulo_obter_y(retangulo) + retangulo_obter_altura(retangulo);

    double dx = circulo_obter_x(circulo) -
                max(retangulo_obter_x(retangulo), min(circulo_obter_x(circulo), ret_x_fim));
    double dy = circulo_obter_y(circulo) -
                max(retangulo_obter_y(retangulo), min(circulo_obter_y(circulo), ret_y_fim));
    return pow(dx, 2) + pow(dy, 2) <= pow(circulo_obter_raio(circulo), 2);
}

// Retorna verdadeiro caso um circulo contem um retângulo.
bool circulo_contem_retangulo(Circulo circulo, Retangulo retangulo) {
    const double ret_x_fim = retangulo_obter_x(retangulo) + retangulo_obter_largura(retangulo);
    const double ret_y_fim = retangulo_obter_y(retangulo) + retangulo_obter_altura(retangulo);

    // Distância entre o círculo e o ponto x mais longe do círculo
    double dx = max(circulo_obter_x(circulo) - retangulo_obter_x(retangulo),
                    ret_x_fim - circulo_obter_x(circulo));
    // Distância entre o círculo e o ponto y mais longe do círculo
    double dy = max(circulo_obter_y(circulo) - retangulo_obter_y(retangulo),
                    ret_y_fim - circulo_obter_y(circulo));
    return pow(dx, 2) + pow(dy, 2) < pow(circulo_obter_raio(circulo), 2);
}
