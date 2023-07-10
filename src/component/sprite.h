// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef RPGNG_SPRITE
#define RPGNG_SPRITE

#include <stdbool.h>

#include <SDL2/SDL.h>

typedef struct Sprite {
    bool flip_h;
    bool flip_v;

    double opacity;

    SDL_Rect clip;
    SDL_Surface* surface;
} Sprite;

/**
 * Associates a Sprite component with the given entity.
 *
 * @param path An image file from which the sprite will be created.
 *
 * @return On success, returns true. On failure, returns false.
 */
bool sprite_create(uint16_t entity_id, char* path);

/**
 * Destroys the Sprite associated with the given entity.
 *
 * @return On success, returns true. If the given entity does not have a sprite
 * component, this function returns false.
 */
bool sprite_destroy(uint16_t entity_id);

/**
 * Mirrors the sprite horizontally.
 */
void sprite_flip_h(Sprite* s);

/**
 * Mirrors the sprite vertically.
 */
void sprite_flip_v(Sprite* s);

/**
 * Sets sprite opacity.
 *
 * @param opacity An opacity value between 0 and 1. 1 is opaque, and 0 is
 * transparent.
 */
void sprite_set_opacity(Sprite* s, double opacity);

#endif
