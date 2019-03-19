#include <stdatomic.h>
#include <SDL_thread.h>

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define CACHESIZE 64

struct ring {
    atomic_uint c_head;
    char _pad0[CACHESIZE - sizeof(atomic_uint)];

    atomic_uint p_head; /* Used to sync producers */
    atomic_uint p_tail;
    char _pad1[CACHESIZE - sizeof(atomic_uint) * 2];

    unsigned mask;
    char _pad2[CACHESIZE - sizeof(unsigned)];
};

static bool stdatomic_cas(atomic_uint *a, unsigned old, unsigned new)
{
    return atomic_compare_exchange_strong(a, &old, new);
}

static bool ring_enqueue(struct ring *ring, int32_t *buf, int32_t entry)
{
    const unsigned mask = ring->mask;

    unsigned producer;
    unsigned consumer;

    /* Increment p_head while ensuring it doesn't pass c_head */
    do {
        producer = atomic_load(&ring->p_head);
        consumer = atomic_load(&ring->c_head);

        /* Check if buffer is full */
        if ((producer - consumer) >= mask)
            return false;

    } while (!stdatomic_cas(&ring->p_head, producer, producer + 1));

    /* producer is now a unique entry that is behind p_head and after c_head 
     * producer should also be ahead of p_tail
     */
    buf[producer & mask] = entry;

    /* Swing p_tail to current position to allow consumption 
     * We stall to ensure we don't expose a slower producer thread
     */
    while (!stdatomic_cas(&ring->p_tail, producer, producer + 1)) {
        /* Stall */
    }

    return true;
}

static bool ring_dequeue(struct ring *ring, int32_t *buf, int32_t *entry)
{
    const unsigned mask = ring->mask;

    unsigned consumer;
    unsigned producer;

    do {
        consumer = atomic_load(&ring->c_head);
        producer = atomic_load(&ring->p_tail);

        /* Ensure buffer is not empty */
        if (producer == consumer)
            return false;

        /* Even if p_tail has been changed, c_head is still going to be valid
         * because p_tail is only ever increased
         */
        *entry = buf[consumer & mask];

        /* consumer may not be unique, but buf[consumer] is only read
         * now try to increment consumer - if it succeeds we are done
         * if it fails, then a different consumer took this entry instead
         */
    } while (!stdatomic_cas(&ring->c_head, consumer, consumer + 1));

    return true;
}

/* Globals */
static struct ring ring = {
    .p_head = ATOMIC_VAR_INIT(0),
    .p_tail = ATOMIC_VAR_INIT(0),
    .c_head = ATOMIC_VAR_INIT(0),
    .mask = (1 << 20) - 1 
};
static int32_t ring_buffer[1 << 20];

int b = 0;

/* Threads */
static int producer(void *data) {

    int i;
    for (i = 0; i < 20; i++) {
        bool ret = ring_enqueue(&ring, ring_buffer, (int32_t)i);
        assert(ret);
    }

    ring_enqueue(&ring, ring_buffer, -1);

    (void)data;
    return 0;
}

static int consumer(void *data) {

    int32_t entry;
    do {
        bool ret = ring_dequeue(&ring, ring_buffer, &entry);
        if (ret)
            fprintf(stderr, "Got value %"PRIi32"!\n", entry);
    } while (entry != -1);

    (void)data;
    return 0;
}

int main(void)
{
    fprintf(stderr, "ring_buffer: %p\n", (void *)ring_buffer);
    ring_enqueue(&ring, ring_buffer, 1);
    SDL_Thread *t1 = SDL_CreateThread(producer, "producer", NULL);
    SDL_Thread *t2 = SDL_CreateThread(producer, "producer", NULL);
    SDL_Thread *t3 = SDL_CreateThread(consumer, "consumer", NULL);
    SDL_Thread *t4 = SDL_CreateThread(consumer, "consumer", NULL);

    SDL_WaitThread(t1, NULL);
    SDL_WaitThread(t2, NULL);
    SDL_WaitThread(t3, NULL);
    SDL_WaitThread(t4, NULL);
}
