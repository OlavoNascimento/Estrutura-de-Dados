#ifndef TEXTO_H
#define TEXTO_H

#include <stdio.h>

/*
Este módulo define o tipo Texto, assim como funções que criam e acessam suas propriedades.
*/

typedef void *Texto;

/*
Cria e inicializa uma struct Texto com os valores passados.
A largura e altura devem ser maiores que 0. O id e as cores não podem ser nulos.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Texto texto_criar(const char id[100], double x, double y, const char cor_borda[20],
                  const char cor_preenchimento[20], const char conteudo[500]);

/*
Cria e inicializa uma struct Texto a partir de um texto obtido no arquivo .geo.
O parâmetro linha não pode ser nulo.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Texto texto_ler(const char *linha);

/*
Escreve todas as informações presentes em um texto em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void texto_escrever_informacoes(FILE *arquivo, Texto tex);

/*
Escreve todas as informações presentes em um texto em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void texto_escrever_svg(FILE *arquivo, Texto tex);

/*
Obtém o id de um texto.
O parâmetro retangulo não pode ser nulo.
Retorna o id de um texto.
*/
const char *texto_obter_id(Texto tex);
/*
Define o id de um texto.
Nenhum dos parâmetros podem ser nulos.
*/
void texto_definir_id(Texto tex, const char *id);

/*
Obtém a coordenada x de um texto.
O parâmetro retangulo não pode ser nulo.
Retorna a coordenada x de um texto.
*/
double texto_obter_x(Texto tex);
/*
Define a coordenada x de um texto.
O parâmetro retangulo não pode ser nulo.
*/
void texto_definir_x(Texto tex, double x);

/*
Obtém a coordenada y de um texto.
O parâmetro retangulo não pode ser nulo.
Retorna a coordenada y de um texto.
*/
double texto_obter_y(Texto tex);
/*
Define a coordenada y de um texto.
O parâmetro retangulo não pode ser nulo.
*/
void texto_definir_y(Texto tex, double y);

/*
Obtém a cor da borda de um texto.
O parâmetro retangulo não pode ser nulo.
Retorna a cor da borda de um texto.
*/
const char *texto_obter_cor_borda(Texto tex);
/*
Define a cor da borda de um texto.
Nenhum dos parâmetros podem ser nulos.
*/
void texto_definir_cor_borda(Texto tex, const char *cor_borda);

/*
Obtém a cor de preenchimento de um texto.
O parâmetro retangulo não pode ser nulo.
Retorna a cor de preenchimento de um texto.
*/
const char *texto_obter_cor_preenchimento(Texto tex);
/*
Define a cor de preenchimento de um texto.
Nenhum dos parâmetros podem ser nulos.
*/
void texto_definir_cor_preenchimento(Texto tex, const char *cor_preenchimento);

/*
Obtém o conteúdo de um texto.
O parâmetro retangulo não pode ser nulo.
Retorna o conteúdo de um texto.
*/
const char *texto_obter_conteudo(Texto tex);
/*
Define o conteúdo de um texto.
Nenhum dos parâmetros podem ser nulos.
*/
void texto_definir_conteudo(Texto tex, const char *conteudo);

/*
Libera a memória alocada por um texto.
O parâmetro retangulo não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada pelo struct. O ponteiro não poderá ser utilizado após isso!
*/
void texto_destruir(Texto tex);

#endif
