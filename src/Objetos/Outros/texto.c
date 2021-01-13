#include "texto.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"

#define ALTURA_LETRA 2
#define LARGURA_LETRA 4

typedef struct {
    FiguraInterface vtable;
    char id[100];
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    char conteudo[500];
    bool centralizar;
} TextoImp;

const char *texto_obter_tipo() {
    return "texto";
}

// Conecta as funções do objeto texto com as da interface figura.
static FiguraInterface texto_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_escrever_informacoes(interface, texto_escrever_informacoes);
    figura_registrar_escrever_svg(interface, texto_escrever_svg);

    figura_registrar_obter_tipo(interface, texto_obter_tipo);

    figura_registrar_obter_id(interface, texto_obter_id);

    figura_registrar_obter_x(interface, texto_obter_x);
    figura_registrar_obter_y(interface, texto_obter_y);

    figura_registrar_obter_x_inicio(interface, texto_obter_x);
    figura_registrar_obter_y_inicio(interface, texto_obter_y);

    figura_registrar_obter_x_fim(interface, texto_obter_x_fim);
    figura_registrar_obter_y_fim(interface, texto_obter_y_fim);

    figura_registrar_obter_x_centro(interface, texto_obter_x_centro);
    figura_registrar_obter_y_centro(interface, texto_obter_y_centro);

    figura_registrar_obter_cor_borda(interface, texto_obter_cor_borda);
    figura_registrar_definir_cor_borda(interface, texto_definir_cor_borda);

    figura_registrar_obter_cor_preenchimento(interface, texto_obter_cor_preenchimento);
    figura_registrar_definir_cor_preenchimento(interface, texto_definir_cor_preenchimento);

    figura_registrar_destruir(interface, texto_destruir);
    return interface;
}

// Cria e inicializa um struct Texto com os valores passados.
Texto texto_criar(const char id[100], double x, double y, const char cor_borda[20],
                  const char cor_preenchimento[20], const char conteudo[500], bool centralizar) {
    TextoImp *texImp = malloc(sizeof *texImp);
    strcpy(texImp->id, id);
    texImp->x = x;
    texImp->y = y;
    strcpy(texImp->cor_borda, cor_borda);
    strcpy(texImp->cor_preenchimento, cor_preenchimento);
    strcpy(texImp->conteudo, conteudo);
    texImp->centralizar = centralizar;

    texImp->vtable = texto_criar_interface_figura();
    return texImp;
}

// Cria um texto com base em informações de uma texto.
Texto texto_ler(const char *texto) {
    char id[100];
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    char conteudo[500];
    sscanf(texto, "%*s %s %lf %lf %s %s %[^\n*]s", id, &x, &y, cor_borda, cor_preenchimento,
           conteudo);
    return texto_criar(id, x, y, cor_borda, cor_preenchimento, conteudo, false);
}

// Escreve todos os dados de um texto em um arquivo.
void texto_escrever_informacoes(Texto texto, FILE *arquivo) {
    TextoImp *texImp = (TextoImp *) texto;
    fprintf(arquivo, "tipo: %s, ", figura_obter_tipo(texto));
    if (strlen(texImp->id) > 0)
        fprintf(arquivo, "id: %s, ", texImp->id);
    fprintf(arquivo, "x: %lf, y: %lf, corb: %s, corp: %s, texto: %s\n", texImp->x, texImp->y,
            texImp->cor_borda, texImp->cor_preenchimento, texImp->conteudo);
}

// Escreve o código svg que representa um texto em um arquivo.
void texto_escrever_svg(Texto texto, FILE *arquivo) {
    TextoImp *texImp = (TextoImp *) texto;
    fprintf(arquivo, "\t<text");
    if (strlen(texImp->id) > 0)
        fprintf(arquivo, " id='%s'", texImp->id);
    if (texImp->centralizar)
        fprintf(arquivo, " text-anchor='middle'");
    fprintf(arquivo, " x='%lf' y='%lf' stroke='%s' fill='%s'>%s</text>\n", texImp->x, texImp->y,
            texImp->cor_borda, texImp->cor_preenchimento, texImp->conteudo);
}

// Obtém o conteúdo de um texto.
const char *texto_obter_conteudo(Texto texto) {
    TextoImp *texImp = (TextoImp *) texto;
    return texImp->conteudo;
}

// Retorna o id de um texto.
const char *texto_obter_id(Texto texto) {
    TextoImp *texImp = (TextoImp *) texto;
    return texImp->id;
}

// Retorna a coordenada x de um texto.
double texto_obter_x(Texto texto) {
    TextoImp *texImp = (TextoImp *) texto;
    return texImp->x;
}

// Retorna a coordenada y de um texto.
double texto_obter_y(Texto texto) {
    TextoImp *texImp = (TextoImp *) texto;
    return texImp->y;
}

// Retorna um valor utilizando uma largura estimada.
double texto_obter_x_fim(Texto texto) {
    TextoImp *texImp = (TextoImp *) texto;
    return texImp->x + strlen(texImp->conteudo) * LARGURA_LETRA;
}

// Retorna um valor utilizando uma altura estimada.
double texto_obter_y_fim(Texto texto) {
    TextoImp *texImp = (TextoImp *) texto;
    return texImp->y + ALTURA_LETRA;
}

// Retorna a coordenada x do centro de um texto.
double texto_obter_x_centro(Texto texto) {
    return (texto_obter_x_fim(texto) - texto_obter_x(texto)) / 2;
}

// Retorna a coordenada y do centro de um texto.
double texto_obter_y_centro(Texto texto) {
    return (texto_obter_y_fim(texto) - texto_obter_y(texto)) / 2;
}

// Retorna a cor da borda de um texto.
const char *texto_obter_cor_borda(Texto texto) {
    TextoImp *texImp = (TextoImp *) texto;
    return texImp->cor_borda;
}

// Define a cor da borda de um texto.
void texto_definir_cor_borda(Texto texto, const char *cor_borda) {
    if (cor_borda == NULL) {
        LOG_ERRO("Não é possível definir NULL como cor da borda de um texto!\n");
        return;
    }
    TextoImp *texImp = (TextoImp *) texto;
    strcpy(texImp->cor_borda, cor_borda);
}

// Retorna a cor de preenchimento de um texto.
const char *texto_obter_cor_preenchimento(Texto texto) {
    TextoImp *texImp = (TextoImp *) texto;
    return texImp->cor_preenchimento;
}

// Define a cor de preenchimento de um texto.
void texto_definir_cor_preenchimento(Texto texto, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        LOG_ERRO("Não é possível definir NULL como cor de preenchimento de um texto!\n");
        return;
    }
    TextoImp *texImp = (TextoImp *) texto;
    strcpy(texImp->cor_preenchimento, cor_preenchimento);
}

// Libera a memória alocada por um texto.
void texto_destruir(Texto texto) {
    TextoImp *texImp = (TextoImp *) texto;
    free(texImp->vtable);
    free(texImp);
}
