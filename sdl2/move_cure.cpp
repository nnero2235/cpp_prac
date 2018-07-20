#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include"../nnero/logging.hpp"
#include"sdl2_base.hpp"

using namespace nnero::logging;
using namespace sdl2::base;

void cleanUp(SDL_Window* win,SDL_Renderer* ren,SDL_Texture* tex){
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
}

SDL_Texture* loadTexture(const std::string& file,SDL_Renderer* ren){
    SDL_Texture *tex = IMG_LoadTexture(ren,file.c_str());
    if(tex == nullptr){
        logSDLError("IMG_LoadTexture");
    }
    return tex;
}

void renderCure(SDL_Renderer* ren,SDL_Texture* cure,int x,int y){
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = 80;
    dst.h = 80;
    SDL_RenderCopy(ren,cure,NULL,&dst);
}

void renderCure1(SDL_Renderer* ren,int x,int y){
    if(SDL_SetRenderDrawColor(ren,0x49,0x77,0xac,0xff) != 0){
        logSDLError("SDL_SetRenderDrawColor");
    }
    SDL_Rect cure;
    cure.x = x;
    cure.y = y;
    cure.w = 80;
    cure.h = 80;
    SDL_RenderFillRect(ren,&cure);
}

void renderBG(SDL_Renderer* ren){
    if(SDL_SetRenderDrawColor(ren,0xff,0xff,0xff,0xff) != 0){
        logSDLError("SDL_SetRenderDrawColor");
    }
    SDL_Rect bg;
    bg.x=0;
    bg.y=0;
    bg.h=480;
    bg.w=640;
    SDL_RenderFillRect(ren,&bg);
}

int main(int,char**){
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        logSDLError("SDL init");
        SDL_Quit();
        return 1;
    }
    //init img for speed up loading images at first.
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
	logSDLError("IMG_Init");
	SDL_Quit();
	return 1;
    }
    const int screen_width = 640;
    const int screen_height = 480;

    SDL_Window *win = SDL_CreateWindow("Hello NNERO!",100,100,screen_width,screen_height,SDL_WINDOW_SHOWN);
    if(win == nullptr){
        logSDLError("SDL_CreateWindow");
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    
    SDL_Renderer *ren = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(ren == nullptr){
        logSDLError("SDL_CreateRenderer");
        cleanUp(win,nullptr,nullptr);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    
    // std::string file = getResoucePath()+"biaobai.png";
    // SDL_Texture* cure = loadTexture(file,ren);
    // if(cure == nullptr){
    //     cleanUp(win,ren,nullptr);
    //     IMG_Quit();
    //     SDL_Quit();
    //     return 1;
    // }
    bool quit{false};
    SDL_Event e;
    int x{0};
    int y{0};
    while(!quit){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                quit = true;
            }
            if(e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                case SDLK_RIGHT:
                    if(x < screen_width - 80){
                        x += 80;
                    }
                    break;
                case SDLK_LEFT:
                    if(x > 0){
                        x -= 80;
                    }
                    break;
                case SDLK_UP:
                    if(y > 0) {
                        y -= 80;
                    }
                    break;
                case SDLK_DOWN:
                    if(y < screen_height-80){
                        y += 80;
                    }
                    break;
                case SDLK_q:
                    quit = true;
                    break;
                }
            }
        }
        SDL_RenderClear(ren);
        renderBG(ren);
        renderCure1(ren,x,y);
        SDL_RenderPresent(ren);
    }
    cleanUp(win,ren,nullptr);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
