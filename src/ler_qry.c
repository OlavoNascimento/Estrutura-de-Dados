#include "ler_qry.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "circulo.h"
#include "figuras.h"
#include "linha.h"
#include "lista.h"
#include "logging.h"
#include "matematica.h"
#include "retangulo.h"
#include "texto.h"

// Tamanho maxímo de um comando do arquivo de consulta.
#define LINHA_MAX 300
// Determina o tamanho e espaçamento da borda do retângulo que envolve duas figuras.
#define CONTORNO_TRACEJADO_TAMANHO 2
// Margem entre o retângulo criado pela função envolver_figuras e as figuras que ele envolve.
#define MARGEM_CONTORNO 2
// Tamanho estimado para cada letra de um texto
#define MARGEM_LETRAS_TEXTO 4

// Cria o texto "sobrepoe" no centro de um retângulo.
Figura criar_texto_sobreposicao(Figura contorno) {
    Texto aviso = texto_criar("", figura_obter_centro_x(contorno), figura_obter_centro_y(contorno),
                              "None", "Black", "sobrepoe");

    return figura_criar(aviso, TIPO_TEXTO);
}

// Cria um retângulo com coordenadas e dimensões necessárias para envolver duas figuras.
Figura *criar_delimitacao_figuras(Figura figura1, Figura figura2) {
    Retangulo contorno = retangulo_criar(
        "", 0, 0,
        // Coordenada x do contorno é a menor coordenada x entre as duas figuras.
        min(figura_obter_x_inicio(figura1), figura_obter_x_inicio(figura2)) - MARGEM_CONTORNO,
        // Coordenada y do contorno é a menor coordenada y entre as duas figuras.
        min(figura_obter_y_inicio(figura1), figura_obter_y_inicio(figura2)) - MARGEM_CONTORNO,
        "black", "none");

    // Largura do contorno é a distancia entre o x do contorno e a coordenada x onde o fim da figura
    // mais longe se encontra.
    retangulo_definir_largura(contorno,
                              max(figura_obter_x_fim(figura1), figura_obter_x_fim(figura2)) -
                                  retangulo_obter_x(contorno) + 2 * MARGEM_CONTORNO);
    // Altura do contorno é a distancia entre o y do contorno e a coordenada y onde o fim da figura
    // mais longe se encontra.
    retangulo_definir_altura(contorno,
                             max(figura_obter_y_fim(figura1), figura_obter_y_fim(figura2)) -
                                 retangulo_obter_y(contorno) + 2 * MARGEM_CONTORNO);
    return figura_criar(contorno, TIPO_RETANGULO);
}

// Executa o comando o? especificado no arquivo de consulta, verificando se um ponto é interno a uma
// figura, conecta este ponto e a figura utilizando uma linha.
void checar_interseccao(Lista lista, const char *linha, FILE *arquivo_log) {
    char id1[100], id2[100];
    sscanf(linha, "o? %s %s", id1, id2);

    No no1 = lista_get_no(lista, id1);
    No no2 = lista_get_no(lista, id2);
    if (no1 == NULL || no2 == NULL)
        return;
    Figura fig1 = lista_get_figura(no1);
    Figura fig2 = lista_get_figura(no2);

    bool intersectam = figura_checar_interseccao(fig1, fig2);

    Figura contorno = criar_delimitacao_figuras(fig1, fig2);

    if (intersectam) {
        // Adiciona uma mensagem de sobreposição caso as figuras se intersectem.
        Figura aviso = criar_texto_sobreposicao(contorno);
        lista_insert_final(lista, aviso);
    } else {
        // Adiciona traços a borda do retângulo de contorno caso as figuras não se intersectem.
        retangulo_definir_espassamento_borda(figura_obter_figura(contorno),
                                             CONTORNO_TRACEJADO_TAMANHO);
    }
    lista_insert_final(lista, contorno);

    fprintf(arquivo_log, "o? %s %s\n", id1, id2);
    fprintf(arquivo_log, "%s: %s %s: %s %s\n\n", id1, figura_obter_string_tipo(fig1), id2,
            figura_obter_string_tipo(fig2), intersectam ? "SIM" : "NAO");
}

// Cria um círculo com as coordenadas especificadas e com cores que dependem de um valor booleano.
Figura criar_ponto(double ponto_x, double ponto_y, bool interno) {
    Circulo ponto = circulo_criar("", 1, ponto_x, ponto_y, interno ? "blue" : "magenta",
                                  interno ? "blue" : "magenta");
    return figura_criar(ponto, TIPO_CIRCULO);
}

