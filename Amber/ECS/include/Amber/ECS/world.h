/**
 * @file world.h
 * @brief Implementation of an ECS
 */
#ifndef AMBER_ECS_WORLD_H
#define AMBER_ECS_WORLD_H

#include <Amber/util/common.h>
#include <Amber/util/vector.h>
#include <Amber/util/ring.h>
#include <Amber/util/hashtable.h>

/** @brief Structure representing the world state */
struct AB_ECS_world;

/** @brief Initialize a worldstate */
int AB_ECS_init_world(struct AB_ECS_world *world);
/** @brief Run a worldstate for one tick */
int AB_ECS_run_world(struct AB_ECS_world *world);
/** @brief Free a worldstate */
int AB_ECS_destroy_world(struct AB_ECS_world *world);

/** @brief Representation of an ECS system */
struct AB_ECS_system;
/** @brief Information needed to add an ECS system */
struct AB_ECS_system_info {
    int (*init)(struct AB_ECS_system *self); 
    /**< Called when the system is initialized */
    int (*run)(struct AB_ECS_world *world, struct AB_ECS_system *self); 
    /**< Called each tick */
    int (*cleanup)(struct AB_ECS_world *world, struct AB_ECS_system *self); 
    /**< Called when the system is closed */
    int (*message_handler)(struct AB_ECS_world *world, void *message, struct AB_ECS_system *self); 
    /**< Called when the system is sent an synchronous message */
    size_t message_size; /**< The size of a message */
};

/* Index = order inserted */
/** @brief Add a default system to the world */
int AB_ECS_add_system(struct AB_ECS_world *world,
        const struct AB_ECS_system_info *info);

/** @brief Representation of an ECS component */
struct AB_ECS_component_manager;
/** Information needed to add an ECS component */
struct AB_ECS_component_manager_info {
    int (*ondelete)(void *elem, struct AB_ECS_component_manager *self); 
    /**< Called when a component is removed */
    int (*oncreate)(void *elem, struct AB_ECS_component_manager *self); 
    /**< Called when a component is added */
    int (*init)(struct AB_ECS_component_manager *self); 
    /**< Called on startup */
    int (*destroy)(struct AB_ECS_component_manager *self); 
    /**< Called on cleanup */
    size_t elem_size; /**< The size of an element */
};

/** @brief Add a component manager to the world */
int AB_ECS_add_component(struct AB_ECS_world *world, 
        const struct AB_ECS_component_manager_info *info);


/*********************************************************
 *
 * Implementation
 *
 ********************************************************/

/** @brief Representation of a concurrent message inbox */
struct AB_ECS_system_inbox {
    struct AB_ring ring; /**< The ring buffer manager */
    void *elems; /**< The ring buffer itself */
    size_t elem_size; /**< The size of elements in the ring buffer */
};

struct AB_ECS_system {
    int (*run)(struct AB_ECS_world *world, struct AB_ECS_system *self); 
    /**< Called on each tick */
    int (*cleanup)(struct AB_ECS_world *world, struct AB_ECS_system *self);
    /**< Called on system cleanup */
    int (*message_handler)(struct AB_ECS_world *world, void *message, struct AB_ECS_system *self);
    /**< Called to handle sychronous messages */
    void *userdata;
    /**< Arbitrary user information */
    struct AB_ECS_system_inbox inbox; /**< System message queue */
};

struct AB_ECS_runtime_system {
    struct AB_hlist_node node; /**< Enable hashing */
    int (*run)(struct AB_ECS_world *world, struct AB_ECS_runtime_system *self);
    /**< Called on each tick */
    int (*cleanup)(struct AB_ECS_world *world, struct AB_ECS_runtime_system *self);
    /**< Called on system cleanup */
    int (*message_handler)(struct AB_ECS_world *world, void *message, struct AB_ECS_runtime_system *self);
    /**< Called to handle synchronous messages */
    void *userdata;
    /**< Arbitrary user information */
    struct AB_ECS_system_inbox inbox; /**< System message queue */
};

struct AB_ECS_component_manager {
    int (*ondelete)(void *elem, struct AB_ECS_component_manager *self);
    /**< Called on component deletion */
    int (*oncreate)(void *elem, struct AB_ECS_component_manager *self);
    /**< Called on component creation */
    int (*destroy)(struct AB_ECS_component_manager *self);
    /**< Called on component_manager cleanup */
    void *userdata;
    /**< Arbitrary user information */
    void *elems;
    /**< The component elements */
    size_t elem_size;
    /**< The size of the elements */
    size_t num_elems,  /**< The number of elements in the component_manager */
           max_elems; /**< The number of allocated elements in the component_manager */
};

struct AB_ECS_world {
    AB_vec(struct AB_ECS_system) default_systems; 
    /**< The runtime systems, identified by enum */
    AB_vec(struct AB_ECS_component_manager) component_managers;
    /**< The component_managers, identified by id */
    AB_HASHTABLE_DECLARE(runtime_systems, 4); 
    /**< The runtime systems - identified by hash */
};

#endif /* AMBER_ECS_WORLD_H */
