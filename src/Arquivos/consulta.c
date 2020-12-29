#include "consulta.h"

#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Estruturas/lista.h"
#include "../Estruturas/pilha.h"
#include "../Interfaces/figura.h"
#include "../Objetos/EquipamentosUrbanos/caso.h"
#include "../Objetos/EquipamentosUrbanos/posto.h"
#include "../Objetos/Formas/circulo.h"
#include "../Objetos/Formas/linha.h"
#include "../Objetos/Formas/poligono.h"
#include "../Objetos/Formas/retangulo.h"
#include "../Objetos/Outros/densidade.h"
#include "../Objetos/Outros/texto.h"
#include "../Ordenacao/quicksort.h"
#include "../Ordenacao/shellsort.h"
#include "../Utils/logging.h"
#include "../Utils/matematica.h"

// Tamanho maxímo de um comando do arquivo de consulta.
#define LINHA_MAX 300
// Margem entre o retângulo criado pela função envolver_figuras e as figuras que ele envolve.
#define MARGEM_CONTORNO 2
// Tamanho estimado para cada letra de um texto
#define MARGEM_LETRAS_TEXTO 4

Retangulo *criar_delimitacao_figuras(Figura figura1, Figura figura2) {
    // Coordenada x do contorno é a menor coordenada x entre as duas figuras.
    double x =
        min(figura_obter_x_inicio(figura1), figura_obter_x_inicio(figura2)) - MARGEM_CONTORNO;
    // Coordenada y do contorno é a menor coordenada y entre as duas figuras.
    double y =
        min(figura_obter_y_inicio(figura1), figura_obter_y_inicio(figura2)) - MARGEM_CONTORNO;

    // Largura do contorno é a distância entre o x do contorno e a coordenada x onde o fim da
    // figura mais longe se encontra.
    double largura =
        max(figura_obter_x_fim(figura1), figura_obter_x_fim(figura2)) - x + MARGEM_CONTORNO;
    // Altura do contorno é a distância entre o y do contorno e a coordenada y onde o fim da figura
    // mais longe se encontra.
    double altura =
        max(figura_obter_y_fim(figura1), figura_obter_y_fim(figura2)) - y + MARGEM_CONTORNO;

    Retangulo contorno = retangulo_criar("", x, y, largura, altura, "black", "none");
    return contorno;
}

// Executa o comando o? especificado no arquivo de consulta, verificando se duas figuras se
// sobrepõem.
void checar_interseccao(Lista lista_formas, const char *linha, FILE *arquivo_log) {
    char id1[100], id2[100];
    sscanf(linha, "o? %s %s", id1, id2);

    No no1 = lista_buscar(lista_formas, id1);
    No no2 = lista_buscar(lista_formas, id2);
    if (no1 == NULL || no2 == NULL)
        return;
    Figura fig1 = lista_obter_figura(no1);
    Figura fig2 = lista_obter_figura(no2);

    const char *tipo_fig1 = figura_obter_tipo(fig1);
    const char *tipo_fig2 = figura_obter_tipo(fig2);
    bool intersectam = false;
    if (strcmp(tipo_fig1, "círculo") == 0 && strcmp(tipo_fig1, tipo_fig2) == 0) {
        intersectam = circulo_checar_interseccao(fig1, fig2);
    } else if (strcmp(tipo_fig1, "retângulo") == 0 && strcmp(tipo_fig1, tipo_fig2) == 0) {
        intersectam = retangulo_checar_interseccao(fig1, fig2);
    } else if (strcmp(tipo_fig1, "círculo") == 0 && strcmp(tipo_fig2, "retângulo") == 0) {
        intersectam = circulo_intersecta_retangulo(fig1, fig2);
    } else if (strcmp(tipo_fig1, "retângulo") == 0 && strcmp(tipo_fig2, "círculo") == 0) {
        intersectam = circulo_intersecta_retangulo(fig2, fig1);
    } else {
        return;
    }

    Retangulo contorno = criar_delimitacao_figuras(fig1, fig2);

    if (intersectam) {
        // Adiciona uma mensagem de sobreposição caso as figuras se intersectem.
        Texto aviso =
            texto_criar("", figura_obter_x_centro(contorno), figura_obter_y_centro(contorno),
                        "None", "Black", "sobrepoe", true);
        lista_inserir_final(lista_formas, aviso);
    } else {
        // Adiciona traços a borda do retângulo de contorno caso as figuras não se intersectem.
        retangulo_definir_borda_tracejada(contorno, true);
    }
    lista_inserir_final(lista_formas, contorno);

    fprintf(arquivo_log, "o? %s %s\n", id1, id2);
    fprintf(arquivo_log, "%s: %s %s: %s %s\n\n", id1, figura_obter_tipo(fig1), id2,
            figura_obter_tipo(fig2), intersectam ? "SIM" : "NAO");
}

