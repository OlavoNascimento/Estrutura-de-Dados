#ifndef LINHA_H
#define LINHA_H

#include <stdio.h>

/*
Este módulo define o tipo Linha, assim como funções que criam, acessam suas propriedades e
realizam operações entre variáveis deste tipo.
*/

typedef void *Linha;

/*
Cria e inicializa um struct Linha com os valores passados.
O id e as cores não podem ser nulos.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Linha linha_criar(double x1, double y1, double x2, double y2, const char cor_borda[20],
                  const char cor_preenchimento[20]);

/*
Escreve o código svg necessário para representar uma linha em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void linha_escrever_svg(FILE *arquivo, Linha linha);

/*
Obtém a coordenada x1 de uma linha.
O parâmetro linha não pode ser nulo.
Retorna a coordenada x1 de uma linha.
*/
double linha_obter_x1(Linha linha);
/*
Define a coordenada x1 de uma linha.
O parâmetro linha não pode ser nulo.
*/
void linha_definir_x1(Linha linha, double x1);

/*
Obtém a coordenada y1 de uma linha.
O parâmetro linha não pode ser nulo.
Retorna a coordenada y1 de uma linha.
*/
double linha_obter_y1(Linha linha);
/*
Define a coordenada y1 de uma linha.
O parâmetro linha não pode ser nulo.
*/
void linha_definir_y1(Linha linha, double y1);

/*
Obtém a coordenada x2 de uma linha.
O parâmetro linha não pode ser nulo.
Retorna a coordenada x2 de uma linha.
*/
double linha_obter_x2(Linha linha);
/*
Define a coordenada x2 de uma linha.
O parâmetro linha não pode ser nulo.
*/
void linha_definir_x2(Linha linha, double x2);

/*
Obtém a coordenada y2 de uma linha.
O parâmetro linha não pode ser nulo.
Retorna a coordenada y2 de uma linha.
*/
double linha_obter_y2(Linha linha);
/*
Define a coordenada y2 de uma linha.
O parâmetro linha não pode ser nulo.
*/
void linha_definir_y2(Linha linha, double y2);

/*
Obtém a cor da borda de uma linha.
O parâmetro linha não pode ser nulo.
Retorna a cor da borda de uma linha.
*/
const char *linha_obter_cor_borda(Linha linha);
/*
Define a cor da borda de uma linha.
Nenhum dos parâmetros podem ser nulos.
*/
void linha_definir_cor_borda(Linha linha, const char *cor_borda);

/*
Obtém a cor de preenchimento de uma linha.
O parâmetro linha não pode ser nulo.
Retorna a cor de preenchimento de uma linha.
*/
const char *linha_obter_cor_preenchimento(Linha linha);
/*
Define a cor de preenchimento de uma linha.
Nenhum dos parâmetros podem ser nulos.
*/
void linha_definir_cor_preenchimento(Linha linha, const char *cor_preenchimento);

/*
Libera a memória alocada por uma linha.
O parâmetro linha não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada pelo struct. O ponteiro não poderá ser utilizado após isso!
*/
void linha_destruir(Linha linha);

#endif
