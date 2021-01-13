#ifndef SVG_H
#define SVG_H

/*
Este módulo define a função svg_lista_para_svg, responsável por transformar todas as figuras em
várias listas em um arquivo svg.
*/

#include "../Estruturas/quadtree.h"

/*
Itera uma quadtree e transforma cada elemento no seu código svg equivalente.
Todos os argumentos não podem ser nulos e todos os diretórios especificados no caminho do arquivo
svg devem existir.
Cria um arquivo svg no caminho especificado.
*/
void svg_quadtree_para_svg(const char *caminho_svg, QuadTree formas, QuadTree quadras,
                           QuadTree hidrantes, QuadTree radios, QuadTree semaforos, QuadTree postos,
                           QuadTree casos);

#endif
