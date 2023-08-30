// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include <SDL2/SDL_image.h>

#include "../entity.h"
#include "../log.h"

#include "sprite.h"

const ComponentType sprite_component_type = SPRITE;

struct Sprite {
    bool flip_h;
    bool flip_v;

    uint8_t z;

    double opacity;

    SDL_Rect clip;
    SDL_Surface* surface;

    SpriteCallbackList cb_list;
};

// Signals

char const* sprite_signal_type_str[] = {
    "flip_h",
    "flip_v",
    "z_order",
    "opacity",
};

bool sprite_regcb(Sprite* s, SpriteSignalType type, sprite_cb_t cb) {
    logmsg(LOG_DEBUG, "component(sprite): Attempting to register callback for signal['%s']", sprite_signal_type_str[type]);

    if (!cb) {
        logmsg(LOG_WARN, "component(sprite): Unable to register NULL callback");

        return false;
    }

    if (!s) {
        logmsg(LOG_WARN, "component(sprite): Unable to register callback for NULL component");

        return false;
    }

    // If the array isn't yet initialized, do that now
    if (!s->cb_list.cb) {
        s->cb_list.cb = calloc(SLOT_DEFAULT_SIZE, sizeof(SpriteCallback));

        if (!s->cb_list.cb) {
            logmsg(LOG_WARN, "component(sprite): Failed to initialize callback array, the system is out of memory");

            return false;
        }
    }

    // If the array is full, double the size
    if (s->cb_list.size == s->cb_list.count) {
        SpriteCallback* tmp = realloc(s->cb_list.cb, s->cb_list.size * 2 * sizeof(SpriteCallback));

        if (!tmp) {
            logmsg(LOG_WARN, "component(sprite): Failed to resize callback array, the system is out of memory");

            return false;
        }

        // Initialize the new memory so that we can find empty slots correctly
        // TODO(zero-one): off-by-one?
        memset(&tmp[s->cb_list.size], 0, s->cb_list.size * sizeof(SpriteCallback));

        s->cb_list.size *= 2;
    }

    // Add the callback
    for (size_t i = 0; i < s->cb_list.size; i++) {
        if (s->cb_list.cb[i].cb == NULL) {
            s->cb_list.cb[i].cb = cb;
            s->cb_list.cb[i].type = type;

            s->cb_list.count++;

            return true;
        }
    }

    logmsg(LOG_ERR,
        "component(sprite): Failed to register callback for signal['%s'], callback array isn't full, but a free slot was never found",
        sprite_signal_type_str[type]);

    _exit(-1);
}

bool sprite_unregcb(Sprite* s, sprite_cb_t cb) {
    logmsg(LOG_DEBUG, "component(sprite): Attempting to unregister callback (%p)", cb);

    for (size_t i = 0; i < s->cb_list.size; i++) {
        if (s->cb_list.cb[i].cb == cb) {
            s->cb_list.cb[i].cb = NULL;
            s->cb_list.cb[i].type = 0;

            s->cb_list.count--;

            return true;
        }
    }

    return false;
}

void sprite_signal(Sprite* s, SpriteSignalType type, SpriteSignalArgs args) {
    for (size_t i = 0; i < s->cb_list.size; i++) {
        if (s->cb_list.cb[i].type == type) {
            s->cb_list.cb[i].cb(args);
        }
    }
}

bool sprite_create(uint16_t entity_id, char* path) {
    logmsg(LOG_DEBUG, "component(sprite): Attempting to create new sprite for entity[%" PRIu16 "]", entity_id);

    Entity* e = entity_get(entity_id);

    if (!e) {
        logmsg(LOG_WARN, "component(sprite): Unable to create sprite, failed to get entity[%" PRIu16 "]", entity_id);

        return false;
    }

    if (entity_has_component(e->id, sprite_component_type)) {
        logmsg(LOG_WARN, "component(sprite): Unable to create sprite, entity[%" PRIu16 "]('%s') already has sprite", e->id, e->name);

        return false;
    }

    SDL_Surface* surface = IMG_Load(path);

    if (!surface) {
        logmsg(
            LOG_WARN, "component(sprite): Unable to create sprite for entity[%" PRIu16 "]('%s'), failed to load image at path '%s'", e->id, e->name, path);

        return false;
    }

    Sprite* s = calloc(1, sizeof(Sprite));

    if (!s) {
        logmsg(LOG_WARN, "component(sprite): Failed to create sprite for entity[%" PRIu16 "]('%s'), the system is out of memory", e->id, e->name);

        return false;
    }

    // TODO(zero-one): Do we need to do this, or is 0,0 the same thing?
    // surface->clip_rect.w = surface->w;
    // surface->clip_rect.h = surface->h;

    s->surface = surface;

    if (htable_add(e->components, (uint8_t*)&sprite_component_type, sizeof(sprite_component_type), s) != 0) {
        logmsg(LOG_WARN, "component(sprite): Failed to map sprite in component table for entity[%" PRIu16 "]('%s')", e->id, e->name);

        SDL_FreeSurface(s->surface);

        free(s);

        return false;
    }

    return true;
}

bool sprite_destroy(uint16_t entity_id) {
    logmsg(LOG_DEBUG, "component(sprite): Attempting to destroy sprite for entity[%" PRIu16 "]", entity_id);

    Entity* e = entity_get(entity_id);

    if (!e) {
        logmsg(LOG_WARN, "component(sprite): Unable to destroy sprite, failed to get entity[%" PRIu16 "]", entity_id);

        return false;
    }

    Sprite* s = htable_lookup(e->components, (uint8_t*)&sprite_component_type, sizeof(sprite_component_type));

    if (!s) {
        logmsg(LOG_WARN, "component(sprite): Unable to destroy sprite, failed to get sprite associated with entity[%" PRIu16 "]('%s')", e->id, e->name);

        return false;
    }

    SDL_FreeSurface(s->surface);

    free(s);

    return true;
}

void sprite_flip_h(Sprite* s) {
    bool flip_h_old = s->flip_h;

    s->flip_h = !s->flip_h;

    SpriteSignalArgs args = {flip_h_old, s->flip_h};

    sprite_signal(s, FLIP_H, args);
}

void sprite_flip_v(Sprite* s) {
    bool flip_v_old = s->flip_v;

    s->flip_v = !s->flip_v;

    SpriteSignalArgs args = {flip_v_old, s->flip_v};

    sprite_signal(s, FLIP_V, args);
}

void sprite_opacity_set(Sprite* s, double opacity) {
    double opacity_old = s->opacity;

    s->opacity = opacity;

    SpriteSignalArgs args = {opacity_old, s->opacity};

    sprite_signal(s, OPACITY, args);
}

void sprite_z_set(Sprite* s, uint8_t z) {
    uint8_t z_old = s->z;

    s->z = z;

    SpriteSignalArgs args = {z_old, s->z};

    sprite_signal(s, Z_ORDER, args);
}
