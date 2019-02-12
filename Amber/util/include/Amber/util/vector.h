/* AB_vector.h
 */
#ifndef AB_VECTOR_AB_VECTOR_H
#define AB_VECTOR_AB_VECTOR_H

#include <Amber/util/common.h> /* AB_VEC_ASSERT, AB_STATIC_ASSERT, AB_INLINE */
#include <stddef.h> /* size_t */
#include <stdlib.h> /* realloc, free */
#include <string.h> /* memcpy */

/* We need an expression for our assertions */
static inline void AB_VEC_ASSERT(int cond, const char *msg) {
#if NDEBUG
    (void)cond;
    (void)msg;
#else
    if (!cond) {
        AB_LOG_ERROR("%s", msg);
        exit(1);
    }
#endif
}

#define AB_vec(type)          \
    struct {                  \
        unsigned num, capacity; \
        type *elems;          \
    }

#define AB_VEC_INITIALIZER \
    { 0, 0, NULL }

#define AB_vec_init(vec) do {                                                                                                       \
    AB_VEC_ASSERT((vec) != NULL, "AB_vec_init: recieved NULL vec");                                                                   \
    (vec)->num = (vec)->capacity = 0;                                                                                               \
    (vec)->elems = NULL;                                                                                                            \
} while (0)

#define AB_vec_destroy(vec) do {                                                                                                    \
    AB_VEC_ASSERT((vec) != NULL, "AB_vec_destroy: received NULL vec");                                                                \
    free((vec)->elems);                                                                                                             \
} while (0)

#define AB_vec_push(vec, elem)                                                                                                      \
    (AB_VEC_ASSERT((vec) != NULL, "AB_vec_push: received NULL vec"),                                                                  \
     (vec)->num == (vec)->capacity ?                                                                                                \
        (AB_vec_resize((vec), AB_vec_internal_next_capacity((vec)->capacity)) ?                                                     \
            1                                                                                                                       \
            : ((vec)->elems[(vec)->num++] = (elem), 0))                                                                             \
        : ((vec)->elems[(vec)->num++] = (elem), 0))

#define AB_vec_pop(vec, elem_ptr)                                                                                                   \
    (AB_VEC_ASSERT((vec) != NULL, "AB_vec_pop: received NULL vec"),                                                                   \
     AB_VEC_ASSERT((vec)->num > 0, "AB_vec_pop: vector is empty!"),                                                                   \
     (vec)->num > 0 ?                                                                                                               \
        ((elem_ptr) == NULL ?                                                                                                       \
            (--(vec)->num, 0)                                                                                                       \
            : (*(elem_ptr) = (vec)->elems[--(vec)->num], 0))                                                                        \
        : 1)

#define AB_vec_internal_next_capacity(capacity)                                                                                     \
    ((capacity) ? (capacity) << 1 : 2)

#define AB_vec_pushp(vec)                                                                                                           \
    (AB_VEC_ASSERT((vec) != NULL, "AB_vec_pushp: recieved NULL vec"),                                                                 \
     (vec)->num == (vec)->capacity ?                                                                                                \
        (AB_vec_resize((vec), AB_vec_internal_next_capacity((vec)->capacity)) == 0 ?                                                \
            &(vec)->elems[(vec)->num++]                                                                                             \
            : NULL)                                                                                                                 \
        : &(vec)->elems[(vec)->num++])

static AB_INLINE unsigned AB_vec_internal_roundup(unsigned x) {
    unsigned power = 2;
    if (x <= 1)
        return 2;
    x--;
    while ((x >>= 1))
        power <<= 1;
    return power;
}

static AB_INLINE int AB_vec_internal_safe_realloc(void **ptr, unsigned size) {
    void *tmp = realloc(*ptr, size);
    if (tmp == NULL)
        return 1;
    *ptr = tmp;
    return 0;
}

#define AB_vec_resize(vec, newsize)                                                                                                 \
    (AB_VEC_ASSERT((vec) != NULL, "AB_vec_resize: received NULL vec"),                                                                \
     (vec)->capacity < AB_vec_internal_roundup((newsize)) ?                                                                         \
        AB_vec_internal_safe_realloc((void **)&(vec)->elems, AB_vec_internal_roundup((newsize)) * sizeof(*(vec)->elems)) ?          \
            1                                                                                                                       \
            : ((vec)->capacity = AB_vec_internal_roundup((newsize)), 0)                                                             \
        : ((vec)->num < (AB_vec_internal_roundup((newsize)) / 2) ?                                                                  \
            (AB_vec_internal_safe_realloc((void **)&(vec)->elems, AB_vec_internal_roundup((newsize)) / 2 * sizeof(*(vec)->elems)) ? \
                1                                                                                                                   \
                : ((vec)->capacity = AB_vec_internal_roundup((newsize)) / 2, 0))                                                    \
            : 0))

#define AB_vec_at(vec, idx)                                                                                                         \
    (*(AB_VEC_ASSERT((vec) != NULL, "AB_vec_at: received NULL vec"),                                                                  \
      AB_VEC_ASSERT((unsigned)(idx) < (vec)->num, "AB_vec_at: idx out of bounds"),                                                              \
      &(vec)->elems[(idx)]))

#define AB_vec_insert(vec, idx, elem)                                                                                               \
    (AB_VEC_ASSERT((vec) != NULL, "AB_vec_insert: received NULL vec"),                                                                \
     (idx) < (vec)->capacity ?                                                                                                      \
        (idx) < (vec)->num ?                                                                                                        \
            ((vec)->elems[(idx)] = (elem), 0)                                                                                       \
            : ((vec)->num = (idx) + 1, (vec)->elems[(idx)] = (elem), 0)                                                             \
        : (AB_vec_resize((vec), AB_vec_internal_roundup((idx) + 1)) ?                                                               \
            1                                                                                                                       \
            : ((vec)->num = (idx) + 1, (vec)->elems[(idx)] = (elem)), 0))

#define AB_vec_copy(dest, src)                                                                                                      \
    (AB_VEC_ASSERT((dest) != NULL, "AB_vec_copy: received NULL dest"),                                                                \
     AB_VEC_ASSERT((src) != NULL, "AB_vec_copy: received NULL src"),                                                                  \
     (void)AB_STATIC_ASSERT(sizeof(*(dest)->elems) == sizeof(*(src)->elems)),                                                                       \
     ((dest)->capacity > (src)->num ?                                                                                                \
      (AB_vec_resize((dest), (src)->num) ?                                                                                        \
           1                                                                                                                      \
           : (memcpy((dest)->elems, (src)->elems, (src)->num * sizeof(*(src)->elems)), 0))                                        \
       : (memcpy((dest)->elems, (src)->elems, (src)->num * sizeof(*(src)->elems)), 0)))

#define AB_vec_size(vec)                                                                                                            \
    (AB_VEC_ASSERT((vec) != NULL, "AB_vec_size: received NULL vec"), (vec)->num)

#define AB_vec_capacity(vec)                                                                                                        \
    (AB_VEC_ASSERT((vec) != NULL, "AB_vec_capacity: received NULL vec"), (vec)->capacity)

#endif /* AB_VECTOR_AB_VECTOR_H */
