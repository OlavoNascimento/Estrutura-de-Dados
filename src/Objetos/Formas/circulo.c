#include "circulo.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"

struct Circulo_s {
    FiguraInterface vtable;
    char id[100];
    double raio;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    double espessura_borda;
    double opacidade;
};

const char *circulo_obter_tipo() {
    return "círculo";
}

// Registra as funções do objeto círculo na interface FiguraInterface.
static FiguraInterface circulo_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, circulo_obter_tipo);

    figura_registrar_escrever_informacoes(interface,
                                          (EscreverInformacoes *) circulo_escrever_informacoes);
    figura_registrar_escrever_svg(interface, (EscreverSvg *) circulo_escrever_svg);

    figura_registrar_obter_id(interface, (ObterId *) circulo_obter_id);

    figura_registrar_obter_x(interface, (ObterX *) circulo_obter_x);
    figura_registrar_obter_y(interface, (ObterY *) circulo_obter_y);

    figura_registrar_obter_x_inicio(interface, (ObterXInicio *) circulo_obter_x_inicio);
    figura_registrar_obter_y_inicio(interface, (ObterYInicio *) circulo_obter_y_inicio);

    figura_registrar_obter_x_fim(interface, (ObterXFim *) circulo_obter_x_fim);
    figura_registrar_obter_y_fim(interface, (ObterYFim *) circulo_obter_y_fim);

    figura_registrar_obter_x_centro(interface, (ObterXCentro *) circulo_obter_x);
    figura_registrar_obter_y_centro(interface, (ObterYCentro *) circulo_obter_y);

    figura_registrar_obter_cor_borda(interface, (ObterCorBorda *) circulo_obter_cor_borda);
    figura_registrar_definir_cor_borda(interface, (DefinirCorBorda *) circulo_definir_cor_borda);

    figura_registrar_obter_cor_preenchimento(
        interface, (ObterCorPreenchimento *) circulo_obter_cor_preenchimento);
    figura_registrar_definir_cor_preenchimento(
        interface, (DefinirCorPreenchimento *) circulo_definir_cor_preenchimento);

    figura_registrar_destruir(interface, (Destruir *) circulo_destruir);
    return interface;
}

// Cria e inicializa um Circulo com os valores passados.
Circulo circulo_criar(const char id[100], double raio, double x, double y, const char cor_borda[20],
                      const char cor_preenchimento[20]) {
    if (id == NULL) {
        LOG_AVISO("Não é possível criar um círculo com id NULL!\n");
        return NULL;
    }
    if (cor_borda == NULL) {
        LOG_AVISO("Não é possível criar um círculo com cor de borda NULL!\n");
        return NULL;
    }
    if (cor_preenchimento == NULL) {
        LOG_AVISO("Não é possível criar um círculo com cor de preenchimento NULL!\n");
        return NULL;
    }
    Circulo circulo = malloc(sizeof *circulo);
    if (circulo == NULL) {
        LOG_ERRO("Falha ao alocar memória\n");
        return NULL;
    }
    strcpy(circulo->id, id);
    circulo->raio = raio;
    circulo->x = x;
    circulo->y = y;
    strcpy(circulo->cor_borda, cor_borda);
    strcpy(circulo->cor_preenchimento, cor_preenchimento);
    circulo->espessura_borda = 1;
    circulo->opacidade = 1;

    circulo->vtable = circulo_criar_interface_figura();
    return circulo;
}

// Cria um círculo com base em informações de uma linha.
Circulo circulo_ler(const char *linha) {
    char id[100];
    double raio;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    sscanf(linha, "c %s %lf %lf %lf %s %s", id, &raio, &x, &y, cor_borda, cor_preenchimento);
    return circulo_criar(id, raio, x, y, cor_borda, cor_preenchimento);
}

// Escreve todos os dados de um círculo em um arquivo.
void circulo_escrever_informacoes(Circulo circulo, FILE *arquivo) {
    fprintf(arquivo, "tipo: %s,", figura_obter_tipo(circulo));
    if (strlen(circulo->id) > 0)
        fprintf(arquivo, " id: %s,", circulo->id);
    fprintf(arquivo, " raio: %lf, x: %lf, y: %lf, corb: %s, corp: %s", circulo->raio, circulo->x,
            circulo->y, circulo->cor_borda, circulo->cor_preenchimento);
    if (circulo->opacidade != 1)
        fprintf(arquivo, ", opacidade: %lf", circulo->opacidade);
    fprintf(arquivo, "\n");
}

