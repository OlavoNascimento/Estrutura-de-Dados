#include <stdio.h>

typedef struct {
    char id[100];
    int x;
    int y;
    char cor_borda[20];
    char cor_preenchimento[20];
    char texto[300];
} Texto;

Texto criar_texto(char *linha);
void texto_para_svg(FILE *arquivo, Texto tex);