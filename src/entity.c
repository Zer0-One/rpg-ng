#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "entity.h"
#include "htable.h"
#include "log.h"

#include "component/inventory.h"

HashTable* entities = NULL;
HashTable* entities_str = NULL;

uint16_t entity_next_id = 1;

bool entity_init(){
    logmsg(LOG_DEBUG, "entity: Attempting to initialize entity");

    if(entities != NULL || entities_str != NULL){
        logmsg(LOG_WARN, "entity: Init failed, this system was already initialized");

        return false;
    }

    entities = htable_create(16);

    if(entities == NULL){
        logmsg(LOG_WARN, "entity: Unable to create entity table, the system is out of memory");

        return false;
    }

    entities_str = htable_create(16);

    if(entities_str == NULL){
        logmsg(LOG_WARN, "entity: Unable to create entity_str table, the system is out of memory");

        return false;
    }

    return true;
}

uint16_t entity_create(const char* name){
    logmsg(LOG_DEBUG, "entity: Attempting to create new entity id:%" PRIu16 ", name:'%s'", entity_next_id, name);

    if(entities == NULL){
        logmsg(LOG_WARN, "entity: Cannot add entity before initializing entity subsystem");

        return -1;
    }

    if(name == NULL){
        logmsg(LOG_WARN, "entity: Cannot create entity:%" PRIu16 " with NULL name", entity_next_id);

        return -1;
    }

    Entity* e = calloc(1, sizeof(Entity));

    if(e == NULL){
        logmsg(LOG_WARN, "entity: Cannot allocate new entity, the system is out of memory");

        return -1;
    }

    strncpy(e->name, name, ENTITY_NAME_LEN_MAX);

    e->id = entity_next_id;

    if(htable_add(entities, (uint8_t*)&entity_next_id, sizeof(entity_next_id), e) != 0){
        logmsg(LOG_WARN, "entity: Unable to add new entity to entity table");

        free(e);

        return -1;
    }

    if(htable_add(entities_str, name, strlen(name), e) != 0){
        logmsg(LOG_WARN, "entity: Unable to add new entity to entity_str table");

        if(htable_remove(entities, (uint8_t*)&entity_next_id, sizeof(entity_next_id)) == -2){
            // We just added that mapping. If we can't remove it, something's really fucked.
            logmsg(LOG_ERR, "entity: Failed to remove mapping from entity table");
            logmsg(LOG_ERR, "entity: Something's fucked");

            _exit(-1);
        }

        free(e);

        return -1;
    }

    entity_next_id++;

    return e->id;
}

bool entity_destroy(uint16_t id){
    if(htable_remove(entities, (uint8_t*)&id, sizeof(id)) != 0){
        logmsg(LOG_WARN, "entity: Failed to remove entity:%" PRIu16 ", not found in entity table", id);

        return -1;
    }

    // Clean up any associated components
    inventory_destroy(id);
}

Entity* entity_get(uint16_t id){
    Entity* e = htable_lookup(entities, (uint8_t*)&id, sizeof(id));

    if(e == NULL){
        logmsg(LOG_WARN, "entity: Failed to get entity:%" PRIu16 ", not found in entity table", id);

        return NULL;
    }
}

//Entity* entity_get_all(){
//    
//}
