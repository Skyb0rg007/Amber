#ifndef AMBER_ECS_ECS_H
#define AMBER_ECS_ECS_H

#include <stddef.h>

/* struct ECS_ComponentConfig { */
    /* size_t size; */
    /* int mask; */
    /* int (*on_create)(void *component, void *userdata); */
    /* int (*on_destroy)(void *component, void *userdata); */
    /* void *userdata; */
/* }; */


typedef struct SystemCtx SystemCtx;

enum ECS_System_thread_order {
    ECS_THREAD_ORDER_MAIN_THREAD,
    ECS_THREAD_ORDER_NONE
};

struct ECS_System {
    int (*init)(SystemCtx *ctx, void **userdata);
    int (*run)(SystemCtx *ctx, void *userdata);
    int (*cleanup)(SystemCtx *ctx, void *userdata);
    int (*message_handler)(int type, ...);
    int system_mask; /* Your message id */
    enum ECS_System_thread_order thread_order;
    const char *name;
};

struct ECS_SystemState;

struct ECS_SystemState *ECS_initialize_systems(const struct ECS_System *systems, int count);
int ECS_run_systems(const struct ECS_System *systems, struct ECS_SystemState *state);
int ECS_cleanup_systems(const struct ECS_System *systems, struct ECS_SystemState *state);

/* void *ECS_entity_get(SystemCtx *ctx, int comp_id); */

/* #define ECS_entity_foreach(entity, ctx) \ */
    /* for (entity = 0; ;) */

#endif
