#pragma once

typedef struct {
    char *nome_dir_entrada;
    char *nome_arq_descricao;
    char *nome_dir_saida;
    char *nome_arq_consulta;
} Parametros;

Parametros ler_parametros(int argc, const char *argv[]);
void destruir_parametros(Parametros params);
