#include "figuras.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "circulo.h"
#include "hidrante.h"
#include "linha.h"
#include "logging.h"
#include "matematica.h"
#include "posto.h"
#include "quadra.h"
#include "radio_base.h"
#include "retangulo.h"
#include "semaforo.h"
#include "texto.h"

typedef struct {
    TiposFigura tipo;
    union {
        Circulo cir;
        Hidrante hid;
        Linha lin;
        Quadra qua;
        Radio rad;
        Retangulo ret;
        Posto pos;
        Semaforo sem;
        Texto tex;
    };
} FiguraImp;

// Cria um struct figura que recebe uma figura a ser armazenada e o tipo dessa figura.
Figura figura_criar(void *figura, TiposFigura tipo) {
    if (figura == NULL) {
        LOG_ERROR("Figura nula passada ao criar figura!\n");
        return NULL;
    }
    if (tipo <= TIPOS_FIGURA_MIN || tipo >= TIPOS_FIGURA_MAX) {
        LOG_ERROR("Tipo de figura inválido passado para criar figura: %d!\n", tipo);
        return NULL;
    }
    FiguraImp *fig = malloc(sizeof(FiguraImp));
    fig->tipo = tipo;
    switch (tipo) {
        case TIPO_CIRCULO:
            fig->cir = figura;
            break;
        case TIPO_HIDRANTE:
            fig->hid = figura;
            break;
        case TIPO_LINHA:
            fig->lin = figura;
            break;
        case TIPO_QUADRA:
            fig->qua = figura;
            break;
        case TIPO_RADIO:
            fig->rad = figura;
            break;
        case TIPO_RETANGULO:
            fig->ret = figura;
            break;
        case TIPO_POSTO:
            fig->pos = figura;
            break;
        case TIPO_SEMAFORO:
            fig->sem = figura;
            break;
        case TIPO_TEXTO:
            fig->tex = figura;
            break;
        default:
            LOG_ERROR("Tipo de figura inválido passado para criar figura: %d!\n", tipo);
            break;
    }
    return fig;
}

// Escreve todos os dados de uma figura em um arquivo passado a função.
void figura_escrever_informacoes(FILE *arquivo, Figura figura) {
    if (arquivo == NULL) {
        LOG_ERROR("Arquivo nulo recebido ao escrever informações!\n");
        return;
    }
    FiguraImp *figuraImp = (FiguraImp *) figura;
    fprintf(arquivo, "tipo: %s, ", figura_obter_string_tipo(figura));
    switch (figuraImp->tipo) {
        case TIPO_CIRCULO:
            circulo_escrever_informacoes(arquivo, figuraImp->cir);
            break;
        case TIPO_HIDRANTE:
            hidrante_escrever_informacoes(arquivo, figuraImp->hid);
            break;
        case TIPO_QUADRA:
            quadra_escrever_informacoes(arquivo, figuraImp->qua);
            break;
        case TIPO_RADIO:
            radio_escrever_informacoes(arquivo, figuraImp->rad);
            break;
        case TIPO_RETANGULO:
            retangulo_escrever_informacoes(arquivo, figuraImp->ret);
            break;
        case TIPO_POSTO:
            posto_escrever_informacoes(arquivo, figuraImp->pos);
            break;
        case TIPO_SEMAFORO:
            semaforo_escrever_informacoes(arquivo, figuraImp->sem);
            break;
        case TIPO_TEXTO:
            texto_escrever_informacoes(arquivo, figuraImp->tex);
            break;
        default:
            LOG_ERROR("Tipo de figura inválido passado para escrever informações: %d!\n",
                      figuraImp->tipo);
            break;
    }
}

// Escreve o código svg que representa uma figura em um arquivo.
void figura_escrever_svg(FILE *arquivo, Figura figura) {
    if (arquivo == NULL) {
        LOG_ERROR("Arquivo nulo recebido ao transformar figura em svg!\n");
        return;
    }
    FiguraImp *figuraImp = (FiguraImp *) figura;
    switch (figuraImp->tipo) {
        case TIPO_CIRCULO:
            circulo_escrever_svg(arquivo, figuraImp->cir);
            break;
        case TIPO_HIDRANTE:
            hidrante_escrever_svg(arquivo, figuraImp->hid);
            break;
        case TIPO_RADIO:
            radio_escrever_svg(arquivo, figuraImp->rad);
            break;
        case TIPO_QUADRA:
            quadra_escrever_svg(arquivo, figuraImp->qua);
            break;
        case TIPO_POSTO:
            posto_escrever_svg(arquivo, figuraImp->pos);
            break;
        case TIPO_SEMAFORO:
            semaforo_escrever_svg(arquivo, figuraImp->sem);
            break;
        case TIPO_LINHA:
            linha_escrever_svg(arquivo, figuraImp->lin);
            break;
        case TIPO_RETANGULO:
            retangulo_escrever_svg(arquivo, figuraImp->ret);
            break;
        case TIPO_TEXTO:
            texto_escrever_svg(arquivo, figuraImp->tex);
            break;
        default:
            LOG_ERROR("Tipo de figura inválido passado para escrever svg: %d!\n", figuraImp->tipo);
            break;
    }
}

