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
    uint8_t id;
    EntityType type;
} Entity;

uint8_t entity_add(EntityType type, char* name);
Entity* entity_get(uint8_t id);
Entity* entity_get_by_name(char* name);
int entity_remove(uint8_t id);

#endif
