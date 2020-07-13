#pragma once

#include <stdio.h>

// Este módulo define o tipo Linha, assim como funções que criam e acessam suas propriedades.

typedef void *Linha;

// Escreve todas as informações presentes em uma Linha em um arquivo. Ambos os parâmetros não podem ser nulos.
void escrever_svg_linha(FILE *arquivo, Linha lin);
// Retorna a coordenada x1 de uma Linha. O parâmetro não pode ser nulo.
double obterX1Linha(Linha lin);
// Retorna a coordenada y1 de uma Linha. O parâmetro não pode ser nulo.
double obterY1Linha(Linha lin);
// Retorna a coordenada x2 de uma Linha. O parâmetro não pode ser nulo.
double obterX2Linha(Linha lin);
// Retorna a coordenada y2 de uma Linha. O parâmetro não pode ser nulo.
double obterY2Linha(Linha lin);
// Retorna a cor da borda de uma Linha. O parâmetro não pode ser nulo.
char obterCorBorda(Linha lin);
// Retorna a cor de preenchimento de uma Linha. O parâmetro não pode ser nulo.
char obterCorPreenchimento(Linha lin);
