#include <Amber/util/common.h>
#include <Amber/config/conf.h>
#include <SDL_filesystem.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <dirent.h>

/* This config struct is purposefully static and flat */
static struct {
    char *datapath;
    char *configpath;
    char *cachepath;
    unsigned verbosity;
} config;

/***************************************************************************/

static AB_bool is_dir(const char *path)
{
    DIR *d = opendir(path);
    AB_bool ret = d != NULL;
    closedir(d);
    return ret;
}

static AB_bool is_file(const char *path)
{
    FILE *f = fopen(path, "r");
    AB_bool ret = f != NULL;
    fclose(f);
    return ret;
}

static char *get_global_string(lua_State *L, const char *str)
{
    int ret;
    ret = lua_getglobal(L, str);
    if (ret == LUA_TSTRING) {
        const char *lval = lua_tostring(L, -1);
        char *val = AB_STRDUP(lval);
        lua_pop(L, 1);
        return val;
    }
    return NULL;
}

static void fill_config(lua_State *L)
{
    char *datapath = get_global_string(L, "datapath");
    if (datapath && is_dir(datapath)) {
        config.datapath = datapath;
    } else {
        free(datapath);
        config.datapath = ""; /* TODO */
    }
}

/***************************************************************************/

AB_errno_t AB_config_loadfile(const char *filename)
{
    int err;
    lua_State *L;

    L = luaL_newstate();
    if (L == NULL)
        return AB_ERRMSG;

    /* Give Lua access to a sandboxed environment
     * ie. coroutine, table, string, math, limited base, limited os
     */
    luaL_open_sandbox_libs(L);

    /* Use mode "t" to prevent precompiled binary files */
    err = luaL_loadfilex(L, filename, "t");
    if (err != LUA_OK)
        return AB_ERRMSG;
    err = lua_pcall(L, 0, 0, 0);
    if (err != LUA_OK)
        return AB_ERRMSG;

    fill_config(L);

    lua_close(L);

    return AB_OK;
}

void AB_config_quit(void)
{
    free(config.datapath);
}

const char *AB_config_get_string(enum AB_config conf)
{
    switch (conf)
    {
    case AB_CONFIG_DATAPATH:
        return config.datapath;
    default:
        return NULL;
    }
}

unsigned AB_config_get_unsigned(enum AB_config conf)
{
    switch (conf)
    {
    case AB_CONFIG_VERBOSITY:
        return config.verbosity;
    default:
        return UINT_MAX;
    }
}
