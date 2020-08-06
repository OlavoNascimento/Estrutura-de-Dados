#pragma once

#include <stdio.h>

// Este módulo define o tipo texto, assim como funções que criam e acessam suas propriedades.

typedef void *Texto;

// Cria e inicializa um struct Texto com os valores passados.
Texto texto_criar(char id[100], double x, double y, char cor_borda[20], char cor_preenchimento[20],
                  char conteudo[500]);
// Cria e inicializa um struct Texto a partir de um texto obtido no arquivo .geo.
// O parâmetro linha não pode ser nulo.
Texto texto_ler(const char *linha);

// Escreve todas as informações presentes em um círculo em um arquivo.
// O parâmetro linha arquivo pode ser nulo.
void texto_escrever_informacoes(FILE *arquivo, Texto tex);
// Escreve o código svg necessário para representar um círculo em um arquivo.
// O parâmetro linha arquivo pode ser nulo.
void texto_escrever_svg(FILE *arquivo, Texto tex);

// Retorna o id de um texto.
const char *texto_obter_id(Texto tex);
// Define o id de um texto. O parâmetro id não pode ser nulo.
void texto_definir_id(Texto *tex, const char *id);

// Retorna a coordenada x de um texto.
double texto_obter_x(Texto tex);
// Retorna a coordenada x de um texto.
void texto_definir_x(Texto *tex, double x);

// Retorna a coordenada y de um texto.
double texto_obter_y(Texto tex);
// Retorna a coordenada y de um texto.
void texto_definir_y(Texto *tex, double y);

// Retorna a cor da borda de um texto.
const char *texto_obter_cor_borda(Texto tex);
// Define a cor da borda de um texto. O parâmetro cor_borda não pode ser nulo.
void texto_definir_cor_borda(Texto *tex, const char *cor_borda);

// Retorna a cor de preenchimento de um texto.
const char *texto_obter_cor_preenchimento(Texto tex);
// Define a cor de preenchimento de um texto. O parâmetro cor_preenchimento não pode ser nulo.
void texto_definir_cor_preenchimento(Texto *tex, const char *cor_preenchimento);

// Retorna o conteudo de um texto.
const char *texto_obter_texto(Texto tex);
// Define o conteudo de um texto. O parâmetro conteudo não pode ser nulo.
void texto_definir_texto(Texto *tex, const char *conteudo);
