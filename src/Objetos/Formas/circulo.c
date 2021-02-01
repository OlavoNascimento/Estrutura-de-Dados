#include "circulo.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"

typedef struct {
    FiguraInterface vtable;
    char id[100];
    double raio;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    char espessura_borda[20];
    double opacidade;
} CirculoImp;

const char *circulo_obter_tipo() {
    return "círculo";
}

// Conecta as funções do objeto círculo com as da interface figura.
static FiguraInterface circulo_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, circulo_obter_tipo);

    figura_registrar_escrever_informacoes(interface, circulo_escrever_informacoes);
    figura_registrar_escrever_svg(interface, circulo_escrever_svg);

    figura_registrar_obter_id(interface, circulo_obter_id);

    figura_registrar_obter_x(interface, circulo_obter_x);
    figura_registrar_obter_y(interface, circulo_obter_y);

    figura_registrar_obter_x_inicio(interface, circulo_obter_x_inicio);
    figura_registrar_obter_y_inicio(interface, circulo_obter_y_inicio);

    figura_registrar_obter_x_fim(interface, circulo_obter_x_fim);
    figura_registrar_obter_y_fim(interface, circulo_obter_y_fim);

    figura_registrar_obter_x_centro(interface, circulo_obter_x);
    figura_registrar_obter_y_centro(interface, circulo_obter_y);

    figura_registrar_obter_cor_borda(interface, circulo_obter_cor_borda);
    figura_registrar_definir_cor_borda(interface, circulo_definir_cor_borda);

    figura_registrar_obter_cor_preenchimento(interface, circulo_obter_cor_preenchimento);
    figura_registrar_definir_cor_preenchimento(interface, circulo_definir_cor_preenchimento);

    figura_registrar_destruir(interface, circulo_destruir);
    return interface;
}

