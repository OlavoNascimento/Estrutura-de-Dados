#ifndef CAMINHOS_H
#define CAMINHOS_H

/*
Este módulo define funções auxiliares relacionadas a caminhos de arquivos e diretórios.
*/

/*
Cria todos os diretórios especificados em um caminho.
O parâmetro diretorio não pode ser nulo.
*/
void criar_diretorio(const char *diretorio);

/*
Une um diretório e um arquivo em um caminho.
Nenhum dos parâmetros podem ser nulos.
Retorna uma string contendo o caminho do diretório e do arquivo concatenados, utilizando o
separador de arquivos padrão do sistema. O usuário é responsável por liberar a memória alocada!
*/
char *unir_caminhos(const char *diretorio, const char *nome_arquivo);

/*
Recebe um caminho para um arquivo e extrai apenas o nome do arquivo sem sua extensão e diretórios.
O parâmetro caminho_arquivo não pode ser nulo.
Retorna uma string contendo o nome do arquivo presente no caminho passado. O usuário é responsável
por liberar a memória alocada!
*/
char *extrair_nome_base(const char *caminho_arquivo);

/*
Recebe um caminho para um arquivo e extrai apenas os diretórios que compõem o caminho.
O parâmetro caminho_arquivo não pode ser nulo.
Retorna uma string contendo os diretórios do caminho. O usuário é responsável por liberar a memória
alocada!
*/
char *extrair_nome_diretorio(const char *caminho_arquivo);

/*
Recebe um caminho de um arquivo, um número de sufixos e um número variável de sufixos que
substituem a extensão do arquivo. O parâmetro caminho_arquivo não pode ser nulo. Retorna uma
string contendo o nome do arquivo com sua extensão substituida pelos sufixos passados. O usuário
é responsável por liberar a memória alocada!
*/
char *alterar_extensao(const char *caminho_arquivo, int num_sufixos, ...);

#endif
