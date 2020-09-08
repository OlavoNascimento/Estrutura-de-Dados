#ifndef RADIO_BASE_H
#define RADIO_BASE_H

#include <stdbool.h>
#include <stdio.h>

#include "circulo.h"

/*
Este módulo define o tipo Radio, assim como funções que criam, acessam suas propriedades e
realizam operações entre variáveis deste tipo.
*/
typedef void* Radio;

/*
Cria e inicializa um struct Radio com os valores passados.
O raio deve ser maior que 0. O id e as cores não podem ser nulos.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Radio radio_criar(const char id[100], double raio, double x, double y,const char cor_borda[20],
                  const char cor_preenchimento[20]);

/*
Cria e inicializa um struct Radio a partir de um texto obtido no arquivo .geo.
O parâmetro linha não pode ser nulo.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Radio radio_ler(const char* linha);

/*
Escreve todas as informações presentes em uma radio-base em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void radio_escrever_informacoes(FILE* arquivo, Radio rad);

/*
Escreve o código svg necessário para representar uma radio-base em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void radio_escrever_svg(FILE* arquivo, Radio rad);

/*
Obtém o id de uma rádio-base.
O parâmetro rad não pode ser nulo.
Retorna o id de uma rádio-base.
*/
const char* radio_obter_id(Radio rad);

/*
Obtém o raio de uma rádio-base.
O parâmetro rad não pode ser nulo.
Retorna o raio de uma rádio-base.
*/
double radio_obter_raio(Radio rad);

/*
Obtém a coordenada y de uma rádio-base.
O parâmetro rad não pode ser nulo.
Retorna a coordenada y de uma rádio-base.
*/
double radio_obter_y(Radio rad);

/*
Obtém a coordenada x de uma rádio-base.
O parâmetro rad não pode ser nulo.
Retorna a coordenada x de uma rádio-base.
*/
double radio_obter_x(Radio rad);

/*
Obtém a cor de preenchimento de uma rádio-base.
O parâmetro rad não pode ser nulo.
Retorna a cor de preenchimento de uma rádio-base.
*/
const char* radio_obter_cor_preenchimento(Radio rad);

/*
Define a cor de preenchimento de uma rádio-base.
Nenhum dos parâmetros podem ser nulos.
*/
void radio_definir_cor_preenchimento(Radio rad, const char* cor_preenchimento);

/*
Obtém a cor da borda de uma rádio-base.
O parâmetro rad não pode ser nulo.
Retorna a cor da borda de uma rádio-base.
*/
const char* radio_obter_cor_borda(Radio rad);

/*
Define a cor da borda de uma rádio-base.
Nenhum dos parâmetros podem ser nulos.
*/
void radio_definir_cor_borda(Radio rad, const char* cor_borda);

/*
Obtém a espessura da borda de uma rádio-base.
O parâmetro rad não pode ser nulo.
Retorna a espessura da borda de uma rádio-base.
*/
const char* radio_obter_espessura_borda(Radio rad);

/*
Define a espessura da borda de um rádio-base.
Nenhum dos parâmetros podem ser nulos.
*/
void radio_definir_espessura_borda(Radio rad, const char* espessura_borda);

/*
Libera a memória alocada para um semáforo
O parâmetro "sem" não pode ser nulo, deve ser um semáforo existente.
*/
void radio_destruir(Radio rad);

#endif
