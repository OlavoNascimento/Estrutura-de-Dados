#include "retangulo.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"

typedef struct {
    FiguraInterface vtable;
    char id[100];
    double largura;
    double altura;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    double arredondamento_borda;
    char espessura_borda[20];
    bool borda_tracejada;
} RetanguloImp;

const char *retangulo_obter_tipo() {
    return "retângulo";
}

// Conecta as funções do objeto retângulo com as da interface figura.
static FiguraInterface retangulo_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_escrever_informacoes(interface, retangulo_escrever_informacoes);
    figura_registrar_escrever_svg(interface, retangulo_escrever_svg);

    figura_registrar_obter_tipo(interface, retangulo_obter_tipo);

    figura_registrar_obter_id(interface, retangulo_obter_id);

    figura_registrar_obter_x(interface, retangulo_obter_x);
    figura_registrar_obter_y(interface, retangulo_obter_y);

    figura_registrar_obter_x_inicio(interface, retangulo_obter_x);
    figura_registrar_obter_y_inicio(interface, retangulo_obter_y);

    figura_registrar_obter_x_fim(interface, retangulo_obter_x_fim);
    figura_registrar_obter_y_fim(interface, retangulo_obter_y_fim);

    figura_registrar_obter_x_centro(interface, retangulo_obter_x_centro);
    figura_registrar_obter_y_centro(interface, retangulo_obter_y_centro);

    figura_registrar_obter_cor_borda(interface, retangulo_obter_cor_borda);
    figura_registrar_definir_cor_borda(interface, retangulo_definir_cor_borda);

    figura_registrar_obter_cor_preenchimento(interface, retangulo_obter_cor_preenchimento);
    figura_registrar_definir_cor_preenchimento(interface, retangulo_definir_cor_preenchimento);

    figura_registrar_destruir(interface, retangulo_destruir);
    return interface;
}

