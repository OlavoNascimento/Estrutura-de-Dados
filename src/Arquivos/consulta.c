#include "consulta.h"

#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Estruturas/lista.h"
#include "../Estruturas/pilha.h"
#include "../Estruturas/quadtree.h"
#include "../Estruturas/tabelahash.h"
#include "../Interfaces/figura.h"
#include "../Objetos/EquipamentosUrbanos/caso.h"
#include "../Objetos/EquipamentosUrbanos/estabelecimento.h"
#include "../Objetos/EquipamentosUrbanos/morador.h"
#include "../Objetos/EquipamentosUrbanos/posto.h"
#include "../Objetos/EquipamentosUrbanos/quadra.h"
#include "../Objetos/Formas/circulo.h"
#include "../Objetos/Formas/linha.h"
#include "../Objetos/Formas/poligono.h"
#include "../Objetos/Formas/retangulo.h"
#include "../Objetos/Outros/densidade.h"
#include "../Objetos/Outros/texto.h"
#include "../Ordenacao/quicksort.h"
#include "../Ordenacao/shellsort.h"
#include "../Utils/caminhos.h"
#include "../Utils/graham_scan.h"
#include "../Utils/logging.h"
#include "../Utils/matematica.h"
#include "./svg.h"

// Tamanho maxímo de um comando do arquivo de consulta.
#define TAMANHO_COMANDO 300
// Margem entre o retângulo criado pela função envolver_figuras e as figuras que ele envolve.
#define MARGEM_CONTORNO 2

Retangulo criar_delimitacao_figuras(Figura figura1, Figura figura2) {
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

    Retangulo contorno = retangulo_criar("", largura, altura, x, y, "black", "none");
    return contorno;
}

// Executa o comando o? especificado no arquivo de consulta, verificando se duas figuras se
// sobrepõem.
void checar_interseccao(QuadTree formas, Tabela id_forma, const char *linha, FILE *arquivo_log) {
    char id1[100], id2[100];
    sscanf(linha, "o? %s %s", id1, id2);

    QtNo no1 = tabela_buscar(id_forma, id1);
    QtNo no2 = tabela_buscar(id_forma, id2);
    if (no1 == NULL || no2 == NULL)
        return;
    Figura fig1 = getInfoQt(formas, no1);
    Figura fig2 = getInfoQt(formas, no2);

    const char *tipo_fig1 = figura_obter_tipo(fig1);
    const char *tipo_fig2 = figura_obter_tipo(fig2);
    bool intersectam = false;
    if (strcmp(tipo_fig1, "círculo") == 0 && strcmp(tipo_fig1, tipo_fig2) == 0) {
        intersectam = circulo_checar_interseccao((Circulo) fig1, (Circulo) fig2);
    } else if (strcmp(tipo_fig1, "retângulo") == 0 && strcmp(tipo_fig1, tipo_fig2) == 0) {
        intersectam = retangulo_checar_interseccao((Retangulo) fig1, (Retangulo) fig2);
    } else if (strcmp(tipo_fig1, "círculo") == 0 && strcmp(tipo_fig2, "retângulo") == 0) {
        intersectam = circulo_intersecta_retangulo((Circulo) fig1, (Retangulo) fig2);
    } else if (strcmp(tipo_fig1, "retângulo") == 0 && strcmp(tipo_fig2, "círculo") == 0) {
        intersectam = circulo_intersecta_retangulo((Circulo) fig2, (Retangulo) fig1);
    } else {
        return;
    }

    Retangulo contorno = criar_delimitacao_figuras(fig1, fig2);
    if (intersectam) {
        // Adiciona uma mensagem de sobreposição caso as figuras se intersectem.
        Texto aviso =
            texto_criar("", figura_obter_x_centro(contorno), figura_obter_y_centro(contorno) + 4,
                        "None", "Black", "sobrepoe", true);
        insereQt(formas, ponto_criar_com_figura(aviso), aviso);
    } else {
        // Adiciona traços a borda do retângulo de contorno caso as figuras não se intersectem.
        retangulo_definir_borda_tracejada(contorno, true);
    }
    insereQt(formas, ponto_criar_com_figura(contorno), contorno);

    fprintf(arquivo_log, "o? %s %s\n", id1, id2);
    fprintf(arquivo_log, "%s: %s %s: %s %s\n\n", id1, tipo_fig1, id2, tipo_fig2,
            intersectam ? "SIM" : "NAO");
}

// Cria um círculo com as coordenadas especificadas e com cores que dependem de um valor booleano.
Circulo criar_ponto(double ponto_x, double ponto_y, bool interno) {
    Circulo ponto = circulo_criar("", 1, ponto_x, ponto_y, interno ? "blue" : "magenta",
                                  interno ? "blue" : "magenta");
    return ponto;
}

