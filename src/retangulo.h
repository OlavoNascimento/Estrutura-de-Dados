#ifndef RETANGULO_H
#define RETANGULO_H

#include <stdbool.h>
#include <stdio.h>

// Este módulo define o tipo Retângulo, assim como funções que criam, acessam suas propriedades e
// realizam operações entre variáveis deste tipo.

typedef void *Retangulo;

// Cria e inicializa um struct Retangulo com os valores passados.
Retangulo retangulo_criar(const char id[100], double largura, double altura, double x, double y,
                          const char cor_borda[20], const char cor_preenchimento[20]);
// Cria e inicializa um struct Retangulo a partir de um texto obtido no arquivo .geo.
// O parâmetro linha não pode ser nulo.
Retangulo retangulo_ler(const char *linha);

// Escreve todas as informações presentes em um retângulo em um arquivo.
// O parâmetro arquivo não pode ser nulo.
void retangulo_escrever_informacoes(FILE *arquivo, Retangulo ret);
// Escreve o código svg necessário para representar um retângulo em um arquivo.
// O parâmetro arquivo não pode ser nulo.
void retangulo_escrever_svg(FILE *arquivo, Retangulo ret);

// Recebe dois circulos e retorna verdadeiro caso se intersectem.
bool retangulo_checar_interseccao(Retangulo ret1, Retangulo ret2);
// Recebe um retângulo, uma cordenada x e uma cordenada y. Retorna verdadeiro se o ponto se
// encontrar dentro do retângulo.
bool retangulo_checar_ponto_interno(Retangulo ret, double ponto_x, double ponto_y);

// Retorna o id de um retângulo.
const char *retangulo_obter_id(Retangulo ret);
// Define o id de um retângulo. O parâmetro id não pode ser nulo.
void retangulo_definir_id(Retangulo ret, const char *id);

// Retorna a largura de um retângulo.
double retangulo_obter_largura(Retangulo ret);
// Define a largura de um retângulo.
void retangulo_definir_largura(Retangulo ret, double largura);

// Retorna a altura de um retângulo.
double retangulo_obter_altura(Retangulo ret);
// Define a largura de um retângulo.
void retangulo_definir_altura(Retangulo ret, double altura);

// Retorna a coordenada x de um retângulo.
double retangulo_obter_x(Retangulo ret);
// Define a coordenada x de um retângulo.
void retangulo_definir_x(Retangulo ret, double x);

// Retorna a coordenada y de um retângulo.
double retangulo_obter_y(Retangulo ret);
// Define a coordenada y de um retângulo.
void retangulo_definir_y(Retangulo ret, double y);

// Retorna a cor da borda de um retângulo.
const char *retangulo_obter_cor_borda(Retangulo ret);
// Define a cor da borda de um retângulo. O parâmetro cor_borda não podem ser nulo.
void retangulo_definir_cor_borda(Retangulo ret, const char *cor_borda);

// Retorna a cor de preenchimento de um retângulo.
const char *retangulo_obter_cor_preenchimento(Retangulo ret);
// Define a cor de preenchimento de um retângulo. O parâmetro cor_preenchimento não pode ser
// nulo.
void retangulo_definir_cor_preenchimento(Retangulo ret, const char *cor_preenchimento);

// Retorna o tamanho da borda de um retângulo.
const char *retangulo_obter_espessura_borda(Retangulo ret);
// Define o tamanho da borda de um retângulo.
void retangulo_definir_espessura_borda(Retangulo ret, const char *espessura_borda);

// Retorna o espaçamento da borda de um retângulo.
int retangulo_obter_espassamento_borda(Retangulo ret);
// Define o espaçamento da borda de um retângulo.
void retangulo_definir_espassamento_borda(Retangulo ret, int espassamento_borda);

// Define o raio de arredondamento da borda do retangulo
void retangulo_definir_arredondamento_borda(Retangulo ret, double raio_borda);

void retangulo_destruir(Retangulo ret);

#endif