// Cria um círculo com as coordenadas especificadas e com cores que dependem de um valor booleano.
Circulo criar_ponto(double ponto_x, double ponto_y, bool interno) {
    Circulo ponto = circulo_criar("", 1, ponto_x, ponto_y, interno ? "blue" : "magenta",
                                  interno ? "blue" : "magenta");
    return ponto;
}

// Executa o comando i? especificado no arquivo de consulta, verificando se um ponto é interno a uma
// figura, conecta este ponto e a figura utilizando uma linha.
void checar_ponto_interno(Lista lista_formas, const char *linha, FILE *arquivo_log) {
    char id[100];
    double ponto_x = 0, ponto_y = 0;
    sscanf(linha, "i? %s %lf %lf", id, &ponto_x, &ponto_y);

    No no = lista_buscar(lista_formas, id);
    if (no == NULL)
        return;
    Figura figura = lista_obter_figura(no);

    const char *tipo_figura = figura_obter_tipo(figura);
    bool interno = false;
    if (strcmp(tipo_figura, "círculo") == 0) {
        interno = circulo_checar_ponto_interno(figura, ponto_x, ponto_y);
    } else if (strcmp(tipo_figura, "retângulo") == 0) {
        interno = retangulo_checar_ponto_interno(figura, ponto_x, ponto_y);
    } else {
        return;
    }

    Circulo ponto = criar_ponto(ponto_x, ponto_y, interno);
    lista_inserir_final(lista_formas, ponto);

    Linha ligacao =
        linha_criar(figura_obter_x_centro(ponto), figura_obter_y_centro(ponto),
                    figura_obter_x_centro(figura), figura_obter_y_centro(figura),
                    figura_obter_cor_borda(ponto), figura_obter_cor_preenchimento(ponto), false);
    lista_inserir_final(lista_formas, ligacao);

    fprintf(arquivo_log, "i? %s %lf %lf\n", id, ponto_x, ponto_y);
    fprintf(arquivo_log, "%s: %s %s\n\n", id, figura_obter_tipo(figura),
            interno ? "INTERNO" : "NAO INTERNO");
}

// Executa o comando pnt especificado no arquivo de consulta, alterando a cor de preenchimento e
// borda da figura com id igual ao id que segue o comando pnt.
void alterar_cor(Lista lista_formas, const char *linha, FILE *arquivo_log) {
    char id[100], nova_corb[20], nova_corp[20];
    sscanf(linha, "pnt %s %s %s", id, nova_corb, nova_corp);

    No no = lista_buscar(lista_formas, id);
    if (no == NULL)
        return;
    Figura fig = lista_obter_figura(no);

    fprintf(arquivo_log, "pnt %s %s %s\n", id, nova_corb, nova_corp);
    fprintf(arquivo_log, "x: %lf, y: %lf\n\n", figura_obter_x(fig), figura_obter_y(fig));

    figura_definir_cor_borda(fig, nova_corb);
    figura_definir_cor_preenchimento(fig, nova_corp);
}

// Executa o comando pnt* especificado no arquivo de consulta, alterando a cor de preenchimento e
// borda de todas as figuras entre id_inicial e id_final (inclusive).
void alterar_cores(Lista lista_formas, const char *linha, FILE *arquivo_log) {
    char id_inicial[100], id_final[100], nova_corb[20], nova_corp[20];
    sscanf(linha, "pnt* %s %s %s %s", id_inicial, id_final, nova_corb, nova_corp);
    No atual = lista_buscar(lista_formas, id_inicial);
    if (atual == NULL)
        return;

    while (atual != NULL) {
        Figura fig = lista_obter_figura(atual);
        const char *id_atual = figura_obter_id(fig);

        fprintf(arquivo_log, "pnt* %s %s %s %s\n", id_inicial, id_final, nova_corb, nova_corp);
        fprintf(arquivo_log, "x: %lf, y: %lf\n\n", figura_obter_x(fig), figura_obter_y(fig));

        figura_definir_cor_borda(fig, nova_corb);
        figura_definir_cor_preenchimento(fig, nova_corp);
        if (strcmp(id_atual, id_final) == 0)
            break;
        atual = lista_obter_proximo(atual);
    }
}

