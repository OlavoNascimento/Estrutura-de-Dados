#ifndef LER_GEO_H
#define LER_GEO_H

#include "lista.h"

// Abre um arquivo .geo no caminho passado a função e interpreta seus comando afim de criar uma
// lista com os elementos especificados no arquivo.
// O paramêtro caminho_arq_descricao não pode ser nulo.
void ler_geo(const char *caminho_geo, Lista lista, Lista lista_quadras, Lista lista_hidrantes,
             Lista lista_radios, Lista lista_semaforos);

#endif
