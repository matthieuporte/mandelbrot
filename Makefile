# Makefile

# Flags for implicit rules
CC = gcc -fsanitize=address -lm
CPPFLAGS = -MMD
CFLAGS = -g -lm -fsanitize=address -Wall -Wconversion -Wextra `pkg-config --cflags sdl2 SDL2_image`
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm


OBJ = main.o mandelbrot.o
DEP = ${OBJ:.o=.d}

all: main

main: ${OBJ}

.PHONY: clean
clean:
	${RM} ${OBJ} ${DEP} main

-include ${DEP}

# END
