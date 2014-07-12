CC = g++
CLIBS = lSDL2 -lSDL_ttf
CFLAGS = -std=c++11

all: pong
pong: pongSDL.cpp utilPong.h
	$(CC) -$(CFLAGS) pongSDL.cpp -o pong -$(CLIBS) 

clean: pong
	rm pong
