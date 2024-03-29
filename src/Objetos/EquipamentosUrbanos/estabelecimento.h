#ifndef ESTABELECIMENTO_H
#define ESTABELECIMENTO_H

/*
Este módulo define um Estabelecimento, o qual representa um retângulo com informações adicionais
como cnpj, cpf, tipo e nome, ademais possui cores e rótulo específico ao seu tipo para fácil
diferenciação no arquivo svg.

Suas propriedades são:
    id: cnpj do estabelecimento.
    largura
    altura
    x: Coordenada x do estabelecimento no plano.
    y: Coordenada y do estabelecimento no plano.
    cor de borda
    cor de preenchimento
    tipo: tipo de estabelecimento.
    nome: nome do estabelecimento.
    cpf: cpf do proprietário.
*/

#include <stdbool.h>
#include <stdio.h>

#include "quadra.h"

/*
Foi usado 'typedef struct *' em vez de 'typedef void *' para permitir ao compilador diferenciar os
tipos, não afetando o encapsulamento de dados!
O struct *não é definido neste header* e sim em seu arquivo .c correspondente, o ponteiro deste tipo
não pode ser criado sem seu construtor, assim como suas propriedades não podem ser acessadas sem uso
de getters/setters.
*/
typedef struct Estabelecimento_s *Estabelecimento;

/*
Cria e inicializa um Estabelecimento com os valores passados.
Argumentos:
    cnpj: Cnpj do estabelecimento.
    cpf: Cpf do proprietário.
    tipo: Tipo do estabelecimento.
    nome: Nome do estabelecimento.
    quadra: Quadra onde o estabelecimento está contido.
    face: Lado da quadra onde o Estabelecimento está presente, deve ser uma das seguintes opções: N,
S, L, O.
    numero: Distância do estabelecimento e extremidade da face.
Os argumentos cnpj, cpf, tipo, nome e quadra não podem ser nulos.
O usuário é responsável por liberar a memória alocada!
*/
Estabelecimento estabelecimento_criar(const char *cnpj, const char *cpf, const char *tipo,
                                      const char *nome, Quadra quadra, char face, int numero);
/*
Cria e inicializa um Estabelecimento a partir de um texto obtido no arquivo .geo.
Nenhum dos parâmetros podem ser nulos.
O usuário é responsável por liberar a memória alocada!
*/
Estabelecimento estabelecimento_ler(const char *linha, Quadra quadra);

/*
Escreve todas as informações presentes em um estabelecimento em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void estabelecimento_escrever_informacoes(Estabelecimento estabelecimento, FILE *arquivo);

/*
Obtém o id de um determinado estabelecimento.
O parâmetro estalecimento não pode ser nulo e deve ser um estalecimento existente.
Retorna o id do estalecimento.
*/
const char *estabelecimento_obter_id(Estabelecimento estabelecimento);

/*
Obtém o tipo de um determinado estabelecimento.
O parâmetro estalecimento não pode ser nulo e deve ser um estalecimento existente.
Retorna o tipo do estalecimento.
*/
const char *estabelecimento_obter_tipo(Estabelecimento estabelecimento);

/*
Obtém o nome de um determinado estabelecimento.
O parâmetro estalecimento não pode ser nulo e deve ser um estalecimento existente.
Retorna o nome do estalecimento.
*/
const char *estabelecimento_obter_nome(Estabelecimento estabelecimento);

/*
Obtém o cpf de um determinado estabelecimento.
O parâmetro estalecimento não pode ser nulo e deve ser um estalecimento existente.
Retorna o cpf do estalecimento.
*/
const char *estabelecimento_obter_cpf(Estabelecimento estabelecimento);

/*
Obtém a coordenada x de um estabelecimento.
O parâmetro estabelecimento não pode ser nulo.
Retorna a coordenada x de um estabelecimento.
*/
double estabelecimento_obter_x(Estabelecimento estabelecimento);

/*
Obtém a coordenada y de um estabelecimento.
O parâmetro estabelecimento não pode ser nulo.
Retorna o raio de um estabelecimento.
*/
double estabelecimento_obter_y(Estabelecimento estabelecimento);

/*
Obtém a largura de um estabelecimento.
O parâmetro estabelecimento não pode ser nulo.
Retorna a largura de um estabelecimento.
*/
double estabelecimento_obter_largura(Estabelecimento estabelecimento);

/*
Obtém a altura de um estabelecimento.
O parâmetro estabelecimento não pode ser nulo.
Retorna a altura de um estabelecimento.
*/
double estabelecimento_obter_altura(Estabelecimento estabelecimento);

/*
Obtém a cor de preenchimento de um estabelecimento.
O parâmetro estabelecimento não pode ser nulo.
Retorna a cor de preenchimento de um estabelecimento.
*/
const char *estabelecimento_obter_cor_preenchimento(Estabelecimento estabelecimento);

/*
Define a cor de preenchimento de um estabelecimento.
Nenhum dos parâmetros podem ser nulos.
*/
void estabelecimento_definir_cor_preenchimento(Estabelecimento estabelecimento,
                                               const char *cor_preenchimento);

/*
Obtém a cor da borda de um estabelecimento.
O parâmetro estabelecimento não pode ser nulo.
Retorna a cor da borda de um estabelecimento.
*/
const char *estabelecimento_obter_cor_borda(Estabelecimento estabelecimento);

/*
Define a cor da borda de um estabelecimento.
Nenhum dos parâmetros podem ser nulos.
*/
void estabelecimento_definir_cor_borda(Estabelecimento estabelecimento, const char *cor_borda);

/*
Define a espessura da borda de um estabelecimento.
Nenhum dos parâmetros podem ser nulos.
*/
void estabelecimento_definir_espessura_borda(Estabelecimento estabelecimento,
                                             double espessura_borda);

/*
Define se o estabelecimento tem borda tracejada.
O parâmetro estabelecimento não pode ser nulo.
*/
void estabelecimento_definir_borda_tracejada(Estabelecimento estabelecimento, bool tracejado);

/*
Define o arredondamento da borda de um estabelecimento.
O parâmetro estabelecimento não pode ser nulo.
*/
void estabelecimento_definir_arredondamento_borda(Estabelecimento estabelecimento,
                                                  double arredondamento_borda);

/*
Libera a memória alocada por um estabelecimento.
O parâmetro estabelecimento não pode ser nulo e deve apontar para um espaço de memória
reservada. Libera a memória alocada. O ponteiro não poderá ser utilizado após isso!
*/
void estabelecimento_destruir(Estabelecimento estabelecimento);

#endif