// Executa o comando i? especificado no arquivo de consulta, verificando se um ponto é interno a uma
// figura, conecta este ponto e a figura utilizando uma linha.
void checar_ponto_interno(QuadTree formas, Tabela id_forma, const char *linha, FILE *arquivo_log) {
    char id[100];
    double ponto_x = 0, ponto_y = 0;
    sscanf(linha, "i? %s %lf %lf", id, &ponto_x, &ponto_y);

    QtNo no = tabela_buscar(id_forma, id);
    if (no == NULL)
        return;
    Figura figura = getInfoQt(formas, no);

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
    insereQt(formas, ponto_criar_com_figura(ponto), ponto);

    Linha ligacao =
        linha_criar(figura_obter_x_centro(ponto), figura_obter_y_centro(ponto),
                    figura_obter_x_centro(figura), figura_obter_y_centro(figura),
                    figura_obter_cor_borda(ponto), figura_obter_cor_preenchimento(ponto), false);
    insereQt(formas, ponto_criar_com_figura(ligacao), ligacao);

    fprintf(arquivo_log, "i? %s %lf %lf\n", id, ponto_x, ponto_y);
    fprintf(arquivo_log, "%s: %s %s\n\n", id, figura_obter_tipo(figura),
            interno ? "INTERNO" : "NAO INTERNO");
}

// Executa o comando pnt especificado no arquivo de consulta, alterando a cor de preenchimento e
// borda da figura com id igual ao id que segue o comando pnt.
void alterar_cor(QuadTree formas, Tabela id_forma, const char *linha, FILE *arquivo_log) {
    char id[100], nova_corb[20], nova_corp[20];
    sscanf(linha, "pnt %s %s %s", id, nova_corb, nova_corp);

    QtNo no = tabela_buscar(id_forma, id);
    if (no == NULL)
        return;
    Figura fig = getInfoQt(formas, no);

    fprintf(arquivo_log, "pnt %s %s %s\n", id, nova_corb, nova_corp);
    fprintf(arquivo_log, "x: %lf, y: %lf\n\n", figura_obter_x(fig), figura_obter_y(fig));

    figura_definir_cor_borda(fig, nova_corb);
    figura_definir_cor_preenchimento(fig, nova_corp);
}

// Executa o comando pnt* especificado no arquivo de consulta, alterando a cor de preenchimento e
// borda de todas as figuras entre id_inicial e id_final (inclusive).
void alterar_cores(QuadTree formas, const char *linha, FILE *arquivo_log) {
    char id_inicial[100], id_final[100], nova_corb[20], nova_corp[20];
    sscanf(linha, "pnt* %s %s %s %s", id_inicial, id_final, nova_corb, nova_corp);

    Lista lista_formas = quadtree_nos_para_lista(formas);
    ListaNo atual = lista_obter_primeiro(lista_formas);

    // Itera até o nó inicial.
    while (atual != NULL) {
        Figura fig = getInfoQt(formas, lista_obter_info(atual));
        const char *id_atual = figura_obter_id(fig);
        if (strcmp(id_atual, id_inicial) == 0)
            break;

        atual = lista_obter_proximo(atual);
    }

    // Itera até o nó final.
    while (atual != NULL) {
        Figura fig = getInfoQt(formas, lista_obter_info(atual));
        const char *id_atual = figura_obter_id(fig);

        fprintf(arquivo_log, "pnt* %s %s %s %s\n", id_inicial, id_final, nova_corb, nova_corp);
        fprintf(arquivo_log, "x: %lf, y: %lf\n\n", figura_obter_x(fig), figura_obter_y(fig));

        figura_definir_cor_borda(fig, nova_corb);
        figura_definir_cor_preenchimento(fig, nova_corp);
        if (strcmp(id_atual, id_final) == 0)
            break;
        atual = lista_obter_proximo(atual);
    }
    lista_destruir(lista_formas);
}

// Executa o comando delf especificado no arquivo de consulta, removendo a figura com id igual ao id
// que segue o comando delf.
void remover_elemento(QuadTree formas, Tabela id_forma, const char *linha, FILE *arquivo_log) {
    char id[100];
    sscanf(linha, "delf %s", id);
    QtNo no = tabela_buscar(id_forma, id);
    if (no == NULL)
        return;
    Figura fig = getInfoQt(formas, no);

    fprintf(arquivo_log, "delf %s\n", id);
    figura_escrever_informacoes(fig, arquivo_log);
    fprintf(arquivo_log, "\n");

    removeNoQt(formas, no);
    tabela_remover(id_forma, figura_obter_id(fig));
    figura_destruir(fig);
}

// Executa o comando delf* especificado pelo arquivo de consulta, removendo todas as figuras que se
// encontrem entre id_inicial e id_final (inclusive).
void remover_elementos(QuadTree formas, Tabela id_forma, const char *linha, FILE *arquivo_log) {
    char id_inicial[100], id_final[100];
    sscanf(linha, "delf* %s %s", id_inicial, id_final);

    Lista lista_formas = quadtree_nos_para_lista(formas);
    ListaNo atual = lista_obter_primeiro(lista_formas);

    // Itera até o nó inicial.
    while (atual != NULL) {
        Figura fig = getInfoQt(formas, lista_obter_info(atual));
        const char *id_atual = figura_obter_id(fig);
        if (strcmp(id_atual, id_inicial) == 0)
            break;
        atual = lista_obter_proximo(atual);
    }

    // Itera até o nó final.
    while (atual != NULL) {
        Figura fig = getInfoQt(formas, lista_obter_info(atual));
        const char *id_atual = figura_obter_id(fig);

        fprintf(arquivo_log, "delf* %s %s\n", id_inicial, id_final);
        figura_escrever_informacoes(fig, arquivo_log);
        fprintf(arquivo_log, "\n");

        ListaNo proximo = lista_obter_proximo(atual);
        if (strcmp(id_atual, id_final) == 0)
            proximo = NULL;

        removeNoQt(formas, lista_obter_info(atual));
        tabela_remover(id_forma, figura_obter_id(fig));

        figura_destruir(fig);
        atual = proximo;
    }
    lista_destruir(lista_formas);
}

