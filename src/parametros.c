#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

typedef struct {
    char *nome_dir_entrada;
    char *caminho_descricao;
    char *caminho_consulta;
    char *nome_dir_saida;
} Parametros;

// Retorna uma struct com todos os parâmetros que podem ser lidos pelo programa.
Parametros parametros_criar() {
    Parametros params = {
        .nome_dir_entrada = NULL,
        .caminho_descricao = NULL,
        .caminho_consulta = NULL,
        .nome_dir_saida = NULL,
    };
    return params;
}

// Retorna uma struct contendo os parâmetros fornecidos ao programa.
Parametros parametros_ler(int argc, const char *argv[]) {
    Parametros params = parametros_criar();
    int i = 0;
    while (i < argc) {
        if (strcmp("-e", argv[i]) == 0) {
            i++;
            params.nome_dir_entrada = malloc(
                (strlen(argv[i]) + 1) * sizeof(char));
            strcpy(params.nome_dir_entrada, argv[i]);
        } else if (strcmp("-f", argv[i]) == 0) {
            i++;
            params.caminho_descricao = malloc(
                (strlen(argv[i]) + 1) * sizeof(char));
            strcpy(params.caminho_descricao, argv[i]);
        } else if (strcmp("-o", argv[i]) == 0) {
            i++;
            params.nome_dir_saida = malloc(
                (strlen(argv[i]) + 1) * sizeof(char));
            strcpy(params.nome_dir_saida, argv[i]);
        } else if (strcmp("-q", argv[i]) == 0) {
            i++;
            params.caminho_consulta = malloc(
                (strlen(argv[i]) + 1) * sizeof(char));
            strcpy(params.caminho_consulta, argv[i]);
        }
        i++;
    }
    return params;
}

// Retorna verdadeiro caso os parâmetros buscados pela função existam.
bool checar_obrigatorios(const Parametros params) {
    if (params.caminho_descricao == NULL) {
        fprintf(
            stderr,
            "Parâmetro obrigatório -f (arquivo .geo) não foi fornecido!\n");
        return false;
    }

    if (params.nome_dir_saida == NULL) {
        fprintf(
            stderr,
            "Parâmetro obrigatório -o (diretório de saída) não foi fornecido!\n");
        return false;
    }
    return true;
}

char *obter_diretorio_saida(const Parametros params) {
    return params.nome_dir_saida;
}

char *obter_caminho_descricao(const Parametros params) {
    char *caminho_descricao = NULL;
    // Adiciona o diretório de entrada ao caminho do arquivo de descrição caso necessário.
    if (params.nome_dir_entrada != NULL) {
        // Concatena o diretório ao caminho do arquivo.
        caminho_descricao = unir_caminhos(params.nome_dir_entrada, params.caminho_descricao);
    } else {
        caminho_descricao = malloc(strlen(params.caminho_descricao) * sizeof(char));
        strcpy(caminho_descricao, params.caminho_descricao);
    }

    return caminho_descricao;
}

char *obter_caminho_consulta(const Parametros params) {
    char *caminho_consulta = NULL;
    // Adiciona o diretório de entrada ao caminho do arquivo de descrição caso necessário.
    if (params.nome_dir_entrada != NULL) {
        // Concatena o diretório ao caminho do arquivo.
        caminho_consulta = unir_caminhos(params.nome_dir_entrada, params.caminho_consulta);
    } else {
        caminho_consulta = malloc(strlen(params.caminho_consulta) * sizeof(char));
        strcpy(caminho_consulta, params.caminho_consulta);
    }

    return caminho_consulta;
}

// Usa o nome do arquivo de descrição para criar o nome do svg.
char *obter_caminho_svg_descricao(const Parametros params) {
    // Extrai o nome base do arquivo e altera sua extensão
    char *nome_svg_descricao = alterar_sufixo(params.caminho_descricao, 1, ".svg");
    char *caminho_svg_descricao = unir_caminhos(params.nome_dir_saida, nome_svg_descricao);
    free(nome_svg_descricao);
    return caminho_svg_descricao;
}

// Cria o caminho do arquivo svg de consulta utilizando o seguinte formato: arquivoDescrição-arquivoConsulta.svg
char *obter_caminho_svg_consulta(const Parametros params) {
    // Extrai o nome base da consulta atual
    char *nome_base_consulta = extrair_nome_base(params.caminho_consulta);
    // Adiciona o nome da consulta como sufixo do nome do registro
    char *nome_svg_consulta = alterar_sufixo(params.caminho_descricao, 3, "-", nome_base_consulta, ".svg");
    char *caminho_svg_consulta = unir_caminhos(params.nome_dir_saida, nome_svg_consulta);
    free(nome_base_consulta);
    free(nome_svg_consulta);
    return caminho_svg_consulta;
}

// Cria o caminho do arquivo txt de consulta utilizando o seguinte formato: arquivoDescrição-arquivoConsulta.txt
char *obter_caminho_registro_consulta(const Parametros params) {
    // Extrai o nome base da consulta atual
    char *nome_base_consulta = extrair_nome_base(params.caminho_consulta);
    // Adiciona o nome da consulta como sufixo do nome do registro
    char *nome_registro_consulta = alterar_sufixo(params.caminho_descricao, 3, "-", nome_base_consulta, ".txt");
    char *caminho_registro_consulta = unir_caminhos(params.nome_dir_saida, nome_registro_consulta);
    free(nome_base_consulta);
    free(nome_registro_consulta);
    return caminho_registro_consulta;
}

// Libera a memória alocada pelos argumentos.
void destruir_parametros(Parametros params) {
    free(params.caminho_descricao);
    free(params.nome_dir_entrada);
    free(params.nome_dir_saida);
    free(params.caminho_consulta);
}
