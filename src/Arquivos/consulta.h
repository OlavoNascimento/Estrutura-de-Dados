#ifndef CONSULTA_H
#define CONSULTA_H

/*
Este módulo define a função consulta_ler, responsável por traduzir os comandos presentes no arquivo
de consulta em operações nas figuras armazenadas em listas.
*/

#include "../Estruturas/tabelahash.h"

/*
Lê um arquivo de consulta (.qry) e aplica os seus comandos nas figuras apropriadas.
Nenhum dos argumentos pode ser nulo e o caminho do arquivo de consulta deve ser válido.
Altera as quadtrees de acordo com os comandos presentes no arquivo de consulta.
*/
void consulta_ler(const char *caminho_consulta, const char *caminho_log, Tabela quadtrees,
                  Tabela relacoes);

#endif
