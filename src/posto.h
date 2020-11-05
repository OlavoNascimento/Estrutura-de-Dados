#ifndef POSTO_H
#define POSTO_H

#include <stdbool.h>
#include <stdio.h>

#include "circulo.h"

/*
Este módulo define o tipo Posto, assim como funções que criam, acessam suas propriedades e
realizam operações entre variáveis deste tipo.
*/

typedef void *Posto;

/*
Cria e inicializa um struct Posto com os valores passados.
O raio deve ser maior que 0. O id e as cores não podem ser nulos.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Posto posto_criar(const char id[100], double x, double y);

/*
Cria e inicializa um struct Posto a partir de um texto obtido no arquivo .geo.
O parâmetro linha não pode ser nulo.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Posto posto_ler(const char *linha);

/*
Escreve todas as informações presentes em um posto em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void posto_escrever_informacoes(FILE *arquivo, Posto posto);

/*
Escreve o código svg necessário para representar um posto em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void posto_escrever_svg(FILE *arquivo, Posto posto);

/*
Obtém o id de um posto.
O parâmetro posto não pode ser nulo.
Retorna o id de um posto.
*/
const char *posto_obter_id(Posto posto);

/*
Obtém o raio de um posto.
O parâmetro posto não pode ser nulo.
Retorna o raio de um posto.
*/
double posto_obter_raio(Posto posto);

/*
Obtém a coordenada x de um posto.
O parâmetro posto não pode ser nulo.
Retorna a coordenada x de um posto.
*/
double posto_obter_x(Posto posto);
/*
Obtém a coordenada y de um posto.
O parâmetro posto não pode ser nulo.
Retorna a coordenada y de um posto.
*/
double posto_obter_y(Posto posto);

/*
Obtém a cor de preenchimento de um posto.
O parâmetro posto não pode ser nulo.
Retorna a cor de preenchimento de um posto.
*/
const char *posto_obter_cor_preenchimento(Posto posto);
/*
Define a cor de preenchimento de um posto.
Nenhum dos parâmetros podem ser nulos.
*/
void posto_definir_cor_preenchimento(Posto posto, const char *cor_preenchimento);

/*
Obtém a cor da borda de um posto.
O parâmetro posto não pode ser nulo.
Retorna a cor da borda de um posto.
*/
const char *posto_obter_cor_borda(Posto posto);
/*
Define a cor da borda de um posto.
Nenhum dos parâmetros podem ser nulos.
*/
void posto_definir_cor_borda(Posto posto, const char *cor_borda);

/*
Define a espessura da borda de um posto.
Nenhum dos parâmetros podem ser nulos.
*/
void posto_definir_espessura_borda(Posto posto, const char *espessura_borda);

/*
Libera a memória alocada por um posto.
O parâmetro posto não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada pelo struct. O ponteiro não poderá ser utilizado após isso!
*/
void posto_destruir(Posto posto);

#endif
