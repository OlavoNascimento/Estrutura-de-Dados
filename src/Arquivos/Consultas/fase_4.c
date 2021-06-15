#include "fase_4.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Estruturas/grafo.h"
#include "../../Estruturas/quadtree.h"
#include "../../Objetos/EquipamentosUrbanos/estabelecimento.h"
#include "../../Objetos/EquipamentosUrbanos/morador.h"
#include "../../Objetos/EquipamentosUrbanos/quadra.h"
#include "../../Objetos/Formas/circulo.h"
#include "../../Objetos/Formas/linha.h"
#include "../../Objetos/Formas/retangulo.h"
#include "../../Objetos/Outros/texto.h"
#include "../../Utils/caminhos.h"
#include "../../Utils/matematica.h"
#include "../svg.h"

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
    Quadra quad = quadtree_obter_info(no);
    double x_inicio = figura_obter_x_inicio(quad);
    double y_inicio = figura_obter_y_inicio(quad);
    double x_fim = figura_obter_x_fim(quad);
    double y_fim = figura_obter_y_fim(quad);

    Lista nos = quadtree_nos_dentro_retangulo(moradores, x_inicio, y_inicio, x_fim, y_fim);
    for_each_lista(i, nos) {
        Morador morador = quadtree_obter_info(lista_obter_info(i));
        morador_escrever_informacoes(morador, arquivo_log);
        fprintf(arquivo_log, "\n");
    }
    lista_destruir(nos);
}

// Escreve todas as informações de um morador, especificado pelo cpf, no arquivo log. Além disso,
// coloca uma linha vertical do morador até o topo.
void mostrar_informacoes_morador(Lista formas, Tabela dados_pessoa, const char *linha,
                                 FILE *arquivo_log) {
    char cpf[100];
    sscanf(linha, "dm? %s", cpf);

    Morador morador = tabela_buscar(dados_pessoa, cpf);
    if (morador == NULL)
        return;
    morador_escrever_informacoes(morador, arquivo_log);
    fprintf(arquivo_log, "\n");

    Linha linha_vertical =
        linha_criar(figura_obter_x_centro(morador), figura_obter_y_centro(morador),
                    figura_obter_x_centro(morador), 0, "black");
    lista_inserir_final(formas, linha_vertical);

    char texto_pessoa_endereco[1024];
    snprintf(texto_pessoa_endereco, 1024,
             "cpf: %s, nome: %s %s, cep: %s, face: %c, num: %d, complemento: %s",
             morador_obter_id(morador), morador_obter_nome(morador),
             morador_obter_sobrenome(morador), morador_obter_endereco_cep(morador),
             morador_obter_endereco_face(morador), morador_obter_endereco_num(morador),
             morador_obter_endereco_complemento(morador));

    Texto texto_endereco = texto_criar("", figura_obter_x_centro(morador) + 1, 0, "none", "black",
                                       texto_pessoa_endereco);
    lista_inserir_final(formas, texto_endereco);
}

// Escreve no arquivo de log todos os dados do estabelecimento comercial identificado por um cnpj.
void mostrar_informacoes_estabelecimento(Tabela cnpj_estabelecimento, Tabela dados_pessoa,
                                         Tabela tipo_descricao, const char *linha,
                                         FILE *arquivo_log) {
    char cnpj[100];
    sscanf(linha, "de? %s", cnpj);

    Estabelecimento est = tabela_buscar(cnpj_estabelecimento, cnpj);
    if (est == NULL)
        return;
    estabelecimento_escrever_informacoes(est, arquivo_log);

    const char *descricao = tabela_buscar(tipo_descricao, estabelecimento_obter_tipo(est));
    if (descricao != NULL)
        fprintf(arquivo_log, "Descrição do tipo: %s\n", descricao);
    else
        fprintf(arquivo_log, "O tipo do estabelecimento não está relacionado a uma descrição!\n");

    Morador morador = tabela_buscar(dados_pessoa, estabelecimento_obter_cpf(est));
    if (morador != NULL) {
        fprintf(arquivo_log, "Dados do proprietário: ");
        morador_escrever_informacoes(morador, arquivo_log);
    } else {
        fprintf(arquivo_log,
                "O cpf do proprietário do estabelecimento não está relacionado a um "
                "morador!\n");
    }
    fprintf(arquivo_log, "\n");
}

