#pragma once

typedef struct {
    char *nome_dir_entrada;
    char *nome_descricao;
    char *nome_dir_saida;
    char *nome_consulta;
} Parametros;

Parametros ler_parametros(int argc, const char *argv[]);
char* preparar_caminho(char* dir_entrada, char* nome_arquivo);
char* preparar_caminho_sufixo(char* diretorio, char* nome_arquivo, char* sufixo);
void destruir_parametros(Parametros params);
