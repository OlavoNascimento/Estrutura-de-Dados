#include "consulta.h"

#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Estruturas/lista.h"
#include "../Estruturas/pilha.h"
#include "../Estruturas/quadtree.h"
#include "../Interfaces/figura.h"
#include "../Objetos/EquipamentosUrbanos/caso.h"
#include "../Objetos/EquipamentosUrbanos/posto.h"
#include "../Objetos/EquipamentosUrbanos/quadra.h"
#include "../Objetos/Formas/circulo.h"
#include "../Objetos/Formas/linha.h"
#include "../Objetos/Formas/poligono.h"
#include "../Objetos/Formas/retangulo.h"
#include "../Objetos/Outros/densidade.h"
#include "../Objetos/Outros/texto.h"
#include "../Ordenacao/quicksort.h"
#include "../Ordenacao/shellsort.h"
#include "../Utils/graham_scan.h"
#include "../Utils/logging.h"
#include "../Utils/matematica.h"

// Tamanho maxímo de um comando do arquivo de consulta.
#define LINHA_MAX 300
// Margem entre o retângulo criado pela função envolver_figuras e as figuras que ele envolve.
#define MARGEM_CONTORNO 2
// Tamanho estimado para cada letra de um texto
#define MARGEM_LETRAS_TEXTO 4

Retangulo *criar_delimitacao_figuras(Figura figura1, Figura figura2) {
    // Coordenada x do contorno é a menor coordenada x entre as duas figuras.
    double x =
        min(figura_obter_x_inicio(figura1), figura_obter_x_inicio(figura2)) - MARGEM_CONTORNO;
    // Coordenada y do contorno é a menor coordenada y entre as duas figuras.
    double y =
        min(figura_obter_y_inicio(figura1), figura_obter_y_inicio(figura2)) - MARGEM_CONTORNO;

    // Largura do contorno é a distância entre o x do contorno e a coordenada x onde o fim da
    // figura mais longe se encontra.
    double largura =
        max(figura_obter_x_fim(figura1), figura_obter_x_fim(figura2)) - x + MARGEM_CONTORNO;
    // Altura do contorno é a distância entre o y do contorno e a coordenada y onde o fim da figura
    // mais longe se encontra.
    double altura =
        max(figura_obter_y_fim(figura1), figura_obter_y_fim(figura2)) - y + MARGEM_CONTORNO;

    Retangulo contorno = retangulo_criar("", largura, altura, x, y, "black", "none");
    return contorno;
}

// Executa o comando o? especificado no arquivo de consulta, verificando se duas figuras se
// sobrepõem.
void checar_interseccao(QuadTree formas, const char *linha, FILE *arquivo_log) {
    char id1[100], id2[100];
    sscanf(linha, "o? %s %s", id1, id2);

    QtNo no1 = quadtree_buscar_id(formas, id1);
    QtNo no2 = quadtree_buscar_id(formas, id2);
    if (no1 == NULL || no2 == NULL)
        return;
    Figura fig1 = getInfoQt(no1);
    Figura fig2 = getInfoQt(no2);

    const char *tipo_fig1 = figura_obter_tipo(fig1);
    const char *tipo_fig2 = figura_obter_tipo(fig2);
    bool intersectam = false;
    if (strcmp(tipo_fig1, "círculo") == 0 && strcmp(tipo_fig1, tipo_fig2) == 0) {
        intersectam = circulo_checar_interseccao(fig1, fig2);
    } else if (strcmp(tipo_fig1, "retângulo") == 0 && strcmp(tipo_fig1, tipo_fig2) == 0) {
        intersectam = retangulo_checar_interseccao(fig1, fig2);
    } else if (strcmp(tipo_fig1, "círculo") == 0 && strcmp(tipo_fig2, "retângulo") == 0) {
        intersectam = circulo_intersecta_retangulo(fig1, fig2);
    } else if (strcmp(tipo_fig1, "retângulo") == 0 && strcmp(tipo_fig2, "círculo") == 0) {
        intersectam = circulo_intersecta_retangulo(fig2, fig1);
    } else {
        return;
    }

    Retangulo contorno = criar_delimitacao_figuras(fig1, fig2);

    if (intersectam) {
        // Adiciona uma mensagem de sobreposição caso as figuras se intersectem.
        Texto aviso =
            texto_criar("", figura_obter_x_centro(contorno), figura_obter_y_centro(contorno) + 4,
                        "None", "Black", "sobrepoe", true);
        insereQt(formas, ponto_criar_com_figura(aviso), aviso);
    } else {
        // Adiciona traços a borda do retângulo de contorno caso as figuras não se intersectem.
        retangulo_definir_borda_tracejada(contorno, true);
    }
    insereQt(formas, ponto_criar_com_figura(contorno), contorno);

    fprintf(arquivo_log, "o? %s %s\n", id1, id2);
    fprintf(arquivo_log, "%s: %s %s: %s %s\n\n", id1, tipo_fig1, id2, tipo_fig2,
            intersectam ? "SIM" : "NAO");
}

