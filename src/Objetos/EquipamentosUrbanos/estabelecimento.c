#include "estabelecimento.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"
#include "../Formas/retangulo.h"
#include "../Outros/texto.h"
#include "quadra.h"

struct Estabelecimento_s {
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
    char tipo[100];
    char nome[100];
    char cpf[100];
};

const char *estabelecimento_obter_string_tipo() {
    return "estabelecimento";
}

// Escreve no svg as informações de um estabelecimento.
void estabelecimento_escrever_svg(Estabelecimento est, FILE *arquivo) {
    retangulo_escrever_svg((Retangulo) est, arquivo);

    double x = figura_obter_x_centro(est);
    double y = figura_obter_y_centro(est) + 6;
    // Rótulo do estabelecimento.
    Texto texto_estabelecimento = texto_criar("", x, y, "none", "white", "E", true);
    texto_escrever_svg(texto_estabelecimento, arquivo);
    texto_destruir(texto_estabelecimento);
}

// Escreve todos os dados de uma estabelecimento em um arquivo.
void estabelecimento_escrever_informacoes(Estabelecimento est, FILE *arquivo) {
    fprintf(arquivo,
            "tipo: %s, cnpj: %s, cpf: %s, tipo de estabelecimento: %s, nome: %s, largura: %lf, "
            "altura: %lf, x: %lf, y: %lf, corb: %s, corp: %s\n",
            figura_obter_tipo(est), est->id, est->cpf, est->tipo, est->nome, est->largura,
            est->altura, est->x, est->y, est->cor_borda, est->cor_preenchimento);
}

// Registra as funções do objeto Estabelecimento na interface FiguraInterface.
// Como o tipo Estabelecimento é derivado do tipo Retangulo as funções podem ser reaproveitadas.
static FiguraInterface estabelecimento_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, estabelecimento_obter_string_tipo);

    figura_registrar_escrever_informacoes(
        interface, (EscreverInformacoes *) estabelecimento_escrever_informacoes);
    figura_registrar_escrever_svg(interface, (EscreverSvg *) estabelecimento_escrever_svg);

    figura_registrar_obter_id(interface, (ObterId *) retangulo_obter_id);

    figura_registrar_obter_x(interface, (ObterX *) retangulo_obter_x);
    figura_registrar_obter_y(interface, (ObterY *) retangulo_obter_y);

    figura_registrar_obter_x_inicio(interface, (ObterXInicio *) retangulo_obter_x);
    figura_registrar_obter_y_inicio(interface, (ObterYInicio *) retangulo_obter_y);

    figura_registrar_obter_x_fim(interface, (ObterXFim *) retangulo_obter_x_fim);
    figura_registrar_obter_y_fim(interface, (ObterYFim *) retangulo_obter_y_fim);

    figura_registrar_obter_x_centro(interface, (ObterXCentro *) retangulo_obter_x_centro);
    figura_registrar_obter_y_centro(interface, (ObterYCentro *) retangulo_obter_y_centro);

    figura_registrar_obter_cor_borda(interface, (ObterCorBorda *) retangulo_obter_cor_borda);
    figura_registrar_definir_cor_borda(interface, (DefinirCorBorda *) retangulo_definir_cor_borda);

    figura_registrar_obter_cor_preenchimento(
        interface, (ObterCorPreenchimento *) retangulo_obter_cor_preenchimento);
    figura_registrar_definir_cor_preenchimento(
        interface, (DefinirCorPreenchimento *) retangulo_definir_cor_preenchimento);

    figura_registrar_destruir(interface, (Destruir *) retangulo_destruir);
    return interface;
}

