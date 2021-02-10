#include "texto.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Interfaces/figura.h"
#include "../../Utils/logging.h"

#define ALTURA_LETRA 2
#define LARGURA_LETRA 4

struct Texto_s {
    FiguraInterface vtable;
    char id[100];
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    char conteudo[1024];
    bool centralizar;
};

const char *texto_obter_tipo() {
    return "texto";
}

// Conecta as funções do objeto texto com as da interface figura.
static FiguraInterface texto_criar_interface_figura() {
    FiguraInterface interface = figura_interface_criar();
    figura_registrar_obter_tipo(interface, (void *) texto_obter_tipo);

    figura_registrar_escrever_informacoes(interface, (void *) texto_escrever_informacoes);
    figura_registrar_escrever_svg(interface, (void *) texto_escrever_svg);

    figura_registrar_obter_id(interface, (void *) texto_obter_id);

    figura_registrar_obter_x(interface, (void *) texto_obter_x);
    figura_registrar_obter_y(interface, (void *) texto_obter_y);

    figura_registrar_obter_x_inicio(interface, (void *) texto_obter_x);
    figura_registrar_obter_y_inicio(interface, (void *) texto_obter_y);

    figura_registrar_obter_x_fim(interface, (void *) texto_obter_x_fim);
    figura_registrar_obter_y_fim(interface, (void *) texto_obter_y_fim);

    figura_registrar_obter_x_centro(interface, (void *) texto_obter_x_centro);
    figura_registrar_obter_y_centro(interface, (void *) texto_obter_y_centro);

    figura_registrar_obter_cor_borda(interface, (void *) texto_obter_cor_borda);
    figura_registrar_definir_cor_borda(interface, (void *) texto_definir_cor_borda);

    figura_registrar_obter_cor_preenchimento(interface, (void *) texto_obter_cor_preenchimento);
    figura_registrar_definir_cor_preenchimento(interface, (void *) texto_definir_cor_preenchimento);

    figura_registrar_destruir(interface, (void *) texto_destruir);
    return interface;
}

// Cria e inicializa um struct Texto com os valores passados.
Texto texto_criar(const char id[100], double x, double y, const char cor_borda[20],
                  const char cor_preenchimento[20], const char conteudo[1024], bool centralizar) {
    Texto texto = malloc(sizeof *texto);
    if (texto == NULL) {
        LOG_ERRO("Falha ao alocar memória\n");
        return NULL;
    }
    strcpy(texto->id, id);
    texto->x = x;
    texto->y = y;
    strcpy(texto->cor_borda, cor_borda);
    strcpy(texto->cor_preenchimento, cor_preenchimento);
    strcpy(texto->conteudo, conteudo);
    texto->centralizar = centralizar;

    texto->vtable = texto_criar_interface_figura();
    return texto;
}

// Cria um texto com base em informações de uma linha.
Texto texto_ler(const char *linha) {
    char id[100];
    double x;
    double y;
    char cor_borda[20];
    char cor_preenchimento[20];
    char conteudo[1024];
    sscanf(linha, "%*s %s %lf %lf %s %s %[^\n*]s", id, &x, &y, cor_borda, cor_preenchimento,
           conteudo);
    return texto_criar(id, x, y, cor_borda, cor_preenchimento, conteudo, false);
}

// Escreve todos os dados de um texto em um arquivo.
void texto_escrever_informacoes(Texto texto, FILE *arquivo) {
    fprintf(arquivo, "tipo: %s, ", figura_obter_tipo(texto));
    if (strlen(texto->id) > 0)
        fprintf(arquivo, "id: %s, ", texto->id);
    fprintf(arquivo, "x: %lf, y: %lf, corb: %s, corp: %s, texto: %s\n", texto->x, texto->y,
            texto->cor_borda, texto->cor_preenchimento, texto->conteudo);
}

// Escreve o código svg que representa um texto em um arquivo.
void texto_escrever_svg(Texto texto, FILE *arquivo) {
    fprintf(arquivo, "\t<text");
    if (strlen(texto->id) > 0)
        fprintf(arquivo, " id='%s'", texto->id);
    if (texto->centralizar)
        fprintf(arquivo, " text-anchor='middle'");
    fprintf(arquivo, " x='%lf' y='%lf' stroke='%s' fill='%s'>%s</text>\n", texto->x, texto->y,
            texto->cor_borda, texto->cor_preenchimento, texto->conteudo);
}

// Obtém o conteúdo de um texto.
const char *texto_obter_conteudo(Texto texto) {
    return texto->conteudo;
}

// Retorna o id de um texto.
const char *texto_obter_id(Texto texto) {
    return texto->id;
}

// Retorna a coordenada x de um texto.
double texto_obter_x(Texto texto) {
    return texto->x;
}

// Retorna a coordenada y de um texto.
double texto_obter_y(Texto texto) {
    return texto->y;
}

// Retorna um valor utilizando uma largura estimada.
double texto_obter_x_fim(Texto texto) {
    return texto->x + strlen(texto->conteudo) * LARGURA_LETRA;
}

// Retorna um valor utilizando uma altura estimada.
double texto_obter_y_fim(Texto texto) {
    return texto->y + ALTURA_LETRA;
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
    return texto->cor_borda;
}

// Define a cor da borda de um texto.
void texto_definir_cor_borda(Texto texto, const char *cor_borda) {
    if (cor_borda == NULL) {
        LOG_AVISO("Não é possível definir NULL como cor da borda de um texto!\n");
        return;
    }
    strcpy(texto->cor_borda, cor_borda);
}

// Retorna a cor de preenchimento de um texto.
const char *texto_obter_cor_preenchimento(Texto texto) {
    return texto->cor_preenchimento;
}

// Define a cor de preenchimento de um texto.
void texto_definir_cor_preenchimento(Texto texto, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        LOG_AVISO("Não é possível definir NULL como cor de preenchimento de um texto!\n");
        return;
    }
    strcpy(texto->cor_preenchimento, cor_preenchimento);
}

// Libera a memória alocada por um texto.
void texto_destruir(Texto texto) {
    free(texto->vtable);
    free(texto);
}
