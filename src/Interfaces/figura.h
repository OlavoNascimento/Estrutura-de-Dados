#ifndef FIGURA_H
#define FIGURA_H

/*
Define uma FiguraInterface a qual pode ser incluída em outros objetos para permitir que sejam
utilizados como uma figura genérica.
Isso é possível já que ao chamar uma das funções que recebem uma Figura como argumento, o objeto
internamente utiliza um ponteiro de uma função registrada anteriormente, tornando polimorfismo
possível.
*/

#include <stdbool.h>
#include <stdio.h>

/*
Figura genérica que possui possui uma FiguraInterface com funções registradas.
*/
typedef void *Figura;
/*
Interface que guarda diversos ponteiros de funções que devem ser implementadas para um objeto poder
ser utilizado como uma Figura.
*/
typedef void *FiguraInterface;

/*
Cria e inicializa uma FiguraInterface, na qual diversas funções podem ser registradas para permitir
que um objeto qualquer possa ser utilizado como uma figura genérica.
O usuário é responsável por liberar a memória alocada!
*/
FiguraInterface figura_interface_criar();

/*
Registra a função escrever_informacoes em uma FiguraInterface.
Nenhum dos parâmetros podem ser nulos.
A função é definida na interface, permitindo que seja utilizada posteriormente.
*/
void figura_registrar_escrever_informacoes(FiguraInterface figuraInterface,
                                           void (*escrever_informacoes)(Figura figura,
                                                                        FILE *arquivo));

/*
Registra a função escrever_svg em uma FiguraInterface.
Nenhum dos parâmetros podem ser nulos.
A função é definida na interface, permitindo que seja utilizada posteriormente.
*/
void figura_registrar_escrever_svg(FiguraInterface figuraInterface,
                                   void (*escrever_svg)(Figura figura, FILE *arquivo));

/*
Registra a função obter_tipo em uma FiguraInterface.
Nenhum dos parâmetros podem ser nulos.
A função é definida na interface, permitindo que seja utilizada posteriormente.
*/
void figura_registrar_obter_tipo(FiguraInterface figuraInterface,
                                 const char *(*obter_tipo)(Figura figura));

/*
Registra a função obter_x em uma FiguraInterface.
Nenhum dos parâmetros podem ser nulos.
A função é definida na interface, permitindo que seja utilizada posteriormente.
*/
void figura_registrar_obter_x(FiguraInterface figuraInterface, double (*obter_x)(Figura figura));

/*
Registra a função obter_y em uma FiguraInterface.
Nenhum dos parâmetros podem ser nulos.
A função é definida na interface, permitindo que seja utilizada posteriormente.
*/
void figura_registrar_obter_y(FiguraInterface figuraInterface, double (*obter_y)(Figura figura));

/*
Registra a função obter_x_inicio em uma FiguraInterface.
Nenhum dos parâmetros podem ser nulos.
A função é definida na interface, permitindo que seja utilizada posteriormente.
*/
void figura_registrar_obter_x_inicio(FiguraInterface figuraInterface,
                                     double (*obter_x_inicio)(Figura figura));

/*
Registra a função obter_y_inicio em uma FiguraInterface.
Nenhum dos parâmetros podem ser nulos.
A função é definida na interface, permitindo que seja utilizada posteriormente.
*/
void figura_registrar_obter_y_inicio(FiguraInterface figuraInterface,
                                     double (*obter_y_inicio)(Figura figura));

/*
Registra a função obter_x_fim em uma FiguraInterface.
Nenhum dos parâmetros podem ser nulos.
A função é definida na interface, permitindo que seja utilizada posteriormente.
*/
void figura_registrar_obter_x_fim(FiguraInterface figuraInterface,
                                  double (*obter_x_fim)(Figura figura));

/*
Registra a função obter_y_fim em uma FiguraInterface.
Nenhum dos parâmetros podem ser nulos.
A função é definida na interface, permitindo que seja utilizada posteriormente.
*/
void figura_registrar_obter_y_fim(FiguraInterface figuraInterface,
                                  double (*obter_y_fim)(Figura figura));

/*
Registra a função obter_x_centro em uma FiguraInterface.
Nenhum dos parâmetros podem ser nulos.
A função é definida na interface, permitindo que seja utilizada posteriormente.
*/
void figura_registrar_obter_x_centro(FiguraInterface figuraInterface,
                                     double (*obter_centro_x)(Figura figura));

