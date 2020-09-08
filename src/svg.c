#include "svg.h"

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "figuras.h"
#include "lista.h"
#include "matematica.h"
#include "texto.h"
#include "utils.h"

// Tamanho maxímo da linha de um arquivo svg
#define LINHA_MAX 300
// Margem entra a figura e seu rótulo.
#define ROTULO_MARGEM 4
// Margem entre o svg e as figuras mais próximas das bordas.
#define SVG_MARGEM 14

// Utilizado para representar as proporções de um arquivo svg.
typedef struct {
    double origem_x;
    double origem_y;
    double largura;
    double altura;
} ExibicaoSVG;

// Retorna uma struct que representa as propriedades iniciais do arquivo svg.
ExibicaoSVG svg_criar_exibicao() {
    ExibicaoSVG exi = {.origem_x = DBL_MAX, .origem_y = DBL_MAX, .largura = 0, .altura = 0};
    return exi;
}

// Checa se é necessário atualizar os valores da struct ExibiçãoSVG para garantir que a figura
// passada a função possa ser vista no arquivo svg.
void svg_atualizar_exibicao(ExibicaoSVG *exi, Figura figura) {
    exi->origem_x = min(exi->origem_x, figura_obter_x_inicio(figura) - ROTULO_MARGEM);
    exi->origem_y = min(exi->origem_y, figura_obter_y_inicio(figura) - ROTULO_MARGEM);
    exi->largura = max(exi->largura, figura_obter_x_fim(figura) - ROTULO_MARGEM);
    exi->altura = max(exi->altura, figura_obter_y_fim(figura) - ROTULO_MARGEM);
}

// Cria um texto igual ao id de uma figura.
Figura svg_criar_rotulo(Figura figura) {
    Texto rotulo = texto_criar("", figura_obter_x_inicio(figura) - ROTULO_MARGEM,
                               figura_obter_y_inicio(figura) - ROTULO_MARGEM, "black", "black",
                               figura_obter_id(figura));
    return figura_criar(rotulo, TIPO_TEXTO);
}

// Escreve uma lista genérica em um arquivo.
void escrever_lista(Lista *lista, FILE *arquivo_tmp, ExibicaoSVG *exibicao) {
    struct No *atual = lista_get_first(lista);
    while (atual != NULL) {
        Figura figura_atual = lista_get_figura(atual);
        figura_escrever_svg(arquivo_tmp, figura_atual);
        // Atualiza as proporções do svg caso necessário.
        svg_atualizar_exibicao(exibicao, figura_atual);

        const char *id_figura = figura_obter_id(figura_atual);
        TiposFigura tipo_figura = figura_obter_tipo(figura_atual);
        // Adiciona um rótulo para todos os círculos e retângulos que tem id.
        if ((tipo_figura == TIPO_CIRCULO || tipo_figura == TIPO_RETANGULO) &&
            strcmp(id_figura, "") != 0) {
            // Adiciona um texto com o id no canto superior esquerdo da figura.
            Figura rotulo = svg_criar_rotulo(figura_atual);
            figura_escrever_svg(arquivo_tmp, rotulo);
            figura_destruir(rotulo);
        }

        atual = lista_get_next(lista, atual);
    }
}

// Escreve as listas passadas para um arquivo svg temporário.
void escrever_svg_temporario(const char *caminho_svg_tmp, ExibicaoSVG *exibicao, Lista lista_formas,
                             Lista lista_quadras, Lista lista_hidrantes, Lista lista_radios,
                             Lista lista_semaforos) {
    FILE *arquivo_tmp = fopen(caminho_svg_tmp, "w");
    if (arquivo_tmp == NULL) {
        fprintf(stderr, "ERRO: Arquivo svg temporário %s não pode ser criado para escrita!\n",
                caminho_svg_tmp);
        return;
    }
    // Escreve as informações em um arquivo temporário, já que o parâmetro viewbow precisa ser
    // substituido após a criação do arquivo.
    fprintf(arquivo_tmp, "<svg>\n");

    // Escreve todas as listas no arquivo temporário
    escrever_lista(lista_formas, arquivo_tmp, exibicao);
    escrever_lista(lista_quadras, arquivo_tmp, exibicao);
    escrever_lista(lista_hidrantes, arquivo_tmp, exibicao);
    escrever_lista(lista_radios, arquivo_tmp, exibicao);
    escrever_lista(lista_semaforos, arquivo_tmp, exibicao);

    fprintf(arquivo_tmp, "</svg>\n");
    fclose(arquivo_tmp);
}

