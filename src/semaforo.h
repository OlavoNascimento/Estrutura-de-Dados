#ifndef SEMAFORO_H
#define SEMAFORO_H

#include <stdbool.h>
#include <stdio.h>

#include "retangulo.h"

typedef void *Semaforo;

/*
Cria e inicializa um struct Semaforo a partir dos parâmetros passados pela função semaforo_ler.
A largura e altura devem ser maiores que 0. O id e as cores não podem ser nulos.
Retorna o ponteiro para a struct criada. O usuário é responsável por liberar a memória alocada!
*/
Semaforo semaforo_criar(const char id[100], double largura, double altura, double x, double y,
                        const char cor_borda[20], const char cor_preenchimento[20]);
/*
Cria e inicializa um struct Semaforo a partir de um texto obtido no arquivo .geo.
O parâmetro linha não pode ser nulo.
Retorna o ponteiro para a struct criada. O usuário é responsável por liberar a memória alocada!
*/
Semaforo semaforo_ler(const char *linha);

/*
Escreve todas as informações presentes de um semaforo em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void semaforo_escrever_informacoes(FILE *arquivo, Semaforo sema);

/*
Escreve o código svg necessário para representar um semaforo em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void semaforo_escrever_svg(FILE *arquivo, Semaforo sema);

/*
Obtém o id de um determinado semáforo.
O parâmetro "sem" não pode ser nulo e deve ser um semáforo existente.
Retorna o id do semáforo.
*/
const char *semaforo_obter_id(Semaforo sem);

/*
Obtém a coordenada y de um determinado semáforo.
O parâmetro "sem" não pode ser nulo e deve ser um semáforo existente.
Retorna a coordenada y do semáforo.
*/
double semaforo_obter_y(Semaforo sem);

/*
Obtém a coordenada x de um determinado semáforo.
O parâmetro "sem" não pode ser nulo e deve ser um semáforo existente.
Retorna a coordenada x do semáforo.
*/
double semaforo_obter_x(Semaforo sem);

/*
Obtém a largura de um determinado semáforo.
O parâmetro "sem" não pode ser nulo e deve ser um semáforo existente.
Retorna a largura do semáforo.
*/
double semaforo_obter_largura(Semaforo sem);

/*
Obtém a altura de um determinado semáforo.
O parâmetro "sem" não pode ser nulo e deve ser um semáforo existente.
Retorna a altura do semáforo.
*/
double semaforo_obter_altura(Semaforo sem);

/*
Obtém cor da borda de um determinado semáforo.
O parâmetro "sem" não pode ser nulo e deve ser um semáforo existente.
Retorna a cor da borda do semáforo.
*/
const char *semaforo_obter_cor_borda(Semaforo sem);

/*
Define a cor da borda de um determinado semáforo.
O parâmetro "sem" não pode ser nulo, deve ser um semáforo existente e cor_borda não pode ser nulo.
*/
void semaforo_definir_cor_borda(Semaforo sema, const char *cor_borda);

/*
Obtém a cor de preenchimento de um determinado semáforo.
O parâmetro "sem" não pode ser nulo e deve ser um semáforo existente.
Retorna a cor de preenchimento do semáforo.
*/
const char *semaforo_obter_cor_preenchimento(Semaforo sem);

/*
Define a cor de preenchimento de um determinado semáforo.
O parâmetro "sem" não pode ser nulo, deve ser um semáforo existente e cor_preenchimento não pode
ser nulo.
*/
void semaforo_definir_cor_preenchimento(Semaforo sema, const char *cor_preenchimento);

/*
Obtém a espessura de um determinado semáforo.
O parâmetro "sem" não pode ser nulo e deve ser um semáforo existente.
Retorna a espessura do semáforo.
*/
const char *semaforo_obter_espessura_borda(Semaforo sem);

/*
Define a espessura de um determinado semáforo.
O parâmetro "sem" não pode ser nulo, deve ser um semáforo existente e espessura_borda não pode ser
nulo.
*/
void semaforo_definir_espessura_borda(Semaforo sem, const char *espessura_borda);

/*
Libera a memória alocada para um semáforo.
O parâmetro "sem" não pode ser nulo, deve ser um semáforo existente.
*/
void semaforo_destruir(Semaforo sema);

#endif
