#ifndef RADIO_H
#define RADIO_H

/*
Este módulo define um Rádio, o qual representa um círculo com cores e rótulo específico ao seu
tipo para fácil diferenciação no arquivo svg.

Suas propriedades são:
    id: Identificador do rádio
    raio
    x: Coordenada x do rádio no plano.
    y: Coordenada y do rádio no plano.
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
typedef struct Radio_s *Radio;

/*
Cria e inicializa um Radio com os valores passados.
O raio deve ser maior que 0. O id e as cores não podem ser nulos.
O usuário é responsável por liberar a memória alocada!
*/
Radio radio_criar(const char id[100], double x, double y);

/*
Cria e inicializa um Radio a partir de um texto obtido no arquivo .geo.
O parâmetro linha não pode ser nulo.
O usuário é responsável por liberar a memória alocada!
*/
Radio radio_ler(const char *linha);

/*
Obtém o id de um rádio.
O parâmetro radio não pode ser nulo.
Retorna o id de um rádio.
*/
const char *radio_obter_id(Radio radio);

/*
Obtém a coordenada x de um rádio.
O parâmetro radio não pode ser nulo.
Retorna a coordenada x de um rádio.
*/
double radio_obter_x(Radio radio);

/*
Obtém a coordenada y de um rádio.
O parâmetro radio não pode ser nulo.
Retorna a coordenada y de um rádio.
*/
double radio_obter_y(Radio radio);

/*
Obtém o raio de um rádio.
O parâmetro radio não pode ser nulo.
Retorna o raio de um rádio.
*/
double radio_obter_raio(Radio radio);

/*
Obtém a cor de preenchimento de um rádio.
O parâmetro radio não pode ser nulo.
Retorna a cor de preenchimento de um rádio.
*/
const char *radio_obter_cor_preenchimento(Radio radio);

/*
Define a cor de preenchimento de um rádio.
Nenhum dos parâmetros podem ser nulos.
*/
void radio_definir_cor_preenchimento(Radio radio, const char *cor_preenchimento);

/*
Obtém a cor da borda de um rádio.
O parâmetro radio não pode ser nulo.
Retorna a cor da borda de um rádio.
*/
const char *radio_obter_cor_borda(Radio radio);

/*
Define a cor da borda de um rádio.
Nenhum dos parâmetros podem ser nulos.
*/
void radio_definir_cor_borda(Radio radio, const char *cor_borda);

/*
Define a espessura da borda de um rádio.
Nenhum dos parâmetros podem ser nulos.
*/
void radio_definir_espessura_borda(Radio radio, double espessura_borda);

/*
Define a opacidade de um radio.
O parâmetro radio não pode ser nulo e a opacidade dever maior ou igual a zero e menor ou igual
a 1.
*/
void radio_definir_opacidade(Radio radio, double opacidade);

/*
Libera a memória alocada por um radio.
O parâmetro radio não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada. O ponteiro não poderá ser utilizado após isso!
*/
void radio_destruir(Radio radio);

#endif
