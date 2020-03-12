#include <stdio.h>

#define LINHA_MAX 300

int main() {
    FILE *entrada = fopen("entrada.txt", "r");
    if(entrada == NULL) {
        fprintf(stderr, "Arquivo de entrada não pode ser criado!");
        return 1;
    }

    char linha[LINHA_MAX];
    while(fgets(linha, LINHA_MAX, entrada) != NULL) {
        printf("%s", linha);
    }

    fclose(entrada);

    return 0;
}