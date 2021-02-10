#include "estrutura_mapeavel.h"

#include <stdlib.h>

// Estrutura genérica que implementa uma função map.
typedef struct {
    // Deve ser o primeiro elemento de um struct que implemente a interface.
    FuncEstruturaMap *map;
} EstruturaImp;

// Utiliza a função registrada para aplicar uma função em todos os elementos armazenados na
// estrutura.
void estrutura_map(EstruturaMap estrutura, EstruturaAplicarInfo f, void *extra) {
    EstruturaImp *est = estrutura;
    est->map(est, f, extra);
}
