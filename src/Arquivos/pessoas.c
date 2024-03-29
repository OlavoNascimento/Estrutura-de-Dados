#include "pessoas.h"

#include <stdio.h>
#include <string.h>

#include "../Estruturas/quadtree.h"
#include "../Estruturas/tabelahash.h"
#include "../Objetos/EquipamentosUrbanos/morador.h"
#include "../Utils/logging.h"

// Tamanho maxímo de um comando do arquivo de moradores.
#define TAMANHO_COMANDO 300

// Um par chave/valor é criado no hashmap com o cpf e dados do morador.
void definir_dados_pessoa(Tabela dados_pessoa, const char *linha) {
    Morador novo_morador = morador_ler(linha);
    tabela_inserir(dados_pessoa, figura_obter_id(novo_morador), novo_morador);
}

// Adiciona um morador a quadra especificada.
void adicionar_morador(QuadTree moradores, Tabela cep_quadra, Tabela dados_pessoa, Tabela cpf_cep,
                       const char *linha) {
    char cpf[100];
    char cep[100];
    sscanf(linha, "m %s %s %*c %*d %*s", cpf, cep);

    Morador modificar_morador = tabela_buscar(dados_pessoa, cpf);
    if (modificar_morador == NULL)
        return;

    QtNo no = tabela_buscar(cep_quadra, cep);
    if (no != NULL) {
        Quadra quadra_pai = quadtree_obter_info(no);
        morador_ler_endereco(modificar_morador, linha, quadra_pai);

        quadtree_inserir(moradores, ponto_criar_com_figura(modificar_morador), modificar_morador);
        tabela_inserir(cpf_cep, cpf, cep);
    }
}

// Lê um arquivo de moradores fornecido a função.
void pessoas_ler(const char *caminho_descricao_moradores, Tabela quadtrees, Tabela relacoes) {
    printf("Lendo moradores\n");
    FILE *arquivo_descricao_moradores = fopen(caminho_descricao_moradores, "r");
    if (arquivo_descricao_moradores == NULL) {
        LOG_ERRO("Falha ao ler arquivo de descrição de moradores: %s!\n",
                 caminho_descricao_moradores);
        return;
    }

    QuadTree moradores = tabela_buscar(quadtrees, "moradores");

    Tabela cep_quadra = tabela_buscar(relacoes, "cep_quadra");
    Tabela cpf_cep = tabela_buscar(relacoes, "cpf_cep");
    Tabela dados_pessoa = tabela_buscar(relacoes, "dados_pessoa");

    char linha[TAMANHO_COMANDO];
    while (fgets(linha, TAMANHO_COMANDO, arquivo_descricao_moradores) != NULL) {
        char comando[TAMANHO_COMANDO];
        sscanf(linha, "%s", comando);

        if (strcmp("p", comando) == 0) {
            definir_dados_pessoa(dados_pessoa, linha);
        } else if (strcmp("m", comando) == 0) {
            adicionar_morador(moradores, cep_quadra, dados_pessoa, cpf_cep, linha);
        }
    }

    fclose(arquivo_descricao_moradores);
}
