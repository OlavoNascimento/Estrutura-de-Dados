#ifndef POLIGONO_H
#define POLIGONO_H

#include <stdbool.h>
#include <stdio.h>

/*
Este módulo define o tipo Poligono, assim como funções que criam, acessam suas propriedades e
realizam operações entre variáveis deste tipo.
*/

typedef void *Poligono;

/*
Cria e inicializa uma struct Poligono com os valores passados.
A largura e altura devem ser maiores que 0. O id e as cores não podem ser nulos.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Poligono poligono_criar(double **pontos, int numero_de_pontos, const char cor_borda[20],
                        const char cor_preenchimento[20], double opacidade);

/*
Escreve o código svg necessário para representar um polígono em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void poligono_escrever_svg(FILE *arquivo, Poligono poligono);

/*
Obtém a largura de um polígono.
O parâmetro poligono não pode ser nulo.
Retorna a largura de um polígono.
*/
double poligono_obter_largura(Poligono poligono);

/*
Obtém a altura de um polígono.
O parâmetro poligono não pode ser nulo.
Retorna a altura de um polígono.
*/
double poligono_obter_altura(Poligono poligono);

/*
Obtém a coordenada x de um polígono.
O parâmetro poligono não pode ser nulo.
Retorna a coordenada x de um polígono.
*/
double poligono_obter_x(Poligono poligono);

/*
Obtém a coordenada y de um polígono.
O parâmetro poligono não pode ser nulo.
Retorna a coordenada y de um polígono.
*/
double poligono_obter_y(Poligono poligono);

/*
Calcula a área de um polígono.
O parâmetro poligono não pode ser nulo.
Retorna a área de um polígono.
*/
double poligono_calcular_area(Poligono poligono);

/*
Calcula a coordenada x do centróide de um polígono.
O parâmetro poligono não pode ser nulo.
Retorna a coordenada x do centróide de um polígono.
*/
double poligono_calcular_x_centroide(Poligono poligono);

/*
Calcula a coordenada y do centróide de um polígono.
O parâmetro poligono não pode ser nulo.
Retorna a coordenada y do centróide de um polígono.
*/
double poligono_calcular_y_centroide(Poligono poligono);

/*
Libera a memória alocada por um polígono.
O parâmetro poligono não pode ser nulo e deve apontar para um espaço de memória
reservada. Libera a memória alocada pelo struct. O ponteiro não poderá ser utilizado
após isso!
*/
void poligono_destruir(Poligono poligono);

#endif
