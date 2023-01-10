#ifndef RPGNG_ENTITY
#define RPGNG_ENTITY

#include <stdint.h>

typedef enum ENTITY_TYPE {
    PC,
    NPC,
    OBJECT
} EntityType;

typedef struct Entity {
    char* name;
    uint32_t id;
    EntityType type;
} Entity;

/**
 * Initializes global state for the entity subsystem.
 */
int entity_init();

/**
 * Creates a new entity of the given type, and assigns it an ID.
 *
 * @return An entity ID, which can be used as a handle to reference this
 * entity.
 */
uint32_t entity_add(EntityType type, char* name);
Entity* entity_get(uint32_t id);
Entity* entity_get_by_name(char* name);
int entity_remove(uint32_t id);

#endif
