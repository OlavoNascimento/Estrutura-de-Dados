#include "posto.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"
#include "../Formas/circulo.h"
#include "../Outros/texto.h"

struct Posto_s {
    FiguraInterface vtable;
    char id[100];
    double raio;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    char espessura_borda[20];
    double opacidade;
};

const char *posto_obter_tipo() {
    return "posto";
}

// Escreve no svg as informações de um posto.
void posto_escrever_svg(Posto posto, FILE *arquivo) {
    circulo_escrever_svg((Circulo) posto, arquivo);

    // Rótulo do posto.
    Texto texto_posto = texto_criar("", posto->x, posto->y + 4, "none", "white", "PC", true);
    texto_escrever_svg(texto_posto, arquivo);
    texto_destruir(texto_posto);
}

// Conecta as funções do objeto Posto com as da interface figura.
// Como o struct Hidrante_s é idêntica ao struct Circulo_s as funções utilizadas em um objeto
// Circulo podem ser reaproveitadas.
static FiguraInterface posto_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, posto_obter_tipo);

    figura_registrar_escrever_informacoes(interface,
                                          (EscreverInformacoes *) circulo_escrever_informacoes);
    figura_registrar_escrever_svg(interface, (EscreverSvg *) posto_escrever_svg);

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

// Cria e inicializa um Posto com os valores passados.
Posto posto_criar(double x, double y) {
    Posto posto = malloc(sizeof *posto);
    if (posto == NULL) {
        LOG_ERRO("Falha ao alocar memória\n");
        return NULL;
    }
    strcpy(posto->id, "");
    posto->raio = 10;
    posto->x = x;
    posto->y = y;
    strcpy(posto->cor_borda, "steelblue");
    strcpy(posto->cor_preenchimento, "green");
    strcpy(posto->espessura_borda, "1px");
    posto->opacidade = 1;

    posto->vtable = posto_criar_interface_figura();
    return posto;
}

// Cria um posto com base em informações de uma linha.
Posto posto_ler(const char *linha) {
    double x;
    double y;
    sscanf(linha, "ps %lf %lf", &x, &y);
    return posto_criar(x, y);
}

// Retorna a coordenada x de um posto. O posto é considerado apenas um ponto.
double posto_obter_x(Posto posto) {
    return circulo_obter_x((Circulo) posto);
}

// Retorna a coordenada y de um posto. O posto é considerado apenas um ponto.
double posto_obter_y(Posto posto) {
    return circulo_obter_y((Circulo) posto);
}

// Retorna o raio de um posto. Usado apenas para cálculo interno, já que o posto é considerado
// um ponto.
double posto_obter_raio(Posto posto) {
    return circulo_obter_raio((Circulo) posto);
}

// Retorna a cor de preenchimento de um posto.
const char *posto_obter_cor_preenchimento(Posto posto) {
    return circulo_obter_cor_preenchimento((Circulo) posto);
}

// Define a cor de preenchimento de um posto.
void posto_definir_cor_preenchimento(Posto posto, const char *cor_preenchimento) {
    circulo_definir_cor_preenchimento((Circulo) posto, cor_preenchimento);
}

// Retorna a cor da borda de um posto.
const char *posto_obter_cor_borda(Posto posto) {
    return circulo_obter_cor_borda((Circulo) posto);
}

// Define a cor da borda de um posto.
void posto_definir_cor_borda(Posto posto, const char *cor_borda) {
    circulo_definir_cor_borda((Circulo) posto, cor_borda);
}

// Define a espessura da borda de um posto.
void posto_definir_espessura_borda(Posto posto, const char *espessura_borda) {
    circulo_definir_espessura_borda((Circulo) posto, espessura_borda);
}

// Define a opacidade de um posto.
void posto_definir_opacidade(Posto posto, double opacidade) {
    circulo_definir_opacidade((Circulo) posto, opacidade);
}

// Libera a memória alocada por um posto.
void posto_destruir(Posto posto) {
    circulo_destruir((Circulo) posto);
}
