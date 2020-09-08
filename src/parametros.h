#ifndef PARAMETROS_H
#define PARAMETROS_H

#include <stdbool.h>

/*
Este módulo define o tipo Parametros, responsável por ler e armazenar os parâmetros do programa.
Assim como criar os caminhos finais destes arquivos com base nos seus dados.
*/

typedef void *Parametros;

/*
Recebe a quantidade de argumentos e os argumentos passados ao programa, armazenando os valores no
valor do struct interno. O parâmetro argv não pode ser nulo.
O parâmetro argv não pode ser nulo.
Retorna o struct Parametros inicializado. O usuário é responsável por liberar a memória alocada pela
struct!
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
Retorna o caminho do diretório de saída armazenado. O usuário é responsável por liberar a memória
alocada!
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
O parâmetro params não pode ser nulo. Retorna o
caminho do arquivo de consulta (.qry) armazenado. O usuário é responsável por liberar a memória
alocada!
*/
char *parametros_obter_caminho_consulta(const Parametros params);

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
Libera a memória alocada pelos dados de um struct Parametros.
O parâmetro params não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada pelos valores do struct. Esses valores não poderão ser utilizados após
isso!
*/
void parametros_destruir(Parametros params);

#endif
