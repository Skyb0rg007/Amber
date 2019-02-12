/** @file vector.h
 * @brief Generic resizable vector
 */
#ifndef AMBER_UTIL_VECTOR_H
#define AMBER_UTIL_VECTOR_H

#include <Amber/util/common.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#if defined(__DOXYGEN__)

/** @brief Vector type that holds elements of type @c type
 * @param type the type of elements contained in the vector
 */
#define AB_vec(type)

/** @brief Initializer for the vector */
#define AB_VEC_INITIALIZER

/** @brief Initialize a vector */
void AB_vec_init(AB_vec *vec);

/** @brief Destroy a vector */
void AB_vec_destroy(AB_vec *vec);

/** @brief Add to the vector
 * @param vec the vector of type @c type
 * @param elem the item to push onto the vector
 * @return 0 on success, 1 on error
 */
int AB_vec_push(AB_vec *vec, type elem);

/** @brief Remove the last item from the vector
 * @param vec the vector of type @c type
 * @param elem_ptr where to pop the element into
 * @return 0 on success, 1 on error
 * @note @c elem_ptr must be of type 'type *', cast NULL if not using value
 */
int AB_vec_pop(AB_vec *vec, type *elem_ptr);

/** @brief Add an item to the end of the vector, returning a pointer to it 
 * @param vec the vector of type @c type
 * @return a pointer to the new element, or NULL on error
 */
type *AB_vec_pushp(AB_vec *vec);

#else /* __DOXYGEN__ */

struct AB_vector_generic {
    unsigned num, capacity;
    void *elems;
};

#define AB_vec(type) \
    struct { unsigned num, capacity; type *elems }

#define AB_VEC_INITIALIZER { 0, 0, NULL }

static inline void AB_vec_init_generic(struct AB_vector_generic *vec) {
    vec->num = vec->capacity = 0; vec->elems = 0;
}
#define AB_vec_init(vec) \
    AB_vec_init_generic((struct AB_vector_generic *)(vec))

static inline void AB_vec_destroy_generic(struct AB_vector_generic *vec) {
    free(vec->elems);
    AB_vec_init(vec);
}
#define AB_vec_destroy(vec) \
    AB_vec_destroy_generic((struct AB_vector_generic *)(vec))

#define AB_vec_push(vec, elem) \
    ((vec)->num == (vec)->capacity ? \
        (AB_vec_resize((vec), AB_vec_internal_next_capacity((vec)->capacity)) ? \
            1 : \
            ((vec)->elems[(vec)->num++] = (elem), 0)) : \
        ((vec)->elems[(vec)->num++] = (elem), 0))

#define AB_vec_pop(vec, elem_ptr) \
    ((vec)->num == 0 ? \
        1 : \
        ((elem_ptr) == NULL ? \
            (--(vec)->num, 0) : \
            (*(elem_ptr) = (vec)->elems[--(vec)->num], 0)))

#define AB_vec_internal_next_capacity(cap) \
    ((cap) ? (cap) << 1 : 2)

#define AB_vec_pushp(vec) \
    ((vec)->num == (vec)->capacity ? \
        (AB_vec_resize((vec), AB_vec_internal_next_capacity((vec)->capacity)) == 0 ? \
            &(vec)->elems[(vec)->num++] : \
            NULL) : \
        &(vec)->elems[(vec)->num++])

static inline int AB_vec_resize_generic(struct AB_vector_generic *vec, unsigned cap) {

}

#endif /* __DOXYGEN__ */
#endif /* AMBER_UTIL_VECTOR_H */
