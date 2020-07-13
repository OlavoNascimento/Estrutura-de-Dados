#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <retangulo.h>

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

// Criar um retângulo com base em informações de uma linha.
Retangulo retangulo_ler(char *linha) {
    Retangulo ret;
    sscanf(linha,
           "%*c %s %lf %lf %lf %lf %s %s",
           ret.id, &ret.largura, &ret.altura, &ret.x, &ret.y, ret.cor_borda, ret.cor_preenchimento);
    ret.tracejado_espaco = 0;
    ret.tracejado_tamanho = 0;
    return ret;
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

char retangulo_obter_id(Retangulo ret) {
    return ret.id;
}

double retangulo_obter_largura(Retangulo ret) {
    return ret.largura;
}

double retangulo_obter_altura(Retangulo ret) {
    return ret.altura;
}

double retangulo_obter_x(Retangulo ret) {
    return ret.x;
}

double retangulo_obter_y(Retangulo ret) {
    return ret.y;
}

char retangulo_obter_cor_borda(Retangulo ret) {
    return ret.cor_borda;
}

char retangulo_obter_cor_preenchimento(Retangulo ret) {
    return ret.cor_preenchimento;
}
