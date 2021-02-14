#include "poligono.h"

#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"
#include "../../Utils/matematica.h"

struct Poligono_s {
    FiguraInterface vtable;
    double x;
    double y;
    double largura;
    double altura;
    int numero_de_pontos;
    char cor_borda[20];
    char cor_preenchimento[20];
    double opacidade;
    double **pontos;
};

const char *poligono_obter_tipo() {
    return "polígono";
}

// Retorna um texto vazio. Precisa ser implementado para um polígono poder ser utilizado como uma
// figura genérica.
const char *poligono_obter_id() {
    return "";
}

// Registra as funções do objeto polígono na interface FiguraInterface.
static FiguraInterface poligono_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, poligono_obter_tipo);

    figura_registrar_escrever_informacoes(interface,
                                          (EscreverInformacoes *) poligono_escrever_informacoes);
    figura_registrar_escrever_svg(interface, (EscreverSvg *) poligono_escrever_svg);

    figura_registrar_obter_id(interface, (ObterId *) poligono_obter_id);

    figura_registrar_obter_x(interface, (ObterX *) poligono_obter_x);
    figura_registrar_obter_y(interface, (ObterY *) poligono_obter_y);

    figura_registrar_obter_x_inicio(interface, (ObterXInicio *) poligono_obter_x);
    figura_registrar_obter_y_inicio(interface, (ObterYInicio *) poligono_obter_y);

    figura_registrar_obter_x_fim(interface, (ObterXFim *) poligono_obter_x_fim);
    figura_registrar_obter_y_fim(interface, (ObterYFim *) poligono_obter_y_fim);

    figura_registrar_obter_x_centro(interface, (ObterXCentro *) poligono_obter_x_centro);
    figura_registrar_obter_y_centro(interface, (ObterYCentro *) poligono_obter_y_centro);

    figura_registrar_obter_cor_borda(interface, (ObterCorBorda *) poligono_obter_cor_borda);
    figura_registrar_definir_cor_borda(interface, (DefinirCorBorda *) poligono_definir_cor_borda);

    figura_registrar_obter_cor_preenchimento(
        interface, (ObterCorPreenchimento *) poligono_obter_cor_preenchimento);
    figura_registrar_definir_cor_preenchimento(
        interface, (DefinirCorPreenchimento *) poligono_definir_cor_preenchimento);

    figura_registrar_destruir(interface, (Destruir *) poligono_destruir);
    return interface;
}

// Cria e inicializa um struct Poligono com os valores passados.
Poligono poligono_criar(double **pontos, int numero_de_pontos, const char cor_borda[20],
                        const char cor_preenchimento[20], double opacidade) {
    if (pontos == NULL) {
        LOG_AVISO("Endereço dos pontos é nulo ao criar polígono!\n");
        return NULL;
    }
    if (cor_borda == NULL) {
        LOG_AVISO("Não é possível criar um polígono com cor de borda NULL!\n");
        return NULL;
    }
    if (cor_preenchimento == NULL) {
        LOG_AVISO("Não é possível criar um polígono com cor de preenchimento NULL!\n");
        return NULL;
    }
    if (opacidade < 0 || opacidade > 1) {
        LOG_AVISO("Não é possível criar um polígono com opacidade menor que 0 ou maior que 1!\n");
        return NULL;
    }
    Poligono poligono = malloc(sizeof *poligono);
    if (poligono == NULL) {
        LOG_ERRO("Falha ao alocar memória\n");
        return NULL;
    }

    double min_x = DBL_MAX;
    double min_y = DBL_MAX;
    double max_x = DBL_MIN;
    double max_y = DBL_MIN;
    for (int i = 0; i < numero_de_pontos; i++) {
        min_x = min(pontos[i][0], min_x);
        min_y = min(pontos[i][1], min_y);
        max_x = max(pontos[i][0], max_x);
        max_y = max(pontos[i][1], max_y);
    }

    poligono->x = min_x;
    poligono->y = min_y;
    poligono->largura = max_x - min_x;
    poligono->altura = max_y - min_y;

    poligono->pontos = pontos;
    poligono->numero_de_pontos = numero_de_pontos;
    strcpy(poligono->cor_borda, cor_borda);
    strcpy(poligono->cor_preenchimento, cor_preenchimento);
    poligono->opacidade = opacidade;

    poligono->vtable = poligono_criar_interface_figura();
    return poligono;
}

// Escreve todos os dados de um polígono em um arquivo.
void poligono_escrever_informacoes(Poligono poligono, FILE *arquivo) {
    fprintf(arquivo, "tipo: %s, largura: %lf, altura: %lf, x: %lf, y: %lf, corb: %s, corp: %s,",
            figura_obter_tipo(poligono), poligono->largura, poligono->altura, poligono->x,
            poligono->y, poligono->cor_borda, poligono->cor_preenchimento);

    fprintf(arquivo, " pontos: (");
    for (int i = 0; i < poligono->numero_de_pontos; i++) {
        if (i != 0)
            fprintf(arquivo, " ,");
        double x = poligono->pontos[i][0];
        double y = poligono->pontos[i][1];
        fprintf(arquivo, "(%lf,%lf)", x, y);
    }
    fprintf(arquivo, ")\n");
}

