#ifndef RADIO_BASE_H
#define RADIO_BASE_H

#include <stdbool.h>
#include <stdio.h>

#include "circulo.h"

typedef void* Radio;

// Cria e inicializa uma struct RadioBase com os valores passados.
// retorna o endereço para a struct criada
Radio radio_criar(const char id[100], double raio, double x, double y, const char cor_borda[20],
                  const char cor_preenchimento[20]);

// Cria e inicializa uma struct RadioBase(igual ao circulo) a partir de um texto obtido no arquivo
// .geo. O parâmetro linha não pode ser nulo.
Radio radio_ler(const char* linha);

void radio_escrever_informacoes(FILE* arquivo, Radio rad);

void radio_escrever_svg(FILE* arquivo, Radio rad);

// Retorna o id de um rádio base.
const char* radio_obter_id(Radio rad);

// Retorna o raio de um radio base. Usado apenas para cálculo interno, já que o radio base é
// considerado um ponto.
double radio_obter_raio(Radio rad);
// Retorna a coordenada y de uma rádio-base.
double radio_obter_y(Radio rad);
// Define a coordenada x de uma rádio-base.
double radio_obter_x(Radio rad);

// Retorna a cor de preenchimento de uma rádio-base.
const char* radio_obter_cor_preenchimento(Radio rad);
// Define a cor de preenchimento de uma RadioBase.
// O parâmetros cor_preenchimento não pode ser nulo.
void radio_definir_cor_preenchimento(Radio rad, const char* cor_preenchimento);

// Retorna a cor da borda de uma rádio-base.
const char* radio_obter_cor_borda(Radio rad);
// Define a cor de borda de um RadioBase.
// O parâmetros cor_borda não pode ser nulo.
void radio_definir_cor_borda(Radio rad, const char* cor_borda);

// Retorna a espessura da borda de um rádio base.
const char* radio_obter_espessura_borda(Radio rad);
// Define a espessura da borda de um rádio base.
void radio_definir_espessura_borda(Radio rad, const char* espessura_borda);

// Libera a memória alocada por um radio base.
void radio_destruir(Radio rad);

#endif
