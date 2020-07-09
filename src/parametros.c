#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#include <parametros.h>

// Retorna uma struct com todos os parâmetros que podem ser lidos pelo programa.
Parametros criar_parametros() {
    Parametros params;
    params.nome_descricao = NULL;
    params.nome_dir_entrada = NULL;
    params.nome_dir_saida = NULL;
    params.nome_consulta = NULL;
    return params;
}

// Retorna uma struct contendo os parâmetros fornecidos ao programa.
Parametros ler_parametros(int argc, const char *argv[]) {
    Parametros params = criar_parametros();
    int i = 0;
    while(i < argc) {
        if(strcmp("-e", argv[i]) == 0) {
            i++;
            params.nome_dir_entrada = malloc(
                (strlen(argv[i]) + 1) * sizeof(char)
            );
            strcpy(params.nome_dir_entrada, argv[i]);
        } else if(strcmp("-f", argv[i]) == 0) {
            i++;
            params.nome_descricao = malloc(
                (strlen(argv[i]) + 1) * sizeof(char)
            );
            strcpy(params.nome_descricao, argv[i]);
        } else if(strcmp("-o", argv[i]) == 0) {
            i++;
            params.nome_dir_saida = malloc(
                (strlen(argv[i]) + 1) * sizeof(char)
            );
            strcpy(params.nome_dir_saida, argv[i]);
        } else if(strcmp("-q", argv[i]) == 0) {
            i++;
            params.nome_consulta = malloc(
                (strlen(argv[i]) + 1) * sizeof(char)
            );
            strcpy(params.nome_consulta, argv[i]);
        }
        i++;
    }
    return params;
}

// Retorna verdadeiro caso os parâmetros buscados pela função existam.
bool checar_parametros_obrigatorios(const Parametros params) {
    if(params.nome_descricao == NULL) {
        fprintf(
            stderr,
            "Parâmetro obrigatório -f (arquivo .geo) não foi fornecido!\n"
        );
        return false;
    }

    if(params.nome_dir_saida == NULL) {
        fprintf(
            stderr,
            "Parâmetro obrigatório -o (diretório de saída) não foi fornecido!\n"
        );
        return false;
    }
    return true;
}

// Concatena um diretório com um arquivo fornecido, separados por uma '/'.
char *unir_caminhos(const char *diretorio, const char *nome_arquivo) {
    if(diretorio == NULL || nome_arquivo == NULL) {
        fprintf(stderr, "Variável nula fornecida ao unir caminhos!\n");
        return NULL;
    }

    char *uniao = malloc(strlen(diretorio) + strlen(nome_arquivo) + 2);
    if(uniao == NULL) {
        fprintf(stderr, "Falha ao alocar memória paro o caminho %s/%s!\n",
                diretorio, nome_arquivo);
        return NULL;
    }
    sprintf(uniao, "%s/%s", diretorio, nome_arquivo);
    return uniao;
}

// Adiciona um diretório ao caminho de um arquivo caso ele seja fornecido.
char *preparar_caminho(const char *diretorio, const char *nome_arquivo) {
    if(nome_arquivo == NULL) {
        fprintf(stderr, "Nome de arquivo nulo passado a preparar caminho!\n");
        return NULL;
    }
    char *caminho_final = NULL;
    if(diretorio != NULL) {
        // Adiciona o diretório ao caminho do arquivo.
        caminho_final = unir_caminhos(diretorio, nome_arquivo);
    } else {
        // Caso o diretório não seja fornecido usa o caminho do arquivo como
        // caminho final.
        caminho_final = malloc(strlen(nome_arquivo) + 1);
        strcpy(caminho_final, nome_arquivo);
    }
    return caminho_final;
}

// Extrai o nome do arquivo sem sua extensão e o diretórios que compõem seu
// caminho de uma string no formato:
// /diretório-a/diretório-b/nome-arquivo.ext
char *extrair_nome_base(const char *caminho_arquivo) {
    // Avança até a última / no caminho do arquivo.
    char *nome_arquivo = strrchr(caminho_arquivo, '/');

    char *nome_base = NULL;
    if(nome_arquivo != NULL) {
        // Arquivo tem uma / em seu caminho.
        // Avança um caratere, removendo a /
        nome_arquivo++;
        nome_base = malloc(
            (strlen(nome_arquivo) + 1) * sizeof(char)
        );
        if(nome_base == NULL)
            return NULL;
        strcpy(nome_base, nome_arquivo);
    } else {
        // Arquivo não tem uma / em seu caminho, consequentemente o caminho
        // passado a função já não possui diretórios antecedentes ao arquivo.
        nome_base = malloc(
            (strlen(caminho_arquivo) + 1) * sizeof(char)
        );
        if(nome_base == NULL)
            return NULL;
        strcpy(nome_base, caminho_arquivo);
    }

    // Avança até o último . no nome do arquivo.
    char *extensao = strrchr(nome_base, '.');
    // Se o arquivo possui uma extensão ela será usada para delimitar o fim da
    // string.
    if(extensao != NULL)
        *extensao = '\0';

    return nome_base;
}

// Cria uma string baseado em um nome de arquivo passado a função, porém sua
// extensão é substituida por sufixos adicionais.
char *alterar_sufixo(const char *nome_arquivo, int num_sufixos, ...) {
    char *sufixo_final = malloc(sizeof(char));
    sufixo_final[0] = '\0';

    va_list sufixos;
    va_start(sufixos, num_sufixos);
    // Concatena todos os sufixos em uma string
    for (int i = 0; i < num_sufixos; i++) {
        // Recebe o próximo sufixo da lista
        char *sufixo = va_arg(sufixos, char*);
        // Aumenta o tamanho da string para receber o proximo sufixo
        char *tmp = realloc(
            sufixo_final,
            (strlen(sufixo_final) + strlen(sufixo) + 1) * sizeof(char)
        );
        if(tmp == NULL) {
            fprintf(stderr, "Falha ao alocar memória para novo sufixo!\n");
            free(sufixo_final);
            return NULL;
        }
        sufixo_final = tmp;
        strcat(sufixo_final, sufixo);
    }
    va_end(sufixos);

    // Recebe o nome do arquivo, sem os diretórios que compõem seu caminho e sua
    // extensão
    char *nome_base = extrair_nome_base(nome_arquivo);
    char *novo_nome = malloc(
        (strlen(nome_base) + strlen(sufixo_final) + 1) * sizeof(char)
    );
    sprintf(novo_nome, "%s%s", nome_base, sufixo_final);
    free(nome_base);
    free(sufixo_final);
    return novo_nome;
}

// Libera a memória alocada pelos argumentos.
void destruir_parametros(Parametros params) {
    free(params.nome_descricao);
    free(params.nome_dir_entrada);
    free(params.nome_dir_saida);
    free(params.nome_consulta);
}
