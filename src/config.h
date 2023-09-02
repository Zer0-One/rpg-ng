// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef RPGNG_CONFIG
#define RPGNG_CONFIG

#include <stdbool.h>
#include <stdint.h>

#include "htable.h"

typedef struct WindowConfig {
    char* mode;

    int x;
    int y;

    int width;
    int height;

    bool borderless;
    bool hidpi;
    bool resizeable;
} WindowConfig;

typedef struct ScriptConfig {
    bool isolate;
    bool safe_api;
} ScriptConfig;

typedef struct EntityConfig {
    char* root_name;

    uint16_t first_id;
} EntityConfig;

typedef struct EngineConfig {
    WindowConfig window;
    ScriptConfig script;
    EntityConfig entity;
    HashTable* custom;
} EngineConfig;

extern EngineConfig global_config;

/**
 * Initializes the custom config table, and applies default settings to the
 * global config.
 *
 * This function must be called once before the global config is used.
 */
bool config_init();

/**
 * Loads the engine configuration from the given path.
 *
 * @return On success, return True. On failure, return False.
 */
bool config_load(const char* path);

/**
 * Adds a key-value pair to the global configuration.
 *
 * @param type The type of the given value.
 */
bool config_add(const char* key, KVType type, void* value);

/**
 * Fetches the given config setting from the global configuration.
 *
 * @param[out] type The type of the config value, if found. This may be set to
 * NULL if the type is not needed.
 */
void* config_get(const char* key, KVType* type);

/**
 * Removes the key-value pair with the given key from the global config.
 */
bool config_remove(const char* key);

/**
 * Writes the config to disk in the file specified by the given path.
 *
 * @param path A path to a file at which to save the config.
 *
 * @return True on success, or false on failure.
 */
bool config_save(const char* path);

#endif