// Cria e inicializa um Estabelecimento com os valores passados.
Estabelecimento estabelecimento_criar(const char *cnpj, const char *cpf, const char *tipo,
                                      const char *nome, Quadra quadra, char face, int numero) {
    if (cnpj == NULL) {
        LOG_AVISO("Não é possível criar um estabelecimento com cnpj nulo!\n");
        return NULL;
    }
    if (cpf == NULL) {
        LOG_AVISO("Não é possível criar um estabelecimento com cpf nulo!\n");
        return NULL;
    }
    if (tipo == NULL) {
        LOG_AVISO("Não é possível criar um estabelecimento com tipo nulo!\n");
        return NULL;
    }
    if (nome == NULL) {
        LOG_AVISO("Não é possível criar um estabelecimento com nome nulo!\n");
        return NULL;
    }
    if (quadra == NULL) {
        LOG_AVISO("Não é possível criar um estabelecimento com uma quadra nula!\n");
        return NULL;
    }

    Estabelecimento est = malloc(sizeof *est);
    if (est == NULL) {
        LOG_ERRO("Falha ao alocar memória\n");
        return NULL;
    }
    strcpy(est->id, cnpj);
    est->largura = 20;
    est->altura = 20;
    est->x = 0;
    est->y = 0;
    strcpy(est->cor_borda, "#3a4a3f");
    strcpy(est->cor_preenchimento, "seagreen");
    est->arredondamento_borda = 0;
    est->borda_tracejada = false;
    strcpy(est->espessura_borda, "1px");
    strcpy(est->tipo, tipo);
    strcpy(est->nome, nome);
    strcpy(est->cpf, cpf);

    quadra_inicializar_coordenada(&est->x, &est->y, est->largura, est->altura, quadra, face,
                                  numero);

    est->vtable = estabelecimento_criar_interface_figura();
    return est;
}

// Cria um estabelecimento com base em informações de uma linha.
Estabelecimento estabelecimento_ler(const char *linha, Quadra quadra) {
    char cnpj[100];
    char cpf[100];
    char tipo[100];
    char face;
    int numero;
    char nome[100];
    sscanf(linha, "e %s %s %s %*s %c %d %s", cnpj, cpf, tipo, &face, &numero, nome);
    return estabelecimento_criar(cnpj, cpf, tipo, nome, quadra, face, numero);
}

// Retorna o tipo de um estabelecimento.
const char *estabelecimento_obter_tipo(Estabelecimento est) {
    return est->tipo;
}

// Retorna o nome de um estabelecimento.
const char *estabelecimento_obter_nome(Estabelecimento est) {
    return est->nome;
}

// Retorna o cpf de um estabelecimento.
const char *estabelecimento_obter_cpf(Estabelecimento est) {
    return est->cpf;
}

// Retorna o cnpj de um estabelecimento.
const char *estabelecimento_obter_id(Estabelecimento est) {
    return retangulo_obter_id((Retangulo) est);
}

// Retorna a coordenada x de um estabelecimento.
double estabelecimento_obter_x(Estabelecimento est) {
    return retangulo_obter_x((Retangulo) est);
}

// Retorna a coordenada y de um estabelecimento.
double estabelecimento_obter_y(Estabelecimento est) {
    return retangulo_obter_y((Retangulo) est);
}

// Retorna a largura de um estabelecimento.
double estabelecimento_obter_largura(Estabelecimento est) {
    return retangulo_obter_largura((Retangulo) est);
}

// Retorna a altura de um estabelecimento.
double estabelecimento_obter_altura(Estabelecimento est) {
    return retangulo_obter_altura((Retangulo) est);
}

// Retorna a cor da borda de um estabelecimento.
const char *estabelecimento_obter_cor_borda(Estabelecimento est) {
    return retangulo_obter_cor_borda((Retangulo) est);
}

// Define a cor da borda de um estabelecimento.
void estabelecimento_definir_cor_borda(Estabelecimento est, const char *cor_borda) {
    retangulo_definir_cor_borda((Retangulo) est, cor_borda);
}

// Retorna a cor de preenchimento de um estabelecimento.
const char *estabelecimento_obter_cor_preenchimento(Estabelecimento est) {
    return retangulo_obter_cor_preenchimento((Retangulo) est);
}

// Define a cor de preenchimento de um estabelecimento.
void estabelecimento_definir_cor_preenchimento(Estabelecimento est, const char *cor_preenchimento) {
    retangulo_definir_cor_preenchimento((Retangulo) est, cor_preenchimento);
}

// Define a espessura da borda de um estabelecimento.
void estabelecimento_definir_espessura_borda(Estabelecimento est, const char *espessura_borda) {
    retangulo_definir_espessura_borda((Retangulo) est, espessura_borda);
}

// Define o arredondamento da borda de um estabelecimento.
void estabelecimento_definir_arredondamento_borda(Estabelecimento est, double raio_borda) {
    retangulo_definir_arredondamento_borda((Retangulo) est, raio_borda);
}

// Libera a memória alocada por um estabelecimento.
void estabelecimento_destruir(Estabelecimento est) {
    retangulo_destruir((Retangulo) est);
}
