#include <stdlib.h>
#include <string.h>

#include <lista.h>
#include <figuras.h>

#define SVG_MARGEM 5

Lista* criar_lista() {
    Lista *lis = (Lista*) malloc(sizeof(Lista));
    lis->cabeca = NULL;
    lis->cauda= NULL;
    return lis;
}

void atualizar_exibicao_svg(Exibicao *exi, Figuras fig, TiposFigura tipo) {
    double novo_x, novo_y, nova_largura, nova_altura;
    switch(tipo) {
        case TipoCirculo:
            novo_x = fig.circ.x - fig.circ.raio;
            novo_y = fig.circ.y - fig.circ.raio;
            nova_largura = fig.circ.x + fig.circ.raio + abs(novo_x);
            nova_altura = fig.circ.y + fig.circ.raio + abs(novo_y);
            break;
        case TipoRetangulo:
            novo_x = fig.ret.x;
            novo_y = fig.ret.y;
            nova_largura = fig.ret.x + fig.ret.largura + abs(novo_x);
            nova_altura = fig.ret.y + fig.ret.altura + abs(novo_y);
            break;
        default:
            return;
            break;
    }

    exi->origem_x = min(exi->origem_x, novo_x);
    exi->origem_y = min(exi->origem_y, novo_y);
    exi->largura = max(exi->largura, nova_largura);
    exi->altura = max(exi->altura, nova_altura);
}

void inserir_lista(Lista *lista, Figuras fig, TiposFigura fig_tipo) {
    struct No *no = (struct No*) malloc(sizeof(struct No));
    no->figura = fig;
    no->tipo = fig_tipo;
    no->prox = NULL;
    atualizar_exibicao_svg(&lista->exibicao, fig, fig_tipo);

    // TODO Remover elementos duplicados
    // TODO Adicionar função comparar_nos
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

struct No* buscar_elemento_lista(Lista *lista, char *id_buscado) {
    struct No *atual = lista->cabeca;
    while(atual != NULL) {
        char *id_atual;
        switch(atual->tipo) {
            case TipoCirculo:
                id_atual = atual->figura.circ.id;
                break;
            case TipoRetangulo:
                id_atual = atual->figura.ret.id;
                break;
            case TipoTexto:
                id_atual = atual->figura.tex.id;
                break;
        }

        if(strcmp(id_atual, id_buscado) == 0)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

void lista_para_svg(Lista *lista, char *caminho_svg) {
    FILE *arquivo = fopen(caminho_svg, "w");
    if(arquivo == NULL) {
        fprintf(stderr, "Arquivo %s não pode ser criado!\n", caminho_svg);
        return;
    }
    struct No* atual = lista->cabeca;
    fprintf(arquivo, "<svg viewBox='%lf %lf %lf %lf'>\n",
            lista->exibicao.origem_x - SVG_MARGEM,
            lista->exibicao.origem_y - SVG_MARGEM,
            lista->exibicao.largura + 2*SVG_MARGEM,
            lista->exibicao.altura + 2*SVG_MARGEM
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