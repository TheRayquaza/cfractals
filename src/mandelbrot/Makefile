# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2` -lm

all: mandelbrot

SRC = mandelbrot.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

mandelbrot: mandelbrot.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
