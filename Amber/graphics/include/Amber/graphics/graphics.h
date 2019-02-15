#ifndef AMBER_GRAPHICS_GRAPHICS_H
#define AMBER_GRAPHICS_GRAPHICS_H

#include <Amber/ECS/world.h>
#include <stddef.h>

#define AB_GRAPHICS_SYSTEM_ID 0

int graphics_system_init(struct AB_ECS_system *self);
int graphics_system_run(struct AB_ECS_world *world, struct AB_ECS_system *self);
int graphics_system_cleanup(struct AB_ECS_world *world, struct AB_ECS_system *self);
int graphics_system_message_handler(struct AB_ECS_world *world, 
        void *message, struct AB_ECS_system *self);

struct graphics_msg {
    int a;
};

static const struct AB_ECS_system_info graphics_system = {
    .init            = graphics_system_init,
    .run             = graphics_system_run,
    .cleanup         = graphics_system_cleanup,
    .message_handler = graphics_system_message_handler,
    .message_size    = sizeof (struct graphics_msg),
};

#endif
