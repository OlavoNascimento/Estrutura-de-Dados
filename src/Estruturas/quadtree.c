
#include "quadtree.h"

#include <stdlib.h>
#include <string.h>

#include "../Estruturas/fila.h"
#include "../Estruturas/lista.h"
#include "../Interfaces/figura.h"
#include "../Objetos/Formas/ponto.h"
#include "../Utils/logging.h"

typedef struct {
    Ponto coordenada;
    QtInfo info;
} QTNoImp;

typedef struct QuadTreeImpT {
    QTNoImp *no;

    funcGetChave *obter_identificador;

    struct QuadTreeImpT *noroeste;
    struct QuadTreeImpT *nordeste;
    struct QuadTreeImpT *sudoeste;
    struct QuadTreeImpT *sudeste;
} QuadTreeImp;

QuadTree criaQt(funcGetChave f) {
    QuadTreeImp *quadtree = malloc(sizeof(QuadTreeImp));
    quadtree->obter_identificador = f;
    quadtree->noroeste = NULL;
    quadtree->nordeste = NULL;
    quadtree->sudoeste = NULL;
    quadtree->sudeste = NULL;

    quadtree->no = NULL;
    return quadtree;
}

const char *obter_identificador_string(ListaInfo info) {
    return info;
}

void *quadtree_extrair_id_info(QuadTreeImp *qt) {
    return (void *) qt->obter_identificador(qt->no->info);
}

void *quadtree_extrair_ponto(QuadTreeImp *qt) {
    return qt->no->coordenada;
}

void *quadtree_extrair_no(QuadTreeImp *qt) {
    return qt->no;
}

// Percorre uma quadtree em busca de valores que estejam contidos dentro de um retangulo. Quando
// um valor atende esse requisito a função passada como argumento é utilizada para extrair a
// informação necessária e adiciona-la na lista.
void valores_dentro_retangulo(QuadTreeImp *quadtree, double x1, double y1, double x2, double y2,
                              Lista *valores, void *extrair_valor(QuadTreeImp *qt)) {
    // Caso o ponto esteja contido no retângulo usa a função passada como argumento para extrair a
    // informação que será armazenada na lista.
    if (quadtree->no != NULL &&
        ponto_contido_em_retangulo(quadtree->no->coordenada, x1, y1, x2, y2))
        lista_inserir_final(valores, extrair_valor(quadtree));

    // O nó não tem filhos.
    if (quadtree->nordeste == NULL)
        return;

    double cord_x = ponto_obter_x(quadtree->no->coordenada);
    double cord_y = ponto_obter_y(quadtree->no->coordenada);
    // Verifica se é possível avançar em cada uma das direções sem sair dos limites do retângulo.
    if (cord_x >= x1 && cord_y >= y1)
        valores_dentro_retangulo(quadtree->noroeste, x1, y1, x2, y2, valores, extrair_valor);

    if (cord_x <= x2 && cord_y >= y1)
        valores_dentro_retangulo(quadtree->nordeste, x1, y1, x2, y2, valores, extrair_valor);

    if (cord_x >= x1 && cord_y <= y2)
        valores_dentro_retangulo(quadtree->sudoeste, x1, y1, x2, y2, valores, extrair_valor);

    if (cord_x <= x2 && cord_y <= y2)
        valores_dentro_retangulo(quadtree->sudeste, x1, y1, x2, y2, valores, extrair_valor);
}

Lista chavesDentroRetanguloQt(QuadTree qt, double x1, double y1, double x2, double y2) {
    if (x1 > x2 || y1 > y2) {
        LOG_ERRO(
            "x1 e y1 devem ser menores que x2 e y2 respectivamente em chavesDentroRetanguloQt!\n");
        return NULL;
    }
    QuadTreeImp *quadtree = (QuadTreeImp *) qt;

    Lista chaves = lista_criar(obter_identificador_string, NULL);

    valores_dentro_retangulo(quadtree, x1, y1, x2, y2, chaves, quadtree_extrair_id_info);

    return chaves;
}

