#include "SDL.h"
#include <SDL2/SDL_ttf.h>

#include <cmath>                     
#include <ctime>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

// constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int NUM_INVADERS = 5;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;

#define TICK_INTERVAL    30

static Uint32 next_time;

// arrays for storing the state of the keyboad
const Uint8 *current_keystate;
Uint8 *previous_keystate;

Uint32 time_left(void) { 
    Uint32 now;
    now = SDL_GetTicks();
    if(next_time <= now)
        return 0;
    else
        return next_time - now;
}

bool playing = false;
bool paused = false;

enum Direction {none=0, up=1, down=2, left=3, right=4};

class Player {
    public:
        int x;
        int y;
        int width;
        int height;
        int speed = 0;
        int inital_speed=2;
        int max_speed=5;
        Direction direction;
        Direction previous_direction;
        Player() {}
        Player(int x, int y, int width, int height){
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
            this->direction = Direction::none;
            this->previous_direction = Direction::none;
        }

        void update() {
            if (current_keystate[SDL_SCANCODE_A]) {
                if (direction == Direction::left) {
                    speed = min(speed + 1, max_speed);
                } else {
                    direction = Direction::left;
                    speed = inital_speed;
                }
            } else if (current_keystate[SDL_SCANCODE_D]) {
                if (direction == Direction::right){
                    speed = min(speed + 1, max_speed);
                }
                else {
                    direction = Direction::right;
                    speed = inital_speed;
                }
            }
            if(direction != Direction::none && 
              !(current_keystate[SDL_SCANCODE_A] || current_keystate[SDL_SCANCODE_D])){
                direction = Direction::none;
                speed = 0;
            }
            switch(direction){
                case Direction::left:
                    x = max(0, x - speed);
                    break;
                case Direction::right:
                    x = min(WINDOW_WIDTH, x + speed);
                    break;
                default:
                    break;
            }

        }
        void draw(SDL_Renderer *renderer) {
            SDL_SetRenderDrawColor(renderer, 0,255,0,255);
            SDL_Rect player_rect = {x, y, width, height};
            SDL_RenderFillRect(renderer, &player_rect);
        }
};


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
                if (x + speed  + width >= WINDOW_WIDTH) {
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

    void draw(SDL_Renderer *renderer) {
        SDL_SetRenderDrawColor(renderer, 255,0,0,255);
        SDL_Rect invader_rect ={x, y, width, height};
        SDL_RenderFillRect(renderer, &invader_rect);
    }
};

Player player;
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


    // initialize events and keystate
    //SDL_PumpEvents();
    //const Uint8 * tmp_keystate = SDL_GetKeyboardState(NULL);
    //current_keystate = malloc(sizeof tmp_keystate)


    playing = true;
    paused = false;
    SDL_ShowCursor(0);


    player = Player( 
            (WINDOW_WIDTH / 2) - 15,
            (WINDOW_HEIGHT - 90),
            30, 30
    );
    for (int i=0; i < NUM_INVADERS; i++){
        Invader in;
        in.x = 50 + (30*i);
        in.y = 50;
        in.width = 15;
        in.height = 15;
        in.direction = Direction::right;
        invaders.push_back(in);
    }
    
}

void handle_input(){
    bool keydown = false;
    const Uint8 * tmp_keystate = SDL_GetKeyboardState(NULL);
    memcpy(&current_keystate, &tmp_keystate, sizeof tmp_keystate);
    if (current_keystate[SDL_SCANCODE_Q] || current_keystate[SDL_SCANCODE_ESCAPE]) {
        playing = false;
    }

    while(SDL_PollEvent(&event) != 0){
        switch(event.type){
            case SDL_QUIT:
                playing = false;
                break;
        }
    }
}

void update() {
    //handle player updates
    player.update();

    // invader updates
    for (Invader & invader : invaders) {
        invader.update();
    }
}

void draw() {
    // draw background
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);

    for (Invader & invader : invaders) {
        invader.draw(renderer);
    }
    player.draw(renderer);

    SDL_RenderPresent(renderer);
}

void clean(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();  
}

void game_loop(){
    init();
    next_time = SDL_GetTicks() + TICK_INTERVAL;
    while(playing){
        handle_input();
        update();
        draw();

        memcpy(&previous_keystate, &current_keystate, sizeof current_keystate);
        SDL_Delay(time_left());
        next_time += TICK_INTERVAL;
    }
    clean();
}


int main(int argc, char *argv[]){
    srand(time(NULL));
    game_loop();
    return 0;   
}
