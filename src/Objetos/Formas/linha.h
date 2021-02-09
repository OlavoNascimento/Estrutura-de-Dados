#ifndef LINHA_H
#define LINHA_H

/*
Este módulo define um Linha, o qual possui propriedades inerentes ao seu tipo como coordenadas
espaciais de inicio e fim, assim como propriedades para alterar sua visualização no arquivo svg,
como as cores de borda e preenchimento e sua aparência.

Suas propriedades são:
    x: Coordenada x inicial da linha no plano.
    y: Coordenada y inicial da linha no plano.
    x1: Coordenada x final da linha no plano.
    y1: Coordenada y final da linha no plano.
    cor da borda
    cor de preenchimento
    tracejado: Define se a linha deve ser continua ou tracejada no arquivo svg.
*/

#include <stdbool.h>
#include <stdio.h>

typedef struct Linha_s *Linha;

/*
Cria e inicializa uma Linha com os valores passados.
O id e as cores não podem ser nulos.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Linha linha_criar(double x1, double y1, double x2, double y2, const char cor_borda[20],
                  const char cor_preenchimento[20], bool tracejado);

/*
Escreve todas as informações presentes em uma linha em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void linha_escrever_informacoes(Linha linha, FILE *arquivo);

/*
Escreve o código svg necessário para representar uma linha em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void linha_escrever_svg(Linha linha, FILE *arquivo);

/*
Obtém a coordenada x onde uma linha começa.
O parâmetro circulo não pode ser nulo.
Retorna a coordenada x onde uma linha começa.
*/
double linha_obter_x(Linha linha);

/*
Obtém a coordenada y onde uma linha começa.
O parâmetro circulo não pode ser nulo.
Retorna a coordenada y onde uma linha começa.
*/
double linha_obter_y(Linha linha);

/*
Obtém a coordenada x onde uma linha termina.
O parâmetro circulo não pode ser nulo.
Retorna a coordenada x onde uma linha termina.
*/
double linha_obter_x_fim(Linha linha);

/*
Obtém a coordenada y onde uma linha termina.
O parâmetro circulo não pode ser nulo.
Retorna a coordenada y onde uma linha termina.
*/
double linha_obter_y_fim(Linha linha);

/*
Obtém a coordenada x onde o centro de uma linha se encontra.
O parâmetro retangulo não pode ser nulo.
Retorna a coordenada x onde o centro de uma linha se encontra.
*/
double linha_obter_x_centro(Linha linha);

/*
Obtém a coordenada y onde o centro de uma linha se encontra.
O parâmetro retangulo não pode ser nulo.
Retorna a coordenada y onde o centro de uma linha se encontra.
*/
double linha_obter_y_centro(Linha linha);

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
