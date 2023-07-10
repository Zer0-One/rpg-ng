// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef RPGNG_HTABLE
#define RPGNG_HTABLE

#include <stdint.h>

#include "log.h"

typedef struct HashTable HashTable;

typedef struct HTableKey {
    size_t key_size;
    uint8_t* key;
} HTableKey;

typedef struct HTableMapping {
    void* value;
    const size_t key_size;
    const uint8_t key[];
} HTableMapping;

/**
 * Creates a new hash table of a given size.
 *
 * @param size The number of buckets to pre-allocate for the new hash table.
 *
 * @return On success, a pointer to a dynamically-allocated hash table.
 * @return If the given size is 0, or if the system is out of memory this
 * function returns NULL.
 */
HashTable* htable_create(size_t size);

/**
 * Frees the memory associated with the given hash table.
 */
void htable_destroy(HashTable* t);

/**
 * Adds a mapping to the given hash table.
 *
 * Keys are copied into the table, but value pointers are stored as-is; no
 * copies are made. If the objects pointed to are short-lived or change
 * locations, create long-lived copies before storing them in the table.
 *
 * @param t A HashTable to which a mapping will be added.
 * @param key A key to map in the given table.
 * @param key_size The size of the given key in bytes.
 * @param value A non-null pointer to any value. This limitation is in place to
 * keep the API simpler. If null values are allowed, then htable_lookup() would
 * return null both for errors and for keys that are mapped to null values.
 *
 * @return On success, this function returns 0.
 * @return If an invalid argument was given, this function returns -1.
 * @return If the given key already exists in the table, this function returns -2.
 * @return If the system was out of memory, this function returns -3.
 */
int htable_add(HashTable* t, const uint8_t* key, size_t key_size, void* value);

/**
 * Removes the mapping for the given key from the table. This function will not
 * scale down the table.
 *
 * @param t A HashTable from which a mapping will be removed.
 * @param key A key which was previously mapped in the given table.
 * @param key_size The size of the given key in bytes.
 *
 * @return On success, this function returns 0.
 * @return If an invalid argument was given, this function returns -1.
 * @return If the key was not present in the table, this function returns -2.
 */
int htable_remove(HashTable* t, const uint8_t* key, size_t key_size);

/**
 * Performs a lookup for the given key.
 *
 * @param t A HashTable from which a mapping will be looked-up.
 * @param key A key which was previously mapped in the given table.
 * @param key_size The size of the given key in bytes.
 *
 * @return On success, returns the object mapped to the given key.
 * @return NULL on error, or if the key was not present in the table.
 */
void* htable_lookup(const HashTable* t, const uint8_t* key, size_t key_size);

/**
 * Returns an array of keys that can be iterated over to lookup every mapping
 * present in the table.
 *
 * This function makes no guarantees about the state of the table between
 * iterations of the returned key array. A key present in the array may no
 * longer be mapped by the time you attempt a lookup.
 *
 * @param t A HashTable whose keys will be added to the returned array.
 * @param[out] size The size of the returned array.
 *
 * @return A dynamically-allocated array of hash table keys. The caller is
 * responsible for freeing this array. The keys in the returned array are
 * pointers to the keys in the table. Do not modify these values.
 */
HTableKey* htable_get_keys(const HashTable* t, size_t* size);

/**
 * Returns an array of keys and values present in the table at the time this
 * function was called.
 *
 * This function makes no guarantees about the state of the table after this
 * function returns. A mapping present in the returned array may no longer
 * exist in the table by the time you read or iterate over it.
 *
 * @param t A HashTable whose mappings will be added to the returned array.
 * @param[out] size The size of the returned array.
 *
 * @return A dynamically-allocated array of hash table mappings. The caller is
 * responsible for freeing this array.
 */
HTableMapping* htable_get_mappings(const HashTable* t, size_t* size);

/**
 * Returns the number of buckets present in the table.
 */
size_t htable_get_size(const HashTable* t);

/**
 * Returns the number of mappings present in the table.
 */
size_t htable_get_mapping_size(const HashTable* t);

#endif
