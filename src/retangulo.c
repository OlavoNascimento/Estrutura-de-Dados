#include <retangulo.h>

Retangulo criar_retangulo(char *linha) {
    Retangulo ret;
    sscanf(linha, "%*c %s %lf %lf %lf %lf %s %s",
        ret.id, &ret.largura, &ret.altura, &ret.x, &ret.y, ret.cor_borda, ret.cor_preenchimento);
    ret.tracejado_espaco = 0;
    ret.tracejado_tamanho = 0;
    return ret;
}

void retangulo_para_svg(FILE *arquivo, Retangulo ret) {
    fprintf(arquivo,
           "\t<rect id='%s' width='%lf' height='%lf' x='%lf' y='%lf' stroke='%s' fill='%s'",
            ret.id,
            ret.largura,
            ret.altura,
            ret.x,
            ret.y,
            ret.cor_borda,
            ret.cor_preenchimento
    );
    if(ret.tracejado_tamanho != 0 || ret.tracejado_espaco != 0)
        fprintf(arquivo, " style='stroke-dasharray: %d %d'",
                ret.tracejado_tamanho,
                ret.tracejado_espaco
        );
    fprintf(arquivo, "/>\n");
}