#ifndef DESCRICAO_H
#define DESCRICAO_H

/*
Este módulo define a função descricao_ler, responsável por interpretar um arquivo de descrição
fornecido ao programa e executar os comandos especificados.
*/

#include "../Estruturas/quadtree.h"

/*
Abre um arquivo .geo no caminho passado a função e interpreta seus comando afim de popular várias
quadtrees com os elementos especificados no arquivo.
Nenhum dos parâmetros podem ser nulos e o caminho do arquivo de descrição deve ser válido.
A listas são preenchidas com as figuras e equipamentos urbanos descritos no arquivo.
*/
void descricao_ler(const char *caminho_descricao, QuadTree formas, QuadTree quadras,
                   QuadTree hidrantes, QuadTree radios, QuadTree semaforos, QuadTree postos,
                   QuadTree densidades);

#endif
