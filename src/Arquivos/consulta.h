#ifndef CONSULTA_H
#define CONSULTA_H

/*
Este módulo define a função consulta_ler, responsável por traduzir os comandos presentes no arquivo
de consulta em operações nas figuras armazenadas em listas.
*/

#include "../Estruturas/quadtree.h"

/*
Lê um arquivo de consulta (.qry) e aplica os seus comandos nas figuras apropriadas.
Nenhum dos argumentos pode ser nulo e o caminho do arquivo de consulta deve ser válido.
Altera as listas de acordo com os comandos presentes no arquivo de consulta.
*/
void consulta_ler(const char *caminho_qry, const char *caminho_log, QuadTree formas,
                  QuadTree quadras, QuadTree hidrantes, QuadTree radios, QuadTree semaforos,
                  QuadTree postos, QuadTree densidades, QuadTree casos);

#endif
