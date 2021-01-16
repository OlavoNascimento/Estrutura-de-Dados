#include "svg.h"

#include <float.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Estruturas/quadtree.h"
#include "../Interfaces/figura.h"
#include "../Objetos/Outros/texto.h"
#include "../Utils/caminhos.h"
#include "../Utils/logging.h"
#include "../Utils/matematica.h"

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
ExibicaoSVG *svg_criar_exibicao(int num_quadtrees, va_list quadtrees) {
    va_list qts;
    va_copy(qts, quadtrees);

    ExibicaoSVG *exi = malloc(sizeof *exi);
    exi->origem_x = DBL_MAX;
    exi->origem_y = DBL_MAX;
    exi->largura = DBL_MIN;
    exi->altura = DBL_MIN;

    // Encontra os valores necessários para que todas as figuras apareçam no arquivo svg.
    for (int i = 0; i < num_quadtrees; i++) {
        QuadTree qt = va_arg(qts, QuadTree);
        percorreLarguraQt(qt, (void *) svg_atualizar_exibicao, exi);
    }

    exi->origem_x -= SVG_MARGEM;
    exi->origem_y -= SVG_MARGEM;
    // A largura do svg deve ser alterada para utilizar a nova origem como base.
    exi->largura = exi->largura - exi->origem_x + 2 * SVG_MARGEM;
    // A largura do svg deve ser alterada para utilizar a nova origem como base.
    exi->altura = exi->altura - exi->origem_y + 2 * SVG_MARGEM;

    return exi;
}

// Adiciona ao arquivo svg definições de filtros que podem ser utilizadas para incluir sombras de
// diferentes cores as figuras.
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

// Transforma as figuras das quadtrees em um código svg que as representam, salvando o resultado em
// um arquivo svg localizado no caminho específicado.
void svg_quadtrees_para_svg(const char *caminho_svg, int num_quadtrees, ...) {
    if (caminho_svg == NULL) {
        LOG_ERRO("Caminho nulo passado a lista para svg!\n");
        return;
    }
    va_list quadtrees;
    va_start(quadtrees, num_quadtrees);

    // Calcula as proporções da imagem svg.
    ExibicaoSVG *exibicao = svg_criar_exibicao(num_quadtrees, quadtrees);

    FILE *arquivo_svg = fopen(caminho_svg, "w");
    if (arquivo_svg == NULL) {
        fprintf(stderr, "ERRO: Arquivo svg %s não pode ser criado para escrita!\n", caminho_svg);
        return;
    }

    fprintf(arquivo_svg, "<svg viewBox='%lf %lf %lf %lf' xmlns='http://www.w3.org/2000/svg'>\n",
            exibicao->origem_x, exibicao->origem_y, exibicao->largura, exibicao->altura);

    escrever_definicoes_sombras(arquivo_svg);

    // Itera pelas quadtrees e escreve suas figuras.
    for (int i = 0; i < num_quadtrees; i++) {
        // Recebe a quadtree atual.
        QuadTree qt = va_arg(quadtrees, QuadTree);
        percorreLarguraQt(qt, (void *) figura_escrever_svg, arquivo_svg);
    }
    va_end(quadtrees);

    fprintf(arquivo_svg, "</svg>\n");

    fclose(arquivo_svg);
    free(exibicao);
}
