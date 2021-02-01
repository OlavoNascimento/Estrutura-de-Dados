
#include "quadra.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"
#include "../Formas/retangulo.h"
#include "../Outros/texto.h"

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
    char cor_sombra[20];
} QuadraImp;

const char *quadra_obter_tipo() {
    return "quadra";
}

// Escreve no svg as informações de uma quadra.
void quadra_escrever_svg(Quadra quadra, FILE *arquivo) {
    QuadraImp *quadImp = quadra;

    fprintf(arquivo, "\t<rect");
    if (strlen(quadImp->id) > 0)
        fprintf(arquivo, " id='%s'", quadImp->id);

    fprintf(arquivo,
            " width='%lf' height='%lf' x='%lf' y='%lf' stroke='%s' fill='%s' rx='%lf' "
            "stroke-width='%s'",
            quadImp->largura, quadImp->altura, quadImp->x, quadImp->y, quadImp->cor_borda,
            quadImp->cor_preenchimento, quadImp->arredondamento_borda, quadImp->espessura_borda);

    if (strlen(quadImp->cor_sombra) > 0)
        fprintf(arquivo, " filter='url(%s)'", quadImp->cor_sombra);

    if (quadImp->borda_tracejada)
        fprintf(arquivo, " style='stroke-dasharray: 2'");
    fprintf(arquivo, "/>\n");

    double x = figura_obter_x_centro(quadra);
    double y = figura_obter_y_centro(quadra) + 4;
    // Rótulo da quadra.
    Texto texto_quadra = texto_criar("", x, y, "none", "white", figura_obter_id(quadra), true);
    texto_escrever_svg(texto_quadra, arquivo);
    texto_destruir(texto_quadra);
}

// Escreve todos os dados de uma quadra em um arquivo.
void quadra_escrever_informacoes(Quadra quadra, FILE *arquivo) {
    QuadraImp *quadImp = quadra;
    fprintf(arquivo, "tipo: %s,", figura_obter_tipo(quadra));
    if (strlen(quadImp->id) > 0)
        fprintf(arquivo, " cep: %s,", quadImp->id);
    fprintf(arquivo, " largura: %lf, altura: %lf, x: %lf, y: %lf, corb: %s, corp: %s\n",
            quadImp->largura, quadImp->altura, quadImp->x, quadImp->y, quadImp->cor_borda,
            quadImp->cor_preenchimento);
}

// Conecta as funções do objeto Quadra com as da interface figura.
// Como o struct QuadraImp é idêntico ao struct RetanguloImp as funções utilizadas em um objeto
// Retangulo podem ser reaproveitadas.
static FiguraInterface quadra_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, quadra_obter_tipo);

    figura_registrar_escrever_informacoes(interface, quadra_escrever_informacoes);
    figura_registrar_escrever_svg(interface, quadra_escrever_svg);

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

// Cria e inicializa um struct QuadraImp com os valores passados.
Quadra quadra_criar(const char id[100], double largura, double altura, double x, double y) {
    if (id == NULL) {
        LOG_ERRO("Não é possível criar uma quadra com id NULL!\n");
        return NULL;
    }
    QuadraImp *quaImp = malloc(sizeof *quaImp);
    strcpy(quaImp->id, id);
    quaImp->largura = largura;
    quaImp->altura = altura;
    quaImp->x = x;
    quaImp->y = y;
    strcpy(quaImp->cor_borda, "saddlebrown");
    strcpy(quaImp->cor_preenchimento, "coral");
    quaImp->arredondamento_borda = 0;
    quaImp->borda_tracejada = false;
    strcpy(quaImp->espessura_borda, "1px");
    strcpy(quaImp->cor_sombra, "");

    quaImp->vtable = quadra_criar_interface_figura();
    return quaImp;
}

// Cria uma quadra com base em informações de uma linha.
Quadra quadra_ler(const char *linha) {
    char id[100];
    double largura;
    double altura;
    double x;
    double y;
    sscanf(linha, "%*s %s %lf %lf %lf %lf", id, &x, &y, &largura, &altura);
    return quadra_criar(id, largura, altura, x, y);
}

