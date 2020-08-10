#include "radioBase.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "circulo.h"

typedef struct {
    Circulo circ;
} RadioBaseImp;

//Cria e inicializa um struct RadioBaseImpl com os valores passados.
Radio radio_criar(char id[100], double raio, double x, double y, char cor_borda[20],
                  char cor_preenchimento[20]) {
    RadioBaseImp *radImp = malloc(sizeof(RadioBaseImp));
    radImp->circ = circulo_criar(id, raio, x, y, cor_borda, cor_preenchimento);
    return radImp;
}

Radio radio_ler(const char *linha) {
    char id[100];
    double raio = 10;  //valor simbólico, padrão para todos os hidrantes
    double x;
    double y;
    char cor_borda[20] = "none";            //será alterado com outro comando
    char cor_preenchimento[20] = "purple";  //será alterado com outro comando
    sscanf(linha, "%*c %s %lf %lf", id, &x, &y);
    return radio_criar(id, raio, x, y, cor_borda, cor_preenchimento);
}

void radio_definir_cor_preenchimento(Radio rad, const char *cor_preenchimento) {
    RadioBaseImp *radImp = (RadioBaseImp *)rad;
    circulo_definir_cor_preenchimento(radImp->circ, cor_preenchimento);
}

void radio_definir_cor_borda(Radio rad, const char *cor_borda) {
    RadioBaseImp *radImp = (RadioBaseImp *)rad;
    circulo_definir_cor_borda(radImp->circ, cor_borda);
}