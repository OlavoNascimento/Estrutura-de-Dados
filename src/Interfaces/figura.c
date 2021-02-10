#include "figura.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Interface que declara todas as funções que devem ser implementadas por um objeto para poder ser
// utilizado como uma figura.
struct FiguraInterface_s {
    EscreverInformacoes *escrever_informacoes;
    EscreverSvg *escrever_svg;
    ObterTipo *obter_tipo;
    ObterId *obter_id;
    ObterX *obter_x;
    ObterY *obter_y;
    ObterXInicio *obter_x_inicio;
    ObterYInicio *obter_y_inicio;
    ObterXFim *obter_x_fim;
    ObterYFim *obter_y_fim;
    ObterXCentro *obter_x_centro;
    ObterYCentro *obter_y_centro;
    ObterCorBorda *obter_cor_borda;
    DefinirCorBorda *definir_cor_borda;
    ObterCorPreenchimento *obter_cor_preenchimento;
    DefinirCorPreenchimento *definir_cor_preenchimento;
    Destruir *destruir;
};

// Figura genérica.
typedef struct {
    // Tabela virtual de funções existentes em uma figura.
    // Deve ser o primeiro elemento de um struct que implemente a interface.
    FiguraInterface vtable;
} FiguraImp;

// Cria e retorna uma interface que pode ser populada com as funções de uma figura específica.
FiguraInterface figura_interface_criar() {
    FiguraInterface interface = calloc(1, sizeof *interface);
    return interface;
}

// Registra a função escrever_informacoes na interface.
void figura_registrar_escrever_informacoes(FiguraInterface interface,
                                           EscreverInformacoes *escrever_informacoes) {
    interface->escrever_informacoes = escrever_informacoes;
}

// Registra a função escrever_svg na interface.
void figura_registrar_escrever_svg(FiguraInterface interface, EscreverSvg *escrever_svg) {
    interface->escrever_svg = escrever_svg;
}

// Registra a função obter_tipo na interface.
void figura_registrar_obter_tipo(FiguraInterface interface, ObterTipo *obter_tipo) {
    interface->obter_tipo = obter_tipo;
}

// Registra a função obter_x na interface.
void figura_registrar_obter_x(FiguraInterface interface, ObterX *obter_x) {
    interface->obter_x = obter_x;
}

// Registra a função obter_y na interface.
void figura_registrar_obter_y(FiguraInterface interface, ObterY *obter_y) {
    interface->obter_y = obter_y;
}

// Registra a função obter_x_inicio na interface.
void figura_registrar_obter_x_inicio(FiguraInterface interface, ObterXInicio *obter_x_inicio) {
    interface->obter_x_inicio = obter_x_inicio;
}

// Registra a função obter_y_inicio na interface.
void figura_registrar_obter_y_inicio(FiguraInterface interface, ObterYInicio *obter_y_inicio) {
    interface->obter_y_inicio = obter_y_inicio;
}

// Registra a função obter_x_fim na interface.
void figura_registrar_obter_x_fim(FiguraInterface interface, ObterXFim *obter_x_fim) {
    interface->obter_x_fim = obter_x_fim;
}

// Registra a função obter_y_fim na interface.
void figura_registrar_obter_y_fim(FiguraInterface interface, ObterYFim *obter_y_fim) {
    interface->obter_y_fim = obter_y_fim;
}

// Registra a função obter_x_centro na interface.
void figura_registrar_obter_x_centro(FiguraInterface interface, ObterXCentro *obter_x_centro) {
    interface->obter_x_centro = obter_x_centro;
}

// Registra a função obter_y_centrona interface.
void figura_registrar_obter_y_centro(FiguraInterface interface, ObterYCentro *obter_y_centro) {
    interface->obter_y_centro = obter_y_centro;
}

// Registra a função obter_id na interface.
void figura_registrar_obter_id(FiguraInterface interface, ObterId *obter_id) {
    interface->obter_id = obter_id;
}

// Registra a função obter_cor_borda na interface.
void figura_registrar_obter_cor_borda(FiguraInterface interface, ObterCorBorda *obter_cor_borda) {
    interface->obter_cor_borda = obter_cor_borda;
}

// Registra a função definir_cor_borda na interface.
void figura_registrar_definir_cor_borda(FiguraInterface interface,
                                        DefinirCorBorda *definir_cor_borda) {
    interface->definir_cor_borda = definir_cor_borda;
}

// Registra a função obter_cor_preenchimento na interface.
void figura_registrar_obter_cor_preenchimento(FiguraInterface interface,
                                              ObterCorPreenchimento *obter_cor_preenchimento) {
    interface->obter_cor_preenchimento = obter_cor_preenchimento;
}

