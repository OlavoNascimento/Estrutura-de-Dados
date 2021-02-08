#ifndef LISTA_H
#define LISTA_H

/*
Este módulo define uma Lista, responsável por armazenar os diferentes tipos de informações que
existem no projeto. Disponibiliza funções para modificar criar, modificar e destruir a lista.
*/

typedef struct Lista_s *Lista;
typedef struct ListaNo_s *ListaNo;
typedef void *ListaInfo;

typedef const char *ObterIdentificadorLista(ListaInfo info);
typedef void ListaDestruirInfoListaInfo info);
typedef void MapInfoLista(ListaInfo info, void *extra);

/*
Itera por todos os nós de uma lista.
Será criado um ListaNo contendo o nó atual. A variável lista deve ser uma Lista não nula.
Não é possível remover um nó durante a iteração!
*/
#define for_each_lista(nome, lista) \
    for (ListaNo nome = lista_obter_primeiro(lista); nome != NULL; nome = lista_obter_proximo(nome))

/*
Cria o ponteiro inicial de lista dinamica duplamente encadeada.
É possível passar uma função que extrai um identificador das informações armazenadas na lista, a
qual é utilizada para realizar buscas na lista.
Também é possível especificar uma função que libere a memória alocada pelas informações da lista,
para que ao liberar a memória da lista suas informações também sejam liberadas.
Retorna o endereço para uma lista vazia.
*/
Lista lista_criar(ObterIdentificadorLista obter_identificador_info,
                  DestruirInfoLista destruir_info);

/*
Insere um elemento no final da lista.
Ambos os parâmetros não podem ser nulos, o parâmetro lista deve ser uma lista existente.
Retrna o endereço para o elemento adicionado.
*/
ListaNo lista_inserir_final(Lista lista, ListaInfo info);

/*
Insere um elemento posterior a um determinado nó "p".
Todos os parâmetros não podem ser nulos e o parâmetro p deve ser um elemento válido da lista.
É retornado o endereço do elemento inserido.
*/
ListaNo lista_inserir_antes(Lista lista, ListaInfo info, ListaNo p);

/*
Insere um elemento anterior a um determinado nó "p".
Todos os parâmetros não podem ser nulos e o parâmetro p deve ser um elemento válido da lista.
É retornado o endereço do elemento inserido.
*/
ListaNo lista_inserir_depois(Lista lista, ListaInfo info, ListaNo p);

/*
Remove um elemento da lista.
Todos os parâmetros não podem ser nulos e o parâmetro lista deve ser uma lista existente.
*/
void lista_remover(Lista lista, ListaNo no_selecionado);

/*
Acessa um nó da lista.
O id passado deve ser válido.
Retorna o endereço do nó.
*/
ListaNo lista_buscar(Lista lista, const char *id);

/*
Troca as informações armazenadas em dois nós.
Ambos os parâmetros não podem ser nulos.
Os nós tem suas informações trocadas.
*/
void lista_trocar_info(ListaNo no1, ListaNo no2);

/*
Aplica uma função a todas as informações de uma lista.
Os parâmetros lista e f não podem ser nulos.
A função f é aplicada em todas as informações da lista.
*/
void lista_map(Lista lis, MapInfoLista f, void *extra);

/*
Obtém o tamanho da lista (número de nós).
O parâmetro lista não pode ser nulo e deve ser uma lista existente.
retorna um inteiro igual ao tamanho da lista.
*/
int lista_obter_tamanho(Lista lista);

/*
Acessa o primeiro elemento da lista
O parâmetro lista não pode ser nulo, deve ser uma lista existe e a lista não pode estar vazia.
Retorna o endereço do primeiro nó.
*/
ListaNo lista_obter_primeiro(Lista lista);

/*
Acessa o último elemento da lista.
O parâmetro lista não pode ser nulo, deve ser uma lista existe e a lista não pode estar vazia.
Retorna o endereço do último nó.
*/
ListaNo lista_obter_ultimo(Lista lista);

/*
Acessa as informações de um nó.
O parâmetro p não pode ser NULL e deve indicar um endereço válido que esteja na lista.
Retorna o endereço para as informações do nó especificado por p.
*/
ListaInfo lista_obter_info(ListaNo p);

/*
Acessa o elemento posterior ao elemento p.
Ambos parâmetros não podem ser nulos e o parâmetro p deve indicar um endereço válido que esteja na
lista.
Retorna o endereço do elemento posterior de p.
*/
ListaNo lista_obter_proximo(ListaNo p);

/*
Acessa o elemento anterior ao elemento p.
Ambos parâmetros não podem ser nulos e o parâmetro p deve indicar um endereço válido que esteja na
lista.
Retorna o endereço do elemento posterior de p.
*/
ListaNo lista_obter_anterior(ListaNo p);

/*
Libera a memória alocada em todos os elementos da lista.
O parâmetro lista não pode ser nulo e deve ser uma lista existente.
*/
void lista_destruir(Lista lista);

#endif