// Retorna verdadeiro caso um circulo e um retângulo se intersectem.
bool circulo_retangulo_checar_interseccao(Circulo cir, Retangulo ret) {
    double deltaX =
        circulo_obter_x(cir) -
        max(retangulo_obter_x(ret),
            min(circulo_obter_x(cir), retangulo_obter_x(ret) + retangulo_obter_largura(ret)));
    double deltaY =
        circulo_obter_y(cir) -
        max(retangulo_obter_y(ret),
            min(circulo_obter_y(cir), retangulo_obter_y(ret) + retangulo_obter_altura(ret)));
    return (deltaX * deltaX + deltaY * deltaY) <=
           (circulo_obter_raio(cir) * circulo_obter_raio(cir));
}

// Retorna verdadeiro caso duas figuras se intersectem.
bool figura_checar_interseccao(Figura figura1, Figura figura2) {
    FiguraImp *figuraImp1 = (FiguraImp *) figura1;
    FiguraImp *figuraImp2 = (FiguraImp *) figura2;
    bool intersectam = false;
    if (figuraImp1->tipo == TIPO_RETANGULO && figuraImp1->tipo == figuraImp2->tipo)
        intersectam = retangulo_checar_interseccao(figuraImp1->ret, figuraImp2->ret);
    else if (figuraImp1->tipo == TIPO_CIRCULO && figuraImp1->tipo == figuraImp2->tipo)
        intersectam = circulo_checar_interseccao(figuraImp1->cir, figuraImp2->cir);
    else if (figuraImp1->tipo == TIPO_CIRCULO && figuraImp2->tipo == TIPO_RETANGULO)
        intersectam = circulo_retangulo_checar_interseccao(figuraImp1->cir, figuraImp2->ret);
    else if (figuraImp1->tipo == TIPO_RETANGULO && figuraImp2->tipo == TIPO_CIRCULO)
        intersectam = circulo_retangulo_checar_interseccao(figuraImp2->cir, figuraImp1->ret);

    return intersectam;
}

// Checa se um ponto se encontra dentro ou fora de uma figura.
bool figura_checar_ponto_interno(Figura figura, double ponto_x, double ponto_y) {
    FiguraImp *figuraImp = (FiguraImp *) figura;
    bool interno = false;
    switch (figuraImp->tipo) {
        case TIPO_CIRCULO:
            interno = circulo_checar_ponto_interno(figuraImp->cir, ponto_x, ponto_y);
            break;
        case TIPO_RETANGULO:
            interno = retangulo_checar_ponto_interno(figuraImp->ret, ponto_x, ponto_y);
            break;
        default:
            LOG_ERROR("Tipo de figura inválido passado para checar ponto interno: %d!\n",
                      figuraImp->tipo);
            break;
    }
    return interno;
}

// Retorna o nome do tipo de uma figura.
TiposFigura figura_obter_tipo(Figura figura) {
    FiguraImp *figuraImp = (FiguraImp *) figura;
    return figuraImp->tipo;
}

// Retorna o nome do tipo de uma figura como uma string.
const char *figura_obter_string_tipo(Figura figura) {
    FiguraImp *figuraImp = (FiguraImp *) figura;
    const char *valores[] = {"círculo",    "caso",      "hidrante", "linha",    "quadra",
                             "rádio-base", "retângulo", "posto",    "semáforo", "texto"};
    return valores[figuraImp->tipo];
}

