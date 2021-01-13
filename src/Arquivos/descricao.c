#include "descricao.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../Estruturas/lista.h"
#include "../Estruturas/pilha.h"
#include "../Estruturas/quadtree.h"
#include "../Interfaces/figura.h"
#include "../Objetos/EquipamentosUrbanos/hidrante.h"
#include "../Objetos/EquipamentosUrbanos/posto.h"
#include "../Objetos/EquipamentosUrbanos/quadra.h"
#include "../Objetos/EquipamentosUrbanos/radio.h"
#include "../Objetos/EquipamentosUrbanos/semaforo.h"
#include "../Objetos/Formas/circulo.h"
#include "../Objetos/Formas/linha.h"
#include "../Objetos/Formas/retangulo.h"
#include "../Objetos/Outros/densidade.h"
#include "../Objetos/Outros/texto.h"
#include "../Utils/graham_scan.h"
#include "../Utils/logging.h"

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
    int lista_max_formas;
    int lista_max_quadras;
    int lista_max_semaforos;
    int lista_max_hidrantes;
    int lista_max_bases;
} NumerosMaximos;

// Inicializa os valores máximo de cada tipo de figura inicialmente como 1000.
NumerosMaximos criar_maximos() {
    NumerosMaximos propMax = {.lista_max_bases = 1000,
                              .lista_max_formas = 1000,
                              .lista_max_hidrantes = 1000,
                              .lista_max_quadras = 1000,
                              .lista_max_semaforos = 1000};
    return propMax;
}

// Inicializa as propriedades de cada tipo de figura.
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
    sscanf(linha, "%*s %d %d %d %d %d", &propMax->lista_max_formas, &propMax->lista_max_quadras,
           &propMax->lista_max_hidrantes, &propMax->lista_max_semaforos, &propMax->lista_max_bases);
}

// Cria uma figura contendo um círculo com base em uma linha e as propriedades definidas.
Circulo ler_circulo(const char *linha, PropriedadesCirculos prop) {
    Circulo cir = circulo_ler(linha);
    if (prop.definido) {
        circulo_definir_espessura_borda(cir, prop.espessura_borda);
    }
    return cir;
}

// Define as propriedades que devem ser aplicadas a todos os círculos com base em uma linha.
void definir_propriedades_circulos(const char *linha, PropriedadesCirculos *prop) {
    sscanf(linha, "%*s %s %*s", prop->espessura_borda);
    prop->definido = true;
}

// Cria uma figura contendo um hidrante com base em uma linha e as propriedades definidas.
Hidrante ler_hidrante(const char *linha, PropriedadesHidrantes prop) {
    Hidrante hid = hidrante_ler(linha);
    if (prop.definido) {
        hidrante_definir_espessura_borda(hid, prop.espessura_borda);
        hidrante_definir_cor_borda(hid, prop.cor_borda);
        hidrante_definir_cor_preenchimento(hid, prop.cor_preenchimento);
    }
    return hid;
}

// Define as propriedades que devem ser aplicadas a todos os hidrantes com base em uma linha.
void definir_propriedades_hidrantes(const char *linha, PropriedadesHidrantes *prop) {
    sscanf(linha, "%*s %s %s %s", prop->espessura_borda, prop->cor_preenchimento, prop->cor_borda);
    prop->definido = true;
}

// Cria uma figura contendo uma quadra com base em uma linha e as propriedades definidas.
Quadra ler_quadra(const char *linha, PropriedadesQuadras prop) {
    Quadra qua = quadra_ler(linha);
    if (prop.definido) {
        quadra_definir_espessura_borda(qua, prop.espessura_borda);
        quadra_definir_cor_borda(qua, prop.cor_borda);
        quadra_definir_cor_preenchimento(qua, prop.cor_preenchimento);
    }
    return qua;
}

// Define as propriedades que devem ser aplicadas a todos as quadras com base em uma linha.
void definir_propriedades_quadras(const char *linha, PropriedadesQuadras *prop) {
    sscanf(linha, "%*s %s %s %s", prop->espessura_borda, prop->cor_preenchimento, prop->cor_borda);
    prop->definido = true;
}

