#ifndef AMBER_UI_UI_H
#define AMBER_UI_UI_H

#include <Amber/ECS/ECS.h>

int ui_subsystem_init(SystemCtx *ctx, void **userdata);
int ui_subsystem_run(SystemCtx *ctx, void *userdata);
int ui_subsystem_cleanup(SystemCtx *ctx, void *userdata);

static const struct ECS_System ui_subsystem = {
    .init = ui_subsystem_init,
    .run  = ui_subsystem_run,
    .cleanup = ui_subsystem_cleanup,
    /* .message_handler = NULL, */
    /* .system_mask     = 0, */
    /* .thread_order    = ECS_THREAD_ORDER_MAIN_THREAD, */
    /* .name = "ui" */
};

#endif /* AMBER_UI_UI_H */
