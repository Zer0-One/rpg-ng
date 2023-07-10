// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include "htable.h"
#include "log.h"

typedef struct HTableEntry {
    size_t key_size;
    uint8_t* key;
    void* value;
} HTableEntry;

typedef struct HashTable {
    size_t mapping_count;
    size_t bucket_count;
    HTableEntry* buckets;
} HashTable;

// Bob Jenkins One-At-A-Time hash
uint32_t hash(const uint8_t* key, size_t len) {
    uint32_t hash = 0;

    for(size_t i = 0; i < len; i++) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

HashTable* htable_create(size_t size) {
    if(size == 0) {
        logmsg(LOG_WARN, "htable: Cannot create hash table of size 0");

        return NULL;
    }

    HashTable* t = calloc(1, sizeof(HashTable));

    if(t == NULL) {
        logmsg(LOG_WARN, "htable: Unable to create table, the system is out of memory");

        return NULL;
    }

    t->buckets = calloc(size, sizeof(HTableEntry));

    if(t == NULL) {
        logmsg(LOG_WARN, "htable: Unable to initialize table, the system is out of memory");

        free(t);

        return NULL;
    }

    t->bucket_count = size;

    logmsg(LOG_DEBUG, "htable: Created new hash table (%p) of size %zd", t, size);

    return t;
}

void htable_destroy(HashTable* t) {
    if(t == NULL) {
        logmsg(LOG_WARN, "htable: Attempted to free null table");

        return;
    }

    for(size_t i = 0; i < t->bucket_count; i++) {
        free(t->buckets[i].key);
    }

    free(t->buckets);

    free(t);
}

void* htable_lookup(const HashTable* t, const uint8_t* key, size_t key_size) {
    if(t == NULL) {
        logmsg(LOG_WARN, "htable: Attempted a lookup in a null table");

        return NULL;
    }

    if(key == NULL) {
        logmsg(LOG_WARN, "htable: Attempted a lookup using a null key");

        return NULL;
    }

    if(key_size == 0) {
        logmsg(LOG_WARN, "htable: Attempted a lookup using a key of size 0");

        return NULL;
    }

    size_t index = hash(key, key_size) % t->bucket_count;

    // Iterate over buckets in a circle until we find the key
    for(size_t i = index; i != index - 1; i = (i + 1) % t->bucket_count) {
        if(t->buckets[i].key_size == key_size) {
            size_t j;

            for(j = 0; j < key_size; j++) {
                if(t->buckets[i].key[j] != key[j]) {
                    break;
                }
            }

            if(j == key_size) {
                return t->buckets[i].value;
            }
        }
    }

    return NULL;
}

int htable_rehash(HashTable* t, size_t scale) {
    if(scale < 2) {
        logmsg(LOG_WARN, "htable: Attempted to scale table by less than 2x");

        return -1;
    }

    HashTable* new_t = htable_create(t->bucket_count * scale);

    if(new_t == NULL) {
        logmsg(LOG_WARN, "htable: Could not resize/rehash table, the system is out of memory");

        return -1;
    }

    for(size_t i = 0; i < t->bucket_count; i++) {
        int result = htable_add(new_t, t->buckets[i].key, t->buckets[i].key_size, t->buckets[i].value);

        if(result != 0) {
            htable_destroy(new_t);

            logmsg(LOG_WARN, "htable: Rehashing failed");

            return -1;
        }
    }

    // Swap hash table contents
    HTableEntry* tmp = t->buckets;
    t->buckets = new_t->buckets;
    t->bucket_count = new_t->bucket_count;
    t->mapping_count = new_t->mapping_count;
    new_t->buckets = tmp;

    // Free old table
    htable_destroy(new_t);

    return 0;
}

int htable_add(HashTable* t, const uint8_t* key, size_t key_size, void* value) {
    if(t == NULL) {
        logmsg(LOG_WARN, "htable: Attempted to add a mapping to a null table");

        return -1;
    }

    if(key == NULL) {
        logmsg(LOG_WARN, "htable: Attempted to add a null key to table");

        return -1;
    }

    if(key_size == 0) {
        logmsg(LOG_WARN, "htable: Attempted to add a key of size 0 to table");

        return -1;
    }

    if(value == NULL) {
        logmsg(LOG_WARN, "htable: Attempted to add a null value to table");

        return -1;
    }

    if(htable_lookup(t, key, key_size) != NULL) {
        logmsg(LOG_WARN, "htable: Unable to add mapping to table, key already exists");

        return -2;
    }

    size_t index = hash(key, key_size) % t->bucket_count;

    // Iterate in a circle until we find an empty bucket
    for(size_t i = index; i != index - 1; i = (i + 1) % t->bucket_count) {
        if(t->buckets[i].key == NULL) {
            t->buckets[i].key = malloc(key_size);

            if(t->buckets[i].key == NULL) {
                logmsg(LOG_WARN, "htable: Unable to add mapping to table, the system is out of memory");

                return -3;
            }

            memcpy(t->buckets[i].key, key, key_size);

            t->buckets[i].key_size = key_size;
            t->buckets[i].value = value;

            t->mapping_count++;

            return 0;
        }
    }

    // We ran out of space, resize and rehash the table
    if(htable_rehash(t, 2) != 0) {
        logmsg(LOG_WARN, "htable: Rehash failed");

        return -3;
    }

    return htable_add(t, key, key_size, value);
}

int htable_remove(HashTable* t, const uint8_t* key, size_t key_size) {
    if(t == NULL) {
        logmsg(LOG_WARN, "htable: Attempted to remove a mapping from a null table");

        return -1;
    }

    if(key == NULL) {
        logmsg(LOG_WARN, "htable: Attempted to remove a null key from table");

        return -1;
    }

    if(key_size == 0) {
        logmsg(LOG_WARN, "htable: Attempted to remove a key of size 0 from table");

        return -1;
    }

    size_t index = hash(key, key_size) % t->bucket_count;

    // Iterate over buckets in a circle until we find the key
    for(size_t i = index; i != index - 1; i = (i + 1) % t->bucket_count) {
        if(t->buckets[i].key_size == key_size) {
            size_t j;

            for(j = 0; j < key_size; j++) {
                if(t->buckets[i].key[j] != key[j]) {
                    break;
                }
            }

            // Key found
            if(j == key_size) {
                free(t->buckets[i].key);
                t->buckets[i].key = NULL;
                t->buckets[i].key_size = 0;
                t->buckets[i].value = NULL;

                return 0;
            }
        }
    }

    logmsg(LOG_WARN, "htable: Unable to remove mapping from table, key not found");

    return -2;
}

HTableKey* htable_get_keys(const HashTable* t, size_t* size) {
    if(t == NULL) {
        logmsg(LOG_WARN, "htable: Unable to get keys from NULL table");

        return NULL;
    }

    HTableKey* ret = calloc(t->mapping_count, sizeof(HTableKey));

    if(ret == NULL) {
        logmsg(LOG_ERR, "htable: Unable to create hash table key array, the system is out of memory");

        _exit(-1);
    }

    size_t cnt = 0;
    for(size_t i = 0; i < t->bucket_count; i++) {
        if(t->buckets[i].key != NULL) {
            ret[cnt].key_size = t->buckets[i].key_size;
            ret[cnt].key = t->buckets[i].key;
        }
    }

    *size = cnt;

    return ret;
}

size_t htable_get_size(const HashTable* t) {
    return t->bucket_count;
}

size_t htable_get_mapping_size(const HashTable* t) {
    return t->mapping_count;
}
