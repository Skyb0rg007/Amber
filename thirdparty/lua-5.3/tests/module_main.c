/* Lua */
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
/* std */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int err;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <module-name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    err = luaL_loadfile(L, argv[1]);
    if (err) {
        fprintf(stderr, "Error loading %s!\n", argv[1]);
        return EXIT_FAILURE;
    }

    err = lua_pcall(L, 0, 1, 0);
    if (err) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
        return EXIT_FAILURE;
    }

    lua_close(L);

    return EXIT_SUCCESS;
}
