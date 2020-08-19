#ifndef SVG_H
#define SVG_H

#include "lista.h"

// Este módulo define funções para transformar uma lista em um arquivo svg, mantendo todas as
// propriedades de seus elementos.

// Itera por uma lista e transforma cada elemento no seu código svg equivalente.
// Ambos os argumentos não podem ser nulos e todos os diretórios especificados no caminho do arquivo
// de saída devem existir.
void svg_lista_para_svg(const char *caminho_svg, Lista lista, Lista lista_quadras,
                        Lista lista_hidrantes, Lista lista_radios, Lista lista_semaforos);

#endif
