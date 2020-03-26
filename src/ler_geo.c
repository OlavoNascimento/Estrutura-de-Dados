#include <ler_geo.h>
#include <lista.h>

#define LINHA_MAX 300

Lista* ler_geo(FILE* arquivo) {
    if(arquivo == NULL) {
        fprintf(stderr, "Arquivo de entrada não pode ser lido!\n");
        return NULL;
    }

    Lista *lista = criar_lista();

    char linha[LINHA_MAX];
    while(fgets(linha, LINHA_MAX, arquivo) != NULL) {
        Figuras nova_figura;
        switch (linha[0]) {
            case 'c':
                nova_figura.circ = criar_circulo(linha);
                inserir_lista(lista, nova_figura, tipo_circulo);
                break;
            case 'r':
                nova_figura.ret = criar_retangulo(linha);
                inserir_lista(lista, nova_figura, tipo_retangulo);
                break;
            case 't':
                nova_figura.tex = criar_texto(linha);
                inserir_lista(lista, nova_figura, tipo_texto);
                break;
        }
    }

    return lista;
}
