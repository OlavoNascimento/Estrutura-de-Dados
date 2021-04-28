#include "comercios.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Estruturas/quadtree.h"
#include "../Estruturas/tabelahash.h"
#include "../Objetos/EquipamentosUrbanos/estabelecimento.h"
#include "../Utils/logging.h"

// Tamanho maxímo de um comando do arquivo de comércios.
#define TAMANHO_COMANDO 300

// Lê um tipo de estabelecimento e a descrição desse tipo. Um par chave/valor é criado no hashmap
// passado a função.
void definir_descricao_tipo(Tabela tipo_descricao, const char *linha) {
    char tipo[100];
    char *descricao = malloc(sizeof *descricao * 100);
    sscanf(linha, "t %s %s", tipo, descricao);
    tabela_inserir(tipo_descricao, tipo, descricao);
}

// Adiciona um estabelecimento a quadra especificada.
void adicionar_estabelecimento(QuadTree estabelecimentos, Tabela cep_quadra,
                               Tabela cnpj_estabelecimento, const char *linha) {
    char cep[100];
    sscanf(linha, "e %*s %*s %*s %s %*c %*d %*s", cep);
    QtNo no = tabela_buscar(cep_quadra, cep);
    if (no != NULL) {
        Quadra quadra_pai = getInfoQt(estabelecimentos, no);
        Estabelecimento est = estabelecimento_ler(linha, quadra_pai);
        insereQt(estabelecimentos, ponto_criar_com_figura(est), est);
        tabela_inserir(cnpj_estabelecimento, figura_obter_id(est), est);
    }
}

// Lê um arquivo de comércios fornecido a função e adiciona as figuras descritas em suas linha como
// elementos das quadtrees e tabelas.
void comercios_ler(const char *caminho_comercios, Tabela quadtrees, Tabela relacoes) {
    printf("Lendo estabelecimentos\n");
    FILE *arquivo_comercios = fopen(caminho_comercios, "r");
    if (arquivo_comercios == NULL) {
        LOG_ERRO("Falha ao ler arquivo de estabelecimentos: %s!\n", caminho_comercios);
        return;
    }

    QuadTree estabelecimentos = tabela_buscar(quadtrees, "estabelecimentos");

    Tabela tipo_descricao = tabela_buscar(relacoes, "tipo_descricao");
    Tabela cep_quadra = tabela_buscar(relacoes, "cep_quadra");
    Tabela cnpj_estabelecimento = tabela_buscar(relacoes, "cnpj_estabelecimento");

    char linha[TAMANHO_COMANDO];
    while (fgets(linha, TAMANHO_COMANDO, arquivo_comercios) != NULL) {
        char comando[TAMANHO_COMANDO];
        sscanf(linha, "%s", comando);

        if (strcmp("t", comando) == 0) {
            definir_descricao_tipo(tipo_descricao, linha);
        } else if (strcmp("e", comando) == 0) {
            adicionar_estabelecimento(estabelecimentos, cep_quadra, cnpj_estabelecimento, linha);
        }
    }

    fclose(arquivo_comercios);
}
