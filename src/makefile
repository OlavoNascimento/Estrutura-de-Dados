# Nome do executável
EXECUTABLE = siguel

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -fstack-protector-all -std=c99 -W -Wall
LDFLAGS = -lm

# Recebe todos os arquivos .c
SOURCES = $(wildcard *.c)
# Substitui a extensão dos arquivos .c para .o
OBJECTS = $(SOURCES:.c=.o)

all: $(EXECUTABLE)

# Opções para debug
debug: CFLAGS += -g
debug: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo "Criando executável..."
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

# Específica que cada arquivo .o deve ser gerado a partir
# de seu correspondente .c
%.o: %.c
# $@ é o alvo atual (arquivo .o)
# $^ representa a dependência atual (arquivo .c)
	@echo "Compilando $^..."
	gcc $(CFLAGS) -I . -c -o $@ $^

clean:
	rm -f *.o