// Encontra e inicializa as coordenadas x e y de um elemento que seja contido por uma quadra.
// Argumentos:
//      x: coordenada x do elemento.
//      y: coordenada y do elemento.
//      largura: largura do elemento, usado para centralizar-lo.
//      altura: altura do elemento, usado para centralizar-lo.
//      quadra: Quadra onde o elemento se encontra.
//      face: Lado da quadra onde o elemento se encontra.
//      numero: Distância do elemento e o início da face.
void quadra_inicializar_coordenada(double *x, double *y, double largura, double altura,
                                   Quadra quadra, char face, int numero) {
    if (face != 'N' && face != 'S' && face != 'L' && face != 'O') {
        LOG_ERRO("Face inválida passada para quadra_inicializar_coordenada");
        return;
    }

    if (face == 'N') {
        *y = figura_obter_y_fim(quadra) - altura;
        *x = figura_obter_x(quadra) - largura / 2 + numero;
    } else if (face == 'S') {
        *y = figura_obter_y(quadra);
        *x = figura_obter_x(quadra) - largura / 2 + numero;
    } else if (face == 'L') {
        *y = figura_obter_y(quadra) - altura / 2 + numero;
        *x = figura_obter_x(quadra);
    } else if (face == 'O') {
        *y = figura_obter_y(quadra) - altura / 2 + numero;
        *x = figura_obter_x_fim(quadra) - largura;
    }
}

// Retorna o id de uma quadra
const char *quadra_obter_id(Quadra quadra) {
    return retangulo_obter_id(quadra);
}

// Retorna a coordenada x de uma quadra.
double quadra_obter_x(Quadra quadra) {
    return retangulo_obter_x(quadra);
}

// Retorna a coordenada y de uma quadra.
double quadra_obter_y(Quadra quadra) {
    return retangulo_obter_y(quadra);
}

// Retorna a largura de uma quadra.
double quadra_obter_largura(Quadra quadra) {
    return retangulo_obter_largura(quadra);
}

// Retorna a altura de uma quadra.
double quadra_obter_altura(Quadra quadra) {
    return retangulo_obter_altura(quadra);
}

// Retorna a cor da borda de uma quadra.
const char *quadra_obter_cor_borda(Quadra quadra) {
    return retangulo_obter_cor_borda(quadra);
}

// Define a cor da borda de uma quadra.
void quadra_definir_cor_borda(Quadra quadra, const char *cor_borda) {
    retangulo_definir_cor_borda(quadra, cor_borda);
}

// Retorna a cor de preenchimento de uma quadra.
const char *quadra_obter_cor_preenchimento(Quadra quadra) {
    return retangulo_obter_cor_preenchimento(quadra);
}

// Define a cor de preenchimento de uma quadra.
void quadra_definir_cor_preenchimento(Quadra quadra, const char *cor_preenchimento) {
    retangulo_definir_cor_preenchimento(quadra, cor_preenchimento);
}

// Define a espessura da borda de uma quadra.
void quadra_definir_espessura_borda(Quadra quadra, const char *espessura_borda) {
    retangulo_definir_espessura_borda(quadra, espessura_borda);
}

// Define se a borda da quadra é tracejada.
void quadra_definir_borda_tracejada(Quadra quadra, bool tracejado) {
    retangulo_definir_borda_tracejada(quadra, tracejado);
}

// Define o arredondamento da borda da quadra.
void quadra_definir_arredondamento_borda(Quadra quadra, double arredondamento_borda) {
    retangulo_definir_arredondamento_borda(quadra, arredondamento_borda);
}

// Define a cor da sombra de uma quadra.
void quadra_definir_cor_sombra(Quadra quadra, const char *cor_sombra) {
    if (cor_sombra == NULL) {
        LOG_ERRO("Não é possível definir NULL como cor da sombra de uma %s!\n",
                 figura_obter_tipo(quadra));
        return;
    }
    QuadraImp *quaImp = quadra;
    strcpy(quaImp->cor_sombra, cor_sombra);
}

// Libera a memória alocada por uma quadra.
void quadra_destruir(Quadra quadra) {
    retangulo_destruir(quadra);
}
