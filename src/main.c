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

// Cria uma nova tabela contendo quadtrees que armazenam figuras que precisam de buscas espacias.
Tabela criar_tabela_quadtrees() {
    Tabela quadtrees = tabela_criar(desalocaQt);
    tabela_inserir(quadtrees, "formas", criaQt(figura_obter_id));
    tabela_inserir(quadtrees, "quadras", criaQt(figura_obter_id));
    tabela_inserir(quadtrees, "hidrantes", criaQt(figura_obter_id));
    tabela_inserir(quadtrees, "radios", criaQt(figura_obter_id));
    tabela_inserir(quadtrees, "semaforos", criaQt(figura_obter_id));
    tabela_inserir(quadtrees, "postos", criaQt(figura_obter_id));
    tabela_inserir(quadtrees, "casos", criaQt(figura_obter_id));
    tabela_inserir(quadtrees, "moradores", criaQt(figura_obter_id));
    tabela_inserir(quadtrees, "estabelecimentos", criaQt(figura_obter_id));
    return quadtrees;
}

// Cria uma nova tabela contendo listas que armazenam figuras que não precisam de buscas espacias.
Tabela criar_tabela_listas() {
    Tabela listas = tabela_criar(lista_destruir);
    tabela_inserir(listas, "densidades", lista_criar(NULL, figura_destruir));
    return listas;
}

// Cria uma nova tabela contendo tabelas que relacionam os dados das figuras.
Tabela criar_tabela_relacoes() {
    Tabela relacoes = tabela_criar(tabela_destruir);
    tabela_inserir(relacoes, "cpf_cep", tabela_criar(NULL));
    tabela_inserir(relacoes, "tipo_descricao", tabela_criar(NULL));
    tabela_inserir(relacoes, "dados_pessoa", tabela_criar(figura_destruir));
    tabela_inserir(relacoes, "cep_quadra", tabela_criar(NULL));
    tabela_inserir(relacoes, "id_hidrante", tabela_criar(NULL));
    tabela_inserir(relacoes, "id_semaforo", tabela_criar(NULL));
    tabela_inserir(relacoes, "id_radio", tabela_criar(NULL));
    tabela_inserir(relacoes, "id_forma", tabela_criar(NULL));
    return relacoes;
}

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

    Tabela quadtrees = criar_tabela_quadtrees();
    Tabela listas = criar_tabela_listas();
    Tabela relacoes = criar_tabela_relacoes();

    QuadTree formas = tabela_buscar(quadtrees, "formas");
    QuadTree quadras = tabela_buscar(quadtrees, "quadras");
    QuadTree hidrantes = tabela_buscar(quadtrees, "hidrantes");
    QuadTree radios = tabela_buscar(quadtrees, "radios");
    QuadTree semaforos = tabela_buscar(quadtrees, "semaforos");
    QuadTree postos = tabela_buscar(quadtrees, "postos");
    QuadTree casos = tabela_buscar(quadtrees, "casos");
    QuadTree moradores = tabela_buscar(quadtrees, "moradores");
    QuadTree estabelecimentos = tabela_buscar(quadtrees, "estabelecimentos");

    descricao_ler(caminho_descricao, quadtrees, listas, relacoes);

    if (caminho_moradores != NULL)
        pessoas_ler(caminho_moradores, quadtrees, relacoes);

    if (caminho_estabelecimentos != NULL)
        comercios_ler(caminho_estabelecimentos, quadtrees, relacoes);

    svg_escrever_quadtrees(caminho_svg_descricao, 9, quadras, hidrantes, semaforos, radios,
                           estabelecimentos, moradores, casos, postos, formas);

    if (caminho_consulta != NULL) {
        char *caminho_registro_consulta = parametros_obter_caminho_registro_consulta(params);
        char *caminho_svg_consulta = parametros_obter_caminho_svg_consulta(params);

        LOG_INFO("Arquivo consulta: %s\n", caminho_consulta);
        LOG_INFO("Arquivo log: %s\n", caminho_registro_consulta);
        LOG_INFO("Arquivo svg consulta: %s\n", caminho_svg_consulta);

        consulta_ler(caminho_consulta, caminho_registro_consulta, quadtrees, listas, relacoes);
        svg_escrever_quadtrees(caminho_svg_consulta, 9, quadras, hidrantes, semaforos, radios,
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

    tabela_destruir(listas);
    tabela_destruir(quadtrees);
    tabela_destruir(relacoes);

    return 0;
}
