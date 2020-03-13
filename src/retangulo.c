#include <retangulo.h>

Retangulo criar_retangulo(char *linha) {
    Retangulo ret;
    sscanf(linha, "%*c %s %d %d %d %d %s %s",
        ret.id, &ret.largura, &ret.altura, &ret.x, &ret.y, ret.cor_borda, ret.cor_preenchimento);
    return ret;
}

void retangulo_para_svg(FILE *arquivo, Retangulo ret) {
    fprintf(arquivo,
           "    <rect id='%s' width='%d' height='%d' x='%d' y='%d' stroke='%s' fill='%s'/>\n",
            ret.id,
            ret.largura,
            ret.altura,
            ret.x,
            ret.y,
            ret.cor_borda,
            ret.cor_preenchimento
    );
}