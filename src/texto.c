#include <stdio.h>

#include <texto.h>

Texto criar_texto(char *linha) {
    Texto tex;
    sscanf(linha, "%*c %s %d %d %s %s %[^\n*]s",
        tex.id, &tex.x, &tex.y, tex.cor_borda, tex.cor_preenchimento, tex.texto);
    return tex;
}

void texto_para_svg(FILE *arquivo, Texto tex) {
    fprintf(arquivo,
            "   <text id='%s' x='%d' y='%d' stroke='%s' fill='%s'>%s</text>\n",
            tex.id,
            tex.x,
            tex.y,
            tex.cor_borda,
            tex.cor_preenchimento,
            tex.texto
    );
}