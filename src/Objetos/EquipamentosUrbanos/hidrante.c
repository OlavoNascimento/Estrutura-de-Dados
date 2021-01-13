#include "hidrante.h"

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
} HidranteImp;

const char *hidrante_obter_tipo() {
    return "hidrante";
}

// Escreve no svg as informações de um hidrante.
void hidrante_escrever_svg(Hidrante hidrante, FILE *arquivo) {
    HidranteImp *hidImp = (HidranteImp *) hidrante;
    circulo_escrever_svg(hidrante, arquivo);

    // Rótulo do hidrante.
    Texto texto_hidrante = texto_criar("", hidImp->x, hidImp->y + 4, "none", "white", "H", true);
    texto_escrever_svg(texto_hidrante, arquivo);
    texto_destruir(texto_hidrante);
}

// Conecta as funções do objeto Hidrante com as da interface figura.
// Como o struct HidranteImp é idêntico ao struct CirculoImp as funções utilizadas em um objeto
// Circulo podem ser reaproveitadas.
static FiguraInterface hidrante_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();

    figura_registrar_obter_id(interface, circulo_obter_id);

    figura_registrar_obter_cor_borda(interface, circulo_obter_cor_borda);
    figura_registrar_definir_cor_borda(interface, circulo_definir_cor_borda);

    figura_registrar_obter_cor_preenchimento(interface, circulo_obter_cor_preenchimento);
    figura_registrar_definir_cor_preenchimento(interface, circulo_definir_cor_preenchimento);

    figura_registrar_obter_x(interface, circulo_obter_x);
    figura_registrar_obter_y(interface, circulo_obter_y);

    figura_registrar_escrever_informacoes(interface, circulo_escrever_informacoes);
    figura_registrar_escrever_svg(interface, hidrante_escrever_svg);

    figura_registrar_obter_tipo(interface, hidrante_obter_tipo);

    figura_registrar_obter_x_inicio(interface, circulo_obter_x_inicio);
    figura_registrar_obter_y_inicio(interface, circulo_obter_y_inicio);

    figura_registrar_obter_x_fim(interface, circulo_obter_x_fim);
    figura_registrar_obter_y_fim(interface, circulo_obter_y_fim);

    figura_registrar_obter_x_centro(interface, circulo_obter_x);
    figura_registrar_obter_y_centro(interface, circulo_obter_y);

    figura_registrar_destruir(interface, circulo_destruir);
    return interface;
}

// Cria e inicializa um struct HidranteImp com os valores passados.
Hidrante hidrante_criar(const char id[100], double raio, double x, double y,
                        const char cor_borda[20], const char cor_preenchimento[20]) {
    if (id == NULL) {
        LOG_ERRO("Não é possível criar um hidrante com id NULL!\n");
        return NULL;
    }
    if (cor_borda == NULL) {
        LOG_ERRO("Não é possível criar um hidrante com cor de borda NULL!\n");
        return NULL;
    }
    if (cor_preenchimento == NULL) {
        LOG_ERRO("Não é possível criar um hidrante com cor de preenchimento NULL!\n");
        return NULL;
    }
    HidranteImp *hidImp = malloc(sizeof *hidImp);
    strcpy(hidImp->id, id);
    hidImp->raio = raio;
    hidImp->x = x;
    hidImp->y = y;
    strcpy(hidImp->cor_borda, cor_borda);
    strcpy(hidImp->cor_preenchimento, cor_preenchimento);
    strcpy(hidImp->espessura_borda, "1px");

    hidImp->vtable = hidrante_criar_interface_figura();
    return hidImp;
}

// Cria um hidrante com base em informações de uma linha.
Hidrante hidrante_ler(const char *linha) {
    char id[100];
    double raio = 10;
    double x;
    double y;
    char cor_borda[20] = "red";
    char cor_preenchimento[20] = "red";
    sscanf(linha, "%*s %s %lf %lf", id, &x, &y);
    return hidrante_criar(id, raio, x, y, cor_borda, cor_preenchimento);
}

// Retorna o id de um hidrante.
const char *hidrante_obter_id(Hidrante hidrante) {
    return circulo_obter_id(hidrante);
}

// Retorna a coordenada y de um hidrante. O hidrante é considerada apenas um ponto.
double hidrante_obter_x(Hidrante hidrante) {
    return circulo_obter_x(hidrante);
}

// Retorna a coordenada y de um hidrante. O hidrante é considerada apenas um ponto.
double hidrante_obter_y(Hidrante hidrante) {
    return circulo_obter_y(hidrante);
}

// Retorna o raio de um hidrante. Usado apenas para cálculo interno, já que o hidrante é considerado
// um ponto.
double hidrante_obter_raio(Hidrante hidrante) {
    return circulo_obter_raio(hidrante);
}

// Retorna a cor da borda de um hidrante.
const char *hidrante_obter_cor_borda(Hidrante hidrante) {
    return circulo_obter_cor_borda(hidrante);
}

// Define a cor da borda de um hidrante.
void hidrante_definir_cor_borda(Hidrante hidrante, const char *cor_borda) {
    circulo_definir_cor_borda(hidrante, cor_borda);
}

// Retorna a cor de preenchimento de um hidrante.
const char *hidrante_obter_cor_preenchimento(Hidrante hidrante) {
    return circulo_obter_cor_preenchimento(hidrante);
}

// Define a cor de preenchimento de um hidrante.
void hidrante_definir_cor_preenchimento(Hidrante hidrante, const char *cor_preenchimento) {
    circulo_definir_cor_preenchimento(hidrante, cor_preenchimento);
}

// Define a espessura da borda de um hidrante.
void hidrante_definir_espessura_borda(Hidrante hidrante, const char *espessura_borda) {
    circulo_definir_espessura_borda(hidrante, espessura_borda);
}

// Libera a memória alocada por um hidrante.
void hidrante_destruir(Hidrante hidrante) {
    circulo_destruir(hidrante);
}
