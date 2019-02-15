#ifndef AMBER_UI_UI_H
#define AMBER_UI_UI_H

#include <Amber/ECS/world.h>

int ui_subsystem_init(struct AB_ECS_system *self);
int ui_subsystem_run(struct AB_ECS_world *world, struct AB_ECS_system *self);
int ui_subsystem_cleanup(struct AB_ECS_world *world, struct AB_ECS_system *self);
int ui_subsystem_message_handler(struct AB_ECS_world *world, void *message, struct AB_ECS_system *self);

struct ui_message {
    int a;
};

static const struct AB_ECS_system_info ui_subsystem = {
    .init = ui_subsystem_init,
    .run  = ui_subsystem_run,
    .cleanup = ui_subsystem_cleanup,
    .message_handler = ui_subsystem_message_handler,
    .message_size = sizeof (struct ui_message)
};

#endif /* AMBER_UI_UI_H */
