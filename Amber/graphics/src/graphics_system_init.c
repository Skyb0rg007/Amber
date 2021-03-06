#include <Amber/graphics/graphics.h>
#include <Amber/ECS/world.h>
#include <Amber/util/common.h>
#include <Amber/util/ring.h>
#include "graphics_system.h"

#include <SDL.h>
#ifdef __EMSCRIPTEN__
# include <GL/gl.h>
#else
# include <glad/glad.h>
#endif

AB_errno_t graphics_system_init(struct AB_ECS_system *self)
{
    /* Initialize internal data structure */
    struct graphics_data *data = malloc(sizeof *data);
    AB_ASSERT(data != NULL);

    /* Initialize SDL2 */
    int status = SDL_Init(SDL_INIT_VIDEO);
    AB_ASSERT(status == 0);

    /* Hint SDL for the OpenGL context profile */
#ifndef __EMSCRIPTEN__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 
            SDL_GL_CONTEXT_DEBUG_FLAG 
            | SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif

    /* Create the SDL OpenGL window */
    data->win = SDL_CreateWindow(
            "Amber Engine",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    data->gl_ctx = SDL_GL_CreateContext(data->win);

#ifndef __EMSCRIPTEN__
    /* Load GLAD */
    status = gladLoadGLLoader(SDL_GL_GetProcAddress);
    AB_ASSERT(status != 0);

    /* Misc SDL2 stuff */
    data->base_path = SDL_GetBasePath();
    data->pref_path = SDL_GetPrefPath(AMBER_ORG, AMBER_APP);
#else
    data->base_path = "<base>";
    data->pref_path = "<pref>";
#endif

    self->userdata = data;
    return AB_OK;
}