// Altera o endereço de um morador e escreve no arquivo de log.
void mudar_endereco_morador(Lista formas, Tabela cep_quadra, Tabela dados_pessoa, const char *linha,
                            FILE *arquivo_log) {
    char cpf[100];
    char cep[100];
    char complemento[100];
    char face;
    int num;
    sscanf(linha, "mud %s %s %c %d %s", cpf, cep, &face, &num, complemento);

    Morador morador = tabela_buscar(dados_pessoa, cpf);
    if (morador == NULL)
        return;

    QtNo no_novo = tabela_buscar(cep_quadra, cep);
    if (no_novo == NULL)
        return;
    Quadra quadra_nova = quadtree_obter_info(no_novo);

    morador_escrever_informacoes(morador, arquivo_log);
    fprintf(arquivo_log, "Endereço antigo: cep: %s, face: %c, número: %d, complemento: %s\n",
            morador_obter_endereco_cep(morador), morador_obter_endereco_face(morador),
            morador_obter_endereco_num(morador), morador_obter_endereco_complemento(morador));
    fprintf(arquivo_log, "Novo endereço: cep: %s, face: %c, número: %d, complemento: %s\n\n", cep,
            face, num, complemento);

    double centro_x_atual = figura_obter_x_centro(morador);
    double centro_y_atual = figura_obter_y_centro(morador);

    morador_definir_endereco(morador, cep, face, num, complemento, quadra_nova);

    double centro_x_novo = figura_obter_x_centro(morador);
    double centro_y_novo = figura_obter_y_centro(morador);

    Linha linha_enderecos =
        linha_criar(centro_x_atual, centro_y_atual, centro_x_novo, centro_y_novo, "red");
    linha_definir_espessura(linha_enderecos, 5);
    lista_inserir_final(formas, linha_enderecos);

    // Cria círculo no endereço atual
    Circulo circulo_atual = circulo_criar("", 4, centro_x_atual, centro_y_atual, "white", "red");
    circulo_definir_espessura_borda(circulo_atual, 3);
    lista_inserir_final(formas, circulo_atual);

    // Cria círculo no endereço novo
    Circulo circulo_novo = circulo_criar("", 4, centro_x_novo, centro_y_novo, "white", "blue");
    circulo_definir_espessura_borda(circulo_novo, 3);
    lista_inserir_final(formas, circulo_novo);
}

// Cria um arquivo svg com o nome especificado, o qual contem a representação da quadtree
// selecionada com os ids e coordenadas de cada figura.
void escrever_quadtree_svg(const char *caminho_log, QuadTree quadras, QuadTree hidrantes,
                           QuadTree semaforos, QuadTree radios, const char *linha) {
    char arvore[10];
    char sufixo[1024];
    sscanf(linha, "dmprbt %s %s", arvore, sufixo);

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
    char *nome_arquivo = alterar_extensao(caminho_log, 3, "-", sufixo, ".svg");
    char *caminho_arquivo = unir_caminhos(diretorios, nome_arquivo);
    printf("Arquivo dmprbt: %s\n", caminho_arquivo);

    Lista lista_formas = quadtree_escrever_svg(qt);
    svg_escrever(caminho_arquivo, 1, lista_formas);

    lista_destruir(lista_formas);
    free(diretorios);
    free(nome_arquivo);
    free(caminho_arquivo);
}

// Destaca os estabelecimentos do tipo especificado e escreve suas informações no arquivo de log.
void destacar_estabelecimentos_contidos(Tabela dados_pessoa, QuadTree estabelecimentos,
                                        const char *linha, FILE *arquivo_log) {
    char tipo[100];
    double x, y, largura, altura;
    bool todos_os_tipos = false;
    sscanf(linha, "eplg? %s %lf %lf %lf %lf", tipo, &x, &y, &largura, &altura);

    if (strcmp(tipo, "*") == 0)
        todos_os_tipos = true;

    Retangulo contorno = retangulo_criar("", largura, altura, x, y, "none", "none");

    Lista nos_contidos =
        quadtree_nos_dentro_retangulo(estabelecimentos, x, y, x + largura, y + altura);
    for_each_lista(no, nos_contidos) {
        Estabelecimento est = quadtree_obter_info(lista_obter_info(no));
        if (!todos_os_tipos && strcmp(estabelecimento_obter_tipo(est), tipo) != 0)
            continue;
        if (!retangulo_contem_retangulo(contorno, (Retangulo) est))
            continue;

        estabelecimento_escrever_informacoes(est, arquivo_log);
        Morador morador = tabela_buscar(dados_pessoa, estabelecimento_obter_cpf(est));
        if (morador != NULL) {
            fprintf(arquivo_log, "Nome do proprietário: %s %s\n", morador_obter_nome(morador),
                    morador_obter_sobrenome(morador));
        } else {
            fprintf(arquivo_log,
                    "O cpf do proprietário do estabelecimento não está relacionado a um "
                    "morador!\n");
        }
        fprintf(arquivo_log, "\n");

        estabelecimento_definir_cor_preenchimento(est, "red");
        estabelecimento_definir_borda_tracejada(est, true);
    }
    retangulo_destruir(contorno);
    lista_destruir(nos_contidos);
}

