#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ler_geo.h"
#include "ler_qry.h"
#include "lista.h"
#include "parametros.h"
#include "utils.h"

int main(int argc, const char *argv[]) {
    const Parametros params = ler_parametros(argc, argv);
    if (!checar_parametros_obrigatorios(params))
        return 1;
    criar_diretorio(parametros_obter_diretorio_saida(params));

    char *caminho_descricao = parametros_obter_caminho_descricao(params);
    char *caminho_consulta = parametros_obter_caminho_consulta(params);
    char *caminho_svg_descricao = parametros_obter_caminho_svg_descricao(params);

    printf("Arquivo descrição: %s\n", caminho_descricao);
    printf("Arquivo svg: %s\n", caminho_svg_descricao);

    Lista *lista = ler_geo(caminho_descricao);
    lista_para_svg(lista, caminho_svg_descricao);

    if (caminho_consulta != NULL) {
        char *caminho_registro_consulta = parametros_obter_caminho_registro_consulta(params);
        char *caminho_svg_consulta = parametros_obter_caminho_svg_consulta(params);

        printf("Arquivo consulta: %s\n", caminho_consulta);
        printf("Arquivo log: %s\n", caminho_registro_consulta);
        printf("Arquivo svg consulta: %s\n", caminho_svg_consulta);

        ler_qry(lista, caminho_consulta, caminho_registro_consulta);
        lista_para_svg(lista, caminho_svg_consulta);

        free(caminho_registro_consulta);
        free(caminho_svg_consulta);
    }

    lista_libera_lista(lista);
    free(caminho_svg_descricao);
    destruir_parametros(params);

    return 0;
}
