#pragma once

#include <stdbool.h>

// Parâmetros reconhecidos pelo programa.
typedef struct {
    char *nome_dir_entrada;
    char *nome_descricao;
    char *nome_dir_saida;
    char *nome_consulta;
} Parametros;

Parametros ler_parametros(int argc, const char *argv[]);
bool checar_parametros_obrigatorios(const Parametros params);

char *preparar_caminho(const char *dir_entrada, const char *nome_arquivo);
char *extrair_nome_base(const char *caminho_arquivo);
char *alterar_sufixo(const char *nome_arquivo, int num_sufixos, ...);

void destruir_parametros(Parametros params);
