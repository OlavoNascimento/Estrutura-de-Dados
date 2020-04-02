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

    char *caminho_dir_saida = NULL;
    if(params.nome_dir_saida != NULL) {
        caminho_dir_saida = (char*) malloc((strlen(params.nome_dir_saida)+1)*sizeof(char));
        strcpy(caminho_dir_saida, params.nome_dir_saida);
    } else {
        fprintf(stderr, "Parâmetro obrigatório -o (diretório de saída) não foi fornecido!\n");
        return 1;
    }

    char *caminho_consulta = NULL;
    if(params.nome_consulta != NULL)
        caminho_consulta =  preparar_caminho(params.nome_dir_entrada, params.nome_consulta);

    // Adapta o nome do arquivo .svg a partir do arquivo de descrição
    char *nome_svg = alterar_sufixo(params.nome_descricao, ".svg");
    char *caminho_svg = preparar_caminho(params.nome_dir_saida, nome_svg);
    free(nome_svg);
    nome_svg = NULL;

    // Libera a memória usada para armazenar os parâmetros.
    destruir_parametros(params);

    printf("Arquivo de descrição: %s\n", caminho_descricao);
    printf("Nome do arquivo svg: %s\n", caminho_svg);

    Lista *lista = ler_geo(caminho_descricao);
    lista_para_svg(lista, caminho_svg);

    if(caminho_consulta != NULL) {
        char *caminho_log = alterar_sufixo(caminho_svg, "-arqcons.txt");
        char *caminho_svg_consulta = alterar_sufixo(caminho_svg, "-arqcons.svg");
        printf("Arquivo de consulta: %s\n", caminho_consulta);
        printf("Arquivo de log: %s\n", caminho_log);
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

    destruir_lista(lista);
    free(caminho_descricao);
    free(caminho_consulta);
    free(caminho_dir_saida);
    free(caminho_svg);

    return 0;
}
