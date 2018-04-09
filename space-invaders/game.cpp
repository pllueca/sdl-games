#include "SDL.h"
#include <SDL2/SDL_ttf.h>

#include <cmath>                     
#include <ctime>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

// constants
//
//
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;

bool playing = false;
bool paused = false;

enum Direction {none=0, up=1, down=2, left=3, right=4};

class Invader {
public:
    int x;
    int y;
    int width;
    int height;
    int speed;
    int max_steps_down = 5;
    int current_steps_down;
    
    Direction direction;
    Direction last_direction = Direction::none;

    Invader() {
        x=0;
        y=0;
        width=0;
        height=0;
        direction = none;
        speed= 5;

    }

    void update() {
        switch(direction) {
            case Direction::left:
                if (x - speed <= 0) {
                    x = 0;
                    direction = Direction::down;
                    last_direction = Direction::left;
                    current_steps_down = 0;
                }
                x = max(0, x - speed);
                break;

            case Direction::right:
                if (x + speed >= WINDOW_WIDTH) {
                    x = WINDOW_WIDTH;
                    direction = Direction::down;
                    last_direction = Direction::right;
                    current_steps_down = 0;
                } else {
                    x += speed;
                }
                break;

            case Direction::up:
                y = max(0, y - speed);
                break;

            case Direction::down:
                y = min(WINDOW_HEIGHT, y + speed);
                current_steps_down += 1;
                if (current_steps_down >= max_steps_down) {
                    if(last_direction == Direction::left)
                        direction = Direction::right;
                    else
                        direction = Direction::left;
                }
                break;
            case none:
                break;

        }
        //printf("Current pos: (%d, %d), speed: %d direction: %u\n", x, y, speed, direction);
    }
};

vector<Invader> invaders;

void init(){
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    //SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL); // key repetition

    window = SDL_CreateWindow( // name, x, y, w, h, flags
        "Space invaders",
        SDL_WINDOWPOS_CENTERED, // window centrada en la pantalla
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);

    // inicializa el renderer que pintara las cosas
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ); 

    playing = true;
    paused = false;
    SDL_ShowCursor(0);


    Invader i;
    i.x = 50;
    i.y = 50;
    i.width = 15;
    i.height = 15;
    i.direction = Direction::right;
    invaders.push_back(i);
    
}

void handle_input(){
    bool keydown = false;
    while(SDL_PollEvent(&event) != 0){
        switch(event.type){
            case SDL_QUIT:
                playing = false;
                break;
        }
    }
}

void update() {
    for (Invader & invader : invaders) {
        invader.update();
    }
}

void draw() {
    // draw background
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255,0,0,255);
    for (const Invader & invader : invaders) {
        SDL_Rect invader_rect = {invader.x, invader.y, invader.width, invader.height};
        SDL_RenderFillRect(renderer, &invader_rect);
    }

    SDL_RenderPresent(renderer);
}

void clean(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();  
}

void game_loop(){
    init();
    while(playing){
        handle_input();
        update();
        draw();
    }
    clean();
}


int main(int argc, char *argv[]){
    srand(time(NULL));
    game_loop();
    return 0;   
}
