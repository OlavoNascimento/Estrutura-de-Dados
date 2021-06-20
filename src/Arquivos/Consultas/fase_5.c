#include "fase_5.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Arquivos/svg.h"
#include "../../Estruturas/grafo.h"
#include "../../Estruturas/lista.h"
#include "../../Estruturas/pilha.h"
#include "../../Estruturas/quadtree.h"
#include "../../Objetos/EquipamentosUrbanos/caso.h"
#include "../../Objetos/EquipamentosUrbanos/morador.h"
#include "../../Objetos/EquipamentosUrbanos/quadra.h"
#include "../../Objetos/Formas/circulo.h"
#include "../../Objetos/Formas/linha.h"
#include "../../Objetos/Formas/poligono.h"
#include "../../Objetos/Outros/animacao.h"
#include "../../Objetos/Outros/texto.h"
#include "../../Utils/caminhos.h"
#include "../../Utils/dijkstra.h"
#include "../../Utils/graham_scan.h"
#include "../../Utils/kruskal.h"
#include "../../Utils/logging.h"
#include "../../Utils/matematica.h"

// Guarda as informações da face de uma quadra.
struct InfoFace {
    char cep[100];
    char face;
    int num_casos;
};

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
        if (ciclovias == NULL)
            return NULL;
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
    char *nome_arquivo = alterar_extensao(caminho_log, 2, sufixo, ".svg");
    char *caminho_arquivo = unir_caminhos(diretorios, nome_arquivo);
    printf("Arquivo ccv: %s\n", caminho_arquivo);

    Lista lista_formas = grafo_escrever_svg(vias, ciclovias);
    svg_escrever(caminho_arquivo, 1, lista_formas);

    lista_destruir(lista_formas);
    free(diretorios);
    free(nome_arquivo);
    free(caminho_arquivo);
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
    Grafo vias = tabela_buscar(grafos, "vias");
    if (vias == NULL)
        return NULL;

    const Ponto ponto_origem = registradores[registrador1];
    const Vertice vertice_origem = quadtree_obter_mais_proximo(qt_vias, ponto_obter_x(ponto_origem),
                                                               ponto_obter_y(ponto_origem));
    if (vertice_origem == NULL) {
        LOG_INFO("Não foi possível encontrar um vértice próximo ao ponto de origem (%lf,%lf)!\n",
                 ponto_obter_x(ponto_origem), ponto_obter_y(ponto_origem));
        return NULL;
    }

    const Ponto ponto_destino = registradores[registrador2];
    const Vertice vertice_destino = quadtree_obter_mais_proximo(
        qt_vias, ponto_obter_x(ponto_destino), ponto_obter_y(ponto_destino));
    if (vertice_destino == NULL) {
        LOG_INFO("Não foi possível encontrar um vértice próximo ao ponto de destino (%lf,%lf)!\n",
                 ponto_obter_x(ponto_destino), ponto_obter_y(ponto_destino));
        return NULL;
    }

    Pilha caminho_curto = dijkstra_distancia(vias, vertice_obter_id(vertice_origem),
                                             vertice_obter_id(vertice_destino));
    Pilha caminho_rapido = dijkstra_velocidade(vias, vertice_obter_id(vertice_origem),
                                               vertice_obter_id(vertice_destino));

    fprintf(arquivo_log, "Caminho mais curto de R%d a R%d:\n", registrador1, registrador2);
    Animacao animacao_curto = dijkstra_criar_representacao(
        caminho_curto, svg_atual, cor_curto, ponto_origem, ponto_destino, arquivo_log);
    if (animacao_curto != NULL) {
        animacao_definir_margem_x(animacao_curto, 2);
        animacao_definir_margem_y(animacao_curto, 2);
    }

    fprintf(arquivo_log, "\nCaminho mais rápido de R%d a R%d:\n", registrador1, registrador2);
    Animacao animacao_rapido = dijkstra_criar_representacao(
        caminho_rapido, svg_atual, cor_rapido, ponto_origem, ponto_destino, arquivo_log);
    if (animacao_rapido != NULL) {
        animacao_definir_margem_x(animacao_rapido, -4);
        animacao_definir_margem_y(animacao_rapido, -4);
    }

    // O comando não recebeu um novo sufixo, continuar a utilizar o antigo.
    if (strcmp(sufixo, "-") == 0)
        return NULL;

    // Retorna o novo sufixo.
    char *novo_sufixo = malloc(sizeof *sufixo * 1024);
    novo_sufixo[0] = '\0';
    strcpy(novo_sufixo, sufixo);
    return novo_sufixo;
}

