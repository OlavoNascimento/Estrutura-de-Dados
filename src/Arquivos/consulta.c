#include "consulta.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Arquivos/svg.h"
#include "../Estruturas/lista.h"
#include "../Estruturas/quadtree.h"
#include "../Estruturas/tabelahash.h"
#include "../Utils/caminhos.h"
#include "../Utils/logging.h"
#include "Consultas/fase_1.h"
#include "Consultas/fase_2.h"
#include "Consultas/fase_3.h"
#include "Consultas/fase_4.h"
#include "Consultas/fase_5.h"

// Tamanho maxímo de um comando do arquivo de consulta.
#define TAMANHO_COMANDO 300

struct EstadoComando {
    char nome_comando[10];
    char sufixo_atual[1024];
    Lista svg_atual;
};

// Mantem um sufixo e lista para cada comando que deve manter o estado entre execuções.
struct EstadoComandos {
    struct EstadoComando p;
    struct EstadoComando sp;
    struct EstadoComando pb;
};

// Cria um svg que possui um caminho entre dois registradores, o nome do arquivo utiliza o sufixo
// especificado.
void escrever_svg_caminho(struct EstadoComando estado_atual, Tabela quadtrees, Tabela listas,
                          const char *caminho_log) {
    QuadTree quadras = tabela_buscar(quadtrees, "quadras");
    QuadTree hidrantes = tabela_buscar(quadtrees, "hidrantes");
    QuadTree radios = tabela_buscar(quadtrees, "radios");
    QuadTree semaforos = tabela_buscar(quadtrees, "semaforos");
    QuadTree postos = tabela_buscar(quadtrees, "postos");
    QuadTree casos = tabela_buscar(quadtrees, "casos");
    QuadTree moradores = tabela_buscar(quadtrees, "moradores");
    QuadTree estabelecimentos = tabela_buscar(quadtrees, "estabelecimentos");
    Lista formas = tabela_buscar(listas, "formas");

    char *diretorios = extrair_nome_diretorio(caminho_log);
    char *nome_arquivo = alterar_extensao(caminho_log, 3, "-", estado_atual.sufixo_atual, ".svg");
    char *caminho_arquivo = unir_caminhos(diretorios, nome_arquivo);
    printf("Arquivo %s: %s\n", estado_atual.nome_comando, caminho_arquivo);

    svg_escrever(caminho_arquivo, 10, quadras, hidrantes, semaforos, radios, estabelecimentos,
                 moradores, casos, postos, formas, estado_atual.svg_atual);

    free(diretorios);
    free(nome_arquivo);
    free(caminho_arquivo);
}

// Verifica se o sufixo precisa ser atualizado, caso um novo sufixo exista o arquivo svg do sufixo
// antigo é criado.
void atualizar_sufixo(struct EstadoComando estado_atual, char *novo_sufixo, Tabela quadtrees,
                      Tabela listas, const char *caminho_log) {
    if (novo_sufixo == NULL)
        return;
    if (strlen(estado_atual.sufixo_atual) > 0) {
        escrever_svg_caminho(estado_atual, quadtrees, listas, caminho_log);
        lista_destruir(estado_atual.svg_atual);
        estado_atual.svg_atual = lista_criar(NULL, NULL);
    }
    strcpy(estado_atual.sufixo_atual, novo_sufixo);
    free(novo_sufixo);
    novo_sufixo = NULL;
}

