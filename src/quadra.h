#ifndef QUADRA_H
#define QUADRA_H

#include <stdbool.h>
#include <stdio.h>

#include "retangulo.h"

typedef void *Quadra;

// Cria e inicializa um struct Quadra com os valores passados.
// retorna o endereço para a struct criada
Quadra quadra_criar(const char id[100], double largura, double altura, double x, double y,
                    const char cor_borda[20], const char cor_preenchimento[20]);

// Cria e inicializa um struct Quadra(igual ao retângulo) a partir de um texto obtido no arquivo
// .geo. O parâmetro linha não pode ser nulo.
Quadra quadra_ler(const char *linha);

void quadra_escrever_informacoes(FILE *arquivo, Quadra quad);

void quadra_escrever_svg(FILE *arquivo, Quadra quad);

// Retorna o id de uma quadra
const char *quadra_obter_id(Quadra qua);

// Retorna a coordenada y de uma quadra
double quadra_obter_y(Quadra quad);

// Retorna a coordenada x de uma quadra
double quadra_obter_x(Quadra quad);

// Retorna a largura de uma quadra
double quadra_obter_largura(Quadra quad);

// Retorna a altura de uma quadra
double quadra_obter_altura(Quadra quad);

// Retorna a cor de preenchimento de uma quadra
const char *quadra_obter_cor_preenchimento(Quadra quad);

// Define a cor de preenchimento de uma quadra.
// O parâmetros cor_preenchimento não pode ser nulo.
void quadra_definir_cor_preenchimento(Quadra quad, const char *cor_preenchimento);

// Retorna a cor da borda de uma quadra
const char *quadra_obter_cor_borda(Quadra quad);

// Define a cor de borda de uma quadra.
// O parâmetros cor_borda não pode ser nulo.
void quadra_definir_cor_borda(Quadra quad, const char *cor_borda);

// define a espessura da borda de uma quadra
void quadra_definir_espessura_borda(Quadra quad, const char *espessura_borda);

// Define o raio de arredondamento da borda de uma quadra
void quadra_definir_arredondamento_borda(Quadra quad, double raio_borda);

// libera a memória alocado para uma quadra
void quadra_destruir(Quadra quad);
#endif
