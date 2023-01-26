#ifndef RPGNG_COMPONENT
#define RPGNG_COMPONENT

#include <stdbool.h>

typedef enum ComponentType {
    DIALOGUE,
    DIALOGUEWIDGET,
    INVENTORY
} ComponentType;

bool component_init();

#endif
