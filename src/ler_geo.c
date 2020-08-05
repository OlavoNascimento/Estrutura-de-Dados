#include "ler_geo.h"

#include <string.h>

#include "lista.h"

// Tamanho maxímo de um comando do arquivo de descrição.
#define LINHA_MAX 300

// Lê um arquivo de descrição fornecido a função e adiciona as figuras descritas
// em suas linha como elementos de uma lista.
Lista *ler_geo(const char *caminho_geo) {
    FILE *arquivo_descricao = fopen(caminho_geo, "r");
    if (arquivo_descricao == NULL) {
        fprintf(stderr, "ERRO: Falha ao ler arquivo de descrição: %s\n", caminho_geo);
        return NULL;
    }
    Lista *lista = criar_lista();

    int figuras_criadas = 0;
    int lista_max_figs = 1000;
    char linha[LINHA_MAX];
    while (fgets(linha, LINHA_MAX, arquivo_descricao) != NULL &&
           figuras_criadas < lista_max_figs) {
        char comando[100];
        sscanf(linha, "%s", comando);

        Figura nova_figura;
        if (strcmp("c", comando) == 0) {
            nova_figura.circ = ler_circulo(linha);
            inserir_lista(lista, nova_figura, TIPO_CIRCULO);
            figuras_criadas++;
        } else if (strcmp("r", comando) == 0) {
            nova_figura.ret = ler_retangulo(linha);
            inserir_lista(lista, nova_figura, TIPO_RETANGULO);
            figuras_criadas++;
        } else if (strcmp("t", comando) == 0) {
            nova_figura.tex = ler_texto(linha);
            inserir_lista(lista, nova_figura, TIPO_TEXTO);
            figuras_criadas++;
        } else if (strcmp("nx", comando) == 0) {
            sscanf(linha, "nx %d", &lista_max_figs);
            printf("Novo valor máximo: %d\n", lista_max_figs);
        }
    }
    fclose(arquivo_descricao);

    return lista;
}
