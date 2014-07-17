#ifdef __APPLE__
#include "SDL.h"
#include "SDL_ttf.h"
#else

#endif
#include <string>
#include <iostream>
using namespace std;

void renderTexture(SDL_Texture * tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr){
      SDL_RenderCopy(ren, tex, clip, &dst);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if(clip != nullptr) {
        dst.w = clip->w;
        dst.h = clip->h;
    }
    else
        SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    renderTexture(tex,ren,dst,clip);
}

SDL_Texture* renderText(const string &text, const string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer){
    TTF_Font *font = TTF_OpenFont("font.ttf", 20);
    //aqui no peta
    SDL_Surface *surf = TTF_RenderText_Blended(font, "aaaa", color);
    cerr <<"text rendered"<<endl;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
    
}
