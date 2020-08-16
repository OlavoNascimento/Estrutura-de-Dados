#include "radio_base.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "circulo.h"

typedef struct {
    Circulo circ;
} RadioBaseImp;

// Cria e inicializa um struct RadioBaseImp com os valores passados.
Radio radio_criar(char id[100], double raio, double x, double y, char cor_borda[20],
                  char cor_preenchimento[20]) {
    RadioBaseImp *radImp = malloc(sizeof(RadioBaseImp));
    radImp->circ = circulo_criar(id, raio, x, y, cor_borda, cor_preenchimento);
    return radImp;
}

Radio radio_ler(const char *linha) {
    char id[100];
    double raio = 10;  // valor simbólico, padrão para todos os hidrantes
    double x;
    double y;
    char cor_borda[20] = "purple";          // será alterado com outro comando
    char cor_preenchimento[20] = "purple";  // será alterado com outro comando
    sscanf(linha, "%*s %s %lf %lf", id, &x, &y);
    return radio_criar(id, raio, x, y, cor_borda, cor_preenchimento);
}

// Escreve as informações de uma radio base
void radio_escrever_informacoes(FILE *arquivo, Radio rad) {
    RadioBaseImp *radImp = (RadioBaseImp *) rad;
    circulo_escrever_informacoes(arquivo, radImp->circ);
}

// Escreve no svg as informações de uma radio base
void radio_escrever_svg(FILE *arquivo, Radio rad) {
    RadioBaseImp *radImp = (RadioBaseImp *) rad;
    circulo_escrever_svg(arquivo, radImp->circ);
}

// Retorna a coordenada y de uma radio base. A radio base é considerada apenas um ponto.
double radio_obter_y(Radio rad) {
    RadioBaseImp *radImp = (RadioBaseImp *) rad;
    return circulo_obter_y(radImp->circ);
}

// Retorna a coordenada y de uma radio base. A radio base é considerada apenas um ponto.
double radio_obter_x(Radio rad) {
    RadioBaseImp *radImp = (RadioBaseImp *) rad;
    return circulo_obter_x(radImp->circ);
}

const char *radio_obter_cor_preenchimento(Radio rad) {
    RadioBaseImp *radImp = (RadioBaseImp *) rad;
    return circulo_obter_cor_preenchimento(radImp->circ);
}

void radio_definir_cor_preenchimento(Radio rad, const char *cor_preenchimento) {
    RadioBaseImp *radImp = (RadioBaseImp *) rad;
    circulo_definir_cor_preenchimento(radImp->circ, cor_preenchimento);
}

const char *radio_obter_cor_borda(Radio rad) {
    RadioBaseImp *radImp = (RadioBaseImp *) rad;
    return circulo_obter_cor_borda(radImp->circ);
}

void radio_definir_cor_borda(Radio rad, const char *cor_borda) {
    RadioBaseImp *radImp = (RadioBaseImp *) rad;
    circulo_definir_cor_borda(radImp->circ, cor_borda);
}

// Retorna a espessura da borda de uma rádio base.
int radio_obter_espessura_borda(Radio rad) {
    RadioBaseImp *radImp = (RadioBaseImp *) rad;
    return circulo_obter_espessura_borda(radImp->circ);
}

// Define a espessura da borda de uma rádio base.
void radio_definir_espessura_borda(Radio rad, int espessura_borda) {
    RadioBaseImp *radImp = (RadioBaseImp *) rad;
    circulo_definir_espessura_borda(radImp->circ, espessura_borda);
}

// Libera a memória alocada por uma radio base.
void radio_destruir(Radio rad) {
    RadioBaseImp *radImp = (RadioBaseImp *) rad;
    circulo_destruir(radImp->circ);
    free(rad);
}