// Cria um círculo com as coordenadas especificadas e com cores que dependem de um valor booleano.
Circulo criar_ponto(double ponto_x, double ponto_y, bool interno) {
    Circulo ponto = circulo_criar("", 1, ponto_x, ponto_y, interno ? "blue" : "magenta",
                                  interno ? "blue" : "magenta");
    return ponto;
}

// Executa o comando i? especificado no arquivo de consulta, verificando se um ponto é interno a uma
// figura, conecta este ponto e a figura utilizando uma linha.
void checar_ponto_interno(QuadTree formas, const char *linha, FILE *arquivo_log) {
    char id[100];
    double ponto_x = 0, ponto_y = 0;
    sscanf(linha, "i? %s %lf %lf", id, &ponto_x, &ponto_y);

    QtNo no = quadtree_buscar_id(formas, id);
    if (no == NULL)
        return;
    Figura figura = getInfoQt(no);

    const char *tipo_figura = figura_obter_tipo(figura);
    bool interno = false;
    if (strcmp(tipo_figura, "círculo") == 0) {
        interno = circulo_checar_ponto_interno(figura, ponto_x, ponto_y);
    } else if (strcmp(tipo_figura, "retângulo") == 0) {
        interno = retangulo_checar_ponto_interno(figura, ponto_x, ponto_y);
    } else {
        return;
    }

    Circulo ponto = criar_ponto(ponto_x, ponto_y, interno);
    insereQt(formas, ponto_criar_com_figura(ponto), ponto);

    Linha ligacao =
        linha_criar(figura_obter_x_centro(ponto), figura_obter_y_centro(ponto),
                    figura_obter_x_centro(figura), figura_obter_y_centro(figura),
                    figura_obter_cor_borda(ponto), figura_obter_cor_preenchimento(ponto), false);
    insereQt(formas, ponto_criar_com_figura(ligacao), ligacao);

    fprintf(arquivo_log, "i? %s %lf %lf\n", id, ponto_x, ponto_y);
    fprintf(arquivo_log, "%s: %s %s\n\n", id, figura_obter_tipo(figura),
            interno ? "INTERNO" : "NAO INTERNO");
}

// Executa o comando pnt especificado no arquivo de consulta, alterando a cor de preenchimento e
// borda da figura com id igual ao id que segue o comando pnt.
void alterar_cor(QuadTree formas, const char *linha, FILE *arquivo_log) {
    char id[100], nova_corb[20], nova_corp[20];
    sscanf(linha, "pnt %s %s %s", id, nova_corb, nova_corp);

    QtNo no = quadtree_buscar_id(formas, id);
    if (no == NULL)
        return;
    Figura fig = getInfoQt(no);

    fprintf(arquivo_log, "pnt %s %s %s\n", id, nova_corb, nova_corp);
    fprintf(arquivo_log, "x: %lf, y: %lf\n\n", figura_obter_x(fig), figura_obter_y(fig));

    figura_definir_cor_borda(fig, nova_corb);
    figura_definir_cor_preenchimento(fig, nova_corp);
}

