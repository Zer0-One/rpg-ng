#ifndef GAM_CONFIG
#define GAM_CONFIG

#include <stdbool.h>

// Logging options

/**
 * If set to true, logs will include debug messages.
 */
extern bool config_log_debug;


// Initialization and windowing options

extern bool config_fullscreen;
extern bool config_borderless;
extern bool config_resizeable;

extern const char* config_window_title;

extern int config_window_pos_x;
extern int config_window_pos_y;
extern int config_window_width;
extern int config_window_height;

// Tiled options

extern const double tiled_req_map_version;

#endif
