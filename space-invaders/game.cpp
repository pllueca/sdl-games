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
#include "render.h"



SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;


bool playing = false;
bool paused = false;
bool win=false;


Player *player;
vector<Bullet*> bullets;
vector<Invader*> invaders;

long long int frame_num;

void init(){
    log_info("Initialitising stuff");
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    //SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL); // key repetition
    log_info("creating window");
    window = SDL_CreateWindow( // name, x, y, w, h, flags
        "Space invaders",
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);

    frame_num = 0;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ); 


    SDL_PumpEvents();
    const Uint8 * tmp_keystate = SDL_GetKeyboardState(NULL);
    memcpy(&current_keystate, &tmp_keystate, sizeof tmp_keystate);

    playing = true;
    paused = false;
    SDL_ShowCursor(0);


    log_info("Creating player and invaders");
    player = new Player( (WINDOW_WIDTH / 2) - 15, (WINDOW_HEIGHT - 90), 30, 30);
    for (int i=0; i < NUM_INVADERS; i++){
        invaders.push_back(new Invader(50 + (30*i),50, Direction::right));
    }
    log_info("Created %d invaders", invaders.size());

    
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
    ++frame_num;
    if ((frame_num % 120) == 0) {
        log_info("%d alive invaders", invaders.size());
        log_info("%d alive bullets", bullets.size());
    }

    //handle player updates

    player->update();
    
    // create new bullet
    if (player->shot){
        log_info("Player shooting: %d, bullets: %d", player->shot, bullets.size());
        bullets.push_back(new Bullet( player->x + (player->width/2) , player->y - 3));
        player -> shot = false;
    }

    // invader updates
    //for (Invader * invader : invaders) {
    for(int i=0; i < invaders.size(); ++i){
        Invader *invader = invaders[i];
        if (invader -> alive){
            invader->update();
        } else {
            // swap to last invader and pop vector
            Invader * tmp = invaders[invaders.size() - 1];
            invaders[invaders.size() - 1] = invader;
            invaders[i] = tmp;
            invaders.pop_back();
        }
    }

    for (int i = 0; i < bullets.size(); ++i){
        Bullet * bullet = bullets[i];
        if (bullet -> alive){
            bullet -> update();
        } else {
            Bullet * tmp = bullets[bullets.size() - 1];
            bullets[bullets.size() - 1] = bullet;
            bullets[i] = tmp;
            bullets.pop_back();
        }
    }
    
    // check intersections
    for (Bullet * bullet: bullets) {
        for (Invader * invader : invaders) {
            if (invader->alive && bullet->collides(invader)) {
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

void draw_win() {
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);

    SDL_Texture * font_text = renderText("You Win!",
                             "sketchy.ttf",
                             {255, 0, 0},
                             40,
                             renderer);
    renderTexture(font_text,
              renderer,
              WINDOW_WIDTH / 2,
              WINDOW_HEIGHT / 2);
    
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

        if (invaders.size() == 0)
            win=true;
        if(win)
            draw_win();
        else
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
