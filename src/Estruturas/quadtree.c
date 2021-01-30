
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
    funcGetChave *obter_identificador;
    QTNoImp *no;
    struct QuadTreeImpT *noroeste;
    struct QuadTreeImpT *nordeste;
    struct QuadTreeImpT *sudoeste;
    struct QuadTreeImpT *sudeste;
} QuadTreeImp;

QuadTree criaQt(funcGetChave f) {
    QuadTreeImp *quadtree = malloc(sizeof *quadtree);
    quadtree->obter_identificador = f;
    quadtree->no = NULL;
    quadtree->noroeste = NULL;
    quadtree->nordeste = NULL;
    quadtree->sudoeste = NULL;
    quadtree->sudeste = NULL;
    return quadtree;
}

// Extrai o identificador de uma string, no caso a própria string.
const char *obter_identificador_string(ListaInfo info) {
    return info;
}

// Extrai o identificador de uma informação da Quadtree.
void *quadtree_extrair_id_info(QuadTreeImp *qt) {
    return (void *) qt->obter_identificador(qt->no->info);
}

// Extrai o ponto associado a uma Quadtree.
void *quadtree_extrair_ponto(QuadTreeImp *qt) {
    return qt->no->coordenada;
}

// Extrai o nó associado a uma quadtree.
void *quadtree_extrair_no(QuadTreeImp *qt) {
    return qt->no;
}

// Percorre uma quadtree em busca de valores que estejam contidos dentro de um retangulo. Quando
// um valor atende esse requisito a função passada como argumento é utilizada para extrair a
// informação necessária e adiciona-la na lista.
void valores_dentro_retangulo(QuadTreeImp *quadtree, double x1, double y1, double x2, double y2,
                              Lista valores, void *extrair_valor(QuadTreeImp *qt)) {
    // Caso o ponto esteja contido no retângulo usa a função passada como argumento para extrair a
    // informação que será armazenada na lista.
    if (quadtree->no != NULL &&
        ponto_contido_em_retangulo(quadtree->no->coordenada, x1, y1, x2, y2))
        lista_inserir_final(valores, extrair_valor(quadtree));

    // O nó não tem filhos.
    if (quadtree->nordeste == NULL)
        return;

    double coord_x = ponto_obter_x(quadtree->no->coordenada);
    double coord_y = ponto_obter_y(quadtree->no->coordenada);
    // Verifica se é possível avançar em cada uma das direções sem sair dos limites do retângulo.
    if (coord_x >= x1 && coord_y >= y1)
        valores_dentro_retangulo(quadtree->noroeste, x1, y1, x2, y2, valores, extrair_valor);

    if (coord_x <= x2 && coord_y >= y1)
        valores_dentro_retangulo(quadtree->nordeste, x1, y1, x2, y2, valores, extrair_valor);

    if (coord_x >= x1 && coord_y <= y2)
        valores_dentro_retangulo(quadtree->sudoeste, x1, y1, x2, y2, valores, extrair_valor);

    if (coord_x <= x2 && coord_y <= y2)
        valores_dentro_retangulo(quadtree->sudeste, x1, y1, x2, y2, valores, extrair_valor);
}

// Busca todos os ids das informações contidas no retângulo passado a função.
Lista chavesDentroRetanguloQt(QuadTree qt, double x1, double y1, double x2, double y2) {
    if (qt == NULL) {
        LOG_ERRO("Quadtree nula passada para chavesDentroRetanguloQt!\n");
        return NULL;
    }
    if (x1 > x2 || y1 > y2) {
        LOG_ERRO(
            "x1 e y1 devem ser menores que x2 e y2 respectivamente em chavesDentroRetanguloQt!\n");
        return NULL;
    }
    Lista chaves = lista_criar(obter_identificador_string, NULL);
    valores_dentro_retangulo(qt, x1, y1, x2, y2, chaves, quadtree_extrair_id_info);
    return chaves;
}

// Busca todos os pontos das informações contidas no retângulo passado a função.
Lista pontosDentroRetanguloQt(QuadTree qt, double x1, double y1, double x2, double y2) {
    if (qt == NULL) {
        LOG_ERRO("Quadtree nula passada para pontosDentroRetanguloQt!\n");
        return NULL;
    }
    if (x1 > x2 || y1 > y2) {
        LOG_ERRO(
            "x1 e y1 devem ser menores que x2 e y2 respectivamente em pontosDentroRetanguloQt!\n");
        return NULL;
    }
    Lista pontos = lista_criar(NULL, NULL);
    valores_dentro_retangulo(qt, x1, y1, x2, y2, pontos, quadtree_extrair_ponto);
    return pontos;
}