void raio_remove_quadras(QuadTree quadras, Tabela cep_quadra, Tabela id_hidrante,
                         Tabela id_semaforo, Tabela id_radio, QuadTree formas, const char *linha,
                         FILE *arquivo_log) {
    double raio;
    double cir_x, cir_y;
    char id[100];
    char c;
    bool remover_quadras = false;

    sscanf(linha, "dq %c", &c);
    if (c == '#') {
        sscanf(linha, "%*s %*c %s %lf ", id, &raio);
    } else {
        sscanf(linha, "%*s %s %lf ", id, &raio);
        remover_quadras = true;
    }

    QtNo no_id = tabela_buscar(id_hidrante, id);
    if (no_id == NULL)
        no_id = tabela_buscar(id_radio, id);
    if (no_id == NULL)
        no_id = tabela_buscar(id_semaforo, id);
    if (no_id == NULL)
        return;
    Figura figura = getInfoQt(formas, no_id);

    cir_x = figura_obter_x_centro(figura);
    cir_y = figura_obter_y_centro(figura);
    Circulo circulo_de_selecao = circulo_criar("", raio, cir_x, cir_y, "black", "none");

    Lista nos_dentro_circulo = nosDentroCirculoQt(quadras, cir_x, cir_y, raio);
    ListaNo atual = lista_obter_primeiro(nos_dentro_circulo);
    while (atual != NULL) {
        ListaNo proximo = lista_obter_proximo(atual);
        Figura quadra = getInfoQt(formas, lista_obter_info(atual));

        bool contido = circulo_contem_retangulo(circulo_de_selecao, quadra);
        if (contido) {
            fprintf(arquivo_log, "id %s: %s, equipamento ", figura_obter_tipo(quadra),
                    figura_obter_id(quadra));
            figura_escrever_informacoes(figura, arquivo_log);
            fprintf(arquivo_log, "\n");

            if (remover_quadras) {
                removeNoQt(quadras, lista_obter_info(atual));
                tabela_remover(cep_quadra, figura_obter_id(quadra));
                figura_destruir(quadra);
            } else {
                figura_definir_cor_borda(quadra, "olive");
                figura_definir_cor_preenchimento(quadra, "beige");
                quadra_definir_arredondamento_borda(quadra, 20);
            }
        }
        atual = proximo;
    }
    lista_destruir(nos_dentro_circulo);

    // Desenhar o raio
    circulo_definir_espessura_borda(circulo_de_selecao, "2px");
    insereQt(formas, ponto_criar_com_figura(circulo_de_selecao), circulo_de_selecao);

    // Desenhar anel de duas cores
    Circulo primeiro_anel = circulo_criar("", 17, cir_x, cir_y, "blue", "none");
    circulo_definir_espessura_borda(primeiro_anel, "5px");
    insereQt(formas, ponto_criar_com_figura(primeiro_anel), primeiro_anel);

    Circulo segundo_anel = circulo_criar("", 12, cir_x, cir_y, "yellow", "none");
    circulo_definir_espessura_borda(segundo_anel, "5px");
    insereQt(formas, ponto_criar_com_figura(segundo_anel), segundo_anel);
}

void remove_equipamento_urbano(QuadTree quadras, Tabela cep_quadra, QuadTree hidrantes,
                               Tabela id_hidrante, QuadTree semaforos, Tabela id_semaforo,
                               QuadTree radios, Tabela id_radio, QuadTree formas, const char *linha,
                               FILE *arquivo_log) {
    char id[100];
    sscanf(linha, "del %s\n", id);

    QtNo no = tabela_buscar(cep_quadra, id);
    QuadTree qt = quadras;
    Tabela tabela = cep_quadra;
    if (no == NULL) {
        no = tabela_buscar(id_hidrante, id);
        qt = hidrantes;
        tabela = id_hidrante;
    }
    if (no == NULL) {
        no = tabela_buscar(id_radio, id);
        qt = radios;
        tabela = id_radio;
    }
    if (no == NULL) {
        no = tabela_buscar(id_semaforo, id);
        qt = semaforos;
        tabela = id_semaforo;
    }
    if (no == NULL)
        return;

    Figura figura = getInfoQt(formas, no);
    double centro_x = figura_obter_x_centro(figura);
    double centro_y = figura_obter_y_centro(figura);

    figura_escrever_informacoes(figura, arquivo_log);
    fprintf(arquivo_log, "\n");

    Linha linha_vertical = linha_criar(centro_x, centro_y, centro_x, 0, "black", "black", false);
    insereQt(formas, ponto_criar_com_figura(linha_vertical), linha_vertical);

    Texto rotulo = texto_criar("", centro_x + 1, 0, "none", "black", id, false);
    insereQt(formas, ponto_criar_com_figura(rotulo), rotulo);

    removeNoQt(qt, no);
    tabela_remover(tabela, figura_obter_id(figura));
    figura_destruir(figura);
}

