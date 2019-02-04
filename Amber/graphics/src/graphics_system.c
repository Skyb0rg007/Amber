#include <Amber/util/common.h>
#include <Amber/ECS/ECS.h>
#include <SDL.h>
#include <stdlib.h>

struct graphics_data {
    SDL_Window *win;
    SDL_GLContext gl_ctx;
};

int graphics_system_init(SystemCtx *_ctx, void **userdata)
{
    (void)_ctx;
    int status;

    struct graphics_data *data = malloc(sizeof *data);
    AB_ASSERT(data != NULL);

    status = SDL_Init(SDL_INIT_VIDEO);
    AB_ASSERT(status == 0);

    data->win = SDL_CreateWindow(
            "Amber Engine",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            800, 1000,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    *userdata = data;

    return 0;
}

int graphics_system_cleanup(SystemCtx *_ctx, void *userdata)
{
    (void)_ctx;
    struct graphics_data *data = userdata;

    SDL_DestroyWindow(data->win);
    SDL_Quit();

    return 0;
}

int graphics_system_run(SystemCtx *_ctx, void *userdata)
{
    (void)_ctx;
    struct graphics_data *data = userdata;
    (void)data;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return 1;
        }
    }

    return 0;
}
