#include "caso.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lista.h"
#include "logging.h"
#include "quadra.h"
#include "retangulo.h"

typedef struct {
    Retangulo ret;
    int nCasos;
} CasoImp;

// Cria e inicializa um struct CasoImp com os valores passados.
Caso caso_criar(double largura, double altura, double x, double y, const char cor_borda[20],
                const char cor_preenchimento[20], int n) {
    if (cor_borda == NULL) {
        LOG_ERROR("Não é possível criar um caso com cor de borda NULL!\n");
        return NULL;
    }
    if (cor_preenchimento == NULL) {
        LOG_ERROR("Não é possível criar um caso com cor de preenchimento NULL!\n");
        return NULL;
    }
    CasoImp *casoImp = malloc(sizeof(CasoImp));
    // TODO definir como será o id dos casos
    char id[6] = "caso";  // apenas simbólico
    casoImp->ret = retangulo_criar(id, largura, altura, x, y, cor_borda, cor_preenchimento);
    casoImp->nCasos = n;
    return casoImp;
}

// Cria uma quadra com base em informações de uma linha.
Caso caso_ler(const char *linha, Lista lista_quadras) {
    int n;
    int numero;
    char face;
    char cep[100];
    double largura = 10;
    double altura = 10;
    char cor_borda[20] = "orange";
    char cor_preenchimento[20] = "orange";
    sscanf(linha, "%*s %d %s %c %d", &n, cep, &face, &numero);

    double x;
    double y;
    if (face == 'N') {
        y = quadra_obter_casa_y_final(lista_quadras, cep);
        x = quadra_obter_casa_x(lista_quadras, cep) + numero - largura / 2;
    } else if (face == 'S') {
        y = quadra_obter_casa_y(lista_quadras, cep);
        x = quadra_obter_casa_x(lista_quadras, cep) + numero - largura / 2;
    } else if (face == 'L') {
        y = quadra_obter_casa_y(lista_quadras, cep) + numero - altura / 2;
        x = quadra_obter_casa_x(lista_quadras, cep);
    } else if (face == 'O') {
        y = quadra_obter_casa_y(lista_quadras, cep) + numero - altura / 2;
        x = quadra_obter_casa_x_final(lista_quadras, cep);
    }

    if (x == -1 || y == -1) {
        LOG_ERROR("Não foi possível encontrar a quadra especificada pelo cep: %s\n", cep);
        return NULL;
    }
    return caso_criar(largura, altura, x, y, cor_borda, cor_preenchimento, n);
}

// Escreve as informações de um caso.
void caso_escrever_informacoes(FILE *arquivo, Caso caso) {
    CasoImp *casoImp = (CasoImp *) caso;
    retangulo_escrever_informacoes(arquivo, casoImp->ret);
}

// Escreve no svg as informações de um caso.
void caso_escrever_svg(FILE *arquivo, Caso caso) {
    CasoImp *casoImp = (CasoImp *) caso;
    retangulo_escrever_svg(arquivo, casoImp->ret);
}

double caso_obter_x(Caso caso) {
    CasoImp *casoImp = (CasoImp *) caso;
    return retangulo_obter_x(casoImp->ret);
}

double caso_obter_y(Caso caso) {
    CasoImp *casoImp = (CasoImp *) caso;
    return retangulo_obter_y(casoImp->ret);
}

double caso_obter_largura(Caso caso) {
    CasoImp *casoImp = (CasoImp *) caso;
    return retangulo_obter_largura(casoImp->ret);
}

double caso_obter_altura(Caso caso) {
    CasoImp *casoImp = (CasoImp *) caso;
    return retangulo_obter_altura(casoImp->ret);
}

int caso_obter_casos(Caso casos) {
    CasoImp *casoImp = (CasoImp *) casos;
    return casoImp->nCasos;
}

// Libera a memória alocada por um caso.
void caso_destruir(Caso caso) {
    CasoImp *casoImp = (CasoImp *) caso;
    circulo_destruir(casoImp->ret);
    free(caso);
}
