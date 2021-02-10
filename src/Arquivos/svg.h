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
void svg_escrever(const char *caminho_svg, int num_listas, ...);

#endif
