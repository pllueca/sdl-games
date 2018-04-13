#pragma once
#include <set>
#include "SDL.h"
#include <SDL2/SDL_ttf.h>
#include "common.h"
#include "geometry.h"


class Entity {
  public:
    int x;
    int y;
    int width;
    int height;
    int speed;
    int color[4];
    bool alive;
    Direction direction;
    Direction previous_direction;
    
    Entity() {}
    ~Entity() {}
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

    bool collides(Entity * other) const{
      SDL_Rect this_rect, other_rect;
      this_rect.x = this->x;
      this_rect.y = this->y;
      this_rect.w = this->width;
      this_rect.h = this->height;
      other_rect.x = other->x;
      other_rect.y = other->y;
      other_rect.w = other->width;
      other_rect.h = other->height;
      return SDL_HasIntersection(&this_rect, &other_rect);
    }


};

class Player : public Entity {
  public:
    int max_speed = 5;
    int inital_speed = 2;
    bool shot;

    Player() {}
    Player(int x, int y, int width, int height){
      this->x = x;
      this->y = y;
      this->width = width;
      this->height = height;
      shot = false;
      direction = Direction::none;
      previous_direction = Direction::none;
      alive=true;
      int tmp_color[4] = {255, 0, 0, 255};
      memcpy(&color, &tmp_color, sizeof tmp_color);
    }
    void update() {
      // fire
      if (current_keystate[SDL_SCANCODE_SPACE]){
        shot = true;
      }
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


    }
};

class Invader: public Entity {
  public:
    int max_steps_down = 8;
    int current_steps_down;
    Invader() {}
    Invader(int x, int y, Direction direction) {
      this -> x = x;
      this -> y = y;
      this -> direction = direction;
      width = 15;
      height = 15;
      speed=7;
      alive = true;
      int tmp_color[4] = {0, 0, 255, 255};
      memcpy(&color, &tmp_color, sizeof tmp_color);
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

class Bullet: public Entity{
    public:
        Bullet() {}
    Bullet(int x, int y){
        this->x = x;
        this->y = y;
        width = 1;
        height = 3;
        speed=5;
        direction = Direction::up;
        int tmp_color[4] = {0, 0,0, 255};
        memcpy(&color, &tmp_color, sizeof tmp_color);
    }
    void update() {
        y -= speed;
        if (y <= 0){
            alive = false;
        }
    }
};

