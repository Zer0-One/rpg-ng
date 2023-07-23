// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef RPGNG_COMPONENT
#define RPGNG_COMPONENT

#include <stdbool.h>

#define SLOT_DEFAULT_SIZE 8

typedef enum ComponentType {
    DIALOGUE,
    DIALOGUEWIDGET,
    INVENTORY,
    SPRITE,
    TRANSFORM
} ComponentType;

bool component_init();

/**
 * Removes all components associated with the given entity by calling their
 * respective _destroy() functions.
 *
 * @return On success, returns true. On failure, returns false.
 */
bool component_cleanup(uint16_t entity_id);

#endif
