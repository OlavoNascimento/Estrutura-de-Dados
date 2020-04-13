#include <stdlib.h>
#include <string.h>
#include <float.h>

#include <lista.h>
#include <figuras.h>

Lista* criar_lista() {
    Lista *lis = (Lista*) malloc(sizeof(Lista));
    lis->cabeca = NULL;
    lis->cauda= NULL;
    lis->exibicao.origem_x = DBL_MAX;
    lis->exibicao.origem_y = DBL_MAX;
    lis->exibicao.largura = 0;
    lis->exibicao.altura = 0;
    return lis;
}

void inserir_lista(Lista *lista, Figuras fig, TiposFigura fig_tipo) {
    struct No *no = (struct No*) malloc(sizeof(struct No));
    no->figura = fig;
    no->tipo = fig_tipo;
    no->prox = NULL;
    atualizar_exibicao_svg(&lista->exibicao, fig, fig_tipo);

    struct No *atual = lista->cauda;
    if(atual == NULL) {
        // Primeiro elemento da lista
        lista->cabeca = no;
        lista->cauda = no;
    } else {
        // Elementos após o primeiro
        atual->prox = no;
        lista->cauda = no;
    }
}

struct No* buscar_elemento_id_lista(Lista *lista, char *id_buscado) {
    struct No *atual = lista->cabeca;
    while(atual != NULL) {
        char *id_atual = obter_id_figura(&atual->figura, atual->tipo);
        if(id_atual != NULL && strcmp(id_atual, id_buscado) == 0)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

struct No* buscar_elemento_posicao_lista(Lista *lista, int posicao_buscada) {
    int posicao_atual = 0;
    struct No *atual = lista->cabeca;
    while(atual != NULL && posicao_atual != posicao_buscada) {
        atual = atual->prox;
        posicao_atual++;
    }
    if(posicao_atual == posicao_buscada)
        return atual;
    return NULL;
}

void remover_elemento_lista(Lista *lista, char *id) {
    struct No *atual = lista->cabeca;
    struct No *anterior;
    while(atual != NULL) {
        char *id_atual = obter_id_figura(&atual->figura, atual->tipo);
        if(id_atual != NULL && strcmp(id_atual, id) == 0) {
            if(atual == lista->cabeca) {
                lista->cabeca = atual->prox;
            } else if(atual == lista->cauda) {
                lista->cauda = anterior;
                anterior->prox = NULL;
            } else {
                anterior->prox = atual->prox;
            }
            free(atual);
            break;
        } else {
            anterior = atual;
            atual = atual->prox;
        }
    }
}

void lista_para_svg(Lista *lista, char *caminho_svg) {
    FILE *arquivo = fopen(caminho_svg, "w");
    if(arquivo == NULL) {
        fprintf(stderr, "Arquivo %s não pode ser criado!\n", caminho_svg);
        return;
    }
    struct No *atual = lista->cabeca;
    fprintf(arquivo, "<svg viewBox='%lf %lf %lf %lf'>\n",
            lista->exibicao.origem_x - SVG_MARGEM,
            lista->exibicao.origem_y - SVG_MARGEM,
            lista->exibicao.largura - abs(lista->exibicao.origem_x) + 2 * SVG_MARGEM,
            lista->exibicao.altura - abs(lista->exibicao.origem_y) + 2 * SVG_MARGEM
    );
    while(atual != NULL) {
        switch(atual->tipo) {
            case TipoCirculo:
                circulo_para_svg(arquivo, atual->figura.circ);
                break;
            case TipoRetangulo:
                retangulo_para_svg(arquivo, atual->figura.ret);
                break;
            case TipoTexto:
                texto_para_svg(arquivo, atual->figura.tex);
                break;
            case TipoLinha:
                linha_para_svg(arquivo, atual->figura.lin);
                break;
        }
        atual = atual->prox;
    }
    fprintf(arquivo, "</svg>\n");
    fclose(arquivo);
}

void destruir_lista(Lista *lista) {
    struct No *atual = lista->cabeca;
    struct No *prox;
    while(atual != NULL) {
        prox = atual->prox;
        free(atual);
        atual = prox;
    }

    lista->cabeca = NULL;
    lista->cauda = NULL;
    free(lista);
}