#pragma once

#include <stdio.h>

// Este módulo define o tipo texto, assim como funções que criam e acessam suas propriedades.

typedef void *Texto;

// Cria e inicializa um struct Circulo a partir de um texto obtido no arquivo .geo. O parâmetro não pode ser nulo.
Texto texto_ler(const char *linha);
// Escreve todas as informações presentes em um círculo em um arquivo. Ambos os parâmetros não podem ser nulos.
void texto_escrever_informacoes(FILE *arquivo, Texto tex);
// Escreve o código svg necessário para representar um círculo em um arquivo. Ambos os parâmetros não podem ser nulos.
void texto_escrever_svg(FILE *arquivo, Texto tex);
// Retorna o id de um texto. O parâmetro não pode ser nulo.

char texto_obter_id(Texto tex);
// Retorna a coordenada x de um texto. O parâmetro não pode ser nulo.
double texto_obter_x(Texto tex);
// Retorna a coordenada y de um texto. O parâmetro não pode ser nulo.
double texto_obter_y(Texto tex);
// Retorna a cor da borda de um texto. O parâmetro não pode ser nulo.
char texto_obter_cor_borda(Texto tex);
// Retorna a cor de preenchimento de um texto. O parâmetro não pode ser nulo.
char texto_obter_cor_preenchimento(Texto tex);
// Retorna o texto de um texto. O parâmetro não pode ser nulo.
char texto_obter(Texto tex);
