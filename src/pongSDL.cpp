/* Classic game Pong implemented using SDL 2.0.3 */

#include <SDL2/SDL.h>
#include <cmath>                      
#include <ctime>                      

// Window attributes
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int STEP = 9;

const int PADDLE_WIDTH = 30;
const int PADDLE_HEIGHT = 180;
const int BALL_WIDTH = 15;


int points_j1;
int points_j2;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;



bool playing = true;

struct ball {
    int x;
    int y;
    int vx;
    int vy;
    int dim;
    
    double angle(){
        return atan(sqrt(pow(vx,2) + pow(vy,2)));
    }
};

struct paddle {
    int x;
    int y;    
};

paddle left_paddle, right_paddle;
ball ball1;


// Check colisions 
// not implemented
bool checkCollisionLeft(){
    if(ball1.x + ball1.vx <= 0)
        return true;
    return false;
}

bool checkCollisionRight(){
    if(ball1.x + ball1.vx + BALL_WIDTH >= WINDOW_WIDTH)
        return true;
    return false;
}

bool checkCollisionUpDown(){
    if(ball1.y + ball1.vy <= 0 || ball1.y + ball1.vy + BALL_WIDTH >= WINDOW_HEIGHT)
        return true;
    return false;
}


//******************************************************
void init(){
    SDL_Init(SDL_INIT_EVERYTHING);

    //SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL); // key repetition

    window = SDL_CreateWindow( // name, x, y, w, h, flags
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
    ball1.x = WINDOW_WIDTH/2;
    ball1.y = WINDOW_HEIGHT/2;
    ball1.dim = BALL_WIDTH;
    ball1.vy = -10 + rand() % 20;
    ball1.vx = -10 + rand() % 20;

    points_j1 = 0;
    points_j2 = 0;
    SDL_ShowCursor(0);
    
}

int mov1, mov2;

void handle_input(){
    bool keydown = false;
    while(SDL_PollEvent(&event) != 0){
        switch(event.type){
            case SDL_QUIT:
                playing = false;
                break;
            case SDL_KEYDOWN:
                keydown = true;
                if(event.key.keysym.sym == SDLK_UP){
                    mov1 = -1;
                }
                else if(event.key.keysym.sym == SDLK_DOWN){
                    mov1 = 1;
                }
                else
                    mov1 = 0;
                  
                if(event.key.keysym.sym == SDLK_w){
                    mov2 = -1;
                }
                else if(event.key.keysym.sym == SDLK_s)
                    mov2 = 1;
                else
                    mov2 = 0;
                   
                break;
        }
    }
    if(!keydown)
        mov1 = mov2 = 0;
}

void update(){
    // update las palas
    right_paddle.y += STEP * mov1;
    if(right_paddle.y < 0)
        right_paddle.y = 0;
    else if(right_paddle.y + PADDLE_HEIGHT > WINDOW_HEIGHT)
        right_paddle.y = WINDOW_HEIGHT - PADDLE_HEIGHT;

    left_paddle.y += STEP*mov2;
    if(left_paddle.y < 0)
        left_paddle.y = 0;
    else if(left_paddle.y + PADDLE_HEIGHT > WINDOW_HEIGHT)
        left_paddle.y = WINDOW_HEIGHT - PADDLE_HEIGHT;

    // update la bola
    if(checkCollisionLeft()){
        ++points_j2;
        ball1.vx = -ball1.vx;
        ball1.vy = ball1.vy;
    }
    else if(checkCollisionRight()){
        ++points_j1;
        ball1.vx = -ball1.vx;
        ball1.vy = ball1.vy;              
    }
    else if (checkCollisionUpDown()){
        ball1.vx = ball1.vx;
        ball1.vy = -ball1.vy;
    }
    else{
        ball1.x += ball1.vx;
        ball1.y += ball1.vy;
    }

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
    SDL_SetRenderDrawColor(renderer, 0,0,255,255);
    SDL_Rect bola = {ball1.x, ball1.y,
                     ball1.dim, ball1.dim};
    SDL_RenderFillRect(renderer, &bola);
    

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
