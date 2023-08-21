CC = gcc
FLAGS = -O0 -g -Wfatal-errors -Wall -pedantic -std=c17
INCLUDE = -Iinclude

LIB = ./libctl.a
SHARED_LIB = ./libctl.so

SRC  = $(wildcard src/*.c)

OBJ = $(SRC:%.c=%.o)

OUT = out
CURRENT_EXAMPLE = examples/hashmap.c

.PHONY: all run

all: build run

build: $(OBJ)
	ar rcs $(LIB) $(OBJ)
	$(CC) -shared -o $(SHARED_LIB) $(OBJ)

	$(CC) -o $(OUT) $(CURRENT_EXAMPLE) $(FLAGS) $(INCLUDE) $(SHARED_LIB)

run:
	./$(OUT)

clean:
	rm $(OBJ)
	rm $(OUT)

src/%.o: src/%.c include/%.h
	$(CC) -o $@ -c $< $(FLAGS) $(INCLUDE)
