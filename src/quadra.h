#ifndef QUADRA_H
#define QUADRA_H

#include <stdbool.h>
#include <stdio.h>

#include "quadra.h"

typedef void *Quadra;
// Cria e inicializa um struct Quadra com os valores passados.
// retorna o endereço para a struct criada
Quadra quadra_criar(char id[100], double largura, double altura, double x, double y,
                    char cor_borda[20], char cor_preenchimento[20]);

// Cria e inicializa um struct Quadra(igual ao retângulo) a partir de um texto obtido no arquivo
// .geo. O parâmetro linha não pode ser nulo.
Quadra quadra_ler(const char *linha);

void quadra_escrever_informacoes(FILE *arquivo, Quadra quad);

void quadra_escrever_svg(FILE *arquivo, Quadra quad);

// Retorna o id de uma quadra
const char *quadra_obter_id(Quadra qua);

double quadra_obter_y(Quadra quad);

double quadra_obter_x(Quadra quad);

double quadra_obter_largura(Quadra quad);

double quadra_obter_altura(Quadra quad);

const char *quadra_obter_cor_preenchimento(Quadra quad);
// Define a cor de preenchimento de uma quadra.
// O parâmetros cor_preenchimento não pode ser nulo.
void quadra_definir_cor_preenchimento(Quadra quad, const char *cor_preenchimento);

const char *quadra_obter_cor_borda(Quadra quad);
// Define a cor de borda de uma quadra.
// O parâmetros cor_borda não pode ser nulo.
void quadra_definir_cor_borda(Quadra quad, const char *cor_borda);

void quadra_definir_espessura_borda(Quadra quad, int espessura_borda);

void quadra_destruir(Quadra quad);

#endif
