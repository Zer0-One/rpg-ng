// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include "../entity.h"
#include "../log.h"

#include "component.h"
#include "transform.h"

const ComponentType transform_component_type = TRANSFORM;

struct Transform {
    int pos_x;
    int pos_y;

    double rotation;

    double scale;

    TransformCallbackList cb_list;
};

// Signals

const char* transform_signal_type_str[] = {
    "translate",
    "rotate",
    "scale",
};

bool transform_regcb(Transform* t, TransformSignalType type, transform_cb_t cb) {
    logmsg(LOG_DEBUG, "component(transform): Attempting to register callback for signal['%s']", transform_signal_type_str[type]);

    if (!cb) {
        logmsg(LOG_WARN, "component(transform): Unable to register NULL callback");

        return false;
    }

    if (!t) {
        logmsg(LOG_WARN, "component(transform): Unable to register callback for NULL component");

        return false;
    }

    // If the array isn't yet initialized, do that now
    if (!t->cb_list.cb) {
        t->cb_list.cb = calloc(SLOT_DEFAULT_SIZE, sizeof(TransformCallback));

        if (!t->cb_list.cb) {
            logmsg(LOG_WARN, "component(transform): Failed to initialize callback array, the system is out of memory");

            return false;
        }
    }

    // If the array is full, double the size
    if (t->cb_list.size == t->cb_list.count) {
        TransformCallback* tmp = realloc(t->cb_list.cb, t->cb_list.size * 2 * sizeof(TransformCallback));

        if (!tmp) {
            logmsg(LOG_WARN, "component(transform): Failed to resize callback array, the system is out of memory");

            return false;
        }

        // Initialize the new memory so that we can find empty slots correctly
        memset(&tmp[t->cb_list.size], 0, t->cb_list.size * sizeof(TransformCallback));

        t->cb_list.size *= 2;
    }

    // Add the callback
    for (size_t i = 0; i < t->cb_list.size; i++) {
        if (t->cb_list.cb[i].cb == NULL) {
            t->cb_list.cb[i].cb = cb;
            t->cb_list.cb[i].type = type;

            t->cb_list.count++;

            return true;
        }
    }

    logmsg(LOG_ERR,
        "component(transform): Failed to register callback for signal['%s'], callback array isn't full, but a free slot was never found",
        transform_signal_type_str[type]);

    _exit(-1);
}

bool transform_unregcb_translate(Transform* t, transform_cb_t cb) {
    logmsg(LOG_DEBUG, "component(transform): Attempting to unregister callback (%p)", cb);

    for (size_t i = 0; i < t->cb_list.size; i++) {
        if (t->cb_list.cb[i].cb == cb) {
            t->cb_list.cb[i].cb = NULL;
            t->cb_list.cb[i].type = 0;

            t->cb_list.count--;

            return true;
        }
    }

    return false;
}

void transform_signal(Transform* t, TransformSignalType type, TransformSignalArgs args) {
    for (size_t i = 0; i < t->cb_list.count; i++) {
        if (t->cb_list.cb[i].type == type) {
            t->cb_list.cb[i].cb(args);
        }
    }
}

// Component operations

bool transform_create(uint16_t entity_id) {
    logmsg(LOG_DEBUG, "component(transform): Attempting to create new transform for entity[%" PRIu16 "]", entity_id);

    Entity* e = entity_get(entity_id);

    if (!e) {
        logmsg(LOG_WARN, "component(transform): Unable to create transform, failed to get entity[%" PRIu16 "]", entity_id);

        return false;
    }

    if (entity_has_component(e->id, transform_component_type)) {
        logmsg(LOG_WARN, "component(transform): Unable to create transform, entity[%" PRIu16 "]('%s') already has transform", e->id, e->name);

        return false;
    }

    Transform* t = calloc(1, sizeof(Transform));

    if (!t) {
        logmsg(LOG_WARN, "component(transform): Failed to create transform, the system is out of memory");

        return false;
    }

    if (htable_add(e->components, (uint8_t*)&transform_component_type, sizeof(transform_component_type), KV_VOIDPTR, t) != 0) {
        logmsg(LOG_WARN, "component(transform): Failed to map transform in component table for entity[%" PRIu16 "]('%s')", e->id, e->name);

        free(t);

        return false;
    }

    return true;
}

bool transform_destroy(uint16_t entity_id) {
    logmsg(LOG_DEBUG, "component(transform): Attempting to destroy transform for entity[%" PRIu16 "]", entity_id);

    Entity* e = entity_get(entity_id);

    if (!e) {
        logmsg(LOG_WARN, "component(transform): Unable to destroy transform, failed to get entity[%" PRIu16 "]", entity_id);

        return false;
    }

    Transform* t = htable_lookup(e->components, (uint8_t*)&transform_component_type, sizeof(transform_component_type), NULL);

    if (!t) {
        logmsg(LOG_WARN, "component(transform): Failed to get transform associated with entity[%" PRIu16 "]('%s')", e->id, e->name);

        return false;
    }

    free(t);

    if (htable_remove(e->components, (uint8_t*)&transform_component_type, sizeof(transform_component_type)) < 0) {
        logmsg(LOG_ERR,
            "component(transform): Failed to remove transform associated with entity[%" PRIu16 "]('%s'), but it was present in the component table",
            e->id,
            e->name);

        _exit(-1);
    }

    return true;
}

void transform_translate(Transform* t, int x, int y) {
    int x_old = t->pos_x;
    int y_old = t->pos_y;

    t->pos_x += x;
    t->pos_y += y;

    TransformSignalArgs args = {x_old, y_old, t->pos_x, t->pos_y};

    transform_signal(t, TRANSLATE, args);
}

void transform_translate_set(Transform* t, int x, int y) {
    int x_old = t->pos_x;
    int y_old = t->pos_y;

    t->pos_x = x;
    t->pos_y = y;

    TransformSignalArgs args = {x_old, y_old, t->pos_x, t->pos_y};

    transform_signal(t, TRANSLATE, args);
}

void transform_translate_reset(Transform* t) {
    int x_old = t->pos_x;
    int y_old = t->pos_y;

    t->pos_x = 0;
    t->pos_y = 0;

    TransformSignalArgs args = {x_old, y_old, t->pos_x, t->pos_y};

    transform_signal(t, TRANSLATE, args);
}

void transform_rotate(Transform* t, double rotation) {
    double rotation_old = t->rotation;

    t->rotation += rotation;

    TransformSignalArgs args = {rotation_old, t->rotation};

    transform_signal(t, ROTATE, args);
}

void transform_rotate_set(Transform* t, double rotation) {
    double rotation_old = t->rotation;

    t->rotation = rotation;

    TransformSignalArgs args = {rotation_old, t->rotation};

    transform_signal(t, ROTATE, args);
}

void transform_rotate_reset(Transform* t) {
    double rotation_old = t->rotation;

    t->rotation = 0;

    TransformSignalArgs args = {rotation_old, t->rotation};

    transform_signal(t, ROTATE, args);
}

void transform_scale(Transform* t, double scale) {
    double scale_old = t->scale;

    t->scale += scale;

    TransformSignalArgs args = {scale_old, t->scale};

    transform_signal(t, SCALE, args);
}

void transform_scale_set(Transform* t, double scale) {
    double scale_old = t->scale;

    t->scale = scale;

    TransformSignalArgs args = {scale_old, t->scale};

    transform_signal(t, SCALE, args);
}

void transform_scale_reset(Transform* t) {
    double scale_old = t->scale;

    t->scale = 0;

    TransformSignalArgs args = {scale_old, t->scale};

    transform_signal(t, SCALE, args);
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
