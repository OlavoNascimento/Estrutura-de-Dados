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

struct Morador_s {
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

    // Informações
    char nome[100];
    char sobrenome[100];
    char sexo;
    int dia;
    int mes;
    int ano;
    char data[20];

    // Endereço
    char cep[100];
    char face;
    int num;
    char complemento[100];
};

const char *morador_obter_string_tipo() {
    return "morador";
}

// Escreve no svg as informações de um morador.
void morador_escrever_svg(Morador morador, FILE *arquivo) {
    retangulo_escrever_svg((Retangulo) morador, arquivo);

    double x = figura_obter_x_centro(morador);
    double y = figura_obter_y_centro(morador) + 6;
    // Rótulo do morador.
    Texto texto_morador = texto_criar("", x, y, "none", "white", "M", true);
    texto_escrever_svg(texto_morador, arquivo);
    texto_destruir(texto_morador);
}

// Escreve todos os dados de uma morador em um arquivo.
void morador_escrever_informacoes(Morador morador, FILE *arquivo) {
    fprintf(arquivo,
            "tipo: %s, cpf: %s, nome: %s, sobrenome: %s, sexo: %c, data: %s, x: %lf y: %lf, corb: "
            "%s, corp: %s\n",
            figura_obter_tipo(morador), morador->cpf, morador->nome, morador->sobrenome,
            morador->sexo, morador_obter_data(morador), morador->x, morador->y, morador->cor_borda,
            morador->cor_preenchimento);
}

void morador_definir_data(Morador morador) {
    char buffer[20];
    if (morador->dia < 10) {
        if (morador->mes < 10) {
            snprintf(buffer, sizeof(buffer), "0%d/0%d/%d", morador->dia, morador->mes,
                     morador->ano);
        } else {
            snprintf(buffer, sizeof(buffer), "0%d/%d/%d", morador->dia, morador->mes, morador->ano);
        }
    } else if (morador->mes < 10) {
        snprintf(buffer, sizeof(buffer), "%d/0%d/%d", morador->dia, morador->mes, morador->ano);
    } else {
        snprintf(buffer, sizeof(buffer), "%d/%d/%d", morador->dia, morador->mes, morador->ano);
    }

    strcpy(morador->data, buffer);
}

static FiguraInterface morador_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, (void *) morador_obter_string_tipo);

    figura_registrar_escrever_informacoes(interface, (void *) morador_escrever_informacoes);
    figura_registrar_escrever_svg(interface, (void *) morador_escrever_svg);

    figura_registrar_obter_id(interface, (void *) retangulo_obter_id);

    figura_registrar_obter_x(interface, (void *) retangulo_obter_x);
    figura_registrar_obter_y(interface, (void *) retangulo_obter_y);

    figura_registrar_obter_x_inicio(interface, (void *) retangulo_obter_x);
    figura_registrar_obter_y_inicio(interface, (void *) retangulo_obter_y);

    figura_registrar_obter_x_fim(interface, (void *) retangulo_obter_x_fim);
    figura_registrar_obter_y_fim(interface, (void *) retangulo_obter_y_fim);

    figura_registrar_obter_x_centro(interface, (void *) retangulo_obter_x_centro);
    figura_registrar_obter_y_centro(interface, (void *) retangulo_obter_y_centro);

    figura_registrar_obter_cor_borda(interface, (void *) retangulo_obter_cor_borda);
    figura_registrar_definir_cor_borda(interface, (void *) retangulo_definir_cor_borda);

    figura_registrar_obter_cor_preenchimento(interface, (void *) retangulo_obter_cor_preenchimento);
    figura_registrar_definir_cor_preenchimento(interface,
                                               (void *) retangulo_definir_cor_preenchimento);

    figura_registrar_destruir(interface, (void *) morador_destruir);
    return interface;
}

