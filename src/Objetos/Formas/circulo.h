#ifndef CIRCULO_H
#define CIRCULO_H

/*
Este módulo define um Circulo, o qual possui propriedades inerentes ao seu tipo como raio e suas
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

/*
Foi usado 'typedef struct *' em vez de 'typedef void *' para permitir ao compilador diferenciar os
tipos, não afetando o encapsulamento de dados!
O struct *não é definido neste header* e sim em seu arquivo .c correspondente, o ponteiro deste tipo
não pode ser criado sem seu construtor, assim como suas propriedades não podem ser acessadas sem uso
de getters/setters.
*/
typedef struct Circulo_s *Circulo;

/*
Cria e inicializa um Circulo com os valores passados.
O raio deve ser maior que 0. O id e as cores não podem ser nulos.
O usuário é responsável por liberar a memória alocada!
*/
Circulo circulo_criar(const char id[100], double raio, double x, double y, const char cor_borda[20],
                      const char cor_preenchimento[20]);

/*
Cria e inicializa um Circulo a partir de um texto obtido no arquivo .geo.
O parâmetro linha não pode ser nulo.
O usuário é responsável por liberar a memória alocada!
*/
Circulo circulo_ler(const char *linha);

/*
Verifica se dois círculos se intersectam.
Nenhum dos parâmetros podem ser nulos.
Retorna verdadeiro caso os dois círculos se intersectam, caso contrário retorna falso.
*/
bool circulo_checar_interseccao(Circulo circulo1, Circulo circulo2);

/*
Verifica se o circulo1 contem o circulo2.
Nenhum dos parâmetros podem ser nulos.
Retorna verdadeiro caso o circulo1 contenha o circulo2.
*/
bool circulo_contem_circulo(Circulo circulo1, Circulo circulo2);

/*
Verifica se um ponto é contido por um círculo.
O parâmetro circulo não pode ser nulo.
Retorna verdadeiro caso o ponto se encontre contido pelo circulo, caso contrário retorna falso.
*/
bool circulo_checar_ponto_interno(Circulo circulo, double ponto_x, double ponto_y);

/*
Escreve todas as informações presentes em um círculo em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void circulo_escrever_informacoes(Circulo circulo, FILE *arquivo);

/*
Escreve o código svg necessário para representar um círculo em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void circulo_escrever_svg(Circulo circulo, FILE *arquivo);

/*
Obtém o id de um círculo.
O parâmetro circulo não pode ser nulo.
Retorna o id de um círculo.
*/
const char *circulo_obter_id(Circulo circulo);

/*
Obtém o raio de um círculo.
O parâmetro circulo não pode ser nulo.
Retorna o raio de um círculo.
*/
double circulo_obter_raio(Circulo circulo);

/*
Obtém a coordenada x de um círculo.
O parâmetro circulo não pode ser nulo.
Retorna a coordenada x de um círculo.
*/
double circulo_obter_x(Circulo circulo);

/*
Obtém a coordenada y de um círculo.
O parâmetro circulo não pode ser nulo.
Retorna a coordenada y de um círculo.
*/
double circulo_obter_y(Circulo circulo);

/*
Obtém a coordenada x onde um círculo se inicia.
O parâmetro circulo não pode ser nulo.
Retorna a coordenada x onde um círculo se inicia.
*/
double circulo_obter_x_inicio(Circulo circulo);

/*
Obtém a coordenada y onde um círculo se inicia.
O parâmetro circulo não pode ser nulo.
Retorna a coordenada y onde um círculo se inicia.
*/
double circulo_obter_y_inicio(Circulo circulo);

/*
Obtém a coordenada x onde um círculo termina.
O parâmetro circulo não pode ser nulo.
Retorna a coordenada x onde um círculo termina.
*/
double circulo_obter_x_fim(Circulo circulo);

/*
Obtém a coordenada y onde um círculo termina.
O parâmetro circulo não pode ser nulo.
Retorna a coordenada y onde um círculo termina.
*/
double circulo_obter_y_fim(Circulo circulo);

/*
Obtém a cor da borda de um círculo.
O parâmetro circulo não pode ser nulo.
Retorna a cor da borda de um círculo.
*/
const char *circulo_obter_cor_borda(Circulo circulo);

/*
Define a cor da borda de um círculo.
Nenhum dos parâmetros podem ser nulos.
*/
void circulo_definir_cor_borda(Circulo circulo, const char *cor_borda);

/*
Obtém a cor de preenchimento de um círculo.
O parâmetro circulo não pode ser nulo.
Retorna a cor de preenchimento de um círculo.
*/
const char *circulo_obter_cor_preenchimento(Circulo circulo);

/*
Define a cor de preenchimento de um círculo.
Nenhum dos parâmetros podem ser nulos.
*/
void circulo_definir_cor_preenchimento(Circulo circulo, const char *cor_preenchimento);

/*
Obtém a espessura da borda de um círculo.
O parâmetro circulo não pode ser nulo.
Retorna a espessura da borda de um círculo.
*/
const char *circulo_obter_espessura_borda(Circulo circulo);

/*
Define a espessura da borda de um círculo.
Nenhum dos parâmetros podem ser nulos.
*/
void circulo_definir_espessura_borda(Circulo circulo, const char *espessura_borda);

/*
Define a opacidade de um círculo.
O parâmetro circulo não pode ser nulo e a opacidade dever maior ou igual a zero e menor ou igual
a 1.
*/
void circulo_definir_opacidade(Circulo circulo, double opacidade);

/*
Libera a memória alocada por um círculo.
O parâmetro circulo não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada. O ponteiro não poderá ser utilizado após isso!
*/
void circulo_destruir(Circulo circulo);

#endif
