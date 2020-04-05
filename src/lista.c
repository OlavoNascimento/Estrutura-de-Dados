#include <stdlib.h>
#include <string.h>

#include <lista.h>
#include <figuras.h>

#define SVG_MARGEM 5
#define MARGEM_TEXTO_LETRAS 4

Lista* criar_lista() {
    Lista *lis = (Lista*) malloc(sizeof(Lista));
    lis->cabeca = NULL;
    lis->cauda= NULL;
    lis->exibicao.origem_x = 0;
    lis->exibicao.origem_y = 0;
    lis->exibicao.largura = 0;
    lis->exibicao.altura = 0;
    return lis;
}

void atualizar_exibicao_svg(Exibicao *exi, Figuras fig, TiposFigura tipo) {
    double novo_x = 0, novo_y = 0, nova_largura = 0, nova_altura = 0;
    switch(tipo) {
        case TipoCirculo:
            novo_x = fig.circ.x - fig.circ.raio;
            novo_y = fig.circ.y - fig.circ.raio;
            nova_largura = fig.circ.x + fig.circ.raio;
            nova_altura = fig.circ.y + fig.circ.raio;
            break;
        case TipoRetangulo:
            novo_x = fig.ret.x;
            novo_y = fig.ret.y;
            nova_largura = fig.ret.x + fig.ret.largura;
            nova_altura = fig.ret.y + fig.ret.altura;
            break;
        case TipoTexto:
            novo_x = fig.tex.x;
            novo_y = fig.tex.y;
            nova_largura = fig.tex.x + strlen(fig.tex.texto) * MARGEM_TEXTO_LETRAS;
            nova_altura = fig.tex.y + MARGEM_TEXTO_LETRAS;
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

struct No* buscar_elemento_id_lista(Lista *lista, char *id_buscado) {
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
            lista->exibicao.largura + abs(lista->exibicao.origem_x) + 2 * SVG_MARGEM,
            lista->exibicao.altura + abs(lista->exibicao.origem_y) + 2 * SVG_MARGEM
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