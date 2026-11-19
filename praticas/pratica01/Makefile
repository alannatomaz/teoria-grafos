CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = main
SRC = grafo_matriz.c grafo_lista.c main.c
OBJ = *.o

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) -lm

clean:
	rm -f $(TARGET) $(OBJ)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
