#include "semaforo.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"
#include "../Formas/retangulo.h"
#include "../Outros/texto.h"

struct Semaforo_s {
    FiguraInterface vtable;
    char id[100];
    double largura;
    double altura;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    double espessura_borda;
    double arredondamento_borda;
    bool borda_tracejada;
};

const char *semaforo_obter_tipo() {
    return "semáforo";
}

// Escreve no svg as informações de um semáforo.
void semaforo_escrever_svg(Semaforo semaforo, FILE *arquivo) {
    retangulo_escrever_svg((Retangulo) semaforo, arquivo);

    double x = figura_obter_x_centro(semaforo);
    double y = figura_obter_y_centro(semaforo) + 4;
    // Rótulo do semáforo.
    Texto texto_semaforo = texto_criar("", x, y, "none", "white", "S");
    texto_definir_alinhamento(texto_semaforo, TEXTO_CENTRO);
    texto_escrever_svg(texto_semaforo, arquivo);
    texto_destruir(texto_semaforo);
}

// Registra as funções do objeto Semaforo na interface FiguraInterface.
// Como o tipo Semaforo é derivado do tipo Retangulo as funções podem ser reaproveitadas.
static FiguraInterface semaforo_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, semaforo_obter_tipo);

    figura_registrar_escrever_informacoes(interface,
                                          (EscreverInformacoes *) retangulo_escrever_informacoes);
    figura_registrar_escrever_svg(interface, (EscreverSvg *) semaforo_escrever_svg);

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

// Cria e inicializa um Semaforo com os valores passados.
Semaforo semaforo_criar(const char id[100], double x, double y) {
    if (id == NULL) {
        LOG_AVISO("Não é possível criar um semáforo com id NULL!\n");
        return NULL;
    }
    Semaforo semaforo = malloc(sizeof *semaforo);
    if (semaforo == NULL) {
        LOG_ERRO("Falha ao alocar memória\n");
        return NULL;
    }
    strcpy(semaforo->id, id);
    semaforo->largura = 12;
    semaforo->altura = 30;
    semaforo->x = x - semaforo->largura / 2;
    semaforo->y = y - semaforo->altura / 2;
    strcpy(semaforo->cor_borda, "red");
    strcpy(semaforo->cor_preenchimento, "green");
    semaforo->arredondamento_borda = 0;
    semaforo->borda_tracejada = false;
    semaforo->espessura_borda = 1;

    semaforo->vtable = semaforo_criar_interface_figura();
    return semaforo;
}

// Cria um semaforo com base em informações de uma linha.
Semaforo semaforo_ler(const char *linha) {
    char id[100];
    double x;
    double y;
    sscanf(linha, "%*s %s %lf %lf", id, &x, &y);
    return semaforo_criar(id, x, y);
}

// Retorna o id de um semáforo.
const char *semaforo_obter_id(Semaforo semaforo) {
    return retangulo_obter_id((Retangulo) semaforo);
}

// Retorna a coordenada x de um semáforo.
double semaforo_obter_x(Semaforo semaforo) {
    return retangulo_obter_x((Retangulo) semaforo);
}

// Retorna a coordenada y de um semáforo.
double semaforo_obter_y(Semaforo semaforo) {
    return retangulo_obter_y((Retangulo) semaforo);
}

// Retorna a largura de um semáforo.
double semaforo_obter_largura(Semaforo semaforo) {
    return retangulo_obter_largura((Retangulo) semaforo);
}

// Retorna a altura de um semáforo.
double semaforo_obter_altura(Semaforo semaforo) {
    return retangulo_obter_altura((Retangulo) semaforo);
}

// Retorna a cor da borda de um semáforo.
const char *semaforo_obter_cor_borda(Semaforo semaforo) {
    return retangulo_obter_cor_borda((Retangulo) semaforo);
}

// Define a cor da borda de um semáforo.
void semaforo_definir_cor_borda(Semaforo semaforo, const char *cor_borda) {
    retangulo_definir_cor_borda((Retangulo) semaforo, cor_borda);
}

// Retorna a cor de preenchimento de um semáforo.
const char *semaforo_obter_cor_preenchimento(Semaforo semaforo) {
    return retangulo_obter_cor_preenchimento((Retangulo) semaforo);
}

// Define a cor de preenchimento de um semáforo.
void semaforo_definir_cor_preenchimento(Semaforo semaforo, const char *cor_preenchimento) {
    retangulo_definir_cor_preenchimento((Retangulo) semaforo, cor_preenchimento);
}

// Define o arredondamento da borda de um semáforo.
void semaforo_definir_espessura_borda(Semaforo semaforo, double espessura_borda) {
    retangulo_definir_espessura_borda((Retangulo) semaforo, espessura_borda);
}

// Define o arredondamento da borda do semaforo.
void semaforo_definir_arredondamento_borda(Semaforo semaforo, double arredondamento_borda) {
    retangulo_definir_arredondamento_borda((Retangulo) semaforo, arredondamento_borda);
}

// Define se a borda de um semáforo é tracejada.
void semaforo_definir_borda_tracejada(Semaforo semaforo, bool tracejado) {
    retangulo_definir_borda_tracejada((Retangulo) semaforo, tracejado);
}

// Libera a memória alocada por um semáforo.
void semaforo_destruir(Semaforo semaforo) {
    retangulo_destruir((Retangulo) semaforo);
}