// Cria uma linha que conecta um ponto ao centro de uma figura
Figura criar_ligacao_ponto_figura(Circulo ponto, Figura figura) {
    Linha ligacao =
        linha_criar(circulo_obter_x(ponto), circulo_obter_y(ponto), figura_obter_centro_x(figura),
                    figura_obter_centro_y(figura), circulo_obter_cor_borda(ponto),
                    circulo_obter_cor_preenchimento(ponto));
    return figura_criar(ligacao, TIPO_LINHA);
}

// Executa o comando i? especificado no arquivo de consulta, verificando se um ponto é interno a uma
// figura, conecta este ponto e a figura utilizando uma linha.
void checar_ponto_interno(Lista lista, const char *linha, FILE *arquivo_log) {
    char id[100];
    double ponto_x = 0, ponto_y = 0;
    sscanf(linha, "i? %s %lf %lf", id, &ponto_x, &ponto_y);

    No no = lista_get_no(lista, id);
    if (no == NULL)
        return;
    Figura fig = lista_get_figura(no);

    bool interno = figura_checar_ponto_interno(fig, ponto_x, ponto_y);

    Figura ponto = criar_ponto(ponto_x, ponto_y, interno);
    lista_insert_final(lista, ponto);

    Figura ligacao = criar_ligacao_ponto_figura(figura_obter_figura(ponto), fig);
    lista_insert_final(lista, ligacao);

    fprintf(arquivo_log, "i? %s %lf %lf\n", id, ponto_x, ponto_y);
    fprintf(arquivo_log, "%s: %s %s\n\n", id, figura_obter_string_tipo(fig),
            interno ? "INTERNO" : "NAO INTERNO");
}

// Executa o comando pnt especificado no arquivo de consulta, alterando a cor de preenchimento e
// borda da figura com id igual ao id que segue o comando pnt.
void alterar_cor(Lista lista, const char *linha, FILE *arquivo_log) {
    char id[100], nova_corb[20], nova_corp[20];
    sscanf(linha, "pnt %s %s %s", id, nova_corb, nova_corp);

    No no = lista_get_no(lista, id);
    if (no == NULL)
        return;
    Figura fig = lista_get_figura(no);

    const char *atual_corp = figura_obter_cor_preenchimento(fig);
    const char *atual_corb = figura_obter_cor_borda(fig);

    fprintf(arquivo_log, "pnt %s %s %s\n", id, nova_corb, nova_corp);
    fprintf(arquivo_log, "corb: %s, corp: %s\n\n", atual_corb, atual_corp);

    figura_definir_cor_borda(fig, nova_corb);
    figura_definir_cor_preenchimento(fig, nova_corp);
}

// Executa o comando pnt* especificado no arquivo de consulta, alterando a cor de preenchimento e
// borda de todas as figuras entre id_inicial e id_final (inclusive).
void alterar_cores(Lista lista, const char *linha, FILE *arquivo_log) {
    char id_inicial[100], id_final[100], nova_corb[20], nova_corp[20];
    sscanf(linha, "pnt* %s %s %s %s", id_inicial, id_final, nova_corb, nova_corp);
    No atual = lista_get_no(lista, id_inicial);
    if (atual == NULL)
        return;

    while (atual != NULL) {
        Figura fig = lista_get_figura(atual);
        const char *id_atual = figura_obter_id(fig);
        const char *atual_corp = figura_obter_cor_preenchimento(fig);
        const char *atual_corb = figura_obter_cor_borda(fig);

        fprintf(arquivo_log, "pnt* %s %s %s %s\n", id_inicial, id_final, nova_corb, nova_corp);
        fprintf(arquivo_log, "corb: %s, corp: %s\n\n", atual_corb, atual_corp);

        figura_definir_cor_borda(fig, nova_corb);
        figura_definir_cor_preenchimento(fig, nova_corp);
        if (strcmp(id_atual, id_final) == 0)
            break;
        atual = lista_get_next(atual);
    }
}

// Executa o comando delf especificado no arquivo de consulta, removendo a figura com id igual ao id
// que segue o comando delf.
void remover_elemento(Lista lista, const char *linha, FILE *arquivo_log) {
    char id[100];
    sscanf(linha, "delf %s", id);
    No no = lista_get_no(lista, id);
    if (no == NULL)
        return;
    Figura fig = lista_get_figura(no);

    fprintf(arquivo_log, "delf %s\n", id);
    figura_escrever_informacoes(arquivo_log, fig);
    fprintf(arquivo_log, "\n");
    lista_remove_no(lista, no);
}

