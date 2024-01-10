# Makefile

# Flags for implicit rules
CC = gcc -lm
CPPFLAGS = -MMD
CFLAGS = -g -lm `pkg-config --cflags gtk+-3.0`
LDLIBS = `pkg-config --libs gtk+-3.0` -lm


OBJ = main.o gui.o draw.o mandelbrot.o palette.o save.o
DEP = ${OBJ:.o=.d}
EXE = mandelbrot-explorer
GLADE = glade/app.glade

all: $(EXE)

$(EXE): ${OBJ}
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

install:
	mkdir -p $(DESTDIR)usr/bin
	mkdir -p $(DESTDIR)usr/share/$(EXE)
	cp $(GLADE) $(DESTDIR)usr/share/$(EXE)
	cp $(EXE) $(DESTDIR)usr/bin

.PHONY: clean
clean:
	${RM} ${OBJ} ${DEP} $(EXE)

-include ${DEP}

# END
