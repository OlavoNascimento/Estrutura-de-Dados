#include "parametros.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "caminhos.h"
#include "logging.h"

struct Parametros_s {
    char *nome_dir_entrada;
    char *caminho_descricao;
    char *caminho_consulta;
    char *caminho_estabelecimentos;
    char *caminho_moradores;
    char *caminho_vias;
    char *nome_dir_saida;
};

// Retorna uma struct com todos os parâmetros que podem ser lidos pelo programa.
Parametros parametros_criar() {
    Parametros params = malloc(sizeof *params);
    if (params == NULL) {
        LOG_ERRO("Falha ao alocar memória para parâmetros!\n");
        return NULL;
    }
    params->nome_dir_entrada = NULL;
    params->caminho_descricao = NULL;
    params->caminho_consulta = NULL;
    params->caminho_estabelecimentos = NULL;
    params->caminho_moradores = NULL;
    params->caminho_vias = NULL;
    params->nome_dir_saida = NULL;
    return params;
}

// Retorna uma struct contendo os parâmetros fornecidos ao programa.
Parametros parametros_ler(int argc, const char *argv[]) {
    Parametros params = parametros_criar();
    int i = 0;
    while (i < argc) {
        if (strcmp("-e", argv[i]) == 0) {
            i++;
            params->nome_dir_entrada =
                malloc((strlen(argv[i]) + 1) * sizeof *params->nome_dir_entrada);
            strcpy(params->nome_dir_entrada, argv[i]);
        } else if (strcmp("-f", argv[i]) == 0) {
            i++;
            params->caminho_descricao =
                malloc((strlen(argv[i]) + 1) * sizeof *params->caminho_descricao);
            strcpy(params->caminho_descricao, argv[i]);
        } else if (strcmp("-o", argv[i]) == 0) {
            i++;
            params->nome_dir_saida = malloc((strlen(argv[i]) + 1) * sizeof *params->nome_dir_saida);
            strcpy(params->nome_dir_saida, argv[i]);
        } else if (strcmp("-q", argv[i]) == 0) {
            i++;
            params->caminho_consulta =
                malloc((strlen(argv[i]) + 1) * sizeof *params->caminho_consulta);
            strcpy(params->caminho_consulta, argv[i]);
        } else if (strcmp("-ec", argv[i]) == 0) {
            i++;
            params->caminho_estabelecimentos =
                malloc((strlen(argv[i]) + 1) * sizeof *params->caminho_estabelecimentos);
            strcpy(params->caminho_estabelecimentos, argv[i]);
        } else if (strcmp("-pm", argv[i]) == 0) {
            i++;
            params->caminho_moradores =
                malloc((strlen(argv[i]) + 1) * sizeof *params->caminho_moradores);
            strcpy(params->caminho_moradores, argv[i]);
        } else if (strcmp("-v", argv[i]) == 0) {
            i++;
            params->caminho_vias = malloc((strlen(argv[i]) + 1) * sizeof *params->caminho_vias);
            strcpy(params->caminho_vias, argv[i]);
        }
        i++;
    }
    return params;
}

// Retorna verdadeiro caso os parâmetros buscados pela função existam.
bool parametros_checar_obrigatorios(const Parametros params) {
    if (params->caminho_descricao == NULL) {
        LOG_ERRO("Parâmetro obrigatório -f (arquivo .geo) não foi fornecido!\n");
        return false;
    }
    if (params->nome_dir_saida == NULL) {
        LOG_ERRO("Parâmetro obrigatório -o (diretório de saída) não foi fornecido!\n");
        return false;
    }
    return true;
}

char *parametros_obter_diretorio_saida(const Parametros params) {
    return params->nome_dir_saida;
}

