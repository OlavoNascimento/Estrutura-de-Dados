#include <string.h>
#include <stdbool.h>

#include <figuras.h>
#include <retangulo.h>
#include <circulo.h>

// Margem entre o retângulo criado pela função envolver_figuras e as figuras que
// ele envolve.
#define MARGEM_CONTORNO 2
// Tamanho estimado para cada letra de um texto
#define TAMANHO_LETRAS_TEXTO 4

double max(double a, double b) {
    return a > b ? a : b;
}

double min(double a, double b) {
    return a < b ? a : b;
}

// Retorna o nome do tipo de uma figura como uma string.
const char *tipo_para_string_figura(TiposFigura tipo) {
    if(tipo <= TIPOS_FIGURA_MIN || tipo >= TIPOS_FIGURA_MAX) {
        fprintf(stderr, "Tipo de figura inválido: %d\n", tipo);
        return NULL;
    }
    char *valores[] = {
        "retângulo", "círculo", "texto", "linha",
    };
    return valores[tipo-1];
}

// Retorna a coordenada x onde uma figura se inicia.
double obter_x_inicio_figura(Figuras figura, TiposFigura tipo) {
    switch(tipo) {
        case TIPO_CIRCULO: return figura.circ.x - figura.circ.raio;
        case TIPO_RETANGULO: return figura.ret.x;
        case TIPO_TEXTO: return figura.tex.x;
        case TIPO_LINHA: return min(figura.lin.x1,figura.lin.x2);
    }
    return 0;
}

// Retorna a coordenada y onde uma figura se inicia.
double obter_y_inicio_figura(Figuras figura, TiposFigura tipo) {
    switch(tipo) {
        case TIPO_CIRCULO: return figura.circ.y - figura.circ.raio;
        case TIPO_RETANGULO: return figura.ret.y;
        case TIPO_TEXTO: return figura.tex.y;
        case TIPO_LINHA: return min(figura.lin.y1, figura.lin.y2);
    }
    return 0;
}

// Retorna a coordenada x onde uma figura acaba.
double obter_x_fim_figura(Figuras figura, TiposFigura tipo) {
    switch(tipo) {
        case TIPO_CIRCULO: return figura.circ.x + figura.circ.raio;
        case TIPO_RETANGULO: return figura.ret.x + figura.ret.largura;
        // Usa uma largura estimada para o texto
        case TIPO_TEXTO: return figura.tex.x +
                                strlen(figura.tex.texto) * TAMANHO_LETRAS_TEXTO;
        case TIPO_LINHA: return max(figura.lin.x1, figura.lin.x2);
    }
    return 0;
}

// Retorna a coordenada y onde uma figura acaba.
double obter_y_fim_figura(Figuras figura, TiposFigura tipo) {
    switch(tipo) {
        case TIPO_CIRCULO: return figura.circ.y + figura.circ.raio;
        case TIPO_RETANGULO: return figura.ret.y + figura.ret.altura;
        // Usa uma altura estimada para o texto
        case TIPO_TEXTO: return figura.tex.y + TAMANHO_LETRAS_TEXTO;
        case TIPO_LINHA: return max(figura.lin.y1, figura.lin.y2);
    }
    return 0;
}

// Retorna o id de uma figura.
const char *obter_id_figura(Figuras *figura, TiposFigura tipo) {
    switch(tipo) {
        case TIPO_CIRCULO: return figura->circ.id;
        case TIPO_RETANGULO: return figura->ret.id;
        case TIPO_TEXTO: return figura->tex.id;
    }
    return NULL;
}

// Retorna a cor da borda de uma figura.
const char *obter_cor_borda_figura(Figuras *figura, TiposFigura tipo) {
    switch(tipo) {
        case TIPO_CIRCULO: return figura->circ.cor_borda;
        case TIPO_RETANGULO: return figura->ret.cor_borda;
        case TIPO_TEXTO: return figura->tex.cor_borda;
        case TIPO_LINHA: return figura->lin.cor_borda;
    }
    return NULL;
}

// Retorna a cor do preenchimento de uma figura.
const char *obter_cor_preenchimento_figura(Figuras *figura, TiposFigura tipo) {
    switch(tipo) {
        case TIPO_CIRCULO: return figura->circ.cor_preenchimento;
        case TIPO_RETANGULO: return figura->tex.cor_preenchimento;
        case TIPO_TEXTO: return figura->tex.cor_preenchimento;
        case TIPO_LINHA: return figura->lin.cor_preenchimento;
    }
    return NULL;
}

// Escreve todos os dados de uma figura em um arquivo passado a função.
void escrever_informacoes_figura(FILE *arquivo,
                                 Figuras figura, TiposFigura tipo) {
    if(arquivo == NULL) {
        fprintf(stderr, "Arquivo nulo recebido ao escrever informações\n");
        return;
    }
    fprintf(arquivo, "tipo: %s, ", tipo_para_string_figura(tipo));
    switch(tipo) {
        case TIPO_CIRCULO:
            escrever_informacoes_circulo(arquivo, figura.circ);
            break;
        case TIPO_RETANGULO:
            escrever_informacoes_retangulo(arquivo, figura.ret);
            break;
        case TIPO_TEXTO:
            escrever_informacoes_texto(arquivo, figura.tex);
            break;
    }
}

