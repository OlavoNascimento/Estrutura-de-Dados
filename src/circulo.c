#include "circulo.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char id[100];
    double raio;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    char espessura_borda[20];
} CirculoImp;

// Cria e inicializa um struct CirculoImp com os valores passados.
Circulo circulo_criar(const char id[100], double raio, double x, double y, const char cor_borda[20],
                      const char cor_preenchimento[20]) {
    if (id == NULL) {
        fprintf(stderr, "ERRO: Não é possível criar um círculo com id NULL!\n");
        return NULL;
    }
    if (cor_borda == NULL) {
        fprintf(stderr, "ERRO: Não é possível criar um círculo com cor de borda NULL!\n");
        return NULL;
    }
    if (cor_preenchimento == NULL) {
        fprintf(stderr, "ERRO: Não é possível criar um círculo com cor de preenchimento NULL!\n");
        return NULL;
    }
    CirculoImp *cirImp = malloc(sizeof(CirculoImp));
    strcpy(cirImp->id, id);
    cirImp->raio = raio;
    cirImp->x = x;
    cirImp->y = y;
    strcpy(cirImp->cor_borda, cor_borda);
    strcpy(cirImp->cor_preenchimento, cor_preenchimento);
    strcpy(cirImp->espessura_borda, "1px");
    return cirImp;
}

// Cria um círculo com base em informações de uma linha.
Circulo circulo_ler(const char *linha) {
    char id[100];
    double raio;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    sscanf(linha, "%*s %s %lf %lf %lf %s %s", id, &raio, &x, &y, cor_borda, cor_preenchimento);
    return circulo_criar(id, raio, x, y, cor_borda, cor_preenchimento);
}

// Escreve todos os dados de um círculo em um arquivo.
void circulo_escrever_informacoes(FILE *arquivo, Circulo cir) {
    CirculoImp *cirImp = (CirculoImp *) cir;
    // Um circulo pode não ter um id
    if (strlen(cirImp->id) > 0)
        fprintf(arquivo, "id: %s, ", cirImp->id);
    fprintf(arquivo, "raio: %lf, x: %lf, y: %lf, corb: %s, corp: %s\n", cirImp->raio, cirImp->x,
            cirImp->y, cirImp->cor_borda, cirImp->cor_preenchimento);
}

// Escreve o código svg que representa um círculo em um arquivo.
void circulo_escrever_svg(FILE *arquivo, Circulo cir) {
    CirculoImp *cirImp = (CirculoImp *) cir;
    fprintf(arquivo, "\t<circle ");
    // Um circulo pode não ter um id
    if (strlen(cirImp->id) > 0)
        fprintf(arquivo, "id='%s' ", cirImp->id);

    fprintf(arquivo, "r='%lf' cx='%lf' cy='%lf' stroke='%s' fill='%s' stroke-width='%s'",
            cirImp->raio, cirImp->x, cirImp->y, cirImp->cor_borda, cirImp->cor_preenchimento,
            cirImp->espessura_borda);
    fprintf(arquivo, "/>\n");
}

// Retorna verdadeiro se dois círculos se intersectam.
bool circulo_checar_interseccao(Circulo cir1, Circulo cir2) {
    CirculoImp *cirImp1 = (CirculoImp *) cir1;
    CirculoImp *cirImp2 = (CirculoImp *) cir2;
    double dist = (cirImp1->x - cirImp2->x) * (cirImp1->x - cirImp2->x) +
                  (cirImp1->y - cirImp2->y) * (cirImp1->y - cirImp2->y);
    double raios = (cirImp1->raio + cirImp2->raio) * (cirImp1->raio + cirImp2->raio);
    if (dist > raios)
        return false;
    return true;
}

// Retorna verdadeiro se um ponto se encontra dentro de um círculo.
bool circulo_checar_ponto_interno(Circulo cir, double ponto_x, double ponto_y) {
    CirculoImp *cirImp = (CirculoImp *) cir;
    if (ponto_x <= cirImp->x - cirImp->raio || ponto_x >= cirImp->x + cirImp->raio)
        return false;
    if (ponto_y <= cirImp->y - cirImp->raio || ponto_y >= cirImp->y + cirImp->raio)
        return false;
    return true;
}