// Encontra todas as quadras contidas dentro de um círculo e muda a corda da borda e escreve o id no
// arquivo de log.
void circulo_contem_quadras(QuadTree quadras, const char *linha, FILE *arquivo_log) {
    double cir_x, cir_y, raio;
    char cor_borda[20];
    sscanf(linha, "cbq %lf %lf %lf %s", &cir_x, &cir_y, &raio, cor_borda);

    Circulo circulo_de_selecao = circulo_criar("", raio, cir_x, cir_y, "", "");

    Lista nos_contidos = nosDentroCirculoQt(quadras, cir_x, cir_y, raio);
    ListaNo atual = lista_obter_primeiro(nos_contidos);
    while (atual != NULL) {
        Figura quadra = getInfoQt(quadras, lista_obter_info(atual));

        if (circulo_contem_retangulo(circulo_de_selecao, quadra)) {
            figura_definir_cor_borda(quadra, cor_borda);
            fprintf(arquivo_log, "cep: %s\n\n", figura_obter_id(quadra));
        }

        atual = lista_obter_proximo(atual);
    }
    circulo_destruir(circulo_de_selecao);
    lista_destruir(nos_contidos);
}

// Encontra um equipamento urbano em uma lista e escreve suas coordenadas e tipo no arquivo de log.
void informacoes_equipamento_urbano(QuadTree quadras, Tabela cep_quadra, Tabela id_hidrante,
                                    Tabela id_radio, Tabela id_semaforo, const char *linha,
                                    FILE *arquivo_log) {
    char id[100];
    sscanf(linha, "crd? %s", id);

    QtNo no_id = tabela_buscar(cep_quadra, id);
    if (no_id == NULL)
        no_id = tabela_buscar(id_hidrante, id);
    if (no_id == NULL)
        no_id = tabela_buscar(id_radio, id);
    if (no_id == NULL)
        no_id = tabela_buscar(id_semaforo, id);
    if (no_id == NULL)
        return;

    Figura equipamento = getInfoQt(quadras, no_id);
    fprintf(arquivo_log, "tipo: %s, x: %lf, y: %lf\n\n", figura_obter_tipo(equipamento),
            figura_obter_x(equipamento), figura_obter_y(equipamento));
}

// Encontra o total das áreas das quadras contidas dentro de um retângulo
void retangulo_area_total_contida(QuadTree formas, QuadTree quadras, const char *linha,
                                  FILE *arquivo_log) {
    double x, y, largura, altura;
    sscanf(linha, "car %lf %lf %lf %lf", &x, &y, &largura, &altura);

    Retangulo contorno = retangulo_criar("", largura, altura, x, y, "black", "none");
    insereQt(formas, ponto_criar_com_figura(contorno), contorno);

    Lista nos_contidos = nosDentroRetanguloQt(quadras, x, y, x + largura, y + altura);
    if (lista_obter_tamanho(nos_contidos) == 0) {
        lista_destruir(nos_contidos);
        return;
    }

    double area_total = 0;
    ListaNo atual = lista_obter_primeiro(nos_contidos);
    while (atual != NULL) {
        Retangulo ret = getInfoQt(formas, lista_obter_info(atual));

        if (retangulo_contem_retangulo(contorno, ret)) {
            double figura_x_inicio = figura_obter_x_inicio(ret);
            double figura_y_inicio = figura_obter_y_inicio(ret);
            double figura_x_fim = figura_obter_x_fim(ret);
            double figura_y_fim = figura_obter_y_fim(ret);
            double area_figura =
                (figura_x_fim - figura_x_inicio) * (figura_y_fim - figura_y_inicio);
            area_total += area_figura;

            char texto_area_figura[100];
            // Converte o valor da área da figura para string
            snprintf(texto_area_figura, 100, "%lf", area_figura);

            Texto area_quadra =
                texto_criar("", figura_obter_x_centro(ret), figura_obter_y_centro(ret) + 4, "none",
                            "black", texto_area_figura, true);
            insereQt(formas, ponto_criar_com_figura(area_quadra), area_quadra);

            fprintf(arquivo_log, "cep: %s, área: %lf\n\n", figura_obter_id(ret), area_figura);
        }
        atual = lista_obter_proximo(atual);
    }
    lista_destruir(nos_contidos);

    Linha linha_vertical = linha_criar(x, y, x, 0, "black", "black", false);
    insereQt(formas, ponto_criar_com_figura(linha_vertical), linha_vertical);

    char texto_area_total[100];
    // Converte o valor total da área para string
    snprintf(texto_area_total, 100, "%lf", area_total);

    Texto area_linha = texto_criar("", x + 1, 0, "none", "black", texto_area_total, false);
    insereQt(formas, ponto_criar_com_figura(area_linha), area_linha);

    fprintf(arquivo_log, "Área total: %lf\n\n", area_total);
}

