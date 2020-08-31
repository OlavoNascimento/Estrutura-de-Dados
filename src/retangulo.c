#include "retangulo.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char id[100];
    double largura;
    double altura;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    double rx;
    int espessura_borda;
    int espassamento_borda;
} RetanguloImp;

// Cria e inicializa um struct Retangulo com os valores passados.
Retangulo retangulo_criar(char id[100], double largura, double altura, double x, double y,
                          char cor_borda[20], char cor_preenchimento[20]) {
    RetanguloImp *retImp = malloc(sizeof(RetanguloImp));
    strcpy(retImp->id, id);
    retImp->largura = largura;
    retImp->altura = altura;
    retImp->x = x;
    retImp->y = y;
    strcpy(retImp->cor_borda, cor_borda);
    strcpy(retImp->cor_preenchimento, cor_preenchimento);
    retImp->rx = 0;
    retImp->espassamento_borda = 0;
    retImp->espessura_borda = 0;
    return retImp;
}

// Cria um retângulo com base em informações de uma linha.
Retangulo retangulo_ler(const char *linha) {
    char id[100];
    double largura;
    double altura;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    sscanf(linha, "%*s %s %lf %lf %lf %lf %s %s", id, &largura, &altura, &x, &y, cor_borda,
           cor_preenchimento);
    return retangulo_criar(id, largura, altura, x, y, cor_borda, cor_preenchimento);
}

// Escreve todos os dados de um retângulo em um arquivo.
void retangulo_escrever_informacoes(FILE *arquivo, Retangulo ret) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    if (strlen(retImp->id) > 0)
        fprintf(arquivo, "id: %s, ", retImp->id);
    fprintf(arquivo, "largura: %lf, altura: %lf, x: %lf, y: %lf, corb: %s, corp: %s\n",
            retImp->largura, retImp->altura, retImp->x, retImp->y, retImp->cor_borda,
            retImp->cor_preenchimento);
}

// Escreve o código svg que representa um retângulo em um arquivo.
void retangulo_escrever_svg(FILE *arquivo, Retangulo ret) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    fprintf(arquivo, "\t<rect");
    // Nem todo retângulo tem um id
    if (strlen(retImp->id) > 0)
        fprintf(arquivo, " id='%s'", retImp->id);

    fprintf(arquivo, " width='%lf' height='%lf' x='%lf' y='%lf' stroke='%s' fill='%s' rx='%lf'",
            retImp->largura, retImp->altura, retImp->x, retImp->y, retImp->cor_borda,
            retImp->cor_preenchimento, retImp->rx);

    // Caso a borda do retângulo seja diferente do padrão
    if (retImp->espessura_borda != 0)
        fprintf(arquivo, " stroke-width='%d'", retImp->espessura_borda);

    // Caso o retângulo deva ser tracejado
    if (retImp->espassamento_borda != 0)
        fprintf(arquivo, " style='stroke-dasharray: %d'", retImp->espassamento_borda);
    fprintf(arquivo, "/>\n");
}

// Retorna verdadeiro se dois retângulos se intersectam.
bool retangulo_checar_interseccao(Retangulo ret1, Retangulo ret2) {
    RetanguloImp *retImp1 = (RetanguloImp *) ret1;
    RetanguloImp *retImp2 = (RetanguloImp *) ret2;
    if (retImp1->x > retImp2->x + retImp2->largura || retImp2->x > retImp1->x + retImp1->largura)
        return false;
    if (retImp1->y + retImp1->altura < retImp2->y || retImp2->y + retImp2->altura < retImp1->y)
        return false;
    return true;
}

// Retorna verdadeiro se um ponto se encontra dentro de um retângulo.
bool retangulo_checar_ponto_interno(Retangulo ret, double ponto_x, double ponto_y) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    if (ponto_x <= retImp->x || ponto_x >= retImp->x + retImp->largura)
        return false;
    if (ponto_y <= retImp->y || ponto_y >= retImp->y + retImp->altura)
        return false;
    return true;
}

const char *retangulo_obter_id(Retangulo ret) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    return retImp->id;
}

void retangulo_definir_id(Retangulo ret, const char *id) {
    if (id == NULL) {
        fprintf(stderr, "ERRO: Não é possível definir null como id de um retângulo!\n");
        return;
    }
    RetanguloImp *retImp = (RetanguloImp *) ret;
    strcpy(retImp->id, id);
}

double retangulo_obter_largura(Retangulo ret) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    return retImp->largura;
}

void retangulo_definir_largura(Retangulo ret, double largura) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    retImp->largura = largura;
}

double retangulo_obter_altura(Retangulo ret) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    return retImp->altura;
}

void retangulo_definir_altura(Retangulo ret, double altura) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    retImp->altura = altura;
}

double retangulo_obter_x(Retangulo ret) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    return retImp->x;
}

void retangulo_definir_x(Retangulo ret, double x) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    retImp->x = x;
}

double retangulo_obter_y(Retangulo ret) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    return retImp->y;
}

void retangulo_definir_y(Retangulo ret, double y) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    retImp->y = y;
}

const char *retangulo_obter_cor_borda(Retangulo ret) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    return retImp->cor_borda;
}

void retangulo_definir_cor_borda(Retangulo ret, const char *cor_borda) {
    if (cor_borda == NULL) {
        fprintf(stderr, "ERRO: Não é possível definir null como cor da borda de um retângulo!\n");
        return;
    }
    RetanguloImp *retImp = (RetanguloImp *) ret;
    strcpy(retImp->cor_borda, cor_borda);
}

const char *retangulo_obter_cor_preenchimento(Retangulo ret) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    return retImp->cor_preenchimento;
}

void retangulo_definir_cor_preenchimento(Retangulo ret, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        fprintf(stderr,
                "ERRO: Não é possível definir null como cor de preenchimento de um retângulo!\n");
        return;
    }
    RetanguloImp *retImp = (RetanguloImp *) ret;
    strcpy(retImp->cor_preenchimento, cor_preenchimento);
}

int retangulo_obter_espessura_borda(Retangulo ret) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    return retImp->espessura_borda;
}

void retangulo_definir_espessura_borda(Retangulo ret, int espessura_borda) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    retImp->espessura_borda = espessura_borda;
}

int retangulo_obter_espassamento_borda(Retangulo ret) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    return retImp->espassamento_borda;
}

void retangulo_definir_espassamento_borda(Retangulo ret, int espassamento_borda) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    retImp->espassamento_borda = espassamento_borda;
}

void retangulo_definir_arredondamento_borda(Retangulo ret, double raio_borda) {
    RetanguloImp *retImp = (RetanguloImp *) ret;
    retImp->rx = raio_borda;
}

void retangulo_destruir(Retangulo ret) {
    free(ret);
}
