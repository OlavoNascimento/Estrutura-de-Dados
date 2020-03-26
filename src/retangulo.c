#include <retangulo.h>

Retangulo criar_retangulo(char *linha) {
    Retangulo ret;
    sscanf(linha, "%*c %s %lf %lf %lf %lf %s %s",
        ret.id, &ret.largura, &ret.altura, &ret.x, &ret.y, ret.cor_borda, ret.cor_preenchimento);
    return ret;
}

void retangulo_para_svg(FILE *arquivo, Retangulo ret) {
    fprintf(arquivo,
           "    <rect id='%s' width='%ld' height='%lf' x='%lf' y='%lf' stroke='%s' fill='%s'/>\n",
            ret.id,
            ret.largura,
            ret.altura,
            ret.x,
            ret.y,
            ret.cor_borda,
            ret.cor_preenchimento
    );
}