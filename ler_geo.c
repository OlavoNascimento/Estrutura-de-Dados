#include "ler_geo.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "circulo.h"
#include "densidade.h"
#include "figuras.h"
#include "hidrante.h"
#include "linha.h"
#include "lista.h"
#include "logging.h"
#include "posto.h"
#include "quadra.h"
#include "radio_base.h"
#include "retangulo.h"
#include "semaforo.h"
#include "texto.h"

// Tamanho maxímo de um comando do arquivo de descrição.
#define LINHA_MAX 300
// Tamanho maxímo da abreviação do tipo de uma figura no arquivo de descrição.
#define TIPO_FIGURA_TAMANHO 100

typedef struct {
    bool definido;
    char espessura_borda[20];
} PropriedadesCirculos;

typedef struct {
    bool definido;
    char espessura_borda[20];
    char cor_borda[20];
    char cor_preenchimento[20];
} PropriedadesHidrantes;

typedef struct {
    bool definido;
    char espessura_borda[20];
    char cor_borda[20];
    char cor_preenchimento[20];
} PropriedadesQuadras;

typedef struct {
    bool definido;
    char espessura_borda[20];
} PropriedadesRetangulos;

typedef struct {
    bool definido;
    char espessura_borda[20];
    char cor_borda[20];
    char cor_preenchimento[20];
} PropriedadesSemaforos;

