/** 
 * @file vector.h
 * @brief A dynamic vector implementation
 */
#ifndef AB_VECTOR_AB_VECTOR_H
#define AB_VECTOR_AB_VECTOR_H

#include <Amber/util/common.h> /* AB_LOG_ERROR, AB_STATIC_ASSERT, AB_INLINE */
#include <stddef.h> /* size_t */
#include <stdlib.h> /* realloc, free */
#include <string.h> /* memcpy */

/** @brief Assertion for this header
 * @param cond The condition to check, aborts on failure
 * @param msg The message to print on failure
 *
 * The default AB_ASSERT() macro is not an expression
 */
#if NDEBUG
# define AB_VEC_ASSERT(cond, msg) /* Ignore */
#else
static AB_INLINE void AB_VEC_ASSERT(int cond, const char *msg) {
    if (!cond) {
        AB_LOG_ERROR("%s", msg);
        exit(1);
    }
}
#endif

/** @brief Type of a vector holding items of type @p type
 * @param type The type of the elements the vector holds
 */
#define AB_vec(type)                                                                                                                \
    struct {                                                                                                                        \
        unsigned num, capacity;                                                                                                     \
        type *elems;                                                                                                                \
    }

/** @brief The vector static initializer */
#define AB_VEC_INITIALIZER                                                                                                          \
    { 0, 0, NULL }

/** @brief Initialize a vector
 * @param vec The vector to initialize
 * @warning This macro evaluates @p vec multiple times
 * @hideinitializer
 */
#define AB_vec_init(vec) do {                                                                                                       \
    AB_VEC_ASSERT((vec) != NULL, "AB_vec_init: recieved NULL vec");                                                                 \
    (vec)->num = (vec)->capacity = 0;                                                                                               \
    (vec)->elems = NULL;                                                                                                            \
} while (0)

/** @brief Destroy a vector
 * @param vec The vector to destroy
 * @warning This macro evaluates @p vec multiple times
 * @hideinitializer
 */
#define AB_vec_destroy(vec) do {                                                                                                    \
    AB_VEC_ASSERT((vec) != NULL, "AB_vec_destroy: received NULL vec");                                                              \
    free((vec)->elems);                                                                                                             \
} while (0)

/** @brief Add an element to a vector
 * @param vec The vector to add to
 * @param elem The element to add to the vector
 * @warning This macro evaluates @p vec and @p elem multiple times
 * @return 0 on success, 1 on error
 * @hideinitializer
 */
#define AB_vec_push(vec, elem)                                                                                                      \
    (AB_VEC_ASSERT((vec) != NULL, "AB_vec_push: received NULL vec"),                                                                \
     (vec)->num == (vec)->capacity ?                                                                                                \
        (AB_vec_resize((vec), AB_vec_internal_next_capacity((vec)->capacity)) ?                                                     \
            1                                                                                                                       \
            : ((vec)->elems[(vec)->num++] = (elem), 0))                                                                             \
        : ((vec)->elems[(vec)->num++] = (elem), 0))

/** @brief Remove the last element from a vector
 * @param vec The vector to remove from
 * @param elem_ptr Pointer to the element to pop into
 * @return 0 on success, 1 on error
 * @warning This macro evaluates @p vec and @p elem_ptr multiple times
 * @hideinitializer
 */
#define AB_vec_pop(vec, elem_ptr)                                                                                                   \
    (AB_VEC_ASSERT((vec) != NULL, "AB_vec_pop: received NULL vec"),                                                                 \
     AB_VEC_ASSERT((vec)->num > 0, "AB_vec_pop: vector is empty!"),                                                                 \
     (vec)->num > 0 ?                                                                                                               \
        ((elem_ptr) == NULL ?                                                                                                       \
            (--(vec)->num, 0)                                                                                                       \
            : (*(elem_ptr) = (vec)->elems[--(vec)->num], 0))                                                                        \
        : 1)

/** @cond false */
#define AB_vec_internal_next_capacity(capacity)                                                                                     \
    ((capacity) ? (capacity) << 1 : 2)
/** @endcond */

/** @brief Push space onto a vector, returning a pointer to the location
 * @param vec The vector to push onto
 * @return A pointer to the new element slot
 * @warning This macro evaluates @p vec multiple times
 * @hideinitializer
 */
#define AB_vec_pushp(vec)                                                                                                           \
    (AB_VEC_ASSERT((vec) != NULL, "AB_vec_pushp: recieved NULL vec"),                                                               \
     (vec)->num == (vec)->capacity ?                                                                                                \
        (AB_vec_resize((vec), AB_vec_internal_next_capacity((vec)->capacity)) == 0 ?                                                \
            &(vec)->elems[(vec)->num++]                                                                                             \
            : NULL)                                                                                                                 \
        : &(vec)->elems[(vec)->num++])

/** @cond false */
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
/** @endcond */