void adicionar_caso(QuadTree casos, Tabela cep_quadra, const char *linha) {
    char cep[100];
    sscanf(linha, "cv %*d %s %*c %*d", cep);
    QtNo no = tabela_buscar(cep_quadra, cep);
    if (no != NULL) {
        Quadra quadra_buscada = getInfoQt(casos, no);
        Caso novo_caso = caso_ler(linha, quadra_buscada);
        insereQt(casos, ponto_criar_com_figura(novo_caso), novo_caso);
    }
}

void salvar_info_em_lista(QtInfo info, ExtraInfo lista) {
    lista_inserir_final(lista, info);
}

void postos_mais_proximos(QuadTree postos, Tabela cep_quadra, QuadTree formas, const char *linha,
                          FILE *arquivo_log) {
    int k;
    int numero;
    char face;
    char cep[100];
    sscanf(linha, "soc %d %s %c %d", &k, cep, &face, &numero);

    QtNo no_quadra = tabela_buscar(cep_quadra, cep);
    if (no_quadra == NULL)
        return;

    // Desenhar o caso azul
    Caso caso = caso_criar(k, getInfoQt(formas, no_quadra), face, numero);
    insereQt(formas, ponto_criar_com_figura(caso), caso);

    Lista lista_postos = lista_criar(NULL, NULL);
    percorreLarguraQt(postos, salvar_info_em_lista, lista_postos);
    if (lista_obter_tamanho(lista_postos) == 0) {
        lista_destruir(lista_postos);
        return;
    }

    shellsort(lista_postos, lista_obter_tamanho(lista_postos) / 2, figura_obter_x(caso),
              figura_obter_y(caso));

    ListaNo i = lista_obter_primeiro(lista_postos);
    for (int j = 0; j < k; j++) {
        if (j == 0)
            fprintf(arquivo_log, "Coordenada dos postos: \n");
        Figura posto = lista_obter_info(i);
        Linha linha_posto = linha_criar(figura_obter_x_centro(caso), figura_obter_y_centro(caso),
                                        figura_obter_x_centro(posto), figura_obter_y_centro(posto),
                                        "black", "black", true);
        insereQt(formas, ponto_criar_com_figura(linha_posto), linha_posto);

        fprintf(arquivo_log, "x: %lf, y: %lf\n\n", figura_obter_x(posto), figura_obter_y(posto));

        i = lista_obter_proximo(i);
        if (i == NULL)
            break;
    }
    lista_destruir(lista_postos);
}

// Utiliza um círculo para definir os casos que devem ser contidos por uma envoltória convexa.
void determinar_regiao_de_incidencia(QuadTree formas, QuadTree casos, QuadTree postos,
                                     Lista densidades, const char *linha, FILE *arquivo_log) {
    double x, y, raio;
    sscanf(linha, "ci %lf %lf %lf", &x, &y, &raio);

    // Adiciona o círculo a lista de formas.
    Circulo raio_de_selecao = circulo_criar("", raio, x, y, "green", "none");
    circulo_definir_espessura_borda(raio_de_selecao, "4px");
    insereQt(formas, ponto_criar_com_figura(raio_de_selecao), raio_de_selecao);

    Lista nos_contidos = nosDentroCirculoQt(casos, x, y, raio);
    int total_de_casos = 0;
    bool cabecalho_escrito = false;
    // Filtra a lista de casos, mantendo apenas aqueles que estão totalmente contidos dentro do
    // círculo.
    ListaNo atual = lista_obter_primeiro(nos_contidos);
    while (atual != NULL) {
        Figura caso = getInfoQt(formas, lista_obter_info(atual));
        ListaNo proximo = lista_obter_proximo(atual);

        if (circulo_contem_retangulo(raio_de_selecao, caso)) {
            if (!cabecalho_escrito) {
                fprintf(arquivo_log, "Pontos selecionados pelo círculo: \n");
                cabecalho_escrito = true;
            }

            total_de_casos += caso_obter_numero_de_casos(caso);
            fprintf(arquivo_log, "x: %lf, y: %lf\n", figura_obter_x(caso), figura_obter_y(caso));
        } else {
            lista_remover(nos_contidos, atual);
        }
        atual = proximo;
    }
    if (cabecalho_escrito)
        fprintf(arquivo_log, "\n");

    // Armazena os casos filtrados em um array.
    int tamanho = lista_obter_tamanho(nos_contidos);
    Figura *casos_filtrados = malloc(tamanho * sizeof(Caso));
    if (casos_filtrados == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        lista_destruir(nos_contidos);
        return;
    }
    int j = 0;
    for_each_lista(no_contido, nos_contidos) {
        casos_filtrados[j++] = getInfoQt(formas, lista_obter_info(no_contido));
    }
    lista_destruir(nos_contidos);
    nos_contidos = NULL;

    // Busca o número de habitantes da densidade correspondente.
    double habitantes = 0;
    for_each_lista(den_no, densidades) {
        Densidade regiao = lista_obter_info(den_no);
        if (densidade_contem_ponto(regiao, x, y)) {
            habitantes = densidade_calcular_habitantes(regiao);
            break;
        }
    }

    double incidencia = 0;
    if (habitantes > 0)
        incidencia = (total_de_casos / habitantes) * 100000;

    char categoria = '\0';
    char *cor_poligono = malloc(sizeof *cor_poligono * 8);
    if (cor_poligono == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        free(casos_filtrados);
        return;
    }
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
    fprintf(arquivo_log, "Total de casos: %d\n", total_de_casos);
    fprintf(arquivo_log, "\nCategoria de incidência: %c\n", categoria);

    // Calcula a envoltória convexa.
    Pilha pilha_pontos_envoltoria = graham_scan(tamanho, casos_filtrados);
    free(casos_filtrados);
    casos_filtrados = NULL;
    if (pilha_pontos_envoltoria == NULL) {
        free(cor_poligono);
        return;
    }

    // Aloca uma matriz para armazenar os pontos encontrados.
    const int tamanho_pilha = pilha_obter_tamanho(pilha_pontos_envoltoria);
    double **pontos = malloc(tamanho_pilha * sizeof(double *));
    if (pontos == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        return;
    }
    for (int i = 0; i < tamanho_pilha; i++)
        pontos[i] = malloc(2 * sizeof(*pontos[i]));

    int i = 0;
    // Carrega os pontos encontrados na matriz.
    while (!pilha_esta_vazia(pilha_pontos_envoltoria)) {
        Figura fig = pilha_remover(pilha_pontos_envoltoria);
        pontos[i][0] = figura_obter_x_centro(fig);
        pontos[i][1] = figura_obter_y_centro(fig);
        i++;
    }

    Poligono poligono = poligono_criar(pontos, tamanho_pilha, "red", cor_poligono, 0.4);
    insereQt(formas, ponto_criar_com_figura(poligono), poligono);
    fprintf(arquivo_log, "\nÁrea da envoltória convexa: %lf\n", poligono_calcular_area(poligono));

    // Adiciona um posto de campanha caso necessário.
    if (categoria == 'E') {
        double x_centroide = poligono_obter_x_centro(poligono);
        double y_centroide = poligono_obter_y_centro(poligono);
        Posto centroide = posto_criar(x_centroide, y_centroide);
        insereQt(postos, ponto_criar_com_figura(centroide), centroide);
    }

    free(cor_poligono);
    pilha_destruir(pilha_pontos_envoltoria);
}

