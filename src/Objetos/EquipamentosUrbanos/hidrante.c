#include "hidrante.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"
#include "../Formas/circulo.h"
#include "../Outros/texto.h"

struct Hidrante_s {
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

const char *hidrante_obter_tipo() {
    return "hidrante";
}

// Escreve no svg as informações de um hidrante.
void hidrante_escrever_svg(Hidrante hidrante, FILE *arquivo) {
    circulo_escrever_svg((Circulo) hidrante, arquivo);

    // Rótulo do hidrante.
    Texto texto_hidrante = texto_criar("", hidrante->x, hidrante->y + 5, "none", "white", "H");
    texto_definir_alinhamento(texto_hidrante, TEXTO_CENTRO);
    texto_escrever_svg(texto_hidrante, arquivo);
    texto_destruir(texto_hidrante);
}

// Registra as funções do objeto Hidrante na interface FiguraInterface.
// Como o tipo Hidrante é derivado do tipo Circulo as funções podem ser reaproveitadas.
static FiguraInterface hidrante_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, hidrante_obter_tipo);

    figura_registrar_escrever_informacoes(interface,
                                          (EscreverInformacoes *) circulo_escrever_informacoes);
    figura_registrar_escrever_svg(interface, (EscreverSvg *) hidrante_escrever_svg);

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

// Cria e inicializa um Hidrante com os valores passados.
Hidrante hidrante_criar(const char id[100], double x, double y) {
    if (id == NULL) {
        LOG_AVISO("Não é possível criar um hidrante com id NULL!\n");
        return NULL;
    }
    Hidrante hidrante = malloc(sizeof *hidrante);
    if (hidrante == NULL) {
        LOG_ERRO("Falha ao alocar memória\n");
        return NULL;
    }
    strcpy(hidrante->id, id);
    hidrante->raio = 10;
    hidrante->x = x;
    hidrante->y = y;
    strcpy(hidrante->cor_borda, "red");
    strcpy(hidrante->cor_preenchimento, "red");
    hidrante->espessura_borda = 1;
    hidrante->opacidade = 1;

    hidrante->vtable = hidrante_criar_interface_figura();
    return hidrante;
}

// Cria um hidrante com base em informações de uma linha.
Hidrante hidrante_ler(const char *linha) {
    char id[100];
    double x;
    double y;
    sscanf(linha, "%*s %s %lf %lf", id, &x, &y);
    return hidrante_criar(id, x, y);
}

// Retorna o id de um hidrante.
const char *hidrante_obter_id(Hidrante hidrante) {
    return circulo_obter_id((Circulo) hidrante);
}

// Retorna a coordenada y de um hidrante. O hidrante é considerada apenas um ponto.
double hidrante_obter_x(Hidrante hidrante) {
    return circulo_obter_x((Circulo) hidrante);
}

// Retorna a coordenada y de um hidrante. O hidrante é considerada apenas um ponto.
double hidrante_obter_y(Hidrante hidrante) {
    return circulo_obter_y((Circulo) hidrante);
}

// Retorna o raio de um hidrante. Usado apenas para cálculo interno, já que o hidrante é considerado
// um ponto.
double hidrante_obter_raio(Hidrante hidrante) {
    return circulo_obter_raio((Circulo) hidrante);
}

// Retorna a cor da borda de um hidrante.
const char *hidrante_obter_cor_borda(Hidrante hidrante) {
    return circulo_obter_cor_borda((Circulo) hidrante);
}

// Define a cor da borda de um hidrante.
void hidrante_definir_cor_borda(Hidrante hidrante, const char *cor_borda) {
    circulo_definir_cor_borda((Circulo) hidrante, cor_borda);
}

// Retorna a cor de preenchimento de um hidrante.
const char *hidrante_obter_cor_preenchimento(Hidrante hidrante) {
    return circulo_obter_cor_preenchimento((Circulo) hidrante);
}

// Define a cor de preenchimento de um hidrante.
void hidrante_definir_cor_preenchimento(Hidrante hidrante, const char *cor_preenchimento) {
    circulo_definir_cor_preenchimento((Circulo) hidrante, cor_preenchimento);
}

// Define a espessura da borda de um hidrante.
void hidrante_definir_espessura_borda(Hidrante hidrante, double espessura_borda) {
    circulo_definir_espessura_borda((Circulo) hidrante, espessura_borda);
}

// Define a opacidade de um hidrante.
void hidrante_definir_opacidade(Hidrante hidrante, double opacidade) {
    circulo_definir_opacidade((Circulo) hidrante, opacidade);
}

// Libera a memória alocada por um hidrante.
void hidrante_destruir(Hidrante hidrante) {
    circulo_destruir((Circulo) hidrante);
}
