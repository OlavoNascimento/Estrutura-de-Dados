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
    double dx =
        figura_obter_x(circulo) -
        max(figura_obter_x(retangulo), min(figura_obter_x(circulo), figura_obter_x_fim(retangulo)));
    double dy =
        figura_obter_y(circulo) -
        max(figura_obter_y(retangulo), min(figura_obter_y(circulo), figura_obter_y_fim(retangulo)));
    return pow(dx, 2) + pow(dy, 2) <= pow(circulo_obter_raio(circulo), 2);
}

// Retorna verdadeiro caso um circulo contem um retângulo.
bool circulo_contem_retangulo(Circulo circulo, Retangulo retangulo) {
    // Distância entre o círculo e o ponto x mais longe do círculo
    double dx = max(figura_obter_x(circulo) - figura_obter_x_inicio(retangulo),
                    figura_obter_x_fim(retangulo) - figura_obter_x(circulo));
    // Distância entre o círculo e o ponto y mais longe do círculo
    double dy = max(figura_obter_y(circulo) - figura_obter_y_inicio(retangulo),
                    figura_obter_y_fim(retangulo) - figura_obter_y(circulo));
    return pow(dx, 2) + pow(dy, 2) < pow(circulo_obter_raio(circulo), 2);
}
