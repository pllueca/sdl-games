CC = g++
CLIBS = lSDL2 -lSDL_ttf
CLIBS_MAC = I/Library/Frameworks/SDL2.framework/Headers 

all: pong
pong: pongSDL.cpp utilPong.h
	$(CC) -$(CFLAGS) pongSDL.cpp -o pong -$(CLIBS_MAC) -ILibrary/Frameworks/SDL2_ttf.framework/Headers
CFLAGS = -std=c++11 

clean: pong
	rm pong