// Registra a função definir_cor_preenchimento na interface.
void figura_registrar_definir_cor_preenchimento(
    FiguraInterface interface, DefinirCorPreenchimento *definir_cor_preenchimento) {
    interface->definir_cor_preenchimento = definir_cor_preenchimento;
}

// Registra a função destruir na interface.
void figura_registrar_destruir(FiguraInterface interface, Destruir *destruir) {
    interface->destruir = destruir;
}

// Utiliza a função registrada na interface para escrever todos os dados de uma figura em um arquivo
// passado a função.
void figura_escrever_informacoes(Figura figura, FILE *arquivo) {
    FiguraImp *fig = figura;
    fig->vtable->escrever_informacoes(fig, arquivo);
}

// Utiliza a função registrada na interface para escrever o código svg que representa uma figura em
// um arquivo.
void figura_escrever_svg(Figura figura, FILE *arquivo) {
    FiguraImp *fig = figura;
    fig->vtable->escrever_svg(fig, arquivo);
}

// Utiliza a função registrada na interface para retornar o tipo de uma figura como uma string.
const char *figura_obter_tipo(Figura figura) {
    FiguraImp *fig = figura;
    return fig->vtable->obter_tipo();
}

// Utiliza a função registrada na interface para retornar a coordenada x de uma figura
double figura_obter_x(Figura figura) {
    FiguraImp *fig = figura;
    return fig->vtable->obter_x(fig);
}

// Utiliza a função registrada na interface para retornar a coordenada y de uma figura
double figura_obter_y(Figura figura) {
    FiguraImp *fig = figura;
    return fig->vtable->obter_y(fig);
}

// Utiliza a função registrada na interface para retornar a coordenada x onde uma figura se inicia.
double figura_obter_x_inicio(Figura figura) {
    FiguraImp *fig = figura;
    return fig->vtable->obter_x_inicio(fig);
}

// Utiliza a função registrada na interface para retornar a coordenada y onde uma figura se inicia.
double figura_obter_y_inicio(Figura figura) {
    FiguraImp *fig = figura;
    return fig->vtable->obter_y_inicio(fig);
}

// Utiliza a função registrada na interface para retornar a coordenada x onde uma figura acaba.
double figura_obter_x_fim(Figura figura) {
    FiguraImp *fig = figura;
    return fig->vtable->obter_x_fim(fig);
}

// Utiliza a função registrada na interface para retornar a coordenada y onde uma figura acaba.
double figura_obter_y_fim(Figura figura) {
    FiguraImp *fig = figura;
    return fig->vtable->obter_y_fim(fig);
}

// Utiliza a função registrada na interface para retornar a coordenada x do centro de uma figura.
double figura_obter_x_centro(Figura figura) {
    FiguraImp *fig = figura;
    return fig->vtable->obter_x_centro(fig);
}

// Utiliza a função registrada na interface para retornar a coordenada y do centro de uma figura.
double figura_obter_y_centro(Figura figura) {
    FiguraImp *fig = figura;
    return fig->vtable->obter_y_centro(fig);
}

// Utiliza a função registrada na interface para retornar o id de uma figura.
const char *figura_obter_id(Figura figura) {
    FiguraImp *fig = figura;
    return fig->vtable->obter_id(fig);
}

// Utiliza a função registrada na interface para retornar a cor da borda de uma figura.
const char *figura_obter_cor_borda(Figura figura) {
    FiguraImp *fig = figura;
    return fig->vtable->obter_cor_borda(fig);
}

// Utiliza a função registrada na interface para definir a cor da borda de uma figura.
void figura_definir_cor_borda(Figura figura, const char *cor_borda) {
    FiguraImp *fig = figura;
    fig->vtable->definir_cor_borda(fig, cor_borda);
}

// Utiliza a função registrada na interface para retornar a cor do preenchimento de uma figura.
const char *figura_obter_cor_preenchimento(Figura figura) {
    FiguraImp *fig = figura;
    return fig->vtable->obter_cor_preenchimento(fig);
}

// Utiliza a função registrada na interface para definir a cor de preenchimento de uma figura.
void figura_definir_cor_preenchimento(Figura figura, const char *cor_preenchimento) {
    FiguraImp *fig = figura;
    fig->vtable->definir_cor_preenchimento(fig, cor_preenchimento);
}

// Utiliza a função registrada na interface para liberar a memória alocada por uma figura.
void figura_destruir(Figura figura) {
    FiguraImp *fig = figura;
    fig->vtable->destruir(fig);
}
