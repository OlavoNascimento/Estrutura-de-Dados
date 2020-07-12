#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <circulo.h>

typedef struct {
    char id[100];
    double raio;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
} Circulo;

// Criar um círculo com base em informações de uma linha.
Circulo ler_circulo(const char *linha) {
    Circulo circ;
    sscanf(
        linha,
        "%*c %s %lf %lf %lf %s %s",
        circ.id, &circ.raio, &circ.x, &circ.y, circ.cor_borda, circ.cor_preenchimento);
    return circ;
}

// Escreve todos os dados de um círculo em um arquivo.
void escrever_informacoes_circulo(FILE *arquivo, Circulo circ) {
    if (strlen(circ.id) > 0)
        fprintf(arquivo, "id: %s, ", circ.id);
    fprintf(
        arquivo,
        "raio: %lf, x: %lf, y: %lf, corb: %s, corp: %s\n",
        circ.raio, circ.x, circ.y, circ.cor_borda, circ.cor_preenchimento);
}

// Escreve o código svg que representa um círculo em um arquivo.
void escrever_svg_circulo(FILE *arquivo, Circulo circ) {
    fprintf(arquivo, "\t<circle ");
    if (strlen(circ.id) > 1)
        fprintf(arquivo, "id='%s' ", circ.id);
    fprintf(arquivo,
            "r='%lf' cx='%lf' cy='%lf' stroke='%s' fill='%s'/>\n",
            circ.raio,
            circ.x,
            circ.y,
            circ.cor_borda,
            circ.cor_preenchimento);
}

// Retorna verdadeiro se dois círculos se intersectam.
bool checar_interseccao_circulo(Circulo circ1, Circulo circ2) {
    double dist = (circ1.x - circ2.x) * (circ1.x - circ2.x) +
                  (circ1.y - circ2.y) * (circ1.y - circ2.y);
    double raios = (circ1.raio + circ2.raio) * (circ1.raio + circ2.raio);
    if (dist > raios)
        return false;
    return true;
}

// Retorna verdadeiro se um ponto se encontra dentro de um círculo.
bool checar_ponto_interno_circulo(Circulo circ,
                                  double ponto_x, double ponto_y) {
    if (ponto_x <= circ.x - circ.raio || ponto_x >= circ.x + circ.raio)
        return false;
    if (ponto_y <= circ.y - circ.raio || ponto_y >= circ.y + circ.raio)
        return false;
    return true;
}

char *obterIdCirculo(Circulo circ) {
    return circ.id;
}

double obterRaioCirculo(Circulo circ) {
    return circ.raio;
}

double obterXCirculo(Circulo circ) {
    return circ.x;
}

double obterYCirculo(Circulo circ) {
    return circ.y;
}

char *obterCorBordaCirculo(Circulo circ) {
    return circ.cor_borda;
}

char *obterCorPreenchimentoCirculo(Circulo circ) {
    return circ.cor_preenchimento;
}