// Escreve as informações de todos os moradores de uma quadra no arquivo log.
void listar_moradores_quadra(Tabela cep_quadra, QuadTree moradores, const char *linha,
                             FILE *arquivo_log) {
    char cep[100];
    sscanf(linha, "m? %s", cep);

    QtNo no = tabela_buscar(cep_quadra, cep);
    if (no == NULL) {
        printf("Quadra buscada pelo comando m? não encontrada!\n");
        return;
    }
    Quadra quad = getInfoQt(moradores, no);
    double x_inicio = figura_obter_x_inicio(quad);
    double y_inicio = figura_obter_y_inicio(quad);
    double x_fim = figura_obter_x_fim(quad);
    double y_fim = figura_obter_y_fim(quad);

    Lista nos = nosDentroRetanguloQt(moradores, x_inicio, y_inicio, x_fim, y_fim);
    for_each_lista(i, nos) {
        Figura fig = getInfoQt(moradores, lista_obter_info(i));
        figura_escrever_informacoes(fig, arquivo_log);
    }
    lista_destruir(nos);
}

// Escreve todas as informações de um morador, especificado pelo cpf, no arquivo log. Além disso,
// coloca uma linha vertical do morador até o topo.
void mostrar_informacoes_morador(QuadTree formas, Tabela dados_pessoa, const char *linha,
                                 FILE *arquivo_log) {
    char cpf[100];
    sscanf(linha, "dm? %s", cpf);

    Morador morador = tabela_buscar(dados_pessoa, cpf);
    if (morador == NULL)
        return;
    figura_escrever_informacoes(morador, arquivo_log);
    fprintf(arquivo_log, "\n");

    Linha linha_vertical =
        linha_criar(figura_obter_x_centro(morador), figura_obter_y_centro(morador),
                    figura_obter_x_centro(morador), 0, "black", "black", false);
    insereQt(formas, ponto_criar_com_figura(linha_vertical), linha_vertical);

    char texto_pessoa_endereco[1024];
    snprintf(texto_pessoa_endereco, 1024,
             "cpf: %s, nome: %s %s, cep: %s, face: %c, num: %d, complemento: %s",
             morador_obter_id(morador), morador_obter_nome(morador),
             morador_obter_sobrenome(morador), morador_obter_endereco_cep(morador),
             morador_obter_endereco_face(morador), morador_obter_endereco_num(morador),
             morador_obter_endereco_complemento(morador));

    Texto area_linha = texto_criar("", figura_obter_x_centro(morador) + 1, 0, "none", "black",
                                   texto_pessoa_endereco, false);
    insereQt(formas, ponto_criar_com_figura(area_linha), area_linha);
}

