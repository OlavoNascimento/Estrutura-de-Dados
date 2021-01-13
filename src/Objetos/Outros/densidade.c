#include "densidade.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Estruturas/lista.h"
#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"

typedef struct {
    FiguraInterface vtable;
    double x;
    double y;
    double altura;
    double largura;
    double densidade;
} DenImp;

const char *densidade_obter_tipo() {
    return "densidade";
}

double densidade_obter_x(Densidade densidade) {
    DenImp *denImp = densidade;
    return denImp->x;
}

double densidade_obter_y(Densidade densidade) {
    DenImp *denImp = densidade;
    return denImp->y;
}

double densidade_obter_x_fim(Densidade densidade) {
    DenImp *denImp = densidade;
    return denImp->x + denImp->largura;
}

double densidade_obter_y_fim(Densidade densidade) {
    DenImp *denImp = densidade;
    return denImp->y + denImp->altura;
}

// Conecta as funções do objeto Densidade com as da interface figura.
static FiguraInterface densidade_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, densidade_obter_tipo);
    figura_registrar_obter_x(interface, densidade_obter_x);
    figura_registrar_obter_y(interface, densidade_obter_y);
    figura_registrar_obter_x_inicio(interface, densidade_obter_x);
    figura_registrar_obter_y_inicio(interface, densidade_obter_y);
    figura_registrar_obter_x_fim(interface, densidade_obter_x_fim);
    figura_registrar_obter_y_fim(interface, densidade_obter_y_fim);
    figura_registrar_destruir(interface, densidade_destruir);
    return interface;
}

Densidade densidade_criar(double x, double y, double largura, double altura, double densidade) {
    DenImp *denImp = malloc(sizeof *denImp);
    denImp->largura = largura;
    denImp->altura = altura;
    denImp->x = x;
    denImp->y = y;
    denImp->densidade = densidade;

    denImp->vtable = densidade_criar_interface_figura();
    return denImp;
}

Densidade densidade_ler(const char *linha) {
    double densidade;
    double x;
    double y;
    double largura;
    double altura;
    sscanf(linha, "dd %lf %lf %lf %lf %lf", &x, &y, &largura, &altura, &densidade);
    return densidade_criar(x, y, largura, altura, densidade);
}

// Retorna o número de habitantes que vivem dentro de uma densidade.
double densidade_obter_densidade(Densidade densidade) {
    DenImp *denImp = (DenImp *) densidade;
    return denImp->densidade;
}

// Retorna o número de habitantes contidos na área da densidade.
double densidade_calcular_habitantes(DenImp *regiao) {
    return regiao->densidade * (regiao->largura / 1000) * (regiao->altura / 1000);
}

// Retorna true se um ponto é contido dentro da densidade.
bool densidade_contem_ponto(DenImp *regiao, double x, double y) {
    if (x >= regiao->x && y >= regiao->y) {
        if (x <= (regiao->x + regiao->largura) && y <= (regiao->y + regiao->altura)) {
            return true;
        }
    }
    return false;
}

// Retorna o número de habitantes que vivem dentro de uma densidade.
double densidade_buscar_habitantes_ponto(Lista lista_densidades, double x, double y) {
    for (ListaNo i = lista_obter_primeiro(lista_densidades); i != NULL;
         i = lista_obter_proximo(i)) {
        DenImp *regiao = (DenImp *) lista_obter_info(i);
        if (densidade_contem_ponto(regiao, x, y))
            return densidade_calcular_habitantes(regiao);
    }
    return 0;
}

// Retorna true se uma quadra esta contida por uma densidade.
bool densidade_contem_quadra(DenImp *densidade, Figura quadra) {
    if (densidade->x > figura_obter_x_inicio(quadra) ||
        densidade->x + densidade->largura < figura_obter_x_fim(quadra))
        return false;
    if (densidade->y > figura_obter_y_inicio(quadra) ||
        densidade->y + densidade->altura < figura_obter_y_fim(quadra))
        return false;
    return true;
}

// Libera a memória utilizada por uma densidade.
void densidade_destruir(Densidade densidade) {
    DenImp *denImp = (DenImp *) densidade;
    free(denImp->vtable);
    free(denImp);
}
