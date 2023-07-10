#ifndef RPGNG_WINDOW
#define RPGNG_WINDOW

#include <stdbool.h>

#include <SDL2/SDL.h>

extern SDL_Surface* main_window;

bool window_init(const char* title, int x, int y, int w, int h, uint32_t flags);

#endif