// Cria e inicializa um Morador com os valores passados.
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
    Morador morador = malloc(sizeof *morador);
    strcpy(morador->cpf, cpf);
    strcpy(morador->nome, nome);
    strcpy(morador->sobrenome, sobrenome);
    morador->sexo = sexo;
    morador->dia = dia;
    morador->mes = mes;
    morador->ano = ano;

    morador_definir_data(morador);

    strcpy(morador->cep, "não registrado");
    strcpy(morador->complemento, "-");
    morador->num = 0;
    morador->face = '-';

    morador->largura = 0;
    morador->altura = 0;
    morador->x = 0;
    morador->y = 0;
    strcpy(morador->cor_borda, "#3a464a");
    strcpy(morador->cor_preenchimento, "#2d3b40");
    morador->arredondamento_borda = 0;
    morador->borda_tracejada = false;
    strcpy(morador->espessura_borda, "1px");

    morador->vtable = morador_criar_interface_figura();
    return morador;
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
    strcpy(morador->cep, cep);
    strcpy(morador->complemento, complemento);
    morador->face = face;
    morador->num = num;

    morador->largura = 12;
    morador->altura = 12;
    morador->x = 0;
    morador->y = 0;
    quadra_inicializar_coordenada(&morador->x, &morador->y, morador->largura, morador->altura,
                                  quadra, face, num);
}

const char *morador_obter_nome(Morador morador) {
    return morador->nome;
}

const char *morador_obter_sobrenome(Morador morador) {
    return morador->sobrenome;
}

char morador_obter_sexo(Morador morador) {
    return morador->sexo;
}

const char *morador_obter_data(Morador morador) {
    return morador->data;
}

const char *morador_obter_endereco_cep(Morador morador) {
    return morador->cep;
}

const char *morador_obter_endereco_complemento(Morador morador) {
    MoradorImp *moradorImp = morador;
    return moradorImp->complemento;
}

char morador_obter_endereco_face(Morador morador) {
    return morador->face;
}

int morador_obter_endereco_num(Morador morador) {
    return morador->num;
}

const char *morador_obter_id(Morador morador) {
    return retangulo_obter_id((Retangulo) morador);
}

// Retorna a coordenada x de um morador.
double morador_obter_x(Morador morador) {
    return retangulo_obter_x((Retangulo) morador);
}

// Retorna a coordenada y de um morador.
double morador_obter_y(Morador morador) {
    return retangulo_obter_y((Retangulo) morador);
}

// Retorna a largura de um morador.
double morador_obter_largura(Morador morador) {
    return retangulo_obter_largura((Retangulo) morador);
}

// Retorna a altura de um morador.
double morador_obter_altura(Morador morador) {
    return retangulo_obter_altura((Retangulo) morador);
}

// Retorna a cor da borda de um morador.
const char *morador_obter_cor_borda(Morador morador) {
    return retangulo_obter_cor_borda((Retangulo) morador);
}

// Define a cor da borda de um morador.
void morador_definir_cor_borda(Morador morador, const char *cor_borda) {
    retangulo_definir_cor_borda((Retangulo) morador, cor_borda);
}

// Retorna a cor de preenchimento de um morador.
const char *morador_obter_cor_preenchimento(Morador morador) {
    return retangulo_obter_cor_preenchimento((Retangulo) morador);
}

// Define a cor de preenchimento de um morador.
void morador_definir_cor_preenchimento(Morador morador, const char *cor_preenchimento) {
    retangulo_definir_cor_preenchimento((Retangulo) morador, cor_preenchimento);
}

// Define a espessura da borda de um morador.
void morador_definir_espessura_borda(Morador morador, const char *espessura_borda) {
    retangulo_definir_espessura_borda((Retangulo) morador, espessura_borda);
}

// Define o arredondamento da borda de um morador.
void morador_definir_arredondamento_borda(Morador morador, double raio_borda) {
    retangulo_definir_arredondamento_borda((Retangulo) morador, raio_borda);
}

// Libera a memória utilizada por um morador.
void morador_destruir(Morador morador) {
    retangulo_destruir((Retangulo) morador);
}