// Retorna o nome do tipo de uma figura.
void *figura_obter_figura(Figura figura) {
    FiguraImp *figuraImp = (FiguraImp *) figura;
    switch (figuraImp->tipo) {
        case TIPO_CIRCULO:
            return figuraImp->cir;
        case TIPO_HIDRANTE:
            return figuraImp->hid;
        case TIPO_QUADRA:
            return figuraImp->qua;
        case TIPO_RADIO:
            return figuraImp->rad;
        case TIPO_RETANGULO:
            return figuraImp->ret;
        case TIPO_POSTO:
            return figuraImp->pos;
        case TIPO_SEMAFORO:
            return figuraImp->sem;
        case TIPO_TEXTO:
            return figuraImp->tex;
        default:
            LOG_ERROR("Tipo de figura inválido passado para obter figura: %d!\n", figuraImp->tipo);
            return NULL;
    }
}

// Retorna a coordenada x onde uma figura se inicia.
double figura_obter_x_inicio(Figura figura) {
    FiguraImp *figuraImp = (FiguraImp *) figura;
    switch (figuraImp->tipo) {
        case TIPO_CIRCULO:
            return circulo_obter_x(figuraImp->cir) - circulo_obter_raio(figuraImp->cir);
        case TIPO_HIDRANTE:
            return hidrante_obter_x(figuraImp->hid) - hidrante_obter_raio(figuraImp->hid);
        case TIPO_LINHA:
            return min(linha_obter_x1(figuraImp->lin), linha_obter_x2(figuraImp->lin));
        case TIPO_QUADRA:
            return quadra_obter_x(figuraImp->qua);
        case TIPO_RADIO:
            return radio_obter_x(figuraImp->rad) - radio_obter_raio(figuraImp->rad);
        case TIPO_RETANGULO:
            return retangulo_obter_x(figuraImp->ret);
        case TIPO_POSTO:
            return posto_obter_x(figuraImp->pos) - posto_obter_raio(figuraImp->pos);
        case TIPO_SEMAFORO:
            return semaforo_obter_x(figuraImp->sem);
        case TIPO_TEXTO:
            return texto_obter_x(figuraImp->tex);
        default:
            LOG_ERROR("Tipo de figura inválido passado para obter x inicio: %d!\n",
                      figuraImp->tipo);
            break;
    }
    return 0;
}

// Retorna a coordenada y onde uma figura se inicia.
double figura_obter_y_inicio(Figura figura) {
    FiguraImp *figuraImp = (FiguraImp *) figura;
    switch (figuraImp->tipo) {
        case TIPO_CIRCULO:
            return circulo_obter_y(figuraImp->cir) - circulo_obter_raio(figuraImp->cir);
        case TIPO_HIDRANTE:
            return hidrante_obter_y(figuraImp->hid) - hidrante_obter_raio(figuraImp->hid);
        case TIPO_LINHA:
            return min(linha_obter_y1(figuraImp->lin), linha_obter_y2(figuraImp->lin));
        case TIPO_QUADRA:
            return quadra_obter_y(figuraImp->qua);
        case TIPO_RADIO:
            return radio_obter_y(figuraImp->rad) - radio_obter_raio(figuraImp->rad);
        case TIPO_RETANGULO:
            return retangulo_obter_y(figuraImp->ret);
        case TIPO_POSTO:
            return posto_obter_y(figuraImp->pos) - posto_obter_raio(figuraImp->pos);
        case TIPO_SEMAFORO:
            return semaforo_obter_y(figuraImp->sem);
        case TIPO_TEXTO:
            return texto_obter_y(figuraImp->tex);
        default:
            LOG_ERROR("Tipo de figura inválido passado para obter y inicio: %d!\n",
                      figuraImp->tipo);
            break;
    }
    return 0;
}

// Retorna a coordenada x onde uma figura acaba.
double figura_obter_x_fim(Figura figura) {
    FiguraImp *figuraImp = (FiguraImp *) figura;
    switch (figuraImp->tipo) {
        case TIPO_CIRCULO:
            return circulo_obter_x(figuraImp->cir) + circulo_obter_raio(figuraImp->cir);
        case TIPO_HIDRANTE:
            return hidrante_obter_x(figuraImp->hid) + hidrante_obter_raio(figuraImp->hid);
        case TIPO_LINHA:
            return max(linha_obter_x1(figuraImp->lin), linha_obter_x2(figuraImp->lin));
        case TIPO_QUADRA:
            return quadra_obter_x(figuraImp->qua) + quadra_obter_largura(figuraImp->qua);
        case TIPO_RADIO:
            return radio_obter_x(figuraImp->rad) + radio_obter_raio(figuraImp->rad);
        case TIPO_RETANGULO:
            return retangulo_obter_x(figuraImp->ret) + retangulo_obter_largura(figuraImp->ret);
        case TIPO_POSTO:
            return posto_obter_x(figuraImp->pos) + posto_obter_raio(figuraImp->pos);
        case TIPO_SEMAFORO:
            return semaforo_obter_x(figuraImp->sem) + semaforo_obter_largura(figuraImp->sem);
        case TIPO_TEXTO:
            // Usa uma largura estimada para o texto
            return texto_obter_x(figuraImp->tex) + strlen(texto_obter_conteudo(figuraImp->tex)) * 4;
        default:
            LOG_ERROR("Tipo de figura inválido passado para obter x fim: %d!\n", figuraImp->tipo);
            break;
    }
    return 0;
}

