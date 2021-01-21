#include "comercios.h"

#include <stdio.h>
#include <string.h>

#include "../Estruturas/quadtree.h"
#include "../Estruturas/tabelahash.h"
#include "../Utils/logging.h"

// Tamanho maxímo de um comando do arquivo de comércios.
#define TAMANHO_COMANDO 300

// Lê um arquivo de comércios fornecido a função e adiciona as figuras descritas em suas linha
// como elementos de uma lista.
void comercios_ler(const char *caminho_comercios, Tabela quadtrees, Tabela relacoes) {
    FILE *arquivo_comercios = fopen(caminho_comercios, "r");
    if (arquivo_comercios == NULL) {
        fprintf(stderr, "ERRO: Falha ao ler arquivo de comércios: %s!\n", caminho_comercios);
        return;
    }

    QuadTree moradores = tabela_buscar(quadtrees, "moradores");
    Tabela tipo_descricao = tabela_buscar(relacoes, "tipo_descricao");

    char tipo_estabelecimentos[100];
    tipo_estabelecimentos[0] = '\0';

    char linha[TAMANHO_COMANDO];
    while (fgets(linha, TAMANHO_COMANDO, arquivo_comercios) != NULL) {
        char comando[TAMANHO_COMANDO];
        sscanf(linha, "%s", comando);

        if (strcmp("t", comando) == 0) {
        } else if (strcmp("e", comando) == 0) {
        }
    }

    fclose(arquivo_comercios);
}
