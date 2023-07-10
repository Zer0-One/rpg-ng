// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include "../entity.h"
#include "../htable.h"
#include "../log.h"

#include "component.h"
#include "dialogue.h"
#include "inventory.h"
#include "transform.h"

bool component_init() {
    if(!inventory_init()) {
        logmsg(LOG_WARN, "component: Unable to initialize inventory component");

        return false;
    }

    if(!dialogue_init()) {
        logmsg(LOG_WARN, "component: Unable to initialize dialogue component");

        return false;
    }

    return true;
}

bool component_cleanup(uint16_t entity_id) {
    Entity* e = entity_get(entity_id);

    if(!e) {
        return false;
    }

    size_t keys_size;
    HTableKey* keys = htable_get_keys(e->components, &keys_size);

    if(!keys) {
        logmsg(LOG_ERR, "component: Unable to get components for entity[%" PRIu16 "]('%s'), the system is out of memory", entity_id, e->name);

        _exit(-1);
    }

    for(size_t i = 0; i < keys_size; i++) {
        bool ret = false;

        switch(keys[i].key[0]) {
            case DIALOGUE:
                ret = dialogue_destroy(entity_id);
                break;
            case INVENTORY:
                ret = inventory_destroy(entity_id);
                break;
            case TRANSFORM:
                ret = transform_destroy(entity_id);
                break;
            default:
                logmsg(LOG_WARN, "component: Unknown component[%" PRIu8 "] associated with entity[%" PRIu16 "]('%s') cannot be destroyed", keys[i].key, entity_id, e->name);
        }

        if(!ret) {
            logmsg(LOG_WARN, "component: Failed to destroy component[%" PRIu8 "] associated with entity[%" PRIu16 "]('%s')", keys[i].key, entity_id, e->name);
        }
    }

    free(keys);

    return true;
}