Lista pontosDentroRetanguloQt(QuadTree qt, double x1, double y1, double x2, double y2) {
    if (x1 > x2 || y1 > y2) {
        LOG_ERRO(
            "x1 e y1 devem ser menores que x2 e y2 respectivamente em pontosDentroRetanguloQt!\n");
        return NULL;
    }
    QuadTreeImp *quadtree = (QuadTreeImp *) qt;

    Lista pontos = lista_criar(NULL, NULL);

    valores_dentro_retangulo(quadtree, x1, y1, x2, y2, pontos, quadtree_extrair_ponto);

    return pontos;
}

Lista nosDentroRetanguloQt(QuadTree qt, double x1, double y1, double x2, double y2) {
    if (x1 > x2 || y1 > y2) {
        LOG_ERRO(
            "x1 e y1 devem ser menores que x2 e y2 respectivamente em nosDentroRetanguloQt!\n");
        return NULL;
    }
    QuadTreeImp *quadtree = (QuadTreeImp *) qt;

    Lista nos = lista_criar(NULL, NULL);

    valores_dentro_retangulo(quadtree, x1, y1, x2, y2, nos, quadtree_extrair_no);

    return nos;
}

// Percorre uma quadtree em busca de valores que estejam contidos dentro de um círculo. Quando
// um valor atende esse requisito a função passada como argumento é utilizada para extrair a
// informação necessária e adiciona-la na lista.
void valores_dentro_circulo(QuadTreeImp *quadtree, double x, double y, double raio, Lista *valores,
                            void *extrair_valor(QuadTreeImp *qt)) {
    // Caso o ponto esteja contido no circulo usa a função passada como argumento para extrair a
    // informação que será armazenada na lista.
    if (quadtree->no != NULL && ponto_contido_em_circulo(quadtree->no->coordenada, x, y, raio))
        lista_inserir_final(valores, extrair_valor(quadtree));

    // O nó não tem filhos.
    if (quadtree->nordeste == NULL)
        return;

    double cord_x = ponto_obter_x(quadtree->no->coordenada);
    double cord_y = ponto_obter_y(quadtree->no->coordenada);
    // Verifica se é possível avançar em cada uma das direções sem sair dos limites do círculo.
    if (cord_x >= x - raio && cord_y >= y - raio)
        valores_dentro_circulo(quadtree->noroeste, x, y, raio, valores, extrair_valor);

    if (cord_x <= x + raio && cord_y >= y - raio)
        valores_dentro_circulo(quadtree->nordeste, x, y, raio, valores, extrair_valor);

    if (cord_x >= x - raio && cord_y <= y + raio)
        valores_dentro_circulo(quadtree->sudoeste, x, y, raio, valores, extrair_valor);

    if (cord_x <= x + raio && cord_y <= y + raio)
        valores_dentro_circulo(quadtree->sudeste, x, y, raio, valores, extrair_valor);
}

Lista chavesDentroCirculoQt(QuadTree qt, double x, double y, double r) {
    QuadTreeImp *quadtree = (QuadTreeImp *) qt;

    Lista chaves = lista_criar(obter_identificador_string, NULL);

    valores_dentro_circulo(quadtree, x, y, r, chaves, quadtree_extrair_id_info);

    return chaves;
}

Lista pontosDentroCirculoQt(QuadTree qt, double x, double y, double r) {
    QuadTreeImp *quadtree = (QuadTreeImp *) qt;

    Lista pontos = lista_criar(NULL, NULL);

    valores_dentro_circulo(quadtree, x, y, r, pontos, quadtree_extrair_ponto);

    return pontos;
}

Lista nosDentroCirculoQt(QuadTree qt, double x, double y, double r) {
    QuadTreeImp *quadtree = (QuadTreeImp *) qt;

    Lista pontos = lista_criar(NULL, NULL);

    valores_dentro_circulo(quadtree, x, y, r, pontos, quadtree_extrair_no);

    return pontos;
}

