#ifndef LER_GEO_H
#define LER_GEO_H

/*
Este módulo define a função ler_geo, responsável por interpretar um arquivo de descrição (.geo)
fornecido ao programa e executar os comandos especificados.
*/

#include "lista.h"

/*
Abre um arquivo .geo no caminho passado a função e interpreta seus comando afim de criar uma
lista com os elementos especificados no arquivo.
Nenhum dos parâmetros podem ser nulos e o caminho do arquivo de descrição deve ser válido.
A listas são preenchidas com as figuras e equipamentos urbanos descritos no arquivo.
*/
void ler_geo(const char *caminho_geo, Lista lista, Lista lista_quadras, Lista lista_hidrantes,
             Lista lista_radios, Lista lista_semaforos, Lista lista_postos, Lista lista_densidades);

#endif
