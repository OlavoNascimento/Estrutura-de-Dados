#ifndef ANIMACAO_H
#define ANIMACAO_H

/*
Este módulo define um Animacão, representando um círculo que possui movimentação em um svg. Possui
propriedades similares a um círculo como coordenadas espaciais, cor de borda e preenchimento,
assim como propriedades específicas como cor do caminho criado.

Suas propriedades são:
    id: Identificador da animação.
    cor da borda
    cor de preenchimento
    cor do caminho
    num_ponto: Tamanho do vetor de pontos.
    pontos: Vetor de pontos que compõem o caminho percorrido pela animação.
*/

#include <stdbool.h>
#include <stdio.h>

#include "../Formas/ponto.h"

/*
Foi usado 'typedef struct *' em vez de 'typedef void *' para permitir ao compilador diferenciar os
tipos, não afetando o encapsulamento de dados!
O struct *não é definido neste header* e sim em seu arquivo .c correspondente, o ponteiro deste tipo
não pode ser criado sem seu construtor, assim como suas propriedades não podem ser acessadas sem uso
de getters/setters.
*/
typedef struct Animacao_s *Animacao;

/*
Cria e inicializa um Animacao com os valores passados.
O id e as cores não podem ser nulos.
O vetor de pontos deve possuir pelo menos dois pontos para criar um caminho.
O usuário é responsável por liberar a memória alocada!
*/
Animacao animacao_criar(const char cor_borda[20], const char cor_preenchimento[20],
                        const char cor_caminho[20], int num_pontos, Ponto *pontos);

/*
Escreve o código svg necessário para representar uma animação em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void animacao_escrever_svg(Animacao animacao, FILE *arquivo);

/*
Define a margem x que deve ser adicionada ao caminho de uma animação. Necessário para que duas
animações não se sobreponham.
O parâmetro animacao não pode ser nulo.
*/
void animacao_definir_margem_x(Animacao animacao, double margem_x);

/*
Define a margem x que deve ser adicionada ao caminho de uma animação. Necessário para que duas
animações não se sobreponham.
O parâmetro animacao não pode ser nulo.
*/
void animacao_definir_margem_y(Animacao animacao, double margem_y);

/*
Libera a memória alocada por uma animação.
O parâmetro animacao não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada. O ponteiro não poderá ser utilizado após isso!
*/
void animacao_destruir(Animacao animacao);

#endif
