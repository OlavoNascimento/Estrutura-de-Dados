#include "quadtree.h"

#include <stdlib.h>
#include <string.h>

#include "../Estruturas/fila.h"
#include "../Estruturas/lista.h"
#include "../Interfaces/estrutura_mapeavel.h"
#include "../Interfaces/figura.h"
#include "../Objetos/Formas/linha.h"
#include "../Objetos/Formas/ponto.h"
#include "../Objetos/Formas/retangulo.h"
#include "../Objetos/Outros/texto.h"
#include "../Utils/logging.h"
#include "../Utils/matematica.h"

struct QtNo_s {
    Ponto coordenada;
    QtInfo info;
};

struct QuadTree_s {
    FuncEstruturaMap *map;
    funcGetChave *obter_identificador;
    struct QtNo_s *no;
    struct QuadTree_s *noroeste;
    struct QuadTree_s *nordeste;
    struct QuadTree_s *sudoeste;
    struct QuadTree_s *sudeste;
};

QuadTree criaQt(funcGetChave f) {
    QuadTree quadtree = malloc(sizeof *quadtree);
    if (quadtree == NULL) {
        LOG_ERRO("Falha ao alocar memória para uma Quadtree!\n");
        return NULL;
    }
    quadtree->obter_identificador = f;
    quadtree->map = (FuncEstruturaMap *) percorreLarguraQt;
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
const char *quadtree_extrair_id_info(QuadTree qt) {
    return qt->obter_identificador(qt->no->info);
}

// Extrai o ponto associado a uma Quadtree.
void *quadtree_extrair_ponto(QuadTree qt) {
    return qt->no->coordenada;
}

// Extrai o nó associado a uma quadtree.
void *quadtree_extrair_no(QuadTree qt) {
    return qt->no;
}

// Percorre uma quadtree em busca de valores que estejam contidos dentro de um retangulo. Quando
// um valor atende esse requisito a função passada como argumento é utilizada para extrair a
// informação necessária e adiciona-la na lista.
void valores_dentro_retangulo(QuadTree qt, double x1, double y1, double x2, double y2,
                              Lista valores, void *extrair_valor(QuadTree qt)) {
    // O nó não tem filhos.
    if (qt == NULL || qt->no == NULL)
        return;

    // Caso o ponto esteja contido no retângulo usa a função passada como argumento para extrair a
    // informação que será armazenada na lista.
    if (ponto_contido_em_retangulo(qt->no->coordenada, x1, y1, x2, y2))
        lista_inserir_final(valores, extrair_valor(qt));

    double coord_x = ponto_obter_x(qt->no->coordenada);
    double coord_y = ponto_obter_y(qt->no->coordenada);
    // Verifica se é possível avançar em cada uma das direções sem sair dos limites do retângulo.
    if (coord_x >= x1 && coord_y >= y1)
        valores_dentro_retangulo(qt->noroeste, x1, y1, x2, y2, valores, extrair_valor);
    if (coord_x <= x2 && coord_y >= y1)
        valores_dentro_retangulo(qt->nordeste, x1, y1, x2, y2, valores, extrair_valor);
    if (coord_x >= x1 && coord_y <= y2)
        valores_dentro_retangulo(qt->sudoeste, x1, y1, x2, y2, valores, extrair_valor);
    if (coord_x <= x2 && coord_y <= y2)
        valores_dentro_retangulo(qt->sudeste, x1, y1, x2, y2, valores, extrair_valor);
}

// Busca todos os ids das informações contidas no retângulo passado a função.
Lista chavesDentroRetanguloQt(QuadTree qt, double x1, double y1, double x2, double y2) {
    if (x1 > x2 || y1 > y2) {
        LOG_AVISO(
            "x1 e y1 devem ser menores que x2 e y2 respectivamente em chavesDentroRetanguloQt!\n");
        return NULL;
    }
    Lista chaves = lista_criar(obter_identificador_string, NULL);
    valores_dentro_retangulo(qt, x1, y1, x2, y2, chaves, (void *) quadtree_extrair_id_info);
    return chaves;
}

// Busca todos os pontos das informações contidas no retângulo passado a função.
Lista pontosDentroRetanguloQt(QuadTree qt, double x1, double y1, double x2, double y2) {
    if (x1 > x2 || y1 > y2) {
        LOG_AVISO(
            "x1 e y1 devem ser menores que x2 e y2 respectivamente em pontosDentroRetanguloQt!\n");
        return NULL;
    }
    Lista pontos = lista_criar(NULL, NULL);
    valores_dentro_retangulo(qt, x1, y1, x2, y2, pontos, quadtree_extrair_ponto);
    return pontos;
}

// Busca todos os nós das informações contidas no retângulo passado a função.
Lista nosDentroRetanguloQt(QuadTree qt, double x1, double y1, double x2, double y2) {
    if (x1 > x2 || y1 > y2) {
        LOG_AVISO(
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
void valores_dentro_circulo(QuadTree qt, double x, double y, double raio, Lista valores,
                            void *extrair_valor(QuadTree qt)) {
    // O nó não tem filhos.
    if (qt == NULL || qt->no == NULL)
        return;

    // Caso o ponto esteja contido no circulo usa a função passada como argumento para extrair a
    // informação que será armazenada na lista.
    if (ponto_contido_em_circulo(qt->no->coordenada, x, y, raio))
        lista_inserir_final(valores, extrair_valor(qt));

    double coord_x = ponto_obter_x(qt->no->coordenada);
    double coord_y = ponto_obter_y(qt->no->coordenada);
    // Verifica se é possível avançar em cada uma das direções sem sair dos limites do círculo.
    if (coord_x >= x - raio && coord_y >= y - raio)
        valores_dentro_circulo(qt->noroeste, x, y, raio, valores, extrair_valor);
    if (coord_x <= x + raio && coord_y >= y - raio)
        valores_dentro_circulo(qt->nordeste, x, y, raio, valores, extrair_valor);
    if (coord_x >= x - raio && coord_y <= y + raio)
        valores_dentro_circulo(qt->sudoeste, x, y, raio, valores, extrair_valor);
    if (coord_x <= x + raio && coord_y <= y + raio)
        valores_dentro_circulo(qt->sudeste, x, y, raio, valores, extrair_valor);
}

// Busca todas as chaves das informações contidas no círculo passado a função.
Lista chavesDentroCirculoQt(QuadTree qt, double x, double y, double r) {
    if (r <= 0) {
        LOG_AVISO("Raio menor ou igual a 0 passado para chavesDentroCirculoQt!\n");
        return NULL;
    }
    Lista chaves = lista_criar(obter_identificador_string, NULL);
    valores_dentro_circulo(qt, x, y, r, chaves, (void *) quadtree_extrair_id_info);
    return chaves;
}

// Busca todos os pontos das informações contidas no círculo passado a função.
Lista pontosDentroCirculoQt(QuadTree qt, double x, double y, double r) {
    if (r <= 0) {
        LOG_AVISO("Raio menor ou igual a 0 passado para pontosDentroCirculoQt!\n");
        return NULL;
    }
    Lista pontos = lista_criar(NULL, NULL);
    valores_dentro_circulo(qt, x, y, r, pontos, quadtree_extrair_ponto);
    return pontos;
}

// Busca todos os nós das informações contidas no círculo passado a função.
Lista nosDentroCirculoQt(QuadTree qt, double x, double y, double r) {
    if (r <= 0) {
        LOG_AVISO("Raio menor ou igual a 0 passado para nosDentroCirculoQt!\n");
        return NULL;
    }
    Lista pontos = lista_criar(NULL, NULL);
    valores_dentro_circulo(qt, x, y, r, pontos, quadtree_extrair_no);
    return pontos;
}

void percorreProfundidadeQt(QuadTree qt, visitaNo f, ExtraInfo ei) {
    if (qt == NULL || qt->no == NULL)
        return;
    f(qt->no->info, ei);
    percorreProfundidadeQt(qt->noroeste, f, ei);
    percorreProfundidadeQt(qt->nordeste, f, ei);
    percorreProfundidadeQt(qt->sudeste, f, ei);
    percorreProfundidadeQt(qt->sudoeste, f, ei);
}

void percorreLarguraQt(QuadTree qt, visitaNo f, ExtraInfo ei) {
    if (f == NULL) {
        LOG_AVISO(
            "Não é possível percorrer em largura uma quadtree sem uma função a ser aplicada nos "
            "nós!\n");
        return;
    }

    Fila nos = fila_criar(NULL);
    fila_inserir(nos, qt);

    while (!fila_esta_vazia(nos)) {
        QuadTree atual = fila_remover(nos);
        if (atual != NULL) {
            if (atual->no != NULL)
                f(atual->no->info, ei);

            fila_inserir(nos, atual->noroeste);
            fila_inserir(nos, atual->nordeste);
            fila_inserir(nos, atual->sudeste);
            fila_inserir(nos, atual->sudoeste);
        }
    }
    fila_destruir(nos);
}

void quadtree_inserir_no(QuadTree qt, QtNo no) {
    // Nó atual não possui informação e coordenada associadas a ele.
    if (qt->no == NULL) {
        qt->no = no;
        return;
    }

    double novo_ponto_x = ponto_obter_x(no->coordenada);
    double novo_ponto_y = ponto_obter_y(no->coordenada);
    double coord_x = ponto_obter_x(qt->no->coordenada);
    double coord_y = ponto_obter_y(qt->no->coordenada);

    // Verifica em qual direção o novo nó deve ser inserido.
    if (novo_ponto_x <= coord_x && novo_ponto_y <= coord_y) {
        if (qt->noroeste == NULL)
            qt->noroeste = criaQt(qt->obter_identificador);
        return quadtree_inserir_no(qt->noroeste, no);
    }
    if (novo_ponto_x >= coord_x && novo_ponto_y <= coord_y) {
        if (qt->nordeste == NULL)
            qt->nordeste = criaQt(qt->obter_identificador);
        return quadtree_inserir_no(qt->nordeste, no);
    }
    if (novo_ponto_x <= coord_x && novo_ponto_y >= coord_y) {
        if (qt->sudoeste == NULL)
            qt->sudoeste = criaQt(qt->obter_identificador);
        return quadtree_inserir_no(qt->sudoeste, no);
    }
    if (qt->sudeste == NULL)
        qt->sudeste = criaQt(qt->obter_identificador);
    return quadtree_inserir_no(qt->sudeste, no);
}

QtNo insereQt(QuadTree qt, Ponto ponto, QtInfo info) {
    if (ponto == NULL) {
        LOG_AVISO("Não é possível inserir um ponto nulo em uma quadtree!\n");
        return NULL;
    }
    if (info == NULL) {
        LOG_AVISO("Não é possível inserir uma informação nula em uma quadtree!\n");
        return NULL;
    }
    QtNo no = malloc(sizeof *no);
    if (no == NULL) {
        LOG_ERRO("Falha ao alocar memória\n");
        return NULL;
    }
    no->coordenada = ponto;
    no->info = info;
    quadtree_inserir_no(qt, no);
    return no;
}

// Insere os nós de uma quadtree em uma outra Quadtree.
void reinserir_nos_quadtree(QuadTree qt, QuadTree raiz) {
    if (qt == NULL)
        return;
    if (qt->no != NULL) {
        quadtree_inserir_no(raiz, qt->no);
        reinserir_nos_quadtree(qt->noroeste, raiz);
        reinserir_nos_quadtree(qt->nordeste, raiz);
        reinserir_nos_quadtree(qt->sudeste, raiz);
        reinserir_nos_quadtree(qt->sudoeste, raiz);
    }
    free(qt);
}

// Encontra o nó especificado na Quadtree e o remove da árvore. Caso ele possua nós filhos eles são
// re-inseridos na raiz da árvore.
QtInfo buscar_e_remover(QuadTree qt, QtNo no, QuadTree raiz) {
    // Nó atual não é o nó procurado.
    if (qt == NULL || qt->no == NULL)
        return NULL;

    // Nó atual não é o procurado mas possui filhos, avalia os nós filhos.
    if (qt->no != no) {
        double buscado_x = ponto_obter_x(no->coordenada);
        double buscado_y = ponto_obter_y(no->coordenada);
        double atual_x = ponto_obter_x(qt->no->coordenada);
        double atual_y = ponto_obter_y(qt->no->coordenada);

        if (buscado_x <= atual_x && buscado_y <= atual_y)
            return buscar_e_remover(qt->noroeste, no, raiz);
        if (buscado_x >= atual_x && buscado_y <= atual_y)
            return buscar_e_remover(qt->nordeste, no, raiz);
        if (buscado_x <= atual_x && buscado_y >= atual_y)
            return buscar_e_remover(qt->sudoeste, no, raiz);
        return buscar_e_remover(qt->sudeste, no, raiz);
    }

    // Nó atual é o procurado.
    QtInfo info = qt->no->info;
    // Libera a memória utilizada pelo nó atual.
    ponto_destruir(qt->no->coordenada);
    free(qt->no);
    qt->no = NULL;

    // Caso o nó que será removido tenha filhos, insere os nós filhos na raiz da árvore.
    QuadTree noroeste = qt->noroeste;
    qt->noroeste = NULL;

    QuadTree nordeste = qt->nordeste;
    qt->nordeste = NULL;

    QuadTree sudoeste = qt->sudoeste;
    qt->sudoeste = NULL;

    QuadTree sudeste = qt->sudeste;
    qt->sudeste = NULL;

    reinserir_nos_quadtree(noroeste, raiz);
    reinserir_nos_quadtree(nordeste, raiz);
    reinserir_nos_quadtree(sudoeste, raiz);
    reinserir_nos_quadtree(sudeste, raiz);
    return info;
}

QtInfo removeNoQt(QuadTree qt, QtNo pNo) {
    if (qt == NULL || pNo == NULL) {
        LOG_AVISO("Valor nulo passado para removeNoQt!\n");
        return NULL;
    }
    return buscar_e_remover(qt, pNo, qt);
}

QtNo getNoQt(QuadTree qt, double x, double y) {
    if (qt == NULL || qt->no == NULL)
        return NULL;

    double coord_x = ponto_obter_x(qt->no->coordenada);
    double coord_y = ponto_obter_y(qt->no->coordenada);
    if (coord_x == x && coord_y == y)
        return qt->no;

    if (x <= coord_x && y <= coord_y)
        return getNoQt(qt->noroeste, x, y);
    if (x >= coord_x && y <= coord_y)
        return getNoQt(qt->nordeste, x, y);
    if (x <= coord_x && y >= coord_y)
        return getNoQt(qt->sudoeste, x, y);
    return getNoQt(qt->sudeste, x, y);
}

QtInfo getInfoQt(QuadTree qt, QtNo pNo) {
    if (pNo == NULL) {
        printf("%p\n", qt);
        return NULL;
    }
    return pNo->info;
}

Retangulo escrever_retangulo_principal(Lista saida, QuadTree qt, double x, double y, double largura,
                                       double altura) {
    // Retângulo com as coordenadas da figura.
    Retangulo ret_coord = retangulo_criar("", largura * 2 + 2, altura, x, y, "black", "#f1ffe8");
    lista_inserir_final(saida, ret_coord);

    char texto_cords[1024] = {'\0'};
    snprintf(texto_cords, 500, "(%d,%d)", (int) ponto_obter_x(qt->no->coordenada),
             (int) ponto_obter_y(qt->no->coordenada));
    Texto coordenadas =
        texto_criar("", figura_obter_x_centro(ret_coord), figura_obter_y_centro(ret_coord) + 6,
                    "none", "black", texto_cords);
    texto_definir_alinhamento(coordenadas, TEXTO_CENTRO);
    lista_inserir_final(saida, coordenadas);

    // Retângulo com o id da figura.
    Retangulo ret_id =
        retangulo_criar("", largura * 2 + 2, altura, x + largura * 2 + 4, y, "black", "#f1ffe8");
    lista_inserir_final(saida, ret_id);

    Texto texto_id =
        texto_criar("", figura_obter_x_centro(ret_id), figura_obter_y_centro(ret_id) + 6, "none",
                    "black", figura_obter_id(qt->no->info));
    texto_definir_alinhamento(texto_id, TEXTO_CENTRO);
    lista_inserir_final(saida, texto_id);

    return ret_id;
}

void escrever_quadrante(Lista saida, char *nome_quadrante, double x, double y, double largura,
                        double altura, char *cor, Retangulo filho) {
    Retangulo ret_id = retangulo_criar("", largura, altura, x, y, "black", cor);
    lista_inserir_final(saida, ret_id);

    Texto texto_id =
        texto_criar("", figura_obter_x_centro(ret_id), figura_obter_y_centro(ret_id) + 6, "none",
                    "black", nome_quadrante);
    texto_definir_alinhamento(texto_id, TEXTO_CENTRO);
    lista_inserir_final(saida, texto_id);

    if (filho != NULL) {
        // Conecta a posição atual (noroeste/nordeste/sudoeste/sudeste) ao filho.
        Linha ligacao = linha_criar(figura_obter_x_centro(ret_id), figura_obter_y_fim(ret_id),
                                    figura_obter_x_inicio(filho) - 1, figura_obter_y_inicio(filho),
                                    "black", "black");
        lista_inserir_final(saida, ligacao);
    }
}

Retangulo escrever_svg_no(Lista saida, QuadTree qt, double *x, double y) {
    if (qt == NULL || qt->no == NULL)
        return NULL;

    const double largura = 44;
    const double altura = 20;
    const double margem_y = 200;

    Retangulo noroeste = escrever_svg_no(saida, qt->noroeste, x, y + margem_y);
    *x += largura;
    Retangulo nordeste = escrever_svg_no(saida, qt->nordeste, x, y + margem_y);
    *x += largura;

    double x_atual = *x;
    Retangulo ret_id = escrever_retangulo_principal(saida, qt, *x, y, largura, altura);
    *x += largura;

    Retangulo sudoeste = escrever_svg_no(saida, qt->sudoeste, x, y + margem_y);
    *x += largura;
    Retangulo sudeste = escrever_svg_no(saida, qt->sudeste, x, y + margem_y);
    *x += largura;

    escrever_quadrante(saida, "NO", x_atual, y + altura + 2, largura, altura, "#c9cab3", noroeste);
    escrever_quadrante(saida, "NE", x_atual + largura + 2, y + altura + 2, largura, altura,
                       "#f1daaa", nordeste);
    escrever_quadrante(saida, "SO", x_atual + largura * 2 + 4, y + altura + 2, largura, altura,
                       "#cde790", sudoeste);
    escrever_quadrante(saida, "SE", x_atual + largura * 3 + 6, y + altura + 2, largura, altura,
                       "#b7e6d8", sudeste);
    return ret_id;
}

Lista quadtree_escrever_svg(QuadTree qt) {
    Lista saida = lista_criar(NULL, figura_destruir);
    double *x = malloc(sizeof *x);
    *x = 0;
    escrever_svg_no(saida, qt, x, 0);
    free(x);
    return saida;
}

void desalocaQt(QuadTree qt) {
    if (qt == NULL)
        return;
    if (qt->no != NULL) {
        ponto_destruir(qt->no->coordenada);
        free(qt->no);
        desalocaQt(qt->noroeste);
        desalocaQt(qt->nordeste);
        desalocaQt(qt->sudeste);
        desalocaQt(qt->sudoeste);
    }
    free(qt);
}
