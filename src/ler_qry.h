#pragma once

#include <lista.h>

FILE* criar_arquivo_log(char *dir_saida, char *caminho_geo);
void ler_qry(Lista *lista, char *caminho_qry, FILE *log);