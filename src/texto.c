#include <stdio.h>
#include <string.h>

#include <texto.h>

Texto criar_texto(char *linha) {
    Texto tex;
    sscanf(linha, "%*c %s %lf %lf %s %s %[^\n*]s",
        tex.id, &tex.x, &tex.y, tex.cor_borda, tex.cor_preenchimento, tex.texto);
    tex.tamanho = 8;
    return tex;
}

void escrever_informacoes_texto(FILE *arquivo, Texto tex) {
    if(strlen(tex.id) > 0)
        fprintf(arquivo, "id: %s, ", tex.id);
    fprintf(arquivo,
            "x: %lf, y: %lf, corb: %s, corp: %s, txto: %s\n",
            tex.x,
            tex.y,
            tex.cor_borda,
            tex.cor_preenchimento,
            tex.texto
    );
}

void texto_para_svg(FILE *arquivo, Texto tex) {
    fprintf(arquivo, "\t<text");
    if(strlen(tex.id) > 0)
        fprintf(arquivo, " id='%s'", tex.id);
    fprintf(arquivo,
            " x='%lf' y='%lf' stroke='%s' fill='%s' font-size='%d'>%s</text>\n",
            tex.x,
            tex.y,
            tex.cor_borda,
            tex.cor_preenchimento,
            tex.tamanho,
            tex.texto
    );
}