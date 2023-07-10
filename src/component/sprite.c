#include <stdbool.h>

#include <SDL2/SDL_image.h>

#include "../entity.h"
#include "../log.h"

#include "sprite.h"

const ComponentType sprite_component_type = SPRITE;

bool sprite_create(uint16_t entity_id, char* path) {
    logmsg(LOG_DEBUG, "component(sprite): Attempting to create new sprite for entity[%" PRIu16 "]", entity_id);

    Entity* e = entity_get(entity_id);

    if(!e) {
        logmsg(LOG_WARN, "component(sprite): Unable to create sprite, failed to get entity[%" PRIu16 "]", entity_id);

        return false;
    }

    if(entity_has_component(e->id, sprite_component_type)) {
        logmsg(LOG_WARN, "component(sprite): Unable to create sprite, entity[%" PRIu16 "]('%s') already has sprite", e->id, e->name);

        return false;
    }

    SDL_Surface* surface = IMG_Load(path);

    if(!surface) {
        logmsg(LOG_WARN, "component(sprite): Unable to create sprite for entity[%" PRIu16 "]('%s'), failed to load image at path '%s'", e->id, e->name, path);

        return false;
    }

    Sprite* s = calloc(1, sizeof(Sprite));

    if(!s) {
        logmsg(LOG_WARN, "component(sprite): Failed to create sprite for entity[%" PRIu16 "]('%s'), the system is out of memory", e->id, e->name);

        return false;
    }

    // TODO(zero-one): Do we need to do this, or is 0,0 the same thing?
    // surface->clip_rect.w = surface->w;
    // surface->clip_rect.h = surface->h;

    s->surface = surface;

    if(htable_add(e->components, (uint8_t*)&sprite_component_type, sizeof(sprite_component_type), s) != 0) {
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

    if(!e) {
        logmsg(LOG_WARN, "component(sprite): Unable to destroy sprite, failed to get entity[%" PRIu16 "]", entity_id);

        return false;
    }

    Sprite* s = htable_lookup(e->components, (uint8_t*)&sprite_component_type, sizeof(sprite_component_type));

    if(!s) {
        logmsg(LOG_WARN, "component(sprite): Unable to destroy sprite, failed to get sprite associated with entity[%" PRIu16 "]('%s')", e->id, e->name);

        return false;
    }

    SDL_FreeSurface(s->surface);

    free(s);

    return true;
}