// Cria e inicializa um struct Retangulo com os valores passados.
Retangulo retangulo_criar(const char id[100], double largura, double altura, double x, double y,
                          const char cor_borda[20], const char cor_preenchimento[20]) {
    if (id == NULL) {
        LOG_ERRO("Não é possível criar um retângulo com id NULL!\n");
        return NULL;
    }
    if (cor_borda == NULL) {
        LOG_ERRO("Não é possível criar um retângulo com cor de borda NULL!\n");
        return NULL;
    }
    if (cor_preenchimento == NULL) {
        LOG_ERRO("Não é possível criar um retângulo com cor de preenchimento NULL!\n");
        return NULL;
    }
    RetanguloImp *retImp = malloc(sizeof(RetanguloImp));
    strcpy(retImp->id, id);
    retImp->largura = largura;
    retImp->altura = altura;
    retImp->x = x;
    retImp->y = y;
    strcpy(retImp->cor_borda, cor_borda);
    strcpy(retImp->cor_preenchimento, cor_preenchimento);
    retImp->arredondamento_borda = 0;
    retImp->borda_tracejada = false;
    strcpy(retImp->espessura_borda, "1px");

    retImp->vtable = retangulo_criar_interface_figura();
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
void retangulo_escrever_informacoes(Retangulo retangulo, FILE *arquivo) {
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    fprintf(arquivo, "tipo: %s,", figura_obter_tipo(retangulo));
    if (strlen(retImp->id) > 0)
        fprintf(arquivo, " id: %s,", retImp->id);
    fprintf(arquivo, " largura: %lf, altura: %lf, x: %lf, y: %lf, corb: %s, corp: %s\n",
            retImp->largura, retImp->altura, retImp->x, retImp->y, retImp->cor_borda,
            retImp->cor_preenchimento);
}

// Escreve o código svg que representa um retângulo em um arquivo.
void retangulo_escrever_svg(Retangulo retangulo, FILE *arquivo) {
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    fprintf(arquivo, "\t<rect");
    if (strlen(retImp->id) > 0)
        fprintf(arquivo, " id='%s'", retImp->id);

    fprintf(arquivo,
            " width='%lf' height='%lf' x='%lf' y='%lf' stroke='%s' fill='%s' rx='%lf' "
            "stroke-width='%s'",
            retImp->largura, retImp->altura, retImp->x, retImp->y, retImp->cor_borda,
            retImp->cor_preenchimento, retImp->arredondamento_borda, retImp->espessura_borda);

    if (retImp->borda_tracejada)
        fprintf(arquivo, " style='stroke-dasharray: 2'");
    fprintf(arquivo, "/>\n");
}

// Retorna verdadeiro se dois retângulos se intersectam.
bool retangulo_checar_interseccao(Retangulo retangulo1, Retangulo retangulo2) {
    RetanguloImp *retImp1 = (RetanguloImp *) retangulo1;
    RetanguloImp *retImp2 = (RetanguloImp *) retangulo2;
    if (retImp1->x > retImp2->x + retImp2->largura || retImp2->x > retImp1->x + retImp1->largura)
        return false;
    if (retImp1->y + retImp1->altura < retImp2->y || retImp2->y + retImp2->altura < retImp1->y)
        return false;
    return true;
}

// Retorna verdadeiro se um ponto se encontra dentro de um retângulo.
bool retangulo_checar_ponto_interno(Retangulo retangulo, double ponto_x, double ponto_y) {
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    if (ponto_x <= retImp->x || ponto_x >= retImp->x + retImp->largura)
        return false;
    if (ponto_y <= retImp->y || ponto_y >= retImp->y + retImp->altura)
        return false;
    return true;
}

// Retorna o id de retângulo.
const char *retangulo_obter_id(Retangulo retangulo) {
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    return retImp->id;
}

// Retorna a largura de retângulo.
double retangulo_obter_largura(Retangulo retangulo) {
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    return retImp->largura;
}

// Retorna a altura de retângulo.
double retangulo_obter_altura(Retangulo retangulo) {
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    return retImp->altura;
}

// Retorna a coordenada x de retângulo.
double retangulo_obter_x(Retangulo retangulo) {
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    return retImp->x;
}

// Retorna a coordenada y de retângulo.
double retangulo_obter_y(Retangulo retangulo) {
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    return retImp->y;
}

// Retorna a coordenada x onde um retângulo termina.
double retangulo_obter_x_fim(Retangulo retangulo) {
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    return retImp->x + retImp->largura;
}

// Retorna a coordenada y onde um retângulo termina.
double retangulo_obter_y_fim(Retangulo retangulo) {
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    return retImp->y + retImp->altura;
}

// Retorna a coordenada x do centro de um retângulo.
double retangulo_obter_x_centro(Retangulo retangulo) {
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    return retImp->x + retImp->largura / 2;
}

// Retorna a coordenada y do centro de um retângulo.
double retangulo_obter_y_centro(Retangulo retangulo) {
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    return retImp->y + retImp->altura / 2;
}

// Retorna a cor da borda de retângulo.
const char *retangulo_obter_cor_borda(Retangulo retangulo) {
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    return retImp->cor_borda;
}

// Define a cor da borda de retângulo.
void retangulo_definir_cor_borda(Retangulo retangulo, const char *cor_borda) {
    if (cor_borda == NULL) {
        LOG_ERRO("Não é possível definir NULL como cor da borda de um %s!\n",
                 figura_obter_tipo(retangulo));
        return;
    }
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    strcpy(retImp->cor_borda, cor_borda);
}

// Retorna a cor de preenchimento de retângulo.
const char *retangulo_obter_cor_preenchimento(Retangulo retangulo) {
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    return retImp->cor_preenchimento;
}

// Define a cor de preenchimento de retângulo.
void retangulo_definir_cor_preenchimento(Retangulo retangulo, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        LOG_ERRO("Não é possível definir NULL como cor de preenchimento de um %s!\n",
                 figura_obter_tipo(retangulo));
        return;
    }
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    strcpy(retImp->cor_preenchimento, cor_preenchimento);
}

// Define a espessura da borda de retângulo.
void retangulo_definir_espessura_borda(Retangulo retangulo, const char *espessura_borda) {
    if (espessura_borda == NULL) {
        LOG_ERRO("Não é possível definir NULL como tamanho da espessura da borda de um %s!\n",
                 figura_obter_tipo(retangulo));
        return;
    }
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    strcpy(retImp->espessura_borda, espessura_borda);
}

// Define se a borda do retângulo é tracejada.
void retangulo_definir_borda_tracejada(Retangulo retangulo, bool tracejado) {
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    retImp->borda_tracejada = tracejado;
}

// Define o arredondamento da borda do retângulo.
void retangulo_definir_arredondamento_borda(Retangulo retangulo, double arredondamento_borda) {
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    retImp->arredondamento_borda = arredondamento_borda;
}

// Libera a memória alocada por.retângulo.
void retangulo_destruir(Retangulo retangulo) {
    RetanguloImp *retImp = (RetanguloImp *) retangulo;
    free(retImp->vtable);
    free(retImp);
}
