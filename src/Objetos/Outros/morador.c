#include "morador.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Estruturas/lista.h"
#include "../../Estruturas/quadtree.h"
#include "../../Estruturas/tabelahash.h"
#include "../../Utils/logging.h"
#include "morador.h"

typedef struct {
    // TODO adicionar interface
    char cpf[100];
    char nome[100];
    char sobrenome[100];
    char sexo;
    int dia;
    int mes;
    int ano;

    // endereço
    char cep[100];
    char face;
    int num;
    char complemento[100];
} MoradorImp;

const char *morador_obter_tipo() {
    return "morador";
}

// Cria e inicializa um struct MoradorImp com os valores passados.
Morador morador_criar(const char cpf[100], const char nome[100], const char sobrenome[100],
                      const char sexo, int dia, int mes, int ano) {
    if (cpf == NULL) {
        LOG_ERRO("Não é possível criar um morador com cpf NULL!\n");
        return NULL;
    }
    if (nome == NULL) {
        LOG_ERRO("Não é possível criar um morador com nome NULL!\n");
        return NULL;
    }
    if (sobrenome == NULL) {
        LOG_ERRO("Não é possível criar um morador com sobrenome NULL!\n");
        return NULL;
    }
    MoradorImp *moradorImp = malloc(sizeof *moradorImp);
    stpcpy(moradorImp->cpf, cpf);
    stpcpy(moradorImp->nome, nome);
    stpcpy(moradorImp->sobrenome, sobrenome);
    stpcpy(moradorImp->sexo, sexo);
    moradorImp->dia = dia;
    moradorImp->mes = mes;
    moradorImp->ano = ano;

    return moradorImp;
}

// Cria um morador com base em informações de uma linha.
Morador morador_ler(const char *linha) {
    char cpf[100];
    char nome[100];
    char sobrenome[100];
    char sexo;
    int dia;
    int mes;
    int ano;
    sscanf(linha, "%*s %s %s %s %c %d %*c %d %*c %d", cpf, nome, sobrenome, &sexo, &dia, &mes,
           &ano);
    return morador_criar(cpf, nome, sobrenome, sexo, dia, mes, ano);
}

void morador_ler_endereco(Morador morador, const char *linha) {
    char cep[100];
    char complemento[100];
    char face;
    int num;

    sscanf(linha, "%*c %*s %s %c %d %s", cep, face, num, complemento);
    morador_definir_endereco(morador, cep, face, num, complemento);
}

void morador_definir_endereco(Morador morador, const char *cep, const char face, int num,
                              const char *complemento) {
    MoradorImp *moradorImp = morador;
    stpcpy(moradorImp->cep, cep);
    stpcpy(moradorImp->complemento, complemento);
    moradorImp->face = face;
    moradorImp->num = num;
}

const char *morador_obter_id(Morador morador) {
    MoradorImp *moradorImp = morador;
    return moradorImp->cep;
}

const char *morador_obter_nome(Morador morador) {
    MoradorImp *moradorImp = morador;
    return moradorImp->nome;
}

const char *morador_obter_sobrenome(Morador morador) {
    MoradorImp *moradorImp = morador;
    return moradorImp->sobrenome;
}

const char morador_obter_sexo(Morador morador) {
    MoradorImp *moradorImp = morador;
    return moradorImp->sexo;
}

const char *morador_obter_data(Morador morador) {
    MoradorImp *moradorImp = morador;
    char buffer[20];
    if (moradorImp->dia < 10) {
        if (moradorImp->mes < 10) {
            snprintf(buffer, sizeof(buffer), "0%d/0%d/%d", moradorImp->dia, moradorImp->mes,
                     moradorImp->ano);
        } else {
            snprintf(buffer, sizeof(buffer), "0%d/%d/%d", moradorImp->dia, moradorImp->mes,
                     moradorImp->ano);
        }
    } else if (moradorImp->mes < 10) {
        snprintf(buffer, sizeof(buffer), "%d/0%d/%d", moradorImp->dia, moradorImp->mes,
                 moradorImp->ano);
    } else {
        snprintf(buffer, sizeof(buffer), "%d/%d/%d", moradorImp->dia, moradorImp->mes,
                 moradorImp->ano);
    }
    return buffer;
}

const char *morador_obter_endereco_cep(Morador morador) {
    MoradorImp *moradorImp = morador;
    return moradorImp->cep;
    char buffer[100];
}

const char morador_obter_endereco_face(Morador morador) {
    MoradorImp *moradorImp = morador;
    return moradorImp->face;
    char buffer[100];
}

int morador_obter_endereco_num(Morador morador) {
    MoradorImp *moradorImp = morador;
    return moradorImp->num;
    char buffer[100];
}

// Libera a memória utilizada por um morador.
void morador_destruir(Morador morador) {
    MoradorImp *moradorImp = morador;
    free(moradorImp);
}
