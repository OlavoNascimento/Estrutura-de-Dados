#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./Arquivos/comercios.h"
#include "./Arquivos/consulta.h"
#include "./Arquivos/descricao.h"
#include "./Arquivos/svg.h"
#include "./Estruturas/quadtree.h"
#include "./Estruturas/tabelahash.h"
#include "./Interfaces/figura.h"
#include "./Utils/caminhos.h"
#include "./Utils/logging.h"
#include "./Utils/parametros.h"

int main(int argc, const char *argv[]) {
    const Parametros params = parametros_ler(argc, argv);
    if (!parametros_checar_obrigatorios(params)) {
        parametros_destruir(params);
        return 1;
    }

    criar_diretorio(parametros_obter_diretorio_saida(params));
    char *caminho_descricao = parametros_obter_caminho_descricao(params);
    char *caminho_consulta = parametros_obter_caminho_consulta(params);
    char *caminho_estabelecimentos = parametros_obter_caminho_estabelecimentos(params);
    char *caminho_moradores = parametros_obter_caminho_moradores(params);
    char *caminho_svg_descricao = parametros_obter_caminho_svg_descricao(params);

    LOG_INFO("Arquivo descrição: %s\n", caminho_descricao);
    LOG_INFO("Arquivo svg descrição: %s\n", caminho_svg_descricao);

    Tabela quadtrees = tabela_criar(desalocaQt);

    QuadTree formas = criaQt(figura_obter_id);
    tabela_inserir(quadtrees, "formas", formas);
    QuadTree quadras = criaQt(figura_obter_id);
    tabela_inserir(quadtrees, "quadras", quadras);
    QuadTree hidrantes = criaQt(figura_obter_id);
    tabela_inserir(quadtrees, "hidrantes", hidrantes);
    QuadTree radios = criaQt(figura_obter_id);
    tabela_inserir(quadtrees, "radios", radios);
    QuadTree semaforos = criaQt(figura_obter_id);
    tabela_inserir(quadtrees, "semaforos", semaforos);
    QuadTree postos = criaQt(figura_obter_id);
    tabela_inserir(quadtrees, "postos", postos);
    QuadTree densidades = criaQt(figura_obter_id);
    tabela_inserir(quadtrees, "densidades", densidades);
    QuadTree casos = criaQt(figura_obter_id);
    tabela_inserir(quadtrees, "casos", casos);
    QuadTree moradores = criaQt(figura_obter_id);
    tabela_inserir(quadtrees, "moradores", moradores);

    Tabela relacoes = tabela_criar(tabela_destruir);

    Tabela cpf_cep = tabela_criar(NULL);
    tabela_inserir(relacoes, "cpf_cep", cpf_cep);
    Tabela tipo_descricao = tabela_criar(NULL);
    tabela_inserir(relacoes, "tipo_descricao", tipo_descricao);
    Tabela dados_pessoa = tabela_criar(NULL);
    tabela_inserir(relacoes, "dados_pessoa", dados_pessoa);
    Tabela comercial_descricao = tabela_criar(NULL);
    tabela_inserir(relacoes, "comercial_descricao", comercial_descricao);

    descricao_ler(caminho_descricao, quadtrees);

    if (caminho_moradores != NULL) {
        // TODO Adicionar ler_moradores
    }
    if (caminho_estabelecimentos != NULL) {
        comercios_ler(caminho_estabelecimentos, quadtrees, relacoes);
    }

    svg_quadtrees_para_svg(caminho_svg_descricao, 7, quadras, semaforos, radios, hidrantes, casos,
                           postos, formas);

    if (caminho_consulta != NULL) {
        char *caminho_registro_consulta = parametros_obter_caminho_registro_consulta(params);
        char *caminho_svg_consulta = parametros_obter_caminho_svg_consulta(params);

        LOG_INFO("Arquivo consulta: %s\n", caminho_consulta);
        LOG_INFO("Arquivo log: %s\n", caminho_registro_consulta);
        LOG_INFO("Arquivo svg consulta: %s\n", caminho_svg_consulta);

        consulta_ler(caminho_consulta, caminho_registro_consulta, quadtrees, relacoes);
        svg_quadtrees_para_svg(caminho_svg_consulta, 7, quadras, semaforos, radios, hidrantes,
                               casos, postos, formas);

        free(caminho_registro_consulta);
        free(caminho_svg_consulta);
    }

    free(caminho_descricao);
    free(caminho_consulta);
    free(caminho_estabelecimentos);
    free(caminho_moradores);
    free(caminho_svg_descricao);
    parametros_destruir(params);
    tabela_destruir(quadtrees);
    tabela_destruir(relacoes);

    return 0;
}
