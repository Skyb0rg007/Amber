
#include <Amber/ECS/world.h>
#include <Amber/util/common.h>
#include <Amber/util/vector.h>
#include <Amber/util/ring.h>
#include <Amber/util/hashtable.h>

int AB_ECS_init_world(struct AB_ECS_world *world)
{
    AB_vec_init(&world->default_systems);
    AB_vec_init(&world->component_managers);
    AB_hash_init(world->runtime_systems);
    return 0;
}

int AB_ECS_destroy_world(struct AB_ECS_world *world)
{
    unsigned bkt;
    struct AB_ECS_runtime_system *sys;
    struct AB_hlist_node *tmp;
    AB_hash_foreach_safe(world->runtime_systems, bkt, tmp, sys,
            struct AB_ECS_runtime_system, node) {
        sys->cleanup(world, sys);
        AB_hash_del(&sys->node);
    }

    int i, n;
    n = AB_vec_size(&world->default_systems);
    for (i = 0; i < n; i++) {
        struct AB_ECS_system *sys = &AB_vec_at(&world->default_systems, i);
        sys->cleanup(world, sys);
        free(sys->inbox.elems);
    }
    AB_vec_destroy(&world->default_systems);

    n = AB_vec_size(&world->component_managers);
    for (i = 0; i < n; i++) {
        struct AB_ECS_component_manager *comp =
            &AB_vec_at(&world->component_managers, i);

        char *end = (char *)comp->elems + comp->elem_size * comp->num_elems;
        char *elem;
        for (elem = comp->elems; elem != end; elem += comp->elem_size) {
            comp->ondelete(elem, comp);
        }
        comp->destroy(comp);
    }
    AB_vec_destroy(&world->component_managers);

    return 0;
}

int AB_ECS_run_world(struct AB_ECS_world *world)
{
    int ret = 0;
    int i, n;
    n = AB_vec_size(&world->default_systems);
    for (i = 0; i < n; i++) {
        struct AB_ECS_system *sys = &AB_vec_at(&world->default_systems, i);
        ret = sys->run(world, sys);
        if (ret)
            goto end;
    }

    
    unsigned bkt;
    struct AB_ECS_runtime_system *sys;
    struct AB_hlist_node *tmp;
    AB_hash_foreach_safe(world->runtime_systems, bkt, tmp, sys,
            struct AB_ECS_runtime_system, node) {
        ret = sys->run(world, sys);
        if (ret) {
            goto end;
        }
    }
end:
    return ret;
}

int AB_ECS_add_system(struct AB_ECS_world *world,
        const struct AB_ECS_system_info *info)
{
    struct AB_ECS_system *sys = AB_vec_pushp(&world->default_systems);
    sys->run             = info->run;
    sys->cleanup         = info->cleanup;
    sys->message_handler = info->message_handler;
    sys->userdata        = NULL;

    static const unsigned RING_SIZE = 64;
    sys->inbox.elem_size = info->message_size;
    sys->inbox.elems     = calloc(info->message_size, RING_SIZE);
    AB_ASSERT(sys->inbox.elems != NULL);
    AB_ring_init(&sys->inbox.ring, RING_SIZE);

    info->init(sys);

    return 0;
}

int AB_ECS_add_component(struct AB_ECS_world *world, 
        const struct AB_ECS_component_manager_info *info)
{
    struct AB_ECS_component_manager *comp = AB_vec_pushp(&world->component_managers);
    comp->ondelete = info->ondelete;
    comp->oncreate = info->oncreate;
    comp->destroy = info->destroy;
    comp->userdata = NULL;
    
    static const unsigned INITIAL_ELEM_SIZE = 64;
    comp->max_elems = INITIAL_ELEM_SIZE;
    comp->num_elems = 0;
    comp->elem_size = info->elem_size;
    comp->elems = calloc(comp->elem_size, comp->num_elems);

    info->init(comp);

    return 0;
}
