#include "morador.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"
#include "../EquipamentosUrbanos/quadra.h"
#include "../Formas/retangulo.h"
#include "../Outros/texto.h"
#include "morador.h"

typedef struct {
    FiguraInterface vtable;
    char cpf[100];
    double largura;
    double altura;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    double arredondamento_borda;
    char espessura_borda[20];
    bool borda_tracejada;

    char nome[100];
    char sobrenome[100];
    char sexo;
    int dia;
    int mes;
    int ano;
    char data[20];

    // endereço
    char cep[100];
    char face;
    int num;
    char complemento[100];
} MoradorImp;

typedef struct {
    double largura;
    double altura;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    double arredondamento_borda;
    char espessura_borda[20];
    bool borda_tracejada;
} Coordenadas;

const char *morador_obter_string_tipo() {
    return "morador";
}

// Escreve no svg as informações de um morador.
void morador_escrever_svg(Morador morador, FILE *arquivo) {
    retangulo_escrever_svg(morador, arquivo);

    double x = figura_obter_x_centro(morador);
    double y = figura_obter_y_centro(morador) + 6;
    // Rótulo do morador.
    Texto texto_morador = texto_criar("", x, y, "none", "white", "M", true);
    texto_escrever_svg(texto_morador, arquivo);
    texto_destruir(texto_morador);
}

// Escreve todos os dados de uma morador em um arquivo.
void morador_escrever_informacoes(Morador morador, FILE *arquivo) {
    MoradorImp *moradorImp = morador;
    fprintf(arquivo,
            "tipo: %s, cpf: %s, nome: %s, sobrenome: %s, sexo: %c, data: %s, x: %lf y: %lf, corb: "
            "%s, corp: %s\n",
            figura_obter_tipo(moradorImp), moradorImp->cpf, moradorImp->nome, moradorImp->sobrenome,
            moradorImp->sexo, morador_obter_data(moradorImp), moradorImp->x, moradorImp->y,
            moradorImp->cor_borda, moradorImp->cor_preenchimento);
}

void morador_definir_data(Morador morador) {
    MoradorImp *moradorImp = morador;
    char buffer[20];
    if (moradorImp->dia < 10) {
        if (moradorImp->mes < 10) {
            snprintf(buffer, sizeof(buffer), "0%d/0%d/%d", moradorImp->dia, moradorImp->mes,
                     moradorImp->ano);
        } else {
            snprintf(buffer, sizeof(buffer), "0%d/%d/%d", moradorImp->dia, moradorImp->mes,
                     moradorImp->ano);
        }
    } else if (moradorImp->mes < 10) {
        snprintf(buffer, sizeof(buffer), "%d/0%d/%d", moradorImp->dia, moradorImp->mes,
                 moradorImp->ano);
    } else {
        snprintf(buffer, sizeof(buffer), "%d/%d/%d", moradorImp->dia, moradorImp->mes,
                 moradorImp->ano);
    }

    strcpy(moradorImp->data, buffer);
}

static FiguraInterface morador_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_escrever_informacoes(interface, morador_escrever_informacoes);
    figura_registrar_escrever_svg(interface, morador_escrever_svg);

    figura_registrar_obter_id(interface, retangulo_obter_id);

    figura_registrar_obter_tipo(interface, morador_obter_string_tipo);

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

    figura_registrar_destruir(interface, morador_destruir);
    return interface;
}

// Cria e inicializa um struct MoradorImp com os valores passados.
Morador morador_criar(const char *cpf, const char *nome, const char *sobrenome, const char sexo,
                      int dia, int mes, int ano) {
    if (cpf == NULL) {
        LOG_ERRO("Não é possível criar um morador com cpf NULL!\n");
        return NULL;
    }
    if (nome == NULL) {
        LOG_ERRO("Não é possível criar um morador com nome NULL!\n");
        return NULL;
    }
    if (sobrenome == NULL) {
        LOG_ERRO("Não é possível criar um morador com sobrenome NULL!\n");
        return NULL;
    }
    MoradorImp *moradorImp = malloc(sizeof *moradorImp);
    strcpy(moradorImp->cpf, cpf);
    strcpy(moradorImp->nome, nome);
    strcpy(moradorImp->sobrenome, sobrenome);
    moradorImp->sexo = sexo;
    moradorImp->dia = dia;
    moradorImp->mes = mes;
    moradorImp->ano = ano;

    morador_definir_data(moradorImp);

    strcpy(moradorImp->cep, "não registrado");
    strcpy(moradorImp->complemento, "-");
    moradorImp->num = 0;
    moradorImp->face = '-';

    moradorImp->largura = 0;
    moradorImp->altura = 0;
    moradorImp->x = 0;
    moradorImp->y = 0;
    strcpy(moradorImp->cor_borda, "#3a464a");
    strcpy(moradorImp->cor_preenchimento, "#2d3b40");
    moradorImp->arredondamento_borda = 0;
    moradorImp->borda_tracejada = false;
    strcpy(moradorImp->espessura_borda, "1px");

    moradorImp->vtable = morador_criar_interface_figura();
    return moradorImp;
}

