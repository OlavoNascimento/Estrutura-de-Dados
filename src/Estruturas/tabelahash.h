#ifndef TABELAHASH_H
#define TABELAHASH_H

#include "lista.h"

typedef void *Tabela;
typedef void *TabelaInfo;

/*
Cria uma tabela de espalhamento
É possível passar uma função que extrai um identificador das informações armazenadas na lista que
está armazenada dentro do balde da tabela, a qual é utilizada para realizar buscas na lista. Também
é possível especificar uma função que libere a memória alocada pelas informações dessa lista, para
que ao liberar a memória da lista suas informações também sejam liberadas. Retorna o endereço para
um array de listas.
*/
Tabela tabela_criar(int size, ObterIdentificadorInfo obter_identificador_info,
                    DestruirInfo destruir_info);

/*
Insere um elemento na tabela de espalhamento.
Nenhum parametro pode ser nulo, o parâmetro tabela deve ser uma tabela existente.
Não retorna nada.
*/
void tabela_inserir(Tabela tabela, ListaInfo info, const char id[100]);

/*
Acessa uma lista da tabela e posteriormente um nó dessa lista.
O id passado deve ser válido.
Retorna o endereço do nó.
*/
ListaNo tabela_buscar(Tabela tabela, const char id[100]);

/*
Libera a memória alocada em todos os do array e consequentemente todos os elementos das listas.
O parâmetro tabela não pode ser nulo e deve ser uma lista existente.
*/
void tabela_destruir(Tabela tabela);

#endif