void percorreProfundidadeQt(QuadTree qt, visitaNo f, ExtraInfo ei) {
    if (qt == NULL) {
        LOG_ERRO("Não é possível percorrer em profundidade uma quadtree nula!\n");
        return;
    }
    if (f == NULL) {
        LOG_ERRO(
            "Não é possível percorrer em profundidade uma quadtree sem uma função a ser aplicada "
            "nos nós!\n");
        return;
    }
    QuadTreeImp *quadtree = (QuadTreeImp *) qt;

    if (quadtree->no != NULL)
        f(quadtree->no->info, ei);

    if (quadtree->noroeste != NULL) {
        percorreProfundidadeQt(quadtree->noroeste, f, ei);
        percorreProfundidadeQt(quadtree->nordeste, f, ei);
        percorreProfundidadeQt(quadtree->sudeste, f, ei);
        percorreProfundidadeQt(quadtree->sudoeste, f, ei);
    }
}

void percorreLarguraQt(QuadTree qt, visitaNo f, ExtraInfo ei) {
    if (qt == NULL) {
        LOG_ERRO("Não é possível percorrer em largura uma quadtree nula!\n");
        return;
    }
    if (f == NULL) {
        LOG_ERRO(
            "Não é possível percorrer em largura uma quadtree sem uma função a ser aplicada nos "
            "nós!\n");
        return;
    }

    QuadTreeImp *raiz = (QuadTreeImp *) qt;
    Fila nos = fila_criar(NULL);
    fila_inserir(nos, raiz);

    while (!fila_esta_vazia(nos)) {
        QuadTreeImp *quadtree = fila_remover(nos);
        if (quadtree->no != NULL)
            f(quadtree->no->info, ei);

        if (quadtree->nordeste != NULL) {
            fila_inserir(nos, quadtree->noroeste);
            fila_inserir(nos, quadtree->nordeste);
            fila_inserir(nos, quadtree->sudeste);
            fila_inserir(nos, quadtree->sudoeste);
        }
    }
    fila_destruir(nos);
}

QtNo quadtree_inserir_no(QuadTree qt, QtNo pNo) {
    if (qt == NULL) {
        LOG_ERRO("Não é possível inserir em uma quadtree nula!\n");
        return NULL;
    }
    if (pNo == NULL) {
        LOG_ERRO("Não é possível inserir um nó nulo em uma quadtree!\n");
        return NULL;
    }

    QuadTreeImp *quadtree = (QuadTreeImp *) qt;
    QTNoImp *no = (QTNoImp *) pNo;
    if (no->coordenada == NULL) {
        LOG_ERRO("Não é possível inserir um nó que possui ponto nulo em uma quadtree!\n");
        return NULL;
    }
    if (no->info == NULL) {
        LOG_ERRO("Não é possível inserir um nó que possui uma informação nula em uma quadtree!\n");
        return NULL;
    }

    // Nó atual não possui informação e coordenada associadas a ele.
    if (quadtree->no == NULL) {
        quadtree->no = no;
        return quadtree;
    }

    // Nó já possui informação e coordenada associadas a ele, é necessário subdividir a árvore.
    if (quadtree->noroeste == NULL) {
        quadtree->noroeste = criaQt(quadtree->obter_identificador);
        quadtree->nordeste = criaQt(quadtree->obter_identificador);
        quadtree->sudoeste = criaQt(quadtree->obter_identificador);
        quadtree->sudeste = criaQt(quadtree->obter_identificador);
    }

    double novo_ponto_x = ponto_obter_x(no->coordenada);
    double novo_ponto_y = ponto_obter_y(no->coordenada);
    double cord_x = ponto_obter_x(quadtree->no->coordenada);
    double cord_y = ponto_obter_y(quadtree->no->coordenada);

    // Verifica em qual direção o novo nó deve ser inserido.
    if (novo_ponto_x <= cord_x && novo_ponto_y <= cord_y)
        return quadtree_inserir_no(quadtree->noroeste, no);

    if (novo_ponto_x >= cord_x && novo_ponto_y <= cord_y)
        return quadtree_inserir_no(quadtree->nordeste, no);

    if (novo_ponto_x <= cord_x && novo_ponto_y >= cord_y)
        return quadtree_inserir_no(quadtree->sudoeste, no);

    if (novo_ponto_x >= cord_x && novo_ponto_y >= cord_y)
        return quadtree_inserir_no(quadtree->sudeste, no);

    return NULL;
}

