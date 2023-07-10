#include <stdbool.h>

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

#include "log.h"

lua_State* state = NULL;

bool script_init() {
    state = luaL_newstate();

    if(state == NULL) {
        logmsg(LOG_WARN, "script: Unable to initialize Lua state, the system is out of memory");

        return false;
    }

    luaL_openlibs(state);

    return true;
}

bool script_run(const char* path) {
    logmsg(LOG_DEBUG, "script(%s): Attempting to execute script", path);

    int ret = luaL_dofile(state, path);

    switch(ret) {
        case LUA_OK:
            logmsg(LOG_INFO, "script(%s): Successfully executed script", path);

            return true;
        case LUA_ERRRUN:
            logmsg(LOG_WARN, "script(%s): Script failed due to runtime error", path);
            logmsg(LOG_WARN, "script(%s): Script error: %s", path, lua_tostring(state, -1));

            break;
        case LUA_ERRMEM:
            logmsg(LOG_WARN, "script(%s): Script failed, the system is out of memory", path);

            break;
        case LUA_ERRERR:
            logmsg(LOG_WARN, "script(%s): Script failed due to message handler error", path);
            logmsg(LOG_WARN, "script(%s): Script error: %s", path, lua_tostring(state, -1));

            break;
        case LUA_ERRSYNTAX:
            logmsg(LOG_WARN, "script(%s): Script failed, syntax error during precompilation", path);
            logmsg(LOG_WARN, "script(%s): Script error: %s", path, lua_tostring(state, -1));

            break;
        case LUA_ERRFILE:
            logmsg(LOG_WARN, "script(%s): Failed to open or read script file", path);

            break;
    }

    return false;
}
