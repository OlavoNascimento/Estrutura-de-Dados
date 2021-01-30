#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./Arquivos/comercios.h"
#include "./Arquivos/consulta.h"
#include "./Arquivos/descricao.h"
#include "./Arquivos/pessoas.h"
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
    QuadTree casos = criaQt(figura_obter_id);
    tabela_inserir(quadtrees, "casos", casos);
    QuadTree moradores = criaQt(figura_obter_id);
    tabela_inserir(quadtrees, "moradores", moradores);
    QuadTree estabelecimentos = criaQt(figura_obter_id);
    tabela_inserir(quadtrees, "estabelecimentos", estabelecimentos);
    QuadTree densidades = criaQt(figura_obter_id);
    tabela_inserir(quadtrees, "densidades", densidades);

    Tabela relacoes = tabela_criar(tabela_destruir);
    tabela_inserir(relacoes, "cpf_cep", tabela_criar(NULL));
    tabela_inserir(relacoes, "tipo_descricao", tabela_criar(NULL));
    tabela_inserir(relacoes, "dados_pessoa", tabela_criar(figura_destruir));
    tabela_inserir(relacoes, "cep_quadra", tabela_criar(NULL));
    tabela_inserir(relacoes, "id_hidrante", tabela_criar(NULL));
    tabela_inserir(relacoes, "id_semaforo", tabela_criar(NULL));
    tabela_inserir(relacoes, "id_radio", tabela_criar(NULL));
    tabela_inserir(relacoes, "id_forma", tabela_criar(NULL));

    descricao_ler(caminho_descricao, quadtrees, relacoes);

    if (caminho_moradores != NULL)
        pessoas_ler(caminho_moradores, quadtrees, relacoes);

    if (caminho_estabelecimentos != NULL)
        comercios_ler(caminho_estabelecimentos, quadtrees, relacoes);

    svg_quadtrees_para_svg(caminho_svg_descricao, 9, quadras, hidrantes, semaforos, radios,
                           estabelecimentos, moradores, casos, postos, formas);

    if (caminho_consulta != NULL) {
        char *caminho_registro_consulta = parametros_obter_caminho_registro_consulta(params);
        char *caminho_svg_consulta = parametros_obter_caminho_svg_consulta(params);

        LOG_INFO("Arquivo consulta: %s\n", caminho_consulta);
        LOG_INFO("Arquivo log: %s\n", caminho_registro_consulta);
        LOG_INFO("Arquivo svg consulta: %s\n", caminho_svg_consulta);

        consulta_ler(caminho_consulta, caminho_registro_consulta, quadtrees, relacoes);
        svg_quadtrees_para_svg(caminho_svg_consulta, 9, quadras, hidrantes, semaforos, radios,
                               estabelecimentos, moradores, casos, postos, formas);

        free(caminho_registro_consulta);
        free(caminho_svg_consulta);
    }

    free(caminho_descricao);
    free(caminho_consulta);
    free(caminho_estabelecimentos);
    free(caminho_moradores);
    free(caminho_svg_descricao);
    parametros_destruir(params);

    percorreLarguraQt(formas, (void *) figura_destruir, NULL);
    percorreLarguraQt(quadras, (void *) figura_destruir, NULL);
    percorreLarguraQt(hidrantes, (void *) figura_destruir, NULL);
    percorreLarguraQt(radios, (void *) figura_destruir, NULL);
    percorreLarguraQt(semaforos, (void *) figura_destruir, NULL);
    percorreLarguraQt(postos, (void *) figura_destruir, NULL);
    percorreLarguraQt(casos, (void *) figura_destruir, NULL);
    percorreLarguraQt(estabelecimentos, (void *) figura_destruir, NULL);
    percorreLarguraQt(densidades, (void *) figura_destruir, NULL);

    tabela_destruir(quadtrees);
    tabela_destruir(relacoes);

    return 0;
}
