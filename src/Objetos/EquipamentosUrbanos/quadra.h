#ifndef QUADRA_H
#define QUADRA_H

/*
Este módulo define uma Quadra, o qual representa um retângulo com cores e rótulo especifico ao seu
tipo para fácil diferenciação no arquivo svg, assim como funções específicas ao seu tipo como
quadra_inicializar_coordenada e quadra_definir_cor_sombra.

Suas propriedades são:
    id: Identificador da quadra.
    largura
    altura
    x: Coordenada x da quadra no plano.
    y: Coordenada y da quadra no plano.
    cor da borda
    cor de preenchimento
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
typedef struct Quadra_s *Quadra;

/*
Cria e inicializa uma Quadra com os valores passados.
A largura e altura devem ser maiores que 0. O id e as cores não podem ser nulos.
O usuário é responsável por liberar a memória alocada!
*/
Quadra quadra_criar(const char id[100], double largura, double altura, double x, double y);

/*
Cria e inicializa uma Quadra a partir de um texto obtido no arquivo .geo.
O parâmetro linha não pode ser nulo.
O usuário é responsável por liberar a memória alocada!
*/
Quadra quadra_ler(const char *linha);

// Encontra e inicializa as coordenadas x e y de um elemento que seja contido por uma quadra.
// Argumentos:
//      largura: largura do elemento, usado para centralizar-lo.
//      altura: altura do elemento, usado para centralizar-lo.
//      quadra: Quadra onde o elemento se encontra.
//      face: Lado da quadra onde o elemento se encontra.
//      numero: Distância do elemento e o início da face.
// Inicializa os parâmetros x e y com as coordenadas derivadas a partir da quadra.
void quadra_inicializar_coordenada(double *x, double *y, double largura, double altura,
                                   Quadra quadra, char face, int numero);

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
void quadra_definir_espessura_borda(Quadra quadra, double espessura_borda);

/*
Define se a quadra tem borda tracejada.
O parâmetro quadra não pode ser nulo.
*/
void quadra_definir_borda_tracejada(Quadra quadra, bool tracejado);

/*
Define o arredondamento da borda de uma quadra.
O parâmetro quadra não pode ser nulo.
*/
void quadra_definir_arredondamento_borda(Quadra quadra, double arredondamento_borda);

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
