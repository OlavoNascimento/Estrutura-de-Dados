#ifndef QUADRA_H
#define QUADRA_H

#include <stdbool.h>
#include <stdio.h>

#include "retangulo.h"

/*
Este módulo define o tipo Quadra, assim como funções que criam, acessam suas propriedades e
realizam operações entre variáveis deste tipo.
*/
typedef void *Quadra;

/*
Cria e inicializa uma struct Quadra com os valores passados.
A largura e altura devem ser maiores que 0. O id e as cores não podem ser nulos.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Quadra quadra_criar(const char id[100], double largura, double altura, double x, double y,
                    const char cor_borda[20], const char cor_preenchimento[20]);

/*
Cria e inicializa uma struct Quadra a partir de um texto obtido no arquivo .geo.
O parâmetro linha não pode ser nulo.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Quadra quadra_ler(const char *linha);

/*
Escreve todas as informações presentes em uma quadra em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void quadra_escrever_informacoes(FILE *arquivo, Quadra quad);

/*
Escreve o código svg necessário para representar uma quadra em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void quadra_escrever_svg(FILE *arquivo, Quadra quad);

/*
Obtém o id de uma quadra.
O parâmetro quad não pode ser nulo.
Retorna o id de uma quadra.
*/
const char *quadra_obter_id(Quadra quad);

/*
Obtém a coordenada y de uma quadra.
O parâmetro quad não pode ser nulo.
Retorna o raio de uma quadra.
*/
double quadra_obter_y(Quadra quad);

/*
Obtém a coordenada x de um quadra.
O parâmetro quad não pode ser nulo.
Retorna a coordenada x de uma quadra.
*/
double quadra_obter_x(Quadra quad);

/*
Obtém a largura de uma quadra.
O parâmetro quad não pode ser nulo.
Retorna a largura de uma quadra.
*/
double quadra_obter_largura(Quadra quad);

/*
Obtém a altura de uma quadra.
O parâmetro quad não pode ser nulo.
Retorna a altura de uma quadra.
*/
double quadra_obter_altura(Quadra quad);

/*
Obtém a cor de preenchimento de uma quadra.
O parâmetro quad não pode ser nulo.
Retorna a cor de preenchimento de uma quadra.
*/
const char *quadra_obter_cor_preenchimento(Quadra quad);

/*
Define a cor de preenchimento de uma quadra.
Nenhum dos parâmetros podem ser nulos.
*/
void quadra_definir_cor_preenchimento(Quadra quad, const char *cor_preenchimento);

/*
Obtém a cor da borda de uma quadra.
O parâmetro quad não pode ser nulo.
Retorna a cor da borda de uma quadra.
*/
const char *quadra_obter_cor_borda(Quadra quad);

/*
Define a cor da borda de uma quadra.
Nenhum dos parâmetros podem ser nulos.
*/
void quadra_definir_cor_borda(Quadra quad, const char *cor_borda);

/*
Define a espessura da borda de uma quadra.
Nenhum dos parâmetros podem ser nulos.
*/
void quadra_definir_espessura_borda(Quadra quad, const char *espessura_borda);

/*
Define o arredondamento de uma determinada quadra.
O parâmetro quad não pode ser nulo, deve ser uma quadra existente e $espessura_borda não pode
ser nulo.
*/
void quadra_definir_arredondamento_borda(Quadra quad, double raio_borda);

/*
Libera a memória alocada por uma quadra.
O parâmetro quad não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada pelo struct. O ponteiro não poderá ser utilizado após isso!
*/
void quadra_destruir(Quadra quad);

#endif
