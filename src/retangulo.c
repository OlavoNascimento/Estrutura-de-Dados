#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <retangulo.h>

Retangulo criar_retangulo(char *linha) {
    Retangulo ret;
    sscanf(linha, "%*c %s %lf %lf %lf %lf %s %s",
        ret.id, &ret.largura, &ret.altura, &ret.x, &ret.y, ret.cor_borda, ret.cor_preenchimento);
    ret.tracejado_espaco = 0;
    ret.tracejado_tamanho = 0;
    return ret;
}

void escrever_informacoes_retangulo(FILE *arquivo, Retangulo ret) {
    if(strlen(ret.id) > 0)
        fprintf(arquivo, "%s", ret.id);
    fprintf(arquivo,
           " %lf %lf %lf %lf %s %s",
            ret.largura,
            ret.altura,
            ret.x,
            ret.y,
            ret.cor_borda,
            ret.cor_preenchimento
    );
    if(ret.tracejado_tamanho != 0 || ret.tracejado_espaco != 0)
        fprintf(arquivo,
                " %d %d",
                ret.tracejado_tamanho,
                ret.tracejado_espaco
        );
    fprintf(arquivo, "\n");
}

void retangulo_para_svg(FILE *arquivo, Retangulo ret) {
    fprintf(arquivo, "\t<rect");
    if(strlen(ret.id) > 0)
        fprintf(arquivo, " id='%s'", ret.id);
    fprintf(arquivo,
           " width='%lf' height='%lf' x='%lf' y='%lf' stroke='%s' fill='%s'",
            ret.largura,
            ret.altura,
            ret.x,
            ret.y,
            ret.cor_borda,
            ret.cor_preenchimento
    );
    if(ret.tracejado_tamanho != 0 || ret.tracejado_espaco != 0)
        fprintf(arquivo,
                " style='stroke-dasharray: %d %d'",
                ret.tracejado_tamanho,
                ret.tracejado_espaco
        );
    fprintf(arquivo, "/>\n");
}

bool interseccao_retangulos(Retangulo ret1, Retangulo ret2) {
    if(ret1.x > ret2.x + ret2.largura || ret2.x > ret1.x + ret1.largura)
        return false;
    if(ret1.y + ret1.altura < ret2.y || ret2.y + ret2.altura < ret1.y)
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