// Remove as quadras, elementos urbanos, moradores e estabelecimentos que estejam inteiramente
// contidos na circunferência especificada.
void remover_elementos_contidos(Lista formas, Tabela quadtrees, Tabela relacoes, Tabela grafos,
                                const char *linha, FILE *arquivo_log) {
    double x, y, raio;
    sscanf(linha, "catac %lf %lf %lf", &x, &y, &raio);

    QuadTree quadras = tabela_buscar(quadtrees, "quadras");
    QuadTree hidrantes = tabela_buscar(quadtrees, "hidrantes");
    QuadTree radios = tabela_buscar(quadtrees, "radios");
    QuadTree semaforos = tabela_buscar(quadtrees, "semaforos");
    QuadTree moradores = tabela_buscar(quadtrees, "moradores");
    QuadTree estabelecimentos = tabela_buscar(quadtrees, "estabelecimentos");
    QuadTree qt_vias = tabela_buscar(quadtrees, "vias");

    Tabela dados_pessoa = tabela_buscar(relacoes, "dados_pessoa");
    Tabela cep_quadra = tabela_buscar(relacoes, "cep_quadra");
    Tabela cnpj_estabelecimento = tabela_buscar(relacoes, "cnpj_estabelecimento");
    Tabela id_hidrante = tabela_buscar(relacoes, "id_hidrante");
    Tabela id_semaforo = tabela_buscar(relacoes, "id_semaforo");
    Tabela id_radio = tabela_buscar(relacoes, "id_radio");

    Grafo vias = tabela_buscar(grafos, "vias");

    Circulo raio_selecao = circulo_criar("", raio, x, y, "#6C6753", "#CCFF00");
    circulo_definir_opacidade(raio_selecao, 0.5);
    lista_inserir_final(formas, raio_selecao);

    // Itera por todas as figuras baseadas em retângulos.
    QuadTree retangulos[] = {quadras, semaforos, moradores, estabelecimentos};
    Tabela tabelas_ret[] = {cep_quadra, id_semaforo, dados_pessoa, cnpj_estabelecimento};
    for (int i = 0; i < (int) (sizeof(retangulos) / sizeof(retangulos[0])); i++) {
        Lista nos = quadtree_nos_dentro_circulo(retangulos[i], x, y, raio);

        for_each_lista(no, nos) {
            Retangulo ret = quadtree_obter_info(lista_obter_info(no));
            if (!circulo_contem_retangulo(raio_selecao, ret))
                continue;

            figura_escrever_informacoes(ret, arquivo_log);
            fprintf(arquivo_log, "\n");

            quadtree_remover_no(retangulos[i], lista_obter_info(no));
            tabela_remover(tabelas_ret[i], figura_obter_id(ret));
            figura_destruir(ret);
        }
        lista_destruir(nos);
    }

    // Itera por todas as figuras baseadas em círculos.
    QuadTree circulos[] = {hidrantes, radios};
    Tabela tabelas_circ[] = {id_hidrante, id_radio};
    for (int i = 0; i < (int) (sizeof(circulos) / sizeof(circulos[0])); i++) {
        Lista nos = quadtree_nos_dentro_circulo(circulos[i], x, y, raio);

        for_each_lista(no, nos) {
            Circulo circ = quadtree_obter_info(lista_obter_info(no));
            if (!circulo_contem_circulo(raio_selecao, circ))
                continue;

            figura_escrever_informacoes(circ, arquivo_log);
            fprintf(arquivo_log, "\n");

            quadtree_remover_no(circulos[i], lista_obter_info(no));
            tabela_remover(tabelas_circ[i], figura_obter_id(circ));
            figura_destruir(circ);
        }
        lista_destruir(nos);
    }

    // Itera por todos os vértices.
    Lista nos = quadtree_nos_dentro_circulo(qt_vias, x, y, raio);
    for_each_lista(no, nos) {
        Vertice vertice = quadtree_obter_info(lista_obter_info(no));
        fprintf(arquivo_log, "tipo: vértice, id: %s, x: %lf, y: %lf\n", vertice_obter_id(vertice),
                vertice_obter_x(vertice), vertice_obter_y(vertice));
        fprintf(arquivo_log, "\n");

        quadtree_remover_no(qt_vias, lista_obter_info(no));
        grafo_remover_vertice(vias, vertice_obter_id(vertice));
        vertice_destruir(vertice);
    }

    // Remove o grafo de ciclovia já que está desatualizado.
    if (lista_obter_tamanho(nos) > 0)
        tabela_remover(grafos, "ciclovias");
    lista_destruir(nos);
}
