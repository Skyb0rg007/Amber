#include <Amber/graphics/graphics.h>
#include <Amber/ECS/world.h>
#include <SDL.h>
#include <assert.h>
#include <time.h>

int main(int argc, char *argv[])
{
    struct AB_ECS_world world;
    AB_ECS_init_world(&world);

    AB_ECS_add_system(&world, &graphics_system);

    int now = time(NULL);
    int then = now + 10;
    while (!AB_ECS_run_world(&world) && time(NULL) != then)
        ;

    AB_ECS_destroy_world(&world);

    return 0;
}
