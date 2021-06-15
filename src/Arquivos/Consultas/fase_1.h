#ifndef FASE_1_H
#define FASE_1_H

/*
Este módulo define as funções de consulta que pertecem a primeira fase do projeto.
*/

#include <stdio.h>

#include "../../Estruturas/lista.h"
#include "../../Estruturas/tabelahash.h"

/*
Executa o comando o? especificado no arquivo de consulta, verificando se duas figuras se
sobrepõem.
*/
void checar_interseccao(Lista formas, Tabela id_forma, const char *linha, FILE *arquivo_log);

/*
Executa o comando i? especificado no arquivo de consulta, verificando se um ponto é interno a uma
figura, conecta este ponto e a figura utilizando uma linha.
*/
void checar_ponto_interno(Lista formas, Tabela id_forma, const char *linha, FILE *arquivo_log);

/*
Executa o comando pnt especificado no arquivo de consulta, alterando a cor de preenchimento e
borda da figura com id igual ao id que segue o comando pnt.
*/
void alterar_cor(Tabela id_forma, const char *linha, FILE *arquivo_log);

/*
Executa o comando pnt* especificado no arquivo de consulta, alterando a cor de preenchimento e
borda de todas as figuras entre id_inicial e id_final (inclusive).
*/
void alterar_cores(Lista formas, const char *linha, FILE *arquivo_log);

/*
Executa o comando delf especificado no arquivo de consulta, removendo a figura com id igual ao id
que segue o comando delf.
*/
void remover_elemento(Lista formas, Tabela id_forma, const char *linha, FILE *arquivo_log);

/*
Executa o comando delf* especificado no arquivo de consulta, removendo todas as figuras que se
encontrem entre id_inicial e id_final (inclusive).
*/
void remover_elementos(Lista formas, Tabela id_forma, const char *linha, FILE *arquivo_log);

#endif
