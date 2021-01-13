
#include "graham_scan.h"

#include <float.h>

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
    // Anti-horário
    if (area > 0)
        return -1;
    // Horário
    if (area < 0)
        return 1;
    // Pontos são colineares
    return 0;
}

// Devolve uma pilha com os pontos de uma envoltória convexa.
// A pilha deve ser liberada pelo o usuário!
Pilha graham_scan(Lista figuras) {
    // Não é possível formar uma envoltória convexa com menos de 3 pontos.
    if (lista_obter_tamanho(figuras) < 3) {
        LOG_INFO("Não é possível criar uma envoltória convexa com menos que 3 figuras!\n");
        return NULL;
    }

    ListaNo no_min = NULL;
    double min_y = DBL_MAX;
    // Encontra o caso com menor y.
    for (ListaNo i = lista_obter_primeiro(figuras); i != NULL; i = lista_obter_proximo(i)) {
        if (figura_obter_y(lista_obter_info(i)) < min_y) {
            no_min = i;
            min_y = figura_obter_y(lista_obter_info(i));
        }
    }
    if (no_min == NULL) {
        LOG_ERRO("Nenhum elemento mínimo encontrado em graham scan!\n");
        return NULL;
    }

    // Move a figura com maior y para a primeira posição da lista.
    lista_trocar_info(no_min, lista_obter_primeiro(figuras));

    // Ordena a lista de casos de acordo como o ângulo formado com o ponto mínimo.
    quicksort(lista_obter_info(lista_obter_primeiro(figuras)),
              lista_obter_proximo(lista_obter_primeiro(figuras)), lista_obter_ultimo(figuras));

    Pilha pontos_envoltoria = pilha_criar(NULL);
    // Primeiro elemento está sempre dentro da envoltória.
    pilha_inserir(pontos_envoltoria, lista_obter_primeiro(figuras));
    // Segundo elemento precisa ser verificado.
    ListaNo segundo_elemento = lista_obter_proximo(lista_obter_primeiro(figuras));
    pilha_inserir(pontos_envoltoria, segundo_elemento);

    for (ListaNo i = lista_obter_proximo(segundo_elemento); i != NULL; i = lista_obter_proximo(i)) {
        Figura proximo_caso = lista_obter_info(i);
        ListaNo ultimo_caso = pilha_remover(pontos_envoltoria);

        // Remove figuras da pilha caso ocorra uma curva em sentido anti-horário.
        while (!pilha_esta_vazia(pontos_envoltoria) &&
               checar_horario(lista_obter_info(pilha_obter_topo(pontos_envoltoria)),
                              lista_obter_info(ultimo_caso), proximo_caso) < 0) {
            ultimo_caso = pilha_remover(pontos_envoltoria);
        }

        pilha_inserir(pontos_envoltoria, ultimo_caso);
        pilha_inserir(pontos_envoltoria, i);
    }

    ListaNo ultimo_no = pilha_remover(pontos_envoltoria);
    ListaNo topo = pilha_obter_topo(pontos_envoltoria);
    // Verifica se o último ponto é inválido.
    if (checar_horario(lista_obter_info(topo), lista_obter_info(ultimo_no),
                       lista_obter_info(lista_obter_primeiro(figuras))) >= 0) {
        pilha_inserir(pontos_envoltoria, ultimo_no);
    }

    return pontos_envoltoria;
}
