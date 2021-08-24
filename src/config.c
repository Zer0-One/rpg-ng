#include <stdbool.h>

#include <SDL2/SDL.h>

bool config_log_debug = true;

bool config_fullscreen = false;
bool config_borderless = false;

char* config_window_title = "Muh Gam";

int config_window_pos_x = SDL_WINDOWPOS_UNDEFINED;
int config_window_pos_y = SDL_WINDOWPOS_UNDEFINED;
int config_window_width = 640;
int config_window_height = 400;

const double tiled_req_map_version = 1.2;