// Executa o comando pnt* especificado no arquivo de consulta, alterando a cor de preenchimento e
// borda de todas as figuras entre id_inicial e id_final (inclusive).
void alterar_cores(QuadTree formas, const char *linha, FILE *arquivo_log) {
    char id_inicial[100], id_final[100], nova_corb[20], nova_corp[20];
    sscanf(linha, "pnt* %s %s %s %s", id_inicial, id_final, nova_corb, nova_corp);

    Lista lista_formas = quadtree_nos_para_lista(formas);
    ListaNo atual = lista_obter_primeiro(lista_formas);

    // Itera até o nó inicial.
    while (atual != NULL) {
        Figura fig = getInfoQt(lista_obter_info(atual));
        const char *id_atual = figura_obter_id(fig);
        if (strcmp(id_atual, id_inicial) == 0)
            break;

        atual = lista_obter_proximo(atual);
    }

    // Itera até o nó final.
    while (atual != NULL) {
        Figura fig = getInfoQt(lista_obter_info(atual));
        const char *id_atual = figura_obter_id(fig);

        fprintf(arquivo_log, "pnt* %s %s %s %s\n", id_inicial, id_final, nova_corb, nova_corp);
        fprintf(arquivo_log, "x: %lf, y: %lf\n\n", figura_obter_x(fig), figura_obter_y(fig));

        figura_definir_cor_borda(fig, nova_corb);
        figura_definir_cor_preenchimento(fig, nova_corp);
        if (strcmp(id_atual, id_final) == 0)
            break;
        atual = lista_obter_proximo(atual);
    }
    lista_destruir(lista_formas);
}

// Executa o comando delf especificado no arquivo de consulta, removendo a figura com id igual ao id
// que segue o comando delf.
void remover_elemento(QuadTree formas, const char *linha, FILE *arquivo_log) {
    char id[100];
    sscanf(linha, "delf %s", id);
    QtNo no = quadtree_buscar_id(formas, id);
    if (no == NULL)
        return;
    Figura fig = getInfoQt(no);

    fprintf(arquivo_log, "delf %s\n", id);
    figura_escrever_informacoes(fig, arquivo_log);
    fprintf(arquivo_log, "\n");
    removeNoQt(formas, no);
    figura_destruir(fig);
}

// Executa o comando delf* especificado pelo arquivo de consulta, removendo todas as figuras que se
// encontrem entre id_inicial e id_final (inclusive).
void remover_elementos(QuadTree formas, const char *linha, FILE *arquivo_log) {
    char id_inicial[100], id_final[100];
    sscanf(linha, "delf* %s %s", id_inicial, id_final);

    Lista lista_formas = quadtree_nos_para_lista(formas);

    ListaNo atual = lista_obter_primeiro(lista_formas);

    // Itera até o nó inicial.
    while (atual != NULL) {
        Figura fig = getInfoQt(lista_obter_info(atual));
        const char *id_atual = figura_obter_id(fig);
        if (strcmp(id_atual, id_inicial) == 0)
            break;
        atual = lista_obter_proximo(atual);
    }

    // Itera até o nó final.
    while (atual != NULL) {
        Figura fig = getInfoQt(lista_obter_info(atual));
        const char *id_atual = figura_obter_id(fig);

        fprintf(arquivo_log, "delf* %s %s\n", id_inicial, id_final);
        figura_escrever_informacoes(fig, arquivo_log);
        fprintf(arquivo_log, "\n");

        ListaNo proximo = lista_obter_proximo(atual);
        if (strcmp(id_atual, id_final) == 0)
            proximo = NULL;

        removeNoQt(formas, lista_obter_info(atual));
        figura_destruir(fig);
        atual = proximo;
    }
    lista_destruir(lista_formas);
}

