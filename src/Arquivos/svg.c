#include "svg.h"

#include <float.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "../Estruturas/quadtree.h"
#include "../Interfaces/estrutura_mapeavel.h"
#include "../Interfaces/figura.h"
#include "../Utils/logging.h"
#include "../Utils/matematica.h"

// Margem entre as bordas do svg e as figuras.
#define SVG_MARGEM 14

// Utilizado para representar as proporções de um arquivo svg.
typedef struct {
    double origem_x;
    double origem_y;
    double largura;
    double altura;
} ProporcoesSVG;

// Checa se é necessário atualizar os valores da struct ExibiçãoSVG para garantir que a figura
// passada a função possa ser vista no arquivo svg.
void svg_atualizar_proporcao(Figura figura, ProporcoesSVG *exi) {
    exi->origem_x = min(exi->origem_x, figura_obter_x_inicio(figura));
    exi->origem_y = min(exi->origem_y, figura_obter_y_inicio(figura));
    exi->largura = max(exi->largura, figura_obter_x_fim(figura));
    exi->altura = max(exi->altura, figura_obter_y_fim(figura));
}

// Retorna uma struct que representa as proporcoes de um arquivo svg. Para isso é preciso iterar por
// todas as figuras de uma estrutura, atualizando a struct conforme necessário.
ProporcoesSVG *svg_criar_proporcao(int num_estruturas, va_list estruturas) {
    va_list copia_estruturas;
    va_copy(copia_estruturas, estruturas);

    ProporcoesSVG *exi = malloc(sizeof *exi);
    if (exi == NULL) {
        LOG_ERRO("Falha ao alocar memória para proporções do svg!\n");
        return NULL;
    }
    exi->origem_x = DBL_MAX;
    exi->origem_y = DBL_MAX;
    exi->largura = DBL_MIN;
    exi->altura = DBL_MIN;

    // Encontra os valores necessários para que todas as figuras apareçam no arquivo svg.
    for (int i = 0; i < num_estruturas; i++) {
        void *estrutura = va_arg(copia_estruturas, void *);
        estrutura_map(estrutura, (EstruturaAplicarInfo *) svg_atualizar_proporcao, exi);
    }

    exi->origem_x -= SVG_MARGEM;
    exi->origem_y -= SVG_MARGEM;
    // A largura do svg deve ser alterada para utilizar a nova origem como base.
    exi->largura = exi->largura - exi->origem_x + 2 * SVG_MARGEM;
    // A altura do svg deve ser alterada para utilizar a nova origem como base.
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

// Escreve um arquivo svg utilizando as figuras armazenadas nas estruturas fornecidas.
void svg_escrever(const char *caminho_svg, int num_estruturas, ...) {
    if (caminho_svg == NULL) {
        LOG_AVISO("Caminho nulo passado para svg_escrever!\n");
        return;
    }

    FILE *arquivo_svg = fopen(caminho_svg, "w");
    if (arquivo_svg == NULL) {
        LOG_ERRO("Arquivo svg %s não pode ser criado para escrita!\n", caminho_svg);
        return;
    }

    va_list estruturas;
    va_start(estruturas, num_estruturas);

    // Calcula as proporções da imagem svg.
    ProporcoesSVG *proporcoes = svg_criar_proporcao(num_estruturas, estruturas);

    fprintf(arquivo_svg, "<svg viewBox='%lf %lf %lf %lf' xmlns='http://www.w3.org/2000/svg'>\n",
            proporcoes->origem_x, proporcoes->origem_y, proporcoes->largura, proporcoes->altura);

    escrever_definicoes_sombras(arquivo_svg);

    // Itera pelas estruturas e escreve suas figuras.
    for (int i = 0; i < num_estruturas; i++) {
        void *estrutura = va_arg(estruturas, void *);
        estrutura_map(estrutura, (EstruturaAplicarInfo *) figura_escrever_svg, arquivo_svg);
    }

    fprintf(arquivo_svg, "</svg>\n");

    free(proporcoes);
    va_end(estruturas);
    fclose(arquivo_svg);
}
