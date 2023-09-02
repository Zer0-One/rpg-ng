// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef RPGNG_ENTITY
#define RPGNG_ENTITY

#include <stdbool.h>
#include <stdint.h>

#include "component/component.h"
#include "htable.h"

#define ENTITY_NAME_LEN_MAX 256

typedef struct Entity {
    char name[ENTITY_NAME_LEN_MAX];
    uint16_t id;

    HashTable* components;
} Entity;

/**
 * Initializes global state for the entity subsystem.
 */
bool entity_init();

/**
 * Creates a new entity and assigns it an ID.
 *
 * @param name A locally-unique human-readable string by which this entity can
 * be referenced.
 *
 * @return On success, returns an entity ID, which can be used as a handle to
 * reference this entity.
 * @return On failure, returns 0.
 */
uint16_t entity_create(const char* name);

/**
 * Destroys the entity represented by the given ID, along with all associated
 * components.
 */
bool entity_destroy(uint16_t entity_id);

/**
 * Performs a lookup for the given entity, and returns a pointer to it, if found.
 *
 * @param entity_id An entity ID.
 */
Entity* entity_get(uint16_t entity_id);

/**
 * Performs a lookup for the given entity, and returns a pointer to it, if found.
 *
 * @param name An entity name.
 *
 * @return On success, returns a dynamically-allocated copy of an Entity with
 * the given ID. The user must free the returned Entity.
 */
Entity* entity_get_by_name(const char* name);

/**
 * Determines if the given entity has the given component.
 */
bool entity_has_component(uint16_t entity_id, ComponentType type);

/**
 * Gets the requested component of the given entity.
 *
 * @param entity_id An entity ID.
 * @param type A component type to retrieve.
 *
 * @return On success, returns a pointer to a component object corresponding to
 * the type requested.
 * @return On failure, returns NULL.
 */
void* entity_get_component(uint16_t entity_id, ComponentType type);

#endif
