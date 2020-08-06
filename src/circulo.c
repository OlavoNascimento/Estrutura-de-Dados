#include "circulo.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char id[100];
    double raio;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
} Circulo;

// Cria e inicializa um struct Circulo com os valores passados.
Circulo circulo_criar(char id[100], double raio, double x, double y, char cor_borda[20],
                      char cor_preenchimento[20]) {
    Circulo cir;
    strcpy(cir.id, id);
    cir.raio = raio;
    cir.x = x;
    cir.y = y;
    strcpy(cir.cor_borda, cor_borda);
    strcpy(cir.cor_preenchimento, cor_preenchimento);
    return cir;
}

// Criar um círculo com base em informações de uma linha.
Circulo circulo_ler(const char *linha) {
    char id[100];
    double raio;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    sscanf(linha, "%*c %s %lf %lf %lf %s %s", id, raio, x, y, cor_borda, cor_preenchimento);
    return circulo_criar(id, raio, x, y, cor_borda, cor_preenchimento);
}

// Escreve todos os dados de um círculo em um arquivo.
void circulo_escrever_informacoes(FILE *arquivo, Circulo cir) {
    if (strlen(cir.id) > 0)
        fprintf(arquivo, "id: %s, ", cir.id);
    fprintf(arquivo,
            "raio: %lf, x: %lf, y: %lf, corb: %s, corp: %s\n",
            cir.raio, cir.x, cir.y, cir.cor_borda, cir.cor_preenchimento);
}

// Escreve o código svg que representa um círculo em um arquivo.
void circulo_escrever_svg(FILE *arquivo, Circulo cir) {
    fprintf(arquivo, "\t<circle ");
    if (strlen(cir.id) > 1)
        fprintf(arquivo, "id='%s' ", cir.id);
    fprintf(arquivo,
            "r='%lf' cx='%lf' cy='%lf' stroke='%s' fill='%s'/>\n",
            cir.raio, cir.x, cir.y, cir.cor_borda, cir.cor_preenchimento);
}

// Retorna verdadeiro se dois círculos se intersectam.
bool circulo_checar_interseccao(Circulo circ1, Circulo circ2) {
    double dist = (circ1.x - circ2.x) * (circ1.x - circ2.x) +
                  (circ1.y - circ2.y) * (circ1.y - circ2.y);
    double raios = (circ1.raio + circ2.raio) * (circ1.raio + circ2.raio);
    if (dist > raios)
        return false;
    return true;
}

// Retorna verdadeiro se um ponto se encontra dentro de um círculo.
bool circulo_checar_ponto_interno(Circulo cir, double ponto_x, double ponto_y) {
    if (ponto_x <= cir.x - cir.raio || ponto_x >= cir.x + cir.raio)
        return false;
    if (ponto_y <= cir.y - cir.raio || ponto_y >= cir.y + cir.raio)
        return false;
    return true;
}

const char *circulo_obter_id(Circulo cir) {
    return cir.id;
}

void circulo_definir_id(Circulo *cir, const char *id) {
    if (id == NULL) {
        fprintf(stderr, "ERRO: Não é possível definir null como id de um círculo!");
        return;
    }
    strcpy(cir->id, id);
}

double circulo_obter_raio(Circulo cir) {
    return cir.raio;
}

void circulo_definir_raio(Circulo *cir, double raio) {
    cir->raio = raio;
}

double circulo_obter_x(Circulo cir) {
    return cir.x;
}

void circulo_definir_x(Circulo *cir, double x) {
    cir->x = x;
}

double circulo_obter_y(Circulo cir) {
    return cir.y;
}

void circulo_definir_y(Circulo *cir, double y) {
    cir->y = y;
}

const char *circulo_obter_cor_borda(Circulo cir) {
    return cir.cor_borda;
}

void circulo_definir_cor_borda(Circulo *cir, const char *cor_borda) {
    if (cor_borda == NULL) {
        fprintf(stderr, "ERRO: Não é possível definir null como cor da borda de um círculo!");
        return;
    }
    strcpy(cir->cor_borda, cor_borda);
}

const char *circulo_obter_cor_preenchimento(Circulo cir) {
    return cir.cor_preenchimento;
}

void circulo_definir_cor_preenchimento(Circulo *cir, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        fprintf(stderr, "ERRO: Não é possível definir null como cor de preenchimento de um círculo!");
        return;
    }
    strcpy(cir->cor_preenchimento, cor_preenchimento);
}