// Adiciona o caminho da pasta de entrada caso necessário.
char *concatenar_caminho_entrada(const Parametros params, const char *caminho_original) {
    if (caminho_original == NULL)
        return NULL;

    char *caminho = NULL;
    // Adiciona o diretório de entrada ao caminho do arquivo de consulta caso necessário.
    if (params->nome_dir_entrada != NULL) {
        // Concatena o diretório ao caminho do arquivo.
        caminho = unir_caminhos(params->nome_dir_entrada, caminho_original);
    } else {
        caminho = malloc((strlen(caminho_original) + 1) * sizeof *caminho);
        strcpy(caminho, caminho_original);
    }
    return caminho;
}

char *parametros_obter_caminho_descricao(const Parametros params) {
    return concatenar_caminho_entrada(params, params->caminho_descricao);
}

char *parametros_obter_caminho_consulta(const Parametros params) {
    return concatenar_caminho_entrada(params, params->caminho_consulta);
}

char *parametros_obter_caminho_estabelecimentos(const Parametros params) {
    return concatenar_caminho_entrada(params, params->caminho_estabelecimentos);
}

char *parametros_obter_caminho_moradores(const Parametros params) {
    return concatenar_caminho_entrada(params, params->caminho_moradores);
}

char *parametros_obter_caminho_vias(const Parametros params) {
    return concatenar_caminho_entrada(params, params->caminho_vias);
}

// Usa o nome do arquivo de descrição para criar o nome do svg.
char *parametros_obter_caminho_svg_descricao(const Parametros params) {
    if (params->caminho_descricao == NULL || params->nome_dir_saida == NULL)
        return NULL;

    // Extrai o nome base do arquivo e altera sua extensão
    char *nome_svg_descricao = alterar_extensao(params->caminho_descricao, 1, ".svg");
    char *caminho_svg_descricao = unir_caminhos(params->nome_dir_saida, nome_svg_descricao);
    free(nome_svg_descricao);
    return caminho_svg_descricao;
}

// Cria o caminho do arquivo svg de consulta utilizando o seguinte formato:
// arquivoDescrição-arquivoConsulta.svg
char *parametros_obter_caminho_svg_consulta(const Parametros params) {
    if (params->caminho_descricao == NULL || params->caminho_consulta == NULL ||
        params->nome_dir_saida == NULL)
        return NULL;

    // Extrai o nome base da consulta atual
    char *nome_base_consulta = extrair_nome_base(params->caminho_consulta);
    // Adiciona o nome da consulta como sufixo do nome do registro
    char *nome_svg_consulta =
        alterar_extensao(params->caminho_descricao, 3, "-", nome_base_consulta, ".svg");
    char *caminho_svg_consulta = unir_caminhos(params->nome_dir_saida, nome_svg_consulta);
    free(nome_base_consulta);
    free(nome_svg_consulta);
    return caminho_svg_consulta;
}

// Cria o caminho do arquivo txt de consulta utilizando o seguinte formato:
// arquivoDescrição-arquivoConsulta.txt
char *parametros_obter_caminho_registro_consulta(const Parametros params) {
    if (params->caminho_descricao == NULL || params->caminho_consulta == NULL ||
        params->nome_dir_saida == NULL)
        return NULL;

    // Extrai o nome base da consulta atual
    char *nome_base_consulta = extrair_nome_base(params->caminho_consulta);
    // Adiciona o nome da consulta como sufixo do nome do registro
    char *nome_registro_consulta =
        alterar_extensao(params->caminho_descricao, 3, "-", nome_base_consulta, ".txt");
    char *caminho_registro_consulta = unir_caminhos(params->nome_dir_saida, nome_registro_consulta);
    free(nome_base_consulta);
    free(nome_registro_consulta);
    return caminho_registro_consulta;
}

// Libera a memória alocada pelos argumentos.
void parametros_destruir(Parametros params) {
    free(params->nome_dir_entrada);
    free(params->caminho_descricao);
    free(params->caminho_consulta);
    free(params->caminho_moradores);
    free(params->caminho_vias);
    free(params->caminho_estabelecimentos);
    free(params->nome_dir_saida);
    free(params);
}
