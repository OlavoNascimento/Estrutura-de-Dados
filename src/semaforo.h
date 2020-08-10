#ifndef SEMAFORO_H
#define SEMAFORO_H

#include "retangulo.h"

typedef void *Semaforo;

// Cria e inicializa um struct Semaforo com os valores passados.
Semaforo semaforo_criar(char id[100], double largura, double altura, double x, double y,
                        char cor_borda[20], char cor_preenchimento[20], int tracejado_tamanho,
                        int tracejado_espaco);

// Cria e inicializa um struct Semaforo(igual ao retângulo) a partir de um texto obtido no arquivo .geo.
// O parâmetro linha não pode ser nulo.
Semaforo semaforo_ler(const char *linha);

// Define a cor de borda de um semaforo.
//O parâmetros cor_borda não pode ser nulo.
void semaforo_definir_cor_borda(Semaforo sema, const char *cor_borda);

// Define a cor de preenchimento de uma quadra.
//O parâmetros cor_preenchimento não pode ser nulo.
void quadra_definir_cor_preenchimento(Semaforo sema, const char *cor_preenchimento);

#endif
