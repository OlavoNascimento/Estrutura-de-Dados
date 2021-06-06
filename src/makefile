# Nome do executável
EXECUTABLE = siguel

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -fstack-protector-all -std=c99 -O3 -W -Wall -Wno-stringop-overread
# Flags de linking
LDFLAGS = -lm

# Recebe todos os arquivos .c
SOURCES = $(wildcard *.c */*.c */*/*.c)
# Substitui a extensão dos arquivos .c para .o
OBJECTS = $(SOURCES:.c=.o)

all: $(EXECUTABLE)

# Opções para debug
# -g inclui informação de debug, permite o valgrind exibir informações de linha e nome de função.
# -D DEBUG_SIGUEL habilita os macros de debug do programa.
debug: CFLAGS += -g -D DEBUG_SIGUEL
debug: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo "Criando executável..."
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

# Específica que cada arquivo .o deve ser gerado a partir de seu correspondente .c
%.o: %.c
# $@ é o alvo atual (arquivo .o)
# $^ representa a dependência atual (arquivo .c)
	@echo "Compilando $^..."
	gcc $(CFLAGS) -I . -c -o $@ $^

clean:
	rm -f *.o */*.o */*/*.o