/*
Registra a função obter_y_centro em uma FiguraInterface.
Nenhum dos parâmetros podem ser nulos.
A função é definida na interface, permitindo que seja utilizada posteriormente.
*/
void figura_registrar_obter_y_centro(FiguraInterface figuraInterface,
                                     double (*obter_centro_y)(Figura figura));

/*
Registra a função obter_id em uma FiguraInterface.
Nenhum dos parâmetros podem ser nulos.
A função é definida na interface, permitindo que seja utilizada posteriormente.
*/
void figura_registrar_obter_id(FiguraInterface figuraInterface,
                               const char *(*obter_id)(Figura figura));

/*
Registra a função obter_cor_borda em uma FiguraInterface.
Nenhum dos parâmetros podem ser nulos.
A função é definida na interface, permitindo que seja utilizada posteriormente.
*/
void figura_registrar_obter_cor_borda(FiguraInterface figuraInterface,
                                      const char *(*obter_cor_borda)(Figura figura));

/*
Registra a função definir_cor_borda em uma FiguraInterface.
Nenhum dos parâmetros podem ser nulos.
A função é definida na interface, permitindo que seja utilizada posteriormente.
*/
void figura_registrar_definir_cor_borda(FiguraInterface figuraInterface,
                                        void (*definir_cor_borda)(Figura figura,
                                                                  const char *cor_borda));

/*
Registra a função obter_cor_preenchimento em uma FiguraInterface.
Nenhum dos parâmetros podem ser nulos.
A função é definida na interface, permitindo que seja utilizada posteriormente.
*/
void figura_registrar_obter_cor_preenchimento(
    FiguraInterface figuraInterface, const char *(*obter_cor_preenchimento)(Figura figura));

/*
Registra a função definir_cor_preenchimento em uma FiguraInterface.
Nenhum dos parâmetros podem ser nulos.
A função é definida na interface, permitindo que seja utilizada posteriormente.
*/
void figura_registrar_definir_cor_preenchimento(
    FiguraInterface figuraInterface,
    void (*definir_cor_preenchimento)(Figura figura, const char *cor_preenchimento));

/*
Registra a função destruir em uma FiguraInterface.
Nenhum dos parâmetros podem ser nulos.
A função é definida na interface, permitindo que seja utilizada posteriormente.
*/
void figura_registrar_destruir(FiguraInterface figuraInterface, void (*destruir)(Figura figura));

/*
Escreve todas as informações presentes em uma figura em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void figura_escrever_informacoes(Figura figura, FILE *arquivo);

/*
Escreve o código svg necessário para representar uma figura em um arquivo.
Nenhum dos parâmetros podem ser nulos. O arquivo deve estar aberto para escrita!
*/
void figura_escrever_svg(Figura figura, FILE *arquivo);

/*
Obtém uma string que representa o tipo da figura.
O parâmetro figura não pode ser nulo.
Retorna uma string que representa o tipo da figura.
*/
const char *figura_obter_tipo(Figura figura);

/*
Obtém o id de uma figura.
O parâmetro figura não pode ser nulo.
Retorna o id de uma figura.
*/
const char *figura_obter_id(Figura figura);

/*
Obtém a coordenada x de uma figura.
O parâmetro figura não pode ser nulo.
Retorna a coordenada x de uma figura.
*/
double figura_obter_x(Figura figura);

/*
Obtém a coordenada y de uma figura.
O parâmetro figura não pode ser nulo.
Retorna a coordenada y de uma figura.
*/
double figura_obter_y(Figura figura);

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
double figura_obter_x_centro(Figura figura);

/*
Obtém a coordenada y onde o centro de uma figura se encontra.
O parâmetro figura não pode ser nulo.
Retorna a coordenada y onde o centro de uma figura se encontra.
*/
double figura_obter_y_centro(Figura figura);

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
Libera a memória alocada por uma figura.
O parâmetro figura não pode ser nulo e deve apontar para um espaço de memória reservada.
Libera a memória alocada. O ponteiro não poderá ser utilizado após isso!
*/
void figura_destruir(Figura figura);

#endif
