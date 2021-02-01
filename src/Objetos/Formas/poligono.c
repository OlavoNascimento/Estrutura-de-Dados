#include "poligono.h"

#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"

typedef struct {
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
} PoligonoImp;

const char *poligono_obter_tipo() {
    return "polígono";
}

// Retorna um texto vazio. Precisa ser implementado para um polígono poder ser utilizado como uma
// figura genérica.
const char *poligono_obter_id() {
    return "";
}

// Conecta as funções do objeto polígono com as da interface figura.
static FiguraInterface poligono_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, poligono_obter_tipo);

    figura_registrar_escrever_informacoes(interface, poligono_escrever_informacoes);
    figura_registrar_escrever_svg(interface, poligono_escrever_svg);

    figura_registrar_obter_id(interface, poligono_obter_id);

    figura_registrar_obter_x(interface, poligono_obter_x);
    figura_registrar_obter_y(interface, poligono_obter_y);

    figura_registrar_obter_x_inicio(interface, poligono_obter_x);
    figura_registrar_obter_y_inicio(interface, poligono_obter_y);

    figura_registrar_obter_x_fim(interface, poligono_obter_x_fim);
    figura_registrar_obter_y_fim(interface, poligono_obter_y_fim);

    figura_registrar_obter_x_centro(interface, poligono_obter_x_centro);
    figura_registrar_obter_y_centro(interface, poligono_obter_y_centro);

    figura_registrar_obter_cor_borda(interface, poligono_obter_cor_borda);
    figura_registrar_definir_cor_borda(interface, poligono_definir_cor_borda);

    figura_registrar_obter_cor_preenchimento(interface, poligono_obter_cor_preenchimento);
    figura_registrar_definir_cor_preenchimento(interface, poligono_definir_cor_preenchimento);

    figura_registrar_destruir(interface, poligono_destruir);
    return interface;
}

// Cria e inicializa um struct Poligono com os valores passados.
Poligono poligono_criar(double **pontos, int numero_de_pontos, const char cor_borda[20],
                        const char cor_preenchimento[20], double opacidade) {
    if (pontos == NULL) {
        LOG_ERRO("Endereço dos pontos é nulo ao criar polígono!\n");
        return NULL;
    }
    if (cor_borda == NULL) {
        LOG_ERRO("Não é possível criar um polígono com cor de borda NULL!\n");
        return NULL;
    }
    if (cor_preenchimento == NULL) {
        LOG_ERRO("Não é possível criar um polígono com cor de preenchimento NULL!\n");
        return NULL;
    }
    if (opacidade < 0 || opacidade > 1) {
        LOG_ERRO("Não é possível criar um polígono com opacidade menor que 0 ou maior que 1!\n");
        return NULL;
    }
    PoligonoImp *polImp = malloc(sizeof *polImp);

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

    polImp->vtable = poligono_criar_interface_figura();
    return polImp;
}

// Escreve todos os dados de um polígono em um arquivo.
void poligono_escrever_informacoes(Poligono poligono, FILE *arquivo) {
    PoligonoImp *polImp = poligono;
    fprintf(arquivo, "tipo: %s, largura: %lf, altura: %lf, x: %lf, y: %lf, corb: %s, corp: %s,",
            figura_obter_tipo(poligono), polImp->largura, polImp->altura, polImp->x, polImp->y,
            polImp->cor_borda, polImp->cor_preenchimento);

    fprintf(arquivo, " pontos: (");
    for (int i = 0; i < polImp->numero_de_pontos; i++) {
        if (i != 0)
            fprintf(arquivo, " ,");
        double x = polImp->pontos[i][0];
        double y = polImp->pontos[i][1];
        fprintf(arquivo, "(%lf,%lf)", x, y);
    }
    fprintf(arquivo, ")\n");
}

// Escreve o código svg que representa um polígono em um arquivo.
void poligono_escrever_svg(Poligono poligono, FILE *arquivo) {
    PoligonoImp *polImp = poligono;
    fprintf(arquivo, "\t<polygon points='");

    for (int i = 0; i < polImp->numero_de_pontos; i++) {
        if (i != 0)
            fprintf(arquivo, " ");
        double x = polImp->pontos[i][0];
        double y = polImp->pontos[i][1];
        fprintf(arquivo, "%lf,%lf", x, y);
    }

    fprintf(arquivo, "' stroke='%s' fill='%s' stroke-width='4px' opacity='%lf' />\n",
            polImp->cor_borda, polImp->cor_preenchimento, polImp->opacidade);
}

