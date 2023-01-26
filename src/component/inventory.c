#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "../entity.h"
#include "../htable.h"
#include "../log.h"

#include "component.h"
#include "inventory.h"

ComponentType inventory_type = INVENTORY;

HashTable* items = NULL;

bool inventory_init(){
    logmsg(LOG_DEBUG, "inventory: Attempting to initialize inventory");

    if(items != NULL){
        logmsg(LOG_WARN, "inventory: Init failed, this component was already initialized");

        return false;
    }

    logmsg(LOG_DEBUG, "inventory: Attempting to create item table");

    items = htable_create(16);

    if(items == NULL){
        logmsg(LOG_WARN, "inventory: Could not allocate item table, the system is out of memory");

        return false;
    }

    return true;
}

bool inventory_create(uint16_t entity_id, uint16_t* item_ids, size_t ids_size){
    logmsg(LOG_DEBUG, "inventory: Creating new inventory for entity:%" PRIu16, entity_id);

    Entity* e = entity_get(entity_id);

    if(e == NULL){
        logmsg(LOG_WARN, "inventory: Unable to create inventory, failed to get entity:%" PRIu16, entity_id);

        return false;
    }

    if(entity_has_component(entity_id, inventory_type)){
        logmsg(LOG_WARN, "inventory: Cannot create inventory for entity:%" PRIu16 ", entity already has inventory", entity_id);

        return false;
    }

    Inventory* inv = calloc(1, sizeof(Inventory));

    if(inv == NULL){
        logmsg(LOG_WARN, "inventory: Failed to create inventory, the system is out of memory");

        return false;
    }

    if(item_ids != NULL){
        for(size_t i = 0; i < ids_size; i++){
            inv->items[i] = item_ids[i];
        }
    }

    if(htable_add(e->components, (uint8_t*)&inventory_type, sizeof(inventory_type), inv) != 0){
        logmsg(LOG_WARN, "inventory: Failed to map inventory in component table for entity:%" PRIu16, entity_id);

        free(inv);

        return false;
    }

    return true;
}

bool inventory_destroy(uint16_t entity_id){
    logmsg(LOG_DEBUG, "inventory: Destroying inventory for entity:%" PRIu16, entity_id);

    Entity* e = entity_get(entity_id);

    if(e == NULL){
        logmsg(LOG_WARN, "inventory: Unable to destroy inventory, failed to get entity:%" PRIu16, entity_id);

        return false;
    }

    if(!entity_has_component(entity_id, inventory_type)){
        logmsg(LOG_WARN, "inventory: Failed to destroy inventory, no inventory found for entity:%" PRIu16, entity_id);

        return false;
    }

    Inventory* inv = entity_get_component(entity_id, inventory_type);

    free(inv);

    if(htable_remove(e->components, (uint8_t*)&inventory_type, sizeof(inventory_type)) != 0){
        logmsg(LOG_WARN, "inventory: Failed to remove inventory mapping from entity component table");

        return false;
    }

    return true;
}
