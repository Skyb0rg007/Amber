#include <Amber/graphics/graphics.h>
#include <SDL.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    struct ECS_SystemState *state;
    state = ECS_initialize_systems(&graphics_system, 1);
    assert(state != NULL);

    while (!ECS_run_systems(&graphics_system, state))
        ;

    ECS_cleanup_systems(&graphics_system, state);

    return 0;
}
