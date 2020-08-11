#ifndef HIDRANTE_H
#define HIDRANTE_H

#include "circulo.h"

typedef void *Hidrante;

// Cria e inicializa um struct Hidrante com os valores passados.
// retorna o endereço para a struct criada
Hidrante hidrante_criar(char id[100], double raio, double x, double y, char cor_borda[20],
                        char cor_preenchimento[20]);

// Cria e inicializa um struct Hidrante(igual ao circulo) a partir de um texto obtido no arquivo
// .geo. O parâmetro linha não pode ser nulo.
Hidrante hidrante_ler(const char *linha);

// Define a cor de borda de um hidrante.
// O parâmetros cor_borda não pode ser nulo.
void hidrante_definir_cor_borda(Hidrante hid, const char *cor_borda);

// Define a cor de preenchimento de um hidrante.
// O parâmetros cor_preenchimento não pode ser nulo.
void hidrante_definir_cor_preenchimento(Hidrante hid, const char *cor_preenchimento);

void hidrante_destruir(Hidrante hid);

#endif
