#include "svg.h"

#include <float.h>
#include <stdio.h>
#include <stdlib.h>

#include "figuras.h"
#include "lista.h"
#include "matematica.h"
#include "texto.h"
#include "utils.h"

// Tamanho maxímo da linha de um arquivo svg
#define LINHA_MAX 300
// Margem entre o svg e a figuras mais próximas de suas bordas.
#define SVG_MARGEM 14
// Margem entra a figura e seu rótulo.
#define ROTULO_MARGEM 4

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
    Texto rotulo = texto_criar("\0", figura_obter_x_inicio(figura) - ROTULO_MARGEM,
                               figura_obter_y_inicio(figura) - ROTULO_MARGEM, "black", "black",
                               figura_obter_id(figura));
    return figura_criar(rotulo, TIPO_TEXTO);
}

// Transforma as figuras de uma lista em um código svg que as representam, salvando o resultado
// em um arquivo localizado no caminho específicado.
void svg_lista_para_svg(Lista lista, const char *caminho_svg) {
    if (caminho_svg == NULL) {
        fprintf(stderr, "ERRO: Caminho de nulo passado para lista para svg!\n");
        return;
    }

    char *caminho_tmp = alterar_sufixo(caminho_svg, 1, ".tmp");
    FILE *arquivo_tmp = fopen(caminho_tmp, "w");
    if (arquivo_tmp == NULL) {
        fprintf(stderr, "ERRO: Arquivo svg temporário %s não pode ser criado!\n", caminho_tmp);
        return;
    }

    // Mantem informações sobre as proporções da imagem svg.
    ExibicaoSVG exibicao = svg_criar_exibicao();

    // Escreve as informações em um arquivo temporário, já que o parâmetro viewbow precisa ser
    // substituido após a criação do arquivo.
    fprintf(arquivo_tmp, "<svg>\n");
    struct No *atual = lista_get_first(lista);
    while (atual != NULL) {
        Figura figura_atual = lista_get_figura(atual);
        figura_escrever_svg(arquivo_tmp, figura_atual);
        // Atualiza as proporções do svg caso necessário.
        svg_atualizar_exibicao(&exibicao, figura_atual);

        // Checa se a figura atual possui um id.
        if (figura_obter_id(figura_atual) != NULL) {
            // Adiciona um texto com o id no canto superior esquerdo da figura.
            Figura rotulo = svg_criar_rotulo(figura_atual);
            figura_escrever_svg(arquivo_tmp, rotulo);
            figura_destruir(rotulo);
        }

        atual = lista_get_next(lista, atual);
    }
    fclose(arquivo_tmp);
    arquivo_tmp = fopen(caminho_tmp, "r");
    if (arquivo_tmp == NULL) {
        fprintf(stderr, "ERRO: Arquivo svg temporário %s não pode ser lido!\n", caminho_tmp);
        return;
    }

    // Abre o arquivo final para escrita
    FILE *arquivo_svg = fopen(caminho_svg, "w");
    if (arquivo_svg == NULL) {
        fprintf(stderr, "ERRO: Arquivo svg %s não pode ser criado!\n", caminho_svg);
        return;
    }

    // Coordenada x da origem é a figura mais a esquerda.
    double svg_origem_x = exibicao.origem_x - SVG_MARGEM;
    // Coordenada y da origem é a figura mais acima.
    double svg_origem_y = exibicao.origem_y - SVG_MARGEM;
    // Coordenada largura é a figura mais a direita. A largura do svg deve ser alterada para
    // utilizar a nova origem como base.
    double svg_largura = exibicao.largura - exibicao.origem_x + 2 * SVG_MARGEM;
    // Coordenada altura é a figura mais a abaixo. A largura do svg deve ser alterada para
    // utilizar a nova origem como base.
    double svg_altura = exibicao.altura - exibicao.origem_y + 2 * SVG_MARGEM;

    // Utiliza o atributo viewbox para garantir que todas as figuras possam ser vistas no
    // arquivo svg.
    int count = 0;
    char linha[LINHA_MAX];
    while ((fgets(linha, LINHA_MAX, arquivo_tmp)) != NULL) {
        // Substitui a primeira linha, no caso a tag <svg> com uma nova tag <svg> que possui o
        // atributo viewbox definido.
        if (count == 0)
            fprintf(arquivo_svg, "<svg viewBox='%lf %lf %lf %lf'>\n", svg_origem_x, svg_origem_y,
                    svg_largura, svg_altura);
        else
            fprintf(arquivo_svg, "%s", linha);
        count++;
    }

    fclose(arquivo_tmp);
    // Deleta o arquivo temporário
    remove(caminho_tmp);
    free(caminho_tmp);

    fclose(arquivo_svg);
}