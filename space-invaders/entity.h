#pragma once
#include "SDL.h"
#include <SDL2/SDL_ttf.h>
#include "common.h"


class Entity {
  public:
    int x;
    int y;
    int width;
    int height;
    int speed;
    uint color[4];
    Direction direction;
    Direction previous_direction;

    void draw(SDL_Renderer *renderer) {
        SDL_SetRenderDrawColor(renderer, 
            color[0],
            color[1],
            color[2],
            color[3]
        );
        SDL_Rect rect = {x, y, width, height};
        SDL_RenderFillRect(renderer, &rect);
    }

    virtual void update() = 0;
};

class Player : public Entity {
  public:
    int max_speed = 5;
    int inital_speed = 2;
    uint color[4] = {255, 0, 0, 255};

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
          // Movements
          // while the key is press accelerate until max_speed
          // if not pressed stop
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

          // fire
          if (current_keystate[SDL_SCANCODE_SPACE]){
          }

      }
};

class Invader: public Entity {
  public:
    int max_steps_down = 8;
    int current_steps_down;
    uint color[4] = {0, 0, 255, 255};
    Invader() {}
    Invader(int x, int y, Direction direction) {
      this -> x = x;
      this ->y = y;
      this -> direction = direction;
      speed=7;
    }

    void update() {
        switch(direction) {
            case Direction::left:
                if (x - speed <= 0) {
                    x = 0;
                    direction = Direction::down;
                    previous_direction = Direction::left;
                    current_steps_down = 0;
                }
                x = max(0, x - speed);
                break;

            case Direction::right:
                if (x + speed  + width >= WINDOW_WIDTH) {
                    x = WINDOW_WIDTH;
                    direction = Direction::down;
                    previous_direction = Direction::right;
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
                    if(previous_direction == Direction::left)
                        direction = Direction::right;
                    else
                        direction = Direction::left;
                }
                break;
            case none:
                break;

        }
    }
};
