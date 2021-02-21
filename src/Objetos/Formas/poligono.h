#ifndef POLIGONO_H
#define POLIGONO_H

/*
Este módulo define um Poligono, o qual possui propriedades coordenadas espaciais, assim como
propriedades para alterar sua visualização no arquivo svg, como as cores de borda e preenchimento e
opacidade.

Suas propriedades são:
    largura
    altura
    x: Coordenada x do círculo no plano.
    y: Coordenada y do círculo no plano.
    cor da borda
    cor de preenchimento
    opacidade: Opacidade do polígono no svg.
*/

#include <stdbool.h>
#include <stdio.h>

/*
Foi usado 'typedef struct *' em vez de 'typedef void *' para permitir ao compilador diferenciar os
tipos, não afetando o encapsulamento de dados!
O struct *não é definido neste header* e sim em seu arquivo .c correspondente, o ponteiro deste tipo
não pode ser criado sem seu construtor, assim como suas propriedades não podem ser acessadas sem uso
de getters/setters.
*/
typedef struct Poligono_s *Poligono;

/*
Cria e inicializa um Poligono com os valores passados.
    pontos: Array de pontos que compõem o polígono.
    numero_de_pontos: Número de pontos que compõem o polígono.
    opacidade: opacidade do polígono no svg.
O id e as cores não podem ser nulos.
A opacidade deve estar entre 0 e 1 (inclusivo).
O usuário é responsável por liberar a memória alocada!
*/
Poligono poligono_criar(double **pontos, int numero_de_pontos, const char cor_borda[20],
                        const char cor_preenchimento[20], double opacidade);

/*
Escreve o código svg necessário para representar um polígono em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void poligono_escrever_svg(Poligono poligono, FILE *arquivo);

/*
Escreve todas as informações presentes em um polígono em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void poligono_escrever_informacoes(Poligono poligono, FILE *arquivo);

/*
Calcula a área de um polígono.
O parâmetro poligono não pode ser nulo.
Retorna a área de um polígono.
*/
double poligono_calcular_area(Poligono poligono);

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
Obtém a coordenada x onde um polígono termina.
O parâmetro circulo não pode ser nulo.
Retorna a coordenada x onde um polígono termina.
*/
double poligono_obter_x_fim(Poligono poligono);

/*
Obtém a coordenada y onde um polígono termina.
O parâmetro circulo não pode ser nulo.
Retorna a coordenada y onde um polígono termina.
*/
double poligono_obter_y_fim(Poligono poligono);

/*
Calcula a coordenada x do centróide de um polígono.
O parâmetro poligono não pode ser nulo.
Retorna a coordenada x do centróide de um polígono.
*/
double poligono_obter_x_centro(Poligono poligono);

/*
Calcula a coordenada y do centróide de um polígono.
O parâmetro poligono não pode ser nulo.
Retorna a coordenada y do centróide de um polígono.
*/
double poligono_obter_y_centro(Poligono poligono);

// Encontra e inicializa as coordenadas x e y da centroide de um polígono.
// Argumentos:
//      poligono: Polígono a ser obtido as coordenadas do centroide.
//      x: Coordenada x
//      y: Coordenada y
// Inicializa os parâmetros x e y com as coordenadas encontradas.
void poligono_inicializar_centroide(Poligono poligono, double *x, double *y);

/*
Obtém a cor da borda de um polígono.
O parâmetro circulo não pode ser nulo.
Retorna a cor da borda de um polígono.
*/
const char *poligono_obter_cor_borda(Poligono poligono);

/*
Define a cor da borda de um polígono.
Nenhum dos parâmetros podem ser nulos.
*/
void poligono_definir_cor_borda(Poligono poligono, const char *cor_borda);

/*
Obtém a cor de preenchimento de um polígono.
O parâmetro circulo não pode ser nulo.
Retorna a cor de preenchimento de um polígono.
*/
const char *poligono_obter_cor_preenchimento(Poligono poligono);

/*
Define a cor de preenchimento de um polígono.
Nenhum dos parâmetros podem ser nulos.
*/
void poligono_definir_cor_preenchimento(Poligono poligono, const char *cor_preenchimento);

/*
Libera a memória alocada por um polígono.
O parâmetro poligono não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada. O ponteiro não poderá ser utilizado após isso!
*/
void poligono_destruir(Poligono poligono);

#endif
