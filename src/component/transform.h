// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef RPGNG_TRANSFORM
#define RPGNG_TRANSFORM

#include <stdbool.h>

typedef struct Transform Transform;

typedef enum TransformSignalType {
    TRANSLATE,
    ROTATE,
    SCALE
} TransformSignalType;

typedef union TransformSignalArgs {
    // Translate
    struct {
        int x_old;
        int y_old;
        int x;
        int y;
    };
    // Rotate
    struct {
        double rotation_old;
        double rotation;
    };
    // Scale
    struct {
        double scale_old;
        double scale;
    };
} TransformSignalArgs;

// Strings representing the above signal types. Use the above enum values as indexes.
extern const char* transform_signal_type_str[];

// Transform cock and ball torture
typedef void (*transform_cb_t)(TransformSignalArgs);

typedef struct TransformCallback {
    TransformSignalType type;

    transform_cb_t cb;
} TransformCallback;

typedef struct TransformCallbackList {
    size_t size;
    size_t count;

    TransformCallback* cb;
} TransformCallbackList;

/**
 * Registers a callback of the given type.
 *
 * @param type The signal type. Only signals of this type will trigger a
 * callback.
 */
bool transform_regcb(Transform* t, TransformSignalType type, transform_cb_t cb);

/**
 * Removes the given callback. Signals will no longer trigger a callback to the
 * given function.
 *
 * @return Returns true on success. Returns false if the given callback was not
 * found.
 */
bool transform_unregcb(Transform* t, transform_cb_t cb);

/**
 * Frees all resources associated with the Transform component system.
 */
void transform_cleanup();

/**
 * Associates a Transform component with the given entity.
 *
 * All transform fields are initialized to 0.
 *
 * @return On success, returns true. On failure, returns false.
 */
bool transform_create(uint16_t entity_id);

/**
 * Destroys the transform associated with the given entity.
 */
bool transform_destroy(uint16_t entity_id);

/**
 * Translates an entity by the given amounts.
 */
void transform_translate(Transform* t, int x, int y);

/**
 * Sets the transform position to the given values.
 */
void transform_translate_set(Transform* t, int x, int y);

/**
 * Resets the position of an entity to 0,0.
 */
void transform_translate_reset(Transform* t);

/**
 * Rotates an entity by the given amount in degrees.
 */
void transform_rotate(Transform* t, double r);

/**
 * Sets the rotation of an entity to the given value (in degrees).
 */
void transform_rotate_set(Transform* t, double r);

/**
 * Resets the rotation of an entity to 0 degrees.
 */
void transform_rotate_reset(Transform* t);

/**
 * Scales an entity by the given amount.
 */
void transform_scale(Transform* t, double scale);

/**
 * Sets the scale of an entity to the given value.
 */
void transform_scale_set(Transform* t, double scale);

/**
 * Resets the scale of an entity.
 */
void transform_scale_reset(Transform* t);

/**
 * Gets the x position of an entity.
 */
int transform_get_pos_x(Transform* t);

/**
 * Gets the y position of an entity.
 */
int transform_get_pos_y(Transform* t);

/**
 * Gets the rotation of an entity.
 */
double transform_get_rotation(Transform* t);

/**
 * Gets the scale of an entity.
 */
double transform_get_scale(Transform* t);

#endif