typedef struct {
    bool definido;
    char espessura_borda[20];
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

typedef struct {
    int lista_max_quadras;
    int lista_max_semaforos;
    int lista_max_hidrantes;
    int lista_max_bases;
    int lista_max_figuras;
} NumerosMaximos;

// Inicializa os valores máximo de cada tipo de figura inicialmente como 1000.
NumerosMaximos criar_maximos() {
    NumerosMaximos propMax;
    propMax.lista_max_bases = 1000;
    propMax.lista_max_figuras = 1000;
    propMax.lista_max_hidrantes = 1000;
    propMax.lista_max_quadras = 1000;
    propMax.lista_max_semaforos = 1000;
    return propMax;
}

// Inicializa as propriedades de cada tipo de figura inicialmente como vazias.
PropriedadesFiguras criar_propriedades() {
    PropriedadesFiguras prop;
    // Inicializa todas as propriedades como vazias (0 para int e NULL para char[]).
    prop.cir = (PropriedadesCirculos){0};
    prop.hid = (PropriedadesHidrantes){0};
    prop.qua = (PropriedadesQuadras){0};
    prop.ret = (PropriedadesRetangulos){0};
    prop.sem = (PropriedadesSemaforos){0};
    prop.rad = (PropriedadesRadios){0};
    return prop;
}

// Define o máximo de figuras que devem ser criadas para cada tipo de figura.
void definir_max_figuras(const char *linha, NumerosMaximos *propMax) {
    sscanf(linha, "%*s %d %d %d %d %d", &propMax->lista_max_figuras, &propMax->lista_max_quadras,
           &propMax->lista_max_hidrantes, &propMax->lista_max_semaforos, &propMax->lista_max_bases);
}

// Cria uma figura contendo um círculo com base em uma linha e as propriedades definidas.
Figura ler_circulo(const char *linha, PropriedadesCirculos prop) {
    Circulo cir = circulo_ler(linha);
    if (prop.definido) {
        circulo_definir_espessura_borda(cir, prop.espessura_borda);
    }
    return figura_criar(cir, TIPO_CIRCULO);
}

// Define as propriedades que devem ser aplicadas a todos os círculos com base em uma linha.
void definir_propriedades_circulos(const char *linha, PropriedadesCirculos *prop) {
    sscanf(linha, "%*s %s %*s", prop->espessura_borda);
    prop->definido = true;
}

// Cria uma figura contendo um hidrante com base em uma linha e as propriedades definidas.
Figura ler_hidrante(const char *linha, PropriedadesHidrantes prop) {
    Hidrante hid = hidrante_ler(linha);
    if (prop.definido) {
        hidrante_definir_espessura_borda(hid, prop.espessura_borda);
        hidrante_definir_cor_borda(hid, prop.cor_borda);
        hidrante_definir_cor_preenchimento(hid, prop.cor_preenchimento);
    }
    return figura_criar(hid, TIPO_HIDRANTE);
}

// Define as propriedades que devem ser aplicadas a todos os hidrantes com base em uma linha.
void definir_propriedades_hidrantes(const char *linha, PropriedadesHidrantes *prop) {
    sscanf(linha, "%*s %s %s %s", prop->espessura_borda, prop->cor_preenchimento, prop->cor_borda);
    prop->definido = true;
}

// Cria uma figura contendo uma quadra com base em uma linha e as propriedades definidas.
Figura ler_quadra(const char *linha, PropriedadesQuadras prop) {
    Quadra qua = quadra_ler(linha);
    if (prop.definido) {
        quadra_definir_espessura_borda(qua, prop.espessura_borda);
        quadra_definir_cor_borda(qua, prop.cor_borda);
        quadra_definir_cor_preenchimento(qua, prop.cor_preenchimento);
    }
    return figura_criar(qua, TIPO_QUADRA);
}

// Define as propriedades que devem ser aplicadas a todos as quadras com base em uma linha.
void definir_propriedades_quadras(const char *linha, PropriedadesQuadras *prop) {
    sscanf(linha, "%*s %s %s %s", prop->espessura_borda, prop->cor_preenchimento, prop->cor_borda);
    prop->definido = true;
}

// Cria uma figura contendo um retângulo com base em uma linha e as propriedades definidas.
Figura ler_retangulo(const char *linha, PropriedadesRetangulos prop) {
    Retangulo ret = retangulo_ler(linha);
    if (prop.definido) {
        retangulo_definir_espessura_borda(ret, prop.espessura_borda);
    }
    return figura_criar(ret, TIPO_RETANGULO);
}

// Define as propriedades que devem ser aplicadas a todos os retângulos com base em uma linha.
void definir_propriedades_retangulos(const char *linha, PropriedadesRetangulos *prop) {
    sscanf(linha, "%*s %*s %s", prop->espessura_borda);
    prop->definido = true;
}

// Cria uma figura contendo um rádio com base em uma linha e as propriedades definidas.
Figura ler_radio(const char *linha, PropriedadesRadios prop) {
    Radio rad = radio_ler(linha);
    if (prop.definido) {
        radio_definir_espessura_borda(rad, prop.espessura_borda);
        radio_definir_cor_borda(rad, prop.cor_borda);
        radio_definir_cor_preenchimento(rad, prop.cor_preenchimento);
    }
    return figura_criar(rad, TIPO_RADIO);
}

// Define as propriedades que devem ser aplicadas a todos os rádios com base em uma linha.
void definir_propriedades_radios(const char *linha, PropriedadesRadios *prop) {
    sscanf(linha, "%*s %s %s %s", prop->espessura_borda, prop->cor_preenchimento, prop->cor_borda);
    prop->definido = true;
}

// Cria uma figura contendo um semáforo com base em uma linha e as propriedades definidas.
Figura ler_semaforo(const char *linha, PropriedadesSemaforos prop) {
    Semaforo sem = semaforo_ler(linha);
    if (prop.definido) {
        semaforo_definir_espessura_borda(sem, prop.espessura_borda);
        semaforo_definir_cor_borda(sem, prop.cor_borda);
        semaforo_definir_cor_preenchimento(sem, prop.cor_preenchimento);
    }
    return figura_criar(sem, TIPO_SEMAFORO);
}

// Define as propriedades que devem ser aplicadas a todos os semáforos com base em uma linha.
void definir_propriedades_semaforos(const char *linha, PropriedadesSemaforos *prop) {
    sscanf(linha, "%*s %s %s %s", prop->espessura_borda, prop->cor_preenchimento, prop->cor_borda);
    prop->definido = true;
}

// Cria uma figura contendo um texto com base em uma linha e as propriedades definidas.
Figura ler_texto(const char *linha) {
    Texto tex = texto_ler(linha);
    return figura_criar(tex, TIPO_TEXTO);
}

// Lê um arquivo de descrição fornecido a função e adiciona as figuras descritas em suas linha como
// elementos de uma lista.
void ler_geo(const char *caminho_geo, Lista lista_formas, Lista lista_quadras,
             Lista lista_hidrantes, Lista lista_radios, Lista lista_semaforos, Lista lista_postos,
             Lista lista_densidades) {
    FILE *arquivo_descricao = fopen(caminho_geo, "r");
    if (arquivo_descricao == NULL) {
        fprintf(stderr, "ERRO: Falha ao ler arquivo de descrição: %s!\n", caminho_geo);
        return;
    }

    NumerosMaximos maximo = criar_maximos();
    int figuras_criadas = 0;
    int quadras_criadas = 0;
    int semaforos_criados = 0;
    int hidrantes_criados = 0;
    int bases_criadas = 0;
    PropriedadesFiguras propriedades = criar_propriedades();

    char linha[LINHA_MAX];
    while (fgets(linha, LINHA_MAX, arquivo_descricao) != NULL) {
        char comando[TIPO_FIGURA_TAMANHO];
        sscanf(linha, "%s", comando);

        Figura nova_figura = NULL;
        if (strcmp("c", comando) == 0 && figuras_criadas <= maximo.lista_max_figuras) {
            nova_figura = ler_circulo(linha, propriedades.cir);
        } else if (strcmp("dd", comando) == 0) {
            nova_figura = figura_criar(densidade_ler(linha), TIPO_DENSIDADE);
        } else if (strcmp("r", comando) == 0 && figuras_criadas <= maximo.lista_max_figuras) {
            nova_figura = ler_retangulo(linha, propriedades.ret);
        } else if (strcmp("q", comando) == 0 && quadras_criadas <= maximo.lista_max_quadras) {
            nova_figura = ler_quadra(linha, propriedades.qua);
        } else if (strcmp("h", comando) == 0 && hidrantes_criados <= maximo.lista_max_hidrantes) {
            nova_figura = ler_hidrante(linha, propriedades.hid);
        } else if (strcmp("ps", comando) == 0) {
            nova_figura = figura_criar(posto_ler(linha), TIPO_POSTO);
        } else if (strcmp("s", comando) == 0 && semaforos_criados <= maximo.lista_max_semaforos) {
            nova_figura = ler_semaforo(linha, propriedades.sem);
        } else if (strcmp("rb", comando) == 0 && bases_criadas <= maximo.lista_max_bases) {
            nova_figura = ler_radio(linha, propriedades.rad);
        } else if (strcmp("t", comando) == 0 && figuras_criadas <= maximo.lista_max_figuras) {
            nova_figura = ler_texto(linha);
        } else if (strcmp("cq", comando) == 0) {
            definir_propriedades_quadras(linha, &propriedades.qua);
        } else if (strcmp("ch", comando) == 0) {
            definir_propriedades_hidrantes(linha, &propriedades.hid);
        } else if (strcmp("cr", comando) == 0) {
            definir_propriedades_radios(linha, &propriedades.rad);
        } else if (strcmp("cs", comando) == 0) {
            definir_propriedades_semaforos(linha, &propriedades.sem);
        } else if (strcmp("sw", comando) == 0) {
            definir_propriedades_circulos(linha, &propriedades.cir);
            definir_propriedades_retangulos(linha, &propriedades.ret);
        } else if (strcmp("nx", comando) == 0) {
            definir_max_figuras(linha, &maximo);
            LOG_INFO("Novo valor máximo de figuras: %d\n", maximo.lista_max_figuras);
            LOG_INFO("Novo valor máximo de quadras: %d\n", maximo.lista_max_quadras);
            LOG_INFO("Novo valor máximo de hidrantes: %d\n", maximo.lista_max_hidrantes);
            LOG_INFO("Novo valor máximo de semaforos: %d\n", maximo.lista_max_semaforos);
            LOG_INFO("Novo valor máximo de radio-bases: %d\n", maximo.lista_max_bases);
        }

        if (nova_figura != NULL) {
            TiposFigura tipo = figura_obter_tipo(nova_figura);
            switch (tipo) {
                case TIPO_DENSIDADE:
                    lista_insert_final(lista_densidades, nova_figura);
                case TIPO_CIRCULO:
                case TIPO_RETANGULO:
                case TIPO_TEXTO:
                    lista_insert_final(lista_formas, nova_figura);
                    figuras_criadas++;
                    break;
                case TIPO_HIDRANTE:
                    lista_insert_final(lista_hidrantes, nova_figura);
                    hidrantes_criados++;
                    break;
                case TIPO_QUADRA:
                    lista_insert_final(lista_quadras, nova_figura);
                    quadras_criadas++;
                    break;
                case TIPO_RADIO:
                    lista_insert_final(lista_radios, nova_figura);
                    bases_criadas++;
                    break;
                case TIPO_POSTO:
                    lista_insert_final(lista_postos, nova_figura);
                    break;
                case TIPO_SEMAFORO:
                    lista_insert_final(lista_semaforos, nova_figura);
                    semaforos_criados++;
                    break;
                default:
                    LOG_ERROR("Tipo de figura desconhecido ao ler arquivo de descrição: %d!\n",
                              tipo);
                    return;
            }
        }
    }
    fclose(arquivo_descricao);
}