// Retorna a área de um polígono.
double poligono_calcular_area(Poligono poligono) {
    PoligonoImp *polImp = poligono;
    double area = 0;
    for (int i = 0; i < polImp->numero_de_pontos; i++) {
        double x = polImp->pontos[i][0];
        double y = polImp->pontos[i][1];
        double x1 = polImp->pontos[(i + 1) % polImp->numero_de_pontos][0];
        double y1 = polImp->pontos[(i + 1) % polImp->numero_de_pontos][1];
        area += x * y1 - x1 * y;
    }
    return area / 2;
}

// Retorna a coordenada x de polígono.
double poligono_obter_x(Poligono poligono) {
    PoligonoImp *polImp = poligono;
    return polImp->x;
}

// Retorna a coordenada y de polígono.
double poligono_obter_y(Poligono poligono) {
    PoligonoImp *polImp = poligono;
    return polImp->y;
}

// Retorna a coordenada x onde uma linha termina.
double poligono_obter_x_fim(Poligono poligono) {
    PoligonoImp *polImp = poligono;
    return polImp->x + polImp->largura;
}

// Retorna a coordenada y onde uma linha termina.
double poligono_obter_y_fim(Poligono poligono) {
    PoligonoImp *polImp = poligono;
    return polImp->y + polImp->altura;
}

// Retorna a cordenada x do centroide de um polígono.
double poligono_obter_x_centro(Poligono poligono) {
    PoligonoImp *polImp = poligono;
    double valor = 0;
    for (int i = 0; i < polImp->numero_de_pontos; i++) {
        double x = polImp->pontos[i][0];
        double y = polImp->pontos[i][1];
        double x1 = polImp->pontos[(i + 1) % polImp->numero_de_pontos][0];
        double y1 = polImp->pontos[(i + 1) % polImp->numero_de_pontos][1];
        valor += (x + x1) * (x * y1 - x1 * y);
    }
    return valor / (6 * poligono_calcular_area(poligono));
}

// Retorna a cordenada y do centroide de um polígono.
double poligono_obter_y_centro(Poligono poligono) {
    PoligonoImp *polImp = poligono;
    double valor = 0;
    for (int i = 0; i < polImp->numero_de_pontos; i++) {
        double x = polImp->pontos[i][0];
        double y = polImp->pontos[i][1];
        double x1 = polImp->pontos[(i + 1) % polImp->numero_de_pontos][0];
        double y1 = polImp->pontos[(i + 1) % polImp->numero_de_pontos][1];
        valor += (y + y1) * (x * y1 - x1 * y);
    }
    return valor / (6 * poligono_calcular_area(poligono));
}

// Retorna a largura de polígono.
double poligono_obter_largura(Poligono poligono) {
    PoligonoImp *polImp = poligono;
    return polImp->largura;
}

// Retorna a altura de polígono.
double poligono_obter_altura(Poligono poligono) {
    PoligonoImp *polImp = poligono;
    return polImp->altura;
}

// Retorna a cor da borda de um polígono.
const char *poligono_obter_cor_borda(Poligono poligono) {
    PoligonoImp *polImp = poligono;
    return polImp->cor_borda;
}

// Define a cor da borda de um polígono.
void poligono_definir_cor_borda(Poligono poligono, const char *cor_borda) {
    if (cor_borda == NULL) {
        LOG_ERRO("Não é possível definir NULL como cor da borda de um %s!\n",
                 figura_obter_tipo(poligono));
        return;
    }
    PoligonoImp *polImp = poligono;
    strcpy(polImp->cor_borda, cor_borda);
}

// Retorna a cor de preenchimento de um polígono..
const char *poligono_obter_cor_preenchimento(Poligono poligono) {
    PoligonoImp *polImp = poligono;
    return polImp->cor_preenchimento;
}

// Define a cor de preenchimento de um polígono.
void poligono_definir_cor_preenchimento(Poligono poligono, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        LOG_ERRO("Não é possível definir NULL como cor de preenchimento de um %s!\n",
                 figura_obter_tipo(poligono));
        return;
    }
    PoligonoImp *polImp = poligono;
    strcpy(polImp->cor_preenchimento, cor_preenchimento);
}

// Libera a memória alocada por polígono.
void poligono_destruir(Poligono poligono) {
    PoligonoImp *polImp = poligono;
    // Libera cada linha da matriz.
    for (int j = 0; j < polImp->numero_de_pontos; j++)
        free(polImp->pontos[j]);
    free(polImp->pontos);
    free(polImp->vtable);
    free(polImp);
}
