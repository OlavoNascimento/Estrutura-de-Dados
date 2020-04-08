#include <string.h>
#include <stdbool.h>

#include <figuras.h>
#include <retangulo.h>
#include <circulo.h>

#define MARGEM_CONTORNO 2
#define TRACEJADO_TAMANHO 2
#define ESPACO_LETRAS_TEXTO 4

char *fig_tipo_para_string(TiposFigura tipo) {
    char *valores[] = {
        "retângulo", "círculo", "texto", "linha"
    };
    return valores[tipo];
}

void escrever_informacoes_figura(FILE *arquivo, Figuras figura, TiposFigura tipo) {
    switch(tipo) {
        case TipoCirculo:
            escrever_informacoes_circulo(arquivo, figura.circ);
            break;
        case TipoRetangulo:
            escrever_informacoes_retangulo(arquivo, figura.ret);
            break;
        case TipoTexto:
            escrever_informacoes_texto(arquivo, figura.tex);
            break;
    }
}

char *obter_id_figura(Figuras *figura, TiposFigura tipo) {
    switch(tipo) {
        case TipoCirculo:
            return figura->circ.id;
            break;
        case TipoRetangulo:
            return figura->ret.id;
            break;
        case TipoTexto:
            return figura->tex.id;
            break;
    }
    return NULL;
}

double obter_x_figura(Figuras figura, TiposFigura tipo) {
    switch(tipo) {
        case TipoCirculo:
            return figura.circ.x;
            break;
        case TipoRetangulo:
            return figura.ret.x;
            break;
        case TipoTexto:
            return figura.tex.x;
            break;
    }
    return 0;
}

double obter_y_figura(Figuras figura, TiposFigura tipo) {
    switch(tipo) {
        case TipoCirculo:
            return figura.circ.y;
            break;
        case TipoRetangulo:
            return figura.ret.y;
            break;
        case TipoTexto:
            return figura.tex.y;
            break;
    }
    return 0;
}

double max(double a, double b) {
    return a > b ? a : b;
}

double min(double a, double b) {
    return a < b ? a : b;
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
            nova_largura = fig.tex.x + strlen(fig.tex.texto) * ESPACO_LETRAS_TEXTO;
            nova_altura = fig.tex.y + ESPACO_LETRAS_TEXTO;
            break;
    }

    exi->origem_x = min(exi->origem_x, novo_x);
    exi->origem_y = min(exi->origem_y, novo_y);
    exi->largura = max(exi->largura, nova_largura);
    exi->altura = max(exi->altura, nova_altura);
}


bool interseccao_circulo_retangulo(Circulo circ, Retangulo ret) {
    double deltaX = circ.x - max(ret.x, min(circ.x, ret.x + ret.largura));
    double deltaY = circ.y - max(ret.y, min(circ.y, ret.y + ret.altura));
    return (deltaX * deltaX + deltaY * deltaY) <= (circ.raio * circ.raio);
}

bool interseccao_figuras(Figuras fig1, TiposFigura tipo1, Figuras fig2, TiposFigura tipo2) {
    bool intersectam = false;
    if(tipo1 == TipoRetangulo && tipo1 == tipo2)
        intersectam = interseccao_retangulos(fig1.ret, fig2.ret);
    else if(tipo1 == TipoCirculo && tipo1 == tipo2)
        intersectam = interseccao_circulos(fig1.circ, fig2.circ);
    else if(tipo1 == TipoCirculo && tipo2 == TipoRetangulo)
        intersectam = interseccao_circulo_retangulo(fig1.circ, fig2.ret);
    else if(tipo1 == TipoRetangulo && tipo2 == TipoCirculo)
        intersectam = interseccao_circulo_retangulo(fig2.circ, fig1.ret);

    return intersectam;
}

void envolver_circulo_retangulo(Retangulo *contorno, Circulo circ, Retangulo ret) {
    contorno->x = min(ret.x, circ.x-circ.raio);
    contorno->y = min(ret.y, circ.y-circ.raio);
    contorno->largura = max(ret.x+ret.largura, circ.x+circ.raio) - contorno->x;
    contorno->altura = max(ret.y+ret.altura, circ.y+circ.raio) - contorno->y;
}

