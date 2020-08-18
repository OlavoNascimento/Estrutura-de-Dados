#ifndef SEMAFORO_H
#define SEMAFORO_H

#include <stdbool.h>
#include <stdio.h>

#include "retangulo.h"

typedef void *Semaforo;

// Cria e inicializa um struct Semaforo com os valores passados.
Semaforo semaforo_criar(char id[100], double largura, double altura, double x, double y,
                        char cor_borda[20], char cor_preenchimento[20]);
// Cria e inicializa um struct Semaforo(igual ao retângulo) a partir de um texto obtido no arquivo
// .geo. O parâmetro linha não pode ser nulo.
Semaforo semaforo_ler(const char *linha);

void semaforo_escrever_informacoes(FILE *arquivo, Semaforo sema);

void semaforo_escrever_svg(FILE *arquivo, Semaforo sema);

// Retorna o id de um semaforo.
const char *semaforo_obter_id(Semaforo sem);

double semaforo_obter_y(Semaforo sem);

double semaforo_obter_x(Semaforo sem);

const char *semaforo_obter_cor_borda(Semaforo sem);
// Define a cor de borda de um semaforo.
// O parâmetros cor_borda não pode ser nulo.
void semaforo_definir_cor_borda(Semaforo sema, const char *cor_borda);

const char *semaforo_obter_cor_preenchimento(Semaforo sem);
// Define a cor de preenchimento de um semaforo.
// O parâmetros cor_preenchimento não pode ser nulo.
void semaforo_definir_cor_preenchimento(Semaforo sema, const char *cor_preenchimento);

int semaforo_obter_espessura_borda(Semaforo sem);
void semaforo_definir_espessura_borda(Semaforo sem, int espessura_borda);

// Libera a memória alocada por um semáforo.
void semaforo_destruir(Semaforo sema);

#endif
