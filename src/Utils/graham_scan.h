#ifndef GRAHAM_SCAN_H
#define GRAHAM_SCAN_H

/*
Este módulo define a função graham_scan a qual utiliza uma lista de figuras para calcular um
envoltória convexa.
*/

#include "../Estruturas/lista.h"
#include "../Estruturas/pilha.h"
#include "../Interfaces/figura.h"

/*
Calcula a envoltória convexa de uma lista de figuras.
O parâmetro figuras não pode ser nulo e deve ser uma lista que contem figuras.
Retorna uma pilha com os nós da lista que compoem a envoltória convexa.
*/
Pilha graham_scan(int tamanho, Figura *figuras);

#endif
