#ifndef VIAS_H
#define VIAS_H

/*
Este módulo define a função vias_ler, responsável por interpretar um arquivo vias fornecido ao
programa e executar os comandos especificados.
*/

#include "../Estruturas/grafo.h"
#include "../Estruturas/tabelahash.h"
#include "../Objetos/Formas/ponto.h"

/*
Abre um arquivo de estabelecimentos comerciasis (.ec) no caminho passado a função e interpreta seus
comandos a fim de popular quadtrees com os estabelecimentos especificados.
Nenhum dos parâmetros podem ser nulos e o caminho do arquivo de descrição deve ser válido.
As quadtrees são preenchidas com os estabelcimentos descritos no arquivo.
*/
void vias_ler(const char *caminho_vias, Tabela quadtrees, Grafo vias);

#endif
