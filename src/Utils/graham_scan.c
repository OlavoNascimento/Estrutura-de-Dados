
#include "graham_scan.h"

#include <stdbool.h>
#include <stdlib.h>

#include "../Estruturas/lista.h"
#include "../Estruturas/pilha.h"
#include "../Interfaces/figura.h"
#include "../Ordenacao/quicksort.h"
#include "../Utils/logging.h"
#include "../Utils/matematica.h"

// Verifica se uma curva é no sentido horário.
double checar_horario(Figura a, Figura b, Figura c) {
    double area = produto_vetorial(
        figura_obter_x(b) - figura_obter_x(a), figura_obter_y(b) - figura_obter_y(a),
        figura_obter_x(c) - figura_obter_x(a), figura_obter_y(c) - figura_obter_y(a));
    // Area > 0, curva no sentido anti-horário.
    // Area = 0, pontos são colineares.
    // Area < 0, curva no sentido horário.
    return area < 0;
}

// Utiliza a heurística de Akl-Toussaint para remover os pontos que definitivamente não pertencem a
// envoltória.
// Para isso encontra o polígono convexo formado pelas figuras que estão nas extremidades e remove
// todos os outros elementos que estão contidos nesse polígono.
// Explicação em detalhes pode ser encontrado em: S.G. Akl & Godfried Toussaint, "A Fast Convex-hull
// Algorithm" (1977).
int filtrar_vertices(int tamanho, Figura **figuras) {
    Figura esquerda = (*figuras)[0];
    Figura baixo = (*figuras)[0];
    Figura direita = (*figuras)[0];
    Figura topo = (*figuras)[0];

    // Encontra as quatro figuras das extremidades que formam o polígono.
    for (int i = 1; i < tamanho; i++) {
        Figura fig = (*figuras)[i];
        const double x = figura_obter_x(fig);
        const double y = figura_obter_y(fig);
        if (x < figura_obter_x(esquerda))
            esquerda = fig;
        if (y < figura_obter_y(baixo))
            baixo = fig;
        if (x > figura_obter_x(direita))
            direita = fig;
        if (y > figura_obter_y(topo))
            topo = fig;
    }

    Lista filtrados = lista_criar(NULL, NULL);
    lista_inserir_final(filtrados, baixo);
    lista_inserir_final(filtrados, topo);
    // Insere as figuras esquerda e direita caso não sejam repetidas.
    if (esquerda != baixo && esquerda != topo)
        lista_inserir_final(filtrados, esquerda);
    if (direita != baixo && direita != topo)
        lista_inserir_final(filtrados, direita);

    // Usa as quatro figuras encontrados para formar um polígono convexo.
    const Figura poligono[] = {baixo, esquerda, topo, direita};
    for (int i = 0; i < tamanho; i++) {
        Figura fig = (*figuras)[i];
        if (fig == topo || fig == esquerda || fig == baixo || fig == direita)
            continue;

        const double x = figura_obter_x(fig);
        const double y = figura_obter_y(fig);
        for (int j = 0; j < 4; j++) {
            int proximo = (j + 1) % 4;
            // > 0: figura fora do polígono.
            // = 0: figura na linha do polígono.
            // < 0: figura dentro do polígono.
            double pv =
                produto_vetorial(figura_obter_x(poligono[proximo]) - figura_obter_x(poligono[j]),
                                 figura_obter_y(poligono[proximo]) - figura_obter_y(poligono[j]),
                                 x - figura_obter_x(poligono[j]), y - figura_obter_y(poligono[j]));
            // Insere apenas as figuras fora do polígono encontrado.
            if (pv > 0) {
                lista_inserir_final(filtrados, fig);
                break;
            }
        }
    }

    const int novo_tamanho = lista_obter_tamanho(filtrados);
    // Diminui o tamanho do array.
    Figura *temp = realloc(*figuras, novo_tamanho * sizeof(Figura));
    if (temp == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        lista_destruir(filtrados);
        return tamanho;
    }
    *figuras = temp;
    int j = 0;
    for_each_lista(no, filtrados) {
        (*figuras)[j++] = lista_obter_info(no);
    }
    lista_destruir(filtrados);

    return novo_tamanho;
}

// Devolve uma pilha com os pontos de uma envoltória convexa.
// A pilha deve ser liberada pelo o usuário!
Pilha graham_scan(int tamanho, Figura **figuras) {
    if (tamanho < 3) {
        LOG_INFO("Não é possível criar uma envoltória convexa com menos que 3 figuras!\n");
        return NULL;
    }
    if (tamanho > 1000)
        tamanho = filtrar_vertices(tamanho, figuras);

    int figura_min = 0;
    double min_y = figura_obter_y((*figuras)[0]);
    double min_x = figura_obter_x((*figuras)[0]);
    // Encontra a figura com menor y.
    for (int i = 1; i < tamanho; i++) {
        double y = figura_obter_y((*figuras)[i]);
        double x = figura_obter_x((*figuras)[i]);
        if (y < min_y || (y == min_y && x < min_x)) {
            figura_min = i;
            min_y = y;
            min_x = x;
        }
    }

    // Move a figura com menor y para a primeira posição do array.
    Figura temp = (*figuras)[0];
    (*figuras)[0] = (*figuras)[figura_min];
    (*figuras)[figura_min] = temp;

    // Ordena o array de figuras de acordo como o ângulo formado com o ponto mínimo.
    quicksort((*figuras)[0], (*figuras), 1, tamanho - 1);

    Pilha pontos_envoltoria = pilha_criar(NULL);
    // Primeiro elemento está sempre dentro da envoltória.
    pilha_inserir(pontos_envoltoria, (*figuras)[0]);
    // Segundo elemento precisa ser verificado.
    pilha_inserir(pontos_envoltoria, (*figuras)[1]);

    for (int i = 2; i < tamanho; i++) {
        Figura proximo_caso = (*figuras)[i];
        Figura ultimo_caso = pilha_remover(pontos_envoltoria);

        // Remove figuras da pilha caso ocorra uma curva em sentido anti-horário.
        while (!pilha_esta_vazia(pontos_envoltoria) &&
               !checar_horario(pilha_obter_topo(pontos_envoltoria), ultimo_caso, proximo_caso)) {
            ultimo_caso = pilha_remover(pontos_envoltoria);
        }

        pilha_inserir(pontos_envoltoria, ultimo_caso);
        pilha_inserir(pontos_envoltoria, proximo_caso);
    }

    Figura ultimo_no = pilha_remover(pontos_envoltoria);
    // Verifica se o último ponto é válido.
    if (checar_horario(pilha_obter_topo(pontos_envoltoria), ultimo_no, (*figuras)[0]))
        pilha_inserir(pontos_envoltoria, ultimo_no);

    return pontos_envoltoria;
}
