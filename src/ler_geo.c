#include "ler_geo.h"

#include <stdio.h>
#include <string.h>

#include "circulo.h"
#include "figuras.h"
#include "hidrante.h"
#include "linha.h"
#include "lista.h"
#include "quadra.h"
#include "radio_base.h"
#include "retangulo.h"
#include "semaforo.h"
#include "texto.h"

// Tamanho maxímo de um comando do arquivo de descrição.
#define LINHA_MAX 300
// Tamanho maxímo da abreviação do tipo de uma figura
#define TIPO_FIGURA_TAMANHO 100

typedef struct {
    int espessura_borda;
} PropriedadesCirculos;

typedef struct {
    int espessura_borda;
    char cor_borda[20];
    char cor_preenchimento[20];
} PropriedadesHidrantes;

typedef struct {
    int espessura_borda;
    char cor_borda[20];
    char cor_preenchimento[20];
} PropriedadesQuadras;

typedef struct {
    int espessura_borda;
} PropriedadesRetangulos;

typedef struct {
    int espessura_borda;
    char cor_borda[20];
    char cor_preenchimento[20];
} PropriedadesSemaforos;

typedef struct {
    int espessura_borda;
    char cor_borda[20];
    char cor_preenchimento[20];
} PropriedadesRadios;

typedef struct {
    PropriedadesCirculos cir;
    PropriedadesHidrantes hid;
    PropriedadesQuadras qua;
    PropriedadesRetangulos ret;
    PropriedadesSemaforos sem;
    PropriedadesRadios rad;
} PropriedadesFiguras;

PropriedadesFiguras criar_propriedades() {
    PropriedadesFiguras prop;
    prop.cir = (PropriedadesCirculos){0};
    prop.hid = (PropriedadesHidrantes){0};
    prop.qua = (PropriedadesQuadras){0};
    prop.ret = (PropriedadesRetangulos){0};
    prop.sem = (PropriedadesSemaforos){0};
    prop.rad = (PropriedadesRadios){0};
    return prop;
}

Figura ler_circulo(const char *linha) {
    Circulo cir = circulo_ler(linha);
    return figura_criar(cir, TIPO_CIRCULO);
}

Figura ler_hidrante(const char *linha) {
    Hidrante hid = hidrante_ler(linha);
    return figura_criar(hid, TIPO_HIDRANTE);
}

Figura ler_quadra(const char *linha) {
    Quadra qua = quadra_ler(linha);
    return figura_criar(qua, TIPO_QUADRA);
}

Figura ler_retangulo(const char *linha) {
    Retangulo ret = retangulo_ler(linha);
    return figura_criar(ret, TIPO_RETANGULO);
}

Figura ler_radio(const char *linha, PropriedadesRadios prop) {
    Radio rad = radio_ler(linha);
    if (prop.espessura_borda != 0) {
        radio_definir_espessura_borda(rad, prop.espessura_borda);
        radio_definir_cor_borda(rad, prop.cor_borda);
        radio_definir_cor_preenchimento(rad, prop.cor_preenchimento);
    }
    return figura_criar(rad, TIPO_RADIO);
}

void definir_propriedades_radio(const char *linha, PropriedadesRadios *prop) {
    int espessura_borda;
    char cor_borda[20];
    char cor_preenchimento[20];
    sscanf(linha, "%*s %d %s %s", &prop->espessura_borda, prop->cor_borda, prop->cor_preenchimento);
}

Figura ler_semaforo(const char *linha) {
    Semaforo sem = semaforo_ler(linha);
    return figura_criar(sem, TIPO_SEMAFORO);
}

Figura ler_texto(const char *linha) {
    Texto tex = texto_ler(linha);
    return figura_criar(tex, TIPO_TEXTO);
}

// Lê um arquivo de descrição fornecido a função e adiciona as figuras descritas em suas linha como
// elementos de uma lista.
Lista *ler_geo(const char *caminho_geo) {
    FILE *arquivo_descricao = fopen(caminho_geo, "r");
    if (arquivo_descricao == NULL) {
        fprintf(stderr, "ERRO: Falha ao ler arquivo de descrição: %s!\n", caminho_geo);
        return NULL;
    }
    Lista *lista = lista_create();

    int figuras_criadas = 0;
    int lista_max_figs = 1000;
    PropriedadesFiguras propriedades = criar_propriedades();

    char linha[LINHA_MAX];
    while (fgets(linha, LINHA_MAX, arquivo_descricao) != NULL && figuras_criadas < lista_max_figs) {
        char comando[TIPO_FIGURA_TAMANHO];
        sscanf(linha, "%s", comando);

        Figura nova_figura = NULL;
        if (strcmp("c", comando) == 0) {
            nova_figura = ler_circulo(linha);
        } else if (strcmp("r", comando) == 0) {
            nova_figura = ler_retangulo(linha);
        } else if (strcmp("q", comando) == 0) {
            nova_figura = ler_quadra(linha);
        } else if (strcmp("h", comando) == 0) {
            nova_figura = ler_hidrante(linha);
        } else if (strcmp("s", comando) == 0) {
            nova_figura = ler_semaforo(linha);
        } else if (strcmp("rb", comando) == 0) {
            nova_figura = ler_radio(linha, propriedades.rad);
        } else if (strcmp("t", comando) == 0) {
            nova_figura = ler_texto(linha);
        } else if (strcmp("cr", comando) == 0) {
            definir_propriedades_radio(linha, &propriedades.rad);
        } else if (strcmp("nx", comando) == 0) {
            sscanf(linha, "nx %d", &lista_max_figs);
            printf("Novo valor máximo: %d\n", lista_max_figs);
        }

        if (nova_figura != NULL) {
            lista_insert_final(lista, nova_figura);
            figuras_criadas++;
        }
    }
    fclose(arquivo_descricao);

    return lista;
}
