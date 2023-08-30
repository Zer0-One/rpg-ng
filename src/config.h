// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef RPGNG_CONFIG
#define RPGNG_CONFIG

#include <stdbool.h>
#include <stdint.h>

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

typedef enum ConfigKVType {
    KV_STRING,
    KV_BOOL,
    KV_INT,
    KV_REAL,
    KV_NULL,
} ConfigKVType;

typedef struct ConfigKV {
    char* key;

    ConfigKVType type;

    union {
        char* value_str;
        bool value_bool;
        int value_int;
        double value_real;
    };
} ConfigKV;

typedef struct EngineConfig {
    WindowConfig window;
    ScriptConfig script;
    EntityConfig entity;
    ConfigKV* custom;
} EngineConfig;

extern EngineConfig global_config;

/**
 * Loads the engine configuration from the given path.
 *
 * @return On success, return True. On failure, return False.
 */
bool config_load(char const* path);

/**
 * Adds a string key-value pair to the global config.
 *
 * This function does not take ownership of or copy the source string. If the
 * original string is deleted or the pointer is invalidated, the added config
 * value will also become invalid.
 *
 * @return True on success, or false if the system is out of memory.
 */
bool config_add_str(char const* key, char const* value);

/**
 * Adds a bool key-value pair to the global config.
 *
 * @return True on success, or false if the system is out of memory.
 */
bool config_add_bool(char const* key, bool value);

/**
 * Adds an int key-value pair to the global config.
 *
 * @return True on success, or false if the system is out of memory.
 */
bool config_add_int(char const* key, int value);

/**
 * Adds a double key-value pair to the global config.
 *
 * @return True on success, or false if the system is out of memory.
 */
bool config_add_real(char const* key, double value);

/**
 * Removes the key-value pair with the given key from the global config.
 */
bool config_remove(char const* key);

/**
 * Writes the config to disk in the file specified by the given path.
 *
 * @param path A path to a file at which to save the config.
 *
 * @return True on success, or false on failure.
 */
bool config_save(char const* path);

/**
 * Sets the given config setting to null.
 */
bool config_set_null(char const* key);

#endif
