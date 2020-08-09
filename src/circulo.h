#ifndef CIRCULO_H
#define CIRCULO_H

#include <stdbool.h>
#include <stdio.h>

// Este módulo define o tipo Circulo, assim como funções que criam, acessam suas propriedades e
// realizam operações entre variáveis deste tipo.

typedef void *Circulo;

// Cria e inicializa um struct Circulo com os valores passados.
Circulo circulo_criar(char id[100], double raio, double x, double y, char cor_borda[20],
                      char cor_preenchimento[20]);
// Cria e inicializa um struct Circulo a partir de um texto obtido no arquivo .geo.
// O parâmetro linha não pode ser nulo.
Circulo circulo_ler(const char *linha);

// Escreve todas as informações presentes em um círculo em um arquivo.
// O parâmetro arquivo não pode ser nulo.
void circulo_escrever_informacoes(FILE *arquivo, Circulo circ);
// Escreve o código svg necessário para representar um círculo em um arquivo.
// O parâmetro arquivo não pode ser nulo.
void circulo_escrever_svg(FILE *arquivo, Circulo circ);

// Recebe dois circulos e retorna verdadeiro caso se intersectem.
bool circulo_checar_interseccao(Circulo circ1, Circulo circ2);
// Recebe um círculo e as coordenadas de um ponto, retorna verdadeiro caso o ponto esteja dentro do
// círculo.
bool circulo_checar_ponto_interno(Circulo circ, double ponto_x, double ponto_y);

// Retorna o id de um círculo.
const char *circulo_obter_id(Circulo circ);
// Define o id de um círculo. Os parâmetro id não pode ser nulo.
void circulo_definir_id(Circulo *circ, const char *id);

// Retorna o raio de um círculo.
double circulo_obter_raio(Circulo circ);
// Define o raio de um círculo.
void circulo_definir_raio(Circulo *circ, double raio);

// Retorna a coordenada x de um círculo.
double circulo_obter_x(Circulo circ);
// Define a coordenada x de um círculo.
void circulo_definir_x(Circulo *circ, double x);

// Retorna a coordenada y de um círculo.
double circulo_obter_y(Circulo circ);
// Define a coordenada y de um círculo.
void circulo_definir_y(Circulo *circ, double y);

// Retorna a cor da borda de um círculo.
const char *circulo_obter_cor_borda(Circulo circ);
// Define a cor da borda de um círculo. O parâmetro cor_borda não pode ser nulo.
void circulo_definir_cor_borda(Circulo *circ, const char *cor_borda);

// Retorna a cor de preenchimento de um círculo..
const char *circulo_obter_cor_preenchimento(Circulo circ);
// Define a cor de preenchimento de um círculo. O parâmetros cor_preenchimento não pode ser nulo.
void circulo_definir_cor_preenchimento(Circulo *circ, const char *cor_preenchimento);

#endif