// Executa o comando delf especificado no arquivo de consulta, removendo a figura com id igual ao id
// que segue o comando delf.
void remover_elemento(Lista lista_formas, const char *linha, FILE *arquivo_log) {
    char id[100];
    sscanf(linha, "delf %s", id);
    No no = lista_buscar(lista_formas, id);
    if (no == NULL)
        return;
    Figura fig = lista_obter_figura(no);

    fprintf(arquivo_log, "delf %s\n", id);
    figura_escrever_informacoes(fig, arquivo_log);
    fprintf(arquivo_log, "\n");
    lista_remover(lista_formas, no);
}

// Executa o comando delf* especificado pelo arquivo de consulta, removendo todas as figuras que se
// encontrem entre id_inicial e id_final (inclusive).
void remover_elementos(Lista lista_formas, const char *linha, FILE *arquivo_log) {
    char id_inicial[100], id_final[100];
    sscanf(linha, "delf* %s %s", id_inicial, id_final);

    No atual = lista_buscar(lista_formas, id_inicial);
    No proximo = lista_obter_proximo(atual);
    while (atual != NULL) {
        Figura fig = lista_obter_figura(atual);
        const char *id_atual = figura_obter_id(fig);

        fprintf(arquivo_log, "delf* %s %s\n", id_inicial, id_final);
        figura_escrever_informacoes(fig, arquivo_log);
        fprintf(arquivo_log, "\n");

        proximo = lista_obter_proximo(atual);
        if (strcmp(id_atual, id_final) == 0)
            proximo = NULL;

        lista_remover(lista_formas, atual);
        atual = proximo;
    }
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

    sscanf(linha, "dq %c", &c);
    if (c == '#') {
        sscanf(linha, "%*s %*c %s %lf ", id, &raio);
    } else {
        sscanf(linha, "%*s %s %lf ", id, &raio);
        remover_quadras = true;
    }

    No figura_id = lista_buscar(lista_hidrantes, id);
    if (figura_id == NULL)
        figura_id = lista_buscar(lista_radios, id);
    if (figura_id == NULL)
        figura_id = lista_buscar(lista_semaforos, id);
    if (figura_id == NULL)
        return;
    figura = lista_obter_figura(figura_id);
    cir_x = figura_obter_x_centro(figura);
    cir_y = figura_obter_y_centro(figura);
    Circulo circulo_de_selecao = circulo_criar("", raio, cir_x, cir_y, "", "");

    No atual = lista_obter_primeiro(lista_quadras);
    while (atual != NULL) {
        Figura quadra = lista_obter_figura(atual);

        atual = lista_obter_proximo(atual);

        bool contido = circulo_contem_retangulo(circulo_de_selecao, quadra);

        if (contido) {
            fprintf(arquivo_log, "id %s: %s, equipamento ", figura_obter_tipo(quadra),
                    figura_obter_id(quadra));
            figura_escrever_informacoes(figura, arquivo_log);
            fprintf(arquivo_log, "\n");
            if (remover_quadras) {
                No no_remove = lista_buscar(lista_quadras, figura_obter_id(quadra));
                lista_remover(lista_quadras, no_remove);
            } else {
                figura_definir_cor_borda(quadra, "olive");
                figura_definir_cor_preenchimento(quadra, "beige");
                retangulo_definir_arredondamento_borda(quadra, 20);
            }
        }
    }
    circulo_destruir(circulo_de_selecao);

    // Desenhar o raio
    Circulo desenho_raio = circulo_criar("", raio, cir_x, cir_y, "black", "none");
    circulo_definir_espessura_borda(desenho_raio, "2px");
    lista_inserir_final(lista_formas, desenho_raio);

    // Desenhar anel de duas cores
    Circulo primeiro_anel = circulo_criar("", 17, cir_x, cir_y, "blue", "none");
    circulo_definir_espessura_borda(primeiro_anel, "5px");
    lista_inserir_final(lista_formas, primeiro_anel);

    Circulo segundo_anel = circulo_criar("", 12, cir_x, cir_y, "yellow", "none");
    circulo_definir_espessura_borda(segundo_anel, "5px");
    lista_inserir_final(lista_formas, segundo_anel);
}

