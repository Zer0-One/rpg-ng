// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef RPGNG_SPRITE
#define RPGNG_SPRITE

#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#include "component.h"

typedef struct Sprite Sprite;

typedef enum SpriteSignalType {
    FLIP_H = 1,
    FLIP_V,
    OPACITY,
    Z_ORDER,
} SpriteSignalType;

typedef union SpriteSignalArgs {
    // Flip H
    struct {
        bool flip_h_old;
        bool flip_h;
    };
    // Flip V
    struct {
        bool flip_v_old;
        bool flip_v;
    };
    // Change Opacity
    struct {
        double opacity_old;
        double opacity;
    };
    // Update Z-Order
    struct {
        uint8_t z_old;
        uint8_t z;
    };
} SpriteSignalArgs;

extern const char* sprite_signal_type_str[];

typedef void (*sprite_cb_t)(SpriteSignalArgs);

typedef struct SpriteCallback {
    SpriteSignalType type;

    sprite_cb_t cb;
} SpriteCallback;

typedef struct SpriteCallbackList {
    size_t size;
    size_t count;

    SpriteCallback* cb;
} SpriteCallbackList;

/**
 * Registers a callback of the given type.
 *
 * @param type The signal type. Only signals of this type will trigger a
 * callback.
 */
bool sprite_regcb(Sprite* s, SpriteSignalType type, sprite_cb_t cb);

/**
 * Removes the given callback. Signals will no longer trigger a callback to the
 * given function.
 *
 * @return Returns true on success. Returns false if the given callback was not
 * found.
 */
bool sprite_unregcb(Sprite* s, sprite_cb_t cb);

/**
 * Frees all resources associated with the Sprite component system.
 */
void sprite_cleanup();

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
void sprite_opacity_set(Sprite* s, double opacity);

/**
 * Sets the sprite z-order. The lower the order, the further to the front the
 * sprite is composited.
 *
 * @param z A z-order between 0 and 255.
 */
void sprite_z_set(Sprite* s, uint8_t z);

#endif
