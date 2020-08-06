#include "figuras.h"

#include <stdbool.h>
#include <string.h>

#include "circulo.h"
#include "linha.h"
#include "retangulo.h"
#include "texto.h"

typedef struct {
    TiposFigura tipo;
    union {
        Retangulo ret;
        Circulo cir;
        Texto tex;
        Linha lin;
    };
} Figura;

// Cria uma figura com uma figura e um tipo
Figura figura_criar(void *figura, TiposFigura tipo) {
    if (tipo <= TIPOS_FIGURA_MIN || tipo >= TIPOS_FIGURA_MAX) {
        fprintf(stderr, "ERRO: Tipo de figura inválido: %d\n", tipo);
        return;
    }
    Figura fig;
    fig.tipo = tipo;
    switch (tipo) {
        case TIPO_CIRCULO:
            fig.cir = figura;
        case TIPO_RETANGULO:
            fig.ret = figura;
        case TIPO_TEXTO:
            fig.tex = figura;
        case TIPO_LINHA:
            fig.lin = figura;
    }
    return fig;
}

// Cria uma figura com base nos dados fornecidos por uma linha
Figura figura_ler(const char *linha, TiposFigura tipo) {
    if (linha == NULL) {
        fprintf(stderr, "ERRO: Linha vazia passada a função ler figura!\n", tipo);
        return;
    }
    if (tipo <= TIPOS_FIGURA_MIN || tipo >= TIPOS_FIGURA_MAX) {
        fprintf(stderr, "ERRO: Tipo de figura inválido passada a função ler figura: %d\n", tipo);
        return;
    }

    switch (tipo) {
        case TIPO_CIRCULO:
            Circulo cir = circulo_ler(linha);
            return figura_criar(cir, tipo);
        case TIPO_RETANGULO:
            Retangulo ret = retangulo_ler(linha);
            return figura_criar(ret, tipo);
        case TIPO_TEXTO:
            Texto tex = texto_ler(linha);
            return figura_criar(tex, tipo);
        case TIPO_LINHA:
            Linha lin = linha_ler(linha);
            return figura_criar(lin, tipo);
    }
}

// Escreve todos os dados de uma figura em um arquivo passado a função.
void figura_escrever_informacoes(FILE *arquivo, Figura figura) {
    if (arquivo == NULL) {
        fprintf(stderr, "ERRO: Arquivo nulo recebido ao escrever informações\n");
        return;
    }
    fprintf(arquivo, "tipo: %s, ", figura_tipo_para_string(figura.tipo));
    switch (figura.tipo) {
        case TIPO_CIRCULO:
            circulo_escrever_informacoes(arquivo, figura.cir);
            break;
        case TIPO_RETANGULO:
            retangulo_escrever_informacoes(arquivo, figura.ret);
            break;
        case TIPO_TEXTO:
            texto_escrever_informacoes(arquivo, figura.tex);
            break;
    }
}

// Escreve o código svg que representa uma figura em um arquivo.
void figura_escrever_svg(FILE *arquivo, Figura figura) {
    if (arquivo == NULL) {
        fprintf(stderr, "ERRO: Arquivo nulo recebido ao transformar figura em svg\n");
        return;
    }
    switch (figura.tipo) {
        case TIPO_CIRCULO:
            circulo_escrever_svg(arquivo, figura.cir);
            break;
        case TIPO_RETANGULO:
            retangulo_escrever_svg(arquivo, figura.ret);
            break;
        case TIPO_TEXTO:
            texto_escrever_svg(arquivo, figura.tex);
            break;
        case TIPO_LINHA:
            linha_escrever_svg(arquivo, figura.lin);
            break;
    }
}

double max(double a, double b) {
    return a > b ? a : b;
}

double min(double a, double b) {
    return a < b ? a : b;
}

// Retorna verdadeiro caso um circulo e um retângulo se intersectem.
bool circulo_retangulo_checar_interseccao(Circulo cir, Retangulo ret) {
    double deltaX = circulo_obter_x(cir) - max(retangulo_obter_x(ret),
                                               min(circulo_obter_x(ret),
                                                   retangulo_obter_x(ret) + retangulo_obter_largura(ret)));
    double deltaY = circulo_obter_y(cir) - max(retangulo_obter_y(ret),
                                               min(circulo_obter_y(ret),
                                                   retangulo_obter_y(ret) + retangulo_obter_altura(ret)));
    return (deltaX * deltaX + deltaY * deltaY) <= (circulo_obter_raio(cir) * circulo_obter_raio(cir));
}

