#ifndef DENSIDADE_H
#define DENSIDADE_H

/*
Este módulo define uma Densidade, o qual representa uma área retangular que contem a densidade
populacional em sua área. Possui o número de habitantes por km^2, assim como altura, largura e
coordenadas espaciais.

Suas propriedades são:
    densidade: Número de habitantes por km^2 na área delimitado pela densidade.
    largura
    altura
    x: Coordenada x da densidade no plano.
    y: Coordenada y da densidade no plano.
*/

#include <stdbool.h>
#include <stdio.h>

#include "../../Estruturas/lista.h"
#include "../../Interfaces/figura.h"

/*
Foi usado 'typedef struct *' em vez de 'typedef void *' para permitir ao compilador diferenciar os
tipos, não afetando o encapsulamento de dados!
O struct *não é definido neste header* e sim em seu arquivo .c correspondente, o ponteiro deste tipo
não pode ser criado sem seu construtor, assim como suas propriedades não podem ser acessadas sem uso
de getters/setters.
*/
typedef struct Densidade_s *Densidade;

/*
Cria e inicializa uma Densidade com os valores passados.
A densidade não pode ser menor que zero.
O usuário é responsável por liberar a memória alocada!
*/
Densidade densidade_criar(double x, double y, double largura, double altura, double densidade);

/*
Cria e inicializa uma Densidade a partir de um texto obtido no arquivo .geo.
O parâmetro linha não pode ser nulo.
O usuário é responsável por liberar a memória alocada!
*/
Densidade densidade_ler(const char *linha);

/*
Retorna a densidade armazenada em uma densidade.
O parâmetro densidade não pode ser nulo.
*/
double densidade_obter_densidade(Densidade densidade);

/*
Retorna o número de habitantes contidos na área da densidade.
O parâmetro densidade não pode ser nulo.
*/
double densidade_calcular_habitantes(Densidade densidade);

/*
Retorna verdadeiro se um ponto é contido dentro da densidade.
O parâmetro densidade não pode ser nulo.
*/
bool densidade_contem_ponto(Densidade densidade, double x, double y);

/*
Libera a memória alocada por uma densidade.
O parâmetro densidade não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada. O ponteiro não poderá ser utilizado após isso!
*/
void densidade_destruir(Densidade densidade);

#endif
