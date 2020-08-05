#pragma once

#include <stdbool.h>

// Este módulo define o tipo Parametros, responsável por ler e armazenar os parâmetros do programa.
// Assim como criar os caminhos finais destes arquivos com base nos seus dados.

typedef void *Parametros;

// Recebe a quantidade de argumentos e os argumentos passados ao programa, armazenando os valores no
// valor do struct interno. O parâmetro argv não pode ser nulo.
// O usuário é responsável por liberar a memória alocada pela struct!
Parametros parametros_ler(int argc, const char *argv[]);
// Avalia os argumentos fornecidos ao programa, caso algum argumento obrigatório não seja encontrado
// retorna falso.
bool parametros_checar_obrigatorios(const Parametros params);

// Retorna o caminho do diretório de saída armazenado.
char *parametros_obter_diretorio_saida(const Parametros params);
// Retorna o caminho do arquivo de descrição(.geo).
char *parametros_obter_caminho_descricao(const Parametros params);
// Retorna o caminho do arquivo de consulta(.qry).
char *parametros_obter_caminho_consulta(const Parametros params);

// Retorna o caminho do arquivo svg criado como saída do arquivo de descrição(.geo).
// O usuário é responsável por liberar a memória alocada!
char *parametros_obter_caminho_svg_descricao(const Parametros params);
// Retorna o caminho do arquivo svg criado como saída do arquivo de consulta(.qry).
// O usuário é responsável por liberar a memória alocada!
char *parametros_obter_caminho_svg_consulta(const Parametros params);
// Retorna o caminho do arquivo txt criado como saída do arquivo de consulta(.qry).
// O usuário é responsável por liberar a memória alocada!
char *parametros_obter_caminho_registro_consulta(const Parametros params);

// Libera a memória alocada pelos parâmetros.
void parametros_destruir(Parametros params);
