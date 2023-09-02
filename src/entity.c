// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include "entity.h"
#include "htable.h"
#include "log.h"

#include "component/inventory.h"

// Maps entities by ID (uint16_t)
HashTable* entities = NULL;
// Maps entities by name (char*)
HashTable* entities_str = NULL;

uint16_t entity_next_id = 1;

bool entity_init(void) {
    logmsg(LOG_DEBUG, "entity: Attempting to initialize entity");

    if (entities != NULL || entities_str != NULL) {
        logmsg(LOG_WARN, "entity: Init failed, this system was already initialized");

        return false;
    }

    entities = htable_create(16);

    if (entities == NULL) {
        logmsg(LOG_WARN, "entity: Unable to create entity table, the system is out of memory");

        return false;
    }

    entities_str = htable_create(16);

    if (entities_str == NULL) {
        logmsg(LOG_WARN, "entity: Unable to create entity_str table, the system is out of memory");

        return false;
    }

    return true;
}

uint16_t entity_create(const char* name) {
    logmsg(LOG_DEBUG, "entity: Attempting to create new entity id:%" PRIu16 ", name:'%s'", entity_next_id, name);

    if (entities == NULL) {
        logmsg(LOG_WARN, "entity: Cannot add entity before initializing entity subsystem");

        return -1;
    }

    if (name == NULL) {
        logmsg(LOG_WARN, "entity: Cannot create entity[%" PRIu16 "] with NULL name", entity_next_id);

        return -1;
    }

    Entity* e = calloc(1, sizeof(Entity));

    if (!e) {
        logmsg(LOG_WARN, "entity: Cannot allocate new entity with name '%s', the system is out of memory", name);

        return -1;
    }

    strncpy(e->name, name, ENTITY_NAME_LEN_MAX);

    e->id = entity_next_id;

    e->components = htable_create(16);

    if (!e->components) {
        logmsg(LOG_WARN, "entity[%" PRIu16 "]('%s'): Failed to create component table, the system is out of memory", e->id, e->name);

        free(e);

        return -1;
    }

    if (htable_add(entities, (uint8_t*)&e->id, sizeof(e->id), KV_VOIDPTR, e) != 0) {
        logmsg(LOG_WARN, "entity[%" PRIu16 "]('%s'): Unable to map newly created entity in entity table", e->id, e->name);

        free(e);

        return -1;
    }

    if (htable_add(entities_str, (uint8_t*)name, strlen(name), KV_VOIDPTR, e) != 0) {
        logmsg(LOG_WARN, "entity[%" PRIu16 "]('%s'): Unable to map newly created entity in entity string table", e->id, e->name);

        if (htable_remove(entities, (uint8_t*)&e->id, sizeof(e->id)) != 0) {
            // We just added that mapping. If we can't remove it, something's really fucked.
            logmsg(LOG_ERR, "entity[%" PRIu16 "]('%s'): Failed to remove mapping from entity table", e->id, e->name);
            logmsg(LOG_ERR, "entity[%" PRIu16 "]('%s'): Something's fucked", e->id, e->name);

            _exit(-1);
        }

        free(e);

        return -1;
    }

    entity_next_id++;

    return e->id;
}

bool entity_destroy(uint16_t id) {
    Entity* e = entity_get(id);

    if (!e) {
        logmsg(LOG_WARN, "entity[%" PRIu16 "]: Unable to destroy entity, not found in entity table", id);

        return false;
    }

    if (!component_cleanup(id)) {
        _exit(-1);
    }

    if (htable_remove(entities, (uint8_t*)&id, sizeof(id)) != 0) {
        logmsg(LOG_ERR, "entity[%" PRIu16 "]('%s'): Unable to destroy entity, failed to remove mapping in entity table", id, e->name);

        _exit(-1);
    }

    if (htable_remove(entities_str, (uint8_t*)e->name, strlen(e->name)) != 0) {
        logmsg(LOG_WARN, "entity[%" PRIu16 "]('%s'): Unable to destroy entity, failed to remove mapping in entity_str table", id, e->name);

        _exit(-1);
    }

    free(e);

    return true;
}

Entity* entity_get(uint16_t id) {
    Entity* e = htable_lookup(entities, (uint8_t*)&id, sizeof(id), NULL);

    if (!e) {
        logmsg(LOG_WARN, "entity[%" PRIu16 "]: Failed to get entity, not found in entity table", id);

        return NULL;
    }

    return e;
}

bool entity_has_component(uint16_t id, ComponentType type) {
    Entity* e = htable_lookup(entities, (uint8_t*)&id, sizeof(id), NULL);

    if (!e) {
        logmsg(LOG_WARN, "entity[%" PRIu16 "]: Failed to check if entity has component, entity not mapped in entity table", id);

        return false;
    }

    if (htable_lookup(e->components, (uint8_t*)&type, sizeof(type), NULL) != NULL) {
        return true;
    }

    return false;
}

void* entity_get_component(uint16_t id, ComponentType type) {
    logmsg(LOG_DEBUG, "entity[%" PRIu16 "]: Attempting to get component %d from entity", type);

    if (!entity_has_component(id, type)) {
        logmsg(LOG_WARN, "entity[%" PRIu16 "]: Failed to get component, entity does not have a component of type %d", type);

        return NULL;
    }

    Entity* e = htable_lookup(entities, (uint8_t*)&id, sizeof(id), NULL);

    if (e == NULL) {
        logmsg(LOG_WARN, "entity[%" PRIu16 "]: Failed to get component, entity not mapped in entity table", id);

        return NULL;
    }

    void* obj = htable_lookup(e->components, (uint8_t*)&type, sizeof(type), NULL);

    if (obj == NULL) {
        logmsg(LOG_WARN, "entity[%" PRIu16 "]: Failed to get component, requested component not mapped to entity", id);

        return NULL;
    }

    return obj;
}
