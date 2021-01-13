#include "parametros.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "caminhos.h"
#include "logging.h"

typedef struct {
    char *nome_dir_entrada;
    char *caminho_descricao;
    char *caminho_consulta;
    char *caminho_estabelecimentos;
    char *caminho_moradores;
    char *nome_dir_saida;
} ParametrosImp;

// Retorna uma struct com todos os parâmetros que podem ser lidos pelo programa.
Parametros parametros_criar() {
    ParametrosImp *params = malloc(sizeof(ParametrosImp));
    params->nome_dir_entrada = NULL;
    params->caminho_descricao = NULL;
    params->caminho_consulta = NULL;
    params->caminho_estabelecimentos = NULL;
    params->caminho_moradores = NULL;
    params->nome_dir_saida = NULL;
    return params;
}

// Retorna uma struct contendo os parâmetros fornecidos ao programa.
Parametros parametros_ler(int argc, const char *argv[]) {
    ParametrosImp *paramsImp = (ParametrosImp *) parametros_criar();
    int i = 0;
    while (i < argc) {
        if (strcmp("-e", argv[i]) == 0) {
            i++;
            paramsImp->nome_dir_entrada = malloc((strlen(argv[i]) + 1) * sizeof(char));
            strcpy(paramsImp->nome_dir_entrada, argv[i]);
        } else if (strcmp("-f", argv[i]) == 0) {
            i++;
            paramsImp->caminho_descricao = malloc((strlen(argv[i]) + 1) * sizeof(char));
            strcpy(paramsImp->caminho_descricao, argv[i]);
        } else if (strcmp("-o", argv[i]) == 0) {
            i++;
            paramsImp->nome_dir_saida = malloc((strlen(argv[i]) + 1) * sizeof(char));
            strcpy(paramsImp->nome_dir_saida, argv[i]);
        } else if (strcmp("-q", argv[i]) == 0) {
            i++;
            paramsImp->caminho_consulta = malloc((strlen(argv[i]) + 1) * sizeof(char));
            strcpy(paramsImp->caminho_consulta, argv[i]);
        } else if (strcmp("-ec", argv[i]) == 0) {
            i++;
            paramsImp->caminho_estabelecimentos = malloc((strlen(argv[i]) + 1) * sizeof(char));
            strcpy(paramsImp->caminho_estabelecimentos, argv[i]);
        }
        i++;
    }
    return paramsImp;
}

// Retorna verdadeiro caso os parâmetros buscados pela função existam.
bool parametros_checar_obrigatorios(const Parametros params) {
    ParametrosImp *paramsImp = (ParametrosImp *) params;
    if (paramsImp->caminho_descricao == NULL) {
        fprintf(stderr, "ERRO: Parâmetro obrigatório -f (arquivo .geo) não foi fornecido!\n");
        return false;
    }

    if (paramsImp->nome_dir_saida == NULL) {
        fprintf(stderr, "ERRO: Parâmetro obrigatório -o (diretório de saída) não foi fornecido!\n");
        return false;
    }
    return true;
}

char *parametros_obter_diretorio_saida(const Parametros params) {
    ParametrosImp *paramsImp = (ParametrosImp *) params;
    return paramsImp->nome_dir_saida;
}

char *parametros_obter_caminho_descricao(const Parametros params) {
    ParametrosImp *paramsImp = (ParametrosImp *) params;
    if (paramsImp->caminho_descricao == NULL)
        return NULL;

    char *caminho_descricao = NULL;
    // Adiciona o diretório de entrada ao caminho do arquivo de descrição caso necessário.
    if (paramsImp->nome_dir_entrada != NULL) {
        // Concatena o diretório ao caminho do arquivo.
        caminho_descricao =
            unir_caminhos(paramsImp->nome_dir_entrada, paramsImp->caminho_descricao);
    } else {
        caminho_descricao = malloc((strlen(paramsImp->caminho_descricao) + 1) * sizeof(char));
        strcpy(caminho_descricao, paramsImp->caminho_descricao);
    }

    return caminho_descricao;
}

char *parametros_obter_caminho_consulta(const Parametros params) {
    ParametrosImp *paramsImp = (ParametrosImp *) params;
    if (paramsImp->caminho_consulta == NULL)
        return NULL;

    char *caminho_consulta = NULL;
    // Adiciona o diretório de entrada ao caminho do arquivo de consulta caso necessário.
    if (paramsImp->nome_dir_entrada != NULL) {
        // Concatena o diretório ao caminho do arquivo.
        caminho_consulta = unir_caminhos(paramsImp->nome_dir_entrada, paramsImp->caminho_consulta);
    } else {
        caminho_consulta = malloc((strlen(paramsImp->caminho_consulta) + 1) * sizeof(char));
        strcpy(caminho_consulta, paramsImp->caminho_consulta);
    }

    return caminho_consulta;
}