// Busca todos os nós das informações contidas no retângulo passado a função.
Lista nosDentroRetanguloQt(QuadTree qt, double x1, double y1, double x2, double y2) {
    if (qt == NULL) {
        LOG_ERRO("Quadtree nula passada para nosDentroRetanguloQt!\n");
        return NULL;
    }
    if (x1 > x2 || y1 > y2) {
        LOG_ERRO(
            "x1 e y1 devem ser menores que x2 e y2 respectivamente em nosDentroRetanguloQt!\n");
        return NULL;
    }
    Lista nos = lista_criar(NULL, NULL);
    valores_dentro_retangulo(qt, x1, y1, x2, y2, nos, quadtree_extrair_no);
    return nos;
}

// Percorre uma quadtree em busca de valores que estejam contidos dentro de um círculo. Quando
// um valor atende esse requisito a função passada como argumento é utilizada para extrair a
// informação necessária e adiciona-la na lista.
void valores_dentro_circulo(QuadTreeImp *quadtree, double x, double y, double raio, Lista valores,
                            void *extrair_valor(QuadTreeImp *qt)) {
    // Caso o ponto esteja contido no circulo usa a função passada como argumento para extrair a
    // informação que será armazenada na lista.
    if (quadtree->no != NULL && ponto_contido_em_circulo(quadtree->no->coordenada, x, y, raio))
        lista_inserir_final(valores, extrair_valor(quadtree));

    // O nó não tem filhos.
    if (quadtree->nordeste == NULL)
        return;

    double coord_x = ponto_obter_x(quadtree->no->coordenada);
    double coord_y = ponto_obter_y(quadtree->no->coordenada);
    // Verifica se é possível avançar em cada uma das direções sem sair dos limites do círculo.
    if (coord_x >= x - raio && coord_y >= y - raio)
        valores_dentro_circulo(quadtree->noroeste, x, y, raio, valores, extrair_valor);

    if (coord_x <= x + raio && coord_y >= y - raio)
        valores_dentro_circulo(quadtree->nordeste, x, y, raio, valores, extrair_valor);

    if (coord_x >= x - raio && coord_y <= y + raio)
        valores_dentro_circulo(quadtree->sudoeste, x, y, raio, valores, extrair_valor);

    if (coord_x <= x + raio && coord_y <= y + raio)
        valores_dentro_circulo(quadtree->sudeste, x, y, raio, valores, extrair_valor);
}

// Busca todas as chaves das informações contidas no círculo passado a função.
Lista chavesDentroCirculoQt(QuadTree qt, double x, double y, double r) {
    if (qt == NULL) {
        LOG_ERRO("Quadtree nula passada para chavesDentroCirculoQt!\n");
        return NULL;
    }
    if (r < 1) {
        LOG_ERRO("Raio menor que 1 passado para chavesDentroCirculoQt!\n");
        return NULL;
    }
    Lista chaves = lista_criar(obter_identificador_string, NULL);
    valores_dentro_circulo(qt, x, y, r, chaves, quadtree_extrair_id_info);
    return chaves;
}

// Busca todos os pontos das informações contidas no círculo passado a função.
Lista pontosDentroCirculoQt(QuadTree qt, double x, double y, double r) {
    if (qt == NULL) {
        LOG_ERRO("Quadtree nula passada para pontosDentroCirculoQt!\n");
        return NULL;
    }
    if (r <= 0) {
        LOG_ERRO("Raio menor ou igual a 0 passado para pontosDentroCirculoQt!\n");
        return NULL;
    }
    Lista pontos = lista_criar(NULL, NULL);
    valores_dentro_circulo(qt, x, y, r, pontos, quadtree_extrair_ponto);
    return pontos;
}

// Busca todos os nós das informações contidas no círculo passado a função.
Lista nosDentroCirculoQt(QuadTree qt, double x, double y, double r) {
    if (qt == NULL) {
        LOG_ERRO("Quadtree nula passada para nosDentroCirculoQt!\n");
        return NULL;
    }
    if (r <= 0) {
        LOG_ERRO("Raio menor ou igual a 0 passado para nosDentroCirculoQt!\n");
        return NULL;
    }
    Lista pontos = lista_criar(NULL, NULL);
    valores_dentro_circulo(qt, x, y, r, pontos, quadtree_extrair_no);
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
    QuadTreeImp *quadtree = qt;

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

    QuadTreeImp *raiz = qt;
    Fila nos = fila_criar(NULL);
    fila_inserir(nos, raiz);

    while (!fila_esta_vazia(nos)) {
        QuadTreeImp *quadtree = fila_remover(nos);
        if (quadtree->no != NULL)
            f(quadtree->no->info, ei);

        if (quadtree->noroeste != NULL) {
            fila_inserir(nos, quadtree->noroeste);
            fila_inserir(nos, quadtree->nordeste);
            fila_inserir(nos, quadtree->sudeste);
            fila_inserir(nos, quadtree->sudoeste);
        }
    }
    fila_destruir(nos);
}

