#include <Amber/util/common.h>
#include <Amber/graphics/graphics.h>
#include "graphics_system.h"

#include <SDL.h>

void graphics_system_cleanup(struct AB_ECS_world *world, struct AB_ECS_system *self)
{
    (void)world;
    struct graphics_data *data = self->userdata;

    SDL_GL_DeleteContext(data->gl_ctx);
    SDL_DestroyWindow(data->win);
    SDL_free(data->base_path);
    SDL_free(data->pref_path);
    free(data);
#ifdef NDEBUG
    /* This calls dlclose(), which prevents leak analysis */
    SDL_Quit();
#endif
}
