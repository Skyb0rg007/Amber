#ifndef AMBER_ECS_WORLD_H
#define AMBER_ECS_WORLD_H

#include <Amber/util/common.h>
#include <Amber/util/vector.h>
#include <Amber/util/ring.h>
#include <Amber/util/linux-hashtable.h>

struct AB_ECS_world;

int AB_ECS_init_world(struct AB_ECS_world *world);
int AB_ECS_run_world(struct AB_ECS_world *world);
int AB_ECS_destroy_world(struct AB_ECS_world *world);

struct AB_ECS_system;
struct AB_ECS_system_info {
    int (*init)(struct AB_ECS_system *self);
    int (*run)(struct AB_ECS_world *world, struct AB_ECS_system *self);
    int (*cleanup)(struct AB_ECS_world *world, struct AB_ECS_system *self);
    int (*message_handler)(struct AB_ECS_world *world, void *message, struct AB_ECS_system *self);
    size_t message_size;
};

/* Index = order inserted */
int AB_ECS_add_system(struct AB_ECS_world *world,
        const struct AB_ECS_system_info *info);

struct AB_ECS_component_manager;
struct AB_ECS_component_manager_info {
    int (*ondelete)(void *elem, struct AB_ECS_component_manager *self);
    int (*oncreate)(void *elem, struct AB_ECS_component_manager *self);
    int (*init)(struct AB_ECS_component_manager *self);
    int (*destroy)(struct AB_ECS_component_manager *self);
    size_t elem_size;
};

int AB_ECS_add_component(struct AB_ECS_world *world, 
        const struct AB_ECS_component_manager_info *info);


/*********************************************************
 *
 * Implementation
 *
 ********************************************************/

struct AB_ECS_system_inbox {
    struct AB_ring ring;
    void *elems;
    size_t elem_size;
};

struct AB_ECS_system {
    int (*run)(struct AB_ECS_world *world, struct AB_ECS_system *self);
    int (*cleanup)(struct AB_ECS_world *world, struct AB_ECS_system *self);
    int (*message_handler)(struct AB_ECS_world *world, void *message, struct AB_ECS_system *self);
    void *userdata;
    struct AB_ECS_system_inbox inbox; /* Message Queue */
};

struct AB_ECS_runtime_system {
    struct AB_hlist_node node;
    int (*run)(struct AB_ECS_world *world, struct AB_ECS_runtime_system *self);
    int (*cleanup)(struct AB_ECS_world *world, struct AB_ECS_runtime_system *self);
    int (*message_handler)(struct AB_ECS_world *world, void *message, struct AB_ECS_runtime_system *self);
    void *userdata;
    struct AB_ECS_system_inbox inbox;
};

struct AB_ECS_component_manager {
    int (*ondelete)(void *elem, struct AB_ECS_component_manager *self);
    int (*oncreate)(void *elem, struct AB_ECS_component_manager *self);
    int (*destroy)(struct AB_ECS_component_manager *self);
    void *userdata;
    void *elems;
    size_t elem_size;
    size_t num_elems, max_elems;
};

struct AB_ECS_world {
    AB_vec(struct AB_ECS_system) default_systems; /* Identified by enum */
    AB_vec(struct AB_ECS_component_manager) component_managers;
    AB_HASHTABLE_DECLARE(runtime_systems, 4); /* Identified by hash */
};

#endif /* AMBER_ECS_WORLD_H */
