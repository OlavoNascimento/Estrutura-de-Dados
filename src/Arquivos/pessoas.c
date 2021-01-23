#include "pessoas.h"

#include <stdio.h>
#include <string.h>

#include "../Estruturas/quadtree.h"
#include "../Estruturas/tabelahash.h"
#include "../Objetos/Outros/morador.h"
#include "../Utils/logging.h"

// Tamanho maxímo de um comando do arquivo de moradores.
#define LINHA_MAX 300

// Um par chave/valor é criado no hashmap com o cpf e dados do morador.
void definir_dados_pessoa(Tabela dados, const char *linha, Morador morador) {
    char cpf[100];
    sscanf(linha, "p %s %*s %*s %*c %*s", cpf);
    tabela_inserir(dados, cpf, morador);
}

// Um par chave/valor é criado no hashmap com o cep e dados do morador.
void definir_cep_pessoa(Tabela endereco, const char *linha, Morador morador) {
    char cep[100];
    sscanf(linha, "m %*s %s %*c %*d %*s", cep);
    tabela_inserir(endereco, cep, morador);
}

// Adiciona um morador a quadra especificada.
void adicionar_morador(QuadTree moradores, QuadTree quadras, const char *linha, Morador morador) {
    char cep[100];
    sscanf(linha, "e %*s %*s %*s %s %*c %*d %*s", cep);
    QtNo no = quadtree_buscar_id(quadras, cep);
    if (no != NULL) {
        Quadra quadra_pai = getInfoQt(quadras, no);
        morador_ler_endereco(morador, linha, quadra_pai);

        // insere um morador na quadra apenas quando endereço do morador é especificado
        insereQt(moradores, ponto_criar_com_figura(morador), morador);
    }
}

// Lê um arquivo de moradores fornecido a função.
void pessoas_ler(const char *caminho_descricao_moradores, Tabela quadtrees, Tabela relacoes) {
    FILE *arquivo_descricao_moradores = fopen(caminho_descricao_moradores, "r");
    if (arquivo_descricao_moradores == NULL) {
        fprintf(stderr, "ERRO: Falha ao ler arquivo de descrição de moradores: %s!\n",
                caminho_descricao_moradores);
        return;
    }

    QuadTree quadras = tabela_buscar(quadtrees, "quadras");
    QuadTree moradores = tabela_buscar(quadtrees, "moradores");
    Tabela cpf_cep = tabela_buscar(relacoes, "cpf_cep");
    Tabela dados_pessoa = tabela_buscar(relacoes, "dados_pessoa");

    char linha[LINHA_MAX];
    while (fgets(linha, LINHA_MAX, arquivo_descricao_moradores) != NULL) {
        char comando[LINHA_MAX];
        sscanf(linha, "%s", comando);

        if (strcmp("p", comando) == 0) {
            Morador novo_morador = morador_ler(linha);
            definir_dados_pessoa(dados_pessoa, linha, novo_morador);
        } else if (strcmp("m", comando) == 0) {
            char cpf[100];
            sscanf(linha, "m %s %*s %*c %*d %*s", cpf);
            Morador modificar_morador = tabela_buscar(dados_pessoa, cpf);
            adicionar_morador(moradores, quadras, linha, modificar_morador);
            definir_cep_pessoa(cpf_cep, linha, modificar_morador);
        }
    }

    fclose(arquivo_descricao_moradores);
}
