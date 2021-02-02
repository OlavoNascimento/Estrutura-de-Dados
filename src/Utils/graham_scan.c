
#include "graham_scan.h"

#include <float.h>
#include <stdbool.h>

#include "../Estruturas/lista.h"
#include "../Estruturas/pilha.h"
#include "../Interfaces/figura.h"
#include "../Ordenacao/quicksort.h"
#include "../Utils/logging.h"

// Verifica se uma curva é no sentido horário.
double checar_horario(Figura a, Figura b, Figura c) {
    double area =
        (figura_obter_x(b) - figura_obter_x(a)) * (figura_obter_y(c) - figura_obter_y(a)) -
        (figura_obter_y(b) - figura_obter_y(a)) * (figura_obter_x(c) - figura_obter_x(a));
    // Area > 0, curva no sentido anti-horário.
    // Area = 0, pontos são colineares.
    // Area < 0, curva no sentido horário.
    return area < 0;
}

// Devolve uma pilha com os pontos de uma envoltória convexa.
// A pilha deve ser liberada pelo o usuário!
Pilha graham_scan(int tamanho, Figura *figuras) {
    // Não é possível formar uma envoltória convexa com menos de 3 pontos.
    if (tamanho < 3) {
        LOG_INFO("Não é possível criar uma envoltória convexa com menos que 3 figuras!\n");
        return NULL;
    }

    int figura_min = -1;
    double min_y = DBL_MAX;
    // Encontra o caso com menor y.
    for (int i = 0; i < tamanho; i++) {
        if (figura_obter_y(figuras[i]) < min_y) {
            figura_min = i;
            min_y = figura_obter_y(figuras[i]);
        }
    }
    if (figura_min == -1) {
        LOG_ERRO("Nenhum elemento mínimo encontrado em graham scan!\n");
        return NULL;
    }

    // Move a figura com menor y para a primeira posição da lista.
    Figura temp = figuras[0];
    figuras[0] = figuras[figura_min];
    figuras[figura_min] = temp;

    // Ordena a lista de casos de acordo como o ângulo formado com o ponto mínimo.
    quicksort(figuras[0], figuras, 0, tamanho - 1);

    Pilha pontos_envoltoria = pilha_criar(NULL);
    // Primeiro elemento está sempre dentro da envoltória.
    pilha_inserir(pontos_envoltoria, figuras[0]);
    // Segundo elemento precisa ser verificado.
    pilha_inserir(pontos_envoltoria, figuras[1]);

    for (int i = 2; i < tamanho; i++) {
        Figura proximo_caso = figuras[i];
        Figura ultimo_caso = pilha_remover(pontos_envoltoria);

        // Remove figuras da pilha caso ocorra uma curva em sentido anti-horário.
        while (!pilha_esta_vazia(pontos_envoltoria) &&
               !checar_horario(pilha_obter_topo(pontos_envoltoria), ultimo_caso, proximo_caso)) {
            ultimo_caso = pilha_remover(pontos_envoltoria);
        }

        pilha_inserir(pontos_envoltoria, ultimo_caso);
        pilha_inserir(pontos_envoltoria, figuras[i]);
    }

    Figura ultimo_no = pilha_remover(pontos_envoltoria);
    Figura topo = pilha_obter_topo(pontos_envoltoria);
    // Verifica se o último ponto é inválido.
    if (checar_horario(topo, ultimo_no, figuras[0]))
        pilha_inserir(pontos_envoltoria, ultimo_no);

    return pontos_envoltoria;
}