void envolver_circulos(Retangulo *contorno, Circulo circ1, Circulo circ2) {
    contorno->x = min(circ1.x-circ1.raio, circ2.x-circ2.raio);
    contorno->y = min(circ1.y-circ1.raio, circ2.y-circ2.raio);
    contorno->largura = max(circ1.x+circ1.raio, circ2.x+circ2.raio) - contorno->x;
    contorno->altura = max(circ1.y+circ1.raio, circ2.y+circ2.raio) - contorno->y;
}

void envolver_retangulos(Retangulo *contorno, Retangulo ret1, Retangulo ret2) {
    contorno->x = min(ret1.x, ret2.x);
    contorno->y = min(ret1.y, ret2.y);
    contorno->largura = max(ret1.x+ret1.largura, ret2.x+ret2.largura) - contorno->x;
    contorno->altura = max(ret1.y+ret1.altura, ret2.y+ret2.altura) - contorno->y;
}

Retangulo envolver_figuras(bool intersectam, Figuras fig1, TiposFigura tipo1, Figuras fig2, TiposFigura tipo2) {
    Retangulo contorno = {
        .id = "\0",
        .cor_borda = "black",
        .cor_preenchimento = "none",
    };

    char *id1, *id2;
    if(tipo1 == TipoRetangulo && tipo1 == tipo2) {
        envolver_retangulos(&contorno, fig1.ret, fig2.ret);
        id1 = fig1.ret.id;
        id2 = fig2.ret.id;
    } else if(tipo1 == TipoCirculo && tipo1 == tipo2) {
        envolver_circulos(&contorno, fig1.circ, fig2.circ);
        id1 = fig1.circ.id;
        id2 = fig2.circ.id;
    } else if(tipo1 == TipoCirculo && tipo2 == TipoRetangulo) {
        envolver_circulo_retangulo(&contorno, fig1.circ, fig2.ret);
        id1 = fig1.circ.id;
        id2 = fig2.ret.id;
    } else if(tipo1 == TipoRetangulo && tipo2 == TipoCirculo) {
        envolver_circulo_retangulo(&contorno, fig2.circ, fig1.ret);
        id1 = fig1.ret.id;
        id2 = fig2.circ.id;
    }

    if(!intersectam) {
        contorno.tracejado_tamanho = TRACEJADO_TAMANHO;
        contorno.tracejado_espaco = TRACEJADO_TAMANHO;
    }
    contorno.x -= MARGEM_CONTORNO;
    contorno.y -= MARGEM_CONTORNO;
    contorno.largura += 2 * MARGEM_CONTORNO;
    contorno.altura += 2 * MARGEM_CONTORNO;
    return contorno;
}

bool ponto_interno_figura(Figuras figura, TiposFigura tipo, double ponto_x, double ponto_y) {
    bool interno = false;
    switch(tipo) {
        case TipoCirculo:
            interno = ponto_interno_circulo(figura.circ, ponto_x, ponto_y);
            break;
        case TipoRetangulo:
            interno = ponto_interno_retangulo(figura.ret, ponto_x, ponto_y);
            break;
    }
    return interno;
}

Linha ligar_ponto_figura(Circulo ponto, Figuras figura, TiposFigura tipo) {
    double fig_centro_x = 0, fig_centro_y = 0;
    switch(tipo) {
        case TipoCirculo:
            fig_centro_x = figura.circ.x;
            fig_centro_y = figura.circ.y;
            break;
        case TipoRetangulo:
            fig_centro_x = figura.ret.x + figura.ret.largura/2;
            fig_centro_y = figura.ret.y + figura.ret.altura/2;
            break;
    }
    Linha ligacao = {
        .x1 = ponto.x,
        .y1 = ponto.y,
        .x2 = fig_centro_x,
        .y2 = fig_centro_y,
    };
    strcpy(ligacao.cor_borda, ponto.cor_borda);
    strcpy(ligacao.cor_preenchimento, ponto.cor_preenchimento);
    return ligacao;
}

void alterar_cor_figura(Figuras *figura, TiposFigura tipo, char *cor_borda, char *cor_preenchimento) {
    switch(tipo) {
        case TipoCirculo:
            strcpy(figura->circ.cor_borda, cor_borda);
            strcpy(figura->circ.cor_preenchimento, cor_preenchimento);
            break;
        case TipoRetangulo:
            strcpy(figura->ret.cor_borda, cor_borda);
            strcpy(figura->ret.cor_preenchimento, cor_preenchimento);
            break;
        case TipoTexto:
            strcpy(figura->tex.cor_borda, cor_borda);
            strcpy(figura->tex.cor_preenchimento, cor_preenchimento);
            break;
    }
}
