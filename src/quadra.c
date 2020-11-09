
#include "quadra.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lista.h"
#include "logging.h"
#include "retangulo.h"

typedef struct {
    Retangulo ret;
} QuadraImp;

// Cria e inicializa um struct QuadraImp com os valores passados.
Quadra quadra_criar(const char id[100], double largura, double altura, double x, double y,
                    const char cor_borda[20], const char cor_preenchimento[20]) {
    if (id == NULL) {
        LOG_ERROR("Não é possível criar uma quadra com id NULL!\n");
        return NULL;
    }
    if (cor_borda == NULL) {
        LOG_ERROR("Não é possível criar uma quadra com cor de borda NULL!\n");
        return NULL;
    }
    if (cor_preenchimento == NULL) {
        LOG_ERROR("Não é possível criar uma quadra com cor de preenchimento NULL!\n");
        return NULL;
    }
    QuadraImp *quaImp = malloc(sizeof(QuadraImp));
    quaImp->ret = retangulo_criar(id, largura, altura, x, y, cor_borda, cor_preenchimento);
    return quaImp;
}

// Cria uma quadra com base em informações de uma linha.
Quadra quadra_ler(const char *linha) {
    char id[100];
    double largura;
    double altura;
    double x;
    double y;
    char cor_borda[20] = "saddlebrown";
    char cor_preenchimento[20] = "coral";
    sscanf(linha, "%*s %s %lf %lf %lf %lf", id, &x, &y, &largura, &altura);
    return quadra_criar(id, largura, altura, x, y, cor_borda, cor_preenchimento);
}

// Escreve as informações de uma radio base
void quadra_escrever_informacoes(FILE *arquivo, Quadra quadra) {
    QuadraImp *quaImp = (QuadraImp *) quadra;
    retangulo_escrever_informacoes(arquivo, quaImp->ret);
}

// Escreve no svg as informações de uma radio base
void quadra_escrever_svg(FILE *arquivo, Quadra quadra) {
    QuadraImp *quaImp = (QuadraImp *) quadra;
    retangulo_escrever_svg(arquivo, quaImp->ret);
    double x = retangulo_obter_x(quaImp->ret) + retangulo_obter_largura(quaImp->ret) / 2;
    double y = retangulo_obter_y(quaImp->ret) + retangulo_obter_altura(quaImp->ret) / 2 + 4;
    Texto texto_quadra =
        texto_criar("", x, y, "none", "white", retangulo_obter_id(quaImp->ret), true);
    texto_escrever_svg(arquivo, texto_quadra);
    texto_destruir(texto_quadra);
}

// Retorna o id de uma quadra
const char *quadra_obter_id(Quadra quadra) {
    QuadraImp *quaImp = (QuadraImp *) quadra;
    return retangulo_obter_id(quaImp->ret);
}

// Retorna a largura de uma quadra.
double quadra_obter_largura(Quadra quadra) {
    QuadraImp *quaImp = (QuadraImp *) quadra;
    return retangulo_obter_largura(quaImp->ret);
}

// Retorna a altura de uma quadra.
double quadra_obter_altura(Quadra quadra) {
    QuadraImp *quaImp = (QuadraImp *) quadra;
    return retangulo_obter_altura(quaImp->ret);
}

// Retorna a coordenada x de uma quadra.
double quadra_obter_x(Quadra quadra) {
    QuadraImp *quaImp = (QuadraImp *) quadra;
    return retangulo_obter_x(quaImp->ret);
}

// Retorna a coordenada y de uma quadra.
double quadra_obter_y(Quadra quadra) {
    QuadraImp *quaImp = (QuadraImp *) quadra;
    return retangulo_obter_y(quaImp->ret);
}

// Retorna a cor da borda de uma quadra.
const char *quadra_obter_cor_borda(Quadra quadra) {
    QuadraImp *quaImp = (QuadraImp *) quadra;
    return retangulo_obter_cor_borda(quaImp->ret);
}

// Define a cor da borda de uma quadra.
void quadra_definir_cor_borda(Quadra quadra, const char *cor_borda) {
    if (cor_borda == NULL) {
        LOG_ERROR("Não é possível definir NULL como cor da borda de uma quadra!\n");
        return;
    }
    QuadraImp *quaImp = (QuadraImp *) quadra;
    retangulo_definir_cor_borda(quaImp->ret, cor_borda);
}

// Retorna a cor de preenchimento de uma quadra.
const char *quadra_obter_cor_preenchimento(Quadra quadra) {
    QuadraImp *quaImp = (QuadraImp *) quadra;
    return retangulo_obter_cor_preenchimento(quaImp->ret);
}

// Define a cor de preenchimento de uma quadra.
void quadra_definir_cor_preenchimento(Quadra quadra, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        LOG_ERROR("Não é possível definir NULL como cor de preenchimento de uma quadra!\n");
        return;
    }
    QuadraImp *quaImp = (QuadraImp *) quadra;
    retangulo_definir_cor_preenchimento(quaImp->ret, cor_preenchimento);
}

// Define a espessura da borda de uma quadra.
void quadra_definir_espessura_borda(Quadra quadra, const char *espessura_borda) {
    if (espessura_borda == NULL) {
        LOG_ERROR("Não é possível definir NULL como tamanho da espessura da borda de um quadra!\n");
        return;
    }
    QuadraImp *quaImp = (QuadraImp *) quadra;
    retangulo_definir_espessura_borda(quaImp->ret, espessura_borda);
}

// Define o arredondamento da borda de uma quadra.
void quadra_definir_arredondamento_borda(Quadra quadra, double raio_borda) {
    QuadraImp *quaImp = (QuadraImp *) quadra;
    retangulo_definir_arredondamento_borda(quaImp->ret, raio_borda);
}

// Libera a memória alocada por uma quadra.
void quadra_destruir(Quadra quadra) {
    QuadraImp *quaImp = (QuadraImp *) quadra;
    retangulo_destruir(quaImp->ret);
    free(quadra);
}
