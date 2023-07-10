#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>

#include "../entity.h"
#include "../log.h"

#include "component.h"
#include "transform.h"

const ComponentType transform_component_type = TRANSFORM;

bool transform_create(uint16_t entity_id) {
    logmsg(LOG_DEBUG, "component(transform): Attempting to create new transform for entity[%" PRIu16 "]", entity_id);

    Entity* e = entity_get(entity_id);

    if(!e) {
        logmsg(LOG_WARN, "component(transform): Unable to create transform, failed to get entity[%" PRIu16 "]", entity_id);

        return false;
    }

    if(entity_has_component(e->id, transform_component_type)) {
        logmsg(LOG_WARN, "component(transform): Unable to create transform, entity[%" PRIu16 "]('%s') already has transform", e->id, e->name);

        return false;
    }

    Transform* t = calloc(1, sizeof(Transform));

    if(!t) {
        logmsg(LOG_WARN, "component(transform): Failed to create transform, the system is out of memory");

        return false;
    }

    if(htable_add(e->components, (uint8_t*)&transform_component_type, sizeof(transform_component_type), t) != 0) {
        logmsg(LOG_WARN, "component(transform): Failed to map transform in component table for entity[%" PRIu16 "]('%s')", e->id, e->name);

        free(t);

        return false;
    }

    return true;
}

bool transform_destroy(uint16_t entity_id) {
    logmsg(LOG_DEBUG, "component(transform): Attempting to destroy transform for entity[%" PRIu16 "]", entity_id);

    Entity* e = entity_get(entity_id);

    if(!e) {
        logmsg(LOG_WARN, "component(transform): Unable to destroy transform, failed to get entity[%" PRIu16 "]", entity_id);

        return false;
    }

    Transform* t = htable_lookup(e->components, (uint8_t*)&transform_component_type, sizeof(transform_component_type));

    if(!t) {
        logmsg(LOG_WARN, "component(transform): Failed to get transform associated with entity[%" PRIu16 "]('%s')", e->id, e->name);

        return false;
    }

    free(t);

    if(htable_remove(e->components, (uint8_t*)&transform_component_type, sizeof(transform_component_type)) < 0) {
        logmsg(LOG_ERR, "component(transform): Failed to remove transform associated with entity[%" PRIu16 "]('%s'), but it was present in the component table", e->id, e->name);

        _exit(-1);
    }

    return true;
}

void transform_set_pos(Transform* t, int x, int y) {
    t->pos_x = x;
    t->pos_y = y;
}

void transform_translate(Transform* t, int x, int y) {
    t->pos_x += x;
    t->pos_y += y;
}

void transform_translate_reset(Transform* t) {
    t->pos_x = 0;
    t->pos_y = 0;
}

void transform_rotate(Transform* t, double rotation) {
    t->rotation += rotation;
}

void transform_rotate_reset(Transform* t) {
    t->rotation = 0;
}

void transform_scale(Transform* t, double scale) {
    t->scale += scale;
}

void transform_scale_reset(Transform* t) {
    t->scale = 0;
}

int transform_get_pos_x(Transform* t) {
    return t->pos_x;
}

int transform_get_pos_y(Transform* t) {
    return t->pos_y;
}

double transform_get_rotation(Transform* t) {
    return t->rotation;
}

double transform_get_scale(Transform* t) {
    return t->scale;
}
