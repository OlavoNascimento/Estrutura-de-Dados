#include <stdbool.h>
#include <string.h>

#include <figuras.h>
#include <retangulo.h>
#include <circulo.h>

#define MARGEM_CONTORNO 2
#define TRACEJADO_TAMANHO 2

char *fig_tipo_para_string(TiposFigura tipo) {
    char *valores[] = {
        "retângulo", "círculo", "texto", "linha"
    };
    return valores[tipo];
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
}

double max(double a, double b) {
    return a > b ? a : b;
}

double min(double a, double b) {
    return a < b ? a : b;
}

bool interseccao_retangulos(Retangulo ret1, Retangulo ret2) {
    if(ret1.x > ret2.x + ret2.largura || ret2.x > ret1.x + ret1.largura)
        return false;
    if(ret1.y + ret1.altura < ret2.y || ret2.y + ret2.altura < ret1.y)
        return false;
    return true;
}

bool interseccao_circulos(Circulo circ1, Circulo circ2) {
    double dist = (circ1.x - circ2.x) * (circ1.x - circ2.x) +
                  (circ1.y - circ2.y) * (circ1.y - circ2.y);
    double raios = (circ1.raio + circ2.raio) * (circ1.raio + circ2.raio);
    if(dist > raios)
        return false;
    return true;
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

void envolver_retangulos(Retangulo *contorno, Retangulo ret1, Retangulo ret2) {
    contorno->x = min(ret1.x, ret2.x);
    contorno->y = min(ret1.y, ret2.y);
    contorno->largura = max(ret1.x+ret1.largura, ret2.x+ret2.largura) - contorno->x;
    contorno->altura = max(ret1.y+ret1.altura, ret2.y+ret2.altura) - contorno->y;
}

void envolver_circulos(Retangulo *contorno, Circulo circ1, Circulo circ2) {
    contorno->x = min(circ1.x-circ1.raio, circ2.x-circ2.raio);
    contorno->y = min(circ1.y-circ1.raio, circ2.y-circ2.raio);
    contorno->largura = max(circ1.x+circ1.raio, circ2.x+circ2.raio) - contorno->x;
    contorno->altura = max(circ1.y+circ1.raio, circ2.y+circ2.raio) - contorno->y;
}

void envolver_circulo_retangulo(Retangulo *contorno, Circulo circ, Retangulo ret) {
    contorno->x = min(ret.x, circ.x-circ.raio);
    contorno->y = min(ret.y, circ.y-circ.raio);
    contorno->largura = max(ret.x+ret.largura, circ.x+circ.raio) - contorno->x;
    contorno->altura = max(ret.y+ret.altura, circ.y+circ.raio) - contorno->y;
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

bool ponto_interno_circulo(Circulo circ, double ponto_x, double ponto_y) {
    if(ponto_x <= circ.x - circ.raio || ponto_x >= circ.x + circ.raio)
        return false;
    if(ponto_y <= circ.y - circ.raio || ponto_y >= circ.y + circ.raio)
        return false;
    return true;
}

bool ponto_interno_retangulo(Retangulo ret, double ponto_x, double ponto_y) {
    if(ponto_x <= ret.x || ponto_x >= ret.x + ret.largura)
        return false;
    if(ponto_y <= ret.y || ponto_y >= ret.y + ret.altura)
        return false;
    return true;
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
