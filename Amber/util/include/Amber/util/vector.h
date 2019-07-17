/* @file vector.h
 * @brief Generic resizable vector
 *
 * Design: Each macro is a statement, not an expression
 * ie. you cannot write @code if (AB_VEC_DESTROY(vec)) {} @endcode
 * All macros may evaluate their arguments multiple times
 * No macros return anything (except AB_VEC_AT())
 *
 * Each unsafe function takes a statement 'on_fail', which is run if failure occurs
 * You can use the no-op macro AB_VEC_ONFAIL() to document what this does.
 *
 * The non-safe variants call the AB_VEC_ABORT() routine on failure instead,
 * which aborts the program. Use the safe variants for library code, but if
 * memory failure means program failure, the non-safe is also good.
 *
 * Note: I use the term 'function' to mean statement-macro.
 * Only AB_VEC_AT() is an expression. (An lvalue, ie. you can assign to it)
 *
 * TODO: functions for custom allocators / deallocators
 */
#ifndef AMBER_UTIL_VECTOR_H
#define AMBER_UTIL_VECTOR_H

#include <Amber/util/common.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/***************************************************************************
 *
 * Non-function macros
 *
 **************************************************************************/

/** @brief Vector type that holds elements of type @c type
 * @param type the type of elements contained in the vector
 */
#define AB_vec(type)                                                                       \
    struct { type *elems; unsigned num, capacity; }

/** @brief Initializer for the vector */
#define AB_VEC_INITIALIZER                                                                 \
    { NULL, 0, 0 }

/***************************************************************************
 *
 * Non-allocating functions
 *
 * If these functions fail, it's your fault. AB_ASSERT() is liberally used.
 *
 **************************************************************************/

/** @brief Initialize a vector */
#define AB_VEC_INIT(vec) do {                                                              \
    AB_ASSERT((vec) != NULL);                                                              \
    (vec)->elems = NULL;                                                                   \
    (vec)->num = (vec)->capacity = 0;                                                      \
} while (0)

/** @brief Destroy a vector */
#define AB_VEC_DESTROY(vec) do {                                                           \
    AB_ASSERT((vec) != NULL);                                                              \
    free((vec)->elems);                                                                    \
} while (0)

/** @brief Destroy a vector with a custom de-allocator
 * @param free_fn void free_f(void *ptr, size_t size)
 */
#define AB_VEC_DESTROY_A(vec, free_fn) do {                                                \
    AB_ASSERT((vec) != NULL);                                                              \
    free_fn((vec)->elems, (vec)->num * sizeof(*(vec)->elems));                             \
} while (0)

/** @brief Remove the last item from the vector
 * @param vec the vector of type @c type
 * @param elem_ptr where to pop the element into
 * @note @c elem_ptr must be of type 'type *', cast NULL if not using value
 */
#define AB_VEC_POP(vec, elem_ptr) do {                                                     \
    AB_VEC_ASSERT((vec) != NULL);                                                          \
    AB_VEC_ASSERT((vec)->num > 0);                                                         \
    if ((vec)->num > 0) {                                                                  \
        (vec)->num--;                                                                      \
        if ((elem_ptr) != NULL) {                                                          \
            *(elem_ptr) = (vec)->elems[(vec)->num];                                        \
        }                                                                                  \
    }                                                                                      \
} while (0)

/** @brief Return the element at position @p idx
 * @param vec The vector to index
 * @param idx The index of the element
 * @return The value at index @p idx as an lvalue
 * @note This returns an lvalue, so you can take the address of the return value
 * @hideinitializer
 */
#define AB_VEC_AT(vec, idx)                                                                \
    (*(AB_ASSERT_EXPR((vec) != NULL),                                                      \
      AB_ASSERT_EXPR((unsigned)(idx) < (vec)->num),                                        \
      &(vec)->elems[(idx)]))

/** @brief Return the number of elements in a vector
 * @param vec The vector
 * @return The number of elements in the vector
 * @warning This macro evaluates @p vec multiple times
 * @hideinitializer
 */
#define AB_VEC_NUM(vec)                                                                    \
    (AB_ASSERT_EXPR((vec) != NULL),                                                        \
     (vec)->num)

/** @brief Return the number of allocated slots currently in a vector
 * @param vec The vector
 * @return The number of allocated slots in the vector
 * @warning This macro evaluates @p vec multiple times
 * @hideinitializer
 */
#define AB_VEC_CAPACITY(vec)                                                               \
    (AB_ASSERT_EXPR((vec) != NULL),                                                        \
     (vec)->capacity)

