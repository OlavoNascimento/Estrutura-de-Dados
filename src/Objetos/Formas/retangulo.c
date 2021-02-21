#include "retangulo.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"

struct Retangulo_s {
    FiguraInterface vtable;
    char id[100];
    double largura;
    double altura;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    char espessura_borda[20];
    double arredondamento_borda;
    bool borda_tracejada;
};

const char *retangulo_obter_tipo() {
    return "retângulo";
}

// Registra as funções do objeto retângulo na interface FiguraInterface.
static FiguraInterface retangulo_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, retangulo_obter_tipo);

    figura_registrar_escrever_informacoes(interface,
                                          (EscreverInformacoes *) retangulo_escrever_informacoes);
    figura_registrar_escrever_svg(interface, (EscreverSvg *) retangulo_escrever_svg);

    figura_registrar_obter_id(interface, (ObterId *) retangulo_obter_id);

    figura_registrar_obter_x(interface, (ObterX *) retangulo_obter_x);
    figura_registrar_obter_y(interface, (ObterY *) retangulo_obter_y);

    figura_registrar_obter_x_inicio(interface, (ObterXInicio *) retangulo_obter_x);
    figura_registrar_obter_y_inicio(interface, (ObterYInicio *) retangulo_obter_y);

    figura_registrar_obter_x_fim(interface, (ObterXFim *) retangulo_obter_x_fim);
    figura_registrar_obter_y_fim(interface, (ObterYFim *) retangulo_obter_y_fim);

    figura_registrar_obter_x_centro(interface, (ObterXCentro *) retangulo_obter_x_centro);
    figura_registrar_obter_y_centro(interface, (ObterYCentro *) retangulo_obter_y_centro);

    figura_registrar_obter_cor_borda(interface, (ObterCorBorda *) retangulo_obter_cor_borda);
    figura_registrar_definir_cor_borda(interface, (DefinirCorBorda *) retangulo_definir_cor_borda);

    figura_registrar_obter_cor_preenchimento(
        interface, (ObterCorPreenchimento *) retangulo_obter_cor_preenchimento);
    figura_registrar_definir_cor_preenchimento(
        interface, (DefinirCorPreenchimento *) retangulo_definir_cor_preenchimento);

    figura_registrar_destruir(interface, (Destruir *) retangulo_destruir);
    return interface;
}

