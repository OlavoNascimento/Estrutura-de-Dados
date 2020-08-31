
#include "quadra.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "retangulo.h"

typedef struct {
    Retangulo ret;
} QuadraImp;

Quadra quadra_criar(char id[100], double largura, double altura, double x, double y,
                    char cor_borda[20], char cor_preenchimento[20]) {
    QuadraImp *quadImp = malloc(sizeof(QuadraImp));
    quadImp->ret = retangulo_criar(id, largura, altura, x, y, cor_borda, cor_preenchimento);
    return quadImp;
}

Quadra quadra_ler(const char *linha) {
    char id[100];
    double largura;
    double altura;
    double x;
    double y;
    char cor_borda[20] = "saddlebrown";
    char cor_preenchimento[20] = "coral";
    sscanf(linha, "%*s %s %lf %lf %lf %lf", id, &x, &y, &largura, &altura);
    return quadra_criar(id, largura, altura, x, y, cor_borda, cor_preenchimento);
}

// Retorna o id de uma quadra
const char *quadra_obter_id(Quadra qua) {
    QuadraImp *quaImp = (QuadraImp *) qua;
    return retangulo_obter_id(quaImp->ret);
}

// Escreve as informações de uma radio base
void quadra_escrever_informacoes(FILE *arquivo, Quadra quad) {
    QuadraImp *quadImp = (QuadraImp *) quad;
    retangulo_escrever_informacoes(arquivo, quadImp->ret);
}

// Escreve no svg as informações de uma radio base
void quadra_escrever_svg(FILE *arquivo, Quadra quad) {
    QuadraImp *quadImp = (QuadraImp *) quad;
    retangulo_escrever_svg(arquivo, quadImp->ret);
}

// Retorna a coordenada x de uma quadra.
double quadra_obter_x(Quadra quad) {
    QuadraImp *quadImp = (QuadraImp *) quad;
    return retangulo_obter_x(quadImp->ret);
}

// Retorna a coordenada y de uma quadra.
double quadra_obter_y(Quadra quad) {
    QuadraImp *quadImp = (QuadraImp *) quad;
    return retangulo_obter_y(quadImp->ret);
}

double quadra_obter_largura(Quadra quad) {
    QuadraImp *quadImp = (QuadraImp *) quad;
    return retangulo_obter_largura(quadImp->ret);
}

double quadra_obter_altura(Quadra quad) {
    QuadraImp *quadImp = (QuadraImp *) quad;
    return retangulo_obter_altura(quadImp->ret);
}

const char *quadra_obter_cor_preenchimento(Quadra quad) {
    QuadraImp *quadImp = (QuadraImp *) quad;
    return retangulo_obter_cor_preenchimento(quadImp->ret);
}

void quadra_definir_cor_preenchimento(Quadra quad, const char *cor_preenchimento) {
    QuadraImp *quadImp = (QuadraImp *) quad;
    retangulo_definir_cor_preenchimento(quadImp->ret, cor_preenchimento);
}

const char *quadra_obter_cor_borda(Quadra quad) {
    QuadraImp *quadImp = (QuadraImp *) quad;
    return retangulo_obter_cor_borda(quadImp->ret);
}

void quadra_definir_cor_borda(Quadra quad, const char *cor_borda) {
    QuadraImp *quadImp = (QuadraImp *) quad;
    retangulo_definir_cor_borda(quadImp->ret, cor_borda);
}

void quadra_definir_espessura_borda(Quadra quad, int espessura_borda) {
    QuadraImp *quadImp = (QuadraImp *) quad;
    retangulo_definir_espessura_borda(quadImp->ret, espessura_borda);
}

void quadra_definir_arredondamento_borda(Quadra quad, double raio_borda) {
    QuadraImp *quadImp = (QuadraImp *) quad;
    retangulo_definir_arredondamento_borda(quadImp->ret, raio_borda);
}

void quadra_destruir(Quadra quad) {
    QuadraImp *quadImp = (QuadraImp *) quad;
    retangulo_destruir(quadImp->ret);
    free(quad);
}
