
#ifndef CIRCULO_H
#define CIRCULO_H

#include <stdbool.h>
#include <stdio.h>

/*
Este módulo define o tipo Circulo, assim como funções que criam, acessam suas propriedades e
realizam operações entre variáveis deste tipo.
*/

typedef void *Circulo;

/*
Cria e inicializa um struct Circulo com os valores passados.
O raio deve ser maior que 0. O id e as cores não podem ser nulos.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Circulo circulo_criar(const char id[100], double raio, double x, double y, const char cor_borda[20],
                      const char cor_preenchimento[20]);
/*
Cria e inicializa um struct Circulo a partir de um texto obtido no arquivo .geo.
O parâmetro linha não pode ser nulo.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Circulo circulo_ler(const char *linha);

/*
Escreve todas as informações presentes em um círculo em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void circulo_escrever_informacoes(FILE *arquivo, Circulo circulo);
/*
Escreve o código svg necessário para representar um círculo em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void circulo_escrever_svg(FILE *arquivo, Circulo circulo);

/*
Verifica se dois círculos se intersectam.
Nenhum dos parâmetros podem ser nulos.
Retorna verdadeiro caso os dois circulos se intersectam, caso contrário retorna falso.
*/
bool circulo_checar_interseccao(Circulo circulo1, Circulo circulo2);
/*
Verifica se um ponto é contido por um círculo.
O parâmetro circulo não pode ser nulo.
Retorna verdadeiro caso o ponto se encontre contido pelo circulo, caso contrário retorna falso.
*/
bool circulo_checar_ponto_interno(Circulo circulo, double ponto_x, double ponto_y);

/*
Obtém o id de um círculo.
O parâmetro circulo não pode ser nulo.
Retorna o id de um círculo.
*/
const char *circulo_obter_id(Circulo circulo);
/*
Define o id de um círculo.
Nenhum dos parâmetros podem ser nulos.
*/
void circulo_definir_id(Circulo circulo, const char *id);

/*
Obtém o raio de um círculo.
O parâmetro circulo não pode ser nulo.
Retorna o raio de um círculo.
*/
double circulo_obter_raio(Circulo circulo);
/*
Define o raio de um círculo.
O parâmetro circulo não pode ser nulo.
*/
void circulo_definir_raio(Circulo circulo, double raio);

/*
Obtém a coordenada x de um círculo.
O parâmetro circulo não pode ser nulo.
Retorna a coordenada x de um círculo.
*/
double circulo_obter_x(Circulo circulo);
/*
Define a coordenada x de um círculo.
O parâmetro circulo não pode ser nulo.
*/
void circulo_definir_x(Circulo circulo, double x);

/*
Obtém a coordenada y de um círculo.
O parâmetro circulo não pode ser nulo.
Retorna a coordenada y de um círculo.
*/
double circulo_obter_y(Circulo circulo);
/*
Define a coordenada y de um círculo.
O parâmetro circulo não pode ser nulo.
*/
void circulo_definir_y(Circulo circulo, double y);

/*
Obtém a cor da borda de um círculo.
O parâmetro circulo não pode ser nulo.
Retorna a cor da borda de um círculo.
*/
const char *circulo_obter_cor_borda(Circulo circulo);
/*
Define a cor da borda de um círculo.
Nenhum dos parâmetros podem ser nulos.
*/
void circulo_definir_cor_borda(Circulo circulo, const char *cor_borda);

/*
Obtém a cor de preenchimento de um círculo.
O parâmetro circulo não pode ser nulo.
Retorna a cor de preenchimento de um círculo.
*/
const char *circulo_obter_cor_preenchimento(Circulo circulo);
/*
Define a cor de preenchimento de um círculo.
Nenhum dos parâmetros podem ser nulos.
*/
void circulo_definir_cor_preenchimento(Circulo circulo, const char *cor_preenchimento);

/*
Obtém a espessura da borda de um círculo.
O parâmetro circulo não pode ser nulo.
Retorna a espessura da borda de um círculo.
*/
const char *circulo_obter_espessura_borda(Circulo circulo);
/*
Define a espessura da borda de um círculo.
Nenhum dos parâmetros podem ser nulos.
*/
void circulo_definir_espessura_borda(Circulo circulo, const char *espessura_borda);

/*
Libera a memória alocada por um círculo.
O parâmetro circulo não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada pelo struct. O ponteiro não poderá ser utilizado após isso!
*/
void circulo_destruir(Circulo circulo);

#endif

