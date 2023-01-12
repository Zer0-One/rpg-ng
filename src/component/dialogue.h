#ifndef RPGNG_DIALOGUE
#define RPGNG_DIALOGUE

#include <stdbool.h>
#include <stdint.h>

typedef struct Dialogue {
    int placeholder;
} Dialogue;

/**
 * Initializes the dialogue subsystem.
 *
 * @return True on success.
 * @return False on failure.
 */
bool dialogue_init();

/**
 * Creates a new dialogue and associates it with the given entity.
 *
 * @param entity_id The ID of an entity with which this dialogue will be associated.
 * @param path The path to a dialogue file.
 *
 * @return On success, returns true.
 * @return On failure, returns false.
 */
bool dialogue_create(uint16_t entity_id, const char* path);

/**
 * Destroys the dialogue associated with the given entity, and removes the
 * mapping from the dialogue table.
 */
void dialogue_destroy(uint16_t entity_id);

#endif
