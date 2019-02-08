#include <Amber/util/common.h>
#include <Amber/ECS/ECS.h>
#include <Amber/graphics/graphics.h>
#include <SDL.h>
#include <stdlib.h>
#include <time.h>

#define AMBER_ORG "sosscorp"
#define AMBER_APP "Amber engine"

struct graphics_data {
    SDL_Window *win;
    SDL_GLContext gl_ctx;
    SDL_Renderer *renderer;
    char *base_path;
    char *pref_path;
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

    data->renderer = SDL_CreateRenderer(data->win, -1, SDL_RENDERER_ACCELERATED);

    data->base_path = SDL_GetBasePath();
    data->pref_path = SDL_GetPrefPath(AMBER_ORG, AMBER_APP);

    *userdata = data;
    return 0;
}

int graphics_system_cleanup(SystemCtx *_ctx, void *userdata)
{
    (void)_ctx;
    struct graphics_data *data = userdata;

    SDL_DestroyRenderer(data->renderer);
    SDL_DestroyWindow(data->win);
    SDL_Quit();

    return 0;
}

int graphics_system_run(SystemCtx *ctx, void *userdata)
{
    struct graphics_data *data = userdata;
    (void)data;

    struct graphics_msg msg;
    if (ECS_msg_queue_pop(ctx, &msg)) {
        fprintf(stderr, "Got message: %d\n", msg.a);
    }

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT
                || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) {
            return 1;
        }

        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_o) {
            char *cmd;
            AB_ASPRINTF(&cmd, "%s \"%s\" 1>/dev/null 2>&1",
                    "xdg-open", "http://google.com");
            system(cmd);
            free(cmd);
        }

        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_i) {
            msg.a = time(NULL);
            ECS_msg_queue_push(ctx, &msg);
        }
    }

    SDL_SetRenderDrawColor(data->renderer, 255, 0, 255, 255);
    SDL_RenderClear(data->renderer);

    SDL_Rect rect = {
        .x = 10, .y = 10, .w = 32, .h = 32
    };
    SDL_SetRenderDrawColor(data->renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(data->renderer, &rect);

    SDL_RenderPresent(data->renderer);

    return 0;
}
