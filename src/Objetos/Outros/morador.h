#ifndef MORADOR_H
#define MORADOR_H

#include <stdbool.h>
#include <stdio.h>

typedef void *Morador;

/*
Cria e inicializa um Morador com os valores passados.
O cpf, nome e sobrenomem não podem ser nulos.
O usuário é responsável por liberar a memória alocada!
*/
Morador morador_criar(const char cpf[100], const char nome[100], const char sobrenome[100],
                      const char sexo, int dia, int mes, int ano);

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
void morador_ler_endereco(Morador morador, const char *linha);

/*
Define o endereço de um morador com os valores passados.
O morador e cep não podem ser nulos.
*/
void morador_definir_endereco(Morador morador, const char *cep, const char face, int num,
                              const char *complemento);

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
const char morador_obter_sexo(Morador morador);

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
Obtém a face do endereço de um morador.
O parâmetro morador não pode ser nulo.
Retorna a face do endereço de um morador.
*/
const char morador_obter_endereco_face(Morador morador);

/*
Obtém o número do endereço de um morador.
O parâmetro morador não pode ser nulo.
Retorna o número do endereço de um morador.
*/
int morador_obter_endereco_num(Morador morador);

/*
Libera a memória alocada por um morador.
O parâmetro morador não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada. O ponteiro não poderá ser utilizado após isso!
*/
void morador_destruir(Morador morador);

#endif
