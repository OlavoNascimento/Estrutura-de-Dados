#include "lista.h"

#include <float.h>
#include <stdlib.h>
#include <string.h>

#include "figuras.h"

// Margem entre o svg e a figuras mais próximas de suas bordas.
#define SVG_MARGEM 14
// Margem entra a figura e seu rótulo.
#define ROTULO_MARGEM 4

// Retorna uma struct que representa as propriedades iniciais do arquivo svg.
ExibicaoSVG criar_exibicao_svg() {
    ExibicaoSVG exi = {
        .origem_x = DBL_MAX,
        .origem_y = DBL_MAX,
        .largura = 0,
        .altura = 0};
    return exi;
}

// Inicializa um struct lista e retorna seu endereço.
Lista *criar_lista() {
    Lista *lis = malloc(sizeof(Lista));
    lis->cabeca = NULL;
    lis->cauda = NULL;
    lis->exibicao = criar_exibicao_svg();
    return lis;
}

// Checa se é necessário atualizar os valores da struct ExibiçãoSVG para
// garantir que a figura passada a função possa ser vista no arquivo svg.
void atualizar_exibicao_svg(ExibicaoSVG *exi,
                            Figuras figura, TiposFigura tipo) {
    exi->origem_x = min(
        exi->origem_x, obter_x_inicio_figura(figura, tipo) - ROTULO_MARGEM);
    exi->origem_y = min(
        exi->origem_y, obter_y_inicio_figura(figura, tipo) - ROTULO_MARGEM);
    exi->largura = max(
        exi->largura, obter_x_fim_figura(figura, tipo) - ROTULO_MARGEM);
    exi->altura = max(
        exi->altura, obter_y_fim_figura(figura, tipo) - ROTULO_MARGEM);
}

// Adiciona um novo elemento a uma lista.
void inserir_lista(Lista *lista, Figuras figura, TiposFigura tipo) {
    struct No *novo = (struct No *)malloc(sizeof(struct No));
    novo->figura = figura;
    novo->tipo = tipo;
    novo->prox = NULL;
    // Muda as proporções do svg caso a nova figura esteja fora das bordas
    // atuais.
    atualizar_exibicao_svg(&lista->exibicao, figura, tipo);

    struct No *ultimo = lista->cauda;
    if (ultimo == NULL) {
        // Primeiro elemento a ser adicionado a lista
        lista->cabeca = novo;
    } else {
        // Elementos adidionados após o primeiro
        ultimo->prox = novo;
    }
    lista->cauda = novo;
}

// Busca um id passado como parâmetro dentro de uma lista e retorna o nó
// correspondente.
struct No *buscar_elemento_lista(Lista *lista, const char *id_buscado) {
    struct No *atual = lista->cabeca;
    while (atual != NULL) {
        const char *id_atual = obter_id_figura(&atual->figura, atual->tipo);

        if (id_atual != NULL && strcmp(id_atual, id_buscado) == 0)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

// Remove um nó que tenha id igual ao id passado como parâmetro para função de
// dentro de uma lista.
void remover_elemento_lista(Lista *lista, const char *id_buscado) {
    struct No *anterior;
    struct No *atual = lista->cabeca;
    while (atual != NULL) {
        const char *id_atual = obter_id_figura(&atual->figura, atual->tipo);

        if (strcmp(id_atual, id_buscado) == 0) {
            if (atual == lista->cabeca) {
                lista->cabeca = atual->prox;
            } else if (atual == lista->cauda) {
                lista->cauda = anterior;
                anterior->prox = NULL;
            } else {
                anterior->prox = atual->prox;
            }
            free(atual);
            atual = NULL;
        } else {
            anterior = atual;
            atual = atual->prox;
        }
    }
}

// Cria um texto igual ao id de uma figura.
Texto criar_rotulo(Figuras figura, TiposFigura tipo) {
    Texto rotulo = {
        .id = "\0",
        .cor_borda = "black",
        .cor_preenchimento = "black"};
    rotulo.x = obter_x_inicio_figura(figura, tipo) - ROTULO_MARGEM;
    rotulo.y = obter_y_inicio_figura(figura, tipo) - ROTULO_MARGEM;
    strcpy(rotulo.texto, obter_id_figura(&figura, tipo));
    return rotulo;
}

// Transforma as figuras de uma lista em um código svg que as representam,
// salvando o resultado em um arquivo localizado no caminho específicado.
void lista_para_svg(Lista *lista, const char *caminho_svg) {
    FILE *arquivo = fopen(caminho_svg, "w");
    if (arquivo == NULL) {
        fprintf(stderr, "ERRO: Arquivo %s não pode ser criado!\n", caminho_svg);
        return;
    }
    double svg_origem_x = lista->exibicao.origem_x - SVG_MARGEM;
    double svg_origem_y = lista->exibicao.origem_y - SVG_MARGEM;

    double svg_largura = lista->exibicao.largura + 2 * SVG_MARGEM;
    // Caso a origem x seja diferente de 0, a largura do svg deve ser alterada
    // para utilizar esta nova origem como base.
    if (svg_origem_x > 0)
        svg_largura -= lista->exibicao.origem_x;
    else
        svg_largura += abs(lista->exibicao.origem_x);

    double svg_altura = lista->exibicao.altura + 2 * SVG_MARGEM;
    // Caso a origem y seja diferente de 0, a altura do svg deve ser alterada
    // para utilizar esta nova origem como base.
    if (svg_origem_y > 0)
        svg_altura -= lista->exibicao.origem_y;
    else
        svg_altura += abs(lista->exibicao.origem_y);

    // Utiliza o atributo viewbox para garantir que todas as figuras possam ser
    // vistas no arquivo svg.
    fprintf(
        arquivo,
        "<svg viewBox='%lf %lf %lf %lf'>\n",
        svg_origem_x, svg_origem_y, svg_largura, svg_altura);
    struct No *atual = lista->cabeca;
    while (atual != NULL) {
        escrever_svg_figura(arquivo, atual->figura, atual->tipo);

        // Checa se a figura atual possui um id.
        if (obter_id_figura(&atual->figura, atual->tipo) != NULL) {
            Figuras rotulo;
            rotulo.tex = criar_rotulo(atual->figura, atual->tipo);
            escrever_svg_figura(arquivo, rotulo, TIPO_TEXTO);
        }
        atual = atual->prox;
    }
    fprintf(arquivo, "</svg>\n");

    fclose(arquivo);
}

// Libera a memória alocada por uma lista e seus elementos.
void destruir_lista(Lista *lista) {
    struct No *atual = lista->cabeca;
    struct No *prox;
    while (atual != NULL) {
        prox = atual->prox;
        free(atual);
        atual = prox;
    }

    lista->cabeca = NULL;
    lista->cauda = NULL;
    free(lista);
    lista = NULL;
}
