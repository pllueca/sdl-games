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


Player *player;
vector<Bullet*> bullets;
vector<Invader*> invaders;

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


    player = new Player( (WINDOW_WIDTH / 2) - 15, (WINDOW_HEIGHT - 90), 30, 30);
    for (int i=0; i < NUM_INVADERS; i++){
        invaders.push_back(new Invader(50 + (30*i),50, Direction::right));
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
    player->update();
    
    // create new bullet
    if (player->shot){
        bullets.push_back(new Bullet( player->x + (player->width/2) , player->y - 3));
        player -> shot = false;
    }

    // invader updates
    for (Invader * invader : invaders) {
        if (invader -> alive)
            invader->update();
    }

    for (Bullet * bullet: bullets) {
        if (bullet -> alive)
            bullet -> update();
    }
    
    // check intersections
    for (Bullet * bullet: bullets) {
        for (Invader * invader : invaders) {
            if (invader->alive && bullet->collides(&invader) {
                bullet -> alive = false;
                invader -> alive = false;
                break;
            }
        }
    }
}

void draw() {
    // draw background
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);

    for (Invader * invader : invaders) {
        if (invader -> alive)
            invader->draw(renderer);
    }

    for (Bullet * bullet: bullets) {
        if (bullet -> alive)
            bullet -> draw(renderer);
    }
    player->draw(renderer);

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