QtNo insereQt(QuadTree qt, Ponto ponto, QtInfo info) {
    if (qt == NULL) {
        LOG_ERRO("Não é possível inserir em uma quadtree nula!\n");
        return NULL;
    }
    if (ponto == NULL) {
        LOG_ERRO("Não é possível inserir um ponto nulo em uma quadtree!\n");
        return NULL;
    }
    if (info == NULL) {
        LOG_ERRO("Não é possível inserir uma informação nula em uma quadtree!\n");
        return NULL;
    }
    QTNoImp *no = malloc(sizeof(QTNoImp));
    no->coordenada = ponto;
    no->info = info;

    return quadtree_inserir_no(qt, no);
}

Lista quadtree_nos_para_lista(QuadTree qt) {
    QuadTreeImp *raiz = (QuadTreeImp *) qt;

    Lista dados = lista_criar(NULL, NULL);
    Fila nos = fila_criar(NULL);
    fila_inserir(nos, raiz->noroeste);
    fila_inserir(nos, raiz->nordeste);
    fila_inserir(nos, raiz->sudoeste);
    fila_inserir(nos, raiz->sudeste);

    while (!fila_esta_vazia(nos)) {
        QuadTreeImp *quadtree = fila_remover(nos);
        if (quadtree->no != NULL)
            lista_inserir_final(dados, quadtree->no);

        if (quadtree->nordeste != NULL) {
            fila_inserir(nos, quadtree->noroeste);
            fila_inserir(nos, quadtree->nordeste);
            fila_inserir(nos, quadtree->sudeste);
            fila_inserir(nos, quadtree->sudoeste);
        }
    }
    fila_destruir(nos);

    return dados;
}

void liberar_quadtree(QuadTree qt) {
    QuadTreeImp *quadtree = (QuadTreeImp *) qt;
    if (quadtree->noroeste != NULL) {
        liberar_quadtree(quadtree->noroeste);
        liberar_quadtree(quadtree->nordeste);
        liberar_quadtree(quadtree->sudeste);
        liberar_quadtree(quadtree->sudoeste);
    }
    free(quadtree);
}

QtInfo buscar_e_remover(QuadTree qt, QtNo no, QuadTree raiz) {
    QuadTreeImp *quadtree = (QuadTreeImp *) qt;
    // Nó atual não é o nó procurado.
    if (quadtree->no == NULL)
        return NULL;
    // Nó atual não é o nó procurado e não possui ponto filhos.
    if (quadtree->no != no && quadtree->nordeste == NULL)
        return NULL;

    // Nó atual não é o procurado mas possui filhos, avalia os nós filhos.
    if (quadtree->no != no && quadtree->nordeste != NULL) {
        QtInfo info = NULL;
        info = buscar_e_remover(quadtree->noroeste, no, raiz);
        if (info != NULL)
            return info;
        info = buscar_e_remover(quadtree->nordeste, no, raiz);
        if (info != NULL)
            return info;
        info = buscar_e_remover(quadtree->sudoeste, no, raiz);
        if (info != NULL)
            return info;
        info = buscar_e_remover(quadtree->sudeste, no, raiz);
        return info;
    }

    // Nó atual é o procurado, armazena a informação.
    QtInfo info = quadtree->no->info;

    // Libera a memória utilizada pelo nó atual.
    ponto_destruir(quadtree->no->coordenada);
    free(quadtree->no);
    quadtree->no = NULL;

    // Caso o nó que será removido tenha filhos, armazena os nós filhos e insere na raiz da árvore
    // novamente.
    if (quadtree->nordeste != NULL) {
        // Armazena os nós filhos.
        Lista dados = quadtree_nos_para_lista(quadtree);

        // Desaloca as quadtrees abaixo.
        liberar_quadtree(quadtree->noroeste);
        quadtree->noroeste = NULL;
        liberar_quadtree(quadtree->nordeste);
        quadtree->nordeste = NULL;
        liberar_quadtree(quadtree->sudoeste);
        quadtree->sudoeste = NULL;
        liberar_quadtree(quadtree->sudeste);
        quadtree->sudeste = NULL;

        // Re-insere os nós filhos na raiz da árvore.
        ListaNo no_atual = lista_obter_primeiro(dados);
        while (no_atual != NULL) {
            ListaNo no_proximo = lista_obter_proximo(no_atual);

            QtNo dado = lista_obter_info(no_atual);
            quadtree_inserir_no(raiz, dado);

            free(no_atual);
            no_atual = no_proximo;
        }
        free(dados);
    }

    return info;
}

