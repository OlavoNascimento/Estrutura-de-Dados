#include "figura.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Interface que declara todas as funções que devem ser implementadas por um objeto para poder ser
// utilizado como uma figura.
typedef struct {
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
} FiguraInterfaceImp;

// Figura genérica.
typedef struct {
    // Tabela virtual de funções existentes em uma figura.
    // Deve ser o primeiro elemento de um struct que implemente a interface.
    FiguraInterfaceImp *vtable;
} FiguraImp;

// Cria e retorna uma interface que pode ser populada com as funções de uma figura específica.
FiguraInterface figura_interface_criar() {
    FiguraInterfaceImp *figuraInterfaceImp = calloc(1, sizeof *figuraInterfaceImp);
    return figuraInterfaceImp;
}

// Registra a função escrever_informacoes na interface.
void figura_registrar_escrever_informacoes(FiguraInterface figuraInterface,
                                           EscreverInformacoes *escrever_informacoes) {
    FiguraInterfaceImp *interface = figuraInterface;
    interface->escrever_informacoes = escrever_informacoes;
}

// Registra a função escrever_svg na interface.
void figura_registrar_escrever_svg(FiguraInterface figuraInterface, EscreverSvg *escrever_svg) {
    FiguraInterfaceImp *interface = figuraInterface;
    interface->escrever_svg = escrever_svg;
}

// Registra a função obter_tipo na interface.
void figura_registrar_obter_tipo(FiguraInterface figuraInterface, ObterTipo *obter_tipo) {
    FiguraInterfaceImp *interface = figuraInterface;
    interface->obter_tipo = obter_tipo;
}

// Registra a função obter_x na interface.
void figura_registrar_obter_x(FiguraInterface figuraInterface, ObterX *obter_x) {
    FiguraInterfaceImp *interface = figuraInterface;
    interface->obter_x = obter_x;
}

// Registra a função obter_y na interface.
void figura_registrar_obter_y(FiguraInterface figuraInterface, ObterY *obter_y) {
    FiguraInterfaceImp *interface = figuraInterface;
    interface->obter_y = obter_y;
}

// Registra a função obter_x_inicio na interface.
void figura_registrar_obter_x_inicio(FiguraInterface figuraInterface,
                                     ObterXInicio *obter_x_inicio) {
    FiguraInterfaceImp *interface = figuraInterface;
    interface->obter_x_inicio = obter_x_inicio;
}

// Registra a função obter_y_inicio na interface.
void figura_registrar_obter_y_inicio(FiguraInterface figuraInterface,
                                     ObterYInicio *obter_y_inicio) {
    FiguraInterfaceImp *interface = figuraInterface;
    interface->obter_y_inicio = obter_y_inicio;
}

// Registra a função obter_x_fim na interface.
void figura_registrar_obter_x_fim(FiguraInterface figuraInterface, ObterXFim *obter_x_fim) {
    FiguraInterfaceImp *interface = figuraInterface;
    interface->obter_x_fim = obter_x_fim;
}

// Registra a função obter_y_fim na interface.
void figura_registrar_obter_y_fim(FiguraInterface figuraInterface, ObterYFim *obter_y_fim) {
    FiguraInterfaceImp *interface = figuraInterface;
    interface->obter_y_fim = obter_y_fim;
}

// Registra a função obter_x_centro na interface.
void figura_registrar_obter_x_centro(FiguraInterface figuraInterface,
                                     ObterXCentro *obter_x_centro) {
    FiguraInterfaceImp *interface = figuraInterface;
    interface->obter_x_centro = obter_x_centro;
}

// Registra a função obter_y_centrona interface.
void figura_registrar_obter_y_centro(FiguraInterface figuraInterface,
                                     ObterYCentro *obter_y_centro) {
    FiguraInterfaceImp *interface = figuraInterface;
    interface->obter_y_centro = obter_y_centro;
}

// Registra a função obter_id na interface.
void figura_registrar_obter_id(FiguraInterface figuraInterface, ObterId *obter_id) {
    FiguraInterfaceImp *interface = figuraInterface;
    interface->obter_id = obter_id;
}

// Registra a função obter_cor_borda na interface.
void figura_registrar_obter_cor_borda(FiguraInterface figuraInterface,
                                      ObterCorBorda *obter_cor_borda) {
    FiguraInterfaceImp *interface = figuraInterface;
    interface->obter_cor_borda = obter_cor_borda;
}

// Registra a função definir_cor_borda na interface.
void figura_registrar_definir_cor_borda(FiguraInterface figuraInterface,
                                        DefinirCorBorda *definir_cor_borda) {
    FiguraInterfaceImp *interface = figuraInterface;
    interface->definir_cor_borda = definir_cor_borda;
}

// Registra a função obter_cor_preenchimento na interface.
void figura_registrar_obter_cor_preenchimento(FiguraInterface figuraInterface,
                                              ObterCorPreenchimento *obter_cor_preenchimento) {
    FiguraInterfaceImp *interface = figuraInterface;
    interface->obter_cor_preenchimento = obter_cor_preenchimento;
}

