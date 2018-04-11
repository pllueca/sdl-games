CC = g++
CLIBS = lSDL2 -lSDL_ttf
LIB_SDL = /Library/Frameworks/SDL2.framework/Headers
LIB_SDL_TTF = /Library/Frameworks/SDL2_ttf.framework/Headers
CLIBS_MAC = -I$(LIB_SDL) -I$(LIB_SDL_TTF)
CFLAGS = -std=c++11 
LDFLAGS = -framework SDL2 -framework SDL2_ttf

all: game

game: game.cpp common.h entity.h geometry.h
	$(CC) game.cpp -c $(CFLAGS) $(CLIBS_MAC)
	$(CC) game.o -o game $(LDFLAGS)

clean: 
	rm game
	rm *.o
