#ifndef ANIMACAO_H
#define ANIMACAO_H

/*
TODO
Este módulo define um Animacao, o qual possui propriedades inerentes ao seu tipo como raio e suas
coordenadas espaciais, assim como propriedades para alterar sua visualização no arquivo svg, como as
cores de borda e preenchimento e a espessura de sua borda.

Suas propriedades são:
    id: Identificador do círculo.
    raio
    x: Coordenada x do círculo no plano.
    y: Coordenada y do círculo no plano.
    cor da borda
    cor de preenchimento
*/

#include <stdbool.h>
#include <stdio.h>

#include "../Formas/ponto.h"

/*
TODO
Foi usado 'typedef struct *' em vez de 'typedef void *' para permitir ao compilador diferenciar os
tipos, não afetando o encapsulamento de dados!
O struct *não é definido neste header* e sim em seu arquivo .c correspondente, o ponteiro deste tipo
não pode ser criado sem seu construtor, assim como suas propriedades não podem ser acessadas sem uso
de getters/setters.
*/
typedef struct Animacao_s *Animacao;

/*
TODO
Cria e inicializa um Animacao com os valores passados.
O raio deve ser maior que 0. O id e as cores não podem ser nulos.
O usuário é responsável por liberar a memória alocada!
*/
Animacao animacao_criar(const char id[100], const char cor_borda[20],
                        const char cor_preenchimento[20], const char cor_caminho[20],
                        int num_pontos, Ponto *pontos);

/*
TODO
Escreve o código svg necessário para representar um círculo em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void animacao_escrever_svg(Animacao animacao, FILE *arquivo);

/*
TODO
Libera a memória alocada por um círculo.
O parâmetro animacao não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada. O ponteiro não poderá ser utilizado após isso!
*/
void animacao_destruir(Animacao animacao);

#endif