void raio_remove_quadras(QuadTree quadras, QuadTree hidrantes, QuadTree semaforos, QuadTree radios,
                         QuadTree formas, const char *linha, FILE *arquivo_log) {
    double raio;
    double cir_x, cir_y;
    char id[100];
    char c;
    bool remover_quadras = false;

    sscanf(linha, "dq %c", &c);
    if (c == '#') {
        sscanf(linha, "%*s %*c %s %lf ", id, &raio);
    } else {
        sscanf(linha, "%*s %s %lf ", id, &raio);
        remover_quadras = true;
    }

    QtNo no_id = quadtree_buscar_id(hidrantes, id);
    if (no_id == NULL)
        no_id = quadtree_buscar_id(radios, id);
    if (no_id == NULL)
        no_id = quadtree_buscar_id(semaforos, id);
    if (no_id == NULL)
        return;
    Figura figura = getInfoQt(no_id);

    cir_x = figura_obter_x_centro(figura);
    cir_y = figura_obter_y_centro(figura);
    Circulo circulo_de_selecao = circulo_criar("", raio, cir_x, cir_y, "", "");

    Lista nos_dentro_circulo = nosDentroCirculoQt(quadras, cir_x, cir_y, raio);
    ListaNo atual = lista_obter_primeiro(nos_dentro_circulo);
    ListaNo proximo = NULL;
    while (atual != NULL) {
        proximo = lista_obter_proximo(atual);
        Figura quadra = getInfoQt(lista_obter_info(atual));

        bool contido = circulo_contem_retangulo(circulo_de_selecao, quadra);
        if (contido) {
            fprintf(arquivo_log, "id %s: %s, equipamento ", figura_obter_tipo(quadra),
                    figura_obter_id(quadra));
            figura_escrever_informacoes(figura, arquivo_log);
            fprintf(arquivo_log, "\n");
            if (remover_quadras) {
                removeNoQt(quadras, lista_obter_info(atual));
                figura_destruir(quadra);
            } else {
                figura_definir_cor_borda(quadra, "olive");
                figura_definir_cor_preenchimento(quadra, "beige");
                quadra_definir_arredondamento_borda(quadra, 20);
            }
        }
        atual = proximo;
    }
    circulo_destruir(circulo_de_selecao);
    lista_destruir(nos_dentro_circulo);

    // Desenhar o raio
    Circulo desenho_raio = circulo_criar("", raio, cir_x, cir_y, "black", "none");
    circulo_definir_espessura_borda(desenho_raio, "2px");
    insereQt(formas, ponto_criar_com_figura(desenho_raio), desenho_raio);

    // Desenhar anel de duas cores
    Circulo primeiro_anel = circulo_criar("", 17, cir_x, cir_y, "blue", "none");
    circulo_definir_espessura_borda(primeiro_anel, "5px");
    insereQt(formas, ponto_criar_com_figura(primeiro_anel), primeiro_anel);

    Circulo segundo_anel = circulo_criar("", 12, cir_x, cir_y, "yellow", "none");
    circulo_definir_espessura_borda(segundo_anel, "5px");
    insereQt(formas, ponto_criar_com_figura(segundo_anel), segundo_anel);
}

void remove_equipamento_urbano(const char *linha, QuadTree *quadras, QuadTree *hidrantes,
                               QuadTree *semaforos, QuadTree *radios, QuadTree *formas,
                               FILE *arquivo_log) {
    char id[100];
    sscanf(linha, "del %s\n", id);

    QtNo no = quadtree_buscar_id(quadras, id);
    QuadTree qt = quadras;
    if (no == NULL) {
        no = quadtree_buscar_id(hidrantes, id);
        qt = hidrantes;
    }
    if (no == NULL) {
        no = quadtree_buscar_id(radios, id);
        qt = radios;
    }
    if (no == NULL) {
        no = quadtree_buscar_id(semaforos, id);
        qt = semaforos;
    }
    if (no == NULL)
        return;

    Figura figura = getInfoQt(no);
    double centro_x = figura_obter_x_centro(figura);
    double centro_y = figura_obter_y_centro(figura);

    figura_escrever_informacoes(figura, arquivo_log);
    fprintf(arquivo_log, "\n");

    Linha linha_vertical = linha_criar(centro_x, centro_y, centro_x, 0, "black", "black", false);
    insereQt(formas, ponto_criar_com_figura(linha_vertical), linha_vertical);

    Texto rotulo = texto_criar("", centro_x + 1, 0, "none", "black", id, false);
    insereQt(formas, ponto_criar_com_figura(rotulo), rotulo);

    removeNoQt(qt, no);
    figura_destruir(figura);
}

