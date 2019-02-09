#include <Amber/util/ring.h>
#include <SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

/* The two threads */
static int producer(void *);
static int consumer(void *);

/* Globals */
static struct AB_ring ring;
static int64_t *rb = NULL;

/* Define prototypes for working with int64_t buffers */
AB_RING_PROTOTYPE(int64_t, int64_t)
#define RING_SIZE (1 << 20)

int main(void)
{
    AB_ring_init(&ring, RING_SIZE);
    rb = calloc(sizeof(int64_t), RING_SIZE);
    AB_ASSERT(rb);

    SDL_Thread *t1 = SDL_CreateThread(producer, "p1", NULL);
    SDL_Thread *t3 = SDL_CreateThread(consumer, "c1", NULL);
    SDL_Thread *t2 = SDL_CreateThread(producer, "p2", NULL);
    SDL_Thread *t4 = SDL_CreateThread(consumer, "c2", NULL);

    SDL_WaitThread(t1, NULL);
    SDL_WaitThread(t2, NULL);
    SDL_WaitThread(t3, NULL);
    SDL_WaitThread(t4, NULL);

    while (AB_ring_size(&ring) > 0) {
        int64_t entry;
        bool res = AB_RING_DEQUEUE_MPMC(int64_t, &ring, rb, &entry);
        fprintf(stderr, "res = %s\tentry = %"PRIi64"\n", res ? "true" : "false", entry);
        if (!res) break;
    }

    free(rb);
    return 0;
}

static int producer(void *data)
{
    for (int i = 0; i < 2000; i++) {
        int64_t entry = i;
        unsigned size;
        bool res = AB_RING_ENQUEUE_MPMC(int64_t, &ring, rb, &entry, &size);
        if (!res) {
            fprintf(stderr, "Failed entering item i = %d, size = %u\n", i, size);
            break;
        } else {
            /* fprintf(stderr, "Entered item %"PRIi64", size = %u\n", entry, size); */
        }
    }

    AB_RING_ENQUEUE_MPMC(int64_t, &ring, rb, &(int64_t){-1}, NULL);

    (void)data;
    return 0;
}

static int consumer(void *data)
{
    SDL_threadID id = SDL_GetThreadID(NULL);
    id &= 0xffff;
    int64_t entry;
    do {
        bool res = AB_RING_DEQUEUE_MPMC(int64_t, &ring, rb, &entry);
        if (res) {
            fprintf(stderr, "[%lu] Got item %"PRIi64"\n", id, entry);
        }
    } while (entry != -1);

    (void)data;
    return 0;
}
