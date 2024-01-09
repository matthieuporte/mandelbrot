# Makefile

# Flags for implicit rules
CC = gcc -lm
CPPFLAGS = -MMD
CFLAGS = -g -lm `pkg-config --cflags gtk+-3.0`
LDLIBS = `pkg-config --libs gtk+-3.0` -lm


OBJ = main.o gui.o draw.o mandelbrot.o palette.o save.o
DEP = ${OBJ:.o=.d}

all: main

main: ${OBJ}

.PHONY: clean
clean:
	${RM} ${OBJ} ${DEP} main

-include ${DEP}

# END
