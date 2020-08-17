#include "hidrante.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "circulo.h"

typedef struct {
    Circulo circ;
} HidranteImp;

// Cria e inicializa um struct HidranteImp com os valores passados.
Hidrante hidrante_criar(char id[100], double raio, double x, double y, char cor_borda[20],
                        char cor_preenchimento[20]) {
    HidranteImp *hidImp = malloc(sizeof(HidranteImp));
    hidImp->circ = circulo_criar(id, raio, x, y, cor_borda, cor_preenchimento);
    return hidImp;
}

// Cria um hidrante com base em informações de uma linha.
Hidrante hidrante_ler(const char *linha) {
    char id[100];
    double raio = 10;  // valor simbólico, padrão para todos os hidrantes
    double x;
    double y;
    char cor_borda[20] = "red";          // será alterado com outro comando
    char cor_preenchimento[20] = "red";  // será alterado com outro comando
    sscanf(linha, "%*s %s %lf %lf", id, &x, &y);
    return hidrante_criar(id, raio, x, y, cor_borda, cor_preenchimento);
}

// TODO Definir obter_id

// escreve as informações de um hidrante
void hidrante_escrever_informacoes(FILE *arquivo, Hidrante hid) {
    HidranteImp *hidImp = (HidranteImp *) hid;
    circulo_escrever_informacoes(arquivo, hidImp->circ);
}

// escreve no svg as informações de um hidrante
void hidrante_escrever_svg(FILE *arquivo, Hidrante hid) {
    HidranteImp *hidImp = (HidranteImp *) hid;
    circulo_escrever_svg(arquivo, hidImp->circ);
}

// Retorna a coordenada y de um hidrante. O hidrante é considerado apenas um ponto.
double hidrante_obter_y(Hidrante hid) {
    HidranteImp *hidImp = (HidranteImp *) hid;
    return circulo_obter_y(hidImp->circ);
}

// Retorna a coordenada x de um hidrante. O hidrante é considerado apenas um ponto.
double hidrante_obter_x(Hidrante hid) {
    HidranteImp *hidImp = (HidranteImp *) hid;
    return circulo_obter_x(hidImp->circ);
}

const char *hidrante_obter_cor_preenchimento(Hidrante hid) {
    HidranteImp *hidImp = (HidranteImp *) hid;
    return circulo_obter_cor_preenchimento(hidImp->circ);
}

// Define a cor de preenchimento de um hidrante.
void hidrante_definir_cor_preenchimento(Hidrante hid, const char *cor_preenchimento) {
    HidranteImp *hidImp = (HidranteImp *) hid;
    circulo_definir_cor_preenchimento(hidImp->circ, cor_preenchimento);
}

const char *hidrante_obter_cor_borda(Hidrante hid) {
    HidranteImp *hidImp = (HidranteImp *) hid;
    return circulo_obter_cor_borda(hidImp->circ);
}

// Define a cor da borda de um hidrante.
void hidrante_definir_cor_borda(Hidrante hid, const char *cor_borda) {
    HidranteImp *hidImp = (HidranteImp *) hid;
    circulo_definir_cor_borda(hidImp->circ, cor_borda);
}

// Define a espessura da borda de um hidrante.
void hidrante_definir_espessura_borda(Hidrante hid, int espessura_borda) {
    HidranteImp *hidImp = (HidranteImp *) hid;
    circulo_definir_espessura_borda(hidImp->circ, espessura_borda);
}

// Libera a memória alocada por um hidrante.
void hidrante_destruir(Hidrante hid) {
    HidranteImp *hidImp = (HidranteImp *) hid;
    circulo_destruir(hidImp->circ);
    free(hid);
}