void salvar_info_em_lista(QtInfo info, ExtraInfo lista) {
    lista_inserir_final(lista, info);
}

Lista quadtree_para_lista(QuadTree qt) {
    QuadTreeImp *quadtree = (QuadTreeImp *) qt;
    Lista lista = lista_criar(quadtree->obter_identificador, NULL);
    percorreLarguraQt(qt, salvar_info_em_lista, lista);
    return lista;
}

QtInfo removeNoQt(QuadTree qt, QtNo pNo) {
    if (qt == NULL || pNo == NULL) {
        LOG_ERRO("Valor nulo passado para removeNoQt!\n");
        return NULL;
    }
    return buscar_e_remover(qt, pNo, qt);
}

QtNo getNoQt(QuadTree qt, double x, double y) {
    if (qt == NULL) {
        LOG_ERRO("Quadtree nula passado para getNoQt!\n");
        return NULL;
    }
    QuadTreeImp *quadtree = (QuadTreeImp *) qt;

    if (quadtree->no != NULL) {
        double coord_x = ponto_obter_x(quadtree->no->coordenada);
        double coord_y = ponto_obter_y(quadtree->no->coordenada);
        if (coord_x == x && coord_y == y)
            return quadtree->no;
    }

    if (quadtree->nordeste != NULL) {
        QtNo no = NULL;
        no = getNoQt(quadtree->noroeste, x, y);
        if (no != NULL)
            return no;
        no = getNoQt(quadtree->nordeste, x, y);
        if (no != NULL)
            return no;
        no = getNoQt(quadtree->sudoeste, x, y);
        if (no != NULL)
            return no;
        no = getNoQt(quadtree->sudeste, x, y);
        return no;
    }

    return NULL;
}

QtNo quadtree_buscar_id(QuadTree qt, const char *id) {
    if (qt == NULL) {
        LOG_ERRO("Quadtree nula passado para getNoQt!\n");
        return NULL;
    }
    if (id == NULL) {
        LOG_ERRO("Não é possível buscar um id nulo em uma quadtree!\n");
        return NULL;
    }
    QuadTreeImp *quadtree = (QuadTreeImp *) qt;

    if (quadtree->no != NULL) {
        const char *info_id = quadtree->obter_identificador(quadtree->no->info);
        if (strcmp(info_id, id) == 0)
            return quadtree->no;
    }

    if (quadtree->nordeste != NULL) {
        QtNo no = quadtree_buscar_id(quadtree->noroeste, id);
        if (no == NULL)
            no = quadtree_buscar_id(quadtree->nordeste, id);
        if (no == NULL)
            no = quadtree_buscar_id(quadtree->sudoeste, id);
        if (no == NULL)
            no = quadtree_buscar_id(quadtree->sudeste, id);
        return no;
    }

    return NULL;
}

QtInfo getInfoQt(QtNo pNo) {
    if (pNo == NULL) {
        LOG_ERRO("Valor nulo passado para getInfoQt!\n");
        return NULL;
    }
    QTNoImp *no = (QTNoImp *) pNo;
    return no->info;
}

void desalocaQt(QuadTree qt) {
    if (qt == NULL) {
        LOG_ERRO("Quadtree nula passado para desalocaQT!\n");
        return;
    }
    QuadTreeImp *quadtree = (QuadTreeImp *) qt;

    if (quadtree->no != NULL) {
        ponto_destruir(quadtree->no->coordenada);
        figura_destruir(quadtree->no->info);
        free(quadtree->no);
    }

    if (quadtree->noroeste != NULL) {
        desalocaQt(quadtree->noroeste);
        desalocaQt(quadtree->nordeste);
        desalocaQt(quadtree->sudeste);
        desalocaQt(quadtree->sudoeste);
    }

    free(quadtree);
}
