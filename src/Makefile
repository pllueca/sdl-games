CC = g++
CLIBS = lSDL2 -lSDL2_ttf

all: pong
pong: pongSDL.cpp utilPong.h
	$(CC) pongSDL.cpp -o pong -$(CLIBS)

clean: pong
	rm pong
