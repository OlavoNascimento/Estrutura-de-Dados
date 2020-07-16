#pragma once

#include <stdio.h>

// Este módulo define o tipo Linha, assim como funções que criam e acessam suas propriedades.

typedef void *Linha;

// Escreve todas as informações presentes em uma Linha em um arquivo. Ambos os parâmetros não podem ser nulos.
void linha_escrever_svg(FILE *arquivo, Linha lin);

// Retorna a coordenada x1 de uma Linha. O parâmetro não pode ser nulo.
double linha_obter_x1(Linha lin);
// Retorna a coordenada y1 de uma Linha. O parâmetro não pode ser nulo.
double linha_obter_y1(Linha lin);
// Retorna a coordenada x2 de uma Linha. O parâmetro não pode ser nulo.
double linha_obter_x2(Linha lin);
// Retorna a coordenada y2 de uma Linha. O parâmetro não pode ser nulo.
double linha_obter_y2(Linha lin);
// Retorna a cor da borda de uma Linha. O parâmetro não pode ser nulo.
char linha_obter_cor_borda(Linha lin);
// Retorna a cor de preenchimento de uma Linha. O parâmetro não pode ser nulo.
char linha_obter_cor_preenchimento(Linha lin);