// Cria uma figura contendo um retângulo com base em uma linha e as propriedades definidas.
Retangulo ler_retangulo(const char *linha, PropriedadesRetangulos prop) {
    Retangulo ret = retangulo_ler(linha);
    if (prop.definido) {
        retangulo_definir_espessura_borda(ret, prop.espessura_borda);
    }
    return ret;
}

// Define as propriedades que devem ser aplicadas a todos os retângulos com base em uma linha.
void definir_propriedades_retangulos(const char *linha, PropriedadesRetangulos *prop) {
    sscanf(linha, "%*s %*s %s", prop->espessura_borda);
    prop->definido = true;
}

// Cria uma figura contendo um rádio com base em uma linha e as propriedades definidas.
Radio ler_radio(const char *linha, PropriedadesRadios prop) {
    Radio rad = radio_ler(linha);
    if (prop.definido) {
        radio_definir_espessura_borda(rad, prop.espessura_borda);
        radio_definir_cor_borda(rad, prop.cor_borda);
        radio_definir_cor_preenchimento(rad, prop.cor_preenchimento);
    }
    return rad;
}

// Define as propriedades que devem ser aplicadas a todos os rádios com base em uma linha.
void definir_propriedades_radios(const char *linha, PropriedadesRadios *prop) {
    sscanf(linha, "%*s %s %s %s", prop->espessura_borda, prop->cor_preenchimento, prop->cor_borda);
    prop->definido = true;
}

// Cria uma figura contendo um semáforo com base em uma linha e as propriedades definidas.
Semaforo ler_semaforo(const char *linha, PropriedadesSemaforos prop) {
    Semaforo sem = semaforo_ler(linha);
    if (prop.definido) {
        semaforo_definir_espessura_borda(sem, prop.espessura_borda);
        semaforo_definir_cor_borda(sem, prop.cor_borda);
        semaforo_definir_cor_preenchimento(sem, prop.cor_preenchimento);
    }
    return sem;
}

// Define as propriedades que devem ser aplicadas a todos os semáforos com base em uma linha.
void definir_propriedades_semaforos(const char *linha, PropriedadesSemaforos *prop) {
    sscanf(linha, "%*s %s %s %s", prop->espessura_borda, prop->cor_preenchimento, prop->cor_borda);
    prop->definido = true;
}

// Atualiza a cor das sombras das quadras de acordo com suas densidades.
void atualizar_sombras_quadras(QuadTree quadras, QuadTree densidades) {
    Lista lista_densidades = quadtree_para_lista(densidades);
    for (ListaNo i = lista_obter_primeiro(lista_densidades); i != NULL;
         i = lista_obter_proximo(i)) {
        Densidade densidade = lista_obter_info(i);
        double habitantes = densidade_obter_densidade(densidade);

        char cor[20];
        cor[0] = '\0';
        if (habitantes >= 10 && habitantes <= 500) {
            strcpy(cor, "#sombra_10_500");
        } else if (habitantes > 500 && habitantes <= 1500) {
            strcpy(cor, "#sombra_500_1500");
        } else if (habitantes > 1500 && habitantes <= 3000) {
            strcpy(cor, "#sombra_1500_3000");
        } else if (habitantes > 3000 && habitantes <= 4500) {
            strcpy(cor, "#sombra_3000_4500");
        } else if (habitantes > 4500 && habitantes <= 6000) {
            strcpy(cor, "#sombra_4500_6000");
        } else if (habitantes > 6000) {
            strcpy(cor, "#sombra_6000");
        }

        double x_inicio = figura_obter_x_inicio(densidade);
        double y_inicio = figura_obter_y_inicio(densidade);
        double x_fim = figura_obter_x_fim(densidade);
        double y_fim = figura_obter_y_fim(densidade);
        Lista quadras_contidas = nosDentroRetanguloQt(quadras, x_inicio, y_inicio, x_fim, y_fim);

        for (ListaNo j = lista_obter_primeiro(quadras_contidas); j != NULL;
             j = lista_obter_proximo(j)) {
            Quadra quad = getInfoQt(lista_obter_info(j));
            quadra_definir_cor_sombra(quad, cor);
        }
        lista_destruir(quadras_contidas);
    }
    lista_destruir(lista_densidades);
}

