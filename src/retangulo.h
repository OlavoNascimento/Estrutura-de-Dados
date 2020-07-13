#pragma once

#include <stdio.h>

// Este módulo define o tipo Retângulo, assim como funções que criam, acessam suas propriedades e realizam operações
// entre variáveis deste tipo.

typedef void *Retangulo;

// Cria e inicializa um struct Retangulo a partir de um texto obtido no arquivo .geo. O parâmetro não pode ser nulo.
Retangulo retangulo_ler(char *linha);
// Escreve todas as informações presentes em um retângulo em um arquivo. Ambos os parâmetros não podem ser nulos.
void retangulo_escrever_informacoes(FILE *arquivo, Retangulo ret);
// Escreve o código svg necessário para representar um retângulo em um arquivo. Ambos os parâmetros não podem ser nulos.
void retangulo_escrever_svg(FILE *arquivo, Retangulo ret);
// Recebe dois circulos e retorna verdadeiro caso se intersectem. Ambos os parâmetros não podem ser nulos.
bool retangulo_checar_interseccao(Retangulo ret1, Retangulo ret2);
// parâmetros não podem ser nulos.
bool retangulo_checar_ponto_interno(Retangulo ret, double ponto_x, double ponto_y);
// Retorna o id de um retângulo. O parâmetro não pode ser nulo.
char retangulo_obter_id(Retangulo ret);
// Retorna a largura de um retângulo. O parâmetro não pode ser nulo.
double retangulo_obter_largura(Retangulo ret);
// Retorna a altura de um retângulo. O parâmetro não pode ser nulo.
double retangulo_obter_altura(Retangulo ret);
// Retorna a coordenada x de um retângulo. O parâmetro não pode ser nulo.
double retangulo_obter_x(Retangulo ret);
// Retorna a coordenada y de um retângulo. O parâmetro não pode ser nulo.
double retangulo_obter_y(Retangulo ret);
// Retorna a cor da borda de um retângulo. O parâmetro não pode ser nulo.
char retangulo_obter_cor_borda(Retangulo ret);
// Retorna a cor de preenchimento de um retângulo. O parâmetro não pode ser nulo.
char retangulo_obter_cor_preenchimento(Retangulo ret);
