
#include <Amber/compat/stdbool.h>
#include <stdatomic.h>

typedef int AB_atomic_int;
typedef unsigned AB_atomic_uint;

static inline
int AB_atomic_load_int(AB_atomic_int *obj)
{
    return atomic_load(obj);
}

static inline
unsigned AB_atomic_load_uint(AB_atomic_uint *obj)
{
    return atomic_load(obj);
}

static inline
void AB_atomic_store_int(AB_atomic_int *obj, int val)
{
    atomic_store(obj, val);
}

static inline
void AB_atomic_store_uint(AB_atomic_uint *obj, unsigned val)
{
    atomic_store(obj, val);
}

static inline
void AB_atomic_init_int(AB_atomic_int *obj, int val)
{
    atomic_init(obj, val);
}

static inline
void AB_atomic_init_uint(AB_atomic_uint *obj, unsigned val)
{
    atomic_init(obj, val);
}

static inline
bool AB_atomic_compare_exchange_weak_int(AB_atomic_int *obj, int *expected, int desired)
{
    return atomic_compare_exchange_weak(obj, expected, desired);
}

static inline
bool AB_atomic_compare_exchange_weak_uint(AB_atomic_uint *obj, unsigned *expected, unsigned desired)
{
    return atomic_compare_exchange_weak(obj, expected, desired);
}
