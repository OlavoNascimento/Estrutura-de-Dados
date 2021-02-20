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

struct Endereco {
    char cep[100];
    char face;
    int num;
    char complemento[100];
};

struct Morador_s {
    FiguraInterface vtable;
    char cpf[100];
    double largura;
    double altura;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    char espessura_borda[20];
    double arredondamento_borda;
    bool borda_tracejada;

    // Informações
    char nome[100];
    char sobrenome[100];
    char sexo;
    int dia;
    int mes;
    int ano;
    char data[20];
    struct Endereco endereco;
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
    fprintf(arquivo, "tipo: %s, cpf: %s, nome: %s, sobrenome: %s, sexo: %c, data: %s",
            figura_obter_tipo(morador), morador->cpf, morador->nome, morador->sobrenome,
            morador->sexo, morador->data);
    fprintf(arquivo, ", cep: %s, face: %c, num: %d, complemento: %s", morador->endereco.cep,
            morador->endereco.face, morador->endereco.num, morador->endereco.complemento);
    fprintf(arquivo, ", x: %lf y: %lf, corb: %s, corp: %s\n", morador->x, morador->y,
            morador->cor_borda, morador->cor_preenchimento);
}

void morador_definir_data(Morador morador) {
    snprintf(morador->data, sizeof(morador->data), "%02d/%02d/%d", morador->dia, morador->mes % 100,
             morador->ano);
}

// Registra as funções do objeto Morador na interface FiguraInterface.
// Como o tipo Morador é derivado do tipo Retangulo as funções podem ser reaproveitadas.
static FiguraInterface morador_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, morador_obter_string_tipo);

    figura_registrar_escrever_informacoes(interface,
                                          (EscreverInformacoes *) morador_escrever_informacoes);
    figura_registrar_escrever_svg(interface, (EscreverSvg *) morador_escrever_svg);

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

    figura_registrar_destruir(interface, (Destruir *) morador_destruir);
    return interface;
}

// Cria e inicializa um Morador com os valores passados.
Morador morador_criar(const char *cpf, const char *nome, const char *sobrenome, const char sexo,
                      int dia, int mes, int ano) {
    if (cpf == NULL) {
        LOG_AVISO("Não é possível criar um morador com cpf NULL!\n");
        return NULL;
    }
    if (nome == NULL) {
        LOG_AVISO("Não é possível criar um morador com nome NULL!\n");
        return NULL;
    }
    if (sobrenome == NULL) {
        LOG_AVISO("Não é possível criar um morador com sobrenome NULL!\n");
        return NULL;
    }
    if (dia > 31 || mes > 12) {
        LOG_AVISO("Não é possível criar um morador com data inválida!\n");
        return NULL;
    }
    Morador morador = malloc(sizeof *morador);
    if (morador == NULL) {
        LOG_ERRO("Falha ao alocar memória\n");
        return NULL;
    }
    strcpy(morador->cpf, cpf);
    strcpy(morador->nome, nome);
    strcpy(morador->sobrenome, sobrenome);
    morador->sexo = sexo;
    morador->dia = dia;
    morador->mes = mes;
    morador->ano = ano;
    morador_definir_data(morador);

    struct Endereco endereco = {.cep = "não registrado", .complemento = "-", .num = 0, .face = '-'};
    morador->endereco = endereco;

    morador->largura = 20;
    morador->altura = 20;
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
    strcpy(morador->endereco.cep, cep);
    strcpy(morador->endereco.complemento, complemento);
    morador->endereco.face = face;
    morador->endereco.num = num;
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
    return morador->endereco.cep;
}

const char *morador_obter_endereco_complemento(Morador morador) {
    return morador->endereco.complemento;
}

char morador_obter_endereco_face(Morador morador) {
    return morador->endereco.face;
}

int morador_obter_endereco_num(Morador morador) {
    return morador->endereco.num;
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

// Define se a borda de um morador é tracejada.
void morador_definir_borda_tracejada(Morador morador, bool tracejado) {
    retangulo_definir_borda_tracejada((Retangulo) morador, tracejado);
}

// Libera a memória utilizada por um morador.
void morador_destruir(Morador morador) {
    retangulo_destruir((Retangulo) morador);
}
