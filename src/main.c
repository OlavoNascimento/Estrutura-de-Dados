#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ler_geo.h>
#include <lista.h>
#include <parametros.h>

char* unir_caminhos(char* dir_entrada, char* nome_arquivo) {
    if(dir_entrada == NULL || nome_arquivo == NULL) {
        fprintf(stderr, "Variável nula fornecida ao unir caminhos!\n");
        return NULL;
    }

    char *temp = (char*) malloc(strlen(dir_entrada)+strlen(nome_arquivo)+2);
    if(temp == NULL) {
        fprintf(stderr, "Falha ao alocar memória paro o caminho %s/%s!\n",
                dir_entrada, nome_arquivo);
        return NULL;
    }
    sprintf(temp, "%s/%s", dir_entrada, nome_arquivo);
    return temp;
}

char* preparar_caminho(char* dir_entrada, char* nome_arquivo) {
    char* caminho_final = NULL;
    if(dir_entrada != NULL) {
        caminho_final = unir_caminhos(dir_entrada, nome_arquivo);
    } else {
        caminho_final = (char*) malloc(strlen(nome_arquivo)+1);
        strcpy(caminho_final, nome_arquivo);
    }
    return caminho_final;
}

int main(int argc, char const *argv[]) {
    Parametros params = ler_parametros(argc, argv);

    char *caminho_arq_descricao = NULL;
    if(params.nome_arq_descricao != NULL) {
        caminho_arq_descricao = preparar_caminho(params.nome_dir_entrada, params.nome_arq_descricao);
    } else {
        fprintf(stderr, "Parâmetro obrigatório -f não foi fornecido!\n");
        return 1;
    }

    char *caminho_dir_saida = NULL;
    if(params.nome_dir_saida != NULL) {
        caminho_dir_saida = (char*) malloc((strlen(params.nome_dir_saida)+1)*sizeof(char));
        strcpy(caminho_dir_saida, params.nome_dir_saida);
    } else {
        fprintf(stderr, "Parâmetro obrigatório -o não foi fornecido!\n");
        return 1;
    }

    char *caminho_arq_consulta = NULL;
    if(params.nome_arq_consulta != NULL)
        caminho_arq_consulta =  preparar_caminho(params.nome_dir_entrada, params.nome_arq_consulta);

    destruir_parametros(params);

    printf("%s\n", caminho_arq_descricao);
    printf("%s\n", caminho_dir_saida);
    if(caminho_arq_consulta != NULL)
        printf("%s\n", caminho_arq_consulta);

    Lista *lista = ler_geo(caminho_arq_descricao);
    if(lista == NULL) {
        fprintf(stderr, "Falha ao ler %s!\n", caminho_arq_descricao);
        return 1;
    }

    // TODO Permitir o nome do arquivo .svg ser variável
    lista_para_svg(lista, "saida.svg");

    destruir_lista(lista);

    return 0;
}
