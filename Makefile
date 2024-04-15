CFLAGS = -g -Wall -Wextra
CC = gcc

# ARQUIVOS e OBJETOS sao variaveis
ARQUIVOS = ordena.c auxiliar.c funcFiltros.c io.c csvreader.c

# $(ARQUIVOS:.c=.o) todo arquivo .c vira .o e é guardado no OBJETOS
OBJETOS = $(ARQUIVOS:.c=.o)

# Nome do executavel
EXECUTAVEL = csvreader

all: $(EXECUTAVEL)

# $@ gera arquivo executavel
$(EXECUTAVEL): $(OBJETOS)
	$(CC) $(OBJETOS) -o $@ -lm

# % é um curinga, %.o é qualquer arquivo .o e assim para %.c
# $< sao os arquivos do tipo "arquivo.c"
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJETOS)

purge:
	rm -f $(OBJETOS) $(EXECUTAVEL)