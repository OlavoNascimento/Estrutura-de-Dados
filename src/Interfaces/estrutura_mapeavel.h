#ifndef ESTRUTURA_MAPEAVEL_H
#define ESTRUTURA_MAPEAVEL_H

/*
Define uma função map, a qual pode ser incluída em estruturas para permitir que uma função seja
aplicada a todos as informações armazenadas.
É utilizada para permitir que funções sejam executadas em diferentes estruturas sem saber o seu tipo
específico, consequentemente agindo como uma forma de poliformismo.
*/

#include <stdio.h>

/*
Estrutura genérica que possui possui uma função map como primeiro elemento.
*/
typedef void *EstruturaMap;

/*
Função aplicada em todas as informações da estrutura.
*/
typedef void EstruturaAplicarInfo(void *info, void *extra);

/*
Itera por todos os elementos de uma estrutura e aplica uma função f.
*/
typedef void FuncEstruturaMap(void *estrutura, EstruturaAplicarInfo f, void *extra);

/*
Utiliza a função registrada para aplicar uma função f em todos os elementos da estrutura.
Os parâmetros estrutura e f não podem ser nulos.
*/
void estrutura_map(EstruturaMap estrutura, EstruturaAplicarInfo f, void *extra);

#endif
