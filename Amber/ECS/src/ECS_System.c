#include <Amber/ECS/ECS.h>
#include <Amber/util/common.h>
#include <stdlib.h>

/* struct MessageQueue;
struct World;

struct SystemCtx {
    struct MessageQueue *inbox, *outbox;
    struct World *world;
}; */

struct ECS_SystemState {
    int num_systems;
    void **userdata;
    /* struct MessageQueue *inboxes; */
};

struct ECS_SystemState *
ECS_initialize_systems(const struct ECS_System *systems, int count)
{
    struct ECS_SystemState *state = malloc(sizeof *state);
    ASSERT(state != NULL);
    state->num_systems = count;
    state->userdata = calloc(count, sizeof(void *));
    ASSERT(state->userdata != NULL);

    int status;
    for (int i = 0; i < count; i++) {
        status = systems[i].init(NULL, &state->userdata[i]);
        if (status != 0)
            abort();
    }

    return state;
}

int ECS_run_systems(const struct ECS_System *systems, struct ECS_SystemState *state)
{
    int num_systems = state->num_systems;
    int status;
    for (int i = 0; i < num_systems; i++) {
        status = systems[i].run(NULL, state->userdata[i]);
        if (status != 0)
            return 1;
    }

    return 0;
}

int ECS_cleanup_systems(const struct ECS_System *systems, struct ECS_SystemState *state)
{
    int num_systems = state->num_systems;
    int status;
    for (int i = 0; i < num_systems; i++) {
        status = systems[i].cleanup(NULL, state->userdata[i]);
        if (status != 0)
            abort();
    }

    free(state->userdata);
    free(state);

    return 0;
}