// Encontra todas as quadras contidas dentro de um círculo e muda a corda da borda e escreve o id no
// arquivo de log.
void circulo_contem_quadras(QuadTree *quadras, const char *linha, FILE *arquivo_log) {
    double cir_x, cir_y, raio;
    char cor_borda[20];
    sscanf(linha, "cbq %lf %lf %lf %s", &cir_x, &cir_y, &raio, cor_borda);

    Circulo circulo_de_selecao = circulo_criar("", raio, cir_x, cir_y, "", "");

    Lista nos_contidos = nosDentroCirculoQt(quadras, cir_x, cir_y, raio);
    ListaNo atual = lista_obter_primeiro(nos_contidos);
    while (atual != NULL) {
        Figura quadra = getInfoQt(lista_obter_info(atual));

        if (circulo_contem_retangulo(circulo_de_selecao, quadra)) {
            figura_definir_cor_borda(quadra, cor_borda);
            fprintf(arquivo_log, "cep: %s\n\n", figura_obter_id(quadra));
        }

        atual = lista_obter_proximo(atual);
    }
    circulo_destruir(circulo_de_selecao);
    lista_destruir(nos_contidos);
}

// Encontra um equipamento urbano em uma lista e escreve suas coordenadas e tipo no arquivo de log.
void informacoes_equipamento_urbano(QuadTree quadras, QuadTree hidrantes, QuadTree radios,
                                    QuadTree semaforos, const char *linha, FILE *arquivo_log) {
    char id[100];
    sscanf(linha, "crd? %s", id);

    QtNo no_id = quadtree_buscar_id(quadras, id);
    if (no_id == NULL)
        no_id = quadtree_buscar_id(hidrantes, id);
    if (no_id == NULL)
        no_id = quadtree_buscar_id(radios, id);
    if (no_id == NULL)
        no_id = quadtree_buscar_id(semaforos, id);
    if (no_id == NULL)
        return;

    Figura equipamento = getInfoQt(no_id);
    fprintf(arquivo_log, "tipo: %s, x: %lf, y: %lf\n\n", figura_obter_tipo(equipamento),
            figura_obter_x(equipamento), figura_obter_y(equipamento));
}