// Salva em uma tabela todos os casos que ultrapassam um valor máximo e os valores abaixo do limite
// em uma lista.
void salvar_casos(Caso caso, void *infos[3]) {
    const int *max = infos[0];
    const Tabela cep_caso_acima = infos[1];
    const Lista lista_casos_abaixo = infos[2];
    if (caso_obter_numero_de_casos(caso) > *max)
        tabela_inserir(cep_caso_acima, caso_obter_cep_quadra(caso), caso);
    else
        lista_inserir_final(lista_casos_abaixo, caso);
}

// Caso uma quadra tenha um caso acima da média uma linha é criada na face apropriada e o número de
// casos presentes é salvo para ser escrito posteriormente no arquivo de log.
void interditar_face_quadra(Caso caso, Quadra quadra, const char *cep, Lista formas,
                            Tabela faces_com_casos) {
    const char face = caso_obter_face_quadra(caso);

    double x1 = 0;
    double y1 = 0;
    double x2 = 0;
    double y2 = 0;
    if (face == 'N') {
        x1 = figura_obter_x_inicio(quadra);
        y1 = figura_obter_y_fim(quadra) + 2;
        x2 = figura_obter_x_fim(quadra);
        y2 = y1;
    } else if (face == 'S') {
        x1 = figura_obter_x_inicio(quadra);
        y1 = figura_obter_y_inicio(quadra) - 2;
        x2 = figura_obter_x_fim(quadra);
        y2 = y1;
    } else if (face == 'L') {
        x1 = figura_obter_x_inicio(quadra) - 2;
        y1 = figura_obter_y_inicio(quadra);
        x2 = x1;
        y2 = figura_obter_y_fim(quadra);
    } else if (face == 'O') {
        x1 = figura_obter_x_fim(quadra) + 2;
        y1 = figura_obter_y_inicio(quadra);
        x2 = x1;
        y2 = figura_obter_y_fim(quadra);
    }

    Linha barreira = linha_criar(x1, y1, x2, y2, "red");
    linha_definir_espessura(barreira, 2);
    lista_inserir_final(formas, barreira);

    // Usa o cep da quadra e a face como chaves para a tabela.
    char chave[500];
    chave[0] = '\0';
    snprintf(chave, 500, "%s%c", cep, face);

    struct InfoFace *info = tabela_buscar(faces_com_casos, chave);
    if (info == NULL) {
        // Caso seja o primeiro caso acima do limite máximo na face da quadra, salva seu número de
        // casos na tabela.
        struct InfoFace *info = malloc(sizeof *info);
        if (info == NULL) {
            LOG_ERRO("Falha ao alocar memória!\n");
            return;
        }
        strcpy(info->cep, cep);
        info->face = face;
        info->num_casos = caso_obter_numero_de_casos(caso);
        tabela_inserir(faces_com_casos, chave, info);
    } else {
        // Caso já exista um caso na face o número de casos é somado ao valor atual.
        info->num_casos += caso_obter_numero_de_casos(caso);
    }
}

// Verifica se uma quadra ultrapassa o limite permitido de casos, interditando a rua caso
// necessário.
bool checar_casos_quadra(const char *cep, Tabela cep_casos, Tabela cep_quadra, Lista formas,
                         Tabela faces_com_casos) {
    const Caso caso = tabela_buscar(cep_casos, cep);
    const QtNo no = tabela_buscar(cep_quadra, cep);
    // A face não possui casos acima do limite.
    if (caso == NULL || no == NULL)
        return false;

    const Quadra quadra = quadtree_obter_info(no);
    interditar_face_quadra(caso, quadra, cep, formas, faces_com_casos);
    tabela_remover(cep_casos, cep);
    return true;
}