// Retorna a coordenada y onde uma figura acaba.
double figura_obter_y_fim(Figura figura) {
    FiguraImp *figuraImp = (FiguraImp *) figura;
    switch (figuraImp->tipo) {
        case TIPO_CIRCULO:
            return circulo_obter_y(figuraImp->cir) + circulo_obter_raio(figuraImp->cir);
        case TIPO_HIDRANTE:
            return hidrante_obter_y(figuraImp->hid) + hidrante_obter_raio(figuraImp->hid);
        case TIPO_LINHA:
            return max(linha_obter_y1(figuraImp->lin), linha_obter_y2(figuraImp->lin));
        case TIPO_QUADRA:
            return quadra_obter_y(figuraImp->qua) + quadra_obter_altura(figuraImp->qua);
        case TIPO_RADIO:
            return radio_obter_y(figuraImp->rad) + radio_obter_raio(figuraImp->rad);
        case TIPO_RETANGULO:
            return retangulo_obter_y(figuraImp->ret) + retangulo_obter_altura(figuraImp->ret);
        case TIPO_POSTO:
            return posto_obter_y(figuraImp->pos) + posto_obter_raio(figuraImp->pos);
        case TIPO_SEMAFORO:
            return semaforo_obter_y(figuraImp->sem);
        case TIPO_TEXTO:
            return texto_obter_y(figuraImp->tex);
        default:
            LOG_ERROR("Tipo de figura inválido passado para obter y fim: %d!\n", figuraImp->tipo);
            break;
    }
    return 0;
}

// Retorna a coordenada x do centro de uma figura.
double figura_obter_centro_x(Figura figura) {
    FiguraImp *figuraImp = (FiguraImp *) figura;
    switch (figuraImp->tipo) {
        case TIPO_CIRCULO:
            return circulo_obter_x(figuraImp->cir);
        case TIPO_HIDRANTE:
            return hidrante_obter_x(figuraImp->hid);
        case TIPO_QUADRA:
            return quadra_obter_x(figuraImp->qua) + quadra_obter_largura(figuraImp->qua) / 2;
        case TIPO_RADIO:
            return radio_obter_x(figuraImp->rad);
        case TIPO_RETANGULO:
            return retangulo_obter_x(figuraImp->ret) + retangulo_obter_largura(figuraImp->ret) / 2;
        case TIPO_POSTO:
            return posto_obter_x(figuraImp->pos);
        case TIPO_SEMAFORO:
            return semaforo_obter_x(figuraImp->sem) + semaforo_obter_largura(figuraImp->sem) / 2;
        default:
            LOG_ERROR("Tipo de figura inválido passado para obter centro x: %d!\n",
                      figuraImp->tipo);
            return 0;
    }
}

// Retorna a coordenada y do centro de uma figura.
double figura_obter_centro_y(Figura figura) {
    FiguraImp *figuraImp = (FiguraImp *) figura;
    switch (figuraImp->tipo) {
        case TIPO_CIRCULO:
            return circulo_obter_y(figuraImp->cir);
        case TIPO_HIDRANTE:
            return hidrante_obter_y(figuraImp->hid);
        case TIPO_QUADRA:
            return quadra_obter_y(figuraImp->qua) + quadra_obter_altura(figuraImp->qua) / 2;
        case TIPO_RADIO:
            return radio_obter_y(figuraImp->rad);
        case TIPO_RETANGULO:
            return retangulo_obter_y(figuraImp->ret) + retangulo_obter_altura(figuraImp->ret) / 2;
        case TIPO_POSTO:
            return posto_obter_y(figuraImp->pos);
        case TIPO_SEMAFORO:
            return semaforo_obter_y(figuraImp->sem) + semaforo_obter_altura(figuraImp->sem) / 2;
        default:
            LOG_ERROR("Tipo de figura inválido passado para obter centro y: %d!\n",
                      figuraImp->tipo);
            return 0;
    }
}

