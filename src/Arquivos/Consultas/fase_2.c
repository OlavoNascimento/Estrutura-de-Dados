#include "fase_2.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../../Estruturas/quadtree.h"
#include "../../Objetos/EquipamentosUrbanos/quadra.h"
#include "../../Objetos/Formas/circulo.h"
#include "../../Objetos/Formas/linha.h"
#include "../../Objetos/Formas/retangulo.h"
#include "../../Objetos/Outros/texto.h"
#include "../../Utils/matematica.h"

void raio_remove_quadras(QuadTree quadras, Tabela cep_quadra, Tabela id_hidrante,
                         Tabela id_semaforo, Tabela id_radio, Lista formas, const char *linha,
                         FILE *arquivo_log) {
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

    QtNo no_id = tabela_buscar(id_hidrante, id);
    if (no_id == NULL)
        no_id = tabela_buscar(id_radio, id);
    if (no_id == NULL)
        no_id = tabela_buscar(id_semaforo, id);
    if (no_id == NULL)
        return;
    Figura figura = quadtree_obter_info(no_id);

    cir_x = figura_obter_x_centro(figura);
    cir_y = figura_obter_y_centro(figura);
    Circulo circulo_de_selecao = circulo_criar("", raio, cir_x, cir_y, "black", "none");

    Lista nos_dentro_circulo = quadtree_nos_dentro_circulo(quadras, cir_x, cir_y, raio);
    for_each_lista(no, nos_dentro_circulo) {
        Figura quadra = quadtree_obter_info(lista_obter_info(no));
        if (!circulo_contem_retangulo(circulo_de_selecao, quadra))
            continue;

        fprintf(arquivo_log, "id %s: %s, equipamento ", figura_obter_tipo(quadra),
                figura_obter_id(quadra));
        figura_escrever_informacoes(figura, arquivo_log);
        fprintf(arquivo_log, "\n");

        if (remover_quadras) {
            quadtree_remover_no(quadras, lista_obter_info(no));
            tabela_remover(cep_quadra, figura_obter_id(quadra));
            figura_destruir(quadra);
        } else {
            quadra_definir_cor_borda(quadra, "olive");
            quadra_definir_cor_preenchimento(quadra, "beige");
            quadra_definir_arredondamento_borda(quadra, 20);
        }
    }
    lista_destruir(nos_dentro_circulo);

    // Desenhar o raio
    circulo_definir_espessura_borda(circulo_de_selecao, 2);
    lista_inserir_final(formas, circulo_de_selecao);

    // Desenhar anel de duas cores
    Circulo primeiro_anel = circulo_criar("", 17, cir_x, cir_y, "blue", "none");
    circulo_definir_espessura_borda(primeiro_anel, 5);
    lista_inserir_final(formas, primeiro_anel);

    Circulo segundo_anel = circulo_criar("", 12, cir_x, cir_y, "yellow", "none");
    circulo_definir_espessura_borda(segundo_anel, 5);
    lista_inserir_final(formas, segundo_anel);
}

void remove_equipamento_urbano(QuadTree quadras, Tabela cep_quadra, QuadTree hidrantes,
                               Tabela id_hidrante, QuadTree semaforos, Tabela id_semaforo,
                               QuadTree radios, Tabela id_radio, Lista formas, const char *linha,
                               FILE *arquivo_log) {
    char id[100];
    sscanf(linha, "del %s\n", id);

    QtNo no = tabela_buscar(cep_quadra, id);
    QuadTree qt = quadras;
    Tabela tabela = cep_quadra;
    if (no == NULL) {
        no = tabela_buscar(id_hidrante, id);
        qt = hidrantes;
        tabela = id_hidrante;
    }
    if (no == NULL) {
        no = tabela_buscar(id_radio, id);
        qt = radios;
        tabela = id_radio;
    }
    if (no == NULL) {
        no = tabela_buscar(id_semaforo, id);
        qt = semaforos;
        tabela = id_semaforo;
    }
    if (no == NULL)
        return;

    Figura figura = quadtree_obter_info(no);
    double centro_x = figura_obter_x_centro(figura);
    double centro_y = figura_obter_y_centro(figura);

    figura_escrever_informacoes(figura, arquivo_log);
    fprintf(arquivo_log, "\n");

    Linha linha_vertical = linha_criar(centro_x, centro_y, centro_x, 0, "black");
    lista_inserir_final(formas, linha_vertical);

    Texto rotulo = texto_criar("", centro_x + 1, 0, "none", "black", id);
    lista_inserir_final(formas, rotulo);

    quadtree_remover_no(qt, no);
    tabela_remover(tabela, figura_obter_id(figura));
    figura_destruir(figura);
}

