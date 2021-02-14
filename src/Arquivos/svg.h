#ifndef SVG_H
#define SVG_H

/*
Este módulo define a funções que convertem uma estrutura de dados para seu arquivo svg
correspondente.
*/

/*
Escreve o svg correspondente a todas as estruturas passadas como argumento.
O argumento caminho_svg não pode ser nulo, assim como as estruturas passadas.
O parâmetro num_estruturas deve ser igual a quantidade de estruturas passadas a função.
Todas as estruturas devem ter uma função de map.
Cria um arquivo svg no caminho especificado.
*/
void svg_escrever(const char *caminho_svg, int num_estruturas, ...);

#endif
