#include "texto.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logging.h"

typedef struct {
    char id[100];
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    char conteudo[500];
    bool centralizar;
} TextoImp;

// Cria e inicializa um struct Texto com os valores passados.
Texto texto_criar(const char id[100], double x, double y, const char cor_borda[20],
                  const char cor_preenchimento[20], const char conteudo[500], bool centralizar) {
    TextoImp *texImp = malloc(sizeof(TextoImp));
    strcpy(texImp->id, id);
    texImp->x = x;
    texImp->y = y;
    strcpy(texImp->cor_borda, cor_borda);
    strcpy(texImp->cor_preenchimento, cor_preenchimento);
    strcpy(texImp->conteudo, conteudo);
    texImp->centralizar = centralizar;
    return texImp;
}

// Cria um texto com base em informações de uma linha.
Texto texto_ler(const char *linha) {
    char id[100];
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    char conteudo[500];
    sscanf(linha, "%*s %s %lf %lf %s %s %[^\n*]s", id, &x, &y, cor_borda, cor_preenchimento,
           conteudo);
    return texto_criar(id, x, y, cor_borda, cor_preenchimento, conteudo, false);
}

// Escreve todos os dados de um texto em um arquivo.
void texto_escrever_informacoes(FILE *arquivo, Texto tex) {
    TextoImp *texImp = (TextoImp *) tex;
    if (strlen(texImp->id) > 0)
        fprintf(arquivo, "id: %s, ", texImp->id);
    fprintf(arquivo, "x: %lf, y: %lf, corb: %s, corp: %s, texto: %s\n", texImp->x, texImp->y,
            texImp->cor_borda, texImp->cor_preenchimento, texImp->conteudo);
}

// Escreve o código svg que representa um texto em um arquivo.
void texto_escrever_svg(FILE *arquivo, Texto tex) {
    TextoImp *texImp = (TextoImp *) tex;
    fprintf(arquivo, "\t<text");
    if (strlen(texImp->id) > 0)
        fprintf(arquivo, " id='%s'", texImp->id);
    if (texImp->centralizar)
        fprintf(arquivo, " text-anchor='middle'");
    fprintf(arquivo, " x='%lf' y='%lf' stroke='%s' fill='%s'>%s</text>\n", texImp->x, texImp->y,
            texImp->cor_borda, texImp->cor_preenchimento, texImp->conteudo);
}

const char *texto_obter_id(Texto tex) {
    TextoImp *texImp = (TextoImp *) tex;
    return texImp->id;
}

void texto_definir_id(Texto tex, const char *id) {
    if (id == NULL) {
        LOG_ERROR("Não é possível definir NULL como id de um texto!\n");
        return;
    }
    TextoImp *texImp = (TextoImp *) tex;
    strcpy(texImp->id, id);
}

double texto_obter_x(Texto tex) {
    TextoImp *texImp = (TextoImp *) tex;
    return texImp->x;
}

void texto_definir_x(Texto tex, double x) {
    TextoImp *texImp = (TextoImp *) tex;
    texImp->x = x;
}

double texto_obter_y(Texto tex) {
    TextoImp *texImp = (TextoImp *) tex;
    return texImp->y;
}

void texto_definir_y(Texto tex, double y) {
    TextoImp *texImp = (TextoImp *) tex;
    texImp->y = y;
}

const char *texto_obter_cor_borda(Texto tex) {
    TextoImp *texImp = (TextoImp *) tex;
    return texImp->cor_borda;
}

void texto_definir_cor_borda(Texto tex, const char *cor_borda) {
    if (cor_borda == NULL) {
        LOG_ERROR("Não é possível definir NULL como cor da borda de um texto!\n");
        return;
    }
    TextoImp *texImp = (TextoImp *) tex;
    strcpy(texImp->cor_borda, cor_borda);
}

const char *texto_obter_cor_preenchimento(Texto tex) {
    TextoImp *texImp = (TextoImp *) tex;
    return texImp->cor_preenchimento;
}

void texto_definir_cor_preenchimento(Texto tex, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        LOG_ERROR("Não é possível definir NULL como cor de preenchimento de um texto!\n");
        return;
    }
    TextoImp *texImp = (TextoImp *) tex;
    strcpy(texImp->cor_preenchimento, cor_preenchimento);
}

const char *texto_obter_conteudo(Texto tex) {
    TextoImp *texImp = (TextoImp *) tex;
    return texImp->conteudo;
}

void texto_definir_conteudo(Texto tex, const char *conteudo) {
    if (conteudo == NULL) {
        LOG_ERROR("Não é possível definir NULL como conteúdo de um texto!\n");
        return;
    }
    TextoImp *texImp = (TextoImp *) tex;
    strcpy(texImp->conteudo, conteudo);
}

void texto_destruir(Texto tex) {
    free(tex);
}