// Retorna verdadeiro caso duas figuras se intersectem.
bool figura_checar_interseccao(Figura figura1, Figura figura2) {
    bool intersectam = false;
    if (figura1.tipo == TIPO_RETANGULO && figura1.tipo == figura2.tipo)
        intersectam = retangulo_checar_interseccao(figura1.ret, figura2.ret);
    else if (figura1.tipo == TIPO_CIRCULO && figura1.tipo == figura2.tipo)
        intersectam = circulo_checar_interseccao(figura1.cir, figura2.cir);
    else if (figura1.tipo == TIPO_CIRCULO && figura2.tipo == TIPO_RETANGULO)
        intersectam = circulo_retangulo_checar_interseccao(figura1.cir, figura2.ret);
    else if (figura1.tipo == TIPO_RETANGULO && figura2.tipo == TIPO_CIRCULO)
        intersectam = circulo_retangulo_checar_interseccao(figura2.cir, figura1.ret);

    return intersectam;
}

// Checa se um ponto se encontra dentro ou fora de uma figura.
bool figura_checar_ponto_interno(Figura figura, double ponto_x, double ponto_y) {
    bool interno = false;
    switch (figura.tipo) {
        case TIPO_CIRCULO:
            interno = checar_ponto_interno_circulo(figura.cir, ponto_x, ponto_y);
            break;
        case TIPO_RETANGULO:
            interno = checar_ponto_interno_retangulo(figura.ret, ponto_x, ponto_y);
            break;
    }
    return interno;
}

// Retorna o nome do tipo de uma figura.
const char *figura_obter_tipo(Figura figura) {
    return figura.tipo;
}

// Retorna o nome do tipo de uma figura como uma string.
const char *figura_obter_string_tipo(Figura figura) {
    if (figura.tipo <= TIPOS_FIGURA_MIN || figura.tipo >= TIPOS_FIGURA_MAX) {
        fprintf(stderr, "ERRO: Tipo de figura inválido: %d\n", figura.tipo);
        return NULL;
    }
    const char *valores[] = {
        "retângulo",
        "círculo",
        "texto",
        "linha",
    };
    return valores[figura.tipo];
}

// Retorna o nome do tipo de uma figura.
void *figura_obter_figura(Figura *figura) {
    switch (figura->tipo) {
        case TIPO_CIRCULO:
            return figura->cir;
        case TIPO_RETANGULO:
            return figura->ret;
        case TIPO_TEXTO:
            return figura->tex;
        case TIPO_LINHA:
            return figura->lin;
    }
}

// Retorna a coordenada x onde uma figura se inicia.
double figura_obter_x_inicio(Figura figura) {
    switch (figura.tipo) {
        case TIPO_CIRCULO:
            return circulo_obter_x(figura.cir) - circulo_obter_raio(figura.cir);
        case TIPO_RETANGULO:
            return retangulo_obter_x(figura.ret);
        case TIPO_TEXTO:
            return texto_obter_x(figura.tex);
        case TIPO_LINHA:
            return min(linha_obter_x1(figura.lin), linha_obter_x2(figura.lin));
    }
    return 0;
}

// Retorna a coordenada y onde uma figura se inicia.
double figura_obter_y_inicio(Figura figura) {
    switch (figura.tipo) {
        case TIPO_CIRCULO:
            return circulo_obter_y(figura.cir) - circulo_obter_raio(figura.cir);
        case TIPO_RETANGULO:
            return retangulo_obter_y(figura.ret);
        case TIPO_TEXTO:
            return texto_obter_y(figura.tex);
        case TIPO_LINHA:
            return min(linha_obter_y1(figura.lin), linha_obter_y2(figura.lin));
    }
    return 0;
}

// Retorna a coordenada x onde uma figura acaba.
double figura_obter_x_fim(Figura figura) {
    switch (figura.tipo) {
        case TIPO_CIRCULO:
            return circulo_obter_x(figura.cir) + circulo_obter_raio(figura.cir);
        case TIPO_RETANGULO:
            return retangulo_obter_x(figura.ret) + retangulo_obter_largura(figura.ret);
        case TIPO_TEXTO:
            // Usa uma largura estimada para o texto
            return texto_obter_x(figura.tex) + strlen(texto_obter_texto(figura.tex)) * 4;
        case TIPO_LINHA:
            return max(linha_obter_x1(figura.lin), linha_obter_x2(figura.lin));
    }
    return 0;
}

