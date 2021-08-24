#include <unistd.h>

#include <SDL2/SDL.h>

#include "init.h"
#include "log.h"

SDL_Event e;

void run(){
    while(true){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                return;
            }
        }

        sleep(1);
    }
}
