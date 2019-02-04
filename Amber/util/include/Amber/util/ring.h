#ifndef AMBER_UTIL_RING_H
#define AMBER_UTIL_RING_H

#include <Amber/util/common.h>
#include <SDL_atomic.h>
#include <stdbool.h>

#define AB_CACHELINE_SIZE 64

struct AB_ring {
    SDL_atomic_t c_head; /* dequeue from here */
    char pad1[AB_CACHELINE_SIZE - sizeof(unsigned)];

    SDL_atomic_t p_tail; /* enqueue from here */
    SDL_atomic_t p_head; /* mpmc -  */
    char pad2[AB_CACHELINE_SIZE - sizeof(unsigned) * 2];

    unsigned mask;
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

static inline unsigned AB_ring_capacity(const struct AB_ring *ring)
{
    return ring->mask + 1;
}

static inline void AB_ring_init(struct AB_ring *ring, unsigned size)
{
    AB_ASSERT(size >= 2);
    AB_ASSERT((size & (size - 1)) == 0); /* size is a power of 2 */

    ring->mask = size - 1;
    SDL_AtomicSet(&ring->p_tail, 0);
    SDL_AtomicSet(&ring->p_head, 0);
    SDL_AtomicSet(&ring->c_head, 0);
}

unsigned AB_ring_size(struct AB_ring *ring);

/* Single producer / single consumer */
bool AB_ring_enqueue_sp(struct AB_ring *ring,
        void *restrict buffer,
        const void *restrict entry,
        unsigned entry_size, unsigned *size);
bool AB_ring_dequeue_sc(struct AB_ring *ring,
        const void *restrict buffer,
        void *restrict entry,
        unsigned entry_size);

/* Multiple producer / multiple consumer */
bool AB_ring_enqueue_mp(struct AB_ring *ring,
        void *buffer,
        const void *entry,
        unsigned entry_size,
        unsigned *size);
bool AB_ring_dequeue_mc(struct AB_ring *ring,
        const void *buffer,
        void *data,
        unsigned entry_size);
bool AB_ring_trydequeue_mc(struct AB_ring *ring,
        const void *buffer,
        void *data,
        unsigned entry_size);

/** @brief Make type-safe prototypes for a datatype
 * @param name the postfix of the function prototypes
 * @param type the type of data in the ring-buffer
 */
#define AB_RING_PROTOTYPE(name, type)                                       \
    static inline bool AB_ring_enqueue_spsc_##name(struct AB_ring *ring,    \
            type *buf, type *entry, unsigned *sz)                           \
    {                                                                       \
        AB_ring_enqueue_sp(ring, buf, entry, sizeof(type), sz);             \
    }                                                                       \
    static inline bool AB_ring_dequeue_spsc_##name(struct AB_ring *ring,    \
            type *buf, type *entry)                                         \
    {                                                                       \
        AB_ring_dequeue_sc(ring, buf, entry, sizeof(type));                 \
    }                                                                       \
    static inline bool AB_ring_enqueue_mpmc_##name(struct AB_ring *ring,    \
            type *buf, type *entry, unsigned *sz)                           \
    {                                                                       \
        AB_ring_enqueue_mp(ring, buf, entry, sizeof(type), sz);             \
    }                                                                       \
    static inline bool AB_ring_dequeue_mpmc_##name(struct AB_ring *ring,    \
            type *buf, type *entry)                                         \
    {                                                                       \
        AB_ring_dequeue_mc(ring, buf, entry, sizeof(type));                 \
    }                                                                       \
    static inline bool AB_ring_trydequeue_mpmc_##name(struct AB_ring *ring, \
            type *buf, type *entry)                                         \
    {                                                                       \
        AB_ring_trydequeue_mc(ring, buf, entry, sizeof(type));              \
    }

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


#endif /* AMBER_UTIL_RING_H */
