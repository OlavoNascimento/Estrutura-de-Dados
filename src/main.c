#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./Arquivos/comercios.h"
#include "./Arquivos/consulta.h"
#include "./Arquivos/descricao.h"
#include "./Arquivos/pessoas.h"
#include "./Arquivos/svg.h"
#include "./Arquivos/vias.h"
#include "./Estruturas/grafo.h"
#include "./Estruturas/lista.h"
#include "./Estruturas/quadtree.h"
#include "./Estruturas/tabelahash.h"
#include "./Interfaces/figura.h"
#include "./Utils/caminhos.h"
#include "./Utils/logging.h"
#include "./Utils/parametros.h"

// Cria uma nova tabela contendo quadtrees que armazenam figuras que precisam de buscas espaciais.
Tabela criar_tabela_quadtrees() {
    Tabela quadtrees = tabela_criar((TabelaDestruirInfo *) quadtree_destruir);
    tabela_inserir(quadtrees, "quadras", quadtree_criar(figura_obter_id));
    tabela_inserir(quadtrees, "hidrantes", quadtree_criar(figura_obter_id));
    tabela_inserir(quadtrees, "radios", quadtree_criar(figura_obter_id));
    tabela_inserir(quadtrees, "semaforos", quadtree_criar(figura_obter_id));
    tabela_inserir(quadtrees, "postos", quadtree_criar(figura_obter_id));
    tabela_inserir(quadtrees, "casos", quadtree_criar(figura_obter_id));
    tabela_inserir(quadtrees, "moradores", quadtree_criar(figura_obter_id));
    tabela_inserir(quadtrees, "estabelecimentos", quadtree_criar(figura_obter_id));
    tabela_inserir(quadtrees, "vias", quadtree_criar((funcGetChave *) vertice_obter_id));
    return quadtrees;
}

// Cria uma nova tabela contendo listas que armazenam figuras que não precisam de buscas espaciais.
Tabela criar_tabela_listas() {
    Tabela listas = tabela_criar((TabelaDestruirInfo *) lista_destruir);
    tabela_inserir(listas, "formas", lista_criar(figura_obter_id, figura_destruir));
    tabela_inserir(listas, "densidades", lista_criar(NULL, figura_destruir));
    return listas;
}

// Cria uma nova tabela contendo tabelas que relacionam os dados das figuras.
Tabela criar_tabela_relacoes() {
    Tabela relacoes = tabela_criar((TabelaDestruirInfo *) tabela_destruir);
    tabela_inserir(relacoes, "cpf_cep", tabela_criar(NULL));
    tabela_inserir(relacoes, "tipo_descricao", tabela_criar(free));
    tabela_inserir(relacoes, "cnpj_estabelecimento", tabela_criar(NULL));
    tabela_inserir(relacoes, "dados_pessoa", tabela_criar(figura_destruir));
    tabela_inserir(relacoes, "cep_quadra", tabela_criar(NULL));
    tabela_inserir(relacoes, "id_hidrante", tabela_criar(NULL));
    tabela_inserir(relacoes, "id_semaforo", tabela_criar(NULL));
    tabela_inserir(relacoes, "id_radio", tabela_criar(NULL));
    tabela_inserir(relacoes, "id_forma", tabela_criar(NULL));
    return relacoes;
}

// Cria uma nova tabela contendo grafos.
Tabela criar_tabela_grafos() {
    Tabela grafos = tabela_criar((TabelaDestruirInfo *) grafo_destruir);
    tabela_inserir(grafos, "vias", NULL);
    tabela_inserir(grafos, "ciclovias", NULL);
    return grafos;
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
    char *caminho_vias = parametros_obter_caminho_vias(params);
    char *caminho_svg_descricao = parametros_obter_caminho_svg_descricao(params);

    printf("Arquivo svg descrição: %s\n", caminho_svg_descricao);

    Tabela quadtrees = criar_tabela_quadtrees();
    Tabela listas = criar_tabela_listas();
    Tabela relacoes = criar_tabela_relacoes();
    Tabela grafos = criar_tabela_grafos();
    Ponto registradores[11] = {
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    };

    Lista formas = tabela_buscar(listas, "formas");
    QuadTree quadras = tabela_buscar(quadtrees, "quadras");
    QuadTree hidrantes = tabela_buscar(quadtrees, "hidrantes");
    QuadTree radios = tabela_buscar(quadtrees, "radios");
    QuadTree semaforos = tabela_buscar(quadtrees, "semaforos");
    QuadTree postos = tabela_buscar(quadtrees, "postos");
    QuadTree casos = tabela_buscar(quadtrees, "casos");
    QuadTree moradores = tabela_buscar(quadtrees, "moradores");
    QuadTree estabelecimentos = tabela_buscar(quadtrees, "estabelecimentos");

    Grafo vias = tabela_buscar(grafos, "vias");

    descricao_ler(caminho_descricao, quadtrees, listas, relacoes);

    if (caminho_moradores != NULL)
        pessoas_ler(caminho_moradores, quadtrees, relacoes);

    if (caminho_estabelecimentos != NULL)
        comercios_ler(caminho_estabelecimentos, quadtrees, relacoes);

    if (caminho_vias != NULL) {
        tabela_remover(grafos, "vias");
        tabela_inserir(grafos, "vias", grafo_criar(100000));
        vias = tabela_buscar(grafos, "vias");
        vias_ler(caminho_vias, quadtrees, vias);
    }

    svg_escrever(caminho_svg_descricao, 9, quadras, hidrantes, semaforos, radios, estabelecimentos,
                 moradores, casos, postos, formas);

    if (caminho_consulta != NULL) {
        char *caminho_registro_consulta = parametros_obter_caminho_registro_consulta(params);
        char *caminho_svg_consulta = parametros_obter_caminho_svg_consulta(params);

        printf("Arquivo log: %s\n", caminho_registro_consulta);
        printf("Arquivo svg consulta: %s\n", caminho_svg_consulta);

        consulta_ler(caminho_consulta, caminho_registro_consulta, quadtrees, listas, relacoes,
                     grafos, registradores);
        svg_escrever(caminho_svg_consulta, 9, quadras, hidrantes, semaforos, radios,
                     estabelecimentos, moradores, casos, postos, formas);

        free(caminho_registro_consulta);
        free(caminho_svg_consulta);
    }

    free(caminho_descricao);
    free(caminho_consulta);
    free(caminho_estabelecimentos);
    free(caminho_moradores);
    free(caminho_vias);
    free(caminho_svg_descricao);
    parametros_destruir(params);

    quadtree_percorrer_largura(quadras, (void *) figura_destruir, NULL);
    quadtree_percorrer_largura(hidrantes, (void *) figura_destruir, NULL);
    quadtree_percorrer_largura(radios, (void *) figura_destruir, NULL);
    quadtree_percorrer_largura(semaforos, (void *) figura_destruir, NULL);
    quadtree_percorrer_largura(postos, (void *) figura_destruir, NULL);
    quadtree_percorrer_largura(casos, (void *) figura_destruir, NULL);
    quadtree_percorrer_largura(estabelecimentos, (void *) figura_destruir, NULL);

    for (int i = 0; i < 11; i++)
        ponto_destruir(registradores[i]);

    tabela_destruir(listas);
    tabela_destruir(quadtrees);
    tabela_destruir(relacoes);
    tabela_destruir(grafos);

    return 0;
}
