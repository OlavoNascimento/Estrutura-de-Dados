#ifndef FASE_3_H
#define FASE_3_H

/*
Este módulo define as funções de consulta que pertecem a terceira fase do projeto.
*/

#include <stdio.h>

#include "../../Estruturas/lista.h"
#include "../../Estruturas/quadtree.h"
#include "../../Estruturas/tabelahash.h"

/*
Executa o comando cv especificado no arquivo de consulta, adicionando n casos em um endereço.
*/
void adicionar_caso(QuadTree casos, Tabela cep_quadra, const char *linha);

/*
Executa o comando soc especificado no arquivo de consulta, determinando o caminho mais próximo para
n postos.
*/
void postos_mais_proximos(QuadTree postos, Tabela cep_quadra, Lista formas, const char *linha,
                          FILE *arquivo_log);

/*
Executa o comando ci especificado no arquivo de consulta, utiliza um círculo para definir os casos
que devem ser contidos por uma envoltória convexa.
*/
void determinar_regiao_de_incidencia(Lista formas, QuadTree casos, QuadTree postos,
                                     Lista densidades, const char *linha, FILE *arquivo_log);

#endif
