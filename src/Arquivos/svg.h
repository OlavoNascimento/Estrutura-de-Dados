#ifndef SVG_H
#define SVG_H

/*
Este módulo define a função svg_lista_para_svg, responsável por transformar todas as figuras em
várias listas em um arquivo svg.
*/

/*
Escreve o svg correspondente a todas as quadtrees passadas como argumento.
O argumento caminho_svg não pode ser nulo, assim como as quadtrees passadas. O parâmetro
num_quadtrees deve ser igual a quantidade de quadtrees passadas a função.
Cria um arquivo svg no caminho especificado.
*/
void svg_quadtrees_para_svg(const char *caminho_svg, int num_quadtrees, ...);

#endif
