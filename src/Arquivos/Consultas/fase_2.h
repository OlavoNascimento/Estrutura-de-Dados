#ifndef FASE_2_H
#define FASE_2_H

/*
Este módulo define as funções de consulta que pertecem a segunda fase do projeto.
*/

#include <stdio.h>

#include "../../Estruturas/lista.h"
#include "../../Estruturas/quadtree.h"
#include "../../Estruturas/tabelahash.h"

/*
Executa o comando dq especificado no arquivo de consulta, removendo todas as quadras dentro de uma
distância do equipamento urbano identificado por um id.
Nenhum parâmetro pode ser nulo.
*/
void raio_remove_quadras(QuadTree quadras, Tabela cep_quadra, Tabela id_hidrante,
                         Tabela id_semaforo, Tabela id_radio, Lista formas, const char *linha,
                         FILE *arquivo_log);

/*
Executa o comando del especificado no arquivo de consulta, removendo a quadra, hidrante, semáforo ou
rádio identificado por um id.
Nenhum parâmetro pode ser nulo.
*/
void remove_equipamento_urbano(QuadTree quadras, Tabela cep_quadra, QuadTree hidrantes,
                               Tabela id_hidrante, QuadTree semaforos, Tabela id_semaforo,
                               QuadTree radios, Tabela id_radio, Lista formas, const char *linha,
                               FILE *arquivo_log);

/*
Executa o comando cbq especificado no arquivo de consulta, encontrando todas as quadras contidas
dentro de um círculo, mudando a corda da borda e escrevendo o id no arquivo de log.
Nenhum parâmetro pode ser nulo.
*/
void circulo_contem_quadras(QuadTree quadras, const char *linha, FILE *arquivo_log);

/*
Executa o comando crd? especificado no arquivo de consulta, encontrando um equipamento urbano em uma
lista e escrevendo suas coordenadas e tipo no arquivo de log.
Nenhum parâmetro pode ser nulo.
*/
void informacoes_equipamento_urbano(Tabela cep_quadra, Tabela id_hidrante, Tabela id_radio,
                                    Tabela id_semaforo, const char *linha, FILE *arquivo_log);

/*
Executa o comando car especificado no arquivo de consulta, encontrando o total das áreas das quadras
contidas dentro de um retângulo.
Nenhum parâmetro pode ser nulo.
*/
void retangulo_area_total_contida(Lista formas, QuadTree quadras, const char *linha,
                                  FILE *arquivo_log);

#endif
