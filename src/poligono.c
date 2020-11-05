#include "poligono.h"

#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logging.h"

typedef struct {
    double x;
    double y;
    double largura;
    double altura;
    double **pontos;
    int numero_de_pontos;
    char cor_borda[20];
    char cor_preenchimento[20];
    double opacidade;
} PoligonoImp;

// Cria e inicializa um struct Poligono com os valores passados.
Poligono poligono_criar(double **pontos, int numero_de_pontos, const char cor_borda[20],
                        const char cor_preenchimento[20], double opacidade) {
    if (cor_borda == NULL) {
        LOG_ERROR("Não é possível criar um polígono com cor de borda NULL!\n");
        return NULL;
    }
    if (cor_preenchimento == NULL) {
        LOG_ERROR("Não é possível criar um polígono com cor de preenchimento NULL!\n");
        return NULL;
    }
    if (opacidade < 0 || opacidade > 1) {
        LOG_ERROR("Não é possível criar um polígono com opacidade inválida!\n");
        return NULL;
    }
    PoligonoImp *polImp = malloc(sizeof(PoligonoImp));
    double min_x = DBL_MAX;
    double min_y = DBL_MAX;
    double max_x = DBL_MIN;
    double max_y = DBL_MIN;
    for (int i = 0; i < numero_de_pontos; i++) {
        if (pontos[i][0] < min_x)
            min_x = pontos[i][0];
        if (pontos[i][1] < min_y)
            min_y = pontos[i][1];
        if (pontos[i][0] > max_x)
            max_x = pontos[i][0];
        if (pontos[i][1] > max_y)
            max_y = pontos[i][1];
    }
    polImp->x = min_x;
    polImp->y = min_y;
    polImp->largura = max_x - min_x;
    polImp->altura = max_y - min_y;

    polImp->pontos = pontos;
    polImp->numero_de_pontos = numero_de_pontos;
    strcpy(polImp->cor_borda, cor_borda);
    strcpy(polImp->cor_preenchimento, cor_preenchimento);
    polImp->opacidade = opacidade;
    return polImp;
}

// Escreve o código svg que representa um polígono em um arquivo.
void poligono_escrever_svg(FILE *arquivo, Poligono poligono) {
    PoligonoImp *polImp = (PoligonoImp *) poligono;
    fprintf(arquivo, "\t<polygon");

    fprintf(arquivo, " points='");
    for (int i = 0; i < polImp->numero_de_pontos; i++) {
        if (i != 0)
            fprintf(arquivo, " ");
        fprintf(arquivo, "%lf,%lf", polImp->pontos[i][0], polImp->pontos[i][1]);
    }
    fprintf(arquivo, "'");

    fprintf(arquivo, " stroke='%s' fill='%s' stroke-width='4px'", polImp->cor_borda,
            polImp->cor_preenchimento);
    // Caso o polígono deva ser tracejado
    if (polImp->opacidade != 0)
        fprintf(arquivo, "  opacity='%lf'", polImp->opacidade);
    fprintf(arquivo, "/>\n");
}

// Retorna a área de um polígono.
double poligono_calcular_area(Poligono poligono) {
    PoligonoImp *polImp = (PoligonoImp *) poligono;
    double area = 0;
    for (int i = 0; i < polImp->numero_de_pontos - 1; i++) {
        area += polImp->pontos[i][0] * polImp->pontos[i + 1][1] -
                polImp->pontos[i + 1][0] * polImp->pontos[i][1];
    }
    return area / 2;
}

// Retorna a cordenada x do centroide de um polígono.
double poligono_calcular_x_centroide(Poligono poligono) {
    PoligonoImp *polImp = (PoligonoImp *) poligono;
    double valor = 0;
    for (int i = 0; i < polImp->numero_de_pontos - 1; i++) {
        valor += (polImp->pontos[i][0] + polImp->pontos[i + 1][0]) *
                 (polImp->pontos[i][0] * polImp->pontos[i + 1][1] -
                  polImp->pontos[i + 1][0] * polImp->pontos[i][1]);
    }
    return valor / (6 * poligono_calcular_area(poligono));
}

// Retorna a cordenada y do centroide de um polígono.
double poligono_calcular_y_centroide(Poligono poligono) {
    PoligonoImp *polImp = (PoligonoImp *) poligono;
    double valor = 0;
    for (int i = 0; i < polImp->numero_de_pontos - 1; i++) {
        valor += (polImp->pontos[i][1] + polImp->pontos[i + 1][1]) *
                 (polImp->pontos[i][0] * polImp->pontos[i + 1][1] -
                  polImp->pontos[i + 1][0] * polImp->pontos[i][1]);
    }
    return valor / (6 * poligono_calcular_area(poligono));
}

// Retorna a largura de polígono.
double poligono_obter_largura(Poligono poligono) {
    PoligonoImp *polImp = (PoligonoImp *) poligono;
    return polImp->largura;
}

// Retorna a altura de polígono.
double poligono_obter_altura(Poligono poligono) {
    PoligonoImp *polImp = (PoligonoImp *) poligono;
    return polImp->altura;
}

// Retorna a coordenada x de polígono.
double poligono_obter_x(Poligono poligono) {
    PoligonoImp *polImp = (PoligonoImp *) poligono;
    return polImp->x;
}

// Retorna a coordenada y de polígono.
double poligono_obter_y(Poligono poligono) {
    PoligonoImp *polImp = (PoligonoImp *) poligono;
    return polImp->y;
}

// Libera a memória alocada por.polígono.
void poligono_destruir(Poligono poligono) {
    PoligonoImp *polImp = (PoligonoImp *) poligono;
    for (int j = 0; j < polImp->numero_de_pontos; j++)
        free(polImp->pontos[j]);
    free(polImp->pontos);
    free(polImp);
}
