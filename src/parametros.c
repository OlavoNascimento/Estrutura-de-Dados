#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <parametros.h>

// Retorna uma struct com todos os parâmetros
// que podem ser lidos pelo programa.
Parametros criar_parametros() {
    Parametros params;
    params.nome_descricao = NULL;
    params.nome_dir_entrada = NULL;
    params.nome_dir_saida = NULL;
    params.nome_consulta = NULL;
    return params;
}

// Retorna uma struct contendo os parâmetros fornecidos ao programa.
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
            params.nome_descricao = (char*) malloc((strlen(argv[i])+1)*sizeof(char));
            strcpy(params.nome_descricao, argv[i]);
        } else if(strcmp("-o", argv[i]) == 0) {
            i++;
            params.nome_dir_saida = (char*) malloc((strlen(argv[i])+1)*sizeof(char));
            strcpy(params.nome_dir_saida, argv[i]);
        } else if(strcmp("-q", argv[i]) == 0) {
            i++;
            params.nome_consulta = (char*) malloc((strlen(argv[i])+1)*sizeof(char));
            strcpy(params.nome_consulta, argv[i]);
        }
        i++;
    }
    return params;
}

// Concatena um diretório com um arquivo fornecido,
// separados por uma '/'.
char* unir_caminhos(char* diretorio, char* nome_arquivo) {
    if(diretorio == NULL || nome_arquivo == NULL) {
        fprintf(stderr, "Variável nula fornecida ao unir caminhos!\n");
        return NULL;
    }

    char *uniao = (char*) malloc(strlen(diretorio)+strlen(nome_arquivo)+2);
    if(uniao == NULL) {
        fprintf(stderr, "Falha ao alocar memória paro o caminho %s/%s!\n",
                diretorio, nome_arquivo);
        return NULL;
    }
    sprintf(uniao, "%s/%s", diretorio, nome_arquivo);
    return uniao;
}

// Adiciona um diretório ao caminho de um arquivo caso
// ele seja fornecido.
char* preparar_caminho(char* diretorio, char* nome_arquivo) {
    char* caminho_final = NULL;
    if(diretorio != NULL) {
        // Adiciona o diretório ada ao caminho do arquivo
        caminho_final = unir_caminhos(diretorio, nome_arquivo);
    } else {
        // Caso o diretório de entrada não seja fornecido
        // usa o caminho do arquivo como caminho final.
        caminho_final = (char*) malloc(strlen(nome_arquivo)+1);
        strcpy(caminho_final, nome_arquivo);
    }
    return caminho_final;
}

// Extrai o nome do arquivo de uma string no formato: nomeArquivo.ext
char* extrair_nome_base(char* nome_arquivo) {
    // Aloca memória apenas para o nome do arquivo.
    char *nome_base = (char*) malloc((strlen(nome_arquivo)+1)*sizeof(char));
    // Extrai tudo até o primeiro . existente na string.
    sscanf(nome_arquivo, "%[^.]", nome_base);
    return nome_base;
}

// Remove a extensão de um arquivo no formato nomeArquivo.ext e
// em seu lugar coloca a nova extensão passada como argumento para
// a função.
char* substituir_extensao(char* nome_arquivo, char* ext_nova) {
    char *nome_base = extrair_nome_base(nome_arquivo);
    char *novo_nome = (char*) malloc((strlen(nome_arquivo)+1)*sizeof(char));
    sprintf(novo_nome, "%s.%s", nome_base, ext_nova);
    free(nome_base);
    return novo_nome;
}

// Libera a memória alocada pelos argumentos.
void destruir_parametros(Parametros params) {
    free(params.nome_descricao);
    free(params.nome_dir_entrada);
    free(params.nome_dir_saida);
    free(params.nome_consulta);
}