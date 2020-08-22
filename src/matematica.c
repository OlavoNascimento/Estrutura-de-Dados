#include "matematica.h"

#include <math.h>

// Retorna o maior valor de dois parâmetros
double max(double a, double b) {
    return a > b ? a : b;
}

// Retorna o menor valor de dois parâmetros
double min(double a, double b) {
    return a < b ? a : b;
}

// retorna a hipotenusa de dois catetos
double pitagoras(double cat1, double cat2) {
    double hipotenusa = sqrt(pow(cat1, 2) + pow(cat2, 2));
    return hipotenusa;
}
