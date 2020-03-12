# Flags de compilação
CFLAGS = -fstack-protector-all -std=c99

# Recebe todos os arquivos .c
SOURCES = $(wildcard *.c)
# Substitui a extensão dos arquivos .c para .o
OBJECTS = $(SOURCES:.c=.o)

all: siguel

siguel: $(OBJECTS)
	@echo "Criando executavel..."
	gcc -o $@ $(OBJECTS)

# Específica que cada arquivo .o deve ser gerado a partir
# de seu correspondente .c
%.o: %.c
# $@ é o alvo atual (arquivo .o)
# $^ representa a dependência atual (arquivo .c)
	@echo "Compilando $^..."
	gcc $(CFLAGS) -I . -c -o $@ $^

clean:
	rm -f *.o
