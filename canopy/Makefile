# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2` -lm

all: static dynamic

SRC = plain.c static.c dynamic.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

plain: plain.o
static: static.o
dynamic: dynamic.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
