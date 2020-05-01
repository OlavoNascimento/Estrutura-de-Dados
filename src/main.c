#include <stdio.h>
#include <stdlib.h>

#include <parametros.h>
#include <lista.h>
#include <ler_geo.h>
#include <ler_qry.h>

// Usa o nome do arquivo de descrição para criar o nome do svg.
char *criar_caminho_svg_descricao(const Parametros params) {
    char *nome_svg_descricao = alterar_sufixo(
        params.nome_descricao, 1, ".svg"
    );
    char *caminho_svg_descricao = preparar_caminho(
        params.nome_dir_saida, nome_svg_descricao
    );
    free(nome_svg_descricao);
    return caminho_svg_descricao;
}

// Cria uma nova string baseada no arquivo de descrição, porém com sufixo
// adicional com o nome da consulta que está sendo executada, utilizado pelo
// arquivo de log e svg da consulta.
char *criar_caminho_resultado_consulta(const Parametros params,
                                       const char *extensao) {
    char *nome_base_consulta = extrair_nome_base(params.nome_consulta);
    char *nome_resultado_consulta = alterar_sufixo(
        params.nome_descricao, 3, "-", nome_base_consulta, extensao
    );
    char *caminho_resultado_consulta = preparar_caminho(
        params.nome_dir_saida, nome_resultado_consulta
    );
    free(nome_base_consulta);
    free(nome_resultado_consulta);
    return caminho_resultado_consulta;
}

int main(int argc, const char *argv[]) {
    const Parametros params = ler_parametros(argc, argv);
    if(!checar_parametros_obrigatorios(params))
        return 1;

    // Adiciona o diretório de entrada ao caminho do arquivo de descrição caso
    // necessário.
    char *caminho_descricao = preparar_caminho(
        params.nome_dir_entrada, params.nome_descricao
    );
    char *caminho_svg_descricao = criar_caminho_svg_descricao(params);
    printf("Arquivo descrição: %s\n", caminho_descricao);
    printf("Arquivo svg: %s\n", caminho_svg_descricao);

    Lista *lista = ler_geo(caminho_descricao);
    lista_para_svg(lista, caminho_svg_descricao);

    if(params.nome_consulta != NULL) {
        // Adiciona o diretório de entrada ao caminho do arquivo de consulta
        // caso necessário.
        char *caminho_consulta = preparar_caminho(
            params.nome_dir_entrada, params.nome_consulta
        );
        char *caminho_log = criar_caminho_resultado_consulta(params, ".txt");
        char *caminho_svg_consulta = criar_caminho_resultado_consulta(
            params, ".svg"
        );
        printf("Arquivo consulta: %s\n", caminho_consulta);
        printf("Arquivo log: %s\n", caminho_log);
        printf("Arquivo svg consulta: %s\n", caminho_svg_consulta);

        ler_qry(lista, caminho_consulta, caminho_log);
        lista_para_svg(lista, caminho_svg_consulta);

        free(caminho_log);
        free(caminho_svg_consulta);
        free(caminho_consulta);
    }

    destruir_parametros(params);
    destruir_lista(lista);
    free(caminho_svg_descricao);
    free(caminho_descricao);

    return 0;
}