QtNo quadtree_inserir_no(QuadTreeImp *quadtree, QTNoImp *no) {
    // Nó atual não possui informação e coordenada associadas a ele.
    if (quadtree->no == NULL) {
        quadtree->no = no;
        return no;
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
    double coord_x = ponto_obter_x(quadtree->no->coordenada);
    double coord_y = ponto_obter_y(quadtree->no->coordenada);

    // Verifica em qual direção o novo nó deve ser inserido.
    if (novo_ponto_x <= coord_x && novo_ponto_y <= coord_y)
        return quadtree_inserir_no(quadtree->noroeste, no);

    if (novo_ponto_x >= coord_x && novo_ponto_y <= coord_y)
        return quadtree_inserir_no(quadtree->nordeste, no);

    if (novo_ponto_x <= coord_x && novo_ponto_y >= coord_y)
        return quadtree_inserir_no(quadtree->sudoeste, no);

    if (novo_ponto_x >= coord_x && novo_ponto_y >= coord_y)
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
    QTNoImp *no = malloc(sizeof *no);
    if (no == NULL) {
        LOG_ERRO("Falha ao alocar memória para novo nó da Quadtree!\n");
        return NULL;
    }
    no->coordenada = ponto;
    no->info = info;

    return quadtree_inserir_no(qt, no);
}

// Libera apenas a Quadtree passada a função e as Quadtrees filhas.
void reinserir_nos_quadtree(QuadTreeImp *quadtree, QuadTreeImp *raiz) {
    if (quadtree->no != NULL)
        quadtree_inserir_no(raiz, quadtree->no);

    if (quadtree->noroeste != NULL) {
        reinserir_nos_quadtree(quadtree->noroeste, raiz);
        reinserir_nos_quadtree(quadtree->nordeste, raiz);
        reinserir_nos_quadtree(quadtree->sudeste, raiz);
        reinserir_nos_quadtree(quadtree->sudoeste, raiz);
    }
    free(quadtree);
}

// Encontra o nó especificado na Quadtree e o remove da árvore. Caso ele possua nós filhos eles são
// re-inseridos na raiz da árvore.
QtInfo buscar_e_remover(QuadTree qt, QtNo no, QuadTree raiz) {
    QuadTreeImp *quadtree = qt;
    // Nó atual não é o nó procurado.
    if (quadtree->no == NULL || (quadtree->no != no && quadtree->nordeste == NULL))
        return NULL;

    // Nó atual não é o procurado mas possui filhos, avalia os nós filhos.
    if (quadtree->no != no && quadtree->noroeste != NULL) {
        QtInfo info = buscar_e_remover(quadtree->noroeste, no, raiz);
        if (info == NULL)
            info = buscar_e_remover(quadtree->nordeste, no, raiz);
        if (info == NULL)
            info = buscar_e_remover(quadtree->sudoeste, no, raiz);
        if (info == NULL)
            info = buscar_e_remover(quadtree->sudeste, no, raiz);
        return info;
    }

    // Nó atual é o procurado, armazena a informação.
    QtInfo info = quadtree->no->info;

    // Libera a memória utilizada pelo nó atual.
    ponto_destruir(quadtree->no->coordenada);
    free(quadtree->no);
    quadtree->no = NULL;

    // Caso o nó que será removido tenha filhos, insere os nós filhos na raiz da árvore.
    if (quadtree->noroeste != NULL) {
        QuadTreeImp *noroeste = quadtree->noroeste;
        quadtree->noroeste = NULL;

        QuadTreeImp *nordeste = quadtree->nordeste;
        quadtree->nordeste = NULL;

        QuadTreeImp *sudoeste = quadtree->sudoeste;
        quadtree->sudoeste = NULL;

        QuadTreeImp *sudeste = quadtree->sudeste;
        quadtree->sudeste = NULL;

        reinserir_nos_quadtree(noroeste, raiz);
        reinserir_nos_quadtree(nordeste, raiz);
        reinserir_nos_quadtree(sudoeste, raiz);
        reinserir_nos_quadtree(sudeste, raiz);
    }

    return info;
}

QtInfo removeNoQt(QuadTree qt, QtNo pNo) {
    if (qt == NULL || pNo == NULL) {
        LOG_ERRO("Valor nulo passado para removeNoQt!\n");
        return NULL;
    }
    return buscar_e_remover(qt, pNo, qt);
}

// Percorre uma Quadtree em largura e armazena todos os nós em uma lista.
Lista quadtree_nos_para_lista(QuadTree qt) {
    if (qt == NULL) {
        LOG_ERRO("Quadtree nula passada para quadtree_nos_para_lista!\n");
        return NULL;
    }
    QuadTreeImp *raiz = qt;

    Lista dados = lista_criar(NULL, NULL);
    Fila nos = fila_criar(NULL);
    fila_inserir(nos, raiz);

    while (!fila_esta_vazia(nos)) {
        QuadTreeImp *quadtree = fila_remover(nos);
        if (quadtree->no != NULL)
            lista_inserir_final(dados, quadtree->no);

        if (quadtree->noroeste != NULL) {
            fila_inserir(nos, quadtree->noroeste);
            fila_inserir(nos, quadtree->nordeste);
            fila_inserir(nos, quadtree->sudeste);
            fila_inserir(nos, quadtree->sudoeste);
        }
    }
    fila_destruir(nos);

    return dados;
}

void salvar_info_em_lista(QtInfo info, ExtraInfo lista) {
    lista_inserir_final(lista, info);
}

// Percorre uma Quadtree em largura e armazena todos as informações em uma lista.
Lista quadtree_para_lista(QuadTree qt) {
    if (qt == NULL) {
        LOG_ERRO("Quadtree nula passada para quadtree_para_lista!\n");
        return NULL;
    }
    QuadTreeImp *quadtree = qt;
    Lista lista = lista_criar(quadtree->obter_identificador, NULL);
    percorreLarguraQt(qt, salvar_info_em_lista, lista);
    return lista;
}

QtNo getNoQt(QuadTree qt, double x, double y) {
    if (qt == NULL) {
        LOG_ERRO("Quadtree nula passado para getNoQt!\n");
        return NULL;
    }
    QuadTreeImp *quadtree = qt;

    if (quadtree->no == NULL)
        return NULL;

    double coord_x = ponto_obter_x(quadtree->no->coordenada);
    double coord_y = ponto_obter_y(quadtree->no->coordenada);
    if (coord_x == x && coord_y == y)
        return quadtree->no;

    if (quadtree->noroeste != NULL) {
        if (x <= coord_x && y <= coord_y)
            return getNoQt(quadtree->noroeste, x, y);

        if (x >= coord_x && y <= coord_y)
            return getNoQt(quadtree->nordeste, x, y);

        if (x <= coord_x && y >= coord_y)
            return getNoQt(quadtree->sudoeste, x, y);

        if (x >= coord_x && y >= coord_y)
            return getNoQt(quadtree->sudeste, x, y);
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
    QuadTreeImp *raiz = qt;
    if (raiz->obter_identificador == NULL) {
        LOG_ERRO(
            "Não é possível buscar um id em uma quadtree que não tem a função obter_identificador "
            "definida!\n");
        return NULL;
    }

    Fila nos = fila_criar(NULL);
    fila_inserir(nos, raiz);

    while (!fila_esta_vazia(nos)) {
        QuadTreeImp *quadtree = fila_remover(nos);
        if (quadtree->no != NULL) {
            const char *info_id = quadtree->obter_identificador(quadtree->no->info);
            if (strcmp(info_id, id) == 0) {
                fila_destruir(nos);
                return quadtree->no;
            }
        }

        if (quadtree->noroeste != NULL) {
            fila_inserir(nos, quadtree->noroeste);
            fila_inserir(nos, quadtree->nordeste);
            fila_inserir(nos, quadtree->sudeste);
            fila_inserir(nos, quadtree->sudoeste);
        }
    }
    fila_destruir(nos);

    return NULL;
}

QtInfo getInfoQt(QuadTree qt, QtNo pNo) {
    if (qt == NULL || pNo == NULL) {
        LOG_ERRO("Valor nulo passado para getInfoQt!\n");
        return NULL;
    }
    QTNoImp *no = pNo;
    return no->info;
}

void desalocaQt(QuadTree qt) {
    if (qt == NULL) {
        LOG_ERRO("Quadtree nula passado para desalocaQT!\n");
        return;
    }
    QuadTreeImp *quadtree = qt;

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
