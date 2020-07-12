#pragma once

// Este módulo define o tipo Circulo, assim como funções que criam, acessam suas propiedades e realizam operações
// exclusivas deste tipo.

#include <stdbool.h>
#include <stdio.h>

typedef void *Circulo;

// Cria e inicializa um struct Circulo a partir de um texto obtido no arquivo .geo. Parâmetro linha não pode ser nulo.
Circulo ler_circulo(const char *linha);
// Escreve todas as informações presentes em um círculo em um arquivo. Ambos os parâmetros não podem ser nulos.
void escrever_informacoes_circulo(FILE *arquivo, Circulo circ);
// Escreve o código svg necessário para representar um círculo em um arquivo. Ambos os parâmetros não podem ser nulos.
void escrever_svg_circulo(FILE *arquivo, Circulo circ);
// Recebe dois circulos e retorna verdadeiro caso se intersectem. Ambos os parâmetros não podem ser nulos.
bool checar_interseccao_circulo(Circulo circ1, Circulo circ2);
// Recebe um círculo e as coordenadas de um ponto, retorna verdadeiro caso o ponto esteja dentro do círculo. Ambos os
// parâmetros não podem ser nulos.
bool checar_ponto_interno_circulo(Circulo circ, double ponto_x, double ponto_y);
// Retorna o id de um círculo. O parâmetro não pode ser nulo.
char *obterIdCirculo(Circulo circ);
// Retorna o raio de um círculo. O parâmetro não pode ser nulo.
double obterRaioCirculo(Circulo circ);
// Retorna a coordenada x de um círculo. O parâmetro não pode ser nulo.
double obterXCirculo(Circulo circ);
// Retorna a coordenada y de um círculo. O parâmetro não pode ser nulo.
double obterYCirculo(Circulo circ);
// Retorna a cor da borda de um círculo. O parâmetro não pode ser nulo.
char *obterCorBordaCirculo(Circulo circ);
// Retorna a cor de preenchimento de um círculo. O parâmetro não pode ser nulo.
char *obterCorPreenchimentoCirculo(Circulo circ);