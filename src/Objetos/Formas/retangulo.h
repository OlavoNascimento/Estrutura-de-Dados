#ifndef RETANGULO_H
#define RETANGULO_H

/*
Este módulo define um Retangulo, o qual possui propriedades inerentes ao seu tipo como altura,
largura e suas coordenadas espaciais, assim como propriedades para alterar sua visualização no
arquivo svg, como as cores de borda e preenchimento.

Suas propriedades são:
    id: Identificador do retângulo.
    largura
    altura
    x: Coordenada x do retângulo no plano.
    y: Coordenada y do retângulo no plano.
    cor_borda
    cor_preenchimento
    arredondamento_borda
    espessura_borda
    borda_tracejada
*/

#include <stdbool.h>
#include <stdio.h>

typedef void *Retangulo;

/*
Cria e inicializa uma Retangulo com os valores passados.
A largura e altura devem ser maiores que 0. O id e as cores não podem ser nulos.
O usuário é responsável por liberar a memória alocada!
*/
Retangulo retangulo_criar(const char id[100], double largura, double altura, double x, double y,
                          const char cor_borda[20], const char cor_preenchimento[20]);

/*
Cria e inicializa uma Retangulo a partir de um texto obtido no arquivo .geo.
O parâmetro linha não pode ser nulo.
O usuário é responsável por liberar a memória alocada!
*/
Retangulo retangulo_ler(const char *linha);

/*
Escreve todas as informações presentes em um retângulo em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void retangulo_escrever_informacoes(Retangulo retangulo, FILE *arquivo);

/*
Escreve o código svg necessário para representar um retângulo em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void retangulo_escrever_svg(Retangulo retangulo, FILE *arquivo);

/*
Verifica se dois retângulos se intersectam.
Nenhum dos parâmetros podem ser nulos.
Retorna verdadeiro caso os dois retângulos se intersectam, caso contrário retorna falso.
*/
bool retangulo_checar_interseccao(Retangulo retangulo1, Retangulo retangulo2);

/*
Verifica se o retangulo1 contem o retangulo2.
Nenhum dos parâmetros podem ser nulos.
Retorna verdadeiro caso o retangulo1 contenha o retangulo2.
*/
bool retangulo_contem_retangulo(Retangulo retangulo1, Retangulo retangulo2);

/*
Verifica se um ponto é contido por um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna verdadeiro caso o ponto se encontre contido pelo retangulo, caso contrário retorna falso.
*/
bool retangulo_checar_ponto_interno(Retangulo retangulo, double ponto_x, double ponto_y);

/*
Obtém o id de um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna o id de um retângulo.
*/
const char *retangulo_obter_id(Retangulo retangulo);

/*
Obtém a largura de um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna a largura de um retângulo.
*/
double retangulo_obter_largura(Retangulo retangulo);

/*
Obtém a altura de um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna a altura de um retângulo.
*/
double retangulo_obter_altura(Retangulo retangulo);

/*
Obtém a coordenada x de um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna a coordenada x de um retângulo.
*/
double retangulo_obter_x(Retangulo retangulo);

/*
Obtém a coordenada y de um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna a coordenada y de um retângulo.
*/
double retangulo_obter_y(Retangulo retangulo);

/*
Obtém a coordenada x onde um retângulo termina.
O parâmetro retangulo não pode ser nulo.
Retorna a coordenada x onde um retângulo termina.
*/
double retangulo_obter_x_fim(Retangulo retangulo);

/*
Obtém a coordenada y onde um retângulo termina.
O parâmetro retangulo não pode ser nulo.
Retorna a coordenada y onde um retângulo termina.
*/
double retangulo_obter_y_fim(Retangulo retangulo);

/*
Obtém a coordenada x onde o centro de um retângulo se encontra.
O parâmetro retangulo não pode ser nulo.
Retorna a coordenada x onde o centro de um retângulo se encontra.
*/
double retangulo_obter_x_centro(Retangulo retangulo);

/*
Obtém a coordenada y onde o centro de um retângulo se encontra.
O parâmetro retangulo não pode ser nulo.
Retorna a coordenada y onde o centro de um retângulo se encontra.
*/
double retangulo_obter_y_centro(Retangulo retangulo);

/*
Obtém a cor da borda de um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna a cor da borda de um retângulo.
*/
const char *retangulo_obter_cor_borda(Retangulo retangulo);

/*
Define a cor da borda de um retângulo.
Nenhum dos parâmetros podem ser nulos.
*/
void retangulo_definir_cor_borda(Retangulo retangulo, const char *cor_borda);

/*
Obtém a cor de preenchimento de um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna a cor de preenchimento de um retângulo.
*/
const char *retangulo_obter_cor_preenchimento(Retangulo retangulo);

/*
Define a cor de preenchimento de um retângulo.
Nenhum dos parâmetros podem ser nulos.
*/
void retangulo_definir_cor_preenchimento(Retangulo retangulo, const char *cor_preenchimento);

/*
Obtém a espessura da borda de um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna a espessura da borda de um retângulo.
*/
const char *retangulo_obter_espessura_borda(Retangulo retangulo);

/*
Define a espessura da borda de um retângulo.
Nenhum dos parâmetros podem ser nulos.
*/
void retangulo_definir_espessura_borda(Retangulo retangulo, const char *espessura_borda);

/*
Define se o retangulo tem borda tracejada.
O parâmetro retangulo não pode ser nulo.
*/
void retangulo_definir_borda_tracejada(Retangulo retangulo, bool tracejado);

/*
Define o arredondamento da borda de um retângulo.
O parâmetro retangulo não pode ser nulo.
*/
void retangulo_definir_arredondamento_borda(Retangulo retangulo, double raio_borda);

/*
Libera a memória alocada por um retângulo.
O parâmetro retangulo não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada. O ponteiro não poderá ser utilizado após isso!
*/
void retangulo_destruir(Retangulo retangulo);

#endif
