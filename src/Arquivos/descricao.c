#include "descricao.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../Estruturas/lista.h"
#include "../Estruturas/pilha.h"
#include "../Estruturas/quadtree.h"
#include "../Estruturas/tabelahash.h"
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
#include "../Utils/logging.h"

// Tamanho maxímo de um comando do arquivo de descrição.
#define TAMANHO_COMANDO 300

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
    PropriedadesCirculos circulos;
    PropriedadesHidrantes hidrantes;
    PropriedadesQuadras quadras;
    PropriedadesRetangulos retangulos;
    PropriedadesSemaforos semaforos;
    PropriedadesRadios radios;
} PropriedadesFiguras;

typedef struct {
    int formas;
    int quadras;
    int semaforos;
    int hidrantes;
    int radios;
} NumerosMaximos;

// Inicializa os valores máximo de cada tipo de figura inicialmente como 1000.
NumerosMaximos criar_maximos() {
    NumerosMaximos propMax = {
        .radios = 1000, .formas = 1000, .hidrantes = 1000, .quadras = 1000, .semaforos = 1000};
    return propMax;
}

// Inicializa as propriedades de cada tipo de figura.
PropriedadesFiguras criar_propriedades() {
    PropriedadesFiguras prop;
    // Inicializa todas as propriedades como vazias (0 para int e NULL para char[]).
    prop.circulos = (PropriedadesCirculos){0};
    prop.hidrantes = (PropriedadesHidrantes){0};
    prop.quadras = (PropriedadesQuadras){0};
    prop.retangulos = (PropriedadesRetangulos){0};
    prop.semaforos = (PropriedadesSemaforos){0};
    prop.radios = (PropriedadesRadios){0};
    return prop;
}

// Define o máximo de figuras que devem ser criadas para cada tipo de figura.
void definir_max_figuras(NumerosMaximos *propMax, const char *linha) {
    sscanf(linha, "%*s %d %d %d %d %d", &propMax->formas, &propMax->quadras, &propMax->hidrantes,
           &propMax->semaforos, &propMax->radios);
}

// Cria um círculo, aplica as propriedades definidas e adiciona nas estruturas.
void adicionar_circulo(QuadTree formas, Tabela id_forma, PropriedadesCirculos prop,
                       const char *linha) {
    Circulo novo_circulo = circulo_ler(linha);
    if (prop.definido) {
        circulo_definir_espessura_borda(novo_circulo, prop.espessura_borda);
    }

    QtNo novo_no = insereQt(formas, ponto_criar_com_figura(novo_circulo), novo_circulo);
    tabela_inserir(id_forma, figura_obter_id(novo_circulo), novo_no);
}

// Define as propriedades que devem ser aplicadas a todos os círculos.
void definir_propriedades_circulos(PropriedadesCirculos *prop, const char *linha) {
    sscanf(linha, "%*s %s %*s", prop->espessura_borda);
    prop->definido = true;
}

// Cria uma densidade e adiciona na estrutura.
void adicionar_densidade(Lista densidades, const char *linha) {
    Densidade nova_densidade = densidade_ler(linha);
    lista_inserir_final(densidades, nova_densidade);
}

// Cria um hidrante, aplica as propriedades definidas e adiciona nas estruturas.
void adicionar_hidrante(QuadTree hidrantes, Tabela id_hidrante, PropriedadesHidrantes prop,
                        const char *linha) {
    Hidrante novo_hidrante = hidrante_ler(linha);
    if (prop.definido) {
        hidrante_definir_cor_borda(novo_hidrante, prop.cor_borda);
        hidrante_definir_cor_preenchimento(novo_hidrante, prop.cor_preenchimento);
        hidrante_definir_espessura_borda(novo_hidrante, prop.espessura_borda);
    }

    QtNo novo_no = insereQt(hidrantes, ponto_criar_com_figura(novo_hidrante), novo_hidrante);
    tabela_inserir(id_hidrante, figura_obter_id(novo_hidrante), novo_no);
}

// Define as propriedades que devem ser aplicadas a todos os hidrantes.
void definir_propriedades_hidrantes(PropriedadesHidrantes *prop, const char *linha) {
    sscanf(linha, "%*s %s %s %s", prop->espessura_borda, prop->cor_preenchimento, prop->cor_borda);
    prop->definido = true;
}

// Cria um posto e adiciona na estrutura.
void adicionar_posto(QuadTree postos, const char *linha) {
    Posto novo_posto = posto_ler(linha);
    insereQt(postos, ponto_criar_com_figura(novo_posto), novo_posto);
}

