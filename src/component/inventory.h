// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef RPGNG_INVENTORY
#define RPGNG_INVENTORY

#include <stdbool.h>
#include <stdint.h>

#define INV_ITEM_NAME_LEN_MAX 128
#define INV_ITEM_DESC_LEN_MAX 256

typedef struct Item {
    uint16_t id;

    char name[INV_ITEM_NAME_LEN_MAX];
    char description[INV_ITEM_DESC_LEN_MAX];

    uint16_t value;
} Item;

#define INV_SIZE_MAX 256

typedef struct Inventory {
    uint16_t items[INV_SIZE_MAX];
} Inventory;

/**
 * Initializes the inventory subsystem.
 *
 * @return True on success.
 * @return False on failure.
 */
bool inventory_init(void);

/**
 * Creates a new item type with the given name, description, and monetary value.
 *
 * @return An ID which can be used to reference this item type.
 */
uint16_t inventory_item_create(char* name, char* description, uint16_t value);

/**
 * Destroys the item referenced by the given ID. This will automatically remove
 * the item from all inventories that currently hold the item.
 */
void inventory_item_destroy(uint16_t item_id);

/**
 * Returns a dynamically-allocated copy of an item referenced by the given ID.
 *
 * The caller is responsible for freeing the item returned by this function.
 */
Item* inventory_item_get(uint16_t item_id);

/**
 * Returns a dynamically-allocated copy of the first item whose name matches
 * the given name. Order is not guaranteed to remain the same between calls, so
 * using this method is not recommended for games in which there are several
 * items with the same name.
 *
 * The caller is responsible for freeing the item returned by this function.
 */
Item* inventory_item_get_byname(const char* name);

/**
 * Creates a new inventory, initializes it with the items represented by the
 * given set of item IDs, and then maps it in the entity component table.
 *
 * @param entity_id The ID of an entity with which the created inventory will be associated.
 * @param item_ids An array of item IDs which will be added to the newly-created inventory.
 * @param ids_size The size of the given item ID array.
 *
 * @return On success, returns true.
 * @return On failure, returns false.
 */
bool inventory_create(uint16_t entity_id, uint16_t* item_ids, size_t ids_size);

/**
 * Destroys the inventory associated with the given entity, and removes the
 * mapping from the entity's component table.
 *
 * @return On success, returns true.
 * @return On failure, returns false.
 */
bool inventory_destroy(uint16_t entity_id);

#endif