void interditar_ruas(QuadTree casos, Tabela relacoes, Tabela grafos, Lista formas,
                     const char *linha, FILE *arquivo_log) {
    int *casos_max = malloc(sizeof *casos_max);
    sscanf(linha, "bf %d", casos_max);

    Grafo vias = tabela_buscar(grafos, "vias");
    Tabela cep_quadra = tabela_buscar(relacoes, "cep_quadra");

    Tabela cep_casos_acima = tabela_criar(NULL);
    Lista lista_casos_abaixo = lista_criar(NULL, NULL);

    void *infos[3] = {casos_max, cep_casos_acima, lista_casos_abaixo};
    quadtree_percorrer_largura(casos, (visitaNo *) salvar_casos, infos);
    free(casos_max);
    casos_max = NULL;

    Tabela faces_com_casos = tabela_criar(free);
    Tabela quadras_nao_permitidas = tabela_criar(NULL);

    // Verifica se as quadras ao lado da aresta possuem casos de covid que excedem o valor
    // permitido.
    int num_arestas = 0;
    bool ciclovia_invalido = false;
    Aresta *arestas = grafo_obter_arestas(vias, &num_arestas);
    for (int i = 0; i < num_arestas; i++) {
        const Aresta aresta = arestas[i];
        const char *cep_esquerda = aresta_obter_quadra_esquerda(aresta);
        const char *cep_direita = aresta_obter_quadra_direita(aresta);

        // Checa se a quadra atual já não foi marcada como contaminada.
        bool quadra_proibida = tabela_buscar(quadras_nao_permitidas, cep_esquerda) != NULL ||
                               tabela_buscar(quadras_nao_permitidas, cep_direita) != NULL;

        // Verifica se a quadra a esquerda precisa ser interditada.
        if (checar_casos_quadra(cep_esquerda, cep_casos_acima, cep_quadra, formas,
                                faces_com_casos)) {
            tabela_inserir(quadras_nao_permitidas, cep_esquerda, vias);
            quadra_proibida = true;
        }

        // Verifica se a quadra a direita precisa ser interditada.
        if (checar_casos_quadra(cep_direita, cep_casos_acima, cep_quadra, formas,
                                faces_com_casos)) {
            tabela_inserir(quadras_nao_permitidas, cep_direita, vias);
            quadra_proibida = true;
        }

        if (quadra_proibida) {
            grafo_remover_aresta(vias, aresta_obter_origem(aresta), aresta_obter_destino(aresta));
            ciclovia_invalido = true;
        }
    }
    free(arestas);
    arestas = NULL;
    tabela_destruir(cep_casos_acima);
    cep_casos_acima = NULL;
    tabela_destruir(quadras_nao_permitidas);
    quadras_nao_permitidas = NULL;

    // Caso uma aresta tenha sido removida a ciclovia atual não é mais válida e deve ser recriada.
    if (ciclovia_invalido) {
        Grafo ciclovias = tabela_remover(grafos, "ciclovias");
        grafo_destruir(ciclovias);
    }

    // Soma o valor dos casos que estão abaixo do limite mais estão na mesma face que um caso que
    // está acima do limite.
    for_each_lista(no, lista_casos_abaixo) {
        const Caso caso = lista_obter_info(no);
        // Usa o cep da quadra e a face como chaves para a tabela.
        char chave[500];
        chave[0] = '\0';
        const char *cep = caso_obter_cep_quadra(caso);
        snprintf(chave, 500, "%s%c", cep, caso_obter_face_quadra(caso));

        struct InfoFace *info_face = tabela_buscar(faces_com_casos, chave);
        if (info_face != NULL)
            info_face->num_casos += caso_obter_numero_de_casos(caso);
    }
    lista_destruir(lista_casos_abaixo);
    lista_casos_abaixo = NULL;

    // Escreve os dados das faces interditadas.
    Lista ceps_interditados = tabela_obter_chaves(faces_com_casos);
    for_each_lista(no, ceps_interditados) {
        const char *chave = lista_obter_info(no);
        struct InfoFace *info_face = tabela_buscar(faces_com_casos, chave);
        fprintf(arquivo_log, "Trecho de cep %s e face %c foi interditado por ter %d casos\n",
                info_face->cep, info_face->face, info_face->num_casos);
    }
    fprintf(arquivo_log, "\n");

    lista_destruir(ceps_interditados);
    tabela_destruir(faces_com_casos);
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
        LOG_INFO("Não foi possível encontrar um vértice próximo ao ponto de origem (%lf,%lf)!\n",
                 ponto_obter_x(ponto_origem), ponto_obter_y(ponto_origem));
        return NULL;
    }

    const Ponto ponto_destino = registradores[registrador2];
    const Vertice vertice_destino = quadtree_obter_mais_proximo(
        qt_vias, ponto_obter_x(ponto_destino), ponto_obter_y(ponto_destino));
    if (vertice_destino == NULL) {
        LOG_INFO("Não foi possível encontrar um vértice próximo ao ponto de destino (%lf,%lf)!\n",
                 ponto_obter_x(ponto_destino), ponto_obter_y(ponto_destino));
        return NULL;
    }

    Pilha caminho = dijkstra_distancia(ciclovias, vertice_obter_id(vertice_origem),
                                       vertice_obter_id(vertice_destino));

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

