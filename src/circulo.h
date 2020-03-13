#include <stdio.h>

typedef struct {
    char id[100];
    int raio;
    int x;
    int y;
    char cor_borda[20];
    char cor_preenchimento[20];
} Circulo;

Circulo criar_circulo(char *linha);
Circulo circulo_para_svg(FILE *arquivo, Circulo circ);