// Lê um arquivo de descrição fornecido a função e adiciona as figuras descritas em suas linha
// como elementos de uma lista.
void descricao_ler(const char *caminho_geo, QuadTree formas, QuadTree quadras, QuadTree hidrantes,
                   QuadTree radios, QuadTree semaforos, QuadTree postos, QuadTree densidades) {
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

        if (strcmp("c", comando) == 0 && figuras_criadas <= maximo.lista_max_formas) {
            Circulo novo_circulo = ler_circulo(linha, propriedades.cir);
            insereQt(formas, ponto_criar_com_figura(novo_circulo), novo_circulo);
            figuras_criadas++;
        } else if (strcmp("dd", comando) == 0) {
            Densidade nova_densidade = densidade_ler(linha);
            insereQt(densidades, ponto_criar_com_figura(nova_densidade), nova_densidade);
        } else if (strcmp("r", comando) == 0 && figuras_criadas <= maximo.lista_max_formas) {
            Retangulo novo_retangulo = ler_retangulo(linha, propriedades.ret);
            insereQt(formas, ponto_criar_com_figura(novo_retangulo), novo_retangulo);
            figuras_criadas++;
        } else if (strcmp("q", comando) == 0 && quadras_criadas <= maximo.lista_max_quadras) {
            Quadra nova_quadra = ler_quadra(linha, propriedades.qua);
            insereQt(quadras, ponto_criar_com_figura(nova_quadra), nova_quadra);
            quadras_criadas++;
        } else if (strcmp("h", comando) == 0 && hidrantes_criados <= maximo.lista_max_hidrantes) {
            Hidrante novo_hidrante = ler_hidrante(linha, propriedades.hid);
            insereQt(hidrantes, ponto_criar_com_figura(novo_hidrante), novo_hidrante);
            hidrantes_criados++;
        } else if (strcmp("ps", comando) == 0) {
            Posto novo_posto = posto_ler(linha);
            insereQt(postos, ponto_criar_com_figura(novo_posto), novo_posto);
        } else if (strcmp("s", comando) == 0 && semaforos_criados <= maximo.lista_max_semaforos) {
            Semaforo novo_semaforo = ler_semaforo(linha, propriedades.sem);
            insereQt(semaforos, ponto_criar_com_figura(novo_semaforo), novo_semaforo);
            semaforos_criados++;
        } else if (strcmp("rb", comando) == 0 && bases_criadas <= maximo.lista_max_bases) {
            Radio novo_radio = ler_radio(linha, propriedades.rad);
            insereQt(radios, ponto_criar_com_figura(novo_radio), novo_radio);
            bases_criadas++;
        } else if (strcmp("t", comando) == 0 && figuras_criadas <= maximo.lista_max_formas) {
            Texto novo_texto = texto_ler(linha);
            insereQt(formas, ponto_criar_com_figura(novo_texto), novo_texto);
            figuras_criadas++;
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
            LOG_INFO("Novo valor máximo de formas: %d\n", maximo.lista_max_formas);
            LOG_INFO("Novo valor máximo de quadras: %d\n", maximo.lista_max_quadras);
            LOG_INFO("Novo valor máximo de hidrantes: %d\n", maximo.lista_max_hidrantes);
            LOG_INFO("Novo valor máximo de semaforos: %d\n", maximo.lista_max_semaforos);
            LOG_INFO("Novo valor máximo de radio-bases: %d\n", maximo.lista_max_bases);
        }
    }

    atualizar_sombras_quadras(quadras, densidades);

    fclose(arquivo_descricao);
}