// Cria uma quadra, aplica as propriedades definidas e adiciona nas estruturas.
void adicionar_quadra(QuadTree quadras, Tabela cep_quadra, PropriedadesQuadras prop,
                      const char *linha) {
    Quadra nova_quadra = quadra_ler(linha);
    if (prop.definido) {
        quadra_definir_cor_borda(nova_quadra, prop.cor_borda);
        quadra_definir_cor_preenchimento(nova_quadra, prop.cor_preenchimento);
        quadra_definir_espessura_borda(nova_quadra, prop.espessura_borda);
    }

    QtNo novo_no = insereQt(quadras, ponto_criar_com_figura(nova_quadra), nova_quadra);
    tabela_inserir(cep_quadra, figura_obter_id(nova_quadra), novo_no);
}

// Define as propriedades que devem ser aplicadas a todas as quadras.
void definir_propriedades_quadras(PropriedadesQuadras *prop, const char *linha) {
    sscanf(linha, "%*s %s %s %s", prop->espessura_borda, prop->cor_preenchimento, prop->cor_borda);
    prop->definido = true;
}

// Cria um retângulo, aplica as propriedades definidas e adiciona nas estruturas.
void adicionar_retangulo(QuadTree formas, Tabela id_forma, PropriedadesRetangulos prop,
                         const char *linha) {
    Retangulo novo_retangulo = retangulo_ler(linha);
    if (prop.definido) {
        retangulo_definir_espessura_borda(novo_retangulo, prop.espessura_borda);
    }

    QtNo novo_no = insereQt(formas, ponto_criar_com_figura(novo_retangulo), novo_retangulo);
    tabela_inserir(id_forma, figura_obter_id(novo_retangulo), novo_no);
}

// Define as propriedades que devem ser aplicadas a todos os retângulos.
void definir_propriedades_retangulos(PropriedadesRetangulos *prop, const char *linha) {
    sscanf(linha, "%*s %*s %s", prop->espessura_borda);
    prop->definido = true;
}

// Cria um rádio, aplica as propriedades definidas e adiciona nas estruturas.
void adicionar_radio(QuadTree radios, Tabela id_radio, PropriedadesRadios prop, const char *linha) {
    Radio novo_radio = radio_ler(linha);
    if (prop.definido) {
        radio_definir_cor_borda(novo_radio, prop.cor_borda);
        radio_definir_cor_preenchimento(novo_radio, prop.cor_preenchimento);
        radio_definir_espessura_borda(novo_radio, prop.espessura_borda);
    }

    QtNo novo_no = insereQt(radios, ponto_criar_com_figura(novo_radio), novo_radio);
    tabela_inserir(id_radio, figura_obter_id(novo_radio), novo_no);
}

// Define as propriedades que devem ser aplicadas a todos os rádios.
void definir_propriedades_radios(PropriedadesRadios *prop, const char *linha) {
    sscanf(linha, "%*s %s %s %s", prop->espessura_borda, prop->cor_preenchimento, prop->cor_borda);
    prop->definido = true;
}

// Cria um semáforo, aplica as propriedades definidas e adiciona nas estruturas.
void adicionar_semaforo(QuadTree semaforos, Tabela id_semaforo, PropriedadesSemaforos prop,
                        const char *linha) {
    Semaforo novo_semaforo = semaforo_ler(linha);
    if (prop.definido) {
        semaforo_definir_cor_borda(novo_semaforo, prop.cor_borda);
        semaforo_definir_cor_preenchimento(novo_semaforo, prop.cor_preenchimento);
        semaforo_definir_espessura_borda(novo_semaforo, prop.espessura_borda);
    }

    QtNo novo_no = insereQt(semaforos, ponto_criar_com_figura(novo_semaforo), novo_semaforo);
    tabela_inserir(id_semaforo, figura_obter_id(novo_semaforo), novo_no);
}

// Define as propriedades que devem ser aplicadas a todos os semáforos.
void definir_propriedades_semaforos(PropriedadesSemaforos *prop, const char *linha) {
    sscanf(linha, "%*s %s %s %s", prop->espessura_borda, prop->cor_preenchimento, prop->cor_borda);
    prop->definido = true;
}

// Cria um texto e adiciona nas estruturas.
void adicionar_texto(QuadTree formas, Tabela id_forma, const char *linha) {
    Texto novo_texto = texto_ler(linha);
    QtNo novo_no = insereQt(formas, ponto_criar_com_figura(novo_texto), novo_texto);
    tabela_inserir(id_forma, figura_obter_id(novo_texto), novo_no);
}

// Atualiza a cor das sombras das quadras de acordo com suas densidades.
void atualizar_sombras_quadras(QuadTree quadras, Lista densidades) {
    for_each_lista(i, densidades) {
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

        for_each_lista(j, quadras_contidas) {
            Quadra quad = getInfoQt(quadras, lista_obter_info(j));
            quadra_definir_cor_sombra(quad, cor);
        }
        lista_destruir(quadras_contidas);
    }
}

