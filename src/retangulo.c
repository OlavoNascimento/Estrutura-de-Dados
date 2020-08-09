#include "retangulo.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char id[100];
    double largura;
    double altura;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    int tracejado_tamanho;
    int tracejado_espaco;
} Retangulo;

// Cria e inicializa um struct Retangulo com os valores passados.
Retangulo retangulo_criar(char id[100], double largura, double altura, double x, double y,
                          char cor_borda[20], char cor_preenchimento[20],
                          int tracejado_tamanho, int tracejado_espaco) {
    Retangulo ret;
    strcpy(ret.id, id);
    ret.largura = largura;
    ret.altura = altura;
    ret.x = x;
    ret.y = y;
    strcpy(ret.cor_borda, cor_borda);
    strcpy(ret.cor_preenchimento, cor_preenchimento);
    ret.tracejado_tamanho = tracejado_tamanho;
    ret.tracejado_espaco = tracejado_espaco;
    return ret;
}

// Cria um retângulo com base em informações de uma linha.
Retangulo retangulo_ler(char *linha) {
    char id[100];
    double largura;
    double altura;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    int tracejado_tamanho;
    int tracejado_espaco;
    sscanf(linha,
           "%*c %s %lf %lf %lf %lf %s %s",
           id, largura, altura, x, y, cor_borda, cor_preenchimento);
    return retangulo_criar(id, largura, altura, x, y, cor_borda, cor_preenchimento, 0, 0);
}

// Escreve todos os dados de um retângulo em um arquivo.
void retangulo_escrever_informacoes(FILE *arquivo, Retangulo ret) {
    if (strlen(ret.id) > 0)
        fprintf(arquivo, "id: %s, ", ret.id);
    fprintf(arquivo,
            "largura: %lf, altura: %lf, x: %lf, y: %lf, corb: %s, corp: %s\n",
            ret.largura, ret.altura, ret.x, ret.y, ret.cor_borda, ret.cor_preenchimento);
}

// Escreve o código svg que representa um retângulo em um arquivo.
void retangulo_escrever_svg(FILE *arquivo, Retangulo ret) {
    fprintf(arquivo, "\t<rect");
    if (strlen(ret.id) > 0)
        fprintf(arquivo, " id='%s'", ret.id);

    fprintf(arquivo,
            " width='%lf' height='%lf' x='%lf' y='%lf' stroke='%s' fill='%s'",
            ret.largura, ret.altura, ret.x, ret.y, ret.cor_borda, ret.cor_preenchimento);

    if (ret.tracejado_tamanho != 0 || ret.tracejado_espaco != 0)
        fprintf(arquivo,
                " style='stroke-dasharray: %d %d'",
                ret.tracejado_tamanho, ret.tracejado_espaco);
    fprintf(arquivo, "/>\n");
}

// Retorna verdadeiro se dois retângulos se intersectam.
bool retangulo_checar_interseccao(Retangulo ret1, Retangulo ret2) {
    if (ret1.x > ret2.x + ret2.largura || ret2.x > ret1.x + ret1.largura)
        return false;
    if (ret1.y + ret1.altura < ret2.y || ret2.y + ret2.altura < ret1.y)
        return false;
    return true;
}

// Retorna verdadeiro se um ponto se encontra dentro de um retângulo.
bool retangulo_checar_ponto_interno(Retangulo ret, double ponto_x, double ponto_y) {
    if (ponto_x <= ret.x || ponto_x >= ret.x + ret.largura)
        return false;
    if (ponto_y <= ret.y || ponto_y >= ret.y + ret.altura)
        return false;
    return true;
}

const char *retangulo_obter_id(Retangulo ret) {
    return ret.id;
}

void retangulo_definir_id(Retangulo *ret, const char *id) {
    if (id == NULL) {
        fprintf(stderr, "ERRO: Não é possível definir null como id de um retângulo!\n");
        return;
    }
    strcpy(ret->id, id);
}

double retangulo_obter_largura(Retangulo ret) {
    return ret.largura;
}

void retangulo_definir_largura(Retangulo *ret, double x1) {
    ret->largura = x1;
}

double retangulo_obter_altura(Retangulo ret) {
    return ret.altura;
}

void retangulo_definir_altura(Retangulo *ret, double x1) {
    ret->altura = x1;
}

double retangulo_obter_x(Retangulo ret) {
    return ret.x;
}

void retangulo_definir_x(Retangulo *ret, double x1) {
    ret->x = x1;
}

double retangulo_obter_y(Retangulo ret) {
    return ret.y;
}

void retangulo_definir_y(Retangulo *ret, double x1) {
    ret->y = x1;
}

const char *retangulo_obter_cor_borda(Retangulo ret) {
    return ret.cor_borda;
}

void retangulo_definir_cor_borda(Retangulo *ret, const char *cor_borda) {
    if (cor_borda == NULL) {
        fprintf(stderr, "ERRO: Não é possível definir null como cor da borda de um retângulo!\n");
        return;
    }
    strcpy(ret->cor_borda, cor_borda);
}

const char *retangulo_obter_cor_preenchimento(Retangulo ret) {
    return ret.cor_preenchimento;
}

void retangulo_definir_cor_preenchimento(Retangulo *ret, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        fprintf(stderr,
                "ERRO: Não é possível definir null como cor de preenchimento de um retângulo!\n");
        return;
    }
    strcpy(ret->cor_preenchimento, cor_preenchimento);
}

int retangulo_obter_tracejado_tamanho(Retangulo ret) {
    return ret.tracejado_tamanho;
}

void retangulo_definir_tracejado_tamanho(Retangulo *ret, int tracejado_tamanho) {
    ret->tracejado_tamanho = tracejado_tamanho;
}

int retangulo_obter_tracejado_espaco(Retangulo ret) {
    return ret.tracejado_espaco;
}

void retangulo_definir_tracejado_espaco(Retangulo *ret, int tracejado_espaco) {
    ret->tracejado_espaco = tracejado_espaco;
}
