#include <ler_geo.h>
#include <lista.h>

#define LINHA_MAX 300

Lista* ler_geo(char *caminho_geo) {
    Lista *lista = criar_lista();

    FILE *arquivo = fopen(caminho_geo, "r");
    if(arquivo == NULL) {
        fprintf(stderr, "Falha ao ler %s!\n", caminho_geo);
        return lista;
    }
    int figuras_criadas = 0;
    int lista_max_figs = 1000;

    char linha[LINHA_MAX];
    while(fgets(linha, LINHA_MAX, arquivo) != NULL
          && figuras_criadas < lista_max_figs) {
        Figuras nova_figura;
        switch(linha[0]) {
            case 'c':
                nova_figura.circ = criar_circulo(linha);
                inserir_lista(lista, nova_figura, TipoCirculo);
                figuras_criadas++;
                break;
            case 'r':
                nova_figura.ret = criar_retangulo(linha);
                inserir_lista(lista, nova_figura, TipoRetangulo);
                figuras_criadas++;
                break;
            case 't':
                nova_figura.tex = criar_texto(linha);
                inserir_lista(lista, nova_figura, TipoTexto);
                figuras_criadas++;
                break;
            default:
                if(linha[0] == 'n' && linha[1] == 'x') {
                    sscanf(linha, "nx %d", &lista_max_figs);
                    printf("Novo valor máximo: %d\n", lista_max_figs);
                }
                break;
        }
    }
    fclose(arquivo);

    return lista;
}
