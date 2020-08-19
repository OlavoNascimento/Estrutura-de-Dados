#ifndef LER_QRY_H
#define LER_QRY_H

// Este módulo define o a função ler_qry, responsável por traduzir os comandos presentes no arquivo
// de consulta em operações nas figuras presentes em uma lista.

#include "lista.h"

// Recebe uma linha, o caminho para o arquivo de consulta e o caminho para o arquivo de log. Altera
// a lista de acordo com os comandos presentes no arquivo de consulta.
// Nenhum dos argumentos pode ser nulo.
void ler_qry(const char *caminho_qry, const char *caminho_log, Lista lista_formas,
             Lista lista_quadras, Lista lista_hidrantes, Lista lista_radios, Lista lista_semaforos);

#endif