// Executa o comando delf* especificado pelo arquivo de consulta, removendo todas as figuras que se
// encontrem entre id_inicial e id_final (inclusive).
void remover_elementos(Lista lista, const char *linha, FILE *arquivo_log) {
    char id_inicial[100], id_final[100];
    sscanf(linha, "delf* %s %s", id_inicial, id_final);

    No atual = lista_get_no(lista, id_inicial);
    No proximo = lista_get_next(atual);
    while (atual != NULL) {
        Figura fig = lista_get_figura(atual);
        const char *id_atual = figura_obter_id(fig);

        fprintf(arquivo_log, "delf* %s %s\n", id_inicial, id_final);
        figura_escrever_informacoes(arquivo_log, fig);
        fprintf(arquivo_log, "\n");

        proximo = lista_get_next(atual);
        if (strcmp(id_atual, id_final) == 0)
            proximo = NULL;

        lista_remove_no(lista, atual);
        atual = proximo;
    }
}

bool circulo_contem_retangulo(Figura retangulo, double cir_x, double cir_y, double raio) {
    // Distância entre o círculo e o ponto x mais longe do círculo
    double dx =
        max(cir_x - figura_obter_x_inicio(retangulo), figura_obter_x_fim(retangulo) - cir_x);
    // Distância entre o círculo e o ponto y mais longe do círculo
    double dy =
        max(cir_y - figura_obter_y_inicio(retangulo), figura_obter_y_fim(retangulo) - cir_y);
    return dx * dx + dy * dy < raio * raio;
}

void raio_remove_quadras(Lista *lista_quadras, Lista *lista_hidrantes, Lista *lista_semaforos,
                         Lista *lista_radios, Lista *lista_formas, const char *linha,
                         FILE *arquivo_log) {
    double raio;
    double cir_x, cir_y;
    char id[100];
    char c;
    Figura figura;
    bool remover_quadras = false;

    sscanf(linha, "%*s %c", &c);
    if (c == '#') {
        sscanf(linha, "%*s %*c %s %lf ", id, &raio);
    } else {
        sscanf(linha, "%*s %s %lf ", id, &raio);
        remover_quadras = true;
    }

    No figura_id = lista_get_no(lista_hidrantes, id);
    if (figura_id == NULL)
        figura_id = lista_get_no(lista_radios, id);
    if (figura_id == NULL)
        figura_id = lista_get_no(lista_semaforos, id);
    if (figura_id == NULL)
        return;
    figura = lista_get_figura(figura_id);
    cir_x = figura_obter_centro_x(figura);
    cir_y = figura_obter_centro_y(figura);

    No atual = lista_get_first(lista_quadras);
    while (atual != NULL) {
        Figura quadra = lista_get_figura(atual);

        atual = lista_get_next(atual);

        bool contido = circulo_contem_retangulo(quadra, cir_x, cir_y, raio);
        if (contido) {
            fprintf(arquivo_log, "id %s: %s, equipamento ", figura_obter_string_tipo(quadra),
                    figura_obter_id(quadra));
            figura_escrever_informacoes(arquivo_log, figura);
            fprintf(arquivo_log, "\n");
            if (remover_quadras) {
                No no_remove = lista_get_no(lista_quadras, figura_obter_id(quadra));
                lista_remove_no(lista_quadras, no_remove);
            } else {
                figura_definir_cor_borda(quadra, "olive");
                figura_definir_cor_preenchimento(quadra, "beige");
                figura_definir_arredondamento_borda(quadra, 20);
            }
        }
    }

    // Desenhar o raio
    Figura nova_figura;
    Circulo desenho_raio = circulo_criar("", raio, cir_x, cir_y, "black", "none");
    circulo_definir_espessura_borda(desenho_raio, "2px");
    nova_figura = figura_criar(desenho_raio, TIPO_CIRCULO);
    lista_insert_final(lista_formas, nova_figura);

    // Desenhar anel de duas cores
    // Primeiro anel
    desenho_raio = circulo_criar("", 17, cir_x, cir_y, "blue", "none");
    circulo_definir_espessura_borda(desenho_raio, "5px");
    nova_figura = figura_criar(desenho_raio, TIPO_CIRCULO);
    lista_insert_final(lista_formas, nova_figura);

    // Segundo anel
    desenho_raio = circulo_criar("", 12, cir_x, cir_y, "yellow", "none");
    circulo_definir_espessura_borda(desenho_raio, "5px");
    nova_figura = figura_criar(desenho_raio, TIPO_CIRCULO);
    lista_insert_final(lista_formas, nova_figura);
}

