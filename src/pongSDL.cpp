/* Classic game Pong implemented using SDL 2.0.3 */

#include <SDL2/SDL.h>
#include <cmath>                      
#include <ctime>                      

// Window attributes
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int STEP = 5;

const int PADDLE_WIDTH = 30;
const int PADDLE_HEIGHT = 180;

Uint32 rmask = 0x000000ff;
Uint32 gmask = 0x0000ff00;
Uint32 bmask = 0x00ff0000;
Uint32 amask = 0xff000000;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;

int desp_l = 210;
int desp_r = 210;

// game loop
bool playing = true;

struct ball {
    int x;
    int y;
    int velx;
    int vely;
    int rad;
};

struct paddle {
    int x;
    int y;    
};

paddle left_paddle, right_paddle;
ball ball1;

/* dibuja src encima de dest */
void apply_surface(int x, int y, SDL_Surface* src, SDL_Surface* dest) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(src,NULL,dest,&offset);
}


//******************************************************
void init(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("No se ha podido inicializar SDL2!\nError: %s\n", SDL_GetError());
        exit(1);
    }
    //SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL); // key repetition
    window = SDL_CreateWindow( // x, y, w, h, flags
        "Pong",
        SDL_WINDOWPOS_CENTERED, // window centrada en la pantalla
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);

    // inicializa el renderer que pintara las cosas
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ); 

    // posicion inicial de las palas
    left_paddle.x = PADDLE_WIDTH;
    left_paddle.y = WINDOW_HEIGHT/2 - (PADDLE_HEIGHT/2);
    right_paddle.y = WINDOW_HEIGHT/2 - (PADDLE_HEIGHT / 2);
    right_paddle.x = WINDOW_WIDTH - 2*PADDLE_WIDTH;

    SDL_ShowCursor(0);
    
}

void handle_input(){
    while(SDL_PollEvent(&event) != 0){
        switch(event.type){
            case SDL_QUIT:
                playing = false;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_UP){
                    right_paddle.y -= STEP;
                    if(right_paddle.y < 0)
                        right_paddle.y = 0;
                }
                else if(event.key.keysym.sym == SDLK_DOWN){
                    right_paddle.y += STEP;                 
                    if(right_paddle.y + PADDLE_HEIGHT > WINDOW_HEIGHT)
                        right_paddle.y = WINDOW_HEIGHT - PADDLE_HEIGHT;
                }
                    
                if(event.key.keysym.sym == SDLK_w){
                    left_paddle.y -= STEP;
                    if(left_paddle.y < 0)
                        left_paddle.y = 0;
                }
                else if(event.key.keysym.sym == SDLK_s){
                   
                }
                    
                break;
        }
    }
}

void update(){
    
}


// dibuja la escena
void draw(){
    // pinta toda la escena de blanco
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);

    // pinta las palas de rojo
    SDL_SetRenderDrawColor(renderer, 255,0,0,255);
    SDL_Rect pala1 = {left_paddle.x, left_paddle.y,
                      PADDLE_WIDTH, PADDLE_HEIGHT};
    SDL_RenderFillRect(renderer, &pala1);
    SDL_Rect pala2 = {right_paddle.x, right_paddle.y,
                      PADDLE_WIDTH, PADDLE_HEIGHT};
    SDL_RenderFillRect(renderer, &pala2);

    // pinta la bola
    // ...
    

    SDL_RenderPresent(renderer);
}

void clean(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();  
}

void gameLoop(){
    while(playing){
        handle_input();
        update();
        draw();
    }
    clean();

}

int main(){
    srand(time(NULL));
    init();
    gameLoop();
    return 0;   
}
