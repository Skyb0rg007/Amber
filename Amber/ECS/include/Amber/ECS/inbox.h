/**
 * @file inbox.h
 * @brief Helpers for sending messages to and from components
 */
#ifndef AMBER_ECS_INBOX_H
#define AMBER_ECS_INBOX_H

#include <Amber/ECS/world.h>
#include <Amber/util/common.h>
#include <Amber/util/ring.h>

/** @brief Send a message asynchronously
 * @param world The worldstate
 * @param system_id The system id to send a message to
 * @param message The message to send
 * @param message_size The size of the message to send
 */
static inline void AB_ECS_send_message_async(struct AB_ECS_world *world, 
        int system_id, const void *message, size_t message_size)
{
    struct AB_ECS_system_inbox *inbox = 
        &(AB_VEC_AT(&world->default_systems, system_id).inbox);

    AB_ASSERT(message_size == inbox->elem_size);
    AB_ring_enqueue_mp(&inbox->ring, inbox->elems, message, inbox->elem_size, NULL);
}

/** @brief Send a message synchronously
 * @param world The worldstate
 * @param system_id The system id to send a message to
 * @param message The message to send
 * @param message_size The size of the message to send
 */
static inline void AB_ECS_send_message_sync(struct AB_ECS_world *world, 
        int system_id, void *message, size_t message_size)
{
    struct AB_ECS_system *sys = &AB_VEC_AT(&world->default_systems, system_id);
    AB_ASSERT(sys->inbox.elem_size == message_size);
    sys->message_handler(world, message, sys);
}

/** @brief Iterate over all pending messages
 * @param msg Pointer to the message type
 * @param self The self pointer, passed in through the AB_ECS_system::run function
 * @warning This macro evaluates @p msg and @p self multiple times
 * @hideinitializer
 */
#define AB_ECS_foreach_message(msg, self)                     \
    while (AB_ring_dequeue_mc(                                \
                &((self)->inbox.ring),                          \
                (self)->inbox.elems,                         \
                (msg),                                         \
                (self)->inbox.elem_size))                    \

#endif /* AMBER_ECS_INBOX_H */
