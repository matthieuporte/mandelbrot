# Makefile

# Flags for implicit rules
CC = gcc -fsanitize=address
CPPFLAGS = -MMD
CFLAGS = -g -lm -fsanitize=address -Wall -Wconversion -Wextra `pkg-config --cflags gtk+-3.0`
LDLIBS = `pkg-config --libs gtk+-3.0`


OBJ = main.o gui.o
DEP = ${OBJ:.o=.d}

all: main

main: ${OBJ}

.PHONY: clean
clean:
	${RM} ${OBJ} ${DEP} main

-include ${DEP}

# END
