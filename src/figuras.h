
#ifndef FIGURAS_H
#define FIGURAS_H

#include <stdbool.h>

#include "circulo.h"
#include "linha.h"
#include "retangulo.h"
#include "texto.h"

// Este módulo define o tipo Figura, assim como funções que criam, acessam suas propriedades e
// realizam operações entre variáveis deste tipo. Sua principal função é servir de intermediador
// entre os diferentes tipos de figuras e outros módulos.

// Tipos de figuras que podem ser instanciados, com exceção dos tipos TIPOS_FIGURA_MIN e
// TIPOS_FIGURA_MAX que servem o propósito de verificar se um tipo está contido no enum.
typedef enum {
    TIPOS_FIGURA_MIN = -1,
    TIPO_CIRCULO,
    TIPO_HIDRANTE,
    TIPO_LINHA,
    TIPO_QUADRA,
    TIPO_RADIO,
    TIPO_RETANGULO,
    TIPO_SEMAFORO,
    TIPO_TEXTO,
    TIPOS_FIGURA_MAX,
} TiposFigura;

typedef void *Figura;

/*
Cria um struct figura que recebe um dos possíveis tipos de figura a ser armazenada e a representação
de seu tipo no enum TiposFiguras.
Nenhum dos parâmetros podem ser nulos e seu tipo deve estar contido no enum TiposFigura, sendo
considerado inválidos os tipos TIPOS_FIGURA_MIN e TIPOS_FIGURA_MAX.
Retorna um ponteiro para o struct. O usuário é responsável por liberar a memória alocada!
*/
Figura figura_criar(void *figura, TiposFigura tipo);

/*
Escreve todas as informações presentes em uma figura em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void figura_escrever_informacoes(FILE *arquivo, Figura figura);
/*
Escreve o código svg necessário para representar uma figura em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void figura_escrever_svg(FILE *arquivo, Figura figura);

/*
Verifica se duas figuras se intersectam.
Nenhum dos parâmetros podem ser nulos.
Retorna verdadeiro caso as duas figura se intersectam, caso contrário retorna falso.
*/
bool figura_checar_interseccao(Figura figura1, Figura figura2);
/*
Verifica se um ponto é contido por uma figura.
O parâmetro figura não pode ser nulo.
Retorna verdadeiro caso o ponto se encontre contido pelo figura, caso contrário retorna falso.
*/
bool figura_checar_ponto_interno(Figura figura, double ponto_x, double ponto_y);

/*
Obtém a figura armazenada no struct Figura.
O parâmetro figura não pode ser nulo.
Retorna a figura armazenada no struct Figura.
*/
void *figura_obter_figura(Figura figura);
/*
Obtém o tipo de uma figura.
O parâmetro figura não pode ser nulo.
Retorna o tipo de uma figura.
*/
TiposFigura figura_obter_tipo(Figura figura);
/*
Obtém o nome do tipo de uma figura como uma string.
O parâmetro figura não pode ser nulo.
Retorna o nome do tipo de uma figura como uma string.
*/
const char *figura_obter_string_tipo(Figura figura);

/*
Obtém a coordenada x onde uma figura se inicia.
O parâmetro figura não pode ser nulo.
Retorna a coordenada x onde uma figura se inicia.
*/
double figura_obter_x_inicio(Figura figura);
/*
Obtém a coordenada y onde uma figura se inicia.
O parâmetro figura não pode ser nulo.
Retorna a coordenada y onde uma figura se inicia.
*/
double figura_obter_y_inicio(Figura figura);

/*
Obtém a coordenada x onde uma figura termina.
O parâmetro figura não pode ser nulo.
Retorna a coordenada x onde uma figura termina.
*/
double figura_obter_x_fim(Figura figura);
/*
Obtém a coordenada y onde uma figura termina.
O parâmetro figura não pode ser nulo.
Retorna a coordenada y onde uma figura termina.
*/
double figura_obter_y_fim(Figura figura);

/*
Obtém a coordenada x onde o centro de uma figura se encontra.
O parâmetro figura não pode ser nulo.
Retorna a coordenada x onde o centro de uma figura se encontra.
*/
double figura_obter_centro_x(Figura figura);
/*
Obtém a coordenada y onde o centro de uma figura se encontra.
O parâmetro figura não pode ser nulo.
Retorna a coordenada y onde o centro de uma figura se encontra.
*/
double figura_obter_centro_y(Figura figura);

/*
Obtém o id de uma figura.
O parâmetro figura não pode ser nulo.
Retorna o id de uma figura.
*/
const char *figura_obter_id(Figura figura);

/*
Obtém a cor da borda de uma figura.
O parâmetro figura não pode ser nulo.
Retorna a cor da borda de uma figura.
*/
const char *figura_obter_cor_borda(Figura figura);
/*
Define a cor da borda de uma figura.
Nenhum dos parâmetros podem ser nulos.
*/
void figura_definir_cor_borda(Figura figura, const char *cor_borda);

/*
Obtém a cor de preenchimento de uma figura.
O parâmetro figura não pode ser nulo.
Retorna a cor de preenchimento de uma figura.
*/
const char *figura_obter_cor_preenchimento(Figura figura);
/*
Define a cor de preenchimento de uma figura.
Nenhum dos parâmetros podem ser nulos.
*/
void figura_definir_cor_preenchimento(Figura figura, const char *cor_preenchimento);

/*
Define o arredondamento da borda de uma figura.
Nenhum dos parâmetros podem ser nulos.
*/
void figura_definir_arredondamento_borda(Figura figura, double raio_borda);

// Libera a memória alocada por uma figura.
void figura_destruir(Figura figura);

#endif

