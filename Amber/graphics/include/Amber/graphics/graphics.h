#ifndef AMBER_GRAPHICS_GRAPHICS_H
#define AMBER_GRAPHICS_GRAPHICS_H

#include <Amber/ECS/ECS.h>
#include <stddef.h>

int graphics_system_init(SystemCtx *_ctx, void **userdata);
int graphics_system_cleanup(SystemCtx *_ctx, void *userdata);
int graphics_system_run(SystemCtx *_ctx, void *userdata);

struct graphics_msg {
    int a;
};

static const struct ECS_System graphics_system = {
    .init            = graphics_system_init,
    .run             = graphics_system_run,
    .cleanup         = graphics_system_cleanup,
    .message_size    = sizeof (struct graphics_msg)
    /* .message_handler = NULL, */
    /* .system_mask     = 0, */
    /* .thread_order    = ECS_THREAD_ORDER_MAIN_THREAD, */
    /* .name            = "graphics" */
};

#endif
