#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "entity.h"
#include "htable.h"
#include "log.h"

HashTable* entities = NULL;

uint32_t entity_next_id = 1;

int entity_init(){
    if(entities != NULL){
        logmsg(LOG_WARN, "entity: Attempted to initialize already-initialized entity subsystem");

        return -1;
    }

    entities = htable_create(16);

    if(entities == NULL){
        logmsg(LOG_WARN, "entity: Unable to create entity table, the system is out of memory");

        return -1;
    }

    return 0;
}

#define ENTITY_NAME_LEN_MAX 128

uint32_t entity_add(EntityType type, char* name){
    logmsg(LOG_DEBUG, "entity: Attempting to add new entity of type %d, name '%s'", type, name);

    if(entities == NULL){
        logmsg(LOG_WARN, "entity: Cannot add entity before initializing entity subsystem");

        return -1;
    }

    Entity* e = malloc(sizeof(Entity));

    if(e == NULL){
        logmsg(LOG_WARN, "entity: Cannot allocate new entity, the system is out of memory");

        return -1;
    }

    e->name = strndup(name, ENTITY_NAME_LEN_MAX);

    if(e->name == NULL){
        logmsg(LOG_WARN, "entity: Cannot copy entity name, the system is out of memory");

        return -1;
    }

    e->type = type;
    e->id = entity_next_id;

    if(htable_add(entities, (uint8_t*)&entity_next_id, sizeof(entity_next_id), e) != 0){
        logmsg(LOG_WARN, "entity: Unable to add new entity to entity table");

        free(e->name);
        free(e);

        return -1;
    }

    entity_next_id++;

    return e->id;
}

int entity_remove(uint32_t id){
    if(htable_remove(entities, (uint8_t*)&id, sizeof(id)) != 0){
        logmsg(LOG_WARN, "entity: Failed to remove entity:%" PRIu32 ", not found in entity table", id);

        return -1;
    }
}

Entity* entity_get(uint32_t id){
    Entity* e = htable_lookup(entities, (uint8_t*)&id, sizeof(id));

    if(e == NULL){
        logmsg(LOG_WARN, "entity: Failed to get entity:%" PRIu32 ", not found in entity table", id);

        return NULL;
    }
}

//Entity* entity_get_all(){
//    
//}