// Retorna o id de um círculo.
const char *circulo_obter_id(Circulo cir) {
    CirculoImp *cirImp = (CirculoImp *) cir;
    return cirImp->id;
}

// Define o id de um círculo.
void circulo_definir_id(Circulo cir, const char *id) {
    if (id == NULL) {
        fprintf(stderr, "ERRO: Não é possível definir NULL como id de um círculo!\n");
        return;
    }
    CirculoImp *cirImp = (CirculoImp *) cir;
    strcpy(cirImp->id, id);
}

// Retorna o raio de um círculo.
double circulo_obter_raio(Circulo cir) {
    CirculoImp *cirImp = (CirculoImp *) cir;
    return cirImp->raio;
}

// Define o raio de um círculo.
void circulo_definir_raio(Circulo cir, double raio) {
    CirculoImp *cirImp = (CirculoImp *) cir;
    cirImp->raio = raio;
}

// Retorna a coordenada x de um círculo.
double circulo_obter_x(Circulo cir) {
    CirculoImp *cirImp = (CirculoImp *) cir;
    return cirImp->x;
}

// Define a coordenada x de um círculo.
void circulo_definir_x(Circulo cir, double x) {
    CirculoImp *cirImp = (CirculoImp *) cir;
    cirImp->x = x;
}

// Retorna a coordenada y de um círculo.
double circulo_obter_y(Circulo cir) {
    CirculoImp *cirImp = (CirculoImp *) cir;
    return cirImp->y;
}

// Define a coordenada y de um círculo.
void circulo_definir_y(Circulo cir, double y) {
    CirculoImp *cirImp = (CirculoImp *) cir;
    cirImp->y = y;
}

// Retorna a cor da borda de um círculo.
const char *circulo_obter_cor_borda(Circulo cir) {
    CirculoImp *cirImp = (CirculoImp *) cir;
    return cirImp->cor_borda;
}

// Define a cor da borda de um círculo.
void circulo_definir_cor_borda(Circulo cir, const char *cor_borda) {
    if (cor_borda == NULL) {
        fprintf(stderr, "ERRO: Não é possível definir NULL como cor da borda de um círculo!\n");
        return;
    }
    CirculoImp *cirImp = (CirculoImp *) cir;
    strcpy(cirImp->cor_borda, cor_borda);
}

// Retorna a cor de preenchimento de um círculo..
const char *circulo_obter_cor_preenchimento(Circulo cir) {
    CirculoImp *cirImp = (CirculoImp *) cir;
    return cirImp->cor_preenchimento;
}

// Define a cor de preenchimento de um círculo.
void circulo_definir_cor_preenchimento(Circulo cir, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        fprintf(stderr,
                "ERRO: Não é possível definir NULL como cor de preenchimento de um círculo!\n");
        return;
    }
    CirculoImp *cirImp = (CirculoImp *) cir;
    strcpy(cirImp->cor_preenchimento, cor_preenchimento);
}

// Retorna a espessura da borda de um círculo.
const char *circulo_obter_espessura_borda(Circulo cir) {
    CirculoImp *cirImp = (CirculoImp *) cir;
    return cirImp->espessura_borda;
}

// Define a espessura da borda de um círculo.
void circulo_definir_espessura_borda(Circulo cir, const char *espessura_borda) {
    if (espessura_borda == NULL) {
        fprintf(stderr,
                "ERRO: Não é possível definir NULL como tamanho da espessura da borda de um "
                "círculo!\n");
        return;
    }
    CirculoImp *cirImp = (CirculoImp *) cir;
    strcpy(cirImp->espessura_borda, espessura_borda);
}

// Libera a memória alocada por um círculo.
void circulo_destruir(Circulo cir) {
    free(cir);
}
