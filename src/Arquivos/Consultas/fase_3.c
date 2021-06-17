#include "fase_3.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Estruturas/pilha.h"
#include "../../Estruturas/quadtree.h"
#include "../../Estruturas/tabelahash.h"
#include "../../Objetos/EquipamentosUrbanos/caso.h"
#include "../../Objetos/EquipamentosUrbanos/posto.h"
#include "../../Objetos/EquipamentosUrbanos/quadra.h"
#include "../../Objetos/Formas/circulo.h"
#include "../../Objetos/Formas/linha.h"
#include "../../Objetos/Formas/poligono.h"
#include "../../Objetos/Formas/retangulo.h"
#include "../../Objetos/Outros/densidade.h"
#include "../../Objetos/Outros/texto.h"
#include "../../Ordenacao/quicksort.h"
#include "../../Ordenacao/shellsort.h"
#include "../../Utils/dijkstra.h"
#include "../../Utils/graham_scan.h"
#include "../../Utils/logging.h"
#include "../../Utils/matematica.h"

void adicionar_caso(QuadTree casos, Tabela cep_quadra, const char *linha) {
    char cep[100];
    sscanf(linha, "cv %*d %s %*c %*d", cep);
    QtNo no = tabela_buscar(cep_quadra, cep);
    if (no != NULL) {
        Quadra quadra_buscada = quadtree_obter_info(no);
        Caso novo_caso = caso_ler(linha, quadra_buscada);
        quadtree_inserir(casos, ponto_criar_com_figura(novo_caso), novo_caso);
    }
}

void salvar_info_em_lista(QtInfo info, ExtraInfo lista) {
    lista_inserir_final(lista, info);
}

void postos_mais_proximos(QuadTree postos, QuadTree qt_vias, Grafo vias, Tabela cep_quadra,
                          Lista formas, const char *linha, FILE *arquivo_log) {
    int k;
    int numero;
    char face;
    char cep[100];
    sscanf(linha, "soc %d %s %c %d", &k, cep, &face, &numero);

    QtNo no_quadra = tabela_buscar(cep_quadra, cep);
    if (no_quadra == NULL)
        return;

    // Desenhar o caso azul
    Caso caso = caso_criar(k, quadtree_obter_info(no_quadra), face, numero);
    lista_inserir_final(formas, caso);

    Lista lista_postos = lista_criar(NULL, NULL);
    quadtree_percorrer_largura(postos, salvar_info_em_lista, lista_postos);

    const int tamanho = lista_obter_tamanho(lista_postos);
    Figura *array_postos = malloc(tamanho * sizeof *array_postos);
    if (array_postos == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        lista_destruir(lista_postos);
        return;
    }

    int i = 0;
    for_each_lista(no, lista_postos) {
        array_postos[i++] = lista_obter_info(no);
    }
    lista_destruir(lista_postos);
    lista_postos = NULL;

    shellsort(array_postos, tamanho, tamanho / 2, figura_obter_x(caso), figura_obter_y(caso));

    Ponto ponto_origem = NULL;
    Vertice vertice_origem = NULL;
    if (vias != NULL) {
        ponto_origem = ponto_criar(figura_obter_x_centro(caso), figura_obter_y_centro(caso));
        vertice_origem = quadtree_obter_mais_proximo(qt_vias, ponto_obter_x(ponto_origem),
                                                     ponto_obter_y(ponto_origem));
    } else {
        printf(
            "Um grafo de vias não foi fornecido, portanto o comando soc não poderá criar caminhos "
            "até os postos mais próximos!\n");
    }

    for (int i = 0; i < k && i < tamanho; i++) {
        if (i == 0)
            fprintf(arquivo_log, "Coordenada dos postos:\n\n");
        Figura posto = array_postos[i];
        fprintf(arquivo_log, "x: %lf, y: %lf\n", figura_obter_x(posto), figura_obter_y(posto));

        if (vias == NULL)
            continue;

        const Ponto ponto_destino =
            ponto_criar(figura_obter_x_centro(posto), figura_obter_y_centro(posto));
        const Vertice vertice_destino = quadtree_obter_mais_proximo(
            qt_vias, ponto_obter_x(ponto_destino), ponto_obter_y(ponto_destino));
        if (vertice_destino == NULL) {
            free(ponto_destino);
            continue;
        }

        Pilha caminho = dijkstra_distancia(vias, vertice_obter_id(vertice_origem),
                                           vertice_obter_id(vertice_destino));
        fprintf(arquivo_log, "Caminho do caso (%lf, %lf) ao posto (%lf, %lf):\n",
                figura_obter_x_centro(caso), figura_obter_x_centro(caso),
                figura_obter_x_centro(posto), figura_obter_x_centro(posto));
        dijkstra_criar_representacao(caminho, formas, "green", ponto_origem, ponto_destino,
                                     arquivo_log);
        fprintf(arquivo_log, "\n");

        free(ponto_destino);
    }

    free(ponto_origem);
    free(array_postos);
}

