#include <string.h>

#include "entity.h"
#include "htable.h"
#include "log.h"

size_t entity_list_size = 0;
Entity* entity_list = NULL;

int entity_add(int id, EntityType type, char* name){
    if(id == 0){
        logmsg(LOG_ERR, "Unable to add entity '%s', 0 is not a valid entity ID", name);

        return -1;
    }

    if(entity_list == NULL){
        entity_list = calloc(10, sizeof(Entity));

        if(entity_list == NULL){
            logmsg(LOG_ERR, "Unable to initialize entity list, the system is out of memory");

            return -1;
        }

        entity_list_size = 10;
    }

    for(size_t i = 0; i < entity_list_size; i++){
        if(entity_list[i].id == 0){
            entity_list[i].id = id;
            entity_list[i].type = type;
            entity_list[i].name = strdup(name);

            if(entity_list[i].name == NULL){
                logmsg(LOG_ERR, "Unable to add entity '%s', the system is out of memory", name);

                return -1;
            }

            return 0;
        }
    }

    // There's no space left, resize

}