// Retorna a coordenada x do centro de uma figura.
double figura_obter_centro_x(Figura figura) {
    switch (figura.tipo) {
        case TIPO_CIRCULO:
            return circulo_obter_x(figura.cir);
        case TIPO_RETANGULO:
            return retangulo_obter_x(figura.ret) + retangulo_obter_largura(figura.ret) / 2;
    }
}

// Retorna a coordenada y do centro de uma figura.
double figura_obter_centro_y(Figura figura) {
    switch (figura.tipo) {
        case TIPO_CIRCULO:
            return circulo_obter_y(figura.cir);
        case TIPO_RETANGULO:
            return retangulo_obter_y(figura.ret) + retangulo_obter_altura(figura.ret) / 2;
    }
}

// Retorna a coordenada y onde uma figura acaba.
double figura_obter_y_fim(Figura figura) {
    switch (figura.tipo) {
        case TIPO_CIRCULO:
            return circulo_obter_y(figura.cir) + circulo_obter_raio(figura.cir);
        case TIPO_RETANGULO:
            return retangulo_obter_y(figura.ret) + retangulo_obter_altura(figura.ret);
        // Usa uma altura estimada para o texto
        case TIPO_TEXTO:
            return texto_obter_y(figura.tex);
        case TIPO_LINHA:
            return max(linha_obter_y1(figura.lin), linha_obter_y2(figura.lin));
    }
    return 0;
}

// Retorna o id de uma figura.
const char *figura_obter_id(Figura *figura) {
    switch (figura->tipo) {
        case TIPO_CIRCULO:
            return circulo_obter_id(figura->cir);
        case TIPO_RETANGULO:
            return retangulo_obter_id(figura->ret);
        case TIPO_TEXTO:
            return texto_obter_id(figura->tex);
    }
    return NULL;
}

// Retorna a cor da borda de uma figura.
const char *figura_obter_cor_borda(Figura *figura) {
    switch (figura->tipo) {
        case TIPO_CIRCULO:
            return circulo_obter_cor_borda(figura->cir);
        case TIPO_RETANGULO:
            return retangulo_obter_cor_borda(figura->ret);
        case TIPO_TEXTO:
            return texto_obter_cor_borda(figura->tex);
        case TIPO_LINHA:
            return linha_obter_cor_borda(figura->lin);
    }
    return NULL;
}

// Substitui a cor da borda de uma figura.
void figura_definir_cor_borda(Figura *figura, const char *cor_borda) {
    if (cor_borda == NULL) {
        fprintf(stderr, "ERRO: Tentando alterar cor da borda de uma figura para nulo!\n");
        return;
    }
    switch (figura->tipo) {
        case TIPO_CIRCULO:
            circulo_definir_cor_borda(figura, cor_borda);
            break;
        case TIPO_RETANGULO:
            retangulo_definir_cor_borda(figura, cor_borda);
            break;
        case TIPO_TEXTO:
            texto_definir_cor_borda(figura, cor_borda);
            break;
    }
}

// Retorna a cor do preenchimento de uma figura.
const char *figura_obter_cor_preenchimento(Figura *figura) {
    switch (figura->tipo) {
        case TIPO_CIRCULO:
            return circulo_obter_cor_preenchimento(figura->cir);
        case TIPO_RETANGULO:
            return retangulo_obter_cor_preenchimento(figura->tex);
        case TIPO_TEXTO:
            return texto_obter_cor_preenchimento(figura->tex);
        case TIPO_LINHA:
            return linha_obter_cor_preenchimento(figura->lin);
    }
    return NULL;
}

// Substitui a cor de preenchimento de uma figura.
void figura_definir_cor_preenchimento(Figura *figura, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        fprintf(stderr, "ERRO: Tentando alterar cor de preenchimento de uma figura para nulo!\n");
        return;
    }
    switch (figura->tipo) {
        case TIPO_CIRCULO:
            circulo_definir_cor_preenchimento(figura, cor_preenchimento);
            break;
        case TIPO_RETANGULO:
            retangulo_definir_cor_preenchimento(figura, cor_preenchimento);
            break;
        case TIPO_TEXTO:
            texto_definir_cor_preenchimento(figura, cor_preenchimento);
            break;
    }
}
