#ifndef CASO_H
#define CASO_H

#include <stdbool.h>
#include <stdio.h>

/*
Este módulo define o tipo Caso, assim como funções que criam, acessam suas propriedades e
realizam operações entre variáveis deste tipo.
*/

typedef void* Caso;

/*
Cria e inicializa um struct Caso com os valores passados.
As cores não podem ser nulos.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Caso caso_criar(double largura, double altura, double x, double y, const char cor_borda[20],
                const char cor_preenchimento[20]);

/*
Cria e inicializa um struct Caso a partir de um texto obtido no arquivo .geo.
O parâmetro linha não pode ser nulo.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Caso caso_ler(const char* linha, Lista lista_quadras);

/*
Escreve todas as informações presentes em um caso em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void caso_escrever_informacoes(FILE* arquivo, Caso caso);

/*
Escreve o código svg necessário para representar um caso em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void caso_escrever_svg(FILE* arquivo, Caso caso);

double caso_obter_x(Caso caso);
double caso_obter_y(Caso caso);
double caso_obter_largura(Caso caso);
double caso_obter_altura(Caso caso);

/*
Escreve o código svg necessário para representar um círculo em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
int caso_obter_casos(Caso casos);

#endif
