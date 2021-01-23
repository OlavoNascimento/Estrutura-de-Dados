#ifndef PESSOAS_H
#define PESSOAS_H

#include "../Estruturas/tabelahash.h"

/*
Abre um arquivo .pm no caminho passado a função e interpreta seus comando a fim de criar moradores.
Nenhum dos parâmetros podem ser nulos e o caminho do arquivo de descrição deve ser válido.
Tabelas são preenchidas com moradores.
*/
void pessoas_ler(const char *caminho_descricao_moradores, Tabela quadtrees, Tabela relacoes);

#endif
