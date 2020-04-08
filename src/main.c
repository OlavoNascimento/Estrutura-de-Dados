#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ler_geo.h>
#include <ler_qry.h>
#include <lista.h>
#include <parametros.h>

int main(int argc, char const *argv[]) {
    Parametros params = ler_parametros(argc, argv);

    char *caminho_descricao = NULL;
    if(params.nome_descricao != NULL) {
        caminho_descricao = preparar_caminho(params.nome_dir_entrada, params.nome_descricao);
    } else {
        fprintf(stderr, "Parâmetro obrigatório -f (arquivo .geo) não foi fornecido!\n");
        return 1;
    }

    if(params.nome_dir_saida == NULL) {
        fprintf(stderr, "Parâmetro obrigatório -o (diretório de saída) não foi fornecido!\n");
        return 1;
    }

    char *caminho_consulta = NULL;
    if(params.nome_consulta != NULL)
        caminho_consulta =  preparar_caminho(params.nome_dir_entrada, params.nome_consulta);

    // Adapta o nome do arquivo .svg a partir do arquivo de descrição
    char *caminho_svg = preparar_caminho_sufixo(params.nome_dir_saida, params.nome_descricao, ".svg");

    printf("Arquivo descrição: %s\n", caminho_descricao);
    printf("Arquivo svg: %s\n", caminho_svg);

    Lista *lista = ler_geo(caminho_descricao);
    lista_para_svg(lista, caminho_svg);

    if(caminho_consulta != NULL) {
        // Cria um sufixo usando o nome do arquivo de consulta.
        char *sufixo_log = preparar_sufixo_consulta(caminho_consulta, ".txt");
        // Cria um caminho com o sufixo criado anteriormente.
        char *caminho_log = preparar_caminho_sufixo(params.nome_dir_saida,
                                                    params.nome_descricao,
                                                    sufixo_log);
        free(sufixo_log);

        char *sufixo_svg_consulta = preparar_sufixo_consulta(caminho_consulta, ".svg");
        char *caminho_svg_consulta = preparar_caminho_sufixo(params.nome_dir_saida,
                                                             params.nome_descricao,
                                                             sufixo_svg_consulta);
        free(sufixo_svg_consulta);

        printf("Arquivo consulta: %s\n", caminho_consulta);
        printf("Arquivo log: %s\n", caminho_log);
        printf("Arquivo svg consulta: %s\n", caminho_svg_consulta);

        FILE *arquivo_log = fopen(caminho_log, "w");
        if(arquivo_log == NULL) {
            fprintf(stderr, "Falha ao criar arquivo de log!\n");
            return 1;
        }

        ler_qry(lista, caminho_consulta, arquivo_log);
        lista_para_svg(lista, caminho_svg_consulta);

        fclose(arquivo_log);
        free(caminho_log);
        free(caminho_svg_consulta);
    }

    destruir_parametros(params);
    destruir_lista(lista);
    free(caminho_descricao);
    free(caminho_consulta);
    free(caminho_svg);

    return 0;
}