// Escreve o código svg que representa uma figura em um arquivo.
void escrever_svg_figura(FILE *arquivo, Figuras figura, TiposFigura tipo) {
    if(arquivo == NULL) {
        fprintf(stderr, "Arquivo nulo recebido ao transformar figura em svg\n");
        return;
    }
    switch(tipo) {
        case TIPO_CIRCULO:
            escrever_svg_circulo(arquivo, figura.circ);
            break;
        case TIPO_RETANGULO:
            escrever_svg_retangulo(arquivo, figura.ret);
            break;
        case TIPO_TEXTO:
            escrever_svg_texto(arquivo, figura.tex);
            break;
        case TIPO_LINHA:
            escrever_svg_linha(arquivo, figura.lin);
            break;
    }
}

// Substitui as cores de preenchimento e borda de uma figura por aquelas
// passadas como parâmetro.
void alterar_cor_figura(Figuras *figura, TiposFigura tipo,
                        char *cor_borda, char *cor_preenchimento) {
    if(cor_borda == NULL || cor_preenchimento == NULL) {
        fprintf(stderr, "Tentando alterar cor de uma figura para nulo!\n");
        return;
    }
    switch(tipo) {
        case TIPO_CIRCULO:
            strcpy(figura->circ.cor_borda, cor_borda);
            strcpy(figura->circ.cor_preenchimento, cor_preenchimento);
            break;
        case TIPO_RETANGULO:
            strcpy(figura->ret.cor_borda, cor_borda);
            strcpy(figura->ret.cor_preenchimento, cor_preenchimento);
            break;
        case TIPO_TEXTO:
            strcpy(figura->tex.cor_borda, cor_borda);
            strcpy(figura->tex.cor_preenchimento, cor_preenchimento);
            break;
    }
}

// Retorna verdadeiro caso um circulo e um retângulo se intersectem.
bool checar_interseccao_circulo_retangulo(Circulo circ, Retangulo ret) {
    double deltaX = circ.x - max(ret.x, min(circ.x, ret.x + ret.largura));
    double deltaY = circ.y - max(ret.y, min(circ.y, ret.y + ret.altura));
    return (deltaX * deltaX + deltaY * deltaY) <= (circ.raio * circ.raio);
}

// Retorna verdadeiro caso duas figuras se intersectem.
bool checar_interseccao_figuras(Figuras fig1, TiposFigura tipo1,
                                Figuras fig2, TiposFigura tipo2) {
    bool intersectam = false;
    if(tipo1 == TIPO_RETANGULO && tipo1 == tipo2)
        intersectam = checar_interseccao_retangulo(fig1.ret, fig2.ret);
    else if(tipo1 == TIPO_CIRCULO && tipo1 == tipo2)
        intersectam = checar_interseccao_circulo(fig1.circ, fig2.circ);
    else if(tipo1 == TIPO_CIRCULO && tipo2 == TIPO_RETANGULO)
        intersectam = checar_interseccao_circulo_retangulo(fig1.circ, fig2.ret);
    else if(tipo1 == TIPO_RETANGULO && tipo2 == TIPO_CIRCULO)
        intersectam = checar_interseccao_circulo_retangulo(fig2.circ, fig1.ret);

    return intersectam;
}

// Cria um retângulo com coordenadas e dimensões necessárias para envolver duas
// figuras.
Retangulo envolver_figuras(Figuras fig1, TiposFigura tipo1,
                           Figuras fig2, TiposFigura tipo2) {
    Retangulo contorno = {
        .id = "\0",
        .cor_borda = "black",
        .cor_preenchimento = "none",
    };
    contorno.x = min(
        obter_x_inicio_figura(fig1, tipo1),
        obter_x_inicio_figura(fig2, tipo2)
    ) - MARGEM_CONTORNO;
    contorno.y = min(
        obter_y_inicio_figura(fig1, tipo1),
        obter_y_inicio_figura(fig2, tipo2)
    ) - MARGEM_CONTORNO;
    contorno.largura = max(
        obter_x_fim_figura(fig1, tipo1),
        obter_x_fim_figura(fig2, tipo2)
    ) - contorno.x + 2 * MARGEM_CONTORNO;
    contorno.altura = max(
        obter_y_fim_figura(fig1, tipo1),
        obter_y_fim_figura(fig2, tipo2)
    ) - contorno.y + 2 * MARGEM_CONTORNO;

    return contorno;
}

// Checa se um ponto se encontra dentro ou fora de uma figura.
bool checar_ponto_interno_figura(Figuras figura, TiposFigura tipo,
                                 double ponto_x, double ponto_y) {
    bool interno = false;
    switch(tipo) {
        case TIPO_CIRCULO:
            interno = checar_ponto_interno_circulo(
                figura.circ, ponto_x, ponto_y
            );
            break;
        case TIPO_RETANGULO:
            interno = checar_ponto_interno_retangulo(
                figura.ret, ponto_x, ponto_y
            );
            break;
    }
    return interno;
}

// Cria uma linha que conecta um ponto ao centro de uma figura
Linha ligar_ponto_figura(Circulo ponto, Figuras figura, TiposFigura tipo) {
    double fig_centro_x = 0, fig_centro_y = 0;
    switch(tipo) {
        case TIPO_CIRCULO:
            fig_centro_x = figura.circ.x;
            fig_centro_y = figura.circ.y;
            break;
        case TIPO_RETANGULO:
            fig_centro_x = figura.ret.x + figura.ret.largura / 2;
            fig_centro_y = figura.ret.y + figura.ret.altura / 2;
            break;
    }
    Linha ligacao = {
        .x1 = ponto.x,
        .y1 = ponto.y,
        .x2 = fig_centro_x,
        .y2 = fig_centro_y,
    };
    strcpy(ligacao.cor_borda, ponto.cor_borda);
    strcpy(ligacao.cor_preenchimento, ponto.cor_preenchimento);
    return ligacao;
}
