// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>
#include <string.h>

#include <jansson.h>

#include "config.h"
#include "htable.h"
#include "log.h"

#define KV_STR_MAX_LEN 1024

#ifdef __WIN32__
#define strdup _strdup
#endif

// TODO(zero-one): Configure with sane defaults on startup
EngineConfig global_config;

void config_set_defaults() {
    logmsg(LOG_DEBUG, "config: Applying default settings to global config");

    // window settings
    global_config.window.mode = "fullscreen";
    global_config.window.x = -1;
    global_config.window.y = -1;
    global_config.window.width = 640;
    global_config.window.height = 360;
    global_config.window.borderless = true;
    global_config.window.hidpi = false;
    global_config.window.resizeable = false;

    // script settings
    global_config.script.isolate = true;
    global_config.script.safe_api = true;

    // entity settings
    global_config.entity.root_name = "root";
    global_config.entity.first_id = 0;
}

bool config_init() {
    if (global_config.custom) {
        logmsg(LOG_WARN, "config: Failed to initialize global config, already initialized");

        return false;
    }

    config_set_defaults();

    global_config.custom = htable_create(16);

    if (!global_config.custom) {
        logmsg(LOG_WARN, "config: Failed to create custom settings table, the system is out of memory");

        return false;
    }

    return true;
}

bool config_load(const char* path) {
    logmsg(LOG_DEBUG, "config: Attempting to load configuration at '%s'", path);

    if (!path) {
        logmsg(LOG_WARN, "config: Unable to load config, path is NULL");

        return false;
    }

    json_error_t err;

    json_t* root = json_load_file(path, JSON_REJECT_DUPLICATES, &err);

    if (!root) {
        logmsg(LOG_WARN, "config(%s): Failed to load config, parsing error", path);
        logmsg(LOG_WARN, "config(%s): %s at line %d, column %d", path, err.text, err.line, err.column);

        return false;
    }

    json_t* window = NULL;
    json_t* entity = NULL;
    json_t* script = NULL;
    json_t* custom = NULL;

    int unpk = json_unpack_ex(root, &err, JSON_STRICT, "{s:o, s:o, s:o, s?o}", "window", &window, "entity", &entity, "script", &script, "custom", &custom);

    if (unpk == -1) {
        logmsg(LOG_WARN, "config(%s): Failed to load config, parsing error", path);
        logmsg(LOG_WARN, "config(%s): %s at line %d, column %d", path, err.text, err.line, err.column);

        goto fail;
    }

    // Load window config
    char* mode = NULL;

    unpk = json_unpack_ex(window,
        &err,
        0,
        "{s?s, s?i, s?i, s?i, s?i, s?b, s?b, s?b}",
        "mode",
        &mode,
        "x_pos",
        &global_config.window.x,
        "y_pos",
        &global_config.window.y,
        "width",
        &global_config.window.width,
        "height",
        &global_config.window.height,
        "borderless",
        &global_config.window.borderless,
        "hidpi",
        &global_config.window.hidpi,
        "resizeable",
        &global_config.window.resizeable);

    if (unpk == -1) {
        logmsg(LOG_WARN, "config(%s): Failed to load window config, parsing error", path);
        logmsg(LOG_WARN, "config(%s): %s at line %d, column %d", path, err.text, err.line, err.column);

        goto fail;
    }

    if (mode) {
        global_config.window.mode = strdup(mode);

        if (!global_config.window.mode) {
            logmsg(LOG_WARN, "config(%s): Failed to load window config, the system is out of memory", path);

            goto fail;
        }
    }

    // Load script config
    unpk = json_unpack_ex(script, &err, 0, "{s?b, s?b}", "isolate", &global_config.script.isolate, "safe_api", &global_config.script.safe_api);

    if (unpk == -1) {
        logmsg(LOG_WARN, "config(%s): Failed to load script config, parsing error", path);
        logmsg(LOG_WARN, "config(%s): %s at line %d, column %d", path, err.text, err.line, err.column);

        goto fail;
    }

    // Load entity config
    char* root_name = NULL;
    int first_id = -1;

    unpk = json_unpack_ex(entity, &err, 0, "{s?s, s?i}", "root_name", &root_name, "first_id", &first_id);

    if (unpk == -1) {
        logmsg(LOG_WARN, "config(%s): Failed to load script config, parsing error", path);
        logmsg(LOG_WARN, "config(%s): %s at line %d, column %d", path, err.text, err.line, err.column);

        goto fail;
    }

    // Check that value fits in a uint16_t before setting it
    if ((first_id > -1) && (first_id < (1 << 16))) {
        global_config.entity.first_id = first_id;
    }

    if (root_name) {
        global_config.entity.root_name = strdup(root_name);

        if (!global_config.entity.root_name) {
            logmsg(LOG_WARN, "config(%s): Failed to load window config, the system is out of memory", path);

            goto fail;
        }
    }

    json_decref(root);

    return true;

fail:
    json_decref(root);

    free(global_config.window.mode);
    free(global_config.entity.root_name);

    return false;
}

bool config_add(const char* key, KVType type, void* value) {
    if (!key) {
        logmsg(LOG_WARN, "config: Unable to add config setting, key must be non-null");

        return false;
    }

    if (!global_config.custom) {
        logmsg(LOG_WARN, "config: Unable to add config setting '%s', global config table not initialized", key);

        return false;
    }

    if (htable_add(global_config.custom, (const uint8_t*)key, strlen(key) + 1, type, value) != 0) {
        logmsg(LOG_WARN, "config: Failed to add config setting '%s'");

        return false;
    }

    return true;
}

void* config_get(const char* key, KVType* type) {
    if (!key) {
        logmsg(LOG_WARN, "config: Unable to get config setting, key must not be null");

        return false;
    }

    if (!global_config.custom) {
        logmsg(LOG_WARN, "config: Unable to get config setting '%s', global config table not initialized", key);

        return false;
    }

    void* ret = htable_lookup(global_config.custom, (uint8_t*)key, strlen(key) + 1, type);

    if (!ret) {
        logmsg(LOG_WARN, "config: Failed to get config setting '%s'", key);
    }

    return ret;
}

bool config_remove(const char* key) {
    if (!key) {
        logmsg(LOG_WARN, "config: Unable to remove config setting, key must not be null");

        return false;
    }

    if (!global_config.custom) {
        logmsg(LOG_WARN, "config: Unable to remove config setting '%s', global config table not initialized", key);

        return false;
    }

    if (htable_remove(global_config.custom, (const uint8_t*)key, strlen(key) + 1) != 0) {
        logmsg(LOG_WARN, "config: Failed to remove config setting '%s'", key);

        return false;
    }

    return true;
}
