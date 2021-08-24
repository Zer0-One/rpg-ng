#ifndef GAM_INIT
#define GAM_INIT

#include <SDL2/SDL.h>

extern SDL_Window* main_window;
extern SDL_Surface* main_surface;

/**
 * Initializes SDL.
 *
 * \return A pointer to the main game window on success.
 * \return NULL on failure.
 */
SDL_Window* init_sdl(const char* window_title, int window_pos_x, int window_pos_y, int window_width, int window_height, Uint32 flags);

#endif
