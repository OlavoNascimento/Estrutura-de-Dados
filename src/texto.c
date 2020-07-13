#include <stdio.h>
#include <string.h>

#include <texto.h>

typedef struct {
    char id[100];
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    char texto[300];
} Texto;

// Criar um texto com base em informações de uma linha.
Texto texto_ler(const char *linha) {
    Texto tex;
    sscanf(linha, "%*c %s %lf %lf %s %s %[^\n*]s",
           tex.id, &tex.x, &tex.y, tex.cor_borda, tex.cor_preenchimento, tex.texto);
    return tex;
}

// Escreve todos os dados de um texto em um arquivo.
void texto_escrever_informacoes(FILE *arquivo, Texto tex) {
    if (strlen(tex.id) > 0)
        fprintf(arquivo, "id: %s, ", tex.id);
    fprintf(arquivo,
            "x: %lf, y: %lf, corb: %s, corp: %s, texto: %s\n",
            tex.x,
            tex.y,
            tex.cor_borda,
            tex.cor_preenchimento,
            tex.texto);
}

// Escreve o código svg que representa um texto em um arquivo.
void texto_escrever_svg(FILE *arquivo, Texto tex) {
    fprintf(arquivo, "\t<text");
    if (strlen(tex.id) > 0)
        fprintf(arquivo, " id='%s'", tex.id);
    fprintf(arquivo,
            " x='%lf' y='%lf' stroke='%s' fill='%s'>%s</text>\n",
            tex.x,
            tex.y,
            tex.cor_borda,
            tex.cor_preenchimento,
            tex.texto);
}

char texto_obter_id(Texto tex) {
    return tex.id;
}

double texto_obter_x(Texto tex) {
    return tex.x;
}

double texto_obter_y(Texto tex) {
    return tex.y;
}

char texto_obter_cor_borda(Texto tex) {
    return tex.cor_borda;
}

char texto_obter_cor_preenchimento(Texto tex) {
    return tex.cor_preenchimento;
}

char texto_obter(Texto tex) {
    return tex.texto;
}
