#include <stdio.h>

#include <ler_geo.h>
#include <lista.h>

// TODO: Máximo de 1000 objetos

int main() {
    FILE *entrada = fopen("entrada.geo", "r");
    if(entrada == NULL) {
        fprintf(stderr, "Arquivo de entrada não pode ser lido!\n");
        return 1;
    }
    FILE *saida = fopen("saida.svg", "w");
    if(saida == NULL) {
        fprintf(stderr, "Arquivo de saída não pode ser criado!\n");
        return 1;
    }

    Lista *lista = ler_geo(entrada);
    if(lista == NULL)
        return 1;

    lista_para_svg(lista, saida);

    destruir_lista(lista);
    fclose(entrada);
    fclose(saida);

    return 0;
}