void remove_equipamento_urbano(const char *linha, Lista *lista_quadras, Lista *lista_hidrantes,
                               Lista *lista_semaforos, Lista *lista_radios, Lista *lista_formas,
                               FILE *arquivo_log) {
    char id[100];
    sscanf(linha, "del %s\n", id);
    No figura_id;
    Figura figura;
    TiposFigura tipo;

    figura_id = lista_get_no(lista_quadras, id);
    if (figura_id == NULL)
        figura_id = lista_get_no(lista_hidrantes, id);
    if (figura_id == NULL)
        figura_id = lista_get_no(lista_radios, id);
    if (figura_id == NULL)
        figura_id = lista_get_no(lista_semaforos, id);
    if (figura_id == NULL)
        return;
    figura = lista_get_figura(figura_id);

    double centro_x = figura_obter_centro_x(figura);
    double centro_y = figura_obter_centro_y(figura);

    figura_escrever_informacoes(arquivo_log, figura);
    fprintf(arquivo_log, "\n");

    tipo = figura_obter_tipo(figura);
    switch (tipo) {
        case TIPO_SEMAFORO:
            lista_remove_no(lista_semaforos, figura_id);
            break;
        case TIPO_QUADRA:
            lista_remove_no(lista_quadras, figura_id);
            break;
        case TIPO_RADIO:
            lista_remove_no(lista_radios, figura_id);
            break;
        case TIPO_HIDRANTE:
            lista_remove_no(lista_hidrantes, figura_id);
            break;
        default:
            break;
    }

    Linha linha_vertical = linha_criar(centro_x, centro_y, centro_x, 0, "black", "black");
    Figura nova_figura = figura_criar(linha_vertical, TIPO_LINHA);
    lista_insert_final(lista_formas, nova_figura);

    Texto rotulo = texto_criar("", centro_x + 1, 0, "black", "black", id);
    Figura texto_figura = figura_criar(rotulo, TIPO_TEXTO);
    lista_insert_final(lista_formas, texto_figura);
}

// Encontra todas as quadras contidas dentro de um círculo e muda a corda da borda e escreve o id no
// arquivo de log
void circulo_contem_quadras(Lista *lista_quadras, const char *linha, FILE *arquivo_log) {
    double cir_x, cir_y, raio;
    char cor_borda[20];
    sscanf(linha, "cbq %lf %lf %lf %s", &cir_x, &cir_y, &raio, cor_borda);

    No atual = lista_get_first(lista_quadras);
    while (atual != NULL) {
        Figura quadra = lista_get_figura(atual);

        if (circulo_contem_retangulo(quadra, cir_x, cir_y, raio)) {
            figura_definir_cor_borda(quadra, cor_borda);
            fprintf(arquivo_log, "cep: %s\n\n", figura_obter_id(quadra));
        }

        atual = lista_get_next(atual);
    }
}

// Encontra um equipamento urbano em uma lista e escreve suas coordenadas e tipo no arquivo de log.
void informacoes_equipamento_urbano(Lista lista_quadras, Lista lista_hidrantes, Lista lista_radios,
                                    Lista lista_semaforos, const char *linha, FILE *arquivo_log) {
    char id[100];
    sscanf(linha, "crd? %s", id);

    No no_id = lista_get_no(lista_quadras, id);
    if (no_id == NULL)
        no_id = lista_get_no(lista_hidrantes, id);
    if (no_id == NULL)
        no_id = lista_get_no(lista_radios, id);
    if (no_id == NULL)
        no_id = lista_get_no(lista_semaforos, id);
    if (no_id == NULL)
        return;

    Figura equipamento = lista_get_figura(no_id);
    fprintf(arquivo_log, "tipo: %s, x: %lf, y: %lf\n\n", figura_obter_string_tipo(equipamento),
            figura_obter_x_inicio(equipamento), figura_obter_y_inicio(equipamento));
}

