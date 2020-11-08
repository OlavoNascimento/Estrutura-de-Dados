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
Caso caso_criar(int casos, char cep[100], char face, int numero, const char cor_borda[20],
                const char cor_preenchimento[20], Lista lista_quadras) {
    if (casos <= 0) {
        LOG_ERROR("Não é possível criar um caso menor ou igual a zero!\n");
        return NULL;
    }
    if (cep == NULL) {
        LOG_ERROR("Não é possível criar um caso buscando por um cep nulo!\n");
        return NULL;
    }
    if (cor_borda == NULL) {
        LOG_ERROR("Não é possível criar um caso com cor de borda NULL!\n");
        return NULL;
    }
    if (cor_preenchimento == NULL) {
        LOG_ERROR("Não é possível criar um caso com cor de preenchimento NULL!\n");
        return NULL;
    }
    Figura quadra_buscada = NULL;
    for (No i = lista_get_first(lista_quadras); i != NULL; i = lista_get_next(i)) {
        Figura quadra = lista_get_figura(i);
        if (strcmp(cep, figura_obter_id(quadra)) == 0)
            quadra_buscada = quadra;
    }
    if (quadra_buscada == NULL)
        return NULL;

    double largura = 12;
    double altura = 12;
    double x = 0;
    double y = 0;
    if (face == 'N') {
        y = figura_obter_y_fim(quadra_buscada) - altura;
        x = figura_obter_x(quadra_buscada) + numero;
    } else if (face == 'S') {
        y = figura_obter_y(quadra_buscada);
        x = figura_obter_x(quadra_buscada) + numero;
    } else if (face == 'L') {
        y = figura_obter_y(quadra_buscada) + numero;
        x = figura_obter_x(quadra_buscada);
    } else if (face == 'O') {
        y = figura_obter_y(quadra_buscada) + numero;
        x = figura_obter_x_fim(quadra_buscada) - largura;
    }

    CasoImp *casoImp = malloc(sizeof(CasoImp));
    casoImp->ret = retangulo_criar("", largura, altura, x, y, cor_borda, cor_preenchimento);
    casoImp->nCasos = casos;
    return casoImp;
}

// Cria uma quadra com base em informações de uma linha.
Caso caso_ler(const char *linha, Lista lista_quadras) {
    int casos;
    int numero;
    char face;
    char cep[100];
    sscanf(linha, "cv %d %s %c %d", &casos, cep, &face, &numero);
    return caso_criar(casos, cep, face, numero, "orange", "orange", lista_quadras);
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
    retangulo_destruir(casoImp->ret);
    free(caso);
}