// Encontra o total das áreas das quadras contidas dentro de um retângulo
void retangulo_area_total_contida(QuadTree formas, QuadTree quadras, const char *linha,
                                  FILE *arquivo_log) {
    double x, y, largura, altura;
    sscanf(linha, "car %lf %lf %lf %lf", &x, &y, &largura, &altura);

    Lista nos_contidos = nosDentroRetanguloQt(quadras, x, y, x + largura, y + altura);
    if (lista_obter_tamanho(nos_contidos) == 0) {
        lista_destruir(nos_contidos);
        return;
    }

    Retangulo contorno = retangulo_criar("", largura, altura, x, y, "black", "none");
    insereQt(formas, ponto_criar_com_figura(contorno), contorno);

    double area_total = 0;
    ListaNo atual = lista_obter_primeiro(nos_contidos);
    while (atual != NULL) {
        Figura figura = getInfoQt(lista_obter_info(atual));
        double figura_x_inicio = figura_obter_x_inicio(figura);
        double figura_y_inicio = figura_obter_y_inicio(figura);
        double figura_x_fim = figura_obter_x_fim(figura);
        double figura_y_fim = figura_obter_y_fim(figura);

        if (figura_x_inicio >= x && figura_y_inicio >= y && figura_x_fim <= x + largura &&
            figura_y_fim <= y + altura) {
            double area_figura =
                (figura_x_fim - figura_x_inicio) * (figura_y_fim - figura_y_inicio);
            area_total += area_figura;

            char texto_area_figura[100];
            // Converte o valor da área da figura para string
            snprintf(texto_area_figura, 100, "%lf", area_figura);

            Texto area_quadra =
                texto_criar("", figura_obter_x_centro(figura), figura_obter_y_centro(figura) + 4,
                            "none", "black", texto_area_figura, true);
            insereQt(formas, ponto_criar_com_figura(area_quadra), area_quadra);

            fprintf(arquivo_log, "cep: %s, área: %lf\n\n", figura_obter_id(figura), area_figura);
        }
        atual = lista_obter_proximo(atual);
    }
    lista_destruir(nos_contidos);

    Linha linha_vertical = linha_criar(x, y, x, 0, "black", "black", false);
    insereQt(formas, ponto_criar_com_figura(linha_vertical), linha_vertical);

    char texto_area_total[100];
    // Converte o valor total da área para string
    snprintf(texto_area_total, 100, "%lf", area_total);

    Texto area_linha = texto_criar("", x + 1, 0, "none", "black", texto_area_total, false);
    insereQt(formas, ponto_criar_com_figura(area_linha), area_linha);

    fprintf(arquivo_log, "Área total: %lf\n\n", area_total);
}

void postos_mais_proximos(QuadTree postos, QuadTree quadras, QuadTree formas, const char *linha,
                          FILE *arquivo_log) {
    int k;
    int numero;
    char face;
    char cep[100];
    sscanf(linha, "soc %d %s %c %d", &k, cep, &face, &numero);

    QtNo no_quadra = quadtree_buscar_id(quadras, cep);
    if (no_quadra == NULL)
        return;

    // Desenhar o caso azul
    Caso caso = caso_criar(k, getInfoQt(no_quadra), face, numero, "white", "blue");
    insereQt(formas, ponto_criar_com_figura(caso), caso);

    Lista lista_postos = quadtree_para_lista(postos);
    if (lista_obter_tamanho(lista_postos) == 0) {
        LOG_INFO("Nenhum posto encontrado!\n");
        lista_destruir(lista_postos);
        return;
    }
    shellsort(lista_postos, lista_obter_tamanho(lista_postos) / 2, figura_obter_x(caso),
              figura_obter_y(caso));

    ListaNo i = lista_obter_primeiro(lista_postos);
    for (int j = 0; j < k; j++) {
        if (j == 0)
            fprintf(arquivo_log, "Coordenada dos postos: \n");
        Figura posto = lista_obter_info(i);
        Linha linha_posto = linha_criar(figura_obter_x_centro(caso), figura_obter_y_centro(caso),
                                        figura_obter_x_centro(posto), figura_obter_y_centro(posto),
                                        "black", "black", true);
        insereQt(formas, ponto_criar_com_figura(linha_posto), linha_posto);

        fprintf(arquivo_log, "x: %lf, y: %lf\n\n", figura_obter_x(posto), figura_obter_y(posto));

        i = lista_obter_proximo(i);
        if (i == NULL)
            break;
    }
    lista_destruir(lista_postos);
}

