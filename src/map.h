#ifndef GAM_MAP
#define GAM_MAP

#include <stdint.h>

struct map{
    int height;
    int width;
    int tile_height;
    int tile_width;
    double version;
    size_t layer_count;
    struct layer* layers;
    size_t tileset_count;
    struct tileset** tilesets;
};

enum layer_type{
    TILE = 0,
    OBJECT = 1
};

struct layer{
    enum layer_type type;
    char* name;
    int height;
    int width;
    double opacity;
    double offset_x;
    double offset_y;
    int* data;
};


enum tileset_type{
    INDIVIDUAL = 0,
    SHEET = 1
};

// For sheets, tiles are numbered left-to-right, top-to-bottom
struct tileset{
    enum tileset_type type;
    double version;
    union{
        struct tile* tiles;
        struct sheet* sheet;
    };
};

struct tile{
    int id;
    char* path;
    int height_px;
    int width_px;
    uint8_t* data;
};

struct sheet{
    char* path;
    int height_px;
    int width_px;
    int margin;
    int spacing;
    uint8_t* data;
};

/**
 * Loads the Tiled map at the given path.
 *
 * \return On success, a pointer to a map. The map is dynamically-allocated,
 * and must be freed by the caller.
 */
struct map* map_load(const char* path);

/**
 * Loads the Tiled tileset at the given path.
 *
 * \return On success, a pointer to a tileset. The tileset is
 * dynamically-allocated, and must be freed by the caller.
 */
struct tileset* tileset_load(const char* path);

/**
 * Frees the memory associated with the given map. This function will not free
 * any associated tilesets, those must be freed separately.
 */
void map_destroy(struct map* map);

/**
 * Frees the memory associated with the given tileset.
 */
void tileset_destroy(struct tileset* tileset);

#endif
