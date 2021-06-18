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
#include "../../Objetos/Formas/poligono.h"
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

// Retorna o grafo de vias,
Grafo obter_vias(Tabela grafos) {
    Grafo vias = tabela_buscar(grafos, "vias");
    // Cria a árvore geradora mínima caso ainda não exista.
    if (vias == NULL) {
        LOG_AVISO("Grafo de vias não foi criado!\n");
        return NULL;
    }

    return vias;
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
    int status = sscanf(linha, "@e? %s %s Face.%c %lf", id_registrador, cep, &face, &num);
    if (status != 4)
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
    Circulo circ_origem = circulo_criar("", 20, ponto_obter_x(ponto_origem),
                                        ponto_obter_y(ponto_origem), "black", "purple");
    lista_inserir_final(svg_atual, circ_origem);
    const Vertice vertice_origem = quadtree_obter_mais_proximo(qt_vias, ponto_obter_x(ponto_origem),
                                                               ponto_obter_y(ponto_origem));
    if (vertice_origem == NULL) {
        LOG_INFO("Não foi possível encontrar um vértice próximo ao ponto de origem (%lf,%lf)!\n",
                 ponto_obter_x(ponto_origem), ponto_obter_y(ponto_origem));
        return NULL;
    }

    const Ponto ponto_destino = registradores[registrador2];
    Circulo circ_destino = circulo_criar("", 20, ponto_obter_x(ponto_destino),
                                         ponto_obter_y(ponto_destino), "black", "purple");
    lista_inserir_final(svg_atual, circ_destino);
    const Vertice vertice_destino = quadtree_obter_mais_proximo(
        qt_vias, ponto_obter_x(ponto_destino), ponto_obter_y(ponto_destino));
    if (vertice_destino == NULL) {
        LOG_INFO("Não foi possível encontrar um vértice próximo ao ponto de destino (%lf,%lf)!\n",
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
    strncpy(novo_sufixo, sufixo, 1024);
    return novo_sufixo;
}

char *calcular_trajeto_vias(Tabela quadtrees, Tabela grafos, Ponto *registradores, Lista svg_atual,
                            const char *linha, FILE *arquivo_log) {
    char sufixo[1024];
    char cor_curto[20];
    char cor_rapido[20];
    int registrador1 = -1;
    int registrador2 = -1;

    sscanf(linha, "p? %s R%d R%d %s %s", sufixo, &registrador1, &registrador2, cor_curto,
           cor_rapido);
    if (!checar_registrador_valido(registrador1) || !checar_registrador_valido(registrador2) ||
        registradores[registrador1] == NULL || registradores[registrador2] == NULL)
        return NULL;

    QuadTree qt_vias = tabela_buscar(quadtrees, "vias");
    Grafo vias = obter_vias(grafos);

    if (vias == NULL)
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

    Pilha caminho_curto = dijkstra_distancia(vias, vertice_obter_id(vertice_origem),
                                             vertice_obter_id(vertice_destino));
    if (caminho_curto == NULL)
        return NULL;
    Pilha caminho_rapido = dijkstra_velocidade(vias, vertice_obter_id(vertice_origem),
                                               vertice_obter_id(vertice_destino));
    if (caminho_rapido == NULL)
        return NULL;

    fprintf(arquivo_log, "Caminho mais curto de R%d a R%d:\n", registrador1, registrador2);

    Animacao animacao_curto = dijkstra_criar_representacao(
        caminho_curto, svg_atual, cor_curto, ponto_origem, ponto_destino, arquivo_log);

    Animacao animacao_rapido = dijkstra_criar_representacao(
        caminho_rapido, svg_atual, cor_rapido, ponto_origem, ponto_destino, arquivo_log);

    // animação do caminho mais curto
    animacao_definir_margem_x(animacao_curto, 10);
    animacao_definir_margem_y(animacao_curto, 10);
    // descrição textual do caminho mais curto
    descricao_textual(caminho_curto, arquivo_log);

    // animação do caminho mais rápdio
    animacao_definir_margem_x(animacao_rapido, 0);
    animacao_definir_margem_y(animacao_rapido, 0);
    // descrição textual do caminho mais rapido
    descricao_textual(caminho_rapido, arquivo_log);

    // O comando não recebeu um novo sufixo, continuar a utilizar o antigo.
    if (strcmp(sufixo, "-") == 0)
        return NULL;

    // Retorna o novo sufixo.
    char *novo_sufixo = malloc(sizeof *sufixo * 1024);
    novo_sufixo[0] = '\0';
    strcpy(novo_sufixo, sufixo);
    return novo_sufixo;
}

void descricao_textual(Pilha caminho, FILE *arquivo_log) {
    const int num_pontos = pilha_obter_tamanho(caminho);
    char *direcao = malloc(sizeof *direcao * 6);
    char *direcao_aux = malloc(sizeof *direcao * 6);
    char *nome_rua = malloc(sizeof *nome_rua * 100);

    Vertice anterior = pilha_remover(caminho);
    Vertice atual = pilha_remover(caminho);
    descricao_obter_nome_rua(anterior, atual, nome_rua);
    descricao_determinar_direcao(vertice_obter_x(anterior), vertice_obter_y(anterior),
                                 vertice_obter_x(atual), vertice_obter_y(atual), direcao);
    strcpy(direcao_aux, direcao);
    fprintf(arquivo_log, "Iniciando representação textual do caminho mais curto:\n");
    fprintf(arquivo_log, "Siga na direção %s na rua %s ", direcao, nome_rua);
    for (int i = 0; i < num_pontos - 2; i++) {
        anterior = atual;
        atual = pilha_remover(caminho);
        descricao_obter_nome_rua(anterior, atual, nome_rua);
        descricao_determinar_direcao(vertice_obter_x(anterior), vertice_obter_y(anterior),
                                     vertice_obter_x(atual), vertice_obter_y(atual), direcao);

        // se mudou a direção, quer dizer que mudará a rua;
        if (strcmp(direcao, direcao_aux) != 0) {
            fprintf(arquivo_log, "até o cruzamento com a rua %s.", nome_rua);
            fprintf(arquivo_log, " Siga na direção %s na rua %s ", direcao, nome_rua);
            stpcpy(direcao_aux, direcao);
        }
    }
    fprintf(arquivo_log, "Chegou ao destino.\n");
}

void descricao_obter_nome_rua(Vertice anterior, Vertice atual, char **nome_rua) {
    Lista arestas = vertice_obter_arestas(anterior);
    for_each_lista(no, arestas) {
        if (strcmp(aresta_obter_destino(no), vertice_obter_id(atual)) == 0) {
            strcpy(*nome_rua, aresta_obter_nome(no));
        }
    }
}

void descricao_determinar_direcao(double x_anterior, double y_anterior, double x_atual,
                                  double y_atual, char **direcao) {
    if (x_anterior == x_atual) {
        if (y_anterior < y_atual) {
            strcpy(*direcao, "sul");
        }
        if (y_anterior > y_atual) {
            strcpy(*direcao, "norte");
        }
    }
    if (y_anterior == y_atual) {
        if (x_anterior < x_atual) {
            strcpy(*direcao, "leste");
        }
        if (x_anterior > x_atual) {
            strcpy(*direcao, "oeste");
        }
    }
}

char *analisar_vertices_contidos_envoltoria(Grafo vias, QuadTree casos, Lista formas,
                                            FILE *arquivo_log, Ponto *registradores,
                                            Lista svg_atual, const char *linha) {
    // obtém todos os casos
    // 2000 = valor simbolico apenas para a ideia de obter tdos os casos da cidade
    Lista total_casos = quadtree_chaves_dentro_retangulo(casos, 0, 0, 2000, 2000);
    int tamanho = lista_obter_tamanho(total_casos);

    // cria uma lista de figuras para utilizar no graham scan
    Figura *lista_casos = malloc(tamanho * sizeof *lista_casos);
    if (lista_casos == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        return;
    }

    // armazenando todos os casos como figura na lista
    int j = 0;
    for_each_lista(no_contido, total_casos) {
        lista_casos[j++] = quadtree_obter_info(lista_obter_info(no_contido));
    }

    // graham scan retorna uma pilha com os vertices da envoltoria convexa
    Pilha pilha_pontos = graham_scan(tamanho, &lista_casos);

    // pilha utilizada para excluir os vertices do grafo
    Pilha pilha_vertices = graham_scan(tamanho, &lista_casos);
    QuadTree qt_vias_copia = quadtree_criar((funcGetChave *) vertice_obter_id);
    Grafo vias_copia = criar_copia_grafo_vias(vias, qt_vias_copia);
    excluir_vertices_contidos_envoltoria(vias_copia, pilha_vertices);
    char *novo_sufixo = calcular_trajeto_vias_seguras(qt_vias_copia, vias_copia, registradores,
                                                      svg_atual, linha, arquivo_log);

    const int tamanho_pilha = pilha_obter_tamanho(pilha_pontos);

    // Aloca uma matriz para armazenar os pontos encontrados.
    double **pontos = malloc(sizeof *pontos * tamanho_pilha);
    if (pontos == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        return;
    }

    int i = 0;
    // Carrega os pontos encontrados na matriz.
    while (!pilha_esta_vazia(pilha_pontos)) {
        pontos[i] = malloc(sizeof **pontos * 2);
        if (pontos[i] == NULL) {
            LOG_ERRO("Falha ao alocar memória!\n");
            free(pontos);

            return;
        }
        Figura fig = pilha_remover(pilha_pontos);
        pontos[i][0] = figura_obter_x_centro(fig);
        pontos[i][1] = figura_obter_y_centro(fig);
        i++;
    }
    pilha_destruir(pilha_pontos);
    pilha_pontos = NULL;

    Poligono poligono = poligono_criar(pontos, tamanho_pilha, "red", "yellow", 0.4);
    lista_inserir_final(formas, poligono);
    fprintf(arquivo_log, "\nÁrea da envoltória convexa: %lf\n", poligono_calcular_area(poligono));

    quadtree_destruir(qt_vias_copia);
    grafo_destruir(vias_copia);
    return novo_sufixo;
}

Grafo criar_copia_grafo_vias(Grafo grafo, QuadTree qt_vias_copia) {
    Grafo grafo_copia = grafo_criar();
    for (int i = 0; i < grafo_obter_tamanho(grafo); i++) {
        Vertice vertice = grafo_obter_vertice_por_indice(grafo, i);
        grafo_inserir_vertice(grafo, vertice_obter_id(vertice), vertice_obter_x(vertice),
                              vertice_obter_y(vertice));
        quadtree_inserir(qt_vias_copia,
                         ponto_criar(vertice_obter_x(vertice), vertice_obter_y(vertice)), vertice);
        Lista lista_arestas = vertice_obter_arestas(vertice);
        for_each_lista(no, lista_arestas) {
            Aresta aresta_aux = lista_obter_info(no);
            grafo_inserir_aresta(
                grafo_copia, aresta_obter_origem(aresta_aux), aresta_obter_destino(aresta_aux),
                aresta_obter_quadra_esquerda(aresta_aux), aresta_obter_quadra_direita(aresta_aux),
                aresta_obter_comprimento(aresta_aux), aresta_obter_velocidade(aresta_aux),
                aresta_obter_nome(aresta_aux));
        }
    }
    return grafo_copia;
}

void excluir_vertices_contidos_envoltoria(Grafo grafo, Pilha vertices_envoltoria) {
    int tamanho = pilha_obter_tamanho(vertices_envoltoria);
    Ponto *pontos = malloc(sizeof *pontos * tamanho);

    // armazen os pontos em um array
    for (int i = 0; i < tamanho; i++) {
        Figura fig = pilha_remover(vertices_envoltoria);
        pontos[i] = ponto_criar(figura_obter_x_centro(fig), figura_obter_y_centro(fig));
    }

    for (int i = 0; i < grafo_obter_tamanho(grafo); i++) {
        Vertice vertice = grafo_obter_vertice_por_indice(grafo, i);
        if (vertice_contido_envoltoria(vertice, &pontos, tamanho)) {
            // Se encontrou um vertice dentro da envoltoria, remove todas as arestas do vertice e o
            // vertice
            Lista remover_arestas = vertice_obter_arestas(vertice);
            for_each_lista(no, remover_arestas) {
                grafo_remover_aresta(grafo, aresta_obter_origem(lista_obter_info(no)),
                                     aresta_obter_destino(lista_obter_info(no)));
            }
            grafo_remover_vertice(grafo, vertice_obter_id(vertice));
        }
    }
}

bool vertice_contido_envoltoria(Vertice vertice, Ponto *pontos, int tamanho_array) {
    // para checar se um ponto está dentro de um poligono convexo com orientação anti-horaria
    // basta realizar a conta: (yp - y1).(x2 - x1)-(xp - x1).(y2-y1)
    // se resultado > 0 : ponto está a esquerda da linha
    // resultado = 0 : ponto está exatamente na linha
    // resultado < : ponto está a direita da linha
    // se para todas as linhas o ponto está do mesmo lado, então ele está dentro do poligono

    char resultado;
    char resultado_anterior;
    double xp = vertice_obter_x(vertice);
    double yp = vertice_obter_y(vertice);

    double x1 = ponto_obter_x(pontos[0]);
    double y1 = ponto_obter_y(pontos[0]);

    double x2 = ponto_obter_x(pontos[1]);
    double y2 = ponto_obter_y(pontos[1]);

    int resp = (yp - y1) * (x2 - x1) - (xp - x1) * (y2 - y1);
    if (resp < 0) {
        resultado_anterior = 'D';
    } else if (resp == 0) {
        resultado_anterior = 'L';
    } else {
        resultado_anterior = 'E';
    }

    for (int i = 1; i < tamanho_array - 1; i++) {
        x1 = ponto_obter_x(pontos[i]);
        y1 = ponto_obter_y(pontos[i]);

        x2 = ponto_obter_x(pontos[i + 1]);
        y2 = ponto_obter_y(pontos[i + 1]);

        resp = (yp - y1) * (x2 - x1) - (xp - x1) * (y2 - y1);
        if (resp < 0) {
            resultado = 'D';
        } else if (resp == 0) {
            resultado = 'L';
        } else {
            resultado = 'E';
        }

        // se houve alguma aresta do poligono em que o ponto ficou em um lado diferente, ele não
        // está contido pelo poligono
        if (resultado != resultado_anterior) {
            return false;
        }
    }

    return true;
}

char *calcular_trajeto_vias_seguras(QuadTree qt_vias_copia, Grafo vias_copia, Ponto *registradores,
                                    Lista svg_atual, const char *linha, FILE *arquivo_log) {
    char sufixo[1024];
    char cor_curto[20];
    char cor_rapido[20];
    int registrador1 = -1;
    int registrador2 = -1;

    sscanf(linha, "sp? %s R%d R%d %s %s", sufixo, &registrador1, &registrador2, cor_curto,
           cor_rapido);
    if (!checar_registrador_valido(registrador1) || !checar_registrador_valido(registrador2) ||
        registradores[registrador1] == NULL || registradores[registrador2] == NULL)
        return NULL;

    if (vias_copia == NULL)
        return NULL;

    const Ponto ponto_origem = registradores[registrador1];
    const Vertice vertice_origem = quadtree_obter_mais_proximo(
        qt_vias_copia, ponto_obter_x(ponto_origem), ponto_obter_y(ponto_origem));
    if (vertice_origem == NULL) {
        LOG_AVISO("Não foi possível encontrar um vértice próximo ao ponto de origem (%lf,%lf)!\n",
                  ponto_obter_x(ponto_origem), ponto_obter_y(ponto_origem));
        return NULL;
    }

    const Ponto ponto_destino = registradores[registrador2];
    const Vertice vertice_destino = quadtree_obter_mais_proximo(
        qt_vias_copia, ponto_obter_x(ponto_destino), ponto_obter_y(ponto_destino));
    if (vertice_destino == NULL) {
        LOG_AVISO("Não foi possível encontrar um vértice próximo ao ponto de destino (%lf,%lf)!\n",
                  ponto_obter_x(ponto_destino), ponto_obter_y(ponto_destino));
        return NULL;
    }

    Pilha caminho_curto = dijkstra_distancia(vias_copia, vertice_obter_id(vertice_origem),
                                             vertice_obter_id(vertice_destino));
    if (caminho_curto == NULL)
        return NULL;
    Pilha caminho_rapido = dijkstra_velocidade(vias_copia, vertice_obter_id(vertice_origem),
                                               vertice_obter_id(vertice_destino));
    if (caminho_rapido == NULL)
        return NULL;

    fprintf(arquivo_log, "Caminho mais curto de R%d a R%d:\n", registrador1, registrador2);

    Animacao animacao_curto = dijkstra_criar_representacao(
        caminho_curto, svg_atual, cor_curto, ponto_origem, ponto_destino, arquivo_log);

    Animacao animacao_rapido = dijkstra_criar_representacao(
        caminho_rapido, svg_atual, cor_rapido, ponto_origem, ponto_destino, arquivo_log);

    // animação do caminho mais curto
    animacao_definir_margem_x(animacao_curto, 10);
    animacao_definir_margem_y(animacao_curto, 10);
    // descrição textual do caminho mais curto
    descricao_textual(caminho_curto, arquivo_log);

    // animação do caminho mais rápdio
    animacao_definir_margem_x(animacao_rapido, 0);
    animacao_definir_margem_y(animacao_rapido, 0);
    // descrição textual do caminho mais rapido
    descricao_textual(caminho_rapido, arquivo_log);

    // O comando não recebeu um novo sufixo, continuar a utilizar o antigo.
    if (strcmp(sufixo, "-") == 0)
        return NULL;

    // Retorna o novo sufixo.
    char *novo_sufixo = malloc(sizeof *sufixo * 1024);
    novo_sufixo[0] = '\0';
    strcpy(novo_sufixo, sufixo);
    return novo_sufixo;
}