/** @brief Resize a vector
 * @param vec The vector to resize
 * @param newsize The new size to make the vector
 * @return 0 on success, 1 on error
 * @warning This macro evaluates @p vec and @p newsize multiple times
 * @hideinitializer
 */
#define AB_vec_resize(vec, newsize)                                                                                                 \
    (AB_VEC_ASSERT((vec) != NULL, "AB_vec_resize: received NULL vec"),                                                              \
     (vec)->capacity < AB_vec_internal_roundup((newsize)) ?                                                                         \
        AB_vec_internal_safe_realloc((void **)&(vec)->elems, AB_vec_internal_roundup((newsize)) * sizeof(*(vec)->elems)) ?          \
            1                                                                                                                       \
            : ((vec)->capacity = AB_vec_internal_roundup((newsize)), 0)                                                             \
        : ((vec)->num < (AB_vec_internal_roundup((newsize)) / 2) ?                                                                  \
            (AB_vec_internal_safe_realloc((void **)&(vec)->elems, AB_vec_internal_roundup((newsize)) / 2 * sizeof(*(vec)->elems)) ? \
                1                                                                                                                   \
                : ((vec)->capacity = AB_vec_internal_roundup((newsize)) / 2, 0))                                                    \
            : 0))

/** @brief Return the element at position @p idx
 * @param vec The vector to index
 * @param idx The index of the element
 * @return The value at index @p idx as an lvalue
 * @warning This macro evaluates @p vec and @p idx multiple times
 * @hideinitializer
 * @note This returns an lvalue, so you can take the address of the return value
 * @hideinitializer
 */
#define AB_vec_at(vec, idx)                                                                                                         \
    (*(AB_VEC_ASSERT((vec) != NULL, "AB_vec_at: received NULL vec"),                                                                \
      AB_VEC_ASSERT((unsigned)(idx) < (vec)->num, "AB_vec_at: idx out of bounds"),                                                  \
      &(vec)->elems[(idx)]))

/** @brief Insert an element into an arbitrary index
 * @param vec The vector to insert into
 * @param idx The index to insert into
 * @param elem The element to insert
 * @return 0 on success, 1 on error
 * @warning This macro evaluates @p vec, @p idx, and @p elem multiple times
 * @hideinitializer
 */
#define AB_vec_insert(vec, idx, elem)                                                                                               \
    (AB_VEC_ASSERT((vec) != NULL, "AB_vec_insert: received NULL vec"),                                                              \
     (idx) < (vec)->capacity ?                                                                                                      \
        (idx) < (vec)->num ?                                                                                                        \
            ((vec)->elems[(idx)] = (elem), 0)                                                                                       \
            : ((vec)->num = (idx) + 1, (vec)->elems[(idx)] = (elem), 0)                                                             \
        : (AB_vec_resize((vec), AB_vec_internal_roundup((idx) + 1)) ?                                                               \
            1                                                                                                                       \
            : ((vec)->num = (idx) + 1, (vec)->elems[(idx)] = (elem)), 0))

/** @brief Copy a vector's elements from @p src to @p dest
 * @param[out] dest The output vector
 * @param[in] src The input vector
 * @return 0 on success, 1 on error
 * @warning This macro evaluates @p dest and @p src multiple times
 * @hideinitializer
 */
#define AB_vec_copy(dest, src)                                                                                                      \
    (AB_VEC_ASSERT((dest) != NULL, "AB_vec_copy: received NULL dest"),                                                              \
     AB_VEC_ASSERT((src) != NULL, "AB_vec_copy: received NULL src"),                                                                \
     (void)AB_STATIC_ASSERT(sizeof(*(dest)->elems) == sizeof(*(src)->elems)),                                                       \
     ((dest)->capacity > (src)->num ?                                                                                               \
      (AB_vec_resize((dest), (src)->num) ?                                                                                          \
           1                                                                                                                        \
           : (memcpy((dest)->elems, (src)->elems, (src)->num * sizeof(*(src)->elems)), 0))                                          \
       : (memcpy((dest)->elems, (src)->elems, (src)->num * sizeof(*(src)->elems)), 0)))

/** @brief Return the number of elements in a vector
 * @param vec The vector
 * @return The number of elements in the vector
 * @warning This macro evaluates @p vec multiple times
 * @hideinitializer
 */
#define AB_vec_size(vec)                                                                                                            \
    (AB_VEC_ASSERT((vec) != NULL, "AB_vec_size: received NULL vec"), (vec)->num)

/** @brief Return the number of allocated slots currently in a vector
 * @param vec The vector
 * @return The number of allocated slots in the vector
 * @warning This macro evaluates @p vec multiple times
 * @hideinitializer
 */
#define AB_vec_capacity(vec)                                                                                                        \
    (AB_VEC_ASSERT((vec) != NULL, "AB_vec_capacity: received NULL vec"), (vec)->capacity)

#endif /* AB_VECTOR_AB_VECTOR_H */
