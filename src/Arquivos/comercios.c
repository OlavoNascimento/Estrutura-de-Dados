#include "comercios.h"

#include <stdio.h>
#include <string.h>

#include "../Estruturas/quadtree.h"
#include "../Estruturas/tabelahash.h"
#include "../Objetos/EquipamentosUrbanos/estabelecimento.h"
#include "../Utils/logging.h"

// Tamanho maxímo de um comando do arquivo de comércios.
#define TAMANHO_COMANDO 300

// Lê um tipo de estabelecimento e a descrição desse tipo. Um par chave/valor é criado no hashmap
// passado a função.
void definir_descricao_tipo(Tabela descricoes, const char *linha) {
    char tipo[100];
    char descricao[100];
    sscanf(linha, "t %s %s", tipo, descricao);
    tabela_inserir(descricoes, tipo, descricao);
}

// Adiciona um estabelecimento a quadra especificada.
void adicionar_estabelecimento(QuadTree estabelecimentos, QuadTree quadras, const char *linha) {
    char cep[100];
    sscanf(linha, "e %*s %*s %*s %s %*c %*d %*s", cep);
    QtNo no = quadtree_buscar_id(quadras, cep);
    if (no != NULL) {
        Quadra quadra_pai = getInfoQt(no);
        Estabelecimento est = estabelecimento_ler(linha, quadra_pai);
        insereQt(estabelecimentos, ponto_criar_com_figura(est), est);
    }
}

// Lê um arquivo de comércios fornecido a função e adiciona as figuras descritas em suas linha
// como elementos de uma lista.
void comercios_ler(const char *caminho_comercios, Tabela quadtrees, Tabela relacoes) {
    FILE *arquivo_comercios = fopen(caminho_comercios, "r");
    if (arquivo_comercios == NULL) {
        fprintf(stderr, "ERRO: Falha ao ler arquivo de comércios: %s!\n", caminho_comercios);
        return;
    }

    QuadTree quadras = tabela_buscar(quadtrees, "quadras");
    QuadTree estabelecimentos = tabela_buscar(quadtrees, "estabelecimentos");
    Tabela tipo_descricao = tabela_buscar(relacoes, "tipo_descricao");

    char linha[TAMANHO_COMANDO];
    while (fgets(linha, TAMANHO_COMANDO, arquivo_comercios) != NULL) {
        char comando[TAMANHO_COMANDO];
        sscanf(linha, "%s", comando);

        if (strcmp("t", comando) == 0) {
            definir_descricao_tipo(tipo_descricao, linha);
        } else if (strcmp("e", comando) == 0) {
            adicionar_estabelecimento(estabelecimentos, quadras, linha);
        }
    }

    fclose(arquivo_comercios);
}