// Encontra o total das áreas das quadras contidas dentro de um retângulo
void retangulo_area_total_contida(Lista lista_formas, Lista lista_quadras, const char *linha,
                                  FILE *arquivo_log) {
    double x, y, largura, altura;
    sscanf(linha, "car %lf %lf %lf %lf", &x, &y, &largura, &altura);

    double area_total = 0;

    No atual = lista_get_first(lista_quadras);
    while (atual != NULL) {
        Figura figura = lista_get_figura(atual);
        double figura_x_inicio = figura_obter_x_inicio(figura);
        double figura_y_inicio = figura_obter_y_inicio(figura);
        double figura_x_fim = figura_obter_x_fim(figura);
        double figura_y_fim = figura_obter_y_fim(figura);

        if (figura_x_inicio >= x && figura_y_inicio >= y && figura_x_fim <= x + largura &&
            figura_y_fim <= y + altura) {
            double area_figura =
                (figura_x_fim - figura_x_inicio) * (figura_y_fim - figura_y_inicio);
            area_total += area_figura;

            Retangulo contorno = retangulo_criar("", largura, altura, x, y, "black", "none");
            Figura fig_contorno = figura_criar(contorno, TIPO_RETANGULO);
            lista_insert_final(lista_formas, fig_contorno);

            char texto_area_figura[100];
            // Converte o valor da área da figura para string
            snprintf(texto_area_figura, 100, "%lf", area_figura);

            Texto area_quadra =
                texto_criar("", figura_obter_centro_x(figura), figura_obter_centro_y(figura),
                            "black", "black", texto_area_figura);
            Figura fig_area_quadra = figura_criar(area_quadra, TIPO_TEXTO);
            lista_insert_final(lista_formas, fig_area_quadra);

            fprintf(arquivo_log, "cep: %s, área: %lf\n\n", figura_obter_id(figura), area_figura);
        }
        atual = lista_get_next(atual);
    }

    Linha linha_vertical = linha_criar(x, y, x, 0, "black", "black");
    Figura fig_linha = figura_criar(linha_vertical, TIPO_LINHA);
    lista_insert_final(lista_formas, fig_linha);

    char texto_area_total[100];
    // Converte o valor total da área para string
    snprintf(texto_area_total, 100, "%lf", area_total);

    Texto area_linha = texto_criar("", x + 1, 0, "black", "black", texto_area_total);
    Figura fig_area_linha = figura_criar(area_linha, TIPO_TEXTO);
    lista_insert_final(lista_formas, fig_area_linha);

    fprintf(arquivo_log, "área total: %lf\n\n", area_total);
}

// Ler o arquivo de consulta localizado no caminho fornecido a função e itera por todas as suas
// linhas, executando funções correspondentes aos comandos.
void ler_qry(const char *caminho_qry, const char *caminho_log, Lista lista_formas,
             Lista lista_quadras, Lista lista_hidrantes, Lista lista_radios, Lista lista_semaforos,
             Lista lista_postos) {
    FILE *arquivo_consulta = fopen(caminho_qry, "r");
    if (arquivo_consulta == NULL) {
        fprintf(stderr, "ERRO: Falha ao ler arquivo de consulta: %s!\n", caminho_qry);
        return;
    }
    FILE *arquivo_log = fopen(caminho_log, "w");
    if (arquivo_log == NULL) {
        fprintf(stderr, "ERRO: Falha ao criar arquivo de log: %s!\n", caminho_log);
        return;
    }

    char linha[LINHA_MAX];
    while (fgets(linha, LINHA_MAX, arquivo_consulta) != NULL) {
        char comando[LINHA_MAX];
        sscanf(linha, "%s", comando);
        if (strcmp("o?", comando) == 0) {
            checar_interseccao(lista_formas, linha, arquivo_log);
        } else if (strcmp("i?", comando) == 0) {
            checar_ponto_interno(lista_formas, linha, arquivo_log);
        } else if (strcmp("pnt", comando) == 0) {
            alterar_cor(lista_formas, linha, arquivo_log);
        } else if (strcmp("pnt*", comando) == 0) {
            alterar_cores(lista_formas, linha, arquivo_log);
        } else if (strcmp("delf", comando) == 0) {
            remover_elemento(lista_formas, linha, arquivo_log);
        } else if (strcmp("delf*", comando) == 0) {
            remover_elementos(lista_formas, linha, arquivo_log);
        } else if (strcmp("dq", comando) == 0) {
            raio_remove_quadras(lista_quadras, lista_hidrantes, lista_semaforos, lista_radios,
                                lista_formas, linha, arquivo_log);
        } else if (strcmp("del", comando) == 0) {
            remove_equipamento_urbano(linha, lista_quadras, lista_hidrantes, lista_semaforos,
                                      lista_radios, lista_formas, arquivo_log);
        } else if (strcmp("cbq", comando) == 0) {
            circulo_contem_quadras(lista_quadras, linha, arquivo_log);
        } else if (strcmp("crd?", comando) == 0) {
            informacoes_equipamento_urbano(lista_quadras, lista_hidrantes, lista_radios,
                                           lista_semaforos, linha, arquivo_log);
        } else if (strcmp("car", comando) == 0) {
            retangulo_area_total_contida(lista_formas, lista_quadras, linha, arquivo_log);
        }
    }

    fclose(arquivo_log);
    fclose(arquivo_consulta);
}
