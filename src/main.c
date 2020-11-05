#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ler_geo.h"
#include "ler_qry.h"
#include "lista.h"
#include "logging.h"
#include "parametros.h"
#include "svg.h"
#include "utils.h"

int main(int argc, const char *argv[]) {
    const Parametros params = parametros_ler(argc, argv);
    if (!parametros_checar_obrigatorios(params))
        return 1;

    criar_diretorio(parametros_obter_diretorio_saida(params));
    char *caminho_descricao = parametros_obter_caminho_descricao(params);
    char *caminho_consulta = parametros_obter_caminho_consulta(params);
    char *caminho_svg_descricao = parametros_obter_caminho_svg_descricao(params);

    LOG_INFO("Arquivo descrição: %s\n", caminho_descricao);
    LOG_INFO("Arquivo svg descrição: %s\n", caminho_svg_descricao);

    Lista *lista_formas = lista_create();
    Lista *lista_quadras = lista_create();
    Lista *lista_hidrantes = lista_create();
    Lista *lista_radios = lista_create();
    Lista *lista_semaforos = lista_create();
    Lista *lista_postos = lista_create();
    Lista *lista_densidades = lista_create();
    ler_geo(caminho_descricao, lista_formas, lista_quadras, lista_hidrantes, lista_radios,
            lista_semaforos, lista_formas, lista_densidades);
    svg_lista_para_svg(caminho_svg_descricao, lista_formas, lista_quadras, lista_hidrantes,
                       lista_radios, lista_semaforos, lista_postos);

    if (caminho_consulta != NULL) {
        char *caminho_registro_consulta = parametros_obter_caminho_registro_consulta(params);
        char *caminho_svg_consulta = parametros_obter_caminho_svg_consulta(params);

        LOG_INFO("Arquivo consulta: %s\n", caminho_consulta);
        LOG_INFO("Arquivo log: %s\n", caminho_registro_consulta);
        LOG_INFO("Arquivo svg consulta: %s\n", caminho_svg_consulta);

        ler_qry(caminho_consulta, caminho_registro_consulta, lista_formas, lista_quadras,
                lista_hidrantes, lista_radios, lista_semaforos, lista_postos, lista_densidades);
        svg_lista_para_svg(caminho_svg_consulta, lista_formas, lista_quadras, lista_hidrantes,
                           lista_radios, lista_semaforos, lista_postos);

        free(caminho_registro_consulta);
        free(caminho_svg_consulta);
    }

    lista_libera_lista(lista_formas);
    lista_libera_lista(lista_quadras);
    lista_libera_lista(lista_hidrantes);
    lista_libera_lista(lista_semaforos);
    lista_libera_lista(lista_radios);
    free(caminho_descricao);
    free(caminho_consulta);
    free(caminho_svg_descricao);
    parametros_destruir(params);

    return 0;
}
