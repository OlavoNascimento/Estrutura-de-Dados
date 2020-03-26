#include <stdio.h>

#include <lista.h>
#include <circulo.h>
#include <retangulo.h>
#include <texto.h>

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

    Lista *lista = criar_lista();

    fprintf(saida, "<svg>\n");

    char linha[LINHA_MAX];
    while(fgets(linha, LINHA_MAX, entrada) != NULL) {
        Figuras nova_figura;
        switch (linha[0]) {
            case 'c':
                nova_figura.circ = criar_circulo(linha);
                inserir_lista(lista, nova_figura, tipo_circulo);
                circulo_para_svg(saida, nova_figura.circ);
                break;
            case 'r':
                nova_figura.ret = criar_retangulo(linha);
                inserir_lista(lista, nova_figura, tipo_retangulo);
                retangulo_para_svg(saida, nova_figura.ret);
                break;
            case 't':
                nova_figura.tex = criar_texto(linha);
                inserir_lista(lista, nova_figura, tipo_texto);
                texto_para_svg(saida, nova_figura.tex);
                break;
        }
    }

    fprintf(saida, "</svg>\n");

    struct No *atual = lista->cabeca;
    while(atual != NULL){
        switch (atual->tipo) {
        case tipo_circulo:
            printf("Circulo: %s\n", atual->figura.circ.id);
            break;
        case tipo_retangulo:
            printf("Retangulo: %s\n", atual->figura.ret.id);
            break;
        case tipo_texto:
            printf("Texto: %s\n", atual->figura.tex.id);
            break;
        }
        atual = atual->prox;
    }

    destruir_lista(lista);
    fclose(entrada);
    fclose(saida);

    return 0;
}
