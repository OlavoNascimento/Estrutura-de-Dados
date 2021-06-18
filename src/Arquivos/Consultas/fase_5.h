#ifndef FASE_5_H
#define FASE_5_H

/*
Este módulo define as funções de consulta que pertecem a quinta fase do projeto.
*/

#include <stdio.h>

#include "../../Estruturas/lista.h"
#include "../../Estruturas/quadtree.h"
#include "../../Estruturas/tabelahash.h"

/*
Executa o comando @m? especificado no arquivo de consulta, salva a posição geográfica da residência
de um morador em um registrador.
*/
void registrar_posicao_morador(Ponto *registradores, Tabela dados_pessoa, Tabela cep_quadra,
                               Lista formas, const char *linha);

/*
Executa o comando @e? especificado no arquivo de consulta, salva a posição de um endereço em um
registrador.
*/
void registrar_posicao_endereco(Ponto *registradores, Tabela cep_quadra, Lista formas,
                                const char *linha);

/*
Executa o comando @g? especificado no arquivo de consulta, salva a posição de um equipamento urbano
em um registrador.
*/
void registrar_equipamento_urbano(Ponto *registradores, Tabela id_hidrante, Tabela id_semaforo,
                                  Tabela id_radio, Tabela id_forma, Lista formas,
                                  const char *linha);

/*
Executa o comando @xy especificado no arquivo de consulta, salva a posição de um ponto em um
registrador.
*/
void registrar_ponto(Ponto *registradores, Lista formas, const char *linha);

/*
Executa o comando ccv especificado no arquivo de consulta, cria um arquivo svg com o nome
especificado, o qual contem a representação do grafo. As arestas da árvore geradora mínima são
destacadas.
*/
void escrever_grafo_svg(const char *caminho_log, Tabela grafos, const char *linha);

/*
Executa o comando pb? especificado no arquivo de consulta, calcula o menor caminho entre dois
registradores utilizando a ciclovia como base.
*/
char *calcular_caminho_ciclo_via(Tabela quadtrees, Tabela grafos, Ponto *registradores,
                                 Lista svg_atual, const char *linha, FILE *arquivo_log);

/*
Executa o comando p? especificado no arquivo de consulta, calcula o menor caminho e o caminho mais
rápido entre dois registradores utilizando as vias como base.
*/
char *calcular_trajeto_vias(Tabela quadtrees, Tabela grafos, Ponto *registradores, Lista svg_atual,
                            const char *linha, FILE *arquivo_log);

/*
Executa o comando sp? especificado no arquivo de consulta, calcula o menor caminho e o caminho
mais rápido entre dois registradores utilizando as vias como base e evitando a envoltória convexa
dos casos de covid.
*/
char *analisar_vertices_contidos_envoltoria(Grafo vias, QuadTree casos, Lista formas,
                                            FILE *arquivo_log, Ponto *registradores,
                                            Lista svg_atual, const char *linha);
#endif