// Cria um morador com base em informações de uma linha.
Morador morador_ler(const char *linha) {
    char cpf[100];
    char nome[100];
    char sobrenome[100];
    char sexo;
    int dia;
    int mes;
    int ano;
    sscanf(linha, "%*s %s %s %s %c %d %*c %d %*c %d", cpf, nome, sobrenome, &sexo, &dia, &mes,
           &ano);
    return morador_criar(cpf, nome, sobrenome, sexo, dia, mes, ano);
}

// Obtém o endereço de um morador com base em informações de uma linha.
void morador_ler_endereco(Morador morador, const char *linha, Quadra quadra) {
    char cep[100];
    char complemento[100];
    char face;
    int num;

    sscanf(linha, "m %*s %s %c %d %s", cep, &face, &num, complemento);
    morador_definir_endereco(morador, cep, face, num, complemento, quadra);
}

void morador_definir_endereco(Morador morador, const char *cep, const char face, int num,
                              const char *complemento, Quadra quadra) {
    MoradorImp *moradorImp = morador;
    strcpy(moradorImp->cep, cep);
    strcpy(moradorImp->complemento, complemento);
    moradorImp->face = face;
    moradorImp->num = num;

    const double largura = 12;
    const double altura = 12;
    double x = 0;
    double y = 0;
    quadra_inicializar_coordenada(&x, &y, largura, altura, quadra, face, num);

    moradorImp->largura = largura;
    moradorImp->altura = altura;
    moradorImp->x = x;
    moradorImp->y = y;
}

const char *morador_obter_id(Morador morador) {
    return retangulo_obter_id(morador);
}

const char *morador_obter_nome(Morador morador) {
    MoradorImp *moradorImp = morador;
    return moradorImp->nome;
}

const char *morador_obter_sobrenome(Morador morador) {
    MoradorImp *moradorImp = morador;
    return moradorImp->sobrenome;
}

char morador_obter_sexo(Morador morador) {
    MoradorImp *moradorImp = morador;
    return moradorImp->sexo;
}

const char *morador_obter_data(Morador morador) {
    MoradorImp *moradorImp = morador;
    return moradorImp->data;
}

const char *morador_obter_endereco_cep(Morador morador) {
    MoradorImp *moradorImp = morador;
    return moradorImp->cep;
}

char morador_obter_endereco_face(Morador morador) {
    MoradorImp *moradorImp = morador;
    return moradorImp->face;
}

int morador_obter_endereco_num(Morador morador) {
    MoradorImp *moradorImp = morador;
    return moradorImp->num;
}

// Retorna a coordenada x de um morador.
double morador_obter_x(Morador morador) {
    return retangulo_obter_x(morador);
}

// Retorna a coordenada y de um morador.
double morador_obter_y(Morador morador) {
    return retangulo_obter_y(morador);
}

// Retorna a largura de um morador.
double morador_obter_largura(Morador morador) {
    return retangulo_obter_largura(morador);
}

// Retorna a altura de um morador.
double morador_obter_altura(Morador morador) {
    return retangulo_obter_altura(morador);
}

// Retorna a cor da borda de um morador.
const char *morador_obter_cor_borda(Morador morador) {
    return retangulo_obter_cor_borda(morador);
}

// Define a cor da borda de um morador.
void morador_definir_cor_borda(Morador morador, const char *cor_borda) {
    retangulo_definir_cor_borda(morador, cor_borda);
}

// Retorna a cor de preenchimento de um morador.
const char *morador_obter_cor_preenchimento(Morador morador) {
    return retangulo_obter_cor_preenchimento(morador);
}

// Define a cor de preenchimento de um morador.
void morador_definir_cor_preenchimento(Morador morador, const char *cor_preenchimento) {
    retangulo_definir_cor_preenchimento(morador, cor_preenchimento);
}

// Define a espessura da borda de um morador.
void morador_definir_espessura_borda(Morador morador, const char *espessura_borda) {
    retangulo_definir_espessura_borda(morador, espessura_borda);
}

// Define o arredondamento da borda de um morador.
void morador_definir_arredondamento_borda(Morador morador, double raio_borda) {
    retangulo_definir_arredondamento_borda(morador, raio_borda);
}

// Libera a memória utilizada por um morador.
void morador_destruir(Morador morador) {
    MoradorImp *moradorImp = morador;
    free(moradorImp);
}
