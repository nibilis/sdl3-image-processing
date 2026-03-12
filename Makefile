# Nome do executável
TARGET = main

# Compilador
CC = gcc

# Diretórios
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Bibliotecas SDL
CFLAGS = -Wall -Wextra -I$(INC_DIR) `pkg-config --cflags sdl3 sdl3-image`
LDFLAGS = `pkg-config --libs sdl3 sdl3-image`

# Arquivos fonte
SRC = $(wildcard $(SRC_DIR)/*.c)

# Arquivos objeto
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Regra principal
all: $(TARGET)

# Linkagem final
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compilação dos objetos
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpar arquivos compilados
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Recompilar tudo
rebuild: clean all

.PHONY: all clean rebuild