void remover_equipamento_criar_linha(No no, char id[100], Lista lista_formas, FILE *arquivo_log) {
    Figura figura = lista_obter_figura(no);
    double centro_x = figura_obter_x_centro(figura);
    double centro_y = figura_obter_y_centro(figura);

    figura_escrever_informacoes(figura, arquivo_log);
    fprintf(arquivo_log, "\n");

    Linha linha_vertical = linha_criar(centro_x, centro_y, centro_x, 0, "black", "black", false);
    lista_inserir_final(lista_formas, linha_vertical);

    Texto rotulo = texto_criar("", centro_x + 1, 0, "none", "black", id, false);
    lista_inserir_final(lista_formas, rotulo);
}

void remove_equipamento_urbano(const char *linha, Lista *lista_quadras, Lista *lista_hidrantes,
                               Lista *lista_semaforos, Lista *lista_radios, Lista *lista_formas,
                               FILE *arquivo_log) {
    char id[100];
    sscanf(linha, "del %s\n", id);

    No figura_no = lista_buscar(lista_quadras, id);
    if (figura_no != NULL) {
        remover_equipamento_criar_linha(figura_no, id, lista_formas, arquivo_log);
        lista_remover(lista_quadras, figura_no);
        return;
    }

    figura_no = lista_buscar(lista_hidrantes, id);
    if (figura_no != NULL) {
        remover_equipamento_criar_linha(figura_no, id, lista_formas, arquivo_log);
        lista_remover(lista_hidrantes, figura_no);
        return;
    }

    figura_no = lista_buscar(lista_radios, id);
    if (figura_no != NULL) {
        remover_equipamento_criar_linha(figura_no, id, lista_formas, arquivo_log);
        lista_remover(lista_radios, figura_no);
        return;
    }

    figura_no = lista_buscar(lista_semaforos, id);
    if (figura_no != NULL) {
        remover_equipamento_criar_linha(figura_no, id, lista_formas, arquivo_log);
        lista_remover(lista_semaforos, figura_no);
        return;
    }
}

// Encontra todas as quadras contidas dentro de um círculo e muda a corda da borda e escreve o id no
// arquivo de log.
void circulo_contem_quadras(Lista *lista_quadras, const char *linha, FILE *arquivo_log) {
    double cir_x, cir_y, raio;
    char cor_borda[20];
    sscanf(linha, "cbq %lf %lf %lf %s", &cir_x, &cir_y, &raio, cor_borda);

    Circulo circulo_de_selecao = circulo_criar("", raio, cir_x, cir_y, "", "");
    No atual = lista_obter_primeiro(lista_quadras);
    while (atual != NULL) {
        Figura quadra = lista_obter_figura(atual);

        if (circulo_contem_retangulo(circulo_de_selecao, quadra)) {
            figura_definir_cor_borda(quadra, cor_borda);
            fprintf(arquivo_log, "cep: %s\n\n", figura_obter_id(quadra));
        }

        atual = lista_obter_proximo(atual);
    }
    circulo_destruir(circulo_de_selecao);
}

// Encontra um equipamento urbano em uma lista e escreve suas coordenadas e tipo no arquivo de log.
void informacoes_equipamento_urbano(Lista lista_quadras, Lista lista_hidrantes, Lista lista_radios,
                                    Lista lista_semaforos, const char *linha, FILE *arquivo_log) {
    char id[100];
    sscanf(linha, "crd? %s", id);

    No no_id = lista_buscar(lista_quadras, id);
    if (no_id == NULL)
        no_id = lista_buscar(lista_hidrantes, id);
    if (no_id == NULL)
        no_id = lista_buscar(lista_radios, id);
    if (no_id == NULL)
        no_id = lista_buscar(lista_semaforos, id);
    if (no_id == NULL)
        return;

    Figura equipamento = lista_obter_figura(no_id);
    fprintf(arquivo_log, "tipo: %s, x: %lf, y: %lf\n\n", figura_obter_tipo(equipamento),
            figura_obter_x(equipamento), figura_obter_y(equipamento));
}