void mostrar_informacoes_estabelecimento(Tabela cnpj_estabelecimento, Tabela dados_pessoa,
                                         const char *linha, FILE *arquivo_log) {
    char cnpj[100];
    sscanf(linha, "de? %s", cnpj);

    Estabelecimento estabelecimento = tabela_buscar(cnpj_estabelecimento, cnpj);
    if (estabelecimento == NULL)
        return;

    Morador morador = tabela_buscar(dados_pessoa, estabelecimento_obter_cpf(estabelecimento));
    if (morador == NULL)
        return;

    figura_escrever_informacoes(estabelecimento, arquivo_log);
    fprintf(arquivo_log, "Dados do proprietário: ");
    figura_escrever_informacoes(morador, arquivo_log);
    fprintf(arquivo_log, "\n");
}

// Cria um arquivo svg com o nome especificado, o qual contem as figuras da quadtree selecionada,
// assim como os ids e coordenadas de cada figura.
void escrever_quadtree_svg(const char *caminho_log, QuadTree quadras, QuadTree hidrantes,
                           QuadTree semaforos, QuadTree radios, const char *linha) {
    char arvore[10];
    char nome[1024];
    sscanf(linha, "dmprbt %s %s", arvore, nome);

    QuadTree qt = NULL;
    if (strcmp(arvore, "q") == 0)
        qt = quadras;
    else if (strcmp(arvore, "h") == 0)
        qt = hidrantes;
    else if (strcmp(arvore, "s") == 0)
        qt = semaforos;
    else if (strcmp(arvore, "t") == 0)
        qt = radios;
    else
        return;

    char *diretorios = extrair_nome_diretorio(caminho_log);
    char *nome_arquivo = alterar_extensao(nome, 1, ".svg");
    char *caminho_arquivo = unir_caminhos(diretorios, nome_arquivo);
    LOG_INFO("Arquivo dmprbt: %s\n", caminho_arquivo);

    Lista lista_dados = quadtree_escrever_svg(qt);

    // Escreve o svg das duas árvores.
    svg_escrever(caminho_arquivo, 1, lista_dados);

    lista_destruir(lista_dados);
    free(caminho_arquivo);
    free(nome_arquivo);
    free(diretorios);
}

void remover_elementos_contidos(QuadTree formas, QuadTree quadras, Tabela cep_quadra,
                                QuadTree hidrantes, Tabela id_hidrante, QuadTree radios,
                                Tabela id_radio, QuadTree semaforos, Tabela id_semaforo,
                                QuadTree moradores, Tabela dados_pessoa, QuadTree estabelecimentos,
                                const char *linha, FILE *arquivo_log) {
    double x, y, raio;
    sscanf(linha, "catac %lf %lf %lf", &x, &y, &raio);

    // Adiciona o círculo a Quadtree formas.
    Circulo raio_selecao = circulo_criar("", raio, x, y, "#6C6753", "#CCFF00");
    circulo_definir_opacidade(raio_selecao, 0.5);
    insereQt(formas, ponto_criar_com_figura(raio_selecao), raio_selecao);

    // Itera por todas as figuras baseadas em retângulos, escrevendo os dados e removendo as figuras
    // que estão contidas no círculo.
    QuadTree retangulos[] = {quadras, semaforos, moradores, estabelecimentos};
    Tabela tabelas_ret[] = {cep_quadra, id_semaforo, dados_pessoa};
    for (int i = 0; i < (int) (sizeof(retangulos) / sizeof(retangulos[0])); i++) {
        Lista nos = nosDentroCirculoQt(retangulos[i], x, y, raio);

        for_each_lista(no, nos) {
            Retangulo ret = getInfoQt(formas, lista_obter_info(no));
            if (circulo_contem_retangulo(raio_selecao, ret)) {
                figura_escrever_informacoes(ret, arquivo_log);
                fprintf(arquivo_log, "\n");

                removeNoQt(retangulos[i], lista_obter_info(no));
                if (i < 3)
                    tabela_remover(tabelas_ret[i], figura_obter_id(ret));
                figura_destruir(ret);
            }
        }
        lista_destruir(nos);
    }

    // Itera por todas as figuras baseadas em círculos, escrevendo os dados e removendo as
    // figuras que estão contidas no círculo.
    QuadTree circulos[] = {hidrantes, radios};
    Tabela tabelas_circ[] = {id_hidrante, id_radio};
    for (int i = 0; i < (int) (sizeof(circulos) / sizeof(circulos[0])); i++) {
        Lista nos = nosDentroCirculoQt(circulos[i], x, y, raio);

        for_each_lista(no, nos) {
            Circulo circ = getInfoQt(formas, lista_obter_info(no));
            if (circulo_contem_circulo(raio_selecao, circ)) {
                figura_escrever_informacoes(circ, arquivo_log);
                fprintf(arquivo_log, "\n");

                removeNoQt(circulos[i], lista_obter_info(no));
                tabela_remover(tabelas_circ[i], figura_obter_id(circ));
                figura_destruir(circ);
            }
        }
        lista_destruir(nos);
    }
}

