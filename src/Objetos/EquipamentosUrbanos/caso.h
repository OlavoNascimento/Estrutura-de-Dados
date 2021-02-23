#ifndef CASO_H
#define CASO_H

/*
Este módulo define um Caso, o qual representa um retângulo que contem um certo número de infectados
em uma área e que possui cores e rótulo específico ao seu tipo para fácil diferenciação no arquivo
svg.

Suas propriedades são:
    casos: Número de infectados na área delimitado pelo caso.
    largura
    altura
    x: Coordenada x do caso no plano.
    y: Coordenada y do caso no plano.
    cor de borda
    cor de preenchimento
*/

#include <stdbool.h>
#include <stdio.h>

#include "quadra.h"

/*
Foi usado 'typedef struct *' em vez de 'typedef void *' para permitir ao compilador diferenciar os
tipos, não afetando o encapsulamento de dados!
O struct *não é definido neste header* e sim em seu arquivo .c correspondente, o ponteiro deste tipo
não pode ser criado sem seu construtor, assim como suas propriedades não podem ser acessadas sem uso
de getters/setters.
*/
typedef struct Caso_s *Caso;

/*
Cria e inicializa um Caso com os valores passados.
Argumentos:
    casos: Número de infectados na área delimitado pelo caso.
    quadra: Quadra onde o caso está contido.
    face: Lado da quadra onde o Caso ocorreu.
    numero: Distância do caso e extremidade da face.
O argumento quadra não pode ser nulo. A face deve ser uma das seguintes opções: N, S, O, L.
O usuário é responsável por liberar a memória alocada!
*/
Caso caso_criar(int casos, Quadra quadra, char face, int numero);

/*
Cria e inicializa um Caso a partir de um texto obtido no arquivo .geo.
Nenhum dos parâmetros podem ser nulos.
O usuário é responsável por liberar a memória alocada!
*/
Caso caso_ler(const char *linha, Quadra quadra);

/*
Obtém o número de casos de um determinada caso.
O parâmetro caso não pode ser nulo.
Retorna o número de casos.
*/
int caso_obter_numero_de_casos(Caso caso);

/*
Obtém a coordenada x de um caso.
O parâmetro caso não pode ser nulo.
Retorna a coordenada x de um caso.
*/
double caso_obter_x(Caso caso);

/*
Obtém a coordenada y de um caso.
O parâmetro caso não pode ser nulo.
Retorna o raio de um caso.
*/
double caso_obter_y(Caso caso);

/*
Obtém a largura de um caso.
O parâmetro caso não pode ser nulo.
Retorna a largura de um caso.
*/
double caso_obter_largura(Caso caso);

/*
Obtém a altura de um caso.
O parâmetro caso não pode ser nulo.
Retorna a altura de um caso.
*/
double caso_obter_altura(Caso caso);

/*
Obtém a cor de preenchimento de um caso.
O parâmetro caso não pode ser nulo.
Retorna a cor de preenchimento de um caso.
*/
const char *caso_obter_cor_preenchimento(Caso caso);

/*
Define a cor de preenchimento de um caso.
Nenhum dos parâmetros podem ser nulos.
*/
void caso_definir_cor_preenchimento(Caso caso, const char *cor_preenchimento);

/*
Obtém a cor da borda de um caso.
O parâmetro caso não pode ser nulo.
Retorna a cor da borda de um caso.
*/
const char *caso_obter_cor_borda(Caso caso);

/*
Define a cor da borda de um caso.
Nenhum dos parâmetros podem ser nulos.
*/
void caso_definir_cor_borda(Caso caso, const char *cor_borda);

/*
Define a espessura da borda de um caso.
Nenhum dos parâmetros podem ser nulos.
*/
void caso_definir_espessura_borda(Caso caso, double espessura_borda);

/*
Define se o caso tem borda tracejada.
O parâmetro caso não pode ser nulo.
*/
void caso_definir_borda_tracejada(Caso caso, bool tracejado);

/*
Define o arredondamento da borda de um caso.
O parâmetro caso não pode ser nulo.
*/
void caso_definir_arredondamento_borda(Caso caso, double arredondamento_borda);

/*
Libera a memória alocada por um caso.
O parâmetro caso não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada. O ponteiro não poderá ser utilizado após isso!
*/
void caso_destruir(Caso caso);

#endif
