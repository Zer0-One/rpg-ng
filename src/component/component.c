#include "../log.h"

#include "component.h"
#include "dialogue.h"
#include "inventory.h"

bool component_init(){
    if(!inventory_init()){
        logmsg(LOG_WARN, "component: Unable to initialize inventory component");

        return false;
    }

    if(!dialogue_init()){
        logmsg(LOG_WARN, "component: Unable to initialize dialogue component");

        return false;
    }

    return true;
}
