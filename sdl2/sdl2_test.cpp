#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include"../nnero/logging.hpp"
#include"sdl2_base.hpp"

using namespace nnero::logging;
using namespace sdl2::base;

void cleanUp(SDL_Window* win,SDL_Renderer* ren,SDL_Texture* tex,SDL_Texture* tex1){
    SDL_DestroyTexture(tex);
    SDL_DestroyTexture(tex1);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
}

SDL_Texture* loadTexture(const std::string& file,SDL_Renderer* ren){
    SDL_Texture *tex = IMG_LoadTexture(ren,file.c_str());
    if(tex == nullptr){
        logSDLError("createTexturefromsurface");
    }
    return tex;
}

void renderTexture(SDL_Texture* tex,SDL_Renderer* ren,int x,int y){
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = 80;
    dst.h = 80;
    SDL_RenderCopy(ren,tex,NULL,&dst);
}

int main(){
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
        cleanUp(win,nullptr,nullptr,nullptr);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    
    std::string image_path = getResoucePath()+"haha.jpg";
    std::string bg = getResoucePath()+"biaobai.png";
    SDL_Texture* image = loadTexture(image_path,ren);
    SDL_Texture* bg_image = loadTexture(bg,ren);
    
    if(image == nullptr){
        cleanUp(win,ren,nullptr,nullptr);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    if(bg_image == nullptr){
        cleanUp(win,ren,nullptr,nullptr);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    int w,h;
    SDL_QueryTexture(image,NULL,NULL,&w,&h);
    bool quit{false};
    SDL_Event e;
    while(!quit){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                quit = true;
            }
            if(e.type == SDL_KEYDOWN){
                quit = true;
            }
            if(e.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
            }
        }
        SDL_RenderClear(ren);
        for(int i=0;i<screen_width/80;++i){
            for(int j=0;j<screen_height/80;++j){
                renderTexture(bg_image,ren,i*80,j*80);
            }
        }
        renderTexture(image,ren,screen_width/2-w/2,screen_height/2-h/2);
        SDL_RenderPresent(ren);
    }
    cleanUp(win,ren,image,bg_image);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
