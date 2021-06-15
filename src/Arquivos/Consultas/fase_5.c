#include "fase_5.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Arquivos/svg.h"
#include "../../Estruturas/grafo.h"
#include "../../Estruturas/pilha.h"
#include "../../Estruturas/quadtree.h"
#include "../../Objetos/EquipamentosUrbanos/morador.h"
#include "../../Objetos/EquipamentosUrbanos/quadra.h"
#include "../../Objetos/Formas/circulo.h"
#include "../../Objetos/Formas/linha.h"
#include "../../Objetos/Outros/animacao.h"
#include "../../Objetos/Outros/texto.h"
#include "../../Utils/caminhos.h"
#include "../../Utils/dijkstra.h"
#include "../../Utils/kruskal.h"
#include "../../Utils/logging.h"

bool checar_registrador_valido(int indice) {
    return indice > 0 && indice <= 10;
}

// Retorna o grafo de ciclovias, caso não exista é criado e armazenado em uma tabela para uso
// posterior.
Grafo obter_ciclovia(Tabela grafos) {
    Grafo ciclovias = tabela_buscar(grafos, "ciclovias");
    // Cria a árvore geradora mínima caso ainda não exista.
    if (ciclovias == NULL) {
        Grafo vias = tabela_buscar(grafos, "vias");
        if (vias == NULL) {
            LOG_AVISO("Grafo de vias não foi criado!\n");
            return NULL;
        }
        ciclovias = criar_arvore_geradora_minima(vias);
        if (ciclovias == NULL) {
            return NULL;
        }
        // Salva a árvore geradora mínima para uso posterior.
        tabela_remover(grafos, "ciclovias");
        tabela_inserir(grafos, "ciclovias", ciclovias);
    }
    return ciclovias;
}

// Salva a posição geográfica da residência de um morador em um registrador.
void registrar_posicao_morador(Ponto *registradores, Tabela dados_pessoa, Tabela cep_quadra,
                               Lista formas, const char *linha) {
    int registrador = -1;
    char id_registrador[3], cpf[100];
    sscanf(linha, "@m? %s %s", id_registrador, cpf);
    sscanf(id_registrador, "R%d", &registrador);
    if (!checar_registrador_valido(registrador))
        return;

    Morador morador = tabela_buscar(dados_pessoa, cpf);
    if (morador == NULL)
        return;

    const char *cep = morador_obter_endereco_cep(morador);
    QtNo no = tabela_buscar(cep_quadra, cep);
    if (no == NULL)
        return;
    Quadra quadra = quadtree_obter_info(no);

    double x = 0;
    double y = 0;
    quadra_inicializar_coordenada(&x, &y, 0, morador_obter_altura(morador), quadra,
                                  morador_obter_endereco_face(morador),
                                  morador_obter_endereco_num(morador));

    // Remove o item anterior caso exista.
    if (registradores[registrador] != NULL)
        free(registradores[registrador]);
    registradores[registrador] = ponto_criar(x, y);

    Linha linha_vertical = linha_criar(x, y, x, 0, "black");
    lista_inserir_final(formas, linha_vertical);

    Texto identificador = texto_criar("", x + 1, 0, "black", "none", id_registrador);
    lista_inserir_final(formas, identificador);
}

// Salva a posição de um endereço em um registrador.
void registrar_posicao_endereco(Ponto *registradores, Tabela cep_quadra, Lista formas,
                                const char *linha) {
    int registrador = -1;
    char id_registrador[3], cep[100], face;
    double num;
    sscanf(linha, "@e? %s %s %c %lf", id_registrador, cep, &face, &num);
    sscanf(id_registrador, "R%d", &registrador);
    if (!checar_registrador_valido(registrador))
        return;

    QtNo no = tabela_buscar(cep_quadra, cep);
    if (no == NULL)
        return;

    Quadra quadra = quadtree_obter_info(no);
    double x = 0;
    double y = 0;
    quadra_inicializar_coordenada(&x, &y, 0, 0, quadra, face, num);

    // Remove o item anterior caso exista.
    if (registradores[registrador] != NULL)
        free(registradores[registrador]);
    registradores[registrador] = ponto_criar(x, y);

    Linha linha_vertical = linha_criar(x, y, x, 0, "black");
    lista_inserir_final(formas, linha_vertical);

    Texto identificador = texto_criar("", x + 1, 0, "black", "none", id_registrador);
    lista_inserir_final(formas, identificador);
}

void registrar_equipamento_urbano(Ponto *registradores, Tabela id_hidrante, Tabela id_semaforo,
                                  Tabela id_radio, Tabela id_forma, Lista formas,
                                  const char *linha) {
    int registrador = -1;
    char id_registrador[3], id_equipamento_urbano[100];

    sscanf(linha, "@g? %s %s", id_registrador, id_equipamento_urbano);
    sscanf(id_registrador, "R%d", &registrador);
    if (!checar_registrador_valido(registrador))
        return;

    QtInfo no = tabela_buscar(id_hidrante, id_equipamento_urbano);
    if (no == NULL)
        no = tabela_buscar(id_semaforo, id_equipamento_urbano);
    if (no == NULL)
        no = tabela_buscar(id_radio, id_equipamento_urbano);
    if (no == NULL)
        no = tabela_buscar(id_forma, id_equipamento_urbano);
    if (no == NULL)
        return;
    Figura equipamento_urbano = quadtree_obter_info(no);

    double x = figura_obter_x_centro(equipamento_urbano);
    double y = figura_obter_y_centro(equipamento_urbano);
    if (registradores[registrador] != NULL)
        free(registradores[registrador]);
    registradores[registrador] = ponto_criar(x, y);

    Linha linha_vertical = linha_criar(x, y, x, 0, "black");
    lista_inserir_final(formas, linha_vertical);

    Texto identificador = texto_criar("", x + 1, 0, "black", "none", id_registrador);
    lista_inserir_final(formas, identificador);
}

