#include <stdio.h>

#include <circulo.h>

#define LINHA_MAX 300

int main() {
    FILE *entrada = fopen("entrada.geo", "r");
    if(entrada == NULL) {
        fprintf(stderr, "Arquivo de entrada não pode ser criado!");
        return 1;
    }
    FILE *saida = fopen("saida.svg", "w");
    if(saida == NULL) {
        fprintf(stderr, "Arquivo de saída não pode ser criado!");
        return 1;
    }

    fprintf(saida, "<svg width='1024' height='768'>\n");

    char linha[LINHA_MAX];
    while(fgets(linha, LINHA_MAX, entrada) != NULL) {
        if(linha[0] == 'c') {
            Circulo circ = criar_circulo(linha);
            circulo_para_svg(saida, circ);
        }
    }

    fprintf(saida, "</svg>\n");

    fclose(entrada);
    fclose(saida);

    return 0;
}