// Utiliza um círculo para definir os casos que devem ser contidos por uma envoltória convexa.
void determinar_regiao_de_incidencia(Lista formas, QuadTree casos, QuadTree postos,
                                     Lista densidades, const char *linha, FILE *arquivo_log) {
    double x, y, raio;
    sscanf(linha, "ci %lf %lf %lf", &x, &y, &raio);

    // Adiciona o círculo a lista de formas.
    Circulo raio_de_selecao = circulo_criar("", raio, x, y, "green", "none");
    circulo_definir_espessura_borda(raio_de_selecao, 4);
    lista_inserir_final(formas, raio_de_selecao);

    Lista nos_contidos = quadtree_nos_dentro_circulo(casos, x, y, raio);
    int total_de_casos = 0;
    bool cabecalho_escrito = false;
    // Filtra a lista de casos, mantendo apenas aqueles que estão totalmente contidos dentro do
    // círculo.
    ListaNo atual = lista_obter_inicio(nos_contidos);
    while (atual != NULL) {
        Caso caso = quadtree_obter_info(lista_obter_info(atual));
        ListaNo proximo = lista_obter_proximo(atual);

        if (circulo_contem_retangulo(raio_de_selecao, (Retangulo) caso)) {
            if (!cabecalho_escrito) {
                fprintf(arquivo_log, "Pontos selecionados pelo círculo: \n");
                cabecalho_escrito = true;
            }
            total_de_casos += caso_obter_numero_de_casos(caso);
            fprintf(arquivo_log, "x: %lf, y: %lf\n", figura_obter_x(caso), figura_obter_y(caso));
        } else {
            lista_remover(nos_contidos, atual);
        }
        atual = proximo;
    }
    if (cabecalho_escrito)
        fprintf(arquivo_log, "\n");

    // Armazena os casos filtrados em um array.
    int tamanho = lista_obter_tamanho(nos_contidos);
    Figura *casos_filtrados = malloc(tamanho * sizeof *casos_filtrados);
    if (casos_filtrados == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        lista_destruir(nos_contidos);
        return;
    }
    int j = 0;
    for_each_lista(no_contido, nos_contidos) {
        casos_filtrados[j++] = quadtree_obter_info(lista_obter_info(no_contido));
    }
    lista_destruir(nos_contidos);
    nos_contidos = NULL;

    // Busca o número de habitantes da densidade correspondente.
    double habitantes = 0;
    for_each_lista(den_no, densidades) {
        Densidade regiao = lista_obter_info(den_no);
        if (densidade_contem_ponto(regiao, x, y)) {
            habitantes = densidade_calcular_habitantes(regiao);
            break;
        }
    }

    double incidencia = 0;
    if (habitantes > 0)
        incidencia = (total_de_casos / habitantes) * 100000;

    char categoria = '\0';
    char *cor_poligono = malloc(sizeof *cor_poligono * 8);
    if (cor_poligono == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        free(casos_filtrados);
        return;
    }
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
    fprintf(arquivo_log, "Total de casos: %d\n", total_de_casos);
    fprintf(arquivo_log, "\nCategoria de incidência: %c\n", categoria);

    // Calcula a envoltória convexa.
    Pilha pilha_pontos_envoltoria = graham_scan(tamanho, &casos_filtrados);
    free(casos_filtrados);
    casos_filtrados = NULL;
    if (pilha_pontos_envoltoria == NULL) {
        free(cor_poligono);
        return;
    }

    // Aloca uma matriz para armazenar os pontos encontrados.
    const int tamanho_pilha = pilha_obter_tamanho(pilha_pontos_envoltoria);
    double **pontos = malloc(sizeof *pontos * tamanho_pilha);
    if (pontos == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        free(cor_poligono);
        return;
    }

    int i = 0;
    // Carrega os pontos encontrados na matriz.
    while (!pilha_esta_vazia(pilha_pontos_envoltoria)) {
        pontos[i] = malloc(sizeof **pontos * 2);
        if (pontos[i] == NULL) {
            LOG_ERRO("Falha ao alocar memória!\n");
            free(pontos);
            free(cor_poligono);
            return;
        }
        Figura fig = pilha_remover(pilha_pontos_envoltoria);
        pontos[i][0] = figura_obter_x_centro(fig);
        pontos[i][1] = figura_obter_y_centro(fig);
        i++;
    }
    pilha_destruir(pilha_pontos_envoltoria);
    pilha_pontos_envoltoria = NULL;

    Poligono poligono = poligono_criar(pontos, tamanho_pilha, "red", cor_poligono, 0.4);
    lista_inserir_final(formas, poligono);
    fprintf(arquivo_log, "\nÁrea da envoltória convexa: %lf\n", poligono_calcular_area(poligono));

    // Adiciona um posto de campanha caso necessário.
    if (categoria == 'E') {
        double x_centroide;
        double y_centroide;
        poligono_inicializar_centroide(poligono, &x_centroide, &y_centroide);
        Posto centroide = posto_criar(x_centroide, y_centroide);
        quadtree_inserir(postos, ponto_criar_com_figura(centroide), centroide);
    }

    free(cor_poligono);
}
