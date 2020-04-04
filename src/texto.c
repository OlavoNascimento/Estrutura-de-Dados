#include <stdio.h>

#include <texto.h>

Texto criar_texto(char *linha) {
    Texto tex;
    sscanf(linha, "%*c %s %lf %lf %s %s %[^\n*]s",
        tex.id, &tex.x, &tex.y, tex.cor_borda, tex.cor_preenchimento, tex.texto);
    tex.tamanho = 8;
    return tex;
}

void texto_para_svg(FILE *arquivo, Texto tex) {
    fprintf(arquivo,
            "\t<text id='%s' x='%lf' y='%lf' stroke='%s' fill='%s' font-size='%d'>%s</text>\n",
            tex.id,
            tex.x,
            tex.y,
            tex.cor_borda,
            tex.cor_preenchimento,
            tex.tamanho,
            tex.texto
    );
}