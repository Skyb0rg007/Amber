#include <Amber/util/common.h>
#include <Amber/ECS/world.h>
#include <Amber/graphics/graphics.h>
#include "graphics_system.h"

int graphics_system_message_handler(struct AB_ECS_world *world, 
        void *message, struct AB_ECS_system *self)
{
    (void)world; (void)self;
    struct graphics_msg *msg = message;
    AB_QUICK_LOG("Got sync message { .a = %d }", msg->a);
    return 0;
}

