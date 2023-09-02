// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include "../htable.h"
#include "../log.h"

#include "dialogue.h"

HashTable* dialogues = NULL;

bool dialogue_init() {
    logmsg(LOG_DEBUG, "dialogue: Attempting to initialize dialogue");

    if (dialogues != NULL) {
        logmsg(LOG_WARN, "dialogue: Init failed, this component was already initialized");

        return false;
    }

    logmsg(LOG_DEBUG, "dialogue: Attempting to create dialogue table");

    dialogues = htable_create(16);

    if (dialogues == NULL) {
        logmsg(LOG_WARN, "dialogue: Could not allocate dialogue table, the system is out of memory");

        return false;
    }

    return true;
}

bool dialogue_create(uint16_t entity_id, const char* path) {
    logmsg(LOG_DEBUG, "dialogue: Creating new dialogue for entity:%" PRIu16, entity_id);

    if (dialogues == NULL) {
        logmsg(LOG_ERR, "dialogue: Attempted to create dialogue, but dialogue table does not exist");

        _exit(-1);
    }

    Dialogue* dlg = calloc(1, sizeof(Dialogue));

    if (dlg == NULL) {
        logmsg(LOG_WARN, "dialogue: Failed to allocate new dialogue, the system is out of memory");

        return false;
    }

    return true;
}

bool dialogue_destroy(uint16_t entity_id) {
    return true;
}
