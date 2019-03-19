
#include <Amber/ECS/world.h>
#include <Amber/util/common.h>
#include <Amber/util/vector.h>
#include <Amber/util/ring.h>
#include <Amber/util/hashtable.h>

void AB_ECS_init_world(struct AB_ECS_world *world)
{
    AB_VEC_INIT(&world->default_systems);
    AB_VEC_INIT(&world->component_managers);
    AB_hash_init(world->runtime_systems);
}

/****************************************************************************/

static void destroy_runtime_systems(struct AB_ECS_world *world)
{
    unsigned bkt;
    struct AB_ECS_runtime_system *sys;
    struct AB_hlist_node *tmp;

    AB_hash_foreach_safe(world->runtime_systems, bkt, tmp, sys,
            struct AB_ECS_runtime_system, node) {
        sys->cleanup(world, sys);
        AB_hash_del(&sys->node);
    }
}

static void destroy_default_systems(struct AB_ECS_world *world)
{
    unsigned i, n;

    n = AB_VEC_NUM(&world->default_systems);
    for (i = 0; i < n; i++) {
        struct AB_ECS_system *sys = &AB_VEC_AT(&world->default_systems, i);
        sys->cleanup(world, sys);
        free(sys->inbox.elems);
    }
    AB_VEC_DESTROY(&world->default_systems);
}

static void destroy_component_managers(struct AB_ECS_world *world)
{
    int i, n;

    n = AB_VEC_NUM(&world->component_managers);
    for (i = 0; i < n; i++) {
        struct AB_ECS_component_manager *comp =
            &AB_VEC_AT(&world->component_managers, i);

        char *end = (char *)comp->elems + comp->elem_size * comp->num_elems;
        char *elem;
        for (elem = comp->elems; elem != end; elem += comp->elem_size) {
            comp->ondelete(elem, comp);
        }
        comp->destroy(comp);
    }
    AB_VEC_DESTROY(&world->component_managers);
}

void AB_ECS_destroy_world(struct AB_ECS_world *world)
{
    destroy_runtime_systems(world);
    destroy_default_systems(world);
    destroy_component_managers(world);
}

/****************************************************************************/

static AB_errno_t run_default_systems(struct AB_ECS_world *world)
{
    unsigned i, n;

    n = AB_VEC_NUM(&world->default_systems);
    for (i = 0; i < n; i++) {
        struct AB_ECS_system *sys = &AB_VEC_AT(&world->default_systems, i);
        AB_errno_t err = sys->run(world, sys);
        if (err)
            return err;
    }
    return AB_OK;
}

static AB_errno_t run_runtime_systems(struct AB_ECS_world *world)
{
    unsigned bkt;
    struct AB_ECS_runtime_system *sys;
    struct AB_hlist_node *tmp;

    AB_hash_foreach_safe(world->runtime_systems, bkt, tmp, sys,
            struct AB_ECS_runtime_system, node) {
        AB_errno_t err = sys->run(world, sys);
        if (err)
            return err;
    }
    return AB_OK;
}

AB_errno_t AB_ECS_run_world(struct AB_ECS_world *world)
{
    AB_errno_t err;

    err = run_default_systems(world);
    if (err)
        return err;

    err = run_runtime_systems(world);
    if (err)
        return err;

    return AB_OK;
}

/****************************************************************************/

AB_errno_t AB_ECS_add_system(struct AB_ECS_world *world,
        const struct AB_ECS_system_info *info)
{
    static const unsigned RING_SIZE = 64;
    struct AB_ECS_system *sys = NULL;
    AB_errno_t err;

    AB_VEC_PUSHP(&world->default_systems, &sys,
            AB_VEC_ONFAIL(err = AB_ENOMEM; goto vec_error));
    sys->run             = info->run;
    sys->cleanup         = info->cleanup;
    sys->message_handler = info->message_handler;
    sys->userdata        = NULL;

    sys->inbox.elem_size = info->message_size;
    sys->inbox.elems     = calloc(info->message_size, RING_SIZE);
    AB_ASSERT(sys->inbox.elems != NULL);
    AB_ring_init(&sys->inbox.ring, RING_SIZE);

    err = info->init(sys);
    if (err)
        goto init_error;

    return AB_OK;

init_error:
    free(sys->inbox.elems);
vec_error:
    return err;
}

/****************************************************************************/

AB_errno_t AB_ECS_add_component(struct AB_ECS_world *world, 
        const struct AB_ECS_component_manager_info *info)
{
    static const unsigned INITIAL_ELEM_SIZE = 64;
    struct AB_ECS_component_manager *comp;
    AB_errno_t err;

    AB_VEC_PUSHP(&world->component_managers, &comp,
            AB_VEC_ONFAIL(err = AB_ENOMEM; goto vec_error));
    comp->ondelete = info->ondelete;
    comp->oncreate = info->oncreate;
    comp->destroy  = info->destroy;
    comp->userdata = NULL;
    
    comp->max_elems = INITIAL_ELEM_SIZE;
    comp->num_elems = 0;
    comp->elem_size = info->elem_size;
    comp->elems = calloc(comp->elem_size, comp->num_elems);
    if (comp->elems == NULL) {
        err = AB_ENOMEM;
        goto vec_error;
    }

    err = info->init(comp);
    if (err)
        goto init_error;

    return AB_OK;

init_error:
    free(comp->elems);
vec_error:
    return err;
}