// Encontra o total das áreas das quadras contidas dentro de um retângulo
void retangulo_area_total_contida(Lista lista_formas, Lista lista_quadras, const char *linha,
                                  FILE *arquivo_log) {
    double x, y, largura, altura;
    sscanf(linha, "car %lf %lf %lf %lf", &x, &y, &largura, &altura);

    double area_total = 0;

    No atual = lista_obter_primeiro(lista_quadras);
    while (atual != NULL) {
        Figura figura = lista_obter_figura(atual);
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
            lista_inserir_final(lista_formas, contorno);

            char texto_area_figura[100];
            // Converte o valor da área da figura para string
            snprintf(texto_area_figura, 100, "%lf", area_figura);

            Texto area_quadra =
                texto_criar("", figura_obter_x_centro(figura), figura_obter_y_centro(figura),
                            "none", "black", texto_area_figura, true);
            lista_inserir_final(lista_formas, area_quadra);

            fprintf(arquivo_log, "cep: %s, área: %lf\n\n", figura_obter_id(figura), area_figura);
        }
        atual = lista_obter_proximo(atual);
    }

    Linha linha_vertical = linha_criar(x, y, x, 0, "black", "black", false);
    lista_inserir_final(lista_formas, linha_vertical);

    char texto_area_total[100];
    // Converte o valor total da área para string
    snprintf(texto_area_total, 100, "%lf", area_total);

    Texto area_linha = texto_criar("", x + 1, 0, "none", "black", texto_area_total, false);
    lista_inserir_final(lista_formas, area_linha);

    fprintf(arquivo_log, "Área total: %lf\n\n", area_total);
}

void postos_mais_proximos(Lista lista_postos, Lista lista_quadras, Lista lista_formas,
                          const char *linha, FILE *arquivo_log) {
    int k;
    int numero;
    char face;
    char cep[100];
    sscanf(linha, "soc %d %s %c %d", &k, cep, &face, &numero);

    // Desenhar o caso azul
    Caso caso = caso_criar(k, cep, face, numero, "white", "blue", lista_quadras);
    if (caso == NULL)
        return;
    lista_inserir_final(lista_formas, caso);

    if (lista_obter_tamanho(lista_postos) == 0) {
        LOG_INFO("Nenhum posto encontrado!\n");
        return;
    }
    shellsort(lista_postos, lista_obter_tamanho(lista_postos) / 2, figura_obter_x(caso),
              figura_obter_y(caso));

    No i = lista_obter_primeiro(lista_postos);
    for (int j = 0; j < k; j++) {
        if (j == 0)
            fprintf(arquivo_log, "Coordenada dos postos: \n");
        Figura posto = lista_obter_figura(i);
        Linha linha_posto = linha_criar(figura_obter_x_centro(caso), figura_obter_y_centro(caso),
                                        figura_obter_x_centro(posto), figura_obter_y_centro(posto),
                                        "black", "black", true);
        lista_inserir_final(lista_formas, linha_posto);

        fprintf(arquivo_log, "x: %lf, y: %lf\n\n", figura_obter_x(posto), figura_obter_y(posto));

        i = lista_obter_proximo(i);
        if (i == NULL)
            break;
    }
}

// Verifica se uma curva é no sentido anti-horário.
double checar_ante_horario(Figura a, Figura b, Figura c) {
    double area =
        (figura_obter_x(b) - figura_obter_x(a)) * (figura_obter_y(c) - figura_obter_y(a)) -
        (figura_obter_y(b) - figura_obter_y(a)) * (figura_obter_x(c) - figura_obter_x(a));
    // Anti-horário
    if (area > 0)
        return -1;
    // Horário
    if (area < 0)
        return 1;
    // Pontos são colineares
    return 0;
}

