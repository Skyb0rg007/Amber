#include <Amber/ECS/world.h>
#include <stdio.h>
#include <limits.h>

#if 0
struct Position {
    int x, y;
};

struct Name {
    const char *name;
};

static const struct ECS_ComponentData comp_data[] = { 
    { sizeof(struct Position) },
    { sizeof(struct Name) }
};

enum {
    POSITION_ID = 0,
    NAME_ID = 1
};

int print_init(SystemCtx *ctx, void **userdata) {
    for (int i = 0; i < 20; i++) {
        unsigned entity = ECS_entity_new(ctx, NAME_ID);
        struct Name *name = ECS_entity_get(ctx, entity, NAME_ID);
        name->name = "My Name!";
    }
    return 0;
}
int print_cleanup(SystemCtx *ctx, void *userdata) { puts("Cleaning up"); return 0; }
int print_run(SystemCtx *ctx, void *userdata) {
    struct Name *name;

    ECS_component_foreach(name, ctx, NAME_ID) {
        printf("Name = %s\n", name->name);
    }
}

struct ECS_System print_system = {
    .init = print_init,
    .cleanup = print_cleanup,
    .run = print_run
};

#endif

int main(void)
{
    /* struct ECS_SystemState *s = ECS_initialize_systems(&print_system, 1, comp_data, 2); */
    /* ECS_run_systems(&print_system, s); */
    /* ECS_run_systems(&print_system, s); */
    /* ECS_run_systems(&print_system, s); */
    /* ECS_cleanup_systems(&print_system, s); */
    puts("TODO");
    return 0;
}
