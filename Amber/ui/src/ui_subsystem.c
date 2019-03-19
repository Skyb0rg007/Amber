#include <Amber/ui/ui.h>
#include <nuklear-config.h>

struct ui_data {
    struct nk_context ctx;
};

int ui_subsystem_init(struct AB_ECS_system *self)
{
    
    return 0;
}

int ui_subsystem_run(struct AB_ECS_world *world, struct AB_ECS_system *self)
{
    return 0;
}

int ui_subsystem_cleanup(struct AB_ECS_world *world, struct AB_ECS_system *self)
{
    return 0;
}

int ui_subsystem_message_handler(struct AB_ECS_world *world, void *message, struct AB_ECS_system *self)
{
    return 0;
}