// Devolve uma pilha com os pontos de uma envoltória convexa.
// A lista deve ser liberada pelo o usuário!
Pilha graham_scan(Lista lista_casos) {
    // Não é possível formar uma envoltória convexa com menos de 3 pontos.
    if (lista_obter_tamanho(lista_casos) < 3) {
        LOG_INFO("Menos que 3 casos, não existe envoltória!\n");
        return NULL;
    }

    No no_min = NULL;
    double min_y = DBL_MAX;
    // Encontra o caso com menor y.
    for (No i = lista_obter_primeiro(lista_casos); i != NULL; i = lista_obter_proximo(i)) {
        if (figura_obter_y(lista_obter_figura(i)) < min_y) {
            no_min = i;
            min_y = figura_obter_y(lista_obter_figura(i));
        } else if (figura_obter_y(lista_obter_figura(i)) == min_y &&
                   figura_obter_x(lista_obter_figura(i)) <
                       figura_obter_x(lista_obter_figura(no_min))) {
            no_min = i;
            min_y = figura_obter_y(lista_obter_figura(i));
        }
    }
    if (no_min == NULL) {
        LOG_ERRO("Nenhum elemento maior encontrado!\n");
        return NULL;
    }

    // Move a figura com menor y para a primeira posição da lista.
    lista_trocar_figuras(no_min, lista_obter_primeiro(lista_casos));

    // Ordena a lista de casos de acordo como o ângulo formado com o ponto mínimo.
    quicksort(lista_obter_figura(lista_obter_primeiro(lista_casos)),
              lista_obter_proximo(lista_obter_primeiro(lista_casos)),
              lista_obter_ultimo(lista_casos));

    Pilha pontos_envoltoria = pilha_criar();
    // Primeiro elemento está sempre dentro da envoltória.
    pilha_inserir(pontos_envoltoria, lista_obter_figura(lista_obter_primeiro(lista_casos)));
    // Segundo elemento precisa ser verificado.
    No segundo_elemento = lista_obter_proximo(lista_obter_primeiro(lista_casos));
    pilha_inserir(pontos_envoltoria, lista_obter_figura(segundo_elemento));

    for (No i = lista_obter_proximo(segundo_elemento); i != NULL; i = lista_obter_proximo(i)) {
        Figura proximo_caso = lista_obter_figura(i);
        Figura ultimo_caso = pilha_remover(pontos_envoltoria);

        // Remove casos até encontrar uma curva no sentido anti-horário.
        while (!pilha_esta_vazia(pontos_envoltoria) &&
               checar_ante_horario(pilha_obter_topo(pontos_envoltoria), ultimo_caso,
                                   proximo_caso) <= 0) {
            ultimo_caso = pilha_remover(pontos_envoltoria);
        }

        pilha_inserir(pontos_envoltoria, ultimo_caso);
        pilha_inserir(pontos_envoltoria, proximo_caso);
    }

    Figura ultimo_ponto = pilha_remover(pontos_envoltoria);
    // Verifica se o último ponto é inválido.
    if (checar_ante_horario(pilha_obter_topo(pontos_envoltoria), ultimo_ponto,
                            lista_obter_figura(lista_obter_primeiro(lista_casos))) > 0) {
        pilha_inserir(pontos_envoltoria, ultimo_ponto);
    }

    return pontos_envoltoria;
}

// Libera a memória alocada pela função determinar_regiao_de_incidencia.
void liberar_regiao_incidencia(Lista lista_casos_filtrados, char *cor_poligono) {
    // Libera a memória alocada.
    No atual = lista_obter_primeiro(lista_casos_filtrados);
    No proximo = NULL;
    while (atual != NULL) {
        proximo = lista_obter_proximo(atual);
        free(atual);
        atual = proximo;
    }
    free(lista_casos_filtrados);
    free(cor_poligono);
}

