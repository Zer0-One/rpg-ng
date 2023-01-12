#include <stdio.h>
#include <unistd.h>

#include "entity.h"
#include "htable.h"
#include "log.h"

#include "component/dialogue.h"
#include "component/inventory.h"

#define _RPGNG_STR(x) #x
#define RPGNG_STR(x) _RPGNG_STR(x)

void print_usage(){
    printf("Usage: rpg-ng [-d] [-l logfile]\n\n");
    printf("Command-line options:\n");
    printf("\n\t-d\t\tEnables debug mode, increasing logging verbosity");
    printf("\n\t-l [logfile]\tA logfile to which logs will be written");
    printf("\n\t-h\t\tPrints this help information");
    printf("\n\t-v\t\tPrints version information");
    printf("\n");
}

const unsigned int VERSION_MAJOR = RPGNG_VERSION_MAJOR;
const unsigned int VERSION_MINOR = RPGNG_VERSION_MINOR;
const unsigned int VERSION_PATCH = RPGNG_VERSION_PATCH;
const char* const VERSION = RPGNG_STR(RPGNG_VERSION);

void print_versions(){
    printf("rpg-ng version %s\n\n", RPGNG_STR(RPGNG_VERSION));
    printf("Built with support for: \n");
    printf("\tSuper Cool Lib 1.5.2\n");
    printf("\tOther Lib 5.3\n");
}

int main(int argc, char* argv[]){
    int opt;

    log_priority log_level;
    char* log_path = NULL;

    while((opt = getopt(argc, argv, "dhvl:")) != -1){
        switch(opt){
            case 'd':
                log_level = LOG_DEBUG;
                break;
            case 'l':
                log_path = optarg;
                break;
            case 'h':
                print_usage();
                _exit(0);
            case 'v':
                print_versions();
                _exit(0);
            case '?':
                print_usage();
                _exit(-1);
        }
    }

    // Initialize logging
    if(log_init(log_level, log_path) != 0){
        _exit(-1);
    }

    logmsg(LOG_INFO, "rpg-ng initializing");

    // Initialize entities
    if(!entity_init()){
        logmsg(LOG_ERR, "Failed to initialize entity subsystem");

        _exit(-1);
    }

    // Initialize component subsystems
    if(!inventory_init()){
        _exit(-1);
    }
    if(!dialogue_init()){
        _exit(-1);
    }

    uint16_t e = entity_create("adoring-fan");

    inventory_create(e, NULL, 0);

    //entity_destroy(69);

    // Initialize SDL
//    if(sdl_init() != 0){
//        _exit(-1);
//    }
}
