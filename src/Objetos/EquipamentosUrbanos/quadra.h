#ifndef QUADRA_H
#define QUADRA_H

/*
Este módulo define uma Quadra, o qual representa um retângulo com cores e rótulo especifico ao seu
tipo para fácil diferenciação no arquivo svg.

Suas propriedades são:
    id: Identificador da quadra.
    largura
    altura
    x: Coordenada x da quadra no plano.
    y: Coordenada y da quadra no plano.
    cor_borda
    cor_preenchimento
    arredondamento_borda;
    espessura_borda
*/

#include <stdbool.h>
#include <stdio.h>

typedef void *Quadra;

/*
Cria e inicializa uma Quadra com os valores passados.
A largura e altura devem ser maiores que 0. O id e as cores não podem ser nulos.
O usuário é responsável por liberar a memória alocada!
*/
Quadra quadra_criar(const char id[100], double largura, double altura, double x, double y,
                    const char cor_borda[20], const char cor_preenchimento[20]);

/*
Cria e inicializa uma Quadra a partir de um texto obtido no arquivo .geo.
O parâmetro linha não pode ser nulo.
O usuário é responsável por liberar a memória alocada!
*/
Quadra quadra_ler(const char *linha);

/*
Obtém o id de uma quadra.
O parâmetro quadra não pode ser nulo.
Retorna o id de uma quadra.
*/
const char *quadra_obter_id(Quadra quadra);

/*
Obtém a coordenada x de um quadra.
O parâmetro quadra não pode ser nulo.
Retorna a coordenada x de uma quadra.
*/
double quadra_obter_x(Quadra quadra);

/*
Obtém a coordenada y de uma quadra.
O parâmetro quadra não pode ser nulo.
Retorna o raio de uma quadra.
*/
double quadra_obter_y(Quadra quadra);

/*
Obtém a largura de uma quadra.
O parâmetro quadra não pode ser nulo.
Retorna a largura de uma quadra.
*/
double quadra_obter_largura(Quadra quadra);

/*
Obtém a altura de uma quadra.
O parâmetro quadra não pode ser nulo.
Retorna a altura de uma quadra.
*/
double quadra_obter_altura(Quadra quadra);

/*
Obtém a cor de preenchimento de uma quadra.
O parâmetro quadra não pode ser nulo.
Retorna a cor de preenchimento de uma quadra.
*/
const char *quadra_obter_cor_preenchimento(Quadra quadra);

/*
Define a cor de preenchimento de uma quadra.
Nenhum dos parâmetros podem ser nulos.
*/
void quadra_definir_cor_preenchimento(Quadra quadra, const char *cor_preenchimento);

/*
Obtém a cor da borda de uma quadra.
O parâmetro quadra não pode ser nulo.
Retorna a cor da borda de uma quadra.
*/
const char *quadra_obter_cor_borda(Quadra quadra);

/*
Define a cor da borda de uma quadra.
Nenhum dos parâmetros podem ser nulos.
*/
void quadra_definir_cor_borda(Quadra quadra, const char *cor_borda);

/*
Define a espessura da borda de uma quadra.
Nenhum dos parâmetros podem ser nulos.
*/
void quadra_definir_espessura_borda(Quadra quadra, const char *espessura_borda);

/*
Define se a quadra tem borda tracejada.
O parâmetro quadra não pode ser nulo.
*/
void quadra_definir_borda_tracejada(Quadra quadra, bool tracejado);

/*
Define o arredondamento da borda de uma quadra.
O parâmetro quadra não pode ser nulo.
*/
void quadra_definir_arredondamento_borda(Quadra quadra, double raio_borda);

/*
Define a cor da sombra de uma quadra. A cor deve estar definida anteriormente no arquivo svg.
Nenhum dos parâmetros podem ser nulos.
*/
void quadra_definir_cor_sombra(Quadra quadra, const char *cor_sombra);

/*
Libera a memória alocada por uma quadra.
O parâmetro quadra não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada. O ponteiro não poderá ser utilizado após isso!
*/
void quadra_destruir(Quadra quadra);

#endif
