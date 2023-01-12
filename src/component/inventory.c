#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "../htable.h"
#include "../log.h"

#include "inventory.h"

HashTable* items = NULL;
HashTable* inventories = NULL;

bool inventory_init(){
    logmsg(LOG_DEBUG, "inventory: Attempting to initialize inventory");

    if(items != NULL || inventories != NULL){
        logmsg(LOG_WARN, "inventory: Init failed, this component was already initialized");

        return false;
    }

    logmsg(LOG_DEBUG, "inventory: Attempting to create item table");

    items = htable_create(16);

    if(items == NULL){
        logmsg(LOG_WARN, "inventory: Could not allocate item table, the system is out of memory");

        return false;
    }

    logmsg(LOG_DEBUG, "inventory: Attempting to create inventory table");

    inventories = htable_create(16);

    if(inventories == NULL){
        logmsg(LOG_WARN, "inventory: Could not allocate inventory table, the system is out of memory");

        free(items);

        return false;
    }

    return true;
}

bool inventory_create(uint16_t entity_id, uint16_t* item_ids, size_t ids_size){
    logmsg(LOG_DEBUG, "inventory: Creating new inventory for entity:%" PRIu16, entity_id);

    if(inventories == NULL){
        logmsg(LOG_ERR, "inventory: Attempted to create inventory, but inventory table does not exist");

        _exit(-1);
    }

    Inventory* inv = calloc(1, sizeof(Inventory));

    if(inv == NULL){
        logmsg(LOG_WARN, "inventory: Failed to allocate new inventory, the system is out of memory");

        return false;
    }

    if(item_ids != NULL){
        for(size_t i = 0; i < ids_size; i++){
            inv->items[i] = item_ids[i];
        }
    }

    if(htable_add(inventories, (uint8_t*)&entity_id, sizeof(entity_id), inv) != 0){
        logmsg(LOG_WARN, "inventory: ");

        return false;
    }

    return true;
}

void inventory_destroy(uint16_t entity_id){
    logmsg(LOG_DEBUG, "inventory: Destroying inventory for entity:%" PRIu16, entity_id);

    if(inventories == NULL){
        logmsg(LOG_ERR, "inventory: Attempted to destroy inventory, but inventory table does not exist");

        _exit(-1);
    }

    Inventory* inv = htable_lookup(inventories, (uint8_t*)&entity_id, sizeof(entity_id));

    if(inv != NULL){
        free(inv);

        htable_remove(inventories, (uint8_t*)&entity_id, sizeof(entity_id));
    }
}
