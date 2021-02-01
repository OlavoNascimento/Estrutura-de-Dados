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
    char tipo[100];
    char nome[100];
    char cpf[100];
} EstabelecimentoImp;

const char *estabelecimento_obter_string_tipo() {
    return "estabelecimento";
}

// Escreve no svg as informações de um estabelecimento.
void estabelecimento_escrever_svg(Estabelecimento estabelecimento, FILE *arquivo) {
    retangulo_escrever_svg(estabelecimento, arquivo);

    double x = figura_obter_x_centro(estabelecimento);
    double y = figura_obter_y_centro(estabelecimento) + 6;
    // Rótulo do estabelecimento.
    Texto texto_estabelecimento = texto_criar("", x, y, "none", "white", "E", true);
    texto_escrever_svg(texto_estabelecimento, arquivo);
    texto_destruir(texto_estabelecimento);
}

// Escreve todos os dados de uma estabelecimento em um arquivo.
void estabelecimento_escrever_informacoes(Estabelecimento estabelecimento, FILE *arquivo) {
    EstabelecimentoImp *estImp = estabelecimento;
    fprintf(arquivo,
            "tipo: %s, cnpj: %s, largura: %lf, altura: %lf, x: %lf, y: %lf, corb: %s, corp: %s, "
            "tipo de estabelecimento: %s, nome: %s, cpf: %s\n",
            figura_obter_tipo(estabelecimento), estImp->id, estImp->largura, estImp->altura,
            estImp->x, estImp->y, estImp->cor_borda, estImp->cor_preenchimento, estImp->tipo,
            estImp->nome, estImp->cpf);
}

// Conecta as funções do objeto Estabelecimento com as da interface figura.
// Como o struct EstabelecimentoImp é idêntico ao struct RetanguloImp as funções utilizadas em um
// objeto Retangulo podem ser reaproveitadas.
static FiguraInterface estabelecimento_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, estabelecimento_obter_string_tipo);

    figura_registrar_escrever_informacoes(interface, estabelecimento_escrever_informacoes);
    figura_registrar_escrever_svg(interface, estabelecimento_escrever_svg);

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

// Cria e inicializa um struct EstabelecimentoImp com os valores passados.
Estabelecimento estabelecimento_criar(const char *cnpj, const char *cpf, const char *tipo,
                                      const char *nome, Quadra quadra, char face, int numero) {
    if (cnpj == NULL) {
        LOG_ERRO("Não é possível criar um estabelecimento com cnpj NULL!\n");
        return NULL;
    }
    if (cpf == NULL) {
        LOG_ERRO("Não é possível criar um estabelecimento com cpf NULL!\n");
        return NULL;
    }
    if (tipo == NULL) {
        LOG_ERRO("Não é possível criar um estabelecimento com tipo NULL!\n");
        return NULL;
    }
    if (nome == NULL) {
        LOG_ERRO("Não é possível criar um estabelecimento com tipo NULL!\n");
        return NULL;
    }
    if (quadra == NULL) {
        LOG_ERRO("Não é possível criar um estabelecimento com uma quadra nula!\n");
        return NULL;
    }

    EstabelecimentoImp *estImp = malloc(sizeof *estImp);
    strcpy(estImp->id, cnpj);
    estImp->largura = 12;
    estImp->altura = 12;
    estImp->x = 0;
    estImp->y = 0;
    strcpy(estImp->cor_borda, "#3a4a3f");
    strcpy(estImp->cor_preenchimento, "seagreen");
    estImp->arredondamento_borda = 0;
    estImp->borda_tracejada = false;
    strcpy(estImp->espessura_borda, "1px");
    strcpy(estImp->tipo, tipo);
    strcpy(estImp->nome, nome);
    strcpy(estImp->cpf, cpf);

    quadra_inicializar_coordenada(&estImp->x, &estImp->y, estImp->largura, estImp->altura, quadra,
                                  face, numero);

    estImp->vtable = estabelecimento_criar_interface_figura();
    return estImp;
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

// Retorna o cnpj de um estabelecimento.
const char *estabelecimento_obter_id(Estabelecimento estabelecimento) {
    return retangulo_obter_id(estabelecimento);
}

// Retorna o tipo de um estabelecimento.
const char *estabelecimento_obter_tipo(Estabelecimento estabelecimento) {
    EstabelecimentoImp *est = estabelecimento;
    return est->tipo;
}

// Retorna o nome de um estabelecimento.
const char *estabelecimento_obter_nome(Estabelecimento estabelecimento) {
    EstabelecimentoImp *est = estabelecimento;
    return est->nome;
}

// Retorna o cpf de um estabelecimento.
const char *estabelecimento_obter_cpf(Estabelecimento estabelecimento) {
    EstabelecimentoImp *est = estabelecimento;
    return est->cpf;
}

// Retorna a coordenada x de um estabelecimento.
double estabelecimento_obter_x(Estabelecimento estabelecimento) {
    return retangulo_obter_x(estabelecimento);
}

// Retorna a coordenada y de um estabelecimento.
double estabelecimento_obter_y(Estabelecimento estabelecimento) {
    return retangulo_obter_y(estabelecimento);
}

// Retorna a largura de um estabelecimento.
double estabelecimento_obter_largura(Estabelecimento estabelecimento) {
    return retangulo_obter_largura(estabelecimento);
}

// Retorna a altura de um estabelecimento.
double estabelecimento_obter_altura(Estabelecimento estabelecimento) {
    return retangulo_obter_altura(estabelecimento);
}

// Retorna a cor da borda de um estabelecimento.
const char *estabelecimento_obter_cor_borda(Estabelecimento estabelecimento) {
    return retangulo_obter_cor_borda(estabelecimento);
}

// Define a cor da borda de um estabelecimento.
void estabelecimento_definir_cor_borda(Estabelecimento estabelecimento, const char *cor_borda) {
    retangulo_definir_cor_borda(estabelecimento, cor_borda);
}

// Retorna a cor de preenchimento de um estabelecimento.
const char *estabelecimento_obter_cor_preenchimento(Estabelecimento estabelecimento) {
    return retangulo_obter_cor_preenchimento(estabelecimento);
}

// Define a cor de preenchimento de um estabelecimento.
void estabelecimento_definir_cor_preenchimento(Estabelecimento estabelecimento,
                                               const char *cor_preenchimento) {
    retangulo_definir_cor_preenchimento(estabelecimento, cor_preenchimento);
}

// Define a espessura da borda de um estabelecimento.
void estabelecimento_definir_espessura_borda(Estabelecimento estabelecimento,
                                             const char *espessura_borda) {
    retangulo_definir_espessura_borda(estabelecimento, espessura_borda);
}

// Define o arredondamento da borda de um estabelecimento.
void estabelecimento_definir_arredondamento_borda(Estabelecimento estabelecimento,
                                                  double raio_borda) {
    retangulo_definir_arredondamento_borda(estabelecimento, raio_borda);
}

// Libera a memória alocada por um estabelecimento.
void estabelecimento_destruir(Estabelecimento estabelecimento) {
    retangulo_destruir(estabelecimento);
}
