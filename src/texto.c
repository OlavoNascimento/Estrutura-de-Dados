#include "texto.h"

#include <stdio.h>
#include <string.h>

typedef struct {
    char id[100];
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    char conteudo[500];
} Texto;

// Cria e inicializa um struct Texto com os valores passados.
Texto texto_criar(char id[100], double x, double y, char cor_borda[20], char cor_preenchimento[20],
                  char conteudo[500]) {
    Texto tex;
    strcpy(tex.id, id);
    tex.x = x;
    tex.y = y;
    strcpy(tex.cor_borda, cor_borda);
    strcpy(tex.cor_preenchimento, cor_preenchimento);
    strcpy(tex.conteudo, conteudo);
    return tex;
}

// Criar um texto com base em informações de uma linha.
Texto texto_ler(const char *linha) {
    char id[100];
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    char conteudo[500];
    Texto tex;
    sscanf(linha, "%*c %s %lf %lf %s %s %[^\n*]s",
           id, x, y, cor_borda, cor_preenchimento, conteudo);
    return texto_criar(id, x, y, cor_borda, cor_preenchimento, conteudo);
}

// Escreve todos os dados de um texto em um arquivo.
void texto_escrever_informacoes(FILE *arquivo, Texto tex) {
    if (strlen(tex.id) > 0)
        fprintf(arquivo, "id: %s, ", tex.id);
    fprintf(arquivo,
            "x: %lf, y: %lf, corb: %s, corp: %s, texto: %s\n",
            tex.x, tex.y, tex.cor_borda, tex.cor_preenchimento, tex.conteudo);
}

// Escreve o código svg que representa um texto em um arquivo.
void texto_escrever_svg(FILE *arquivo, Texto tex) {
    fprintf(arquivo, "\t<text");
    if (strlen(tex.id) > 0)
        fprintf(arquivo, " id='%s'", tex.id);
    fprintf(arquivo,
            " x='%lf' y='%lf' stroke='%s' fill='%s'>%s</text>\n",
            tex.x, tex.y, tex.cor_borda, tex.cor_preenchimento, tex.conteudo);
}

const char *texto_obter_id(Texto tex) {
    return tex.id;
}

void texto_definir_id(Texto *tex, const char *id) {
    if (id == NULL) {
        fprintf(stderr, "ERRO: Não é possível definir null como id de um texto!");
        return;
    }
    strcpy(tex->id, id);
}

double texto_obter_x(Texto tex) {
    return tex.x;
}

void texto_definir_x(Texto *tex, double x) {
    tex->x = x;
}

double texto_obter_y(Texto tex) {
    return tex.y;
}

void texto_definir_y(Texto *tex, double y) {
    tex->y = y;
}

const char *texto_obter_cor_borda(Texto tex) {
    return tex.cor_borda;
}

void texto_definir_cor_borda(Texto *tex, const char *cor_borda) {
    if (cor_borda == NULL) {
        fprintf(stderr, "ERRO: Não é possível definir null como cor da borda de um texto!");
        return;
    }
    strcpy(tex->cor_borda, cor_borda);
}

const char *texto_obter_cor_preenchimento(Texto tex) {
    return tex.cor_preenchimento;
}

void texto_definir_cor_preenchimento(Texto *tex, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        fprintf(stderr, "ERRO: Não é possível definir null como cor de preenchimento de um texto!");
        return;
    }
    strcpy(tex->cor_preenchimento, cor_preenchimento);
}

const char *texto_obter_conteudo(Texto tex) {
    return tex.conteudo;
}

void texto_definir_conteudo(Texto *tex, const char *conteudo) {
    if (conteudo == NULL) {
        fprintf(stderr, "ERRO: Não é possível definir null como conteúdo de um texto!");
        return;
    }
    strcpy(tex->conteudo, conteudo);
}