Grafo criar_copia_grafo_vias(Grafo grafo, QuadTree qt_vias_copia) {
    Grafo grafo_copia = grafo_criar();
    for (int i = 0; i < grafo_obter_tamanho(grafo); i++) {
        Vertice vertice = grafo_obter_vertice_por_indice(grafo, i);

        grafo_inserir_vertice(grafo_copia, vertice_obter_id(vertice), vertice_obter_x(vertice),
                              vertice_obter_y(vertice));
        quadtree_inserir(qt_vias_copia,
                         ponto_criar(vertice_obter_x(vertice), vertice_obter_y(vertice)), vertice);

        Lista lista_arestas = vertice_obter_arestas(vertice);
        for_each_lista(no, lista_arestas) {
            Aresta aresta = lista_obter_info(no);
            grafo_inserir_aresta(grafo_copia, aresta_obter_origem(aresta),
                                 aresta_obter_destino(aresta), aresta_obter_quadra_esquerda(aresta),
                                 aresta_obter_quadra_direita(aresta),
                                 aresta_obter_comprimento(aresta), aresta_obter_velocidade(aresta),
                                 aresta_obter_nome(aresta));
        }
    }
    return grafo_copia;
}

bool vertice_contido_envoltoria(Vertice vertice, Ponto *pontos, int tamanho_array) {
    // para checar se um ponto está dentro de um poligono convexo com orientação anti-horaria
    // basta realizar a conta: (yp - y1).(x2 - x1)-(xp - x1).(y2-y1)
    // se resultado > 0 : ponto está a esquerda da linha
    // resultado = 0 : ponto está exatamente na linha
    // resultado < : ponto está a direita da linha
    // se para todas as linhas o ponto está do mesmo lado, então ele está dentro do poligono

    double xp = vertice_obter_x(vertice);
    double yp = vertice_obter_y(vertice);

    for (int i = 0; i < tamanho_array; i++) {
        double x1 = ponto_obter_x(pontos[i]);
        double y1 = ponto_obter_y(pontos[i]);

        double x2 = ponto_obter_x(pontos[i + 1]);
        double y2 = ponto_obter_y(pontos[i + 1]);
        double pv = produto_vetorial(x2 - x1, y2 - y1, xp - x1, yp - y1);

        // Se houve alguma aresta do poligono em que o ponto ficou em um lado diferente, ele não
        // está contido pelo poligono.
        if (pv > 0)
            return false;
    }

    return true;
}

