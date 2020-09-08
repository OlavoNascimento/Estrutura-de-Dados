#ifndef DEBUG_SIGUEL_H
#define DEBUG_SIGUEL_H

#ifdef DEBUG_SIGUEL
    #define LOG_INFO(fmt, ...) printf(fmt, ##__VA_ARGS__)
    #define LOG_ERROR(fmt, ...) fprintf(stderr, "ERRO: " fmt, ##__VA_ARGS__)
#else
    #define LOG_INFO(fmt, ...)
    #define LOG_ERROR(fmt, ...)
#endif

#endif