// Escreve o código svg que representa um círculo em um arquivo.
void circulo_escrever_svg(Circulo circulo, FILE *arquivo) {
    fprintf(arquivo, "\t<circle ");
    if (strlen(circulo->id) > 0)
        fprintf(arquivo, "id='%s' ", circulo->id);
    fprintf(arquivo,
            "r='%lf' cx='%lf' cy='%lf' stroke='%s' fill='%s' stroke-width='%fpx' opacity='%f'/>\n",
            circulo->raio, circulo->x, circulo->y, circulo->cor_borda, circulo->cor_preenchimento,
            circulo->espessura_borda, circulo->opacidade);
}

// Retorna verdadeiro se dois círculos se intersectam.
bool circulo_checar_interseccao(Circulo circulo1, Circulo circulo2) {
    return pow(circulo1->x - circulo2->x, 2) + pow(circulo1->y - circulo2->y, 2) <=
           pow(circulo1->raio + circulo2->raio, 2);
}

// Retorna verdadeiro se o circulo1 contem o círculo2.
bool circulo_contem_circulo(Circulo circulo1, Circulo circulo2) {
    return circulo1->raio >
           circulo2->raio + hypot(circulo2->x - circulo1->x, circulo2->y - circulo1->y);
}

// Retorna verdadeiro se um ponto se encontra dentro de um círculo.
bool circulo_checar_ponto_interno(Circulo circulo, double ponto_x, double ponto_y) {
    return pow(ponto_x - circulo->x, 2) + pow(ponto_y - circulo->y, 2) < pow(circulo->raio, 2);
}

// Retorna o id de um círculo.
const char *circulo_obter_id(Circulo circulo) {
    return circulo->id;
}

// Retorna a coordenada x de um círculo.
double circulo_obter_x(Circulo circulo) {
    return circulo->x;
}

// Retorna a coordenada y de um círculo.
double circulo_obter_y(Circulo circulo) {
    return circulo->y;
}

// Retorna a coordenada x onde um círculo se inicia.
double circulo_obter_x_inicio(Circulo circulo) {
    return circulo->x - circulo->raio;
}

// Retorna a coordenada y onde um círculo se inicia.
double circulo_obter_y_inicio(Circulo circulo) {
    return circulo->y - circulo->raio;
}

// Retorna a coordenada x onde um círculo termina.
double circulo_obter_x_fim(Circulo circulo) {
    return circulo->x + circulo->raio;
}

// Retorna a coordenada y onde um círculo termina.
double circulo_obter_y_fim(Circulo circulo) {
    return circulo->y + circulo->raio;
}

// Retorna o raio de um círculo.
double circulo_obter_raio(Circulo circulo) {
    return circulo->raio;
}

// Retorna a cor da borda de um círculo.
const char *circulo_obter_cor_borda(Circulo circulo) {
    return circulo->cor_borda;
}

// Define a cor da borda de um círculo.
void circulo_definir_cor_borda(Circulo circulo, const char *cor_borda) {
    if (cor_borda == NULL) {
        LOG_AVISO("Não é possível definir NULL como cor da borda de um %s!\n",
                  figura_obter_tipo(circulo));
        return;
    }
    strcpy(circulo->cor_borda, cor_borda);
}

// Retorna a cor de preenchimento de um círculo..
const char *circulo_obter_cor_preenchimento(Circulo circulo) {
    return circulo->cor_preenchimento;
}

// Define a cor de preenchimento de um círculo.
void circulo_definir_cor_preenchimento(Circulo circulo, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        LOG_AVISO("Não é possível definir NULL como cor de preenchimento de um %s!\n",
                  figura_obter_tipo(circulo));
        return;
    }
    strcpy(circulo->cor_preenchimento, cor_preenchimento);
}

// Define a espessura da borda de um círculo.
void circulo_definir_espessura_borda(Circulo circulo, double espessura_borda) {
    if (espessura_borda < 0) {
        LOG_AVISO("Não é possível definir uma espessura de borda menor que 0 para um %s!\n",
                  figura_obter_tipo(circulo));
        return;
    }
    circulo->espessura_borda = espessura_borda;
}

// Define a opacidade de um círculo.
void circulo_definir_opacidade(Circulo circulo, double opacidade) {
    if (opacidade < 0 || opacidade > 1) {
        LOG_AVISO("A opacidade de um %s deve ser maior ou igual a zero 0 e menor ou igual a 1!\n",
                  figura_obter_tipo(circulo));
        return;
    }
    circulo->opacidade = opacidade;
}

// Libera a memória alocada por um círculo.
void circulo_destruir(Circulo circulo) {
    free(circulo->vtable);
    free(circulo);
}
