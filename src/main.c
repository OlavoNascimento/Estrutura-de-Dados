#include <stdio.h>

#include <circulo.h>
#include <retangulo.h>

#define LINHA_MAX 300

// TODO: Máximo de 1000 objetos

int main() {
    FILE *entrada = fopen("entrada.geo", "r");
    if(entrada == NULL) {
        fprintf(stderr, "Arquivo de entrada não pode ser criado!\n");
        return 1;
    }
    FILE *saida = fopen("saida.svg", "w");
    if(saida == NULL) {
        fprintf(stderr, "Arquivo de saída não pode ser criado!\n");
        return 1;
    }

    fprintf(saida, "<svg width='1024' height='768'>\n");

    char linha[LINHA_MAX];
    while(fgets(linha, LINHA_MAX, entrada) != NULL) {
        if(linha[0] == 'c') {
            Circulo circ = criar_circulo(linha);
            circulo_para_svg(saida, circ);
        } else if(linha[0] == 'r') {
            Retangulo ret = criar_retangulo(linha);
            retangulo_para_svg(saida, ret);
        }
    }

    fprintf(saida, "</svg>\n");

    fclose(entrada);
    fclose(saida);

    return 0;
}