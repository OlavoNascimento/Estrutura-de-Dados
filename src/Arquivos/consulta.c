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

// Sufixo adicionado ao final dos arquivos criados por alguns comandos.
struct SufixosComandos {
    char p[1024];
    char sp[1024];
    char pb[1024];
};

// Cria um svg que possui um caminho entre dois registradores utilizando um sufixo para o nome do
// arquivo.
void escrever_svg_caminho(Tabela quadtrees, Tabela listas, Lista svg_atual, const char *sufixo,
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
    char *nome_arquivo = alterar_extensao(caminho_log, 3, "-", sufixo, ".svg");
    char *caminho_arquivo = unir_caminhos(diretorios, nome_arquivo);
    printf("Arquivo pb?: %s\n", caminho_arquivo);

    svg_escrever(caminho_arquivo, 10, quadras, hidrantes, semaforos, radios, estabelecimentos,
                 moradores, casos, postos, formas, svg_atual);
    lista_destruir(svg_atual);
    svg_atual = NULL;

    free(diretorios);
    free(nome_arquivo);
    free(caminho_arquivo);
}

// Verifica se o sufixo precisa ser atualizado, caso um novo sufixo exista o arquivo svg do sufixo
// antigo é criado.
void atualizar_sufixo(char *sufixo, char *novo_sufixo, const char *caminho_log, Tabela quadtrees,
                      Tabela listas, Lista svg_atual) {
    if (strlen(sufixo) == 0 && novo_sufixo != NULL) {
        strcpy(sufixo, novo_sufixo);
    } else if (novo_sufixo != NULL && strcmp(novo_sufixo, sufixo) != 0) {
        escrever_svg_caminho(quadtrees, listas, svg_atual, sufixo, caminho_log);
        svg_atual = lista_criar(NULL, NULL);
        strcpy(sufixo, novo_sufixo);
        free(novo_sufixo);
    }
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

    Lista formas = tabela_buscar(listas, "formas");
    Lista densidades = tabela_buscar(listas, "densidades");

    Tabela dados_pessoa = tabela_buscar(relacoes, "dados_pessoa");
    Tabela tipo_descricao = tabela_buscar(relacoes, "tipo_descricao");
    Tabela cep_quadra = tabela_buscar(relacoes, "cep_quadra");
    Tabela cnpj_estabelecimento = tabela_buscar(relacoes, "cnpj_estabelecimento");
    Tabela id_hidrante = tabela_buscar(relacoes, "id_hidrante");
    Tabela id_semaforo = tabela_buscar(relacoes, "id_semaforo");
    Tabela id_radio = tabela_buscar(relacoes, "id_radio");
    Tabela id_forma = tabela_buscar(relacoes, "id_forma");

    struct SufixosComandos sufixos = {.p = "", .pb = "", .sp = ""};
    Lista svg_pb = lista_criar(NULL, figura_destruir);

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
            postos_mais_proximos(postos, cep_quadra, formas, linha, arquivo_log);
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
        } else if (strcmp("pb?", comando) == 0) {
            char *novo_sufixo = calcular_caminho_ciclo_via(quadtrees, grafos, registradores, svg_pb,
                                                           linha, arquivo_log);
            atualizar_sufixo(sufixos.pb, novo_sufixo, caminho_log, quadtrees, listas, svg_pb);
        }
    }

    if (strlen(sufixos.pb) != 0)
        escrever_svg_caminho(quadtrees, listas, svg_pb, sufixos.pb, caminho_log);

    fclose(arquivo_log);
    fclose(arquivo_consulta);
}