// Registra a função definir_cor_preenchimento na interface.
void figura_registrar_definir_cor_preenchimento(
    FiguraInterface figuraInterface, DefinirCorPreenchimento *definir_cor_preenchimento) {
    FiguraInterfaceImp *interface = figuraInterface;
    interface->definir_cor_preenchimento = definir_cor_preenchimento;
}

// Registra a função destruir na interface.
void figura_registrar_destruir(FiguraInterface figuraInterface, Destruir *destruir) {
    FiguraInterfaceImp *interface = figuraInterface;
    interface->destruir = destruir;
}

// Utiliza a função registrada na interface para escrever todos os dados de uma figura em um arquivo
// passado a função.
void figura_escrever_informacoes(Figura figura, FILE *arquivo) {
    FiguraImp *figuraImp = figura;
    figuraImp->vtable->escrever_informacoes(figuraImp, arquivo);
}

// Utiliza a função registrada na interface para escrever o código svg que representa uma figura em
// um arquivo.
void figura_escrever_svg(Figura figura, FILE *arquivo) {
    FiguraImp *figuraImp = figura;
    figuraImp->vtable->escrever_svg(figuraImp, arquivo);
}

// Utiliza a função registrada na interface para retornar o tipo de uma figura como uma string.
const char *figura_obter_tipo(Figura figura) {
    FiguraImp *figuraImp = figura;
    return figuraImp->vtable->obter_tipo();
}

// Utiliza a função registrada na interface para retornar a coordenada x de uma figura
double figura_obter_x(Figura figura) {
    FiguraImp *figuraImp = figura;
    return figuraImp->vtable->obter_x(figuraImp);
}

// Utiliza a função registrada na interface para retornar a coordenada y de uma figura
double figura_obter_y(Figura figura) {
    FiguraImp *figuraImp = figura;
    return figuraImp->vtable->obter_y(figuraImp);
}

// Utiliza a função registrada na interface para retornar a coordenada x onde uma figura se inicia.
double figura_obter_x_inicio(Figura figura) {
    FiguraImp *figuraImp = figura;
    return figuraImp->vtable->obter_x_inicio(figuraImp);
}

// Utiliza a função registrada na interface para retornar a coordenada y onde uma figura se inicia.
double figura_obter_y_inicio(Figura figura) {
    FiguraImp *figuraImp = figura;
    return figuraImp->vtable->obter_y_inicio(figuraImp);
}

// Utiliza a função registrada na interface para retornar a coordenada x onde uma figura acaba.
double figura_obter_x_fim(Figura figura) {
    FiguraImp *figuraImp = figura;
    return figuraImp->vtable->obter_x_fim(figuraImp);
}

// Utiliza a função registrada na interface para retornar a coordenada y onde uma figura acaba.
double figura_obter_y_fim(Figura figura) {
    FiguraImp *figuraImp = figura;
    return figuraImp->vtable->obter_y_fim(figuraImp);
}

// Utiliza a função registrada na interface para retornar a coordenada x do centro de uma figura.
double figura_obter_x_centro(Figura figura) {
    FiguraImp *figuraImp = figura;
    return figuraImp->vtable->obter_x_centro(figuraImp);
}

// Utiliza a função registrada na interface para retornar a coordenada y do centro de uma figura.
double figura_obter_y_centro(Figura figura) {
    FiguraImp *figuraImp = figura;
    return figuraImp->vtable->obter_y_centro(figuraImp);
}

// Utiliza a função registrada na interface para retornar o id de uma figura.
const char *figura_obter_id(Figura figura) {
    FiguraImp *figuraImp = figura;
    return figuraImp->vtable->obter_id(figuraImp);
}

// Utiliza a função registrada na interface para retornar a cor da borda de uma figura.
const char *figura_obter_cor_borda(Figura figura) {
    FiguraImp *figuraImp = figura;
    return figuraImp->vtable->obter_cor_borda(figuraImp);
}

// Utiliza a função registrada na interface para definir a cor da borda de uma figura.
void figura_definir_cor_borda(Figura figura, const char *cor_borda) {
    FiguraImp *figuraImp = figura;
    figuraImp->vtable->definir_cor_borda(figuraImp, cor_borda);
}

// Utiliza a função registrada na interface para retornar a cor do preenchimento de uma figura.
const char *figura_obter_cor_preenchimento(Figura figura) {
    FiguraImp *figuraImp = figura;
    return figuraImp->vtable->obter_cor_preenchimento(figuraImp);
}

// Utiliza a função registrada na interface para definir a cor de preenchimento de uma figura.
void figura_definir_cor_preenchimento(Figura figura, const char *cor_preenchimento) {
    FiguraImp *figuraImp = figura;
    figuraImp->vtable->definir_cor_preenchimento(figuraImp, cor_preenchimento);
}

// Utiliza a função registrada na interface para liberar a memória alocada por uma figura.
void figura_destruir(Figura figura) {
    FiguraImp *figuraImp = figura;
    figuraImp->vtable->destruir(figuraImp);
}
