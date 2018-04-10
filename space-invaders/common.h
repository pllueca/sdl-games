#pragma once

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int NUM_INVADERS = 5;

#define TICK_INTERVAL    30
static Uint32 next_time;

Uint32 time_left(void) { 
    Uint32 now;
    now = SDL_GetTicks();
    if(next_time <= now)
        return 0;
    else
        return next_time - now;
}

enum Direction {none=0, up=1, down=2, left=3, right=4};

// arrays for storing the state of the keyboad
const Uint8 *current_keystate;
Uint8 *previous_keystate;
