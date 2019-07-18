
# include <Amber/compat/stdbool.h>

typedef int AB_atomic_int;
typedef unsigned AB_atomic_uint;

static inline
int AB_atomic_load_int(AB_atomic_int *n)
{
    return __atomic_load_n(n, __ATOMIC_SEQ_CST);
}

static inline
unsigned AB_atomic_load_uint(AB_atomic_uint *n)
{
    return __atomic_load_n(n, __ATOMIC_SEQ_CST);
}

static inline
void AB_atomic_store_int(AB_atomic_int *n, int val)
{
    __atomic_store_n(n, val, __ATOMIC_SEQ_CST);
}

static inline
void AB_atomic_store_uint(AB_atomic_uint *n, unsigned val)
{
    __atomic_store_n(n, val, __ATOMIC_SEQ_CST);
}

static inline
void AB_atomic_init_int(AB_atomic_int *n, int val)
{
    *n = val;
}

static inline
void AB_atomic_init_uint(AB_atomic_uint *n, unsigned val)
{
    *n = val;
}

static inline
bool AB_atomic_compare_exchange_weak_int(AB_atomic_int *n, int *oldval, int newval)
{
    return __atomic_compare_exchange_n(n, oldval, newval,
            true, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

static inline
bool AB_atomic_compare_exchange_weak_uint(AB_atomic_uint *n, unsigned *oldval, unsigned newval)
{
    return __atomic_compare_exchange_n(n, oldval, newval,
            true, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}
