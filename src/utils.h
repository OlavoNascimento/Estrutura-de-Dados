#ifndef UTILS_H
#define UTILS_H

// Este módulo define funções auxiliares, em sua maioria relacionadas a caminhos de arquivos e diretórios.

void criar_diretorio(char const *diretorio);
char *unir_caminhos(const char *diretorio, const char *nome_arquivo);
char *extrair_nome_base(const char *caminho_arquivo);
char *alterar_sufixo(const char *nome_arquivo, int num_sufixos, ...);

#endif
