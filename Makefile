COMPILER = gcc
FLAGS = -std=gnu11 -Wall -Werror
GTK_LIBS = pkg-config --libs gtk+-3.0
GTK_CFLAGS =  pkg-config --cflags gtk+-3.0

all: dirs bin/exengl

bin/exengl: build/code/main.o
	$(COMPILER) $(FLAGS) `$(GTK_CFLAGS)` -o $@ $^ `$(GTK_LIBS)`

build/code/main.o: src/code/main.c
	$(COMPILER) $(FLAGS) `$(GTK_CFLAGS)`  -c -o $@ $< `$(GTK_LIBS)`

dirs:
	mkdir -p build/code
	mkdir -p bin

clean:
	rm -rf build bin