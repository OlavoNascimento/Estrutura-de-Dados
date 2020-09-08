#ifndef HIDRANTE_H
#define HIDRANTE_H

#include <stdbool.h>
#include <stdio.h>

#include "circulo.h"

/*
Este módulo define o tipo Hidrante, assim como funções que criam, acessam suas propriedades e
realizam operações entre variáveis deste tipo.
*/

typedef void *Hidrante;

/*
Cria e inicializa um struct Hidrante com os valores passados.
O raio deve ser maior que 0. O id e as cores não podem ser nulos.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Hidrante hidrante_criar(const char id[100], double raio, double x, double y,
                        const char cor_borda[20], const char cor_preenchimento[20]);

/*
Cria e inicializa um struct Hidrante a partir de um texto obtido no arquivo .geo.
O parâmetro linha não pode ser nulo.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Hidrante hidrante_ler(const char *linha);

/*
Escreve todas as informações presentes em um hidrante em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void hidrante_escrever_informacoes(FILE *arquivo, Hidrante hidrante);

/*
Escreve o código svg necessário para representar um hidrante em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void hidrante_escrever_svg(FILE *arquivo, Hidrante hidrante);

/*
Obtém o id de um hidrante.
O parâmetro hidrante não pode ser nulo.
Retorna o id de um hidrante.
*/
const char *hidrante_obter_id(Hidrante hidrante);

/*
Obtém o raio de um hidrante.
O parâmetro hidrante não pode ser nulo.
Retorna o raio de um hidrante.
*/
double hidrante_obter_raio(Hidrante hidrante);

/*
Obtém a coordenada x de um hidrante.
O parâmetro hidrante não pode ser nulo.
Retorna a coordenada x de um hidrante.
*/
double hidrante_obter_x(Hidrante hidrante);
/*
Obtém a coordenada y de um hidrante.
O parâmetro hidrante não pode ser nulo.
Retorna a coordenada y de um hidrante.
*/
double hidrante_obter_y(Hidrante hidrante);

/*
Obtém a cor de preenchimento de um hidrante.
O parâmetro hidrante não pode ser nulo.
Retorna a cor de preenchimento de um hidrante.
*/
const char *hidrante_obter_cor_preenchimento(Hidrante hidrante);
/*
Define a cor de preenchimento de um hidrante.
Nenhum dos parâmetros podem ser nulos.
*/
void hidrante_definir_cor_preenchimento(Hidrante hidrante, const char *cor_preenchimento);

/*
Obtém a cor da borda de um hidrante.
O parâmetro hidrante não pode ser nulo.
Retorna a cor da borda de um hidrante.
*/
const char *hidrante_obter_cor_borda(Hidrante hidrante);
/*
Define a cor da borda de um hidrante.
Nenhum dos parâmetros podem ser nulos.
*/
void hidrante_definir_cor_borda(Hidrante hidrante, const char *cor_borda);

/*
Define a espessura da borda de um hidrante.
Nenhum dos parâmetros podem ser nulos.
*/
void hidrante_definir_espessura_borda(Hidrante hidrante, const char *espessura_borda);

/*
Libera a memória alocada por um hidrante.
O parâmetro hidrante não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada pelo struct. O ponteiro não poderá ser utilizado após isso!
*/
void hidrante_destruir(Hidrante hidrante);

#endif
