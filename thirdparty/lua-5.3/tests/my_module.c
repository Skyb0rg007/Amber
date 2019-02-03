#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <math.h>

static int c_swap(lua_State *L)
{
    double arg1 = luaL_checknumber(L, 1);
    double arg2 = luaL_checknumber(L, 2);

    lua_pushnumber(L, arg2);
    lua_pushnumber(L, arg1);

    return 2;
}

static int my_sin(lua_State *L)
{
    double arg = luaL_checknumber(L, 1);
    lua_pushnumber(L, sin(arg));
    return 1;
}

static const struct luaL_Reg my_lib[] = {
    {"c_swap", c_swap},
    {"mysin", my_sin},
    {NULL, NULL}
};

int luaopen_my_module(lua_State *L)
{
    luaL_newlib(L, my_lib);
    return 1;
}