char *parametros_obter_caminho_estabelecimentos(const Parametros params) {
    ParametrosImp *paramsImp = (ParametrosImp *) params;
    if (paramsImp->caminho_estabelecimentos == NULL)
        return NULL;

    char *caminho_estabelecimentos = NULL;
    // Adiciona o diretório de entrada ao caminho do arquivo de estabelecimentos caso necessário.
    if (paramsImp->nome_dir_entrada != NULL) {
        // Concatena o diretório ao caminho do arquivo.
        caminho_estabelecimentos =
            unir_caminhos(paramsImp->nome_dir_entrada, paramsImp->caminho_estabelecimentos);
    } else {
        caminho_estabelecimentos =
            malloc((strlen(paramsImp->caminho_estabelecimentos) + 1) * sizeof(char));
        strcpy(caminho_estabelecimentos, paramsImp->caminho_estabelecimentos);
    }

    return caminho_estabelecimentos;
}

char *parametros_obter_caminho_moradores(const Parametros params) {
    ParametrosImp *paramsImp = (ParametrosImp *) params;
    if (paramsImp->caminho_moradores == NULL)
        return NULL;

    char *caminho_moradores = NULL;
    // Adiciona o diretório de entrada ao caminho do arquivo de moradores caso necessário.
    if (paramsImp->nome_dir_entrada != NULL) {
        // Concatena o diretório ao caminho do arquivo.
        caminho_moradores =
            unir_caminhos(paramsImp->nome_dir_entrada, paramsImp->caminho_moradores);
    } else {
        caminho_moradores = malloc((strlen(paramsImp->caminho_moradores) + 1) * sizeof(char));
        strcpy(caminho_moradores, paramsImp->caminho_moradores);
    }

    return caminho_moradores;
}

// Usa o nome do arquivo de descrição para criar o nome do svg.
char *parametros_obter_caminho_svg_descricao(const Parametros params) {
    ParametrosImp *paramsImp = (ParametrosImp *) params;
    if (paramsImp->caminho_descricao == NULL || paramsImp->nome_dir_saida == NULL)
        return NULL;

    // Extrai o nome base do arquivo e altera sua extensão
    char *nome_svg_descricao = alterar_sufixo(paramsImp->caminho_descricao, 1, ".svg");
    char *caminho_svg_descricao = unir_caminhos(paramsImp->nome_dir_saida, nome_svg_descricao);
    free(nome_svg_descricao);
    return caminho_svg_descricao;
}

// Cria o caminho do arquivo svg de consulta utilizando o seguinte formato:
// arquivoDescrição-arquivoConsulta.svg
char *parametros_obter_caminho_svg_consulta(const Parametros params) {
    ParametrosImp *paramsImp = (ParametrosImp *) params;
    if (paramsImp->caminho_descricao == NULL || paramsImp->caminho_consulta == NULL ||
        paramsImp->nome_dir_saida == NULL)
        return NULL;

    // Extrai o nome base da consulta atual
    char *nome_base_consulta = extrair_nome_base(paramsImp->caminho_consulta);
    // Adiciona o nome da consulta como sufixo do nome do registro
    char *nome_svg_consulta =
        alterar_sufixo(paramsImp->caminho_descricao, 3, "-", nome_base_consulta, ".svg");
    char *caminho_svg_consulta = unir_caminhos(paramsImp->nome_dir_saida, nome_svg_consulta);
    free(nome_base_consulta);
    free(nome_svg_consulta);
    return caminho_svg_consulta;
}

// Cria o caminho do arquivo txt de consulta utilizando o seguinte formato:
// arquivoDescrição-arquivoConsulta.txt
char *parametros_obter_caminho_registro_consulta(const Parametros params) {
    ParametrosImp *paramsImp = (ParametrosImp *) params;
    if (paramsImp->caminho_descricao == NULL || paramsImp->caminho_consulta == NULL ||
        paramsImp->nome_dir_saida == NULL)
        return NULL;

    // Extrai o nome base da consulta atual
    char *nome_base_consulta = extrair_nome_base(paramsImp->caminho_consulta);
    // Adiciona o nome da consulta como sufixo do nome do registro
    char *nome_registro_consulta =
        alterar_sufixo(paramsImp->caminho_descricao, 3, "-", nome_base_consulta, ".txt");
    char *caminho_registro_consulta =
        unir_caminhos(paramsImp->nome_dir_saida, nome_registro_consulta);
    free(nome_base_consulta);
    free(nome_registro_consulta);
    return caminho_registro_consulta;
}

// Libera a memória alocada pelos argumentos.
void parametros_destruir(Parametros params) {
    ParametrosImp *paramsImp = (ParametrosImp *) params;
    free(paramsImp->caminho_descricao);
    free(paramsImp->nome_dir_entrada);
    free(paramsImp->nome_dir_saida);
    free(paramsImp->caminho_consulta);
    free(params);
}