// Lê um arquivo de descrição fornecido a função e adiciona as figuras descritas em suas linha
// como elementos de uma lista.
void descricao_ler(const char *caminho_descricao, Tabela quadtrees, Tabela listas,
                   Tabela relacoes) {
    LOG_INFO("Lendo descrição\n");
    FILE *arquivo_descricao = fopen(caminho_descricao, "r");
    if (arquivo_descricao == NULL) {
        LOG_ERRO("ERRO: Falha ao ler arquivo de descrição: %s!\n", caminho_descricao);
        return;
    }

    QuadTree formas = tabela_buscar(quadtrees, "formas");
    QuadTree quadras = tabela_buscar(quadtrees, "quadras");
    QuadTree hidrantes = tabela_buscar(quadtrees, "hidrantes");
    QuadTree radios = tabela_buscar(quadtrees, "radios");
    QuadTree semaforos = tabela_buscar(quadtrees, "semaforos");
    QuadTree postos = tabela_buscar(quadtrees, "postos");

    Lista densidades = tabela_buscar(listas, "densidades");

    Tabela cep_quadra = tabela_buscar(relacoes, "cep_quadra");
    Tabela id_hidrante = tabela_buscar(relacoes, "id_hidrante");
    Tabela id_semaforo = tabela_buscar(relacoes, "id_semaforo");
    Tabela id_radio = tabela_buscar(relacoes, "id_radio");
    Tabela id_forma = tabela_buscar(relacoes, "id_forma");

    NumerosMaximos maximo = criar_maximos();
    int formas_criadas = 0;
    int quadras_criadas = 0;
    int semaforos_criados = 0;
    int hidrantes_criados = 0;
    int radios_criadas = 0;
    PropriedadesFiguras propriedades = criar_propriedades();

    char linha[TAMANHO_COMANDO];
    while (fgets(linha, TAMANHO_COMANDO, arquivo_descricao) != NULL) {
        char comando[TAMANHO_COMANDO];
        sscanf(linha, "%s", comando);

        if (strcmp("c", comando) == 0 && formas_criadas <= maximo.formas) {
            adicionar_circulo(formas, id_forma, propriedades.circulos, linha);
            formas_criadas++;
        } else if (strcmp("dd", comando) == 0) {
            adicionar_densidade(densidades, linha);
        } else if (strcmp("r", comando) == 0 && formas_criadas <= maximo.formas) {
            adicionar_retangulo(formas, id_forma, propriedades.retangulos, linha);
            formas_criadas++;
        } else if (strcmp("q", comando) == 0 && quadras_criadas <= maximo.quadras) {
            adicionar_quadra(quadras, cep_quadra, propriedades.quadras, linha);
            quadras_criadas++;
        } else if (strcmp("h", comando) == 0 && hidrantes_criados <= maximo.hidrantes) {
            adicionar_hidrante(hidrantes, id_hidrante, propriedades.hidrantes, linha);
            hidrantes_criados++;
        } else if (strcmp("ps", comando) == 0) {
            adicionar_posto(postos, linha);
        } else if (strcmp("s", comando) == 0 && semaforos_criados <= maximo.semaforos) {
            adicionar_semaforo(semaforos, id_semaforo, propriedades.semaforos, linha);
            semaforos_criados++;
        } else if (strcmp("rb", comando) == 0 && radios_criadas <= maximo.radios) {
            adicionar_radio(radios, id_radio, propriedades.radios, linha);
            radios_criadas++;
        } else if (strcmp("t", comando) == 0 && formas_criadas <= maximo.formas) {
            adicionar_texto(formas, id_forma, linha);
            formas_criadas++;
        } else if (strcmp("cq", comando) == 0) {
            definir_propriedades_quadras(&propriedades.quadras, linha);
        } else if (strcmp("ch", comando) == 0) {
            definir_propriedades_hidrantes(&propriedades.hidrantes, linha);
        } else if (strcmp("cr", comando) == 0) {
            definir_propriedades_radios(&propriedades.radios, linha);
        } else if (strcmp("cs", comando) == 0) {
            definir_propriedades_semaforos(&propriedades.semaforos, linha);
        } else if (strcmp("sw", comando) == 0) {
            definir_propriedades_circulos(&propriedades.circulos, linha);
            definir_propriedades_retangulos(&propriedades.retangulos, linha);
        } else if (strcmp("nx", comando) == 0) {
            definir_max_figuras(&maximo, linha);
            LOG_INFO("Novo valor máximo de formas: %d\n", maximo.formas);
            LOG_INFO("Novo valor máximo de quadras: %d\n", maximo.quadras);
            LOG_INFO("Novo valor máximo de hidrantes: %d\n", maximo.hidrantes);
            LOG_INFO("Novo valor máximo de semaforos: %d\n", maximo.semaforos);
            LOG_INFO("Novo valor máximo de radio-bases: %d\n", maximo.radios);
        }
    }

    atualizar_sombras_quadras(quadras, densidades);

    fclose(arquivo_descricao);
}