// Cria e inicializa um struct CirculoImp com os valores passados.
Circulo circulo_criar(const char id[100], double raio, double x, double y, const char cor_borda[20],
                      const char cor_preenchimento[20]) {
    if (id == NULL) {
        LOG_ERRO("Não é possível criar um círculo com id NULL!\n");
        return NULL;
    }
    if (cor_borda == NULL) {
        LOG_ERRO("Não é possível criar um círculo com cor de borda NULL!\n");
        return NULL;
    }
    if (cor_preenchimento == NULL) {
        LOG_ERRO("Não é possível criar um círculo com cor de preenchimento NULL!\n");
        return NULL;
    }
    CirculoImp *cirImp = malloc(sizeof *cirImp);
    strcpy(cirImp->id, id);
    cirImp->raio = raio;
    cirImp->x = x;
    cirImp->y = y;
    strcpy(cirImp->cor_borda, cor_borda);
    strcpy(cirImp->cor_preenchimento, cor_preenchimento);
    strcpy(cirImp->espessura_borda, "1px");
    cirImp->opacidade = 1;

    cirImp->vtable = circulo_criar_interface_figura();
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

// Retorna verdadeiro se dois círculos se intersectam.
bool circulo_checar_interseccao(Circulo circulo1, Circulo circulo2) {
    CirculoImp *cirImp1 = circulo1;
    CirculoImp *cirImp2 = circulo2;
    double dist = (cirImp1->x - cirImp2->x) * (cirImp1->x - cirImp2->x) +
                  (cirImp1->y - cirImp2->y) * (cirImp1->y - cirImp2->y);
    double raios = (cirImp1->raio + cirImp2->raio) * (cirImp1->raio + cirImp2->raio);
    return dist <= raios;
}

// Retorna verdadeiro se o circulo1 contem o círculo2.
bool circulo_contem_circulo(Circulo circulo1, Circulo circulo2) {
    CirculoImp *cirImp1 = circulo1;
    CirculoImp *cirImp2 = circulo2;
    double dist = sqrt(pow(cirImp2->x - cirImp1->x, 2) + pow(cirImp2->y - cirImp1->y, 2));
    return cirImp1->raio >= dist + cirImp2->raio;
}

// Retorna verdadeiro se um ponto se encontra dentro de um círculo.
bool circulo_checar_ponto_interno(Circulo circulo, double ponto_x, double ponto_y) {
    CirculoImp *cirImp = circulo;
    return pow((ponto_x - cirImp->x), 2) + pow((ponto_y - cirImp->y), 2) <= pow(cirImp->raio, 2);
}

// Escreve todos os dados de um círculo em um arquivo.
void circulo_escrever_informacoes(Circulo circulo, FILE *arquivo) {
    CirculoImp *cirImp = circulo;
    fprintf(arquivo, "tipo: %s,", figura_obter_tipo(circulo));
    if (strlen(cirImp->id) > 0)
        fprintf(arquivo, " id: %s,", cirImp->id);
    fprintf(arquivo, " raio: %lf, x: %lf, y: %lf, corb: %s, corp: %s", cirImp->raio, cirImp->x,
            cirImp->y, cirImp->cor_borda, cirImp->cor_preenchimento);
    if (cirImp->opacidade != 1)
        fprintf(arquivo, ", opacidade: %lf", cirImp->opacidade);
    fprintf(arquivo, "\n");
}

// Escreve o código svg que representa um círculo em um arquivo.
void circulo_escrever_svg(Circulo circulo, FILE *arquivo) {
    CirculoImp *cirImp = circulo;
    fprintf(arquivo, "\t<circle ");
    if (strlen(cirImp->id) > 0)
        fprintf(arquivo, "id='%s' ", cirImp->id);

    fprintf(arquivo,
            "r='%lf' cx='%lf' cy='%lf' stroke='%s' fill='%s' stroke-width='%s' opacity='%lf'/>\n",
            cirImp->raio, cirImp->x, cirImp->y, cirImp->cor_borda, cirImp->cor_preenchimento,
            cirImp->espessura_borda, cirImp->opacidade);
}

// Retorna o id de um círculo.
const char *circulo_obter_id(Circulo circulo) {
    CirculoImp *cirImp = circulo;
    return cirImp->id;
}

// Retorna a coordenada x de um círculo.
double circulo_obter_x(Circulo circulo) {
    CirculoImp *cirImp = circulo;
    return cirImp->x;
}

// Retorna a coordenada y de um círculo.
double circulo_obter_y(Circulo circulo) {
    CirculoImp *cirImp = circulo;
    return cirImp->y;
}

// Retorna a coordenada x onde um círculo se inicia.
double circulo_obter_x_inicio(Circulo circulo) {
    CirculoImp *cirImp = circulo;
    return cirImp->x - cirImp->raio;
}

// Retorna a coordenada y onde um círculo se inicia.
double circulo_obter_y_inicio(Circulo circulo) {
    CirculoImp *cirImp = circulo;
    return cirImp->y - cirImp->raio;
}

// Retorna a coordenada x onde um círculo termina.
double circulo_obter_x_fim(Circulo circulo) {
    CirculoImp *cirImp = circulo;
    return cirImp->x + cirImp->raio;
}

// Retorna a coordenada y onde um círculo termina.
double circulo_obter_y_fim(Circulo circulo) {
    CirculoImp *cirImp = circulo;
    return cirImp->y + cirImp->raio;
}

// Retorna o raio de um círculo.
double circulo_obter_raio(Circulo circulo) {
    CirculoImp *cirImp = circulo;
    return cirImp->raio;
}

// Retorna a cor da borda de um círculo.
const char *circulo_obter_cor_borda(Circulo circulo) {
    CirculoImp *cirImp = circulo;
    return cirImp->cor_borda;
}

// Define a cor da borda de um círculo.
void circulo_definir_cor_borda(Circulo circulo, const char *cor_borda) {
    if (cor_borda == NULL) {
        LOG_ERRO("Não é possível definir NULL como cor da borda de um %s!\n",
                 figura_obter_tipo(circulo));
        return;
    }
    CirculoImp *cirImp = circulo;
    strcpy(cirImp->cor_borda, cor_borda);
}

// Retorna a cor de preenchimento de um círculo..
const char *circulo_obter_cor_preenchimento(Circulo circulo) {
    CirculoImp *cirImp = circulo;
    return cirImp->cor_preenchimento;
}

// Define a cor de preenchimento de um círculo.
void circulo_definir_cor_preenchimento(Circulo circulo, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        LOG_ERRO("Não é possível definir NULL como cor de preenchimento de um %s!\n",
                 figura_obter_tipo(circulo));
        return;
    }
    CirculoImp *cirImp = circulo;
    strcpy(cirImp->cor_preenchimento, cor_preenchimento);
}

// Define a espessura da borda de um círculo.
void circulo_definir_espessura_borda(Circulo circulo, const char *espessura_borda) {
    if (espessura_borda == NULL) {
        LOG_ERRO("Não é possível definir NULL como tamanho da espessura da borda de um %s!\n",
                 figura_obter_tipo(circulo));
        return;
    }
    CirculoImp *cirImp = circulo;
    strcpy(cirImp->espessura_borda, espessura_borda);
}

// Define a opacidade de um círculo.
void circulo_definir_opacidade(Circulo circulo, double opacidade) {
    if (opacidade < 0 || opacidade > 1) {
        LOG_ERRO("A opacidade de um %s deve ser maior ou igual a zero 0 e menor ou igual a 1!\n",
                 figura_obter_tipo(circulo));
        return;
    }
    CirculoImp *cirImp = circulo;
    cirImp->opacidade = opacidade;
}

// Libera a memória alocada por um círculo.
void circulo_destruir(Circulo circulo) {
    CirculoImp *cirImp = circulo;
    free(cirImp->vtable);
    free(cirImp);
}
