#ifndef TEXTO_H
#define TEXTO_H

/*
Este módulo define um Texto, permitindo exibir textos no arquivo svg. Possui como propriedades seu
identificador, coordenadas espaciais e para alterar sua visualização no arquivo svg cores de borda e
preenchimento.

Suas propriedades são:
    id: Identificador do texto.
    x: Coordenada x do texto no plano.
    y: Coordenada y do texto no plano.
    cor da borda
    cor de preenchimento
    conteudo
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
typedef struct Texto_s *Texto;

enum TextoAlinhamento { TEXTO_ESQUERDA, TEXTO_CENTRO };

/*
Cria e inicializa um Texto com os valores passados.
A largura e altura devem ser maiores que 0. O id e as cores não podem ser nulos.
O usuário é responsável por liberar a memória alocada!
*/
Texto texto_criar(const char id[100], double x, double y, const char cor_borda[20],
                  const char cor_preenchimento[20], const char conteudo[1024]);

/*
Cria e inicializa um Texto a partir de um texto obtido no arquivo .geo.
O parâmetro linha não pode ser nulo.
O usuário é responsável por liberar a memória alocada!
*/
Texto texto_ler(const char *linha);

/*
Escreve todas as informações presentes em um texto em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void texto_escrever_informacoes(Texto texto, FILE *arquivo);

/*
Escreve todas as informações presentes em um texto em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void texto_escrever_svg(Texto texto, FILE *arquivo);

/*
Obtém o conteúdo de um texto.
O parâmetro retangulo não pode ser nulo.
Retorna o conteúdo de um texto.
*/
const char *texto_obter_conteudo(Texto texto);

/*
Obtém o id de um texto.
O parâmetro retangulo não pode ser nulo.
Retorna o id de um texto.
*/
const char *texto_obter_id(Texto texto);

/*
Obtém a coordenada x de um texto.
O parâmetro retangulo não pode ser nulo.
Retorna a coordenada x de um texto.
*/
double texto_obter_x(Texto texto);

/*
Obtém a coordenada y de um texto.
O parâmetro retangulo não pode ser nulo.
Retorna a coordenada y de um texto.
*/
double texto_obter_y(Texto texto);

/*
Obtém a coordenada x onde um texto termina.
O parâmetro retangulo não pode ser nulo.
Retorna a coordenada x onde um texto termina.
*/
double texto_obter_x_fim(Texto texto);

/*
Obtém a coordenada y onde um texto termina.
O parâmetro retangulo não pode ser nulo.
Retorna a coordenada y onde um texto termina.
*/
double texto_obter_y_fim(Texto texto);

/*
Obtém a coordenada x onde o centro de um texto se encontra.
O parâmetro retangulo não pode ser nulo.
Retorna a coordenada x onde o centro de um texto se encontra.
*/
double texto_obter_x_centro(Texto texto);

/*
Obtém a coordenada y onde o centro de um texto se encontra.
O parâmetro retangulo não pode ser nulo.
Retorna a coordenada y onde o centro de um texto se encontra.
*/
double texto_obter_y_centro(Texto texto);

/*
Obtém a cor da borda de um texto.
O parâmetro retangulo não pode ser nulo.
Retorna a cor da borda de um texto.
*/
const char *texto_obter_cor_borda(Texto texto);

/*
Define a cor da borda de um texto.
Nenhum dos parâmetros podem ser nulos.
*/
void texto_definir_cor_borda(Texto texto, const char *cor_borda);

/*
Obtém a cor de preenchimento de um texto.
O parâmetro retangulo não pode ser nulo.
Retorna a cor de preenchimento de um texto.
*/
const char *texto_obter_cor_preenchimento(Texto texto);

/*
Define a cor de preenchimento de um texto.
Nenhum dos parâmetros podem ser nulos.
*/
void texto_definir_cor_preenchimento(Texto texto, const char *cor_preenchimento);

/*
Define o alinhamento de um texto, definindo qual posição do texto as coordenas x e y representam.
O parâmetro texto não pode ser nulo.
*/
void texto_definir_alinhamento(Texto texto, enum TextoAlinhamento alinhamento);

/*
Libera a memória alocada por um texto.
O parâmetro retangulo não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada. O ponteiro não poderá ser utilizado após isso!
*/
void texto_destruir(Texto texto);

#endif
