#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./Arquivos/consulta.h"
#include "./Arquivos/descricao.h"
#include "./Arquivos/svg.h"
#include "./Estruturas/quadtree.h"
#include "./Interfaces/figura.h"
#include "./Objetos/Formas/retangulo.h"
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

    QuadTree *formas = criaQt(figura_obter_id);
    QuadTree *quadras = criaQt(figura_obter_id);
    QuadTree *hidrantes = criaQt(figura_obter_id);
    QuadTree *radios = criaQt(figura_obter_id);
    QuadTree *semaforos = criaQt(figura_obter_id);
    QuadTree *postos = criaQt(figura_obter_id);
    QuadTree *densidades = criaQt(figura_obter_id);
    QuadTree *casos = criaQt(figura_obter_id);

    descricao_ler(caminho_descricao, formas, quadras, hidrantes, radios, semaforos, postos,
                  densidades);

    if (caminho_estabelecimentos != NULL) {
        // TODO Adicionar ler_estabelecimentos
    }
    if (caminho_moradores != NULL) {
        // TODO Adicionar ler_moradores
    }

    svg_quadtree_para_svg(caminho_svg_descricao, formas, quadras, hidrantes, radios, semaforos,
                          postos, casos);

    if (caminho_consulta != NULL) {
        char *caminho_registro_consulta = parametros_obter_caminho_registro_consulta(params);
        char *caminho_svg_consulta = parametros_obter_caminho_svg_consulta(params);

        LOG_INFO("Arquivo consulta: %s\n", caminho_consulta);
        LOG_INFO("Arquivo log: %s\n", caminho_registro_consulta);
        LOG_INFO("Arquivo svg consulta: %s\n", caminho_svg_consulta);

        consulta_ler(caminho_consulta, caminho_registro_consulta, formas, quadras, hidrantes,
                     radios, semaforos, postos, densidades, casos);
        svg_quadtree_para_svg(caminho_svg_consulta, formas, quadras, hidrantes, radios, semaforos,
                              postos, casos);

        free(caminho_registro_consulta);
        free(caminho_svg_consulta);
    }

    free(caminho_descricao);
    free(caminho_consulta);
    free(caminho_estabelecimentos);
    free(caminho_moradores);
    free(caminho_svg_descricao);
    parametros_destruir(params);
    desalocaQt(formas);
    desalocaQt(quadras);
    desalocaQt(hidrantes);
    desalocaQt(semaforos);
    desalocaQt(radios);
    desalocaQt(postos);
    desalocaQt(densidades);
    desalocaQt(casos);

    return 0;
}