// Utiliza um círculo para definir os casos que devem ser contidos por uma envoltória convexa.
void determinar_regiao_de_incidencia(QuadTree formas, QuadTree densidades, QuadTree casos,
                                     QuadTree postos, const char *linha, FILE *arquivo_log) {
    double x, y, raio;
    sscanf(linha, "ci %lf %lf %lf", &x, &y, &raio);

    // Adiciona o círculo a lista de formas.
    Circulo raio_de_selecao = circulo_criar("", raio, x, y, "green", "none");
    circulo_definir_espessura_borda(raio_de_selecao, "4px");
    insereQt(formas, ponto_criar_com_figura(raio_de_selecao), raio_de_selecao);

    Lista nos_contidos = nosDentroCirculoQt(casos, x, y, raio);
    Lista casos_filtrados = lista_criar(NULL, NULL);
    int total_de_casos = 0;
    // Filtra a lista de casos, mantendo apenas aqueles que estão totalmente contidos dentro do
    // círculo.
    if (lista_obter_tamanho(nos_contidos) > 0)
        fprintf(arquivo_log, "Pontos selecionados pelo círculo: \n");
    for (ListaNo i = lista_obter_primeiro(nos_contidos); i != NULL; i = lista_obter_proximo(i)) {
        // Distância entre o círculo e o ponto x mais longe do círculo
        Figura caso = getInfoQt(lista_obter_info(i));
        double dx = max(x - figura_obter_x_centro(caso), figura_obter_x_centro(caso) - x);
        // Distância entre o círculo e o ponto y mais longe do círculo
        double dy = max(y - figura_obter_y_centro(caso), figura_obter_y_centro(caso) - y);
        if (dx * dx + dy * dy < raio * raio) {
            lista_inserir_final(casos_filtrados, caso);
            total_de_casos += caso_obter_numero_de_casos(caso);
            fprintf(arquivo_log, "x: %lf, y: %lf\n", figura_obter_x(caso), figura_obter_y(caso));
        }
    }
    if (lista_obter_tamanho(nos_contidos) > 0)
        fprintf(arquivo_log, "\n");
    lista_destruir(nos_contidos);

    Lista lista_densidades = quadtree_para_lista(densidades);
    double habitantes = densidade_buscar_habitantes_ponto(lista_densidades, x, y);
    lista_destruir(lista_densidades);

    double incidencia = (total_de_casos / habitantes) * 100000;
    char categoria = '\0';
    char *cor_poligono = malloc(sizeof *cor_poligono * 8);
    cor_poligono[0] = '\0';

    // Define a categoria e cor do polígono baseado na incidência.
    if (incidencia < 0.1) {
        categoria = 'A';
        strcpy(cor_poligono, "#00FFFF");
    } else if (incidencia < 5) {
        categoria = 'B';
        strcpy(cor_poligono, "#008080");
    } else if (incidencia < 10) {
        categoria = 'C';
        strcpy(cor_poligono, "#FFFF00");
    } else if (incidencia < 20) {
        categoria = 'D';
        strcpy(cor_poligono, "#FF0000");
    } else {
        categoria = 'E';
        strcpy(cor_poligono, "#800080");
    }
    // Escreve as informações no arquivo de log
    fprintf(arquivo_log, "Total de casos: %d\n", total_de_casos);
    fprintf(arquivo_log, "\nCategoria de incidência: %c\n", categoria);

    // Calcula a envoltória convexa.
    Pilha pilha_pontos_envoltoria = graham_scan(casos_filtrados);
    if (pilha_pontos_envoltoria == NULL) {
        lista_destruir(casos_filtrados);
        free(cor_poligono);
        return;
    }

    // Aloca uma matriz para armazenar os pontos encontrados.
    double **pontos = malloc(pilha_obter_tamanho(pilha_pontos_envoltoria) * sizeof(double *));
    for (int i = 0; i < pilha_obter_tamanho(pilha_pontos_envoltoria); i++)
        pontos[i] = malloc(2 * sizeof(*pontos[i]));

    int i = 0;
    // Carrega os pontos encontrados na matriz.
    while (!pilha_esta_vazia(pilha_pontos_envoltoria)) {
        Figura fig = lista_obter_info(pilha_remover(pilha_pontos_envoltoria));
        pontos[i][0] = figura_obter_x_centro(fig);
        pontos[i][1] = figura_obter_y_centro(fig);
        i++;
    }

    // Cria o polígono com os pontos encontrados e adiciona a lista de formas.
    Poligono poligono = poligono_criar(pontos, i, "red", cor_poligono, 0.4);
    insereQt(formas, ponto_criar_com_figura(poligono), poligono);
    // Escreve a área do polígono no arquivo de log.
    fprintf(arquivo_log, "\nÁrea da envoltória convexa: %lf\n", poligono_calcular_area(poligono));

    // Adiciona um posto de campanha caso necessário.
    if (categoria == 'E') {
        double x_centroide = poligono_obter_x_centro(poligono);
        double y_centroide = poligono_obter_y_centro(poligono);
        Posto centroide = posto_criar(x_centroide, y_centroide);
        insereQt(postos, ponto_criar_com_figura(centroide), centroide);
    }

    free(cor_poligono);
    lista_destruir(casos_filtrados);
    pilha_destruir(pilha_pontos_envoltoria);
}