// Copia o arquivo temporário com exceção de sua primeira linha, a qual é substituida por uma tag
// <svg> com o atributo viewbox definido.
void escrever_svg_com_viewbox(const char *caminho_svg_final, const char *caminho_svg_tmp,
                              ExibicaoSVG *exibicao) {
    // Abre o arquivo final para escrita
    FILE *arquivo_svg = fopen(caminho_svg_final, "w");
    if (arquivo_svg == NULL) {
        fprintf(stderr, "ERRO: Arquivo svg %s não pode ser criado!\n", caminho_svg_final);
        return;
    }
    // Abre o arquivo temporario para leitura
    FILE *arquivo_tmp = fopen(caminho_svg_tmp, "r");
    if (arquivo_tmp == NULL) {
        fprintf(stderr, "ERRO: Arquivo svg temporário %s não pode ser lido!\n", caminho_svg_tmp);
        return;
    }

    // Coordenada x da origem é a figura mais a esquerda.
    double svg_origem_x = exibicao->origem_x - SVG_MARGEM;
    // Coordenada y da origem é a figura mais acima.
    double svg_origem_y = exibicao->origem_y - SVG_MARGEM;
    // Coordenada largura é a figura mais a direita. A largura do svg deve ser alterada para
    // utilizar a nova origem como base.
    double svg_largura = exibicao->largura - exibicao->origem_x + 2 * SVG_MARGEM;
    // Coordenada altura é a figura mais a abaixo. A largura do svg deve ser alterada para utilizar
    // a nova origem como base.
    double svg_altura = exibicao->altura - exibicao->origem_y + 2 * SVG_MARGEM;

    // Utiliza o atributo viewbox para garantir que todas as figuras possam ser vistas no arquivo
    // svg.
    int numero_linha = 0;
    char linha[LINHA_MAX];
    while ((fgets(linha, LINHA_MAX, arquivo_tmp)) != NULL) {
        // Substitui a primeira linha, no caso a tag <svg> com uma nova tag <svg> que possui o
        // atributo viewbox definido.
        if (numero_linha == 0)
            fprintf(arquivo_svg, "<svg viewBox='%lf %lf %lf %lf'>\n", svg_origem_x, svg_origem_y,
                    svg_largura, svg_altura);
        else
            fprintf(arquivo_svg, "%s", linha);
        numero_linha++;
    }

    fclose(arquivo_tmp);
    fclose(arquivo_svg);
}

// Transforma as figuras das listas em um código svg que as representam, salvando o resultado em um
// arquivo .svg localizado no caminho específicado.
void svg_lista_para_svg(const char *caminho_svg, Lista lista_formas, Lista lista_quadras,
                        Lista lista_hidrantes, Lista lista_radios, Lista lista_semaforos) {
    if (caminho_svg == NULL) {
        fprintf(stderr, "ERRO: Caminho nulo passado a lista para svg!\n");
        return;
    }

    // Mantem informações sobre as proporções da imagem svg.
    ExibicaoSVG exibicao = svg_criar_exibicao();

    // Cria uma cópia do caminho do arquivo svg porem com o sufixo .tmp
    char *caminho_tmp = alterar_sufixo(caminho_svg, 1, ".tmp");
    // Svg temporário criado para anotar as proporções necessárias pelo svg final.
    escrever_svg_temporario(caminho_tmp, &exibicao, lista_formas, lista_quadras, lista_hidrantes,
                            lista_radios, lista_semaforos);

    // Svg final com a propriedade viewbox definida.
    escrever_svg_com_viewbox(caminho_svg, caminho_tmp, &exibicao);

    // Deleta o arquivo temporário
    remove(caminho_tmp);
    free(caminho_tmp);
}