// Retorna o id de uma figura.
const char *figura_obter_id(Figura figura) {
    FiguraImp *figuraImp = (FiguraImp *) figura;
    switch (figuraImp->tipo) {
        case TIPO_CIRCULO:
            return circulo_obter_id(figuraImp->cir);
        case TIPO_HIDRANTE:
            return hidrante_obter_id(figuraImp->hid);
        case TIPO_LINHA:
            return "";
        case TIPO_QUADRA:
            return quadra_obter_id(figuraImp->qua);
        case TIPO_RADIO:
            return radio_obter_id(figuraImp->rad);
        case TIPO_RETANGULO:
            return retangulo_obter_id(figuraImp->ret);
        case TIPO_POSTO:
            return posto_obter_id(figuraImp->pos);
        case TIPO_SEMAFORO:
            return semaforo_obter_id(figuraImp->sem);
        case TIPO_TEXTO:
            return texto_obter_id(figuraImp->tex);
        default:
            LOG_ERROR("Tipo de figura inválido passado para obter id: %d!\n", figuraImp->tipo);
            break;
    }
    return NULL;
}

// Retorna a cor da borda de uma figura.
const char *figura_obter_cor_borda(Figura figura) {
    FiguraImp *figuraImp = (FiguraImp *) figura;
    switch (figuraImp->tipo) {
        case TIPO_CIRCULO:
            return circulo_obter_cor_borda(figuraImp->cir);
        case TIPO_HIDRANTE:
            return hidrante_obter_cor_borda(figuraImp->hid);
        case TIPO_LINHA:
            return linha_obter_cor_borda(figuraImp->lin);
        case TIPO_QUADRA:
            return quadra_obter_cor_borda(figuraImp->qua);
        case TIPO_RADIO:
            return radio_obter_cor_borda(figuraImp->rad);
        case TIPO_RETANGULO:
            return retangulo_obter_cor_borda(figuraImp->ret);
        case TIPO_POSTO:
            return posto_obter_cor_borda(figuraImp->pos);
        case TIPO_SEMAFORO:
            return semaforo_obter_cor_borda(figuraImp->sem);
        case TIPO_TEXTO:
            return texto_obter_cor_borda(figuraImp->tex);
        default:
            LOG_ERROR("Tipo de figura inválido passado para obter cor borda: %d!\n",
                      figuraImp->tipo);
            break;
    }
    return NULL;
}

// Define a cor da borda de uma figura.
void figura_definir_cor_borda(Figura figura, const char *cor_borda) {
    if (cor_borda == NULL) {
        LOG_ERROR("Tentando alterar cor da borda de uma figura para nulo!\n");
        return;
    }
    FiguraImp *figuraImp = (FiguraImp *) figura;
    switch (figuraImp->tipo) {
        case TIPO_CIRCULO:
            circulo_definir_cor_borda(figuraImp->cir, cor_borda);
            break;
        case TIPO_HIDRANTE:
            hidrante_definir_cor_borda(figuraImp->hid, cor_borda);
            break;
        case TIPO_LINHA:
            linha_definir_cor_borda(figuraImp->lin, cor_borda);
            break;
        case TIPO_QUADRA:
            quadra_definir_cor_borda(figuraImp->qua, cor_borda);
            break;
        case TIPO_RADIO:
            radio_definir_cor_borda(figuraImp->rad, cor_borda);
            break;
        case TIPO_RETANGULO:
            retangulo_definir_cor_borda(figuraImp->ret, cor_borda);
            break;
        case TIPO_POSTO:
            radio_definir_cor_borda(figuraImp->pos, cor_borda);
            break;
        case TIPO_SEMAFORO:
            semaforo_definir_cor_borda(figuraImp->sem, cor_borda);
            break;
        case TIPO_TEXTO:
            texto_definir_cor_borda(figuraImp->tex, cor_borda);
            break;
        default:
            LOG_ERROR("Tipo de figura inválido passado para definir cor borda: %d!\n",
                      figuraImp->tipo);
            break;
    }
}

