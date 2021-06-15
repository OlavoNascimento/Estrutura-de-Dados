#ifndef FASE_4_H
#define FASE_4_H

/*
Este módulo define as funções de consulta que pertecem a quarta fase do projeto.
*/

#include <stdio.h>

#include "../../Estruturas/lista.h"
#include "../../Estruturas/quadtree.h"
#include "../../Estruturas/tabelahash.h"

/*
Executa o comando m? especificado no arquivo de consulta, escreve as informações de todos os
moradores de uma quadra no arquivo log.
*/
void listar_moradores_quadra(Tabela cep_quadra, QuadTree moradores, const char *linha,
                             FILE *arquivo_log);

/*
Executa o comando dm? especificado no arquivo de consulta, escreve todas as informações de um
morador, especificado pelo cpf, no arquivo log. Além disso, coloca uma linha vertical do morador até
o topo.
*/
void mostrar_informacoes_morador(Lista formas, Tabela dados_pessoa, const char *linha,
                                 FILE *arquivo_log);

/*
Executa o comando de? especificado no arquivo de consulta, escreve no arquivo de log todos os dados
do estabelecimento comercial identificado por um cnpj.
*/
void mostrar_informacoes_estabelecimento(Tabela cnpj_estabelecimento, Tabela dados_pessoa,
                                         Tabela tipo_descricao, const char *linha,
                                         FILE *arquivo_log);

/*
Executa o comando mud especificado no arquivo de consulta, altera o endereço de um morador e escreve
no arquivo de log.
*/
void mudar_endereco_morador(Lista formas, Tabela cep_quadra, Tabela dados_pessoa, const char *linha,
                            FILE *arquivo_log);

/*
Executa o comando dmprbt especificado no arquivo de consulta, cria um arquivo svg com o nome
especificado, o qual contem a representação da quadtree selecionada com os ids e coordenadas de cada
figura.
*/
void escrever_quadtree_svg(const char *caminho_log, QuadTree quadras, QuadTree hidrantes,
                           QuadTree semaforos, QuadTree radios, const char *linha);

/*
Executa o comando eplg? especificado no arquivo de consulta, destaca os estabelecimentos do tipo
especificado e escreve suas informações no arquivo de log.
*/
void destacar_estabelecimentos_contidos(Tabela dados_pessoa, QuadTree estabelecimentos,
                                        const char *linha, FILE *arquivo_log);

/*
Executa o comando catac especificado no arquivo de consulta, remove as quadras, elementos urbanos,
moradores, estabelecimentos e vértices que estejam inteiramente contidos na circunferência
especificada.
*/
void remover_elementos_contidos(Lista formas, Tabela quadtrees, Tabela relacoes, Tabela grafos,
                                const char *linha, FILE *arquivo_log);

#endif
