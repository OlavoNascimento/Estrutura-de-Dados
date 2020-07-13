#pragma once

#include <stdio.h>

// Este módulo define o tipo Retângulo, assim como funções que criam, acessam suas propriedades e realizam operações
// entre variáveis deste tipo.

typedef void *Retangulo;

// Cria e inicializa um struct Retangulo a partir de um texto obtido no arquivo .geo. O parâmetro não pode ser nulo.
Retangulo ler_retangulo(char *linha);
// Escreve todas as informações presentes em um retângulo em um arquivo. Ambos os parâmetros não podem ser nulos.
void escrever_informacoes_retangulo(FILE *arquivo, Retangulo ret);
// Escreve o código svg necessário para representar um retângulo em um arquivo. Ambos os parâmetros não podem ser nulos.
void escrever_svg_retangulo(FILE *arquivo, Retangulo ret);
// Recebe dois circulos e retorna verdadeiro caso se intersectem. Ambos os parâmetros não podem ser nulos.
bool checar_interseccao_retangulo(Retangulo ret1, Retangulo ret2);
// parâmetros não podem ser nulos.
bool checar_ponto_interno_retangulo(Retangulo ret, double ponto_x, double ponto_y);
// Retorna o id de um retângulo. O parâmetro não pode ser nulo.
char obterIdRetangulo(Retangulo ret);
// Retorna a largura de um retângulo. O parâmetro não pode ser nulo.
double obterLarguraRetangulo(Retangulo ret);
// Retorna a altura de um retângulo. O parâmetro não pode ser nulo.
double obterAlturaRetangulo(Retangulo ret);
// Retorna a coordenada x de um retângulo. O parâmetro não pode ser nulo.
double obterXRetangulo(Retangulo ret);
// Retorna a coordenada y de um retângulo. O parâmetro não pode ser nulo.
double obterYRetangulo(Retangulo ret);
// Retorna a cor da borda de um retângulo. O parâmetro não pode ser nulo.
char obterCorBordaRetangulo(Retangulo ret);
// Retorna a cor de preenchimento de um retângulo. O parâmetro não pode ser nulo.
char obterCorPreenchimentoRetangulo(Retangulo ret);
