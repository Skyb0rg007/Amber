#ifndef AMBER_ECS_ECS_H
#define AMBER_ECS_ECS_H

#include <stddef.h>

#define MESSAGEQUEUE_MAX 128

/* struct ECS_ComponentConfig { */
    /* size_t size; */
    /* int mask; */
    /* int (*on_create)(void *component, void *userdata); */
    /* int (*on_destroy)(void *component, void *userdata); */
    /* void *userdata; */
/* }; */

struct ECS_ComponentData {
    size_t component_size;
};

typedef struct SystemCtx SystemCtx;

enum ECS_System_thread_order {
    ECS_THREAD_ORDER_MAIN_THREAD,
    ECS_THREAD_ORDER_NONE
};

struct ECS_System {
    int (*init)(SystemCtx *ctx, void **userdata);
    int (*run)(SystemCtx *ctx, void *userdata);
    int (*cleanup)(SystemCtx *ctx, void *userdata);
    /* int (*message_handler)(int type, ...); */
    /* int system_mask; [> Your message id <] */
    /* enum ECS_System_thread_order thread_order; */
    /* const char *name; */
    size_t message_size;
};

struct ECS_SystemState;

struct ECS_SystemState *ECS_initialize_systems(
        const struct ECS_System *systems, size_t system_count,
        const struct ECS_ComponentData *components, size_t component_count);
int ECS_run_systems(const struct ECS_System *systems, struct ECS_SystemState *state);
int ECS_cleanup_systems(const struct ECS_System *systems, struct ECS_SystemState *state);

void *ECS_component_begin(SystemCtx *ctx, unsigned comp_id);
void *ECS_component_next(SystemCtx *ctx, unsigned comp_id, void *current);
int ECS_component_end(SystemCtx *ctx, unsigned comp_id, const void *current);

#define ECS_component_foreach(comp, ctx, comp_id) \
    for (comp = ECS_component_begin(ctx, comp_id); \
            !ECS_component_end(ctx, comp_id, comp); \
            comp = ECS_component_next(ctx, comp_id, comp))

int ECS_msg_queue_pop(SystemCtx *ctx, void *elem);
int ECS_msg_queue_push(SystemCtx *ctx, void *elem);

unsigned ECS_entity_new(SystemCtx *ctx, unsigned comp_id);
void *ECS_entity_get(SystemCtx *ctx, unsigned entity, unsigned comp_id);
void ECS_entity_delete(SystemCtx *ctx, unsigned entity);


#endif
