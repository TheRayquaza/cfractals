# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2` -lm

all: mandelbrot_static mandelbrot_dynamic

SRC = static.c dynamic.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

mandelbrot_static: static.o
	gcc -o static $(CFLGAS)  static.o $(LDLIBS) 
mandelbrot_dynamic: dynamic.o
	gcc -o dynamic $(CFLAGS) dynamic.o $(LDLIBS)

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
