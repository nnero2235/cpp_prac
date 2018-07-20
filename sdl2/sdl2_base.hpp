#ifndef SDL2_BASE_HPP
#define SDL2_BASE_HPP

#include<string>
#include<SDL2/SDL.h>
#include"../nnero/logging.hpp"

namespace sdl2{
    namespace base{
        using namespace nnero::logging;
        
        std::string getResoucePath(const std::string& sub_dir=""){
            #ifdef _WIN32
            const char PATH_BASE = '\\';
            #else
            const char PATH_BASE = '/';
            #endif

            static std::string base_path;
            if(base_path.empty()){
                char* base_dir = SDL_GetBasePath();
                if(base_dir){
                    base_path = base_dir;
                    SDL_free(base_dir);
                } else {
                    return "";
                }
                base_path = base_path + "res" + PATH_BASE;
            }

            return sub_dir.empty() ? base_path : base_path + sub_dir + PATH_BASE;
        }

        void logSDLError(const std::string& msg){
            LOG(ERROR)<<msg<<" Error: "<<SDL_GetError();
        }
    }
}

#endif