// Utiliza um círculo para definir os casos que devem ser contidos por uma envoltória convexa.
void determinar_regiao_de_incidencia(Lista lista_formas, Lista lista_densidades, Lista lista_casos,
                                     Lista lista_postos, const char *linha, FILE *arquivo_log) {
    double x, y, raio;
    sscanf(linha, "ci %lf %lf %lf", &x, &y, &raio);

    // Adiciona o círculo a lista de formas.
    Circulo raio_de_selecao = circulo_criar("", raio, x, y, "green", "none");
    circulo_definir_espessura_borda(raio_de_selecao, "4px");
    lista_inserir_final(lista_formas, raio_de_selecao);

    Lista lista_casos_filtrados = lista_criar();
    int total_de_casos = 0;
    // Filtra a lista de casos, mantendo apenas aqueles que estão totalmente contidos dentro do
    // círculo.
    for (No i = lista_obter_primeiro(lista_casos); i != NULL; i = lista_obter_proximo(i)) {
        if (i == lista_obter_primeiro(lista_casos))
            fprintf(arquivo_log, "Pontos selecionados pelo círculo: \n");

        // Distância entre o círculo e o ponto x mais longe do círculo
        Figura caso = lista_obter_figura(i);
        double dx = max(x - figura_obter_x_centro(caso), figura_obter_x_centro(caso) - x);
        // Distância entre o círculo e o ponto y mais longe do círculo
        double dy = max(y - figura_obter_y_centro(caso), figura_obter_y_centro(caso) - y);
        if (dx * dx + dy * dy < raio * raio) {
            lista_inserir_final(lista_casos_filtrados, caso);
            total_de_casos += caso_obter_numero_de_casos(caso);
            fprintf(arquivo_log, "x: %lf, y: %lf\n", figura_obter_x(caso), figura_obter_y(caso));
        }
    }

    double habitantes = densidade_buscar_coordenada(lista_densidades, x, y);
    double incidencia = (total_de_casos / habitantes) * 100000;
    char categoria = '\0';
    char *cor_poligono = malloc(8 * sizeof(char));
    cor_poligono[0] = '\0';

    // Define a categoria e cor do polígono baseado na incidência.
    if (incidencia < 0.1) {
        categoria = 'A';
        strcpy(cor_poligono, "#00FFFF");
    } else if (incidencia < 5) {
        categoria = 'B';
        strcpy(cor_poligono, "#008080");
    } else if (incidencia < 10) {
        categoria = 'C';
        strcpy(cor_poligono, "#FFFF00");
    } else if (incidencia < 20) {
        categoria = 'D';
        strcpy(cor_poligono, "#FF0000");
    } else {
        categoria = 'E';
        strcpy(cor_poligono, "#800080");
    }
    // Escreve as informações no arquivo de log
    fprintf(arquivo_log, "\nTotal de casos: %d\n", total_de_casos);
    fprintf(arquivo_log, "\nCategoria de incidência: %c\n", categoria);

    // Calcula a envoltória convexa.
    Pilha pilha_pontos_envoltoria = graham_scan(lista_casos_filtrados);
    if (pilha_pontos_envoltoria == NULL) {
        liberar_regiao_incidencia(lista_casos_filtrados, cor_poligono);
        return;
    }

    // Aloca uma matriz para armazenar os pontos encontrados.
    double **pontos = malloc(pilha_obter_tamanho(pilha_pontos_envoltoria) * sizeof(double *));
    for (int i = 0; i < pilha_obter_tamanho(pilha_pontos_envoltoria); i++)
        pontos[i] = malloc(2 * sizeof(*pontos[i]));

    int i = 0;
    // Carrega os pontos encontrados na matriz.
    while (!pilha_esta_vazia(pilha_pontos_envoltoria)) {
        Figura fig = pilha_remover(pilha_pontos_envoltoria);
        pontos[i][0] = figura_obter_x_centro(fig);
        pontos[i][1] = figura_obter_y_centro(fig);
        i++;
    }

    // Cria o polígono com os pontos encontrados e adiciona a lista de formas.
    Poligono poligono = poligono_criar(pontos, i, "red", cor_poligono, 0.4);
    lista_inserir_final(lista_formas, poligono);
    // Escreve a área do polígono no arquivo de log.
    fprintf(arquivo_log, "\nÁrea da envoltória convexa: %lf\n", poligono_calcular_area(poligono));

    // Adiciona um posto de campanha caso necessário.
    if (categoria == 'E') {
        double x_centroide = poligono_obter_x_centro(poligono);
        double y_centroide = poligono_obter_y_centro(poligono);
        Posto centroide = posto_criar(x_centroide, y_centroide);
        lista_inserir_final(lista_postos, centroide);
    }
    liberar_regiao_incidencia(lista_casos_filtrados, cor_poligono);
    pilha_destruir(pilha_pontos_envoltoria);
}

// Ler o arquivo de consulta localizado no caminho fornecido a função e itera por todas as suas
// linhas, executando funções correspondentes aos comandos.
void consulta_ler(const char *caminho_qry, const char *caminho_log, Lista lista_formas,
                  Lista lista_quadras, Lista lista_hidrantes, Lista lista_radios,
                  Lista lista_semaforos, Lista lista_postos, Lista lista_densidades,
                  Lista lista_casos) {
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
        } else if (strcmp("cv", comando) == 0) {
            Caso novo_caso = caso_ler(linha, lista_quadras);
            lista_inserir_final(lista_casos, novo_caso);
        } else if (strcmp("soc", comando) == 0) {
            postos_mais_proximos(lista_postos, lista_quadras, lista_formas, linha, arquivo_log);
        } else if (strcmp("ci", comando) == 0) {
            determinar_regiao_de_incidencia(lista_formas, lista_densidades, lista_casos,
                                            lista_postos, linha, arquivo_log);
        }
    }

    fclose(arquivo_log);
    fclose(arquivo_consulta);
}
