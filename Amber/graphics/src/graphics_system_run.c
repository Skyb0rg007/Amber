#include <Amber/ECS/inbox.h>
#include <Amber/graphics/graphics.h>
#include "graphics_system.h"

#include <SDL.h>
#include <glad/glad.h>

#include <time.h>

static AB_errno_t handle_messages(struct AB_ECS_world *world,
        struct graphics_data *data)
{
    struct graphics_msg msg;
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT
                || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) {
            return AB_EXIT;
        }

        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_o) {
            char *cmd;
            AB_ASPRINTF(&cmd, "%s \"%s\" 1>/dev/null 2>&1",
                    "xdg-open", "http://google.com");
            system(cmd);
            free(cmd);
            AB_QUICK_LOG("Finished opening up google!");
        }

        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_a) {
            msg.a = time(NULL);
            AB_ECS_send_message_async(world, AB_GRAPHICS_SYSTEM_ID, &msg, sizeof msg);
        }

        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s) {
            msg.a = time(NULL);
            AB_ECS_send_message_sync(world, AB_GRAPHICS_SYSTEM_ID, &msg, sizeof msg);
        }

        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            int w, h;
            SDL_GetWindowSize(data->win, &w, &h);
            glViewport(0, 0, w, h);
        }
    }
}

AB_errno_t graphics_system_run(struct AB_ECS_world *world, struct AB_ECS_system *self)
{
    AB_errno_t err;
    struct graphics_data *data = self->userdata;
    struct graphics_msg msg;

    AB_ECS_foreach_message(&msg, self) {
        AB_QUICK_LOG("Got async message { .a = %d }", msg.a);
    }

    err = handle_messages(world, data);
    if (err)
        return err;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SDL_GL_SwapWindow(data->win);

    return AB_OK;
}
