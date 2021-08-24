#include <stdio.h>
#include <unistd.h>

#include <jansson.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "config.h"
#include "init.h"
#include "log.h"
#include "map.h"
#include "run.h"

void print_usage(){
    printf("Usage: rpg-ng [-d]\n");
    printf("Command-line options:\n");
    printf("\n\t-d,\tEnables debug mode, increasing logging verbosity");
    printf("\n\t-h,\tPrints this help information");
    printf("\n\t-v,\tPrints version information");
    printf("\n");
}

void print_versions(){
    SDL_version sdl_ver;
    SDL_VERSION(&sdl_ver);

    printf("This build has been compiled with support for:\n");
    printf("SDL: %d.%d.%d\n", sdl_ver.major, sdl_ver.minor, sdl_ver.patch);
    printf("SDL_image: %d.%d.%d\n", SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_PATCHLEVEL);
    printf("Jansson: %s\n", JANSSON_VERSION);
}

int main(int argc, char* argv[]){
    int opt;
    while((opt = getopt(argc, argv, "dhv")) != -1){
        switch(opt){
            case 'd':
                config_log_debug = true;
                break;
            case 'h':
                print_usage();
                _exit(0);
            case 'v':
                print_versions();
                _exit(0);
            case '?':
                print_usage();
                _exit(-1);
        }
    }

    if(config_fullscreen){
        init_sdl(config_window_title, config_window_pos_x, config_window_pos_y, config_window_width, config_window_height, SDL_WINDOW_FULLSCREEN);
    }
    else if(config_borderless){
        init_sdl(config_window_title, config_window_pos_x, config_window_pos_y, config_window_width, config_window_height, SDL_WINDOW_BORDERLESS);
    }
    else{
        init_sdl(config_window_title, config_window_pos_x, config_window_pos_y, config_window_width, config_window_height, SDL_WINDOW_SHOWN);
    }

    if(main_window == NULL){
        _exit(-1);
    }


    SDL_FillRect(main_surface, NULL, SDL_MapRGB(main_surface->format, 0xFF, 0x45, 0xFF));

    for(size_t i = 0; i < 5; i++){
        SDL_UpdateWindowSurface(main_window);
    }

    SDL_Surface* bg = SDL_LoadBMP("resources/bg/splash.bmp");
    if(bg == NULL){
        logmsg(LOG_ERR, "Unable to load splash, %s", SDL_GetError());
        _exit(-1);
    }

    SDL_BlitSurface(bg, NULL, main_surface, NULL);

    SDL_UpdateWindowSurface(main_window);

    //struct map* testmap = map_load("resources/map/vasu_ring_shop/vasu_ring_shop.json");
    struct map* testmap = map_load("/home/zero-one/bullshit/muh_map.json");

    if(testmap == NULL){
        goto quit;
    }

    run();

quit:
    SDL_FreeSurface(bg);
    SDL_DestroyWindow(main_window);
    SDL_Quit();

    _exit(0);
}
