#include "animacao.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"
#include "../Formas/circulo.h"
#include "../Formas/ponto.h"

struct Animacao_s {
    FiguraInterface vtable;
    char id[100];
    double raio;
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    double espessura_borda;
    double opacidade;
    char cor_caminho[20];
    Ponto *pontos;
    int num_pontos;
    double margem_x;
    double margem_y;
};

const char *animacao_obter_tipo() {
    return "animação";
}

// Registra as funções do objeto animação na interface FiguraInterface.
static FiguraInterface animacao_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, animacao_obter_tipo);

    figura_registrar_escrever_informacoes(interface,
                                          (EscreverInformacoes *) circulo_escrever_informacoes);
    figura_registrar_escrever_svg(interface, (EscreverSvg *) animacao_escrever_svg);

    figura_registrar_obter_id(interface, (ObterId *) circulo_obter_id);

    figura_registrar_obter_x(interface, (ObterX *) circulo_obter_x);
    figura_registrar_obter_y(interface, (ObterY *) circulo_obter_y);

    figura_registrar_obter_x_inicio(interface, (ObterXInicio *) circulo_obter_x_inicio);
    figura_registrar_obter_y_inicio(interface, (ObterYInicio *) circulo_obter_y_inicio);

    figura_registrar_obter_x_fim(interface, (ObterXFim *) circulo_obter_x_fim);
    figura_registrar_obter_y_fim(interface, (ObterYFim *) circulo_obter_y_fim);

    figura_registrar_obter_x_centro(interface, (ObterXCentro *) circulo_obter_x);
    figura_registrar_obter_y_centro(interface, (ObterYCentro *) circulo_obter_y);

    figura_registrar_destruir(interface, (Destruir *) animacao_destruir);
    return interface;
}

// Cria e inicializa um Animacao com os valores passados.
Animacao animacao_criar(const char id[100], const char cor_borda[20],
                        const char cor_preenchimento[20], const char cor_caminho[20],
                        int num_pontos, Ponto *pontos) {
    if (id == NULL) {
        LOG_AVISO("Não é possível criar um círculo com id NULL!\n");
        return NULL;
    }
    if (cor_borda == NULL) {
        LOG_AVISO("Não é possível criar uma animação com cor de borda NULL!\n");
        return NULL;
    }
    if (cor_preenchimento == NULL) {
        LOG_AVISO("Não é possível criar uma animação com cor de preenchimento NULL!\n");
        return NULL;
    }
    if (cor_caminho == NULL) {
        LOG_AVISO("Não é possível criar uma animação com cor de caminho NULL!\n");
        return NULL;
    }
    if (num_pontos < 2 || pontos == NULL) {
        LOG_AVISO("Não é possível criar uma animação com menos que dois pontos!\n");
        return NULL;
    }
    Animacao animacao = malloc(sizeof *animacao);
    if (animacao == NULL) {
        LOG_ERRO("Falha ao alocar memória\n");
        return NULL;
    }
    strcpy(animacao->id, id);
    animacao->raio = 10;
    animacao->x = ponto_obter_x(pontos[0]);
    animacao->y = ponto_obter_y(pontos[0]);
    strcpy(animacao->cor_borda, cor_borda);
    strcpy(animacao->cor_preenchimento, cor_preenchimento);
    strcpy(animacao->cor_caminho, cor_caminho);
    animacao->espessura_borda = 1;
    animacao->opacidade = 1;
    animacao->num_pontos = num_pontos;
    animacao->pontos = pontos;
    animacao->margem_x = 0;
    animacao->margem_y = 0;

    animacao->vtable = animacao_criar_interface_figura();
    return animacao;
}

// Escreve o código svg que representa uma animação em um arquivo.
void animacao_escrever_svg(Animacao animacao, FILE *arquivo) {
    fprintf(arquivo, "\t<path d='M");
    for (int i = 0; i < animacao->num_pontos; i++) {
        fprintf(arquivo, "%lf,%lf", ponto_obter_x(animacao->pontos[i]) + animacao->margem_x,
                ponto_obter_y(animacao->pontos[i]) + animacao->margem_y);
        if (i < animacao->num_pontos - 1)
            fprintf(arquivo, " ");
    }
    fprintf(arquivo, "' stroke='%s' stroke-width='4' fill='none' id='%s'/>\n",
            animacao->cor_caminho, animacao->id);

    fprintf(arquivo,
            "\t<circle r='%lf' cx='' cy='' stroke='%s' fill='%s' stroke-width='%lfpx' "
            "opacity='%lf'>\n",
            animacao->raio, animacao->cor_borda, animacao->cor_preenchimento,
            animacao->espessura_borda, animacao->opacidade);

    fprintf(arquivo, "\t\t<animateMotion dur='6s' repeatCount='indefinite'>\n");
    fprintf(arquivo, "\t\t\t<mpath xlink:href='#%s'/>\n", animacao->id);
    fprintf(arquivo, "\t\t</animateMotion>\n");

    fprintf(arquivo, "\t</circle>\n");
}

void animacao_definir_margem_x(Animacao animacao, double margem_x) {
    animacao->margem_x = margem_x;
}

void animacao_definir_margem_y(Animacao animacao, double margem_y) {
    animacao->margem_y = margem_y;
}

// Libera a memória alocada por uma animação.
void animacao_destruir(Animacao animacao) {
    for (int i = 0; i < animacao->num_pontos; i++)
        ponto_destruir(animacao->pontos[i]);
    free(animacao->pontos);
    circulo_destruir((Circulo) animacao);
}
