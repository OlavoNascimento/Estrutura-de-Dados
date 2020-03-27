#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <parametros.h>

Parametros criar_parametros() {
    Parametros params;
    params.nome_arq_descricao = NULL;
    params.nome_dir_entrada = NULL;
    params.nome_dir_saida = NULL;
    params.nome_arq_consulta = NULL;
    return params;
}

Parametros ler_parametros(int argc, char const *argv[]) {
    Parametros params = criar_parametros();
    int i = 0;
    while(i < argc) {
        if(strcmp("-e", argv[i]) == 0) {
            i++;
            params.nome_dir_entrada = (char*) malloc((strlen(argv[i])+1)*sizeof(char));
            strcpy(params.nome_dir_entrada, argv[i]);
        } else if(strcmp("-f", argv[i]) == 0) {
            i++;
            params.nome_arq_descricao = (char*) malloc((strlen(argv[i])+1)*sizeof(char));
            strcpy(params.nome_arq_descricao, argv[i]);
        } else if(strcmp("-o", argv[i]) == 0) {
            i++;
            params.nome_dir_saida = (char*) malloc((strlen(argv[i])+1)*sizeof(char));
            strcpy(params.nome_dir_saida, argv[i]);
        } else if(strcmp("-q", argv[i]) == 0) {
            i++;
            params.nome_arq_consulta = (char*) malloc((strlen(argv[i])+1)*sizeof(char));
            strcpy(params.nome_arq_consulta, argv[i]);
        }
        i++;
    }
    return params;
}

void destruir_parametros(Parametros params) {
    free(params.nome_arq_descricao);
    free(params.nome_dir_entrada);
    free(params.nome_dir_saida);
    free(params.nome_arq_consulta);
}