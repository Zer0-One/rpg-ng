#ifndef RPGNG_ENTITY
#define RPGNG_ENTITY

#include <stdbool.h>
#include <stdint.h>

#define ENTITY_NAME_LEN_MAX 256

typedef struct Entity {
    char name[ENTITY_NAME_LEN_MAX];
    uint16_t id;
} Entity;

/**
 * Initializes global state for the entity subsystem.
 */
bool entity_init();

/**
 * Creates a new entity and assigns it an ID.
 *
 * @param name A "globally" unique human-readable string by which this entity
 * can be referenced.
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
 * Performs a lookup for the given entity, and returns a copy if found.
 *
 * @param entity_id An entity ID.
 *
 * @return On success, returns a dynamically-allocated copy of an Entity with
 * the given ID. The user must free the returned Entity.
 */
Entity* entity_get(uint16_t entity_id);

/**
 * Performs a lookup for the given entity, and returns a copy if found.
 *
 * @param name An entity name.
 *
 * @return On success, returns a dynamically-allocated copy of an Entity with
 * the given ID. The user must free the returned Entity.
 */
Entity* entity_get_by_name(const char* name);

#endif
