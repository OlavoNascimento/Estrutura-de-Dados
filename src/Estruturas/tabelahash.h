#ifndef TABELAHASH_H
#define TABELAHASH_H

/*
Implementa uma tabela de espalhamento. A tabela é criada através da função tabela_criar(), podendo
receber uma função para destruir as informações ao liberar a tabela.
Após a criação, é possível armazenar informações através da função tabela_inserir().
Informações são associadas a uma chave, essa chave permite a busca e acesso dentro da tabela.
Além disso são implementadas funções para buscar e remover informações, tabela_buscar() e
tabela_remover() respectivamente.
*/

#include "lista.h"

typedef struct Tabela_s *Tabela;

typedef void *TabelaInfo;

typedef void TabelaDestruirInfo(TabelaInfo info);

/*
Cria uma tabela de espalhamento
É possível especificar uma função que libere a memória alocada pelas informações dessa tabela, para
que ao liberar a memória da tabela suas informações também sejam liberadas.
Retorna o endereço para uma tabela vazia.
*/
Tabela tabela_criar(TabelaDestruirInfo destruir_info);

/*
Insere um elemento na tabela de espalhamento.
Os parâmetros tabela e id não podem ser nulos!
*/
void tabela_inserir(Tabela tabela, const char *id, TabelaInfo info);

/*
Acessa uma lista da tabela e posteriormente um nó dessa lista.
Nenhum parâmetro pode ser nulo!
Retorna a informação buscada. Caso o id não exista na tabela retorna NULL.
*/
TabelaInfo tabela_buscar(Tabela tabela, const char *id);

/*
Remove o elemento de id especificado da tabela.
Nenhum parâmetro pode ser nulo!
Retorna a informação removida. Caso o id não exista na tabela retorna NULL.
*/
TabelaInfo tabela_remover(Tabela tabela, const char *id);

/*
Libera a memória alocada em todos os do array e consequentemente todos os elementos das listas.
O parâmetro tabela não pode ser nulo!.
*/
void tabela_destruir(Tabela tabela);

#endif
