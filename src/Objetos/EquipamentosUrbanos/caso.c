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

typedef struct {
    FiguraInterface vtable;
    char id[100];
    double largura;
    double altura;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    double arredondamento_borda;
    char espessura_borda[20];
    bool borda_tracejada;
    int numero_de_casos;
} CasoImp;

const char *caso_obter_tipo() {
    return "caso";
}

// Escreve no svg as informações de um rádio.
void caso_escrever_svg(Caso caso, FILE *arquivo) {
    CasoImp *casoImp = caso;
    retangulo_escrever_svg(caso, arquivo);

    double x = figura_obter_x_centro(caso);
    double y = figura_obter_y_centro(caso) + 4;
    // Converte o número de casos para uma string.
    char conteudo[500];
    snprintf(conteudo, 500, "%d", casoImp->numero_de_casos);
    // Rótulo do caso.
    Texto texto_quadra = texto_criar("", x, y, "none", "white", conteudo, true);
    texto_escrever_svg(texto_quadra, arquivo);
    texto_destruir(texto_quadra);
}

// Conecta as funções do objeto Caso com as da interface figura.
// Como o struct CasoImp é idêntico ao struct RetanguloImp as funções utilizadas em um objeto
// Retangulo podem ser reaproveitadas.
static FiguraInterface caso_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_escrever_informacoes(interface, retangulo_escrever_informacoes);
    figura_registrar_escrever_svg(interface, caso_escrever_svg);

    figura_registrar_obter_tipo(interface, caso_obter_tipo);

    figura_registrar_obter_id(interface, retangulo_obter_id);

    figura_registrar_obter_x(interface, retangulo_obter_x);
    figura_registrar_obter_y(interface, retangulo_obter_y);

    figura_registrar_obter_x_inicio(interface, retangulo_obter_x);
    figura_registrar_obter_y_inicio(interface, retangulo_obter_y);

    figura_registrar_obter_x_fim(interface, retangulo_obter_x_fim);
    figura_registrar_obter_y_fim(interface, retangulo_obter_y_fim);

    figura_registrar_obter_x_centro(interface, retangulo_obter_x_centro);
    figura_registrar_obter_y_centro(interface, retangulo_obter_y_centro);

    figura_registrar_obter_cor_borda(interface, retangulo_obter_cor_borda);
    figura_registrar_definir_cor_borda(interface, retangulo_definir_cor_borda);

    figura_registrar_obter_cor_preenchimento(interface, retangulo_obter_cor_preenchimento);
    figura_registrar_definir_cor_preenchimento(interface, retangulo_definir_cor_preenchimento);

    figura_registrar_destruir(interface, retangulo_destruir);
    return interface;
}

// Cria e inicializa um struct CasoImp com os valores passados.
Caso caso_criar(int casos, Quadra quadra, char face, int numero) {
    if (casos <= 0) {
        LOG_ERRO("Não é possível criar um caso menor ou igual a zero!\n");
        return NULL;
    }
    if (quadra == NULL) {
        LOG_ERRO("Não é possível criar um caso com uma quadra nula!\n");
        return NULL;
    }
    const double largura = 12;
    const double altura = 12;
    double x = 0;
    double y = 0;
    quadra_inicializar_coordenada(&x, &y, largura, altura, quadra, face, numero);

    CasoImp *casoImp = malloc(sizeof *casoImp);
    strcpy(casoImp->id, "");
    casoImp->largura = largura;
    casoImp->altura = altura;
    casoImp->x = x;
    casoImp->y = y;
    strcpy(casoImp->cor_borda, "red");
    strcpy(casoImp->cor_preenchimento, "orange");
    casoImp->arredondamento_borda = 0;
    casoImp->borda_tracejada = false;
    strcpy(casoImp->espessura_borda, "1px");
    casoImp->numero_de_casos = casos;

    casoImp->vtable = caso_criar_interface_figura();
    return casoImp;
}

// Cria um caso com base em informações de uma linha.
Caso caso_ler(const char *linha, Quadra quadra) {
    int casos;
    int numero;
    char face;
    sscanf(linha, "cv %d %*s %c %d", &casos, &face, &numero);
    return caso_criar(casos, quadra, face, numero);
}

// Retorna o número de casos armazenado em um objeto caso.
int caso_obter_numero_de_casos(Caso caso) {
    CasoImp *casoImp = caso;
    return casoImp->numero_de_casos;
}

// Retorna a coordenada x de um caso.
double caso_obter_x(Caso caso) {
    return retangulo_obter_x(caso);
}

// Retorna a coordenada y de um caso.
double caso_obter_y(Caso caso) {
    return retangulo_obter_y(caso);
}

// Retorna a largura de um caso.
double caso_obter_largura(Caso caso) {
    return retangulo_obter_largura(caso);
}

// Retorna a altura de um caso.
double caso_obter_altura(Caso caso) {
    return retangulo_obter_altura(caso);
}

// Retorna a cor da borda de um caso.
const char *caso_obter_cor_borda(Caso caso) {
    return retangulo_obter_cor_borda(caso);
}

// Define a cor da borda de um caso.
void caso_definir_cor_borda(Caso caso, const char *cor_borda) {
    retangulo_definir_cor_borda(caso, cor_borda);
}

// Retorna a cor de preenchimento de um caso.
const char *caso_obter_cor_preenchimento(Caso caso) {
    return retangulo_obter_cor_preenchimento(caso);
}

// Define a cor de preenchimento de um caso.
void caso_definir_cor_preenchimento(Caso caso, const char *cor_preenchimento) {
    retangulo_definir_cor_preenchimento(caso, cor_preenchimento);
}

// Define a espessura da borda de um caso.
void caso_definir_espessura_borda(Caso caso, const char *espessura_borda) {
    retangulo_definir_espessura_borda(caso, espessura_borda);
}

// Define o arredondamento da borda de um caso.
void caso_definir_arredondamento_borda(Caso caso, double raio_borda) {
    retangulo_definir_arredondamento_borda(caso, raio_borda);
}

// Libera a memória alocada por um caso.
void caso_destruir(Caso caso) {
    retangulo_destruir(caso);
}
