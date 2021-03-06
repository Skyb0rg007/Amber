#include <Amber/compat/atomic.h>
#include <Amber/util/common.h>
#include <Amber/util/ring.h>
#include <string.h>

unsigned AB_ring_size(struct AB_ring *ring)
{
    unsigned c = AB_atomic_load_uint(&ring->c_head);
    unsigned p = AB_atomic_load_uint(&ring->p_head);

    return (p - c) & ring->mask;
}

/******************************
 * Single-producer, single-consumer
 ******************************/
bool AB_ring_enqueue_sp(struct AB_ring *ring,
        void *restrict buffer,
        const void *restrict entry,
        unsigned entry_size, unsigned *size)
{
    const unsigned mask = ring->mask;

    unsigned consumer = AB_atomic_load_uint(&ring->c_head);
    unsigned producer = AB_atomic_load_uint(&ring->p_tail);
    unsigned delta = producer + 1;

    if (size != NULL)
        *size = (producer - consumer) & mask;

    /* Buffer is full */
    if ((delta & mask) == (consumer & mask))
        return false;

    buffer = (char *)buffer + entry_size * (producer & mask);
    memcpy(buffer, entry, entry_size);

    AB_atomic_store_uint(&ring->p_tail, delta);

    return true;
}

bool AB_ring_dequeue_sc(struct AB_ring *ring,
        const void *restrict buffer,
        void *restrict entry,
        unsigned entry_size)
{
    const unsigned mask = ring->mask;

    unsigned consumer = AB_atomic_load_uint(&ring->c_head);
    unsigned producer = AB_atomic_load_uint(&ring->p_tail);

    /* Buffer is empty */
    if (consumer == producer)
        return false;

    buffer = (const char *)buffer + entry_size * (consumer & mask);
    memcpy(entry, buffer, entry_size);

    AB_atomic_store_uint(&ring->c_head, (int)(consumer + 1));

    return true;
}

/***********************************
 * mpmc
 **********************************/
bool AB_ring_enqueue_mp(struct AB_ring *ring,
        void *buffer,
        const void *entry,
        unsigned entry_size,
        unsigned *size)
{
    int ret = true;
    const unsigned mask = ring->mask;

    unsigned producer = AB_atomic_load_uint(&ring->p_head);
    unsigned consumer, delta;

    for (;;) {
        consumer = AB_atomic_load_uint(&ring->c_head);
        delta = producer + 1;

        /* Check if buffer is full */
        if ((producer - consumer) < mask) {
            /* Swing p_head from producer to delta */
            if (AB_atomic_compare_exchange_weak_uint(&ring->p_head, &producer, (int)delta))
                break;
        } else {
            /* Buffer is full, but see if consumer and producer were
             * out of sync */
            unsigned new_producer = AB_atomic_load_uint(&ring->p_head);
            
            if (producer == new_producer) {
                /* They weren't out of sync, buffer is full */
                ret = false;
                goto leave;
            } else {
                /* They were out of sync, update and try again */
                producer = new_producer;
            }
        }
    }

    buffer = (char *)buffer + entry_size * (producer & mask);
    memcpy(buffer, entry, entry_size);

    /* Swing p_tail to current position to allow consumption 
     * Must CAS to ensure sync with other producers
     */
    unsigned tmp_producer = producer;
    while (!AB_atomic_compare_exchange_weak_uint(&ring->p_tail, &tmp_producer, delta))
        tmp_producer = producer;

leave:
    if (size != NULL)
        *size = (producer - consumer) & mask;
    return ret;
}

bool AB_ring_dequeue_mc(struct AB_ring *ring,
        const void *buffer,
        void *data,
        unsigned entry_size)
{
    const unsigned mask = ring->mask;

    unsigned producer;
    unsigned consumer = AB_atomic_load_uint(&ring->c_head);

    do {
        const void *target;
        producer = AB_atomic_load_uint(&ring->p_tail);

        /* Check if buffer is empty */
        if (producer == consumer)
            return false;

        target = (const char *)buffer + entry_size * (consumer & mask);
        memcpy(data, target, entry_size);

        /* Keep trying to swing c_head from previous spot to allow
         * writing to the previously-used space
         */
    } while (!AB_atomic_compare_exchange_weak_uint(&ring->c_head, &consumer, (int)(consumer + 1)));

    return true;
}

bool AB_ring_trydequeue_mc(struct AB_ring *ring,
        const void *buffer,
        void *data,
        unsigned entry_size)
{
    const unsigned mask = ring->mask;

    unsigned consumer = AB_atomic_load_uint(&ring->c_head);
    unsigned producer = AB_atomic_load_uint(&ring->p_tail);

    if (consumer == producer)
        return false;

    buffer = (const char *)buffer + entry_size * (consumer & mask);
    memcpy(data, buffer, entry_size);

    return AB_atomic_compare_exchange_weak_uint(&ring->c_head, &consumer, (int)(consumer + 1));
}
