#ifndef RETANGULO_H
#define RETANGULO_H

#include <stdbool.h>
#include <stdio.h>

/*
Este módulo define o tipo Retângulo, assim como funções que criam, acessam suas propriedades e
realizam operações entre variáveis deste tipo.
*/

typedef void *Retangulo;

/*
Cria e inicializa uma struct Retangulo com os valores passados.
A largura e altura devem ser maiores que 0. O id e as cores não podem ser nulos.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Retangulo retangulo_criar(const char id[100], double largura, double altura, double x, double y,
                          const char cor_borda[20], const char cor_preenchimento[20]);

/*
Cria e inicializa uma struct Retangulo a partir de um texto obtido no arquivo .geo.
O parâmetro linha não pode ser nulo.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Retangulo retangulo_ler(const char *linha);

/*
Escreve todas as informações presentes em um retângulo em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void retangulo_escrever_informacoes(FILE *arquivo, Retangulo retangulo);

/*
Escreve o código svg necessário para representar um retângulo em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void retangulo_escrever_svg(FILE *arquivo, Retangulo retangulo);

/*
Verifica se dois círculos se intersectam.
Nenhum dos parâmetros podem ser nulos.
Retorna verdadeiro caso os dois retângulos se intersectam, caso contrário retorna falso.
*/
bool retangulo_checar_interseccao(Retangulo ret1, Retangulo ret2);
/*
Verifica se um ponto é contido por um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna verdadeiro caso o ponto se encontre contido pelo retangulo, caso contrário retorna falso.
*/
bool retangulo_checar_ponto_interno(Retangulo retangulo, double ponto_x, double ponto_y);

/*
Obtém o id de um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna o id de um retângulo.
*/
const char *retangulo_obter_id(Retangulo retangulo);
/*
Define o id de um retângulo.
Nenhum dos parâmetros podem ser nulos.
*/
void retangulo_definir_id(Retangulo retangulo, const char *id);

/*
Obtém a largura de um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna a largura de um retângulo.
*/
double retangulo_obter_largura(Retangulo retangulo);
/*
Define a largura de um retângulo.
Nenhum dos parâmetros podem ser nulos.
*/
void retangulo_definir_largura(Retangulo retangulo, double largura);

/*
Obtém a altura de um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna a altura de um retângulo.
*/
double retangulo_obter_altura(Retangulo retangulo);
/*
Define a altura de um retângulo.
Nenhum dos parâmetros podem ser nulos.
*/
void retangulo_definir_altura(Retangulo retangulo, double altura);

/*
Obtém a coordenada x de um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna a coordenada x de um retângulo.
*/
double retangulo_obter_x(Retangulo retangulo);
/*
Define a coordenada x de um retângulo.
O parâmetro retangulo não pode ser nulo.
*/
void retangulo_definir_x(Retangulo retangulo, double x);

/*
Obtém a coordenada y de um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna a coordenada y de um retângulo.
*/
double retangulo_obter_y(Retangulo retangulo);
/*
Define a coordenada y de um retângulo.
O parâmetro retangulo não pode ser nulo.
*/
void retangulo_definir_y(Retangulo retangulo, double y);

/*
Obtém a cor da borda de um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna a cor da borda de um retângulo.
*/
const char *retangulo_obter_cor_borda(Retangulo retangulo);
/*
Define a cor da borda de um retângulo.
Nenhum dos parâmetros podem ser nulos.
*/
void retangulo_definir_cor_borda(Retangulo retangulo, const char *cor_borda);

/*
Obtém a cor de preenchimento de um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna a cor de preenchimento de um retângulo.
*/
const char *retangulo_obter_cor_preenchimento(Retangulo retangulo);
/*
Define a cor de preenchimento de um retângulo.
Nenhum dos parâmetros podem ser nulos.
*/
void retangulo_definir_cor_preenchimento(Retangulo retangulo, const char *cor_preenchimento);

/*
Obtém a espessura da borda de um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna a espessura da borda de um retângulo.
*/
const char *retangulo_obter_espessura_borda(Retangulo retangulo);
/*
Define a espessura da borda de um retângulo.
Nenhum dos parâmetros podem ser nulos.
*/
void retangulo_definir_espessura_borda(Retangulo retangulo, const char *espessura_borda);

/*
Obtém o espassamento da borda de um retângulo.
O parâmetro retangulo não pode ser nulo.
Retorna o espassamento da borda de um retângulo.
*/
int retangulo_obter_espassamento_borda(Retangulo retangulo);
/*
Define o espassamento da borda de um retângulo.
Nenhum dos parâmetros podem ser nulos.
*/
void retangulo_definir_espassamento_borda(Retangulo retangulo, int espassamento_borda);

/*
Define o arredondamento da borda de um retângulo.
Nenhum dos parâmetros podem ser nulos.
*/
void retangulo_definir_arredondamento_borda(Retangulo retangulo, double raio_borda);

/*
Libera a memória alocada por um retângulo.
O parâmetro retangulo não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada pelo struct. O ponteiro não poderá ser utilizado após isso!
*/
void retangulo_destruir(Retangulo retangulo);

#endif
