#include <Amber/util/common.h>
#include <Amber/scripting/load_config.h>

#include <SDL.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <stdio.h>
#include <string.h>


int AB_load_config(struct AB_config *cfg, const char *file)
{
    int ret = 0;
    lua_State *L = luaL_newstate();
    /* Enable all of Lua for this TODO: limited IO */
    luaL_openlibs(L);

    /* Run the config file */
    ret = luaL_dofile(L, file);

    lua_getglobal(L, "resource_dir");
    if (lua_isstring(L, -1)) {
        cfg->resource_dir = strdup(lua_tostring(L, -1));
    } else {
        AB_QUICK_LOG("Expected string, got %s", luaL_typename(L, -1));
        goto cleanup;
        ret = 1;
    }

cleanup:
    lua_close(L);
    return ret;
}
