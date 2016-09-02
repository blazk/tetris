OBJ = main.o piece.o game.o board.o ai.o gfx_gl.o timing.o scores.o


CFLAGS = -g -Wall `sdl-config --cflags` -I./ -I/usr/X11R6/include/
CPPFLAGS =
LIB= -lcurses
LDFLAGS=`sdl-config --libs` -lGL -lGLU -L/usr/X11R6/lib -lSDL


all: tetris


tetris: $(OBJ)
	$(CC) -o $@ $(OBJ) $(LIB) $(LDFLAGS)


main.o:     game.h piece.h scores.h gfx.h timing.h
game.o:     piece.h timing.h
board.o:    board.h piece.h
ai.o:       ai.h game.h
gfx_gl.o:   game.h piece.h scores.h gfx.h timing.h
piece.o:    piece.h
scores.o:   scores.h
timing.o:   timing.h


clean:
	rm -rf $(OBJ) tetris
