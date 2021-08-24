#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "config.h"
#include "log.h"

SDL_Window* main_window = NULL;
SDL_Surface* main_surface = NULL;

SDL_Window* init_sdl(const char* window_title, int window_pos_x, int window_pos_y, int window_width, int window_height, Uint32 flags){
    logmsg(LOG_DEBUG, "Initializing SDL");

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        logmsg(LOG_ERR, "Unable to initialize SDL, %s", SDL_GetError());
        return NULL;
    }

    logmsg(LOG_DEBUG, "Initializing SDL_image");

    int img_flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
    if((IMG_Init(img_flags) & img_flags) != img_flags){
        logmsg(LOG_ERR, "Unable to initialize SDL_image, %s", IMG_GetError());

        return NULL;
    }

    SDL_Window* window = NULL;

    logmsg(LOG_DEBUG, "Creating window at position: x=%d, y=%d; size: %dx%d", window_pos_x, window_pos_y, window_width, window_height);

    window = SDL_CreateWindow(window_title, window_pos_x, window_pos_y, window_width, window_height, flags);
    if(window == NULL){
        logmsg(LOG_ERR, "Unable to create window, %s", SDL_GetError());

        return NULL;
    }

    main_window = window;
    main_surface = SDL_GetWindowSurface(window);

    if(main_surface == NULL){
        logmsg(LOG_ERR, "Unable to get main window surface, %s", SDL_GetError());

        return NULL;
    }

    return window;
}
