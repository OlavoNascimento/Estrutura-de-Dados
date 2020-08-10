#ifndef HIDRANTE_H
#define HIDRANTE_H

#include "circulo.h"

typedef void* Radio;

// Cria e inicializa uma struct RadioBase com os valores passados.
// retorna o endereço para a struct criada
Radio radio_criar(char id[100], double raio, double x, double y, char cor_borda[20],
                  char cor_preenchimento[20]);

// Cria e inicializa uma struct RadioBase(igual ao circulo) a partir de um texto obtido no arquivo .geo.
// O parâmetro linha não pode ser nulo.
Radio radio_ler(const char* linha);

// Define a cor de preenchimento de uma RadioBase.
//O parâmetros cor_preenchimento não pode ser nulo.
void radio_definir_cor_preenchimento(Radio rad, const char* cor_preenchimento);

// Define a cor de borda de um RadioBase.
//O parâmetros cor_borda não pode ser nulo.
void radio_definir_cor_borda(Radio rad, const char* cor_borda);

#endif
