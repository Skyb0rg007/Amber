
# include <Amber/compat/compat.h>
# include <Amber/compat/stdbool.h>
# include <SDL_atomic.h>

typedef SDL_atomic_t AB_atomic_int;
typedef SDL_atomic_t AB_atomic_uint;

static inline
int AB_atomic_load_int(AB_atomic_int *n)
{
    return SDL_AtomicGet(n);
}

static inline
unsigned AB_atomic_load_uint(AB_atomic_uint *n)
{
    return (unsigned)SDL_AtomicGet(n);
}

static inline
void AB_atomic_store_int(AB_atomic_int *n, int val)
{
    SDL_AtomicSet(n, val);
}

static inline
void AB_atomic_store_uint(AB_atomic_uint *n, unsigned val)
{
    SDL_AtomicSet(n, (int)val);
}

static inline
void AB_atomic_init_int(AB_atomic_int *n, int val)
{
    n->value = val;
}

static inline
void AB_atomic_init_uint(AB_atomic_uint *n, unsigned val)
{
    n->value = (int)val;
}

static inline
bool AB_atomic_compare_exchange_weak_int(AB_atomic_int *n, int *oldval, int newval)
{
    bool res = SDL_AtomicCAS(n, *oldval, newval);
    if (!res)
        *oldval = SDL_AtomicGet(n);
    return res;
}

static inline
bool AB_atomic_compare_exchange_weak_uint(AB_atomic_uint *n, unsigned *oldval, unsigned newval)
{
    bool res = SDL_AtomicCAS(n, (int)*oldval, (int)newval);
    if (!res)
        *oldval = (unsigned)SDL_AtomicGet(n);
    return res;
}