// Escreve o código svg que representa um polígono em um arquivo.
void poligono_escrever_svg(Poligono poligono, FILE *arquivo) {
    fprintf(arquivo, "\t<polygon points='");

    for (int i = 0; i < poligono->numero_de_pontos; i++) {
        if (i != 0)
            fprintf(arquivo, " ");
        double x = poligono->pontos[i][0];
        double y = poligono->pontos[i][1];
        fprintf(arquivo, "%lf,%lf", x, y);
    }

    fprintf(arquivo, "' stroke='%s' fill='%s' stroke-width='4px' opacity='%lf' />\n",
            poligono->cor_borda, poligono->cor_preenchimento, poligono->opacidade);
}

// Retorna a área de um polígono.
double poligono_calcular_area(Poligono poligono) {
    double area = 0;
    for (int i = 0; i < poligono->numero_de_pontos; i++) {
        int proximo_indice = (i + 1) % poligono->numero_de_pontos;

        double x = poligono->pontos[i][0];
        double y = poligono->pontos[i][1];
        double x1 = poligono->pontos[proximo_indice][0];
        double y1 = poligono->pontos[proximo_indice][1];
        area += x * y1 - x1 * y;
    }
    return area / 2;
}

// Retorna a coordenada x de polígono.
double poligono_obter_x(Poligono poligono) {
    return poligono->x;
}

// Retorna a coordenada y de polígono.
double poligono_obter_y(Poligono poligono) {
    return poligono->y;
}

// Retorna a coordenada x onde uma linha termina.
double poligono_obter_x_fim(Poligono poligono) {
    return poligono->x + poligono->largura;
}

// Retorna a coordenada y onde uma linha termina.
double poligono_obter_y_fim(Poligono poligono) {
    return poligono->y + poligono->altura;
}

// Inicializa as coordenadas x e y do centroide de um polígono.
void poligono_inicializar_centroide(Poligono poligono, double *x, double *y) {
    double valor_x = 0;
    double valor_y = 0;
    double area = poligono_calcular_area(poligono);
    for (int i = 0; i < poligono->numero_de_pontos; i++) {
        int proximo_indice = (i + 1) % poligono->numero_de_pontos;

        double x = poligono->pontos[i][0];
        double y = poligono->pontos[i][1];
        double x1 = poligono->pontos[proximo_indice][0];
        double y1 = poligono->pontos[proximo_indice][1];
        valor_x += (x + x1) * (x * y1 - x1 * y);
        valor_y += (y + y1) * (x * y1 - x1 * y);
    }
    *x = valor_x / (6 * area);
    *y = valor_y / (6 * area);
}

// Retorna a cordenada x do centroide de um polígono.
double poligono_obter_x_centro(Poligono poligono) {
    double valor = 0;
    for (int i = 0; i < poligono->numero_de_pontos; i++) {
        int proximo_indice = (i + 1) % poligono->numero_de_pontos;

        double x = poligono->pontos[i][0];
        double y = poligono->pontos[i][1];
        double x1 = poligono->pontos[proximo_indice][0];
        double y1 = poligono->pontos[proximo_indice][1];
        valor += (x + x1) * (x * y1 - x1 * y);
    }
    return valor / (6 * poligono_calcular_area(poligono));
}

// Retorna a cordenada y do centroide de um polígono.
double poligono_obter_y_centro(Poligono poligono) {
    double valor = 0;
    for (int i = 0; i < poligono->numero_de_pontos; i++) {
        int proximo_indice = (i + 1) % poligono->numero_de_pontos;

        double x = poligono->pontos[i][0];
        double y = poligono->pontos[i][1];
        double x1 = poligono->pontos[proximo_indice][0];
        double y1 = poligono->pontos[proximo_indice][1];
        valor += (y + y1) * (x * y1 - x1 * y);
    }
    return valor / (6 * poligono_calcular_area(poligono));
}

// Retorna a largura de polígono.
double poligono_obter_largura(Poligono poligono) {
    return poligono->largura;
}

// Retorna a altura de polígono.
double poligono_obter_altura(Poligono poligono) {
    return poligono->altura;
}

// Retorna a cor da borda de um polígono.
const char *poligono_obter_cor_borda(Poligono poligono) {
    return poligono->cor_borda;
}

// Define a cor da borda de um polígono.
void poligono_definir_cor_borda(Poligono poligono, const char *cor_borda) {
    if (cor_borda == NULL) {
        LOG_AVISO("Não é possível definir NULL como cor da borda de um %s!\n",
                  figura_obter_tipo(poligono));
        return;
    }
    strcpy(poligono->cor_borda, cor_borda);
}

// Retorna a cor de preenchimento de um polígono..
const char *poligono_obter_cor_preenchimento(Poligono poligono) {
    return poligono->cor_preenchimento;
}

// Define a cor de preenchimento de um polígono.
void poligono_definir_cor_preenchimento(Poligono poligono, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        LOG_AVISO("Não é possível definir NULL como cor de preenchimento de um %s!\n",
                  figura_obter_tipo(poligono));
        return;
    }
    strcpy(poligono->cor_preenchimento, cor_preenchimento);
}

// Libera a memória alocada por polígono.
void poligono_destruir(Poligono poligono) {
    // Libera cada linha da matriz.
    for (int j = 0; j < poligono->numero_de_pontos; j++)
        free(poligono->pontos[j]);
    free(poligono->pontos);
    free(poligono->vtable);
    free(poligono);
}
