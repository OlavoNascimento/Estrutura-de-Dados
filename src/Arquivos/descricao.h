#ifndef DESCRICAO_H
#define DESCRICAO_H

/*
Este módulo define a função descricao_ler, responsável por interpretar um arquivo de descrição
fornecido ao programa e executar os comandos especificados.
*/

#include "../Estruturas/lista.h"

/*
Abre um arquivo .geo no caminho passado a função e interpreta seus comando afim de criar uma lista
com os elementos especificados no arquivo.
Nenhum dos parâmetros podem ser nulos e o caminho do arquivo de descrição deve ser válido.
A listas são preenchidas com as figuras e equipamentos urbanos descritos no arquivo.
*/
void descricao_ler(const char *caminho_geo, Lista lista, Lista lista_quadras, Lista lista_hidrantes,
                   Lista lista_radios, Lista lista_semaforos, Lista lista_postos,
                   Lista lista_densidades);

#endif
