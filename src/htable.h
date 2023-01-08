#ifndef RPGNG_HTABLE
#define RPGNG_HTABLE

#include <stdint.h>

#include "log.h"

typedef struct HashTable HashTable;

typedef struct HTableKey {
    const size_t key_size;
    const uint8_t key[];
} HTableKey;

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
 * Adds an entry to the given hash table.
 *
 * Key and value pointers are stored as-is in the table. No copies of the data
 * are made. If the objects pointed to are short-lived or change locations,
 * create copies before storing them in the table.
 *
 * Notice that this function takes no length for the given value. You should
 * know the size of the thing you're storing before storing it.
 *
 * @param value A non-null pointer to any value. This limitation is in place to
 * keep the API simpler. If null values are allowed, then htable_lookup() would
 * return null for both errors and for keys that are mapped to null values.
 *
 * @return On success, this function returns 0.
 * @return If an invalid argument was given, this function returns -1.
 * @return If the given key already exists in the table, this function returns -2.
 * @return If the system was out of memory, this function returns -3.
 */
int htable_add(HashTable* t, const uint8_t* key, size_t key_size, const void* value);
int htable_remove(HashTable* t, const uint8_t* key, size_t key_size);
void* htable_lookup(const HashTable* t, const uint8_t* key, size_t key_size);
HTableKey* htable_get_keys(const HashTable* t, size_t* size);

#endif
