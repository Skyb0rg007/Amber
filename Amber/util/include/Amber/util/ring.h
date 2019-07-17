/**
 * @file ring.h
 * @brief A concurrent ring-buffer implementation
 *
 * This implementation was taken from ConcurrencyKit
 */
#ifndef AMBER_UTIL_RING_H
#define AMBER_UTIL_RING_H

#include <Amber/compat/atomic.h>
#include <Amber/util/common.h>

#define AB_CACHELINE_SIZE 64

/** @brief The ring buffer tracker.
 * @note Don't access these fields directly
 */
struct AB_ring {
    atomic_int c_head; /**< Where to dequeue from */
    char pad1[AB_CACHELINE_SIZE - sizeof(atomic_int)]; /**< padding */

    atomic_int p_tail; /**< Where to enqueue from */
    atomic_int p_head; /**< mpmc - synchronize writers */
    char pad2[AB_CACHELINE_SIZE - sizeof(atomic_int) * 2]; /**< padding */

    unsigned mask; /**< The ring buffer size - 1, used as bitmask */
};

#if 0

Enqueue-ing

producer = p_head
consumer = c_head
if ((producer - consumer) < mask) {
    p_head = producer + 1
} else {
    producer = p_head
    no change -> return false
}
target = buf[producer]
while (p_tail != producer)
  wait();
p_tail = producer + 1

Dequeue-ing

do {
    consumer = c_head
    producer = p_tail
    target = buf[consumer]
} while (c_head = consumer + 1)

#endif

/** @brief Determine the ring buffer capacity */
static inline unsigned AB_ring_capacity(const struct AB_ring *ring)
{
    return ring->mask + 1;
}

/** @brief Initialize a ring buffer object */
static inline void AB_ring_init(struct AB_ring *ring, unsigned size)
{
    AB_ASSERT(size >= 2);
    AB_ASSERT((size & (size - 1)) == 0); /* size is a power of 2 */

    ring->mask = size - 1;
    atomic_init(&ring->p_tail, 0);
    atomic_init(&ring->p_head, 0);
    atomic_init(&ring->c_head, 0);
}

/** @brief Determine the number of elements in the ring buffer 
 * @note @p ring is not const because atomic access is necessary
 */
unsigned AB_ring_size(struct AB_ring *ring);

/* Single producer / single consumer */
/** @brief Single producer - enqueue an item
 * @param ring The ring object
 * @param buffer The ring buffer
 * @param[in] entry The entry to enqueue
 * @param entry_size The size of the entry
 * @param[out] size The number of elements in the ring buffer
 * @return true on success, false on error (max size reached)
 */
bool AB_ring_enqueue_sp(struct AB_ring *ring,
        void *restrict buffer,
        const void *restrict entry,
        unsigned entry_size, unsigned *size);
/** @brief Single consumer - dequeue an item
 * @param ring The ring object
 * @param buffer The ring buffer
 * @param[out] entry The entry to dequeue into
 * @param entry_size The size of the entry
 * @return true on success, false on error (size == 0)
 */
bool AB_ring_dequeue_sc(struct AB_ring *ring,
        const void *restrict buffer,
        void *restrict entry,
        unsigned entry_size);

/* Multiple producer / multiple consumer */
/** @brief Multiple producers - enqueue an item
 * @param ring The ring object
 * @param buffer The ring buffer
 * @param[in] entry The entry to enqueue
 * @param entry_size The size of the entry
 * @param[out] size Optional - number of items enqueued
 * @return true on success, false on error
 */
bool AB_ring_enqueue_mp(struct AB_ring *ring,
        void *buffer,
        const void *entry,
        unsigned entry_size,
        unsigned *size);
/** @brief Multiple consumers - dequeue an item
 * @param ring The ring object
 * @param buffer The ring buffer
 * @param[out] entry The entry to enqueue
 * @param entry_size The size of the entry
 * @return true on success, false on error
 */
bool AB_ring_dequeue_mc(struct AB_ring *ring,
        const void *buffer,
        void *data,
        unsigned entry_size);

/** @brief Multiple consumers - trydequeue
 * @param ring The ring object
 * @param buffer The ring buffer
 * @param[out] entry The entry to dequeue into
 * @param entry_size The entry size
 * @return true on dequeue, false on error / not dequeue
 */
bool AB_ring_trydequeue_mc(struct AB_ring *ring,
        const void *buffer,
        void *entry,
        unsigned entry_size);

/** @brief Define type-safe prototypes for a given datatype
 * @param name the postfix of the function prototypes
 * @param type the type of data in the ring-buffer
 * @hideinitializer
 */
#define AB_RING_PROTOTYPE(name, type)                                         \
    static inline bool AB_ring_enqueue_spsc_##name(                     \
            struct AB_ring *ring, type *buf, type *entry, unsigned *sz)       \
    {                                                                         \
        return AB_ring_enqueue_sp(ring, buf, entry, sizeof(type), sz);        \
    }                                                                         \
    static inline bool AB_ring_dequeue_spsc_##name(                     \
            struct AB_ring *ring, type *buf, type *entry)                     \
    {                                                                         \
        return AB_ring_dequeue_sc(ring, buf, entry, sizeof(type));            \
    }                                                                         \
    static inline bool AB_ring_enqueue_mpmc_##name(                     \
            struct AB_ring *ring, type *buf, type *entry, unsigned *sz)       \
    {                                                                         \
        return AB_ring_enqueue_mp(ring, buf, entry, sizeof(type), sz);        \
    }                                                                         \
    static inline bool AB_ring_dequeue_mpmc_##name(                     \
            struct AB_ring *ring, type *buf, type *entry)                     \
    {                                                                         \
        return AB_ring_dequeue_mc(ring, buf, entry, sizeof(type));            \
    }                                                                         \
    static inline bool AB_ring_trydequeue_mpmc_##name(                  \
            struct AB_ring *ring,  type *buf, type *entry)                    \
    {                                                                         \
        return AB_ring_trydequeue_mc(ring, buf, entry, sizeof(type));         \
    }

/** @cond false */
#define AB_RING_DEQUEUE_SPSC(name, ring, buf, entry) \
    AB_ring_dequeue_spsc_##name(ring, buf, entry)
#define AB_RING_ENQUEUE_SPSC(name, ring, buf, entry, size) \
    AB_ring_enqueue_spsc_##name(ring, buf, entry, size)

#define AB_RING_DEQUEUE_MPMC(name, ring, buf, entry) \
    AB_ring_dequeue_mpmc_##name(ring, buf, entry)
#define AB_RING_TRYDEQUEUE_MPMC(name, ring, buf, entry) \
    AB_ring_trydequeue_mpmc_##name(ring, buf, entry)
#define AB_RING_ENQUEUE_MPMC(name, ring, buf, entry, size) \
    AB_ring_enqueue_mpmc_##name(ring, buf, entry, size)
/** @endcond */


#endif /* AMBER_UTIL_RING_H */
