#include "utils.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logging.h"

#ifdef _WIN32
    #include <windows.h>
    #define mkdir(dir, mode) _mkdir(dir)
    #define SEPARADOR_DE_DIRETORIOS '\\'
#elif __linux__
    #include <sys/stat.h>
    #include <sys/types.h>
    #define SEPARADOR_DE_DIRETORIOS '/'
#endif

// Cria todos os diretórios especificados no caminho passado a função.
void criar_diretorio(const char *diretorio) {
    if (diretorio == NULL) {
        LOG_ERROR("Diretório nulo fornecida ao criar diretório!\n");
        return;
    }

    char *caminho = malloc((strlen(diretorio) + 1) * sizeof(char));
    strcpy(caminho, diretorio);
    // Avança caractere por caractere até encontrar o separador de diretórios do sistema, quando o
    // separador é encontrado cria o diretório especificado até aquele ponto.
    for (char *p = caminho + 1; *p; p++) {
        if (*p == SEPARADOR_DE_DIRETORIOS) {
            // Marca o caractere atual como fim da string temporariamente.
            *p = '\0';
            mkdir(caminho, 0755);
            *p = SEPARADOR_DE_DIRETORIOS;
        }
    }
    mkdir(caminho, 0755);
    free(caminho);
}

// Concatena um diretório com um arquivo fornecido, separados por uma / ou \.
char *unir_caminhos(const char *diretorio, const char *nome_arquivo) {
    if (diretorio == NULL || nome_arquivo == NULL) {
        LOG_ERROR("Variável nula fornecida ao unir caminhos!\n");
        return NULL;
    }

    char *uniao = malloc(strlen(diretorio) + strlen(nome_arquivo) + 2);
    if (uniao == NULL) {
        LOG_ERROR("Falha ao alocar memória paro o caminho %s%c%s!\n", diretorio,
                  SEPARADOR_DE_DIRETORIOS, nome_arquivo);
        return NULL;
    }
    sprintf(uniao, "%s%c%s", diretorio, SEPARADOR_DE_DIRETORIOS, nome_arquivo);
    return uniao;
}

// Extrai o nome do arquivo sem sua extensão e o diretórios que compõem seu caminho de uma string no
// formato: /diretório/.../nome-arquivo.ext
char *extrair_nome_base(const char *caminho_arquivo) {
    if (caminho_arquivo == NULL) {
        LOG_ERROR("Caminho de arquivo nulo fornecida ao extrair nome base!\n");
        return NULL;
    }

    // Avança até a última / ou \ no caminho do arquivo.
    char *nome_arquivo = strrchr(caminho_arquivo, SEPARADOR_DE_DIRETORIOS);

    char *nome_base = NULL;
    if (nome_arquivo != NULL) {
        // Caso o arquivo tem uma / ou \ em seu caminho, avança um caratere, removendo a /
        nome_arquivo++;
        nome_base = malloc((strlen(nome_arquivo) + 1) * sizeof(char));
        if (nome_base == NULL)
            return NULL;
        strcpy(nome_base, nome_arquivo);
    } else {
        // Arquivo não tem uma / ou \ em seu caminho, consequentemente o caminho passado a função
        // não possui diretórios antecedentes ao arquivo.
        nome_base = malloc((strlen(caminho_arquivo) + 1) * sizeof(char));
        if (nome_base == NULL)
            return NULL;
        strcpy(nome_base, caminho_arquivo);
    }

    // Avança até o último . no nome do arquivo.
    char *extensao = strrchr(nome_base, '.');
    // Substitui o ponto da extensão do arquivo com um caractere nulo, marcando sua posição como
    // fim da string.
    if (extensao != NULL)
        *extensao = '\0';

    return nome_base;
}

// Extrai os diretórios de uma string no formato: /diretório1/diretório2/.../nome-arquivo.ext
char *extrair_nome_diretorio(const char *caminho_arquivo) {
    if (caminho_arquivo == NULL) {
        LOG_ERROR("Caminho de arquivo nulo fornecida a extrair nome diretório!\n");
        return NULL;
    }

    char *caminho_diretorios = malloc((strlen(caminho_arquivo) + 1) * sizeof(char));
    strcpy(caminho_diretorios, caminho_arquivo);

    // Avança até a última / ou \ no caminho do arquivo.
    char *diretorios = strrchr(caminho_diretorios, SEPARADOR_DE_DIRETORIOS);

    // Substitui o último separador com '\0', marcando o fim da string.
    if (diretorios != NULL)
        *diretorios = '\0';

    return caminho_diretorios;
}

// Cria uma string baseado em um nome de arquivo passado a função, porém sua extensão é
// substituida por sufixos adicionais.
char *alterar_sufixo(const char *caminho_arquivo, int num_sufixos, ...) {
    if (caminho_arquivo == NULL) {
        LOG_ERROR("Caminho de arquivo nulo fornecida ao alterar sufixo!\n");
        return NULL;
    }

    char *sufixo_final = malloc(sizeof(char));
    sufixo_final[0] = '\0';

    va_list sufixos;
    va_start(sufixos, num_sufixos);
    // Concatena todos os sufixos passados para a função em uma string
    for (int i = 0; i < num_sufixos; i++) {
        // Recebe o próximo sufixo da lista
        char *sufixo = va_arg(sufixos, char *);
        // Aumenta o tamanho da string para receber o proximo sufixo
        char *tmp =
            realloc(sufixo_final, (strlen(sufixo_final) + strlen(sufixo) + 1) * sizeof(char));
        if (tmp == NULL) {
            LOG_ERROR("Falha ao alocar memória para novo sufixo!\n");
            free(sufixo_final);
            return NULL;
        }
        sufixo_final = tmp;
        strcat(sufixo_final, sufixo);
    }
    va_end(sufixos);

    // Recebe o nome do arquivo, sem os diretórios que compõem seu caminho e sua extensão
    char *nome_arquivo = extrair_nome_base(caminho_arquivo);

    char *novo_nome = malloc((strlen(nome_arquivo) + strlen(sufixo_final) + 1) * sizeof(char));
    sprintf(novo_nome, "%s%s", nome_arquivo, sufixo_final);

    free(nome_arquivo);
    free(sufixo_final);
    return novo_nome;
}
