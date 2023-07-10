// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef RPGNG_TRANSFORM
#define RPGNG_TRANSFORM

#include <stdbool.h>

typedef struct Transform {
    int pos_x;
    int pos_y;

    double rotation;

    double scale;
} Transform;

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
 * Sets the transform position to the given values.
 */
void transform_set_pos(Transform* t, int x, int y);

/**
 * Translates an entity by the given amounts.
 */
void transform_translate(Transform* t, int x, int y);

/**
 * Resets the position of an entity to 0,0.
 */
void transform_translate_reset(Transform* t);

/**
 * Rotates an entity by the given amount in degrees.
 */
void transform_rotate(Transform* t, double r);

/**
 * Resets the rotation of an entity to 0 degrees.
 */
void transform_rotate_reset(Transform* t);

/**
 * Scales an entity by the given amount.
 */
void transform_scale(Transform* t, double scale);

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
