#include "semaforo.h"

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
} SemaforoImp;

const char *semaforo_obter_tipo() {
    return "semáforo";
}

// Escreve no svg as informações de uma radio base
void semaforo_escrever_svg(Semaforo semaforo, FILE *arquivo) {
    retangulo_escrever_svg(semaforo, arquivo);

    double x = figura_obter_x_centro(semaforo);
    double y = figura_obter_y_centro(semaforo) + 4;
    // Rótulo do semáforo.
    Texto texto_semaforo = texto_criar("", x, y, "none", "white", "S", true);
    texto_escrever_svg(texto_semaforo, arquivo);
    texto_destruir(texto_semaforo);
}

// Conecta as funções do objeto Semaforo com as da interface figura.
// Como o struct SemaforoImp é idêntico ao struct RetanguloImp as funções utilizadas em um
// objeto Retangulo podem ser reaproveitadas.
static FiguraInterface semaforo_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_escrever_informacoes(interface, retangulo_escrever_informacoes);
    figura_registrar_escrever_svg(interface, semaforo_escrever_svg);

    figura_registrar_obter_tipo(interface, semaforo_obter_tipo);

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

// Cria e inicializa um struct SemaforoImp com os valores passados.
Semaforo semaforo_criar(const char id[100], double largura, double altura, double x, double y) {
    if (id == NULL) {
        LOG_ERRO("Não é possível criar um semáforo com id NULL!\n");
        return NULL;
    }
    SemaforoImp *semImp = malloc(sizeof *semImp);
    strcpy(semImp->id, id);
    semImp->largura = largura;
    semImp->altura = altura;
    semImp->x = x;
    semImp->y = y;
    strcpy(semImp->cor_borda, "red");
    strcpy(semImp->cor_preenchimento, "green");
    semImp->arredondamento_borda = 0;
    semImp->borda_tracejada = false;
    strcpy(semImp->espessura_borda, "1px");

    semImp->vtable = semaforo_criar_interface_figura();
    return semImp;
}

// Cria um semaforo com base em informações de uma linha.
Semaforo semaforo_ler(const char *linha) {
    char id[100];
    double x;
    double y;
    double largura = 12;
    double altura = 30;
    sscanf(linha, "%*s %s %lf %lf", id, &x, &y);
    return semaforo_criar(id, largura, altura, x - largura / 2, y - altura / 2);
}

// Retorna o id de um semáforo.
const char *semaforo_obter_id(Semaforo semaforo) {
    return retangulo_obter_id(semaforo);
}

// Retorna a coordenada x de um semáforo.
double semaforo_obter_x(Semaforo semaforo) {
    return retangulo_obter_x(semaforo);
}

// Retorna a coordenada y de um semáforo.
double semaforo_obter_y(Semaforo semaforo) {
    return retangulo_obter_y(semaforo);
}

// Retorna a largura de um semáforo.
double semaforo_obter_largura(Semaforo semaforo) {
    return retangulo_obter_largura(semaforo);
}

// Retorna a altura de um semáforo.
double semaforo_obter_altura(Semaforo semaforo) {
    return retangulo_obter_altura(semaforo);
}

// Retorna a cor da borda de um semáforo.
const char *semaforo_obter_cor_borda(Semaforo semaforo) {
    return retangulo_obter_cor_borda(semaforo);
}

// Define a cor da borda de um semaforo.
void semaforo_definir_cor_borda(Semaforo semaforo, const char *cor_borda) {
    retangulo_definir_cor_borda(semaforo, cor_borda);
}

// Retorna a cor de preenchimento de um semáforo.
const char *semaforo_obter_cor_preenchimento(Semaforo semaforo) {
    return retangulo_obter_cor_preenchimento(semaforo);
}

// Define a cor de preenchimento de um semaforo.
void semaforo_definir_cor_preenchimento(Semaforo semaforo, const char *cor_preenchimento) {
    retangulo_definir_cor_preenchimento(semaforo, cor_preenchimento);
}

// Define o arredondamento da borda de um semáforo.
void semaforo_definir_espessura_borda(Semaforo semaforo, const char *espessura_borda) {
    retangulo_definir_espessura_borda(semaforo, espessura_borda);
}

// Define se a borda do semaforo é tracejada.
void semaforo_definir_borda_tracejada(Semaforo semaforo, bool tracejado) {
    retangulo_definir_borda_tracejada(semaforo, tracejado);
}

// Define o arredondamento da borda do semaforo.
void semaforo_definir_arredondamento_borda(Semaforo semaforo, double arredondamento_borda) {
    retangulo_definir_arredondamento_borda(semaforo, arredondamento_borda);
}

// Libera a memória alocada por um semáforo.
void semaforo_destruir(Semaforo semaforo) {
    retangulo_destruir(semaforo);
}
