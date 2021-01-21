#include "radio.h"

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
} RadioImp;

const char *radio_obter_tipo() {
    return "rádio-base";
}

// Escreve no svg as informações de um rádio.
void radio_escrever_svg(Radio radio, FILE *arquivo) {
    RadioImp *radImp = radio;
    circulo_escrever_svg(radio, arquivo);

    // Rótulo do rádio.
    Texto texto_radio_base = texto_criar("", radImp->x, radImp->y + 5, "none", "white", "R", true);
    texto_escrever_svg(texto_radio_base, arquivo);
    texto_destruir(texto_radio_base);
}

// Conecta as funções do objeto Radio com as da interface figura.
// Como o struct RadioImp é idêntico ao struct CirculoImp as funções utilizadas em um objeto
// Circulo podem ser reaproveitadas.
static FiguraInterface radio_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_escrever_informacoes(interface, circulo_escrever_informacoes);
    figura_registrar_escrever_svg(interface, radio_escrever_svg);

    figura_registrar_obter_tipo(interface, radio_obter_tipo);

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

// Cria e inicializa um struct RadioImp com os valores passados.
Radio radio_criar(const char id[100], double raio, double x, double y) {
    if (id == NULL) {
        LOG_ERRO("Não é possível criar um rádio com id NULL!\n");
        return NULL;
    }
    RadioImp *radImp = malloc(sizeof *radImp);
    strcpy(radImp->id, id);
    radImp->raio = raio;
    radImp->x = x;
    radImp->y = y;
    strcpy(radImp->cor_borda, "purple");
    strcpy(radImp->cor_preenchimento, "purple");
    strcpy(radImp->espessura_borda, "1px");

    radImp->vtable = radio_criar_interface_figura();
    return radImp;
}

// Cria um rádio com base em informações de uma linha.
Radio radio_ler(const char *linha) {
    char id[100];
    double raio = 10;
    double x;
    double y;
    sscanf(linha, "%*s %s %lf %lf", id, &x, &y);
    return radio_criar(id, raio, x, y);
}

// Retorna o id de um rádio.
const char *radio_obter_id(Radio radio) {
    return circulo_obter_id(radio);
}

// Retorna a coordenada y de um rádio. O rádio é considerada apenas um ponto.
double radio_obter_x(Radio radio) {
    return circulo_obter_x(radio);
}

// Retorna a coordenada y de um rádio. O rádio é considerada apenas um ponto.
double radio_obter_y(Radio radio) {
    return circulo_obter_y(radio);
}

// Retorna o raio de um rádio. Usado apenas para cálculo interno, já que o rádio é considerado um
// ponto.
double radio_obter_raio(Radio radio) {
    return circulo_obter_raio(radio);
}

// Retorna a cor da borda de um rádio.
const char *radio_obter_cor_borda(Radio radio) {
    return circulo_obter_cor_borda(radio);
}

// Define a cor da borda de um rádio.
void radio_definir_cor_borda(Radio radio, const char *cor_borda) {
    circulo_definir_cor_borda(radio, cor_borda);
}

// Retorna a cor de preenchimento de um rádio.
const char *radio_obter_cor_preenchimento(Radio radio) {
    return circulo_obter_cor_preenchimento(radio);
}

// Define a cor de preenchimento de um rádio.
void radio_definir_cor_preenchimento(Radio radio, const char *cor_preenchimento) {
    circulo_definir_cor_preenchimento(radio, cor_preenchimento);
}

// Define a espessura da borda de um rádio.
void radio_definir_espessura_borda(Radio radio, const char *espessura_borda) {
    circulo_definir_espessura_borda(radio, espessura_borda);
}

// Libera a memória alocada por um rádio.
void radio_destruir(Radio radio) {
    circulo_destruir(radio);
}
