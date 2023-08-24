CC = gcc
FLAGS = -O0 -g -Wfatal-errors -Wall -pedantic -std=c17
INCLUDE = -Iinclude

LIB = ./bin/libctl.a
SHARED_LIB = ./bin/libctl.so

SRC  = $(wildcard src/*.c)
EXAMPLE_SRC = $(wildcard examples/*.c)

OBJ = $(SRC:src/%.c=bin/%.o)
EXAMPLE_OUT = $(EXAMPLE_SRC:examples/%.c=bin/%.out)

.PHONY: all

all: build

release: FLAGS = -O3 -std=c17
release: build

examples: build $(EXAMPLE_OUT)

build: $(OBJ)
	ar rcs $(LIB) $(OBJ)
	$(CC) -shared -o $(SHARED_LIB) $(OBJ)

clean:
	rm $(OBJ)
	rm $(LIB) $(SHARED_LIB)
	rm $(EXAMPLE_OUT)

bin/%.out: examples/%.c
	$(CC) -o $@ $< $(SHARED_LIB) $(FLAGS) $(INCLUDE)

bin/%.o: src/%.c include/%.h
	$(CC) -o $@ -c $< $(FLAGS) $(INCLUDE)
