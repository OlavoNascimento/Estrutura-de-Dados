#include "ler_geo.h"

#include <stdio.h>
#include <string.h>

#include "figuras.h"
#include "lista.h"

// Tamanho maxímo de um comando do arquivo de descrição.
#define LINHA_MAX 300
// Tamanho maxímo da abreviação do tipo de uma figura
#define TIPO_FIGURA_TAMANHO 100

// Lê um arquivo de descrição fornecido a função e adiciona as figuras descritas em suas linha como
// elementos de uma lista.
Lista *ler_geo(const char *caminho_geo) {
    FILE *arquivo_descricao = fopen(caminho_geo, "r");
    if (arquivo_descricao == NULL) {
        fprintf(stderr, "ERRO: Falha ao ler arquivo de descrição: %s!\n", caminho_geo);
        return NULL;
    }
    Lista *lista = criar_lista();

    int figuras_criadas = 0;
    int lista_max_figs = 1000;
    char linha[LINHA_MAX];
    while (fgets(linha, LINHA_MAX, arquivo_descricao) != NULL &&
           figuras_criadas < lista_max_figs) {
        char tipo_figura[TIPO_FIGURA_TAMANHO];
        sscanf(linha, "%s", tipo_figura);

        Figura nova_figura;
        if (strcmp("c", tipo_figura) == 0) {
            nova_figura = figura_ler(linha, TIPO_CIRCULO);
            inserir_lista(lista, nova_figura);
            figuras_criadas++;
        } else if (strcmp("r", tipo_figura) == 0) {
            nova_figura = figura_ler(linha, TIPO_RETANGULO);
            inserir_lista(lista, nova_figura);
            figuras_criadas++;
        } else if (strcmp("t", tipo_figura) == 0) {
            nova_figura = figura_ler(linha, TIPO_LINHA);
            inserir_lista(lista, nova_figura);
            figuras_criadas++;
        } else if (strcmp("nx", tipo_figura) == 0) {
            sscanf(linha, "nx %d", &lista_max_figs);
            printf("Novo valor máximo: %d\n", lista_max_figs);
        }
    }
    fclose(arquivo_descricao);

    return lista;
}
