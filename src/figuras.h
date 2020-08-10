#ifndef FIGURAS_H
#define FIGURAS_H

#include <stdbool.h>

#include "circulo.h"
#include "linha.h"
#include "retangulo.h"
#include "texto.h"

// Este módulo define o tipo Figura, assim como funções que criam, acessam suas propriedades e
// realizam operações entre variáveis deste tipo. Sua principal função é servir de intermediador
// entre a possíveis figuras e outros módulos.

// Tipos de figuras que podem ser instanciados, com exceção dos tipos TIPOS_FIGURA_MIN e
// TIPOS_FIGURA_MAX que servem o propósito de verificar se um tipo está contido no enum.
typedef enum {
    TIPO_CIRCULO,
    TIPO_LINHA,
    TIPO_RETANGULO,
    TIPO_TEXTO,
} TiposFigura;

typedef void *Figura;

// Cria um struct figura que recebe uma figura a ser armazenada e o tipo dessa figura.
// A figura não pode nula e seu tipo deve estar contido no enum TiposFigura.
Figura figura_criar(void *figura, TiposFigura tipo);
// Instancia uma figura de tipo especificado com base nas informações de uma linha.
// A linha não pode ser nula e o tipo deve estar contido no enum TiposFigura.
Figura figura_ler(const char *linha, TiposFigura tipo);

// Escreve todos os dados de uma figura em um arquivo passado a função.
// Nenhum dos argumentos pode ser nulo.
void figura_escrever_informacoes(FILE *arquivo, Figura figura);
// Escreve o código svg que representa uma figura em um arquivo.
void figura_escrever_svg(FILE *arquivo, Figura figura);

// Retorna verdadeiro caso duas figuras se intersectem.
// Ambos os argumentos não podem ser nulos.
bool figura_checar_interseccao(Figura figura1, Figura figura2);
// Checa se um ponto se encontra dentro ou fora de uma figura.
// O argumento figura não pode ser nulo
bool figura_checar_ponto_interno(Figura figura, double ponto_x, double ponto_y);

// Retorna o nome do tipo de uma figura.
// O argumento figura não pode ser nulo.
TiposFigura figura_obter_tipo(Figura figura);
// Retorna o nome do tipo de uma figura como uma string.
// O argumento figura não pode ser nulo.
const char *figura_obter_string_tipo(Figura figura);
// Retorna a figura armazenada no struct Figura.
// O argumento figura não pode ser nulo.
void *figura_obter_figura(Figura figura);
// Retorna a coordenada x onde uma figura se inicia.
// O argumento figura não pode ser nulo.
double figura_obter_x_inicio(Figura figura);
// Retorna a coordenada y onde uma figura se inicia.
// O argumento figura não pode ser nulo.
double figura_obter_y_inicio(Figura figura);
// Retorna a coordenada x onde uma figura acaba.
// O argumento figura não pode ser nulo.
double figura_obter_x_fim(Figura figura);
// Retorna a coordenada y onde uma figura acaba.
// O argumento figura não pode ser nulo.
double figura_obter_y_fim(Figura figura);
// Retorna a coordenada x do centro de uma figura.
// O argumento figura não pode ser nulo.
double figura_obter_centro_x(Figura figura);
// Retorna a coordenada y do centro de uma figura.
// O argumento figura não pode ser nulo.
double figura_obter_centro_y(Figura figura);

// Retorna o id de uma figura.
// O argumento figura não pode ser nulo.
const char *figura_obter_id(Figura figura);
// Retorna a cor da borda de uma figura.
// O argumento figura não pode ser nulo.
const char *figura_obter_cor_borda(Figura figura);
// Substitui a cor da borda de uma figura.
// Nenhum dos argumentos pode ser nulo.
void figura_definir_cor_borda(Figura figura, const char *cor_borda);
// Retorna a cor do preenchimento de uma figura.
// O argumento figura não pode ser nulo.
const char *figura_obter_cor_preenchimento(Figura figura);
// Substitui a cor de preenchimento de uma figura.
// Nenhum dos argumentos pode ser nulo.
void figura_definir_cor_preenchimento(Figura figura, const char *cor_preenchimento);

#endif
