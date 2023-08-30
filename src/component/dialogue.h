// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef RPGNG_DIALOGUE
#define RPGNG_DIALOGUE

#include <stdbool.h>
#include <stdint.h>

typedef struct Message {
    // An ID which is unique across all messages in the dialogue.
    uint16_t id;

    // A condition which must be satisfied before the message can be delivered.
    char* milestone;

    // An action (probably a user-defined function in a game script) which
    // occurs when this message is delivered.
    char* action;

    // A list of text messages, marked up with timing cues.
    // The messages are separated to indicate that they should be rendered or
    // handled separately. e.g, each one should appear in a new text box.
    size_t text_count;
    char* text;

    // A list of IDs which are valid next-hops in the tree.
    size_t next_count;
    uint16_t* next;
} Message;

typedef struct Dialogue {
    size_t message_count;
    Message* messages;
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
bool dialogue_create(uint16_t entity_id, char const* path);

/**
 * Destroys the dialogue associated with the given entity, and removes the
 * mapping from the dialogue table.
 */
bool dialogue_destroy(uint16_t entity_id);

#endif
