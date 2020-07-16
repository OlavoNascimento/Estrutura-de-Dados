#pragma once

#include <stdbool.h>
#include <stdio.h>

// Este módulo define o tipo Circulo, assim como funções que criam, acessam suas propriedades e realizam operações
// entre variáveis deste tipo.

typedef void *Circulo;

// Cria e inicializa um struct Circulo a partir de um texto obtido no arquivo .geo. O parâmetro não pode ser nulo.
Circulo circulo_ler(const char *linha);
// Escreve todas as informações presentes em um círculo em um arquivo. Ambos os parâmetros não podem ser nulos.
void circulo_escrever_informacoes(FILE *arquivo, Circulo circ);
// Escreve o código svg necessário para representar um círculo em um arquivo. Ambos os parâmetros não podem ser nulos.
void circulo_escrever_svg(FILE *arquivo, Circulo circ);
// Recebe dois circulos e retorna verdadeiro caso se intersectem. Ambos os parâmetros não podem ser nulos.
bool circulo_checar_interseccao(Circulo circ1, Circulo circ2);
// Recebe um círculo e as coordenadas de um ponto, retorna verdadeiro caso o ponto esteja dentro do círculo. Ambos os
// parâmetros não podem ser nulos.
bool circulo_checar_ponto_interno(Circulo circ, double ponto_x, double ponto_y);

// Retorna o id de um círculo. O parâmetro não pode ser nulo.
char *circulo_obter_id(Circulo circ);
// Retorna o raio de um círculo. O parâmetro não pode ser nulo.
double circulo_obter_raio(Circulo circ);
// Retorna a coordenada x de um círculo. O parâmetro não pode ser nulo.
double circulo_obter_x(Circulo circ);
// Retorna a coordenada y de um círculo. O parâmetro não pode ser nulo.
double circulo_obter_y(Circulo circ);
// Retorna a cor da borda de um círculo. O parâmetro não pode ser nulo.
char *circulo_obter_cor_borda(Circulo circ);
// Retorna a cor de preenchimento de um círculo. O parâmetro não pode ser nulo.
char *circulo_obter_cor_preenchimento(Circulo circ);
