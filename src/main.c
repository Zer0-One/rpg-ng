// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdio.h>

#ifdef _MSC_VER
#include <stdlib.h>
#else
#include <unistd.h>
#endif

#include <SDL2/SDL.h>

#include "entity.h"
#ifdef _MSC_VER
#include "getopt.h"
#endif
#include "htable.h"
#include "log.h"
#include "script.h"

#include "component/component.h"
#include "component/sprite.h"
#include "component/transform.h"

#define _RPGNG_STR(x) #x
#define RPGNG_STR(x)  _RPGNG_STR(x)

void print_usage() {
    printf("Usage: rpgng [-d] [-l logfile]\n\n");
    printf("Command-line options:\n");
    printf("\n\t-d\t\tEnables debug mode, increasing logging verbosity");
    printf("\n\t-e [gamescript]\tThe main game script to execute on start");
    printf("\n\t-l [logfile]\tA logfile to which logs will be written");
    printf("\n\t-h\t\tPrints this help information");
    printf("\n\t-v\t\tPrints version information");
    printf("\n");
}

const unsigned int VERSION_MAJOR = RPGNG_VERSION_MAJOR;
const unsigned int VERSION_MINOR = RPGNG_VERSION_MINOR;
const unsigned int VERSION_PATCH = RPGNG_VERSION_PATCH;
const char* const VERSION = RPGNG_STR(RPGNG_VERSION);

void print_versions() {
    printf("rpgng version %s\n\n", RPGNG_STR(RPGNG_VERSION));
    printf("Built with support for: \n");
    printf("\tSuper Cool Lib 1.5.2\n");
    printf("\tOther Lib 5.3\n");
}

int main(int argc, char* argv[]) {
    int opt;

    log_priority log_level;
    char* log_path = NULL;
    char* mainscript_path = NULL;

    while((opt = getopt(argc, argv, "de:hvl:")) != -1) {
        switch(opt) {
            case 'd':
                log_level = LOG_DEBUG;
                break;
            case 'e':
                mainscript_path = optarg;
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
    if(log_init(log_level, log_path) != 0) {
        _exit(-1);
    }

    logmsg(LOG_INFO, "rpgng initializing");

    // Initialize SDL
    logmsg(LOG_DEBUG, "main: Initializing SDL");

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        logmsg(LOG_ERR, "main: Failed to initialize SDL: '%s'", SDL_GetError());

        _exit(-1);
    }

    // Initialize entities
    logmsg(LOG_DEBUG, "main: Initializing entity system");

    if(!entity_init()) {
        logmsg(LOG_ERR, "Failed to initialize entity subsystem");

        _exit(-1);
    }

    // Initialize component subsystems
    logmsg(LOG_DEBUG, "main: Initializing components");

    if(!component_init()) {
        _exit(-1);
    }

    // Initialize Lua scripting subsystem
    logmsg(LOG_DEBUG, "main: Initializing scripting interface");

    script_init();

    // if(mainscript_path == NULL){
    //     _exit(-1);
    // }

    // Run the main game script
    // script_run(mainscript_path);

    uint16_t e = entity_create("kitty");

    if(!transform_create(e)) {
        logmsg(LOG_ERR, "ZOMG THE TRANSFORM IS BROKEN");
        _exit(-1);
    }

    if(!sprite_create(e, "/home/zero-one/kitten.jpg")) {
        logmsg(LOG_ERR, "ZOMG THE SPRITE IS BROKEN");
        _exit(-1);
    }

    if(!transform_destroy(e)) {
        logmsg(LOG_ERR, "FAILED TO DESTROY TRANSFORM");

        _exit(-1);
    }

    if(!sprite_destroy(e)) {
        logmsg(LOG_ERR, "FAILED TO DESTROY SPRITE");

        _exit(-1);
    }

    script_foo();

    script_cleanup();

    //    uint16_t e = entity_create("adoring-fan");

    //    inventory_create(e, NULL, 0);

    // entity_destroy(69);

    // Initialize SDL
    //    if(sdl_init() != 0){
    //        _exit(-1);
    //    }
}