void registrar_ponto(Ponto *registradores, Lista formas, const char *linha) {
    int registrador = -1;
    char id_registrador[3];
    double x = 0;
    double y = 0;

    sscanf(linha, "@xy %s %lf %lf", id_registrador, &x, &y);
    sscanf(id_registrador, "R%d", &registrador);
    if (!checar_registrador_valido(registrador))
        return;

    if (registradores[registrador] != NULL)
        free(registradores[registrador]);
    registradores[registrador] = ponto_criar(x, y);

    Linha linha_vertical = linha_criar(x, y, x, 0, "black");
    lista_inserir_final(formas, linha_vertical);

    Texto identificador = texto_criar("", x + 1, 0, "black", "none", id_registrador);
    lista_inserir_final(formas, identificador);
}

// Cria um arquivo svg com o nome especificado, o qual contem a representação do grafo. As arestas
// da árvore geradora mínima são destacadas.
void escrever_grafo_svg(const char *caminho_log, Tabela grafos, const char *linha) {
    char sufixo[1024];
    sscanf(linha, "ccv %s", sufixo);

    Grafo vias = tabela_buscar(grafos, "vias");
    if (vias == NULL) {
        LOG_AVISO("Grafo de vias não foi criado!\n");
        return;
    }
    Grafo ciclovias = obter_ciclovia(grafos);
    if (ciclovias == NULL)
        return;

    char *diretorios = extrair_nome_diretorio(caminho_log);
    char *nome_arquivo = alterar_extensao(caminho_log, 3, "-", sufixo, ".svg");
    char *caminho_arquivo = unir_caminhos(diretorios, nome_arquivo);
    printf("Arquivo ccv: %s\n", caminho_arquivo);

    Lista lista_formas = grafo_escrever_svg(vias, ciclovias);
    svg_escrever(caminho_arquivo, 1, lista_formas);

    lista_destruir(lista_formas);
    free(diretorios);
    free(nome_arquivo);
    free(caminho_arquivo);
}

char *calcular_caminho_ciclo_via(Tabela quadtrees, Tabela grafos, Ponto *registradores,
                                 Lista svg_atual, const char *linha, FILE *arquivo_log) {
    char sufixo[1024];
    char cor[20];
    int registrador1 = -1;
    int registrador2 = -1;

    sscanf(linha, "pb? %s R%d R%d %s", sufixo, &registrador1, &registrador2, cor);
    if (!checar_registrador_valido(registrador1) || !checar_registrador_valido(registrador2) ||
        registradores[registrador1] == NULL || registradores[registrador2] == NULL)
        return NULL;

    QuadTree qt_vias = tabela_buscar(quadtrees, "vias");
    Grafo ciclovias = obter_ciclovia(grafos);
    if (ciclovias == NULL)
        return NULL;

    const Ponto ponto_origem = registradores[registrador1];
    const Vertice vertice_origem = quadtree_obter_mais_proximo(qt_vias, ponto_obter_x(ponto_origem),
                                                               ponto_obter_y(ponto_origem));
    if (vertice_origem == NULL) {
        LOG_AVISO("Não foi possível encontrar um vértice próximo ao ponto de origem (%lf,%lf)!\n",
                  ponto_obter_x(ponto_origem), ponto_obter_y(ponto_origem));
        return NULL;
    }

    const Ponto ponto_destino = registradores[registrador2];
    const Vertice vertice_destino = quadtree_obter_mais_proximo(
        qt_vias, ponto_obter_x(ponto_destino), ponto_obter_y(ponto_destino));
    if (vertice_destino == NULL) {
        LOG_AVISO("Não foi possível encontrar um vértice próximo ao ponto de destino (%lf,%lf)!\n",
                  ponto_obter_x(ponto_destino), ponto_obter_y(ponto_destino));
        return NULL;
    }

    Pilha caminho = dijkstra_distancia(ciclovias, vertice_obter_id(vertice_origem),
                                       vertice_obter_id(vertice_destino));
    if (caminho == NULL)
        return NULL;

    fprintf(arquivo_log, "Caminho de R%d a R%d:\n", registrador1, registrador2);
    dijkstra_criar_representacao(caminho, svg_atual, cor, ponto_origem, ponto_destino, arquivo_log);

    // O comando não recebeu um novo sufixo, continuar a utilizar o antigo.
    if (strcmp(sufixo, "-") == 0)
        return NULL;

    // Retorna o novo sufixo.
    char *novo_sufixo = malloc(sizeof *sufixo * 1024);
    novo_sufixo[0] = '\0';
    strcpy(novo_sufixo, sufixo);
    return novo_sufixo;
}