// Encontra todas as quadras contidas dentro de um círculo e muda a corda da borda e escreve o id no
// arquivo de log.
void circulo_contem_quadras(QuadTree quadras, const char *linha, FILE *arquivo_log) {
    double cir_x, cir_y, raio;
    char cor_borda[20];
    sscanf(linha, "cbq %lf %lf %lf %s", &cir_x, &cir_y, &raio, cor_borda);

    Circulo circulo_de_selecao = circulo_criar("", raio, cir_x, cir_y, "", "");

    Lista nos_contidos = quadtree_nos_dentro_circulo(quadras, cir_x, cir_y, raio);
    for_each_lista(no, nos_contidos) {
        Figura quadra = quadtree_obter_info(lista_obter_info(no));

        if (circulo_contem_retangulo(circulo_de_selecao, quadra)) {
            figura_definir_cor_borda(quadra, cor_borda);
            fprintf(arquivo_log, "cep: %s\n\n", figura_obter_id(quadra));
        }
    }
    circulo_destruir(circulo_de_selecao);
    lista_destruir(nos_contidos);
}

// Encontra um equipamento urbano em uma lista e escreve suas coordenadas e tipo no arquivo de log.
void informacoes_equipamento_urbano(Tabela cep_quadra, Tabela id_hidrante, Tabela id_radio,
                                    Tabela id_semaforo, const char *linha, FILE *arquivo_log) {
    char id[100];
    sscanf(linha, "crd? %s", id);

    QtNo no_id = tabela_buscar(cep_quadra, id);
    if (no_id == NULL)
        no_id = tabela_buscar(id_hidrante, id);
    if (no_id == NULL)
        no_id = tabela_buscar(id_radio, id);
    if (no_id == NULL)
        no_id = tabela_buscar(id_semaforo, id);
    if (no_id == NULL)
        return;

    Figura equipamento = quadtree_obter_info(no_id);
    fprintf(arquivo_log, "tipo: %s, x: %lf, y: %lf\n\n", figura_obter_tipo(equipamento),
            figura_obter_x(equipamento), figura_obter_y(equipamento));
}

// Encontra o total das áreas das quadras contidas dentro de um retângulo
void retangulo_area_total_contida(Lista formas, QuadTree quadras, const char *linha,
                                  FILE *arquivo_log) {
    double x, y, largura, altura;
    sscanf(linha, "car %lf %lf %lf %lf", &x, &y, &largura, &altura);

    Retangulo contorno = retangulo_criar("", largura, altura, x, y, "black", "none");
    lista_inserir_final(formas, contorno);

    Lista nos_contidos = quadtree_nos_dentro_retangulo(quadras, x, y, x + largura, y + altura);

    double area_total = 0;
    for_each_lista(no, nos_contidos) {
        Quadra quadra = quadtree_obter_info(lista_obter_info(no));
        if (!retangulo_contem_retangulo(contorno, (Retangulo) quadra))
            continue;

        double area_quadra = quadra_obter_largura(quadra) * quadra_obter_altura(quadra);
        area_total += area_quadra;

        char string_area[100];
        // Converte o valor da área da figura para string
        snprintf(string_area, 100, "%lf", area_quadra);

        Texto texto_area_quadra =
            texto_criar("", figura_obter_x_centro(quadra), figura_obter_y_centro(quadra) + 4,
                        "none", "black", string_area);
        texto_definir_alinhamento(texto_area_quadra, TEXTO_CENTRO);
        lista_inserir_final(formas, texto_area_quadra);

        fprintf(arquivo_log, "cep: %s, área: %lf\n\n", figura_obter_id(quadra), area_quadra);
    }
    lista_destruir(nos_contidos);

    Linha linha_vertical = linha_criar(x, y, x, 0, "black");
    lista_inserir_final(formas, linha_vertical);

    char texto_area_total[100];
    // Converte o valor total da área para string
    snprintf(texto_area_total, 100, "%lf", area_total);

    Texto area_linha = texto_criar("", x + 1, 0, "none", "black", texto_area_total);
    lista_inserir_final(formas, area_linha);

    fprintf(arquivo_log, "Área total: %lf\n\n", area_total);
}
