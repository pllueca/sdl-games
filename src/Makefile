CC = g++
CLIBS = lSDL2 -lSDL_ttf
CLIBS_MAC = I/Library/Frameworks/SDL2.framework/Headers	-I/Library/Frameworks/SDL2_ttf.framework/Headers	
CFLAGS = -std=c++11 

all: pong
pong: pongSDL.cpp utilPong.h
	$(CC) pongSDL.cpp -o $(CFLAGS) -$(CLIBS)

pongmac: pongSDL.cpp utilPong.h
	$(CC) pongSDL.cpp -c $(CFLAGS) -$(CLIBS_MAC)
	$(CC) pongSDL.o -o pong -framework SDL2 -framework SDL2_ttf

clean: pong
	rm pong
