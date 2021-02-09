#ifndef MORADOR_H
#define MORADOR_H

/*
Este módulo define um Morador, o qual representa um retângulo com informações adicionais
como cpf, nome, sobrenome e data de nascimento
 ademais possui cores e rótulo específico ao seu tipo para fácil
diferenciação no arquivo svg.

Suas propriedades são:
    id: cpf do morador
    largura
    altura
    x: Coordenada x do morador no plano.
    y: Coordenada y do morador no plano.
    cor da borda
    cor de preenchimento
    tipo: tipo de morador.
    nome: nome do morador.
    sobrenome: sobrenome do morador.
    dia/mes/ano: data de nascimento do morador
*/

#include <stdbool.h>
#include <stdio.h>

#include "../EquipamentosUrbanos/quadra.h"

typedef struct Morador_s *Morador;

/*
Cria e inicializa um Morador com os valores passados.
Argumentos:
    cpf: Identificador do morador sendo criado.
    nome: Nome do morador.
    sobrenome: Sobrenome do morador.
    sexo: M sendo másculino ou F sendo feminino.
    dia: Dia de nascimento do morador.
    mes: Mês de nascimento do morador.
    ano: Ano de nascimento do morador.
cpf, nome e sobrenome nao podem ser nulos
O usuário é responsável por liberar a memória alocada!
*/
Morador morador_criar(const char *cpf, const char *nome, const char *sobrenome, const char sexo,
                      int dia, int mes, int ano);

/*
Cria e inicializa um Morador a partir de um texto obtido no arquivo .pm.
O parâmetro linha não pode ser nulo.
O usuário é responsável por liberar a memória alocada!
*/
Morador morador_ler(const char *linha);

/*
Define o endereço de um morador a partir de um texto obtido no arquivo.pm.
Nenhum dos parâmetros pode ser nulo.
*/
void morador_ler_endereco(Morador morador, const char *linha, Quadra quadra);

/*
Define o endereço de um morador com os valores passados.
O morador é inserido em uma quadtree assim que as suas
coordenadas são obtidas através do endereço.
O morador e cep não podem ser nulos.
*/
void morador_definir_endereco(Morador morador, const char *cep, const char face, int num,
                              const char *complemento, Quadra quadra);

/*
Obtém o id(cpf) de um morador.
O parâmetro morador não pode ser nulo.
Retorna o cpf de um morador.
*/
const char *morador_obter_id(Morador morador);

/*
Obtém o nome de um morador.
O parâmetro morador não pode ser nulo.
Retorna o nome de um morador.
*/
const char *morador_obter_nome(Morador morador);

/*
Obtém o sobrenome de um morador.
O parâmetro morador não pode ser nulo.
Retorna o sobrenome de um morador.
*/
const char *morador_obter_sobrenome(Morador morador);

/*
Obtém o sexo de um morador.
O parâmetro morador não pode ser nulo.
Retorna o sexo de um morador.
*/
char morador_obter_sexo(Morador morador);

/*
Obtém a data de nascimento de um morador.
O parâmetro morador não pode ser nulo.
Retorna a data de nascimento de um morador no formata dd/mm/aaaa.
*/
const char *morador_obter_data(Morador morador);

/*
Obtém o cep de um morador.
O parâmetro morador não pode ser nulo.
Retorna o cep de um morador.
*/
const char *morador_obter_endereco_cep(Morador morador);

/*
Obtém o complemento do endereço de um morador.
O parâmetro morador não pode ser nulo.
Retorna o complemento do endereço de um morador.
*/
const char *morador_obter_endereco_complemento(Morador morador);

/*
Obtém a face do endereço de um morador.
O parâmetro morador não pode ser nulo.
Retorna a face do endereço de um morador.
*/
char morador_obter_endereco_face(Morador morador);

/*
Obtém o número do endereço de um morador.
O parâmetro morador não pode ser nulo.
Retorna o número do endereço de um morador.
*/
int morador_obter_endereco_num(Morador morador);

// /////////////////////////
/*
Obtém a coordenada x de um morador.
O parâmetro morador não pode ser nulo.
Retorna a coordenada x de um morador.
*/
double morador_obter_x(Morador morador);

/*
Obtém a coordenada y de um morador.
O parâmetro morador não pode ser nulo.
Retorna o raio de um morador.
*/
double morador_obter_y(Morador morador);

/*
Obtém a largura de um morador.
O parâmetro morador não pode ser nulo.
Retorna a largura de um morador.
*/
double morador_obter_largura(Morador morador);

/*
Obtém a altura de um morador.
O parâmetro morador não pode ser nulo.
Retorna a altura de um morador.
*/
double morador_obter_altura(Morador morador);

/*
Obtém a cor de preenchimento de um morador.
O parâmetro morador não pode ser nulo.
Retorna a cor de preenchimento de um morador.
*/
const char *morador_obter_cor_preenchimento(Morador morador);

/*
Define a cor de preenchimento de um morador.
Nenhum dos parâmetros podem ser nulos.
*/
void morador_definir_cor_preenchimento(Morador morador, const char *cor_preenchimento);

/*
Obtém a cor da borda de um morador.
O parâmetro morador não pode ser nulo.
Retorna a cor da borda de um morador.
*/
const char *morador_obter_cor_borda(Morador morador);

/*
Define a cor da borda de um morador.
Nenhum dos parâmetros podem ser nulos.
*/
void morador_definir_cor_borda(Morador morador, const char *cor_borda);

/*
Define a espessura da borda de um morador.
Nenhum dos parâmetros podem ser nulos.
*/
void morador_definir_espessura_borda(Morador morador, const char *espessura_borda);

/*
Define se o morador tem borda tracejada.
O parâmetro morador não pode ser nulo.
*/
void morador_definir_borda_tracejada(Morador morador, bool tracejado);

/*
Define o arredondamento da borda de um morador.
O parâmetro morador não pode ser nulo.
*/
void morador_definir_arredondamento_borda(Morador morador, double raio_borda);

/*
Libera a memória alocada por um morador.
O parâmetro morador não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada. O ponteiro não poderá ser utilizado após isso!
*/
void morador_destruir(Morador morador);

#endif