// Retorna a cor do preenchimento de uma figura.
const char *figura_obter_cor_preenchimento(Figura figura) {
    FiguraImp *figuraImp = (FiguraImp *) figura;
    switch (figuraImp->tipo) {
        case TIPO_CIRCULO:
            return circulo_obter_cor_preenchimento(figuraImp->cir);
        case TIPO_HIDRANTE:
            return hidrante_obter_cor_preenchimento(figuraImp->hid);
        case TIPO_LINHA:
            return linha_obter_cor_preenchimento(figuraImp->lin);
        case TIPO_QUADRA:
            return quadra_obter_cor_preenchimento(figuraImp->qua);
        case TIPO_RADIO:
            return radio_obter_cor_preenchimento(figuraImp->rad);
        case TIPO_RETANGULO:
            return retangulo_obter_cor_preenchimento(figuraImp->ret);
        case TIPO_POSTO:
            return posto_obter_cor_preenchimento(figuraImp->pos);
        case TIPO_SEMAFORO:
            return semaforo_obter_cor_preenchimento(figuraImp->sem);
        case TIPO_TEXTO:
            return texto_obter_cor_preenchimento(figuraImp->tex);
        default:
            LOG_ERROR("Tipo de figura inválido passado para obter cor preenchimento: %d!\n",
                      figuraImp->tipo);
            break;
    }
    return NULL;
}

// Define a cor de preenchimento de uma figura.
void figura_definir_cor_preenchimento(Figura figura, const char *cor_preenchimento) {
    if (cor_preenchimento == NULL) {
        LOG_ERROR("Tentando alterar cor de preenchimento de uma figura para nulo!\n");
        return;
    }
    FiguraImp *figuraImp = (FiguraImp *) figura;
    switch (figuraImp->tipo) {
        case TIPO_CIRCULO:
            circulo_definir_cor_preenchimento(figuraImp->cir, cor_preenchimento);
            break;
        case TIPO_HIDRANTE:
            quadra_definir_cor_preenchimento(figuraImp->hid, cor_preenchimento);
            break;
        case TIPO_LINHA:
            linha_definir_cor_preenchimento(figuraImp->lin, cor_preenchimento);
            break;
        case TIPO_QUADRA:
            quadra_definir_cor_preenchimento(figuraImp->qua, cor_preenchimento);
            break;
        case TIPO_RADIO:
            radio_definir_cor_preenchimento(figuraImp->rad, cor_preenchimento);
            break;
        case TIPO_RETANGULO:
            retangulo_definir_cor_preenchimento(figuraImp->ret, cor_preenchimento);
            break;
        case TIPO_POSTO:
            posto_definir_cor_preenchimento(figuraImp->pos, cor_preenchimento);
            break;
        case TIPO_SEMAFORO:
            semaforo_definir_cor_preenchimento(figuraImp->sem, cor_preenchimento);
            break;
        case TIPO_TEXTO:
            texto_definir_cor_preenchimento(figuraImp->tex, cor_preenchimento);
            break;
        default:
            LOG_ERROR("Tipo de figura inválido passado para definir cor preenchimento: %d!\n",
                      figuraImp->tipo);
            break;
    }
}

// Define o arrendondamento da borda de uma figura.
void figura_definir_arredondamento_borda(Figura fig, double raio_borda) {
    FiguraImp *figuraImp = (FiguraImp *) fig;
    switch (figuraImp->tipo) {
        case TIPO_QUADRA:
            quadra_definir_arredondamento_borda(figuraImp->qua, raio_borda);
            break;
        default:
            LOG_ERROR(
                "Tipo de figura inválido passado para definir arredondamento borda figura: %d!\n",
                figuraImp->tipo);
            break;
    }
}

// Libera a memória alocada por uma figura.
void figura_destruir(Figura figura) {
    FiguraImp *figuraImp = (FiguraImp *) figura;
    switch (figuraImp->tipo) {
        case TIPO_CIRCULO:
            circulo_destruir(figuraImp->cir);
            break;
        case TIPO_HIDRANTE:
            quadra_destruir(figuraImp->hid);
            break;
        case TIPO_LINHA:
            linha_destruir(figuraImp->lin);
            break;
        case TIPO_QUADRA:
            quadra_destruir(figuraImp->qua);
            break;
        case TIPO_RADIO:
            radio_destruir(figuraImp->rad);
            break;
        case TIPO_RETANGULO:
            retangulo_destruir(figuraImp->ret);
            break;
        case TIPO_POSTO:
            posto_destruir(figuraImp->pos);
            break;
        case TIPO_SEMAFORO:
            semaforo_destruir(figuraImp->sem);
            break;
        case TIPO_TEXTO:
            texto_destruir(figuraImp->tex);
            break;
        default:
            LOG_ERROR("Tipo de figura inválido passado para destruir figura: %d!\n",
                      figuraImp->tipo);
            break;
    }
    free(figura);
}
