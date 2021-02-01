#include "posto.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"
#include "../Formas/circulo.h"
#include "../Outros/texto.h"

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
} PostoImp;

const char *posto_obter_tipo() {
    return "posto";
}

// Escreve no svg as informações de um posto.
void posto_escrever_svg(Posto posto, FILE *arquivo) {
    PostoImp *posImp = posto;
    circulo_escrever_svg(posto, arquivo);

    // Rótulo do posto.
    Texto texto_posto = texto_criar("", posImp->x, posImp->y + 4, "none", "white", "PC", true);
    texto_escrever_svg(texto_posto, arquivo);
    texto_destruir(texto_posto);
}

// Conecta as funções do objeto Posto com as da interface figura.
// Como o struct HidranteImp é idêntica ao struct CirculoImp as funções utilizadas em um objeto
// Circulo podem ser reaproveitadas.
static FiguraInterface posto_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, posto_obter_tipo);

    figura_registrar_escrever_informacoes(interface, circulo_escrever_informacoes);
    figura_registrar_escrever_svg(interface, posto_escrever_svg);

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

// Cria e inicializa um struct PostoImp com os valores passados.
Posto posto_criar(double x, double y) {
    PostoImp *posImp = malloc(sizeof *posImp);
    strcpy(posImp->id, "");
    posImp->raio = 10;
    posImp->x = x;
    posImp->y = y;
    strcpy(posImp->cor_borda, "steelblue");
    strcpy(posImp->cor_preenchimento, "green");
    strcpy(posImp->espessura_borda, "1px");
    posImp->opacidade = 1;

    posImp->vtable = posto_criar_interface_figura();
    return posImp;
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
    return circulo_obter_x(posto);
}

// Retorna a coordenada y de um posto. O posto é considerado apenas um ponto.
double posto_obter_y(Posto posto) {
    return circulo_obter_y(posto);
}

// Retorna o raio de um posto. Usado apenas para cálculo interno, já que o posto é considerado
// um ponto.
double posto_obter_raio(Posto posto) {
    return circulo_obter_raio(posto);
}

// Retorna a cor de preenchimento de um posto.
const char *posto_obter_cor_preenchimento(Posto posto) {
    return circulo_obter_cor_preenchimento(posto);
}

// Define a cor de preenchimento de um posto.
void posto_definir_cor_preenchimento(Posto posto, const char *cor_preenchimento) {
    circulo_definir_cor_preenchimento(posto, cor_preenchimento);
}

// Retorna a cor da borda de um posto.
const char *posto_obter_cor_borda(Posto posto) {
    return circulo_obter_cor_borda(posto);
}

// Define a cor da borda de um posto.
void posto_definir_cor_borda(Posto posto, const char *cor_borda) {
    circulo_definir_cor_borda(posto, cor_borda);
}

// Define a espessura da borda de um posto.
void posto_definir_espessura_borda(Posto posto, const char *espessura_borda) {
    circulo_definir_espessura_borda(posto, espessura_borda);
}

// Define a opacidade de um posto.
void posto_definir_opacidade(Posto posto, double opacidade) {
    circulo_definir_opacidade(posto, opacidade);
}

// Libera a memória alocada por um posto.
void posto_destruir(Posto posto) {
    circulo_destruir(posto);
}
