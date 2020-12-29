#include "hidrante.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "circulo.h"
#include "logging.h"
#include "texto.h"

typedef struct {
    Circulo circ;
} HidranteImp;

// Cria e inicializa um struct HidranteImp com os valores passados.
Hidrante hidrante_criar(const char id[100], double raio, double x, double y,
                        const char cor_borda[20], const char cor_preenchimento[20]) {
    if (id == NULL) {
        LOG_ERROR("Não é possível criar um hidrânte com id NULL!\n");
        return NULL;
    }
    if (cor_borda == NULL) {
        LOG_ERROR("Não é possível criar um hidrânte com cor de borda NULL!\n");
        return NULL;
    }
    if (cor_preenchimento == NULL) {
        LOG_ERROR("Não é possível criar um hidrânte com cor de preenchimento NULL!\n");
        return NULL;
    }
    HidranteImp *hidImp = malloc(sizeof(HidranteImp));
    hidImp->circ = circulo_criar(id, raio, x, y, cor_borda, cor_preenchimento);
    return hidImp;
}

// Cria um hidrante com base em informações de uma linha.
Hidrante hidrante_ler(const char *linha) {
    char id[100];
    double raio = 10;  // valor simbólico, padrão para todos os hidrantes
    double x;
    double y;
    char cor_borda[20] = "red";          // será alterado com outro comando
    char cor_preenchimento[20] = "red";  // será alterado com outro comando
    sscanf(linha, "%*s %s %lf %lf", id, &x, &y);
    return hidrante_criar(id, raio, x, y, cor_borda, cor_preenchimento);
}

// Retorna o id de um hidrante.
const char *hidrante_obter_id(Hidrante hidrante) {
    HidranteImp *hidImp = (HidranteImp *) hidrante;
    return circulo_obter_id(hidImp->circ);
}

// Escreve as informações de um hidrante.
void hidrante_escrever_informacoes(FILE *arquivo, Hidrante hidrante) {
    HidranteImp *hidImp = (HidranteImp *) hidrante;
    circulo_escrever_informacoes(arquivo, hidImp->circ);
}

// Escreve no svg as informações de um hidrante.
void hidrante_escrever_svg(FILE *arquivo, Hidrante hidrante) {
    HidranteImp *hidImp = (HidranteImp *) hidrante;
    circulo_escrever_svg(arquivo, hidImp->circ);
    double x = circulo_obter_x(hidImp->circ);
    double y = circulo_obter_y(hidImp->circ) + 4;
    Texto texto_hidrante = texto_criar("", x, y, "none", "white", "H", true);
    texto_escrever_svg(arquivo, texto_hidrante);
    texto_destruir(texto_hidrante);
}

// Retorna o raio de um hidrante. Usado apenas para cálculo interno, já que o hidrante é considerado
// um ponto.
double hidrante_obter_raio(Hidrante hidrante) {
    HidranteImp *hidImp = (HidranteImp *) hidrante;
    return circulo_obter_raio(hidImp->circ);
}

// Retorna a coordenada y de um hidrante. O hidrante é considerado apenas um ponto.
double hidrante_obter_y(Hidrante hidrante) {
    HidranteImp *hidImp = (HidranteImp *) hidrante;
    return circulo_obter_y(hidImp->circ);
}

// Retorna a coordenada x de um hidrante. O hidrante é considerado apenas um ponto.
double hidrante_obter_x(Hidrante hidrante) {
    HidranteImp *hidImp = (HidranteImp *) hidrante;
    return circulo_obter_x(hidImp->circ);
}

// Retorna a cor de preenchimento de um hidrante.
const char *hidrante_obter_cor_preenchimento(Hidrante hidrante) {
    HidranteImp *hidImp = (HidranteImp *) hidrante;
    return circulo_obter_cor_preenchimento(hidImp->circ);
}

// Define a cor de preenchimento de um hidrante.
void hidrante_definir_cor_preenchimento(Hidrante hidrante, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        LOG_ERROR("Não é possível definir NULL como cor de preenchimento de um hidrante!\n");
        return;
    }
    HidranteImp *hidImp = (HidranteImp *) hidrante;
    circulo_definir_cor_preenchimento(hidImp->circ, cor_preenchimento);
}

// Retorna a cor da borda de um hidrante.
const char *hidrante_obter_cor_borda(Hidrante hidrante) {
    HidranteImp *hidImp = (HidranteImp *) hidrante;
    return circulo_obter_cor_borda(hidImp->circ);
}

// Define a cor da borda de um hidrante.
void hidrante_definir_cor_borda(Hidrante hidrante, const char *cor_borda) {
    if (cor_borda == NULL) {
        LOG_ERROR("Não é possível definir NULL como cor da borda de um hidrante!\n");
        return;
    }
    HidranteImp *hidImp = (HidranteImp *) hidrante;
    circulo_definir_cor_borda(hidImp->circ, cor_borda);
}

// Define a espessura da borda de um hidrante.
void hidrante_definir_espessura_borda(Hidrante hidrante, const char *espessura_borda) {
    if (espessura_borda == NULL) {
        LOG_ERROR(
            "Não é possível definir NULL como tamanho da espessura da borda de um hidrante!\n");
        return;
    }
    HidranteImp *hidImp = (HidranteImp *) hidrante;
    circulo_definir_espessura_borda(hidImp->circ, espessura_borda);
}

// Libera a memória alocada por um hidrante.
void hidrante_destruir(Hidrante hidrante) {
    HidranteImp *hidImp = (HidranteImp *) hidrante;
    circulo_destruir(hidImp->circ);
    free(hidrante);
}