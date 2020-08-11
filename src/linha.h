#ifndef LINHA_H
#define LINHA_H

#include <stdio.h>

// Este módulo define o tipo Linha, assim como funções que criam e acessam suas propriedades.

typedef void *Linha;

// Cria e inicializa um struct Linha com os valores passados.
Linha linha_criar(double x1, double y1, double x2, double y2, const char cor_borda[20],
                  const char cor_preenchimento[20]);
// Escreve todas as informações presentes em uma linha em um arquivo.
// O parâmetro arquivo não pode ser nulo.
void linha_escrever_svg(FILE *arquivo, Linha lin);

// Retorna a coordenada x inicial de uma linha.
double linha_obter_x1(Linha lin);
// Define a coordenada x inicial de uma linha.
void linha_definir_x1(Linha lin, double x1);

// Retorna a coordenada y inicial de uma linha.
double linha_obter_y1(Linha lin);
// Define a coordenada y inicial de uma linha.
void linha_definir_y1(Linha lin, double y1);

// Retorna a coordenada x final de uma linha.
double linha_obter_x2(Linha lin);
// Define a coordenada x final de uma linha.
void linha_definir_x2(Linha lin, double x2);

// Retorna a coordenada y final de uma linha.
double linha_obter_y2(Linha lin);
// Define a coordenada y final de uma linha.
void linha_definir_y2(Linha lin, double y2);

// Retorna a cor da borda de uma linha.
const char *linha_obter_cor_borda(Linha lin);
// Define a cor da borda de uma linha. O parâmetro cor_borda não pode ser nulo.
void linha_definir_cor_borda(Linha lin, const char *cor_borda);

// Retorna a cor de preenchimento de uma linha.
const char *linha_obter_cor_preenchimento(Linha lin);
// Define a cor de preenchimento de uma linha. O parâmetro cor_preenchimento não pode ser nulo.
void linha_definir_cor_preenchimento(Linha lin, const char *cor_preenchimento);

void linha_destruir(Linha lin);

#endif
