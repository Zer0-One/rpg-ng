// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>
#include <string.h>

#include <jansson.h>

#include "config.h"
#include "log.h"

#define KV_STR_MAX_LEN 1024

#ifdef __WIN32__
#define strdup _strdup
#endif

// TODO(zero-one): Configure with sane defaults on startup
EngineConfig global_config;

bool config_load(char const* path) {
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

    unpk = json_unpack_ex(script, &err, 0, "{s?b, s?b}", "isolate", &global_config.script.isolate, "safe_api", &global_config.script.safe_api);

    if (unpk == -1) {
        logmsg(LOG_WARN, "config(%s): Failed to load script config, parsing error", path);
        logmsg(LOG_WARN, "config(%s): %s at line %d, column %d", path, err.text, err.line, err.column);

        goto fail;
    }

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

ConfigKV* config_create_kv(char const* key) {
    if (!key) {
        logmsg(LOG_WARN, "config: Unable to add config setting, key must be non-null");

        return NULL;
    }

    ConfigKV* c = malloc(sizeof(ConfigKV));

    if (!c) {
        logmsg(LOG_WARN, "config: Unable to add config setting '%s', the system is out of memory", key);

        return NULL;
    }

    c->key = strdup(key);

    if (!c->key) {
        free(c);

        logmsg(LOG_WARN, "config: Unable to add config setting '%s', the system is out of memory", key);

        return NULL;
    }

    return c;
}

bool config_add_str(char const* key, char const* value) {
    if (!value) {
        logmsg(LOG_WARN, "config: Unable to add config setting '%s', value must be non-null", key);

        return false;
    }

    ConfigKV* c = config_create_kv(key);

    if (!c) {
        return false;
    }

    c->value_str = strdup(value);

    if (!c->value_str) {
        free(c->key);
        free(c);

        logmsg(LOG_WARN, "config: Unable to add config setting '%s', the system is out of memory", key);

        return false;
    }

    c->type = KV_STRING;

    return true;
}

bool config_add_bool(char const* key, bool value) {
    ConfigKV* c = config_create_kv(key);

    if (!c) {
        return false;
    }

    c->value_bool = value;

    c->type = KV_BOOL;

    return true;
}

bool config_add_int(char const* key, int value) {
    ConfigKV* c = config_create_kv(key);

    if (!c) {
        return false;
    }

    c->value_int = value;

    c->type = KV_INT;

    return true;
}

bool config_add_real(char const* key, double value) {
    ConfigKV* c = config_create_kv(key);

    if (!c) {
        return false;
    }

    c->value_real = value;

    c->type = KV_REAL;

    return true;
}

bool config_set_null(char const* key) {}