// Ler o arquivo de consulta localizado no caminho fornecido a função e itera por todas as suas
// linhas, executando funções correspondentes aos comandos.
void consulta_ler(const char *caminho_consulta, const char *caminho_log, Tabela quadtrees,
                  Tabela listas, Tabela relacoes) {
    printf("Lendo consulta\n");
    FILE *arquivo_consulta = fopen(caminho_consulta, "r");
    if (arquivo_consulta == NULL) {
        LOG_ERRO("Falha ao ler arquivo de consulta: %s!\n", caminho_consulta);
        return;
    }
    FILE *arquivo_log = fopen(caminho_log, "w");
    if (arquivo_log == NULL) {
        LOG_ERRO("Falha ao criar arquivo de log: %s!\n", caminho_log);
        return;
    }

    QuadTree formas = tabela_buscar(quadtrees, "formas");
    QuadTree quadras = tabela_buscar(quadtrees, "quadras");
    QuadTree hidrantes = tabela_buscar(quadtrees, "hidrantes");
    QuadTree radios = tabela_buscar(quadtrees, "radios");
    QuadTree semaforos = tabela_buscar(quadtrees, "semaforos");
    QuadTree postos = tabela_buscar(quadtrees, "postos");
    QuadTree casos = tabela_buscar(quadtrees, "casos");
    QuadTree moradores = tabela_buscar(quadtrees, "moradores");
    QuadTree estabelecimentos = tabela_buscar(quadtrees, "estabelecimentos");

    Lista densidades = tabela_buscar(listas, "densidades");

    Tabela dados_pessoa = tabela_buscar(relacoes, "dados_pessoa");
    Tabela cep_quadra = tabela_buscar(relacoes, "cep_quadra");
    Tabela cnpj_estabelecimento = tabela_buscar(relacoes, "cnpj_estabelecimento");
    Tabela id_hidrante = tabela_buscar(relacoes, "id_hidrante");
    Tabela id_semaforo = tabela_buscar(relacoes, "id_semaforo");
    Tabela id_radio = tabela_buscar(relacoes, "id_radio");
    Tabela id_forma = tabela_buscar(relacoes, "id_forma");

    char linha[TAMANHO_COMANDO];
    while (fgets(linha, TAMANHO_COMANDO, arquivo_consulta) != NULL) {
        char comando[TAMANHO_COMANDO];
        sscanf(linha, "%s", comando);

        if (strcmp("o?", comando) == 0) {
            checar_interseccao(formas, id_forma, linha, arquivo_log);
        } else if (strcmp("i?", comando) == 0) {
            checar_ponto_interno(formas, id_forma, linha, arquivo_log);
        } else if (strcmp("pnt", comando) == 0) {
            alterar_cor(formas, id_forma, linha, arquivo_log);
        } else if (strcmp("pnt*", comando) == 0) {
            alterar_cores(formas, linha, arquivo_log);
        } else if (strcmp("delf", comando) == 0) {
            remover_elemento(formas, id_forma, linha, arquivo_log);
        } else if (strcmp("delf*", comando) == 0) {
            remover_elementos(formas, id_forma, linha, arquivo_log);
        } else if (strcmp("dq", comando) == 0) {
            raio_remove_quadras(quadras, cep_quadra, id_hidrante, id_semaforo, id_radio, formas,
                                linha, arquivo_log);
        } else if (strcmp("del", comando) == 0) {
            remove_equipamento_urbano(quadras, cep_quadra, hidrantes, id_hidrante, semaforos,
                                      id_semaforo, radios, id_radio, formas, linha, arquivo_log);
        } else if (strcmp("cbq", comando) == 0) {
            circulo_contem_quadras(quadras, linha, arquivo_log);
        } else if (strcmp("crd?", comando) == 0) {
            informacoes_equipamento_urbano(quadras, cep_quadra, id_hidrante, id_radio, id_semaforo,
                                           linha, arquivo_log);
        } else if (strcmp("car", comando) == 0) {
            retangulo_area_total_contida(formas, quadras, linha, arquivo_log);
        } else if (strcmp("cv", comando) == 0) {
            adicionar_caso(casos, cep_quadra, linha);
        } else if (strcmp("soc", comando) == 0) {
            postos_mais_proximos(postos, cep_quadra, formas, linha, arquivo_log);
        } else if (strcmp("ci", comando) == 0) {
            determinar_regiao_de_incidencia(formas, casos, postos, densidades, linha, arquivo_log);
        } else if (strcmp("m?", comando) == 0) {
            listar_moradores_quadra(cep_quadra, moradores, linha, arquivo_log);
        } else if (strcmp("dm?", comando) == 0) {
            mostrar_informacoes_morador(formas, dados_pessoa, linha, arquivo_log);
        } else if (strcmp("de?", comando) == 0) {
            mostrar_informacoes_estabelecimento(cnpj_estabelecimento, dados_pessoa, linha,
                                                arquivo_log);
        } else if (strcmp("dmprbt", comando) == 0) {
            escrever_quadtree_svg(caminho_log, quadras, hidrantes, semaforos, radios, linha);
        } else if (strcmp("catac", comando) == 0) {
            remover_elementos_contidos(formas, quadras, cep_quadra, hidrantes, id_hidrante, radios,
                                       id_radio, semaforos, id_semaforo, moradores, dados_pessoa,
                                       estabelecimentos, linha, arquivo_log);
        }
    }

    fclose(arquivo_log);
    fclose(arquivo_consulta);
}
