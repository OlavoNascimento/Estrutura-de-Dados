#include "caso.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"
#include "../Formas/retangulo.h"
#include "../Outros/texto.h"
#include "quadra.h"

struct Caso_s {
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
    int numero_de_casos;
};

const char *caso_obter_tipo() {
    return "caso";
}

// Escreve no svg as informações de um caso.
void caso_escrever_svg(Caso caso, FILE *arquivo) {
    retangulo_escrever_svg((Retangulo) caso, arquivo);

    double x = figura_obter_x_centro(caso);
    double y = figura_obter_y_centro(caso) + 5;
    // Converte o número de casos para uma string.
    char conteudo[1024];
    snprintf(conteudo, 500, "%d", caso->numero_de_casos);
    // Rótulo do caso.
    Texto texto_quadra = texto_criar("", x, y, "none", "white", conteudo, true);
    texto_escrever_svg(texto_quadra, arquivo);
    texto_destruir(texto_quadra);
}

// Escreve todos os dados de um caso em um arquivo.
void caso_escrever_informacoes(Caso caso, FILE *arquivo) {
    fprintf(arquivo, "tipo: %s,", figura_obter_tipo(caso));
    if (strlen(caso->id) > 0)
        fprintf(arquivo, " id: %s,", caso->id);
    fprintf(arquivo,
            " número de casos: %d, largura: %lf, altura: %lf, x: %lf, y: %lf, corb: %s, corp: %s\n",
            caso->numero_de_casos, caso->largura, caso->altura, caso->x, caso->y, caso->cor_borda,
            caso->cor_preenchimento);
}

// Registra as funções do objeto Caso na interface FiguraInterface.
// Como o tipo Caso é derivado do tipo Retangulo as funções podem ser reaproveitadas.
static FiguraInterface caso_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, caso_obter_tipo);

    figura_registrar_escrever_informacoes(interface,
                                          (EscreverInformacoes *) caso_escrever_informacoes);
    figura_registrar_escrever_svg(interface, (EscreverSvg *) caso_escrever_svg);

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

// Cria e inicializa um Caso com os valores passados.
Caso caso_criar(int casos, Quadra quadra, char face, int numero) {
    if (casos <= 0) {
        LOG_AVISO("Não é possível criar um caso menor ou igual a zero!\n");
        return NULL;
    }
    if (quadra == NULL) {
        LOG_AVISO("Não é possível criar um caso com uma quadra nula!\n");
        return NULL;
    }

    Caso caso = malloc(sizeof *caso);
    if (caso == NULL) {
        LOG_ERRO("Falha ao alocar memória\n");
        return NULL;
    }
    strcpy(caso->id, "");
    caso->largura = 16;
    caso->altura = 16;
    caso->x = 0;
    caso->y = 0;
    strcpy(caso->cor_borda, "red");
    strcpy(caso->cor_preenchimento, "orange");
    caso->arredondamento_borda = 0;
    caso->borda_tracejada = false;
    strcpy(caso->espessura_borda, "1px");
    caso->numero_de_casos = casos;

    quadra_inicializar_coordenada(&caso->x, &caso->y, caso->largura, caso->altura, quadra, face,
                                  numero);

    caso->vtable = caso_criar_interface_figura();
    return caso;
}

// Cria um caso com base em informações de uma linha.
Caso caso_ler(const char *linha, Quadra quadra) {
    int casos;
    int numero;
    char face;
    sscanf(linha, "cv %d %*s %c %d", &casos, &face, &numero);
    return caso_criar(casos, quadra, face, numero);
}

// Retorna o número de casos armazenado em um caso.
int caso_obter_numero_de_casos(Caso caso) {
    return caso->numero_de_casos;
}

// Retorna a coordenada x de um caso.
double caso_obter_x(Caso caso) {
    return retangulo_obter_x((Retangulo) caso);
}

// Retorna a coordenada y de um caso.
double caso_obter_y(Caso caso) {
    return retangulo_obter_y((Retangulo) caso);
}

// Retorna a largura de um caso.
double caso_obter_largura(Caso caso) {
    return retangulo_obter_largura((Retangulo) caso);
}

// Retorna a altura de um caso.
double caso_obter_altura(Caso caso) {
    return retangulo_obter_altura((Retangulo) caso);
}

// Retorna a cor da borda de um caso.
const char *caso_obter_cor_borda(Caso caso) {
    return retangulo_obter_cor_borda((Retangulo) caso);
}

// Define a cor da borda de um caso.
void caso_definir_cor_borda(Caso caso, const char *cor_borda) {
    retangulo_definir_cor_borda((Retangulo) caso, cor_borda);
}

// Retorna a cor de preenchimento de um caso.
const char *caso_obter_cor_preenchimento(Caso caso) {
    return retangulo_obter_cor_preenchimento((Retangulo) caso);
}

// Define a cor de preenchimento de um caso.
void caso_definir_cor_preenchimento(Caso caso, const char *cor_preenchimento) {
    retangulo_definir_cor_preenchimento((Retangulo) caso, cor_preenchimento);
}

// Define a espessura da borda de um caso.
void caso_definir_espessura_borda(Caso caso, const char *espessura_borda) {
    retangulo_definir_espessura_borda((Retangulo) caso, espessura_borda);
}

// Define o arredondamento da borda de um caso.
void caso_definir_arredondamento_borda(Caso caso, double raio_borda) {
    retangulo_definir_arredondamento_borda((Retangulo) caso, raio_borda);
}

// Define se a borda de um caso é tracejada.
void caso_definir_borda_tracejada(Caso caso, bool tracejado) {
    retangulo_definir_borda_tracejada((Retangulo) caso, tracejado);
}

// Libera a memória alocada por um caso.
void caso_destruir(Caso caso) {
    retangulo_destruir((Retangulo) caso);
}
