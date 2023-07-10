// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef RPGNG_WIDGET_DIALOGUE
#define RPGNG_WIDGET_DIALOGUE

#include <stdbool.h>
#include <stdint.h>

#include "dialogue.h"

// This gets drawn once per frame by the draw code, which iterates through all entities and looks for drawables.
// Components, including this one, should register with the entities they're associated with:
// entity_register_component(ComponentType type, void* component); // Adds component to the entity's component list
typedef struct DialogueWidget {
    // Position on the screen in pixel coordinates
    int x;
    int y;

    // Length and width of the widget
    int length;
    int width;

    // Margins from the top, bottom, and sides to add before rendering text
    int hmargin;
    int vmargin;

    // "Typing" speed. 0 for immediate display, 1 for 1 character per second, 2
    // for 2 characters per second, etc
    double speed;

    // Background texture. Can be used for borders, colors, stylized
    // backgrounds, etc. Text will be rendered on top.
    Texture* bg;

    // Font face, family, size, etc
    Font* font;

    // Dialog tree
    Dialogue* dialogue;

    // The drawing surface for the widget.
    Texture* widget;
} DialogueWidget;

// Automatically hooks up the dialogue associated with the given entity to the
// widget and fails if the entity has no dialogue
bool dialogue_widget_create(uint16_t entity_id);

// Continues processing of the dialogue tree
bool dialogue_widget_continue(uint16_t entity_id);

bool dialogue_widget_set_pos(uint16_t entity_id, int x, int y);
bool dialogue_widget_set_size(uint16_t entity_id, int length, int width);
bool dialogue_widget_set_margin(uint16_t entity_id, int hmargin, int vmargin);
bool dialogue_widget_set_speed(uint16_t entity_id, double speed);
bool dialogue_widget_set_bg(uint16_t entity_id, Texture* bg);
bool dialogue_widget_set_font(uint16_t entity_id, Font* bg);

#endif
