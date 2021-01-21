#ifndef COMERCIOS_H
#define COMERCIOS_H

/*
Este módulo define a função descricao_ler, responsável por interpretar um arquivo de descrição
fornecido ao programa e executar os comandos especificados.
*/

#include "../Estruturas/tabelahash.h"

/*
Abre um arquivo de estabelecimentos comerciasis (.ec) no caminho passado a função e interpreta seus
comandos afim de popular quadtrees com os estabelecimentos especificados.
Nenhum dos parâmetros podem ser nulos e o caminho do arquivo de descrição deve ser válido.
As quadtrees são preenchidas com os estabelcimentos descritos no arquivo.
*/
void comercios_ler(const char *caminho_comercios, Tabela quadtrees, Tabela relacoes);

#endif
