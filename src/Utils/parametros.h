#ifndef PARAMETROS_H
#define PARAMETROS_H

/*
Este módulo define o tipo Parametros, responsável por ler e armazenar os parâmetros do programa.
Assim como criar os caminhos finais destes arquivos com base nos seus dados.
*/

#include <stdbool.h>

typedef void *Parametros;

/*
Recebe a quantidade de argumentos e os argumentos passados ao programa.
O parâmetro argv não pode ser nulo.
O usuário é responsável por liberar a memória alocada pela
*/
Parametros parametros_ler(int argc, const char *argv[]);

/*
Avalia os argumentos fornecidos ao programa, caso algum argumento obrigatório não seja encontrado
retorna falso.
O parâmetro params não pode ser nulo.
Retorna verdadeiro caso todos os parâmetros obrigatórios tenham sidos fornecidos. Falso caso o
contrário.
*/
bool parametros_checar_obrigatorios(const Parametros params);

/*
Obtém o diretório de saída passado como argumento para o programa.
O parâmetro params não pode ser nulo.
Retorna o caminho do diretório de saída armazenado.
*/
char *parametros_obter_diretorio_saida(const Parametros params);

/*
Obtém o caminho do arquivo de descrição(.geo) passado como argumento para o programa. O caminho é
concatenado ao diretório de entrada caso necessário.
O parâmetro params não pode ser nulo.
Retorna o caminho do arquivo de descrição(.geo) armazenado. O usuário é responsável por liberar a
memória alocada!
*/
char *parametros_obter_caminho_descricao(const Parametros params);

/*
Obtém o caminho do arquivo de consulta (.qry) passado como argumento para o programa. O caminho é
concatenado ao diretório de entrada caso necessário.
O parâmetro params não pode ser nulo.
Retorna o caminho do arquivo de consulta (.qry) armazenado. O usuário é responsável por liberar a
memória alocada!
*/
char *parametros_obter_caminho_consulta(const Parametros params);

/*
Obtém o caminho do arquivo de estabelecimentos (.ec) passado como argumento para o programa. O
caminho é concatenado ao diretório de entrada caso necessário.
O parâmetro params não pode ser nulo.
Retorna o caminho do arquivo de estabelecimentos (.ec) armazenado. O usuário é responsável por
liberar a memória alocada!
*/
char *parametros_obter_caminho_estabelecimentos(const Parametros params);

/*
Obtém o caminho do arquivo svg criado como saída do arquivo de descrição (.geo). O caminho é
concatenado ao diretório de saída.
O parâmetro params não pode ser nulo.
Retorna o caminho do arquivo svg de descrição concatenado ao diretório de saída. O usuário é
responsável por liberar a memória alocada!
*/
char *parametros_obter_caminho_svg_descricao(const Parametros params);

/*
Obtém o caminho do arquivo svg criado como saída do arquivo de consulta (.qry). O caminho é
concatenado ao diretório de saída.
O parâmetro params não pode ser nulo.
Retorna o caminho do arquivo svg de consulta concatenado ao diretório de saída. O usuário é
responsável por liberar a memória alocada!
*/
char *parametros_obter_caminho_svg_consulta(const Parametros params);

/*
Obtém o caminho do arquivo txt criado como saída do arquivo de consulta (.qry). O caminho é
concatenado ao diretório de saída.
O parâmetro params não pode ser nulo.
Retorna o caminho do arquivo txt de consulta concatenado ao diretório de saída. O usuário é
responsável por liberar a memória alocada!
*/
char *parametros_obter_caminho_registro_consulta(const Parametros params);

/*
Libera a memória alocada pelos parametros.
O parâmetro params não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada. Esses valores não poderão ser utilizados após isso!
*/
void parametros_destruir(Parametros params);

#endif
