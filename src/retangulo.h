#include <stdio.h>

typedef struct {
    char id[100];
    int largura;
    int altura;
    int x;
    int y;
    char cor_borda[20];
    char cor_preenchimento[20];
} Retangulo;

Retangulo criar_retangulo(char *linha);
void retangulo_para_svg(FILE *arquivo, Retangulo ret);