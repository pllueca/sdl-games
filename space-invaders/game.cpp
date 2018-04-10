#include <SDL.h>
#include <SDL2/SDL_ttf.h>

#include <cmath>                     
#include <ctime>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

#include "common.h"
#include "entity.h"



SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;


bool playing = false;
bool paused = false;

class Bullet{
    public:
    int x;
    int y;
    int width=3;
    int height= 5;
    int speed = 8;
    bool alive;
    Direction direction;
    Bullet() {}
    Bullet(int x, int y){
        this->x = x;
        this->y = y;
        direction = Direction::up;
        alive=true;
    }
    void draw(SDL_Renderer *renderer) {
            SDL_SetRenderDrawColor(renderer, 255,255,255,128);
            SDL_Rect bullet_rect = {x, y, width, height};
            SDL_RenderFillRect(renderer, &bullet_rect);
    }
    void update() {
        y -= speed;
        if (y <= 0){
            alive = false;
        }
    }
};


Player player;
vector<Bullet> bullets;
vector<Invader> invaders;

void init(){
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    //SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL); // key repetition

    window = SDL_CreateWindow( // name, x, y, w, h, flags
        "Space invaders",
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ); 


    SDL_PumpEvents();
    const Uint8 * tmp_keystate = SDL_GetKeyboardState(NULL);
    memcpy(&current_keystate, &tmp_keystate, sizeof tmp_keystate);

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
    SDL_PumpEvents();
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
