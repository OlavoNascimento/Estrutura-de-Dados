#include "svg.h"

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Estruturas/quadtree.h"
#include "../Interfaces/figura.h"
#include "../Objetos/Outros/texto.h"
#include "../Utils/caminhos.h"
#include "../Utils/logging.h"
#include "../Utils/matematica.h"

// Tamanho maxímo da linha de um arquivo svg
#define LINHA_MAX 300
// Margem entre o svg e as figuras mais próximas das bordas.
#define SVG_MARGEM 14

// Utilizado para representar as proporções de um arquivo svg.
typedef struct {
    double origem_x;
    double origem_y;
    double largura;
    double altura;
} ExibicaoSVG;

// Checa se é necessário atualizar os valores da struct ExibiçãoSVG para garantir que a figura
// passada a função possa ser vista no arquivo svg.
void svg_atualizar_exibicao(Figura figura, ExibicaoSVG *exi) {
    exi->origem_x = min(exi->origem_x, figura_obter_x_inicio(figura));
    exi->origem_y = min(exi->origem_y, figura_obter_y_inicio(figura));
    exi->largura = max(exi->largura, figura_obter_x_fim(figura));
    exi->altura = max(exi->altura, figura_obter_y_fim(figura));
}

// Retorna uma struct que representa as propriedades iniciais do arquivo svg.
ExibicaoSVG *svg_criar_exibicao(QuadTree formas, QuadTree quadras, QuadTree hidrantes,
                                QuadTree radios, QuadTree semaforos, QuadTree postos,
                                QuadTree casos) {
    ExibicaoSVG *exi = malloc(sizeof *exi);
    exi->origem_x = DBL_MAX;
    exi->origem_y = DBL_MAX;
    exi->largura = DBL_MIN;
    exi->altura = DBL_MIN;

    // Encontra os valores necessários para que todas as figuras apareçam no arquivo svg.
    percorreLarguraQt(formas, (void *) svg_atualizar_exibicao, exi);
    percorreLarguraQt(quadras, (void *) svg_atualizar_exibicao, exi);
    percorreLarguraQt(hidrantes, (void *) svg_atualizar_exibicao, exi);
    percorreLarguraQt(radios, (void *) svg_atualizar_exibicao, exi);
    percorreLarguraQt(semaforos, (void *) svg_atualizar_exibicao, exi);
    percorreLarguraQt(postos, (void *) svg_atualizar_exibicao, exi);
    percorreLarguraQt(casos, (void *) svg_atualizar_exibicao, exi);

    return exi;
}

void escrever_definicoes_sombras(FILE *arquivo) {
    fprintf(arquivo,
            "\t<defs>\n"
            "\t\t<filter id='sombra_10_500'>\n"
            "\t\t\t<feDropShadow dx='8' dy='8' flood-color='#FFFF00'/>\n"
            "\t\t</filter>\n"
            "\t\t<filter id='sombra_500_1500'>\n"
            "\t\t\t<feDropShadow dx='8' dy='8' flood-color='#FF9955'/>\n"
            "\t\t</filter>\n"
            "\t\t<filter id='sombra_1500_3000'>\n"
            "\t\t\t<feDropShadow dx='8' dy='8' flood-color='#FF0000'/>\n"
            "\t\t</filter>\n"
            "\t\t<filter id='sombra_3000_4500'>\n"
            "\t\t\t<feDropShadow dx='8' dy='8' flood-color='#FF00CC'/>\n"
            "\t\t</filter>\n"
            "\t\t<filter id='sombra_4500_6000'>\n"
            "\t\t\t<feDropShadow dx='8' dy='8' flood-color='#6600FF'/>\n"
            "\t\t</filter>\n"
            "\t\t<filter id='sombra_6000'>\n"
            "\t\t\t<feDropShadow dx='8' dy='8' flood-color='#A02C5A'/>\n"
            "\t\t</filter>\n"
            "\t</defs>\n");
}

// Transforma as figuras das listas em um código svg que as representam, salvando o resultado em um
// arquivo .svg localizado no caminho específicado.
void svg_quadtree_para_svg(const char *caminho_svg, QuadTree formas, QuadTree quadras,
                           QuadTree hidrantes, QuadTree radios, QuadTree semaforos, QuadTree postos,
                           QuadTree casos) {
    if (caminho_svg == NULL) {
        LOG_ERRO("Caminho nulo passado a lista para svg!\n");
        return;
    }

    // Calcula as proporções da imagem svg.
    ExibicaoSVG *exibicao =
        svg_criar_exibicao(formas, quadras, hidrantes, radios, semaforos, postos, casos);

    // Cria uma cópia do caminho do arquivo svg porem com o sufixo .tmp
    char *diretorio_saida = extrair_nome_diretorio(caminho_svg);

    FILE *arquivo_svg = fopen(caminho_svg, "w");
    if (arquivo_svg == NULL) {
        fprintf(stderr, "ERRO: Arquivo svg %s não pode ser criado para escrita!\n", caminho_svg);
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
    fprintf(arquivo_svg, "<svg viewBox='%lf %lf %lf %lf' xmlns='http://www.w3.org/2000/svg'>\n",
            svg_origem_x, svg_origem_y, svg_largura, svg_altura);

    escrever_definicoes_sombras(arquivo_svg);

    percorreLarguraQt(quadras, (void *) figura_escrever_svg, arquivo_svg);
    percorreLarguraQt(semaforos, (void *) figura_escrever_svg, arquivo_svg);
    percorreLarguraQt(radios, (void *) figura_escrever_svg, arquivo_svg);
    percorreLarguraQt(hidrantes, (void *) figura_escrever_svg, arquivo_svg);
    percorreLarguraQt(casos, (void *) figura_escrever_svg, arquivo_svg);
    percorreLarguraQt(postos, (void *) figura_escrever_svg, arquivo_svg);
    percorreLarguraQt(formas, (void *) figura_escrever_svg, arquivo_svg);
    fprintf(arquivo_svg, "</svg>\n");

    fclose(arquivo_svg);
    free(diretorio_saida);
    free(exibicao);
}
