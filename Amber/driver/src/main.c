#include <Amber/graphics/graphics.h>
#include <Amber/ECS/world.h>
#include <SDL.h>
#include <assert.h>
#include <time.h>
#ifdef __EMSCRIPTEN__
# include <emscripten.h>
#endif

static void mainloop(void *data);

int main(int argc, char *argv[])
{
    struct AB_ECS_world world;
    AB_ECS_init_world(&world);

    AB_ECS_add_system(&world, &graphics_system);

    int now = time(NULL);
    int then = now + 10;
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(mainloop, &world, 0, true);
#else
    while (time(NULL) < then) {
        AB_ECS_run_world(&world);
    }
#endif

    AB_ECS_destroy_world(&world);

    return 0;
}

static void mainloop(void *data)
{
    struct AB_ECS_world *world = data;
    AB_ECS_run_world(world);
}