// Cria e inicializa um struct Retangulo com os valores passados.
Retangulo retangulo_criar(const char id[100], double largura, double altura, double x, double y,
                          const char cor_borda[20], const char cor_preenchimento[20]) {
    if (id == NULL) {
        LOG_AVISO("Não é possível criar um retângulo com id NULL!\n");
        return NULL;
    }
    if (cor_borda == NULL) {
        LOG_AVISO("Não é possível criar um retângulo com cor de borda NULL!\n");
        return NULL;
    }
    if (cor_preenchimento == NULL) {
        LOG_AVISO("Não é possível criar um retângulo com cor de preenchimento NULL!\n");
        return NULL;
    }
    Retangulo retangulo = malloc(sizeof *retangulo);
    if (retangulo == NULL) {
        LOG_ERRO("Falha ao alocar memória\n");
        return NULL;
    }
    strcpy(retangulo->id, id);
    retangulo->largura = largura;
    retangulo->altura = altura;
    retangulo->x = x;
    retangulo->y = y;
    strcpy(retangulo->cor_borda, cor_borda);
    strcpy(retangulo->cor_preenchimento, cor_preenchimento);
    retangulo->arredondamento_borda = 0;
    retangulo->borda_tracejada = false;
    strcpy(retangulo->espessura_borda, "1px");

    retangulo->vtable = retangulo_criar_interface_figura();
    return retangulo;
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
void retangulo_escrever_informacoes(Retangulo retangulo, FILE *arquivo) {
    fprintf(arquivo, "tipo: %s,", figura_obter_tipo(retangulo));
    if (strlen(retangulo->id) > 0)
        fprintf(arquivo, " id: %s,", retangulo->id);
    fprintf(arquivo, " largura: %lf, altura: %lf, x: %lf, y: %lf, corb: %s, corp: %s\n",
            retangulo->largura, retangulo->altura, retangulo->x, retangulo->y, retangulo->cor_borda,
            retangulo->cor_preenchimento);
}

// Escreve o código svg que representa um retângulo em um arquivo.
void retangulo_escrever_svg(Retangulo retangulo, FILE *arquivo) {
    fprintf(arquivo, "\t<rect");
    if (strlen(retangulo->id) > 0)
        fprintf(arquivo, " id='%s'", retangulo->id);

    fprintf(arquivo,
            " width='%lf' height='%lf' x='%lf' y='%lf' stroke='%s' fill='%s' rx='%lf' "
            "stroke-width='%s'",
            retangulo->largura, retangulo->altura, retangulo->x, retangulo->y, retangulo->cor_borda,
            retangulo->cor_preenchimento, retangulo->arredondamento_borda,
            retangulo->espessura_borda);

    if (retangulo->borda_tracejada)
        fprintf(arquivo, " style='stroke-dasharray: 2'");
    fprintf(arquivo, "/>\n");
}

// Retorna verdadeiro se dois retângulos se intersectam.
bool retangulo_checar_interseccao(Retangulo retangulo1, Retangulo retangulo2) {
    if (retangulo1->x > retangulo2->x + retangulo2->largura ||
        retangulo2->x > retangulo1->x + retangulo1->largura)
        return false;
    if (retangulo1->y + retangulo1->altura < retangulo2->y ||
        retangulo2->y + retangulo2->altura < retangulo1->y)
        return false;
    return true;
}

// Retorna verdadeiro se o retangulo1 contem o retangulo2.
bool retangulo_contem_retangulo(Retangulo retangulo1, Retangulo retangulo2) {
    return retangulo2->x > retangulo1->x && retangulo2->y > retangulo1->y &&
           retangulo2->x + retangulo2->largura < retangulo1->x + retangulo1->largura &&
           retangulo2->y + retangulo2->altura < retangulo1->y + retangulo1->altura;
}

// Retorna verdadeiro se um ponto se encontra dentro de um retângulo.
bool retangulo_checar_ponto_interno(Retangulo retangulo, double ponto_x, double ponto_y) {
    if (ponto_x < retangulo->x || ponto_x > retangulo->x + retangulo->largura)
        return false;
    if (ponto_y < retangulo->y || ponto_y > retangulo->y + retangulo->altura)
        return false;
    return true;
}

// Retorna o id de um retângulo.
const char *retangulo_obter_id(Retangulo retangulo) {
    return retangulo->id;
}

// Retorna a largura de um retângulo.
double retangulo_obter_largura(Retangulo retangulo) {
    return retangulo->largura;
}

// Retorna a altura de um retângulo.
double retangulo_obter_altura(Retangulo retangulo) {
    return retangulo->altura;
}

// Retorna a coordenada x de um retângulo.
double retangulo_obter_x(Retangulo retangulo) {
    return retangulo->x;
}

// Retorna a coordenada y de um retângulo.
double retangulo_obter_y(Retangulo retangulo) {
    return retangulo->y;
}

// Retorna a coordenada x onde um retângulo termina.
double retangulo_obter_x_fim(Retangulo retangulo) {
    return retangulo->x + retangulo->largura;
}

// Retorna a coordenada y onde um retângulo termina.
double retangulo_obter_y_fim(Retangulo retangulo) {
    return retangulo->y + retangulo->altura;
}

// Retorna a coordenada x do centro de um retângulo.
double retangulo_obter_x_centro(Retangulo retangulo) {
    return retangulo->x + retangulo->largura / 2;
}

// Retorna a coordenada y do centro de um retângulo.
double retangulo_obter_y_centro(Retangulo retangulo) {
    return retangulo->y + retangulo->altura / 2;
}

// Retorna a cor da borda de um retângulo.
const char *retangulo_obter_cor_borda(Retangulo retangulo) {
    return retangulo->cor_borda;
}

// Define a cor da borda de um retângulo.
void retangulo_definir_cor_borda(Retangulo retangulo, const char *cor_borda) {
    if (cor_borda == NULL) {
        LOG_AVISO("Não é possível definir NULL como cor da borda de um %s!\n",
                  figura_obter_tipo(retangulo));
        return;
    }

    strcpy(retangulo->cor_borda, cor_borda);
}

// Retorna a cor de preenchimento de um retângulo.
const char *retangulo_obter_cor_preenchimento(Retangulo retangulo) {
    return retangulo->cor_preenchimento;
}

// Define a cor de preenchimento de um retângulo.
void retangulo_definir_cor_preenchimento(Retangulo retangulo, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        LOG_AVISO("Não é possível definir NULL como cor de preenchimento de um %s!\n",
                  figura_obter_tipo(retangulo));
        return;
    }

    strcpy(retangulo->cor_preenchimento, cor_preenchimento);
}

// Define a espessura da borda de um retângulo.
void retangulo_definir_espessura_borda(Retangulo retangulo, const char *espessura_borda) {
    if (espessura_borda == NULL) {
        LOG_AVISO("Não é possível definir NULL como tamanho da espessura da borda de um %s!\n",
                  figura_obter_tipo(retangulo));
        return;
    }

    strcpy(retangulo->espessura_borda, espessura_borda);
}

// Define o arredondamento da borda do retângulo.
void retangulo_definir_arredondamento_borda(Retangulo retangulo, double arredondamento_borda) {
    retangulo->arredondamento_borda = arredondamento_borda;
}

// Define se a borda de um retângulo é tracejada.
void retangulo_definir_borda_tracejada(Retangulo retangulo, bool tracejado) {
    retangulo->borda_tracejada = tracejado;
}

// Libera a memória alocada por.retângulo.
void retangulo_destruir(Retangulo retangulo) {
    free(retangulo->vtable);
    free(retangulo);
}
