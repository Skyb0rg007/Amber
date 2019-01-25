#include <Amber/ECS/ECS.h>
#include <Amber/util/common.h>
#include <Amber/util/ring.h>
#include <stdlib.h>

struct MessageQueue {
    struct AB_ring inbox;
    size_t elem_size;
    void *elems;
};

struct World;

struct SystemCtx {
    struct World *world;
};

struct ECS_SystemState {
    size_t num_systems;
    void **userdata;
    struct MessageQueue *inboxes;
};

struct ECS_SystemState *
ECS_initialize_systems(const struct ECS_System *systems, size_t count)
{
    struct ECS_SystemState *state = malloc(sizeof *state);
    ASSERT(state != NULL);
    state->num_systems = count;
    state->userdata = calloc(count, sizeof(void *));
    ASSERT(state->userdata != NULL);
    state->inboxes = calloc(count, sizeof(struct MessageQueue));

    int status;
    for (int i = 0; i < count; i++) {
        status = systems[i].init(NULL, &state->userdata[i]);
        if (status != 0)
            abort();
        
        /* message system */
        if (systems[i].message_size > 0) {
            struct MessageQueue *inbox = &state->inboxes[i];
            inbox->elem_size = systems[i].message_size;
            inbox->elems = calloc(MESSAGEQUEUE_MAX, inbox->elem_size);
            ASSERT(inbox->elems != NULL);
            AB_ring_init(&inbox->inbox, MESSAGEQUEUE_MAX);
        }
    }

    return state;
}

int ECS_run_systems(const struct ECS_System *systems, struct ECS_SystemState *state)
{
    size_t num_systems = state->num_systems;
    struct SystemCtx ctx = {
        .world = NULL
    };

    int status;
    for (int i = 0; i < num_systems; i++) {
        status = systems[i].run(&ctx, state->userdata[i]);
        if (status != 0)
            return 1;
    }

    return 0;
}

int ECS_cleanup_systems(const struct ECS_System *systems, struct ECS_SystemState *state)
{
    size_t num_systems = state->num_systems;
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
