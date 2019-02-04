#include <Amber/ECS/ECS.h>
#include <Amber/util/common.h>
/* #include <Amber/util/vector.h> */
/* #include <Amber/util/ring.h> */
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>

/* struct MessageQueue {
    struct AB_ring inbox;
    size_t elem_size;
    void *elems;
}; */

#define ENTITY_COUNT 128

struct World {
    size_t num_entities;
    size_t max_entities;
    struct Component **components;
    unsigned *component_mask;
};

struct SystemCtx {
    struct World *world;
};

struct Component {
    size_t size;
    char data[];
};

struct ECS_SystemState {
    size_t num_systems;
    void **userdata;
    struct World world;
    /* struct MessageQueue *inboxes; */
};

struct ECS_SystemState *
ECS_initialize_systems(const struct ECS_System *systems, size_t system_count,
        const struct ECS_ComponentData *components, size_t component_count)
{
    struct ECS_SystemState *state = malloc(sizeof *state);
    AB_ASSERT(state != NULL);
    state->num_systems = system_count;
    state->userdata = calloc(system_count, sizeof(void *));
    AB_ASSERT(state->userdata != NULL);
    /* state->inboxes = calloc(count, sizeof(struct MessageQueue)); */

    /* Components / World */
    state->world.num_entities = 0;
    state->world.max_entities = ENTITY_COUNT;
    state->world.components = calloc(component_count, sizeof(void *));
    AB_ASSERT(state->world.components != NULL);
    for (int i = 0; i < component_count; i++) {
        state->world.components[i] = calloc(1,
                sizeof(struct Component) +
                ENTITY_COUNT * components[i].component_size);
        AB_ASSERT(state->world.components[i] != NULL);
        state->world.components[i]->size = components[i].component_size;
    }


    struct SystemCtx ctx;
    ctx.world = &state->world;

    int status;
    for (int i = 0; i < system_count; i++) {
        status = systems[i].init(&ctx, &state->userdata[i]);
        if (status != 0)
            abort();
        
        /* message system */
        /* if (systems[i].message_size > 0) {
            struct MessageQueue *inbox = &state->inboxes[i];
            inbox->elem_size = systems[i].message_size;
            inbox->elems = calloc(MESSAGEQUEUE_MAX, inbox->elem_size);
            AB_ASSERT(inbox->elems != NULL);
            AB_ring_init(&inbox->inbox, MESSAGEQUEUE_MAX);
        } */
    }

    return state;
}

int ECS_run_systems(const struct ECS_System *systems, struct ECS_SystemState *state)
{
    size_t num_systems = state->num_systems;
    struct SystemCtx ctx;
    ctx.world = &state->world;

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

void *ECS_component_begin(SystemCtx *ctx, unsigned comp_id)
{
    return ctx->world->components[comp_id]->data;
}

void *ECS_component_next(SystemCtx *ctx, unsigned comp_id, void *current)
{
    struct Component *comp = ctx->world->components[comp_id];
    size_t current_entity = (size_t)((char *)current - (char *)comp->data) / comp->size;
    if (current_entity >= ctx->world->num_entities)
        return NULL;

    size_t diff = 1;
    while (!(ctx->world->component_mask[current_entity] & (1u << comp_id))) {
        diff++, current_entity++;
    }

    return (char *)current + comp->size * diff;
}

int ECS_component_end(SystemCtx *ctx, unsigned comp_id, const void *current)
{
    struct Component *comp = ctx->world->components[comp_id];
    const char *comp_data = comp->data;
    size_t comp_size = comp->size;
    size_t entity_count = ctx->world->num_entities;

    return (const char *)current + comp_size >=
        comp_data + comp_size * entity_count;
}

unsigned ECS_entity_new(SystemCtx *ctx, unsigned comp_id)
{
    unsigned comp_mask = 1u << comp_id;

    unsigned entity = (unsigned)ctx->world->num_entities++;
    AB_ASSERT(entity < ctx->world->max_entities);

    ctx->world->component_mask[entity] = comp_mask;
    return entity;
}

void ECS_entity_delete(SystemCtx *ctx, unsigned entity)
{
    /* TODO */
}

void *ECS_entity_get(SystemCtx *ctx, unsigned entity, unsigned comp_id)
{
    struct Component *comp = ctx->world->components[comp_id];

    return comp->data + entity * comp->size;
}
