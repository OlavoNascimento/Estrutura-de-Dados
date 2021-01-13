#ifndef DEBUG_SIGUEL_H
#define DEBUG_SIGUEL_H

#include <stdio.h>

/*
Este módulo define macros auxiliares para logging da execução do programa.
*/

#ifdef DEBUG_SIGUEL
    #define LOG_INFO(fmt, ...) printf(fmt, ##__VA_ARGS__)
    #define LOG_ERRO(fmt, ...) fprintf(stderr, "ERRO: " fmt, ##__VA_ARGS__)
#else
    #define LOG_INFO(fmt, ...)
    #define LOG_ERRO(fmt, ...)
#endif

#endif