/***************************************************************************
 *
 * Allocating functions
 *
 * Each function takes a statement 'on_fail', which is run if failure occurs
 * You can use the no-op macro AB_VEC_ONFAIL() to document what this does.
 *
 * Normal usage:
 *
 * void my_func(void)
 * {
 *         AB_vec(int) my_vec = AB_VEC_INITIALIZER;
 *         int i;
 *         for (i = 0; i < 20; i++) {
 *                 AB_VEC_PUSH(&my_vec, i, AB_VEC_ONFAIL(goto cleanup));
 *         }
 *         printf("Num: %\n", AB_VEC_NUM(&my_vec));
 * cleanup:
 *         AB_VEC_DESTROY(my_vec);
 * }
 *
 **************************************************************************/

/** @brief Document the on_fail argument for the following macros */
#define AB_VEC_ONFAIL(X) X
/** @brief Abort the program, showing a basic message regarding allocation */
#define AB_VEC_ABORT()                                                         \
    AB_LOG(AB_LOG_CATEGORY_GENERAL, AB_LOG_PRIORITY_CRITICAL,                  \
            "[AB_vec]: Failed to allocate memory, aborting...");               \
    abort()

/** @brief Add to the vector
 * @param vec the vector of type @c type
 * @param elem the item to push onto the vector
 */
#define AB_VEC_PUSH(vec, elem, on_fail) do {                                               \
    AB_ASSERT((vec) != NULL);                                                              \
    if ((vec)->num == (vec)->capacity) {                                                   \
        AB_VEC_RESIZE((vec),                                                               \
                AB_vec_internal_next_capacity((vec)->capacity),                            \
                on_fail);                                                                  \
    }                                                                                      \
    (vec)->elems[(vec)->num++] = (elem);                                                   \
} while (0)

/** @brief Add an item to the end of the vector, loading a pointer to it
 * @param vec the vector of type @c type
 * @param elemptr pointer to @c type*, loaded with the location of the new slot
 */
#define AB_VEC_PUSHP(vec, elemptr, on_fail) do {                                           \
    AB_ASSERT((vec) != NULL);                                                              \
    if ((vec)->num == (vec)->capacity) {                                                   \
        AB_VEC_RESIZE((vec),                                                               \
                AB_vec_internal_next_capacity((vec)->capacity),                            \
                on_fail);                                                                  \
    }                                                                                      \
    *(elemptr) = &(vec)->elems[(vec)->num++];                                              \
} while (0)

/** @brief Resize a vector
 * @param vec The vector to resize
 * @param newsize The new size to make the vector
 * @return 0 on success, 1 on error
 * @warning This macro evaluates @p vec and @p newsize multiple times
 * @hideinitializer
 */
#define AB_VEC_RESIZE(vec, newsize, on_fail) do {                                          \
    AB_ASSERT((vec) != NULL);                                                              \
    if ((vec)->capacity < AB_vec_internal_roundup((newsize))) {                            \
        if (!AB_vec_internal_safe_realloc(                                                 \
                    (void **)&(vec)->elems,                                                \
                    AB_vec_internal_roundup((newsize)) * (unsigned)sizeof(*(vec)->elems))) { \
            on_fail;                                                                       \
       } else { \
           (vec)->capacity = AB_vec_internal_roundup((newsize)); \
       }                                                                                   \
    }                                                                                      \
} while (0)


/** @brief Push an element, aborting if memory allocation fails */
#define AB_VEC_PUSH_(vec, elem)      AB_VEC_PUSH(vec, elem, AB_VEC_ABORT())
/** @brief Push an element pointer, aborting if memory allocation fails */
#define AB_VEC_PUSHP_(vec, elemptr)  AB_VEC_PUSHP(vec, elemptr, AB_VEC_ABORT())
/** @brief Resize a vector, aborting if memory allocation fails */
#define AB_VEC_RESIZE_(vec, newsize) AB_VEC_RESIZE(vec, newsize, AB_VEC_ABORT())

/***************************************************************************
 *
 * Internal functions
 *
 * These should not be used by normal programs, and are only used from
 * the above macros.
 *
 **************************************************************************/
/** @cond false */
/* Determine the next capacity to allocate a vector */
AB_ATTR_CONST
static inline unsigned AB_vec_internal_next_capacity(unsigned x) {
    return x == 0 ? 2 : x << 1;
}

/* Find the next power of 2
 * TODO: make more efficient
 */
AB_ATTR_CONST
static inline unsigned AB_vec_internal_roundup(unsigned x) {
    unsigned power = 2;
    if (x <= 1)
        return 2;
    x--;
    while ((x >>= 1) != 0)
        power <<= 1;
    return power;
}

/* Reallocate the pointer *ptr, returning 1 on error */
static inline bool AB_vec_internal_safe_realloc(void **ptr, unsigned size) {
    void *tmp = realloc(*ptr, size);
    if (tmp == NULL)
        return false;
    *ptr = tmp;
    return true;
}
/** @endcond */

#endif