// Ler o arquivo de consulta localizado no caminho fornecido a função e itera por todas as suas
// linhas, executando funções correspondentes aos comandos.
void consulta_ler(const char *caminho_qry, const char *caminho_log, QuadTree formas,
                  QuadTree quadras, QuadTree hidrantes, QuadTree radios, QuadTree semaforos,
                  QuadTree postos, QuadTree densidades, QuadTree casos) {
    FILE *arquivo_consulta = fopen(caminho_qry, "r");
    if (arquivo_consulta == NULL) {
        fprintf(stderr, "ERRO: Falha ao ler arquivo de consulta: %s!\n", caminho_qry);
        return;
    }
    FILE *arquivo_log = fopen(caminho_log, "w");
    if (arquivo_log == NULL) {
        fprintf(stderr, "ERRO: Falha ao criar arquivo de log: %s!\n", caminho_log);
        return;
    }

    char linha[LINHA_MAX];
    while (fgets(linha, LINHA_MAX, arquivo_consulta) != NULL) {
        char comando[LINHA_MAX];
        sscanf(linha, "%s", comando);

        if (strcmp("o?", comando) == 0) {
            checar_interseccao(formas, linha, arquivo_log);
        } else if (strcmp("i?", comando) == 0) {
            checar_ponto_interno(formas, linha, arquivo_log);
        } else if (strcmp("pnt", comando) == 0) {
            alterar_cor(formas, linha, arquivo_log);
        } else if (strcmp("pnt*", comando) == 0) {
            alterar_cores(formas, linha, arquivo_log);
        } else if (strcmp("delf", comando) == 0) {
            remover_elemento(formas, linha, arquivo_log);
        } else if (strcmp("delf*", comando) == 0) {
            remover_elementos(formas, linha, arquivo_log);
        } else if (strcmp("dq", comando) == 0) {
            raio_remove_quadras(quadras, hidrantes, semaforos, radios, formas, linha, arquivo_log);
        } else if (strcmp("del", comando) == 0) {
            remove_equipamento_urbano(linha, quadras, hidrantes, semaforos, radios, formas,
                                      arquivo_log);
        } else if (strcmp("cbq", comando) == 0) {
            circulo_contem_quadras(quadras, linha, arquivo_log);
        } else if (strcmp("crd?", comando) == 0) {
            informacoes_equipamento_urbano(quadras, hidrantes, radios, semaforos, linha,
                                           arquivo_log);
        } else if (strcmp("car", comando) == 0) {
            retangulo_area_total_contida(formas, quadras, linha, arquivo_log);
        } else if (strcmp("cv", comando) == 0) {
            char cep[100];
            sscanf(linha, "cv %*d %s %*c %*d", cep);
            QtNo no = quadtree_buscar_id(quadras, cep);
            if (no != NULL) {
                Figura quadra_buscada = getInfoQt(no);
                Caso novo_caso = caso_ler(linha, quadra_buscada);
                insereQt(casos, ponto_criar_com_figura(novo_caso), novo_caso);
            }
        } else if (strcmp("soc", comando) == 0) {
            postos_mais_proximos(postos, quadras, formas, linha, arquivo_log);
        } else if (strcmp("ci", comando) == 0) {
            determinar_regiao_de_incidencia(formas, densidades, casos, postos, linha, arquivo_log);
        }
    }

    fclose(arquivo_log);
    fclose(arquivo_consulta);
}
