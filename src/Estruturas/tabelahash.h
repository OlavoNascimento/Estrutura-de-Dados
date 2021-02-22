#ifndef TABELAHASH_H
#define TABELAHASH_H

/*
Implementa uma tabela de espalhamento. A tabela é criada através da função tabela_criar().
Após a criação, é possível armazenar informações através da função tabela_inserir().
Informações são associadas a uma chave, essa chave facilita a busca e acesso dentro da tabela.
Além disso são implementadas funções para buscar e remover informações.
A tabela armazena listas duplamente encadeadas em cada um de seus baldes e as informações inseridas
serão armazenadas dentro dessas listas.
*/

#include "lista.h"

typedef struct Tabela_s *Tabela;

typedef void *TabelaInfo;

typedef void TabelaDestruirInfo(TabelaInfo info);

/*
Cria uma tabela de espalhamento
É possível passar uma função que extrai um identificador das informações armazenadas na lista que
está armazenada dentro do balde da tabela, a qual é utilizada para realizar buscas na lista.
Também é possível especificar uma função que libere a memória alocada pelas informações dessa lista,
para que ao liberar a memória da lista suas informações também sejam liberadas.
Retorna o endereço para um array de listas.
*/
Tabela tabela_criar(TabelaDestruirInfo destruir_info);

/*
Insere um elemento na tabela de espalhamento.
Nenhum parametro pode ser nulo, o parâmetro tabela deve ser uma tabela existente.
Não retorna nada.
*/
void tabela_inserir(Tabela tabela, const char *id, TabelaInfo info);

/*
Acessa uma lista da tabela e posteriormente um nó dessa lista.
O id passado deve ser válido.
Retorna a informação buscada. Caso o id não exista na tabela retorna NULL.
*/
TabelaInfo tabela_buscar(Tabela tabela, const char *id);

/*
Remove o elemento de id especificado da tabela.
Nenhum dos parâmetros podem ser nulos.
Retorna a informação removida. Caso o id não exista na tabela retorna NULL.
*/
TabelaInfo tabela_remover(Tabela tabela, const char *id);

/*
Libera a memória alocada em todos os do array e consequentemente todos os elementos das listas.
O parâmetro tabela não pode ser nulo e deve ser uma lista existente.
*/
void tabela_destruir(Tabela tabela);

#endif