void excluir_vertices_contidos_envoltoria(Grafo grafo, Pilha vertices_envoltoria) {
    int tamanho = pilha_obter_tamanho(vertices_envoltoria);
    Ponto *pontos = malloc(sizeof *pontos * tamanho);

    // Armazena os pontos em um array.
    for (int i = 0; i < tamanho; i++) {
        Figura fig = pilha_remover(vertices_envoltoria);
        pontos[i] = ponto_criar(figura_obter_x_centro(fig), figura_obter_y_centro(fig));
    }

    for (int i = 0; i < grafo_obter_tamanho(grafo); i++) {
        Vertice vertice = grafo_obter_vertice_por_indice(grafo, i);
        if (vertice_contido_envoltoria(vertice, pontos, tamanho)) {
            // Se encontrou um vertice dentro da envoltoria, remove todas as arestas do vertice e o
            // vertice
            Vertice removido = grafo_remover_vertice(grafo, vertice_obter_id(vertice));
            vertice_destruir(removido);
        }
    }
    for (int i = 0; i < tamanho; i++)
        free(pontos[i]);
    free(pontos);
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

    if (qt_vias_copia == NULL || vias_copia == NULL)
        return NULL;

    const Ponto ponto_origem = registradores[registrador1];
    const Vertice vertice_origem = quadtree_obter_mais_proximo(
        qt_vias_copia, ponto_obter_x(ponto_origem), ponto_obter_y(ponto_origem));
    if (vertice_origem == NULL) {
        LOG_INFO("Não foi possível encontrar um vértice próximo ao ponto de origem (%lf,%lf)!\n",
                 ponto_obter_x(ponto_origem), ponto_obter_y(ponto_origem));
        return NULL;
    }

    const Ponto ponto_destino = registradores[registrador2];
    const Vertice vertice_destino = quadtree_obter_mais_proximo(
        qt_vias_copia, ponto_obter_x(ponto_destino), ponto_obter_y(ponto_destino));
    if (vertice_destino == NULL) {
        LOG_INFO("Não foi possível encontrar um vértice próximo ao ponto de destino (%lf,%lf)!\n",
                 ponto_obter_x(ponto_destino), ponto_obter_y(ponto_destino));
        return NULL;
    }

    Pilha caminho_curto = dijkstra_distancia(vias_copia, vertice_obter_id(vertice_origem),
                                             vertice_obter_id(vertice_destino));
    Pilha caminho_rapido = dijkstra_velocidade(vias_copia, vertice_obter_id(vertice_origem),
                                               vertice_obter_id(vertice_destino));

    fprintf(arquivo_log, "Caminho mais curto de R%d a R%d:\n", registrador1, registrador2);
    // Animação do caminho mais curto
    Animacao animacao_curto = dijkstra_criar_representacao(
        caminho_curto, svg_atual, cor_curto, ponto_origem, ponto_destino, arquivo_log);
    if (animacao_curto != NULL) {
        animacao_definir_margem_x(animacao_curto, 2);
        animacao_definir_margem_y(animacao_curto, 2);
    }

    fprintf(arquivo_log, "\nCaminho mais rápido de R%d a R%d:\n", registrador1, registrador2);
    // Animação do caminho mais rápido
    Animacao animacao_rapido = dijkstra_criar_representacao(
        caminho_rapido, svg_atual, cor_rapido, ponto_origem, ponto_destino, arquivo_log);
    if (animacao_rapido != NULL) {
        animacao_definir_margem_x(animacao_rapido, -4);
        animacao_definir_margem_y(animacao_rapido, -4);
    }

    // O comando não recebeu um novo sufixo, continuar a utilizar o antigo.
    if (strcmp(sufixo, "-") == 0)
        return NULL;

    // Retorna o novo sufixo.
    char *novo_sufixo = malloc(sizeof *sufixo * 1024);
    novo_sufixo[0] = '\0';
    strcpy(novo_sufixo, sufixo);
    return novo_sufixo;
}

char *analisar_vertices_contidos_envoltoria(Grafo vias, QuadTree casos, Lista formas,
                                            FILE *arquivo_log, Ponto *registradores,
                                            Lista svg_atual, const char *linha) {
    // obtém todos os casos
    // 2000 = valor simbolico apenas para a ideia de obter todos os casos da cidade
    Lista total_casos = quadtree_nos_dentro_retangulo(casos, 0, 0, 2000, 2000);
    int tamanho = lista_obter_tamanho(total_casos);

    // Cria uma lista de figuras para utilizar no graham scan.
    Figura *lista_casos = malloc(tamanho * sizeof *lista_casos);
    if (lista_casos == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        return NULL;
    }

    // Armazenando todos os casos como figura na lista.
    int j = 0;
    for_each_lista(no_contido, total_casos) {
        lista_casos[j++] = quadtree_obter_info(lista_obter_info(no_contido));
    }
    lista_destruir(total_casos);
    total_casos = NULL;

    // Pilha utilizada para excluir os vertices do grafo.
    Pilha pilha_vertices = graham_scan(tamanho, &lista_casos);
    // Graham scan retorna uma pilha com os vertices da envoltoria convexa.
    Pilha pilha_pontos = graham_scan(tamanho, &lista_casos);
    free(lista_casos);
    lista_casos = NULL;

    QuadTree qt_vias_copia = quadtree_criar((funcGetChave *) vertice_obter_id);
    Grafo vias_copia = criar_copia_grafo_vias(vias, qt_vias_copia);
    excluir_vertices_contidos_envoltoria(vias_copia, pilha_vertices);
    pilha_destruir(pilha_vertices);
    pilha_vertices = NULL;

    char *novo_sufixo = calcular_trajeto_vias_seguras(qt_vias_copia, vias_copia, registradores,
                                                      svg_atual, linha, arquivo_log);

    const int tamanho_pilha = pilha_obter_tamanho(pilha_pontos);
    // Aloca uma matriz para armazenar os pontos encontrados.
    double **pontos = malloc(sizeof *pontos * tamanho_pilha);
    if (pontos == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        return NULL;
    }

    int i = 0;
    // Carrega os pontos encontrados na matriz.
    while (!pilha_esta_vazia(pilha_pontos)) {
        pontos[i] = malloc(sizeof **pontos * 2);
        if (pontos[i] == NULL) {
            LOG_ERRO("Falha ao alocar memória!\n");
            free(pontos);
            return NULL;
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
