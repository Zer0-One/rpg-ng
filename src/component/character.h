#ifndef RPGNG_CHARACTER
#define RPGNG_CHARACTER

#include <stdint.h>

#include "component/inventory.h"

typedef struct Character {
    Inventory inv;
} Character;

#endif
