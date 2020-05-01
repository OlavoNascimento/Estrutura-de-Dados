#include <stdio.h>
#include <string.h>

#include <texto.h>

// Criar um texto com base em informações de uma linha.
Texto ler_texto(const char *linha) {
    Texto tex;
    sscanf(linha, "%*c %s %lf %lf %s %s %[^\n*]s",
        tex.id, &tex.x, &tex.y, tex.cor_borda, tex.cor_preenchimento, tex.texto);
    return tex;
}

// Escreve todos os dados de um texto em um arquivo.
void escrever_informacoes_texto(FILE *arquivo, Texto tex) {
    if(strlen(tex.id) > 0)
        fprintf(arquivo, "id: %s, ", tex.id);
    fprintf(arquivo,
            "x: %lf, y: %lf, corb: %s, corp: %s, texto: %s\n",
            tex.x,
            tex.y,
            tex.cor_borda,
            tex.cor_preenchimento,
            tex.texto
    );
}

// Escreve o código svg que representa um texto em um arquivo.
void escrever_svg_texto(FILE *arquivo, Texto tex) {
    fprintf(arquivo, "\t<text");
    if(strlen(tex.id) > 0)
        fprintf(arquivo, " id='%s'", tex.id);
    fprintf(arquivo,
            " x='%lf' y='%lf' stroke='%s' fill='%s'>%s</text>\n",
            tex.x,
            tex.y,
            tex.cor_borda,
            tex.cor_preenchimento,
            tex.texto
    );
}
