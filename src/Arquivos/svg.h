#ifndef SVG_H
#define SVG_H

/*
Este módulo define a funções que convertem uma estrutura de dados para seu arquivo svg
correspondente.
*/

/*
Escreve o svg correspondente a todas as listas passadas como argumento.
O argumento caminho_svg não pode ser nulo, assim como as listas passadas. O parâmetro
num_listas deve ser igual a quantidade de listas passadas a função.
Cria um arquivo svg no caminho especificado.
*/
void svg_escrever_listas(const char *caminho_svg, int num_listas, ...);

/*
Escreve o svg correspondente a todas as quadtrees passadas como argumento.
O argumento caminho_svg não pode ser nulo, assim como as quadtrees passadas. O parâmetro
num_quadtrees deve ser igual a quantidade de quadtrees passadas a função.
Cria um arquivo svg no caminho especificado.
*/
void svg_escrever_quadtrees(const char *caminho_svg, int num_quadtrees, ...);

#endif