// Ler o arquivo de consulta localizado no caminho fornecido a função e itera por todas as suas
// linhas, executando funções correspondentes aos comandos.
void consulta_ler(const char *caminho_consulta, const char *caminho_log, Tabela quadtrees,
                  Tabela listas, Tabela relacoes, Tabela grafos, Ponto *registradores) {
    printf("Lendo consulta\n");
    FILE *arquivo_consulta = fopen(caminho_consulta, "r");
    if (arquivo_consulta == NULL) {
        LOG_ERRO("Falha ao ler arquivo de consulta: %s!\n", caminho_consulta);
        return;
    }
    FILE *arquivo_log = fopen(caminho_log, "w");
    if (arquivo_log == NULL) {
        LOG_ERRO("Falha ao criar arquivo de log: %s!\n", caminho_log);
        return;
    }

    QuadTree quadras = tabela_buscar(quadtrees, "quadras");
    QuadTree hidrantes = tabela_buscar(quadtrees, "hidrantes");
    QuadTree radios = tabela_buscar(quadtrees, "radios");
    QuadTree semaforos = tabela_buscar(quadtrees, "semaforos");
    QuadTree postos = tabela_buscar(quadtrees, "postos");
    QuadTree casos = tabela_buscar(quadtrees, "casos");
    QuadTree moradores = tabela_buscar(quadtrees, "moradores");
    QuadTree estabelecimentos = tabela_buscar(quadtrees, "estabelecimentos");
    QuadTree qt_vias = tabela_buscar(quadtrees, "vias");

    Lista formas = tabela_buscar(listas, "formas");
    Lista densidades = tabela_buscar(listas, "densidades");

    Grafo vias = tabela_buscar(grafos, "vias");

    Tabela dados_pessoa = tabela_buscar(relacoes, "dados_pessoa");
    Tabela tipo_descricao = tabela_buscar(relacoes, "tipo_descricao");
    Tabela cep_quadra = tabela_buscar(relacoes, "cep_quadra");
    Tabela cnpj_estabelecimento = tabela_buscar(relacoes, "cnpj_estabelecimento");
    Tabela id_hidrante = tabela_buscar(relacoes, "id_hidrante");
    Tabela id_semaforo = tabela_buscar(relacoes, "id_semaforo");
    Tabela id_radio = tabela_buscar(relacoes, "id_radio");
    Tabela id_forma = tabela_buscar(relacoes, "id_forma");

    // Mantem variáveis que devem ser preservadas entre várias execuções dos comandos p?, sp? e pb?.
    struct EstadoComandos estadoComandos = {
        .p = {.nome_comando = "p?",
              .sufixo_atual = "",
              .svg_atual = lista_criar(figura_obter_id, figura_destruir)},
        .sp = {.nome_comando = "sp?",
               .sufixo_atual = "",
               .svg_atual = lista_criar(figura_obter_id, figura_destruir)},
        .pb = {.nome_comando = "pb?",
               .sufixo_atual = "",
               .svg_atual = lista_criar(figura_obter_id, figura_destruir)}};

    char linha[TAMANHO_COMANDO];
    while (fgets(linha, TAMANHO_COMANDO, arquivo_consulta) != NULL) {
        char comando[TAMANHO_COMANDO];
        sscanf(linha, "%s", comando);

        if (strcmp("o?", comando) == 0) {
            checar_interseccao(formas, id_forma, linha, arquivo_log);
        } else if (strcmp("i?", comando) == 0) {
            checar_ponto_interno(formas, id_forma, linha, arquivo_log);
        } else if (strcmp("pnt", comando) == 0) {
            alterar_cor(id_forma, linha, arquivo_log);
        } else if (strcmp("pnt*", comando) == 0) {
            alterar_cores(formas, linha, arquivo_log);
        } else if (strcmp("delf", comando) == 0) {
            remover_elemento(formas, id_forma, linha, arquivo_log);
        } else if (strcmp("delf*", comando) == 0) {
            remover_elementos(formas, id_forma, linha, arquivo_log);
        } else if (strcmp("dq", comando) == 0) {
            raio_remove_quadras(quadras, cep_quadra, id_hidrante, id_semaforo, id_radio, formas,
                                linha, arquivo_log);
        } else if (strcmp("del", comando) == 0) {
            remove_equipamento_urbano(quadras, cep_quadra, hidrantes, id_hidrante, semaforos,
                                      id_semaforo, radios, id_radio, formas, linha, arquivo_log);
        } else if (strcmp("cbq", comando) == 0) {
            circulo_contem_quadras(quadras, linha, arquivo_log);
        } else if (strcmp("crd?", comando) == 0) {
            informacoes_equipamento_urbano(cep_quadra, id_hidrante, id_radio, id_semaforo, linha,
                                           arquivo_log);
        } else if (strcmp("car", comando) == 0) {
            retangulo_area_total_contida(formas, quadras, linha, arquivo_log);
        } else if (strcmp("cv", comando) == 0) {
            adicionar_caso(casos, cep_quadra, linha);
        } else if (strcmp("soc", comando) == 0) {
            postos_mais_proximos(postos, qt_vias, vias, cep_quadra, formas, linha, arquivo_log);
        } else if (strcmp("ci", comando) == 0) {
            determinar_regiao_de_incidencia(formas, casos, postos, densidades, linha, arquivo_log);
        } else if (strcmp("m?", comando) == 0) {
            listar_moradores_quadra(cep_quadra, moradores, linha, arquivo_log);
        } else if (strcmp("dm?", comando) == 0) {
            mostrar_informacoes_morador(formas, dados_pessoa, linha, arquivo_log);
        } else if (strcmp("de?", comando) == 0) {
            mostrar_informacoes_estabelecimento(cnpj_estabelecimento, dados_pessoa, tipo_descricao,
                                                linha, arquivo_log);
        } else if (strcmp("mud", comando) == 0) {
            mudar_endereco_morador(formas, cep_quadra, dados_pessoa, linha, arquivo_log);
        } else if (strcmp("dmprbt", comando) == 0) {
            escrever_quadtree_svg(caminho_log, quadras, hidrantes, semaforos, radios, linha);
        } else if (strcmp("eplg?", comando) == 0) {
            destacar_estabelecimentos_contidos(dados_pessoa, estabelecimentos, linha, arquivo_log);
        } else if (strcmp("catac", comando) == 0) {
            remover_elementos_contidos(formas, quadtrees, relacoes, grafos, linha, arquivo_log);
        } else if (strcmp("@m?", comando) == 0) {
            registrar_posicao_morador(registradores, dados_pessoa, cep_quadra, formas, linha);
        } else if (strcmp("@e?", comando) == 0) {
            registrar_posicao_endereco(registradores, cep_quadra, formas, linha);
        } else if (strcmp("@g?", comando) == 0) {
            registrar_equipamento_urbano(registradores, id_hidrante, id_semaforo, id_radio,
                                         id_forma, formas, linha);
        } else if (strcmp("@xy", comando) == 0) {
            registrar_ponto(registradores, formas, linha);
        } else if (strcmp("ccv", comando) == 0) {
            escrever_grafo_svg(caminho_log, grafos, linha);
        } else if (strcmp("p?", comando) == 0) {
            // TODO Descomentar
            // char *novo_sufixo = calcular_trajeto_vias(
            //     quadtrees, grafos, registradores, estadoComandos.p.svg_atual, linha,
            //     arquivo_log);
            // atualizar_sufixo(estadoComandos.p, novo_sufixo, quadtrees, listas, caminho_log);
        } else if (strcmp("bf", comando) == 0) {
            interditar_ruas(casos, relacoes, vias, formas, linha, arquivo_log);
        } else if (strcmp("pb?", comando) == 0) {
            char *novo_sufixo = calcular_caminho_ciclo_via(
                quadtrees, grafos, registradores, estadoComandos.pb.svg_atual, linha, arquivo_log);
            atualizar_sufixo(estadoComandos.pb, novo_sufixo, quadtrees, listas, caminho_log);
        }
    }

    if (strlen(estadoComandos.p.sufixo_atual) != 0)
        escrever_svg_caminho(estadoComandos.p, quadtrees, listas, caminho_log);
    if (strlen(estadoComandos.pb.sufixo_atual) != 0)
        escrever_svg_caminho(estadoComandos.pb, quadtrees, listas, caminho_log);

    lista_destruir(estadoComandos.p.svg_atual);
    lista_destruir(estadoComandos.sp.svg_atual);
    lista_destruir(estadoComandos.pb.svg_atual);

    fclose(arquivo_log);
    fclose(arquivo_consulta);
}
