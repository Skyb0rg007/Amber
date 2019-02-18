/** @file hashtable-khash.h
 * @brief A hashtable implementation based on khash.h
 */
#ifndef AMBER_UTIL_HASHTABLE_H
#define AMBER_UTIL_HASHTABLE_H

#include <Amber/util/common.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

/** @brief Decorator for functions that should be marked inline
 * @note This macro can be overidden
 */
#ifndef AB_HT_INLINE
# if __STDC_VERSION__ >= 199901L || defined(__DOXYGEN__)
#  define AB_HT_INLINE inline
# elif __GNUC__
#  define AB_HT_INLINE __inline__
# else
#  define AB_HT_INLINE
# endif
#endif /* AB_HT_INLINE */

/** @brief Runtime assertion function
 * @note This macro can be overidden
 */
#ifndef AB_HT_ASSERT
# include <assert.h>
# define AB_HT_ASSERT(cond) assert(cond)
#endif /* AB_HT_ASSERT */

/*********************
 * Bit manipulation 
 *
 * Determine if an element is empty or deleted
 *  using the least amount of memory possible
 ********************/
static AB_HT_INLINE int AB_ac_isempty(int32_t *flags, int32_t i)
{ return flags[i >> 4] >> ((i & 0xf) << 1) & 2; }
static AB_HT_INLINE int AB_ac_isdel(int32_t *flags, int32_t i)
{ return flags[i >> 4] >> ((i & 0xf) << 1) & 1; }
static AB_HT_INLINE int AB_ac_iseither(int32_t *flags, int32_t i)
{ return flags[i >> 4] >> ((i & 0xf) << 1) & 3; }

static AB_HT_INLINE void AB_ac_set_isempty_false(int32_t *flags, int32_t i)
{ flags[i >> 4] &= ~(2 << ((i & 0xf) << 1)); }
static AB_HT_INLINE void AB_ac_set_isboth_false(int32_t *flags, int32_t i)
{ flags[i >> 4] &= ~(3 << ((i & 0xf) << 1)); }
static AB_HT_INLINE void AB_ac_set_isdel_true(int32_t *flags, int32_t i)
{ flags[i >> 4] |= 1 << ((i & 0xf) << 1); }

static AB_HT_INLINE size_t AB_ac_fsize(int32_t v)
{ return v < 16 ? 1 : (size_t)(v >> 4); }

/******************
 * Utility functions
 ******************/
static AB_HT_INLINE int32_t AB_ht_roundup32(int32_t x) {
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}

/** @brief Function return values - interpreting errors
 * All error statuses are negative, so test for "< 0" for errors
 */
enum {
    AB_HT_NOERROR     =  0,  /**< No error                              */
    AB_HT_ENOMEM      = -1,  /**< Memory allocation returned NULL       */
    AB_HT_EINVAL      = -2,  /**< Invalid argument (ie. null hashtable) */
    AB_HT_ENOTPRESENT =  1,  /**< A slot is not present                 */
    AB_HT_EDELETED    =  2,  /**< A slot was deleted                    */
    AB_HT_ENOCHANGE   =  3   /**< A slot already existed                */
};

#if !defined(AB_HT_MALLOC) || !defined(AB_HT_REALLOC) || !defined(AB_HT_FREE)
# include <stdlib.h>
#endif

#ifndef AB_HT_MALLOC
/** @brief Generic memory allocation
 * @param size The size of the memory needed
 * @param userdata The hashtable's userdata, or NULL if no userdata
 * Pulls in stdlib.h if not defined
 */
# define AB_HT_MALLOC(size, userdata) malloc(size)
#endif /* AB_HT_MALLOC */

#ifndef AB_HT_REALLOC
/** @brief Reallocation function
 * @param ptr The original pointer
 * @param old_size The original size of the memory
 * @param new_size The new required memory size
 * @param userdata The hashtable's userdata, or NULL if no userdata
 * Pulls in stdlib.h if not defined
 * @note in some cases, @c old_size may under-represent the actual size of the previously allocated
 * memory. This can occur when a resize of a map-style hashtable successfully reallocates the
 * keys but fails the reallocation of the values.
 */
# define AB_HT_REALLOC(ptr, old_size, new_size, userdata) realloc(ptr, new_size)
#endif /* AB_HT_REALLOC */

#ifndef AB_HT_FREE
/** @brief Deallocation function
 * @param ptr The original pointer
 * @param size The memory size
 * @param userdata The hashtable's userdata, or NULL if no userdata
 * Pulls in stdlib.h if not defined
 */
# define AB_HT_FREE(ptr, size, userdata) free(ptr)
#endif /* AB_HT_FREE */

#ifndef AB_HT_HASH_UPPER
/** @brief The hashtable's load limit */
# define AB_HT_HASH_UPPER ((double)0.77)
#endif

/** @brief Define a type to handle the given key and value types
 * @param name The name of the type
 * @param key_type The type of keys stored
 * @param val_type The type of values stored
 * @hideinitializer
 */
#define AB_HT_DECLARE_TYPE(name, key_type, val_type)                                              \
    typedef struct {                                                                              \
        int32_t n_buckets, size, n_occupied, upper_bound;                                         \
        int32_t *flags;                                                                           \
        key_type *keys;                                                                           \
        val_type *vals;                                                                           \
    } name;
#define AB_HT_INITIALIZER { 0, 0, 0, 0, NULL, NULL, NULL }

/** @brief Prepare a hashtable for use
 * @param ht The hashtable
 * Either this function or AB_HT_INITIALIZER is needed before hashtable use
 * @hideinitializer
 */
#define AB_ht_init(ht) do {                                                                       \
    (ht)->n_buckets = (ht)->size = (ht)->n_occupied = (ht)->upper_bound = 0;                      \
    (ht)->flags = NULL;                                                                           \
    (ht)->keys = NULL;                                                                            \
    (ht)->vals = NULL;                                                                            \
} while (0)

/** @brief Free memory associated with the hashtable
 * @param ht The hashtable
 * @hideinitializer
 */
#define AB_ht_destroy(ht) do {                                                                    \
    if ((ht) != NULL) {                                                                           \
        AB_HT_FREE((ht)->keys, 0, NULL);                                                          \
        AB_HT_FREE((ht)->flags, 0, NULL);                                                         \
        AB_HT_FREE((ht)->vals, 0, NULL);                                                          \
    }                                                                                             \
} while (0)

/** @brief Empty the hashtable
 * @param ht The hashtable
 * @hideinitializer
 */
#define AB_ht_clear(ht) do {                                                                      \
    if (ht && ht->flags) {                                                                        \
        size_t flag_size = AB_ac_fsize(ht->n_buckets) * sizeof(int32_t);                          \
        memset(ht->flags, 0xaa, flag_size);                                                       \
        ht->size = ht->n_occupied = 0;                                                            \
    }                                                                                             \
} while (0)

/** @brief Delete the element at index idx
 * @param ht The hashtable
 * @param idx The handle to the hashtable element to delete
 * @return non-negative on success
 * Returns AB_HT_EINVAL on invalid index, AB_HT_ENOCHANGE if the index was deleted,
 * or AB_HT_NOERROR if the item was removed properly
 * @hideinitializer
 */
#define AB_ht_del(ht, idx)                                                                        \
    (((ht) == NULL || (idx) >= (ht)->n_buckets) ?                                                 \
         AB_HT_EINVAL                                                                             \
         : AB_ac_iseither((ht)->flags, (idx)) ?                                                   \
             AB_HT_ENOCHANGE                                                                      \
             : (AB_ac_set_isdel_true((ht)->flags, (idx)), --(ht)->size, AB_HT_NOERROR))

/** @brief Produces prototypes for the hashtable functions
 * @param name The postfix name for each routine
 * @param key_type The type of keys
 * @param val_type The type of values
 */
#define AB_HT_DECLARE_PROTOTYPES(name, key_type, val_type)                                        \
    extern int32_t AB_ht_get_##name(const name *ht, key_type key);                                \
    extern int32_t AB_ht_put_##name(name *ht, key_type key, int *ret);                            \
    extern int     AB_ht_resize_##name(name *ht, int32_t new_n_buckets);

/** @brief Produces definitions for the hashtable functions
 * @param name The postfix name for each routine
 * @param scope The decorator for each function, ie. static, extern, inline
 * @param key_type The type of keys
 * @param val_type The type of values
 * @param is_map true if the hashtable has values, false if only used as a set
 * @param hash_fn Hashing a key. Can be a macro
 * @param key_eq Comparing two keys for equality. Can be a macro
 * @hideinitializer
 */
#define AB_HT_DECLARE_IMPLEMENTATION(name, scope,                                                 \
        key_type, val_type, is_map, hash_fn, key_eq)                                              \
    scope int32_t AB_ht_get_##name(const name *ht, key_type key) {                                \
        if (ht == NULL || ht->n_buckets == 0) {                                                   \
            return AB_HT_EINVAL;                                                                  \
        } else {                                                                                  \
            int32_t mask = ht->n_buckets - 1;                                                     \
            int32_t k = hash_fn(key);                                                             \
            int32_t i = k & mask;                                                                 \
            int32_t last = i;                                                                     \
            int32_t step = 0;                                                                     \
            while (!AB_ac_isempty(ht->flags, i) &&                                                \
                    (AB_ac_isdel(ht->flags, i) || key_eq(ht->keys[i], key))) {                    \
                i = (i + (++step)) & mask;                                                        \
                if (i == last)                                                                    \
                    return ht->n_buckets;                                                         \
            }                                                                                     \
            return AB_ac_iseither(ht->flags, i) ? ht->n_buckets : i;                              \
        }                                                                                         \
    }                                                                                             \
    scope int32_t AB_ht_put_##name(name *ht, key_type key, int *ret) {                            \
        int32_t x;                                                                                \
        /* Test if hash table needs updating */                                                   \
        if (ht->n_occupied >= ht->upper_bound) {                                                  \
            /* clear the deleted items */                                                         \
            if (ht->n_buckets > (ht->size << 1)) {                                                \
                if (AB_ht_resize_##name(ht, ht->n_buckets - 1) < 0) {                             \
                    if (ret)                                                                      \
                        *ret = AB_HT_ENOMEM;                                                      \
                    return ht->n_buckets;                                                         \
                }                                                                                 \
            /* expand the hashtable */                                                            \
            } else if (AB_ht_resize_##name(ht, ht->n_buckets + 1) < 0) {                          \
                if (ret)                                                                          \
                    *ret = AB_HT_ENOMEM;                                                          \
                return ht->n_buckets;                                                             \
            }                                                                                     \
        }                                                                                         \
        {                                                                                         \
            int32_t k, i, site, last;                                                             \
            int32_t mask = ht->n_buckets - 1, step = 0;                                           \
            x = site = ht->n_buckets;                                                             \
            k = hash_fn(key);                                                                     \
            i = k & mask;                                                                         \
            if (AB_ac_isempty(ht->flags, i)) {                                                    \
                x = i;                                                                            \
            } else {                                                                              \
                last = i;                                                                         \
                while (!AB_ac_isempty(ht->flags, i) &&                                            \
                        (AB_ac_isdel(ht->flags, i) || !key_eq(ht->keys[i], key))) {               \
                    if (AB_ac_isdel(ht->flags, i))                                                \
                        site = i;                                                                 \
                    i = (i + (++step)) & mask;                                                    \
                    if (i == last) {                                                              \
                        x = site;                                                                 \
                        break;                                                                    \
                    }                                                                             \
                }                                                                                 \
                if (x == ht->n_buckets) {                                                         \
                    if (AB_ac_isempty(ht->flags, i) && site != ht->n_buckets)                     \
                        x = site;                                                                 \
                    else                                                                          \
                        x = i;                                                                    \
                }                                                                                 \
            }                                                                                     \
        }                                                                                         \
        if (AB_ac_isempty(ht->flags, x)) { /* Element is not present */                           \
            ht->keys[x] = key;                                                                    \
            AB_ac_set_isboth_false(ht->flags, x);                                                 \
            ++ht->size;                                                                           \
            ++ht->n_occupied;                                                                     \
            if (ret)                                                                              \
                *ret = AB_HT_ENOTPRESENT;                                                         \
        } else if (AB_ac_isdel(ht->flags, x)) { /* Element was deleted */                         \
            ht->keys[x] = key;                                                                    \
            AB_ac_set_isboth_false(ht->flags, x);                                                 \
            ++ht->size;                                                                           \
            if (ret)                                                                              \
                *ret = AB_HT_EDELETED;                                                            \
        } else { /* Dont change anything if present and not deleted */                            \
            if (ret)                                                                              \
                *ret = AB_HT_ENOCHANGE;                                                           \
        }                                                                                         \
        return x;                                                                                 \
    }                                                                                             \
    scope int AB_ht_resize_##name(name *ht, int32_t new_n_buckets) {                              \
        int32_t *new_flags = NULL;                                                                \
        size_t new_flags_size;                                                                    \
        {                                                                                         \
            new_n_buckets = AB_MAX(4, AB_ht_roundup32(new_n_buckets));                               \
                                                                                                  \
            if (ht->size >= new_n_buckets * AB_HT_HASH_UPPER + 0.5)                               \
                return AB_HT_ENOCHANGE; /* Requested size is too small */                         \
                                                                                                  \
            new_flags_size = AB_ac_fsize(new_n_buckets) * sizeof(int32_t);                        \
            new_flags = AB_HT_MALLOC(new_flags_size, NULL);                                       \
            if (!new_flags)                                                                       \
                return AB_HT_ENOMEM;                                                              \
            memset(new_flags, 0xaa, new_flags_size);                                              \
                                                                                                  \
            if (ht->n_buckets < new_n_buckets) { /* We need to expand */                          \
                key_type *new_keys = AB_HT_REALLOC(ht->keys,                                      \
                        (size_t)ht->n_buckets * sizeof(key_type),                                 \
                        (size_t)new_n_buckets * sizeof(key_type), NULL);                          \
                if (new_keys == NULL) {                                                           \
                    AB_HT_FREE(new_flags, new_flags_size, NULL);                                  \
                    return AB_HT_ENOMEM;                                                          \
                }                                                                                 \
                ht->keys = new_keys;                                                              \
                if (is_map) {                                                                     \
                    val_type *new_vals = AB_HT_REALLOC(ht->vals,                                  \
                            (size_t)ht->n_buckets * sizeof(val_type),                             \
                            (size_t)new_n_buckets * sizeof(val_type), NULL);                      \
                    if (new_vals == NULL) {                                                       \
                        AB_HT_FREE(new_vals, new_flags_size, NULL);                               \
                        return AB_HT_ENOMEM;                                                      \
                    }                                                                             \
                    ht->vals = new_vals;                                                          \
                }                                                                                 \
            }                                                                                     \
        }                                                                                         \
        {   /* Rehash */                                                                          \
            int32_t j;                                                                            \
            int32_t new_mask = new_n_buckets - 1;                                                 \
            for (j = 0; j != ht->n_buckets; j++) {                                                \
                if (AB_ac_iseither(ht->flags, j) == 0) {                                          \
                    key_type key = ht->keys[j];                                                   \
                    val_type val;                                                                 \
                    if (is_map)                                                                   \
                        val = ht->vals[j];                                                        \
                    AB_ac_set_isdel_true(ht->flags, j);                                           \
                    while (1) {                                                                   \
                        int32_t k = hash_fn(key);                                                 \
                        int32_t i = k & new_mask;                                                 \
                        int32_t step = 0;                                                         \
                        while (!AB_ac_isempty(new_flags, i))                                      \
                            i = (i + (++step)) & new_mask;                                        \
                        AB_ac_set_isempty_false(new_flags, i);                                    \
                        if (i < ht->n_buckets &&                                                  \
                                !AB_ac_iseither(ht->flags, i)) {                                  \
                            key_type tmp = ht->keys[i];                                           \
                            ht->keys[i] = key;                                                    \
                            key = tmp;                                                            \
                            if (is_map) {                                                         \
                                val_type vtmp = ht->vals[i];                                      \
                                ht->vals[i] = val;                                                \
                                val = vtmp;                                                       \
                            }                                                                     \
                            AB_ac_set_isdel_true(ht->flags, i);                                   \
                        } else {                                                                  \
                            ht->keys[i] = key;                                                    \
                            if (is_map)                                                           \
                                ht->vals[i] = val;                                                \
                            break;                                                                \
                        }                                                                         \
                    }                                                                             \
                }                                                                                 \
            }                                                                                     \
            if (ht->n_buckets > new_n_buckets) { /* shrink the hash table */                      \
                ht->keys = AB_HT_REALLOC(ht->keys,                                                \
                        (size_t)ht->n_buckets * sizeof(key_type),                                 \
                        (size_t)new_n_buckets * sizeof(key_type), NULL);                          \
                if (is_map)                                                                       \
                    ht->vals = AB_HT_REALLOC(ht->vals,                                            \
                            (size_t)ht->n_buckets * sizeof(val_type),                             \
                            (size_t)new_n_buckets * sizeof(val_type), NULL);                      \
            }                                                                                     \
            AB_HT_FREE(ht->flags, new_flags_size, NULL);                                          \
            ht->flags = new_flags;                                                                \
            ht->n_buckets = new_n_buckets;                                                        \
            ht->n_occupied = ht->size;                                                            \
            ht->upper_bound = (int32_t)(ht->n_buckets * AB_HT_HASH_UPPER + 0.5);                  \
        }                                                                                         \
        return 0;                                                                                 \
    }

/** @brief Determine the index for a given key
 * @param name The name of the routines
 * @param ht The hashtable
 * @param key The key
 * @return The index of the key
 * @hideinitializer
 */
#define AB_ht_get(name, ht, key) AB_ht_get_##name(ht, key)

/** @brief Change the size of the hashtable
 * @param name The name of the routines
 * @param ht The hashtable
 * @param new_n_buckets The new number of buckets
 * @hideinitializer
 */
#define AB_ht_resize(name, ht, new_n_buckets) AB_ht_resize_##name(ht, new_n_buckets)

/** @brief Determine the index for a given key
 * @param name The name of the routines
 * @param ht The hashtable
 * @param key The key
 * @param ret Pointer to where the return value will be placed
 * @return The index of the inserted key
 * @hideinitializer
 */
#define AB_ht_put(name, ht, key, ret) AB_ht_put_##name(ht, key, ret)

/** @brief Determine if the entry at the given index exists
 * @param ht The hashtable
 * @param idx The index
 * @hideinitializer
 */
#define AB_ht_exists(ht, idx) (!AB_ac_iseither((ht)->flags, (idx)))
/** @brief Access the key value at the given index
 * @param ht The hashtable
 * @param idx The index
 */
#define AB_ht_key(ht, idx)    ((ht)->keys[idx])
/** @brief Access the value at the given index
 * @param ht The hashtable
 * @param idx The index
 */
#define AB_ht_val(ht, idx)    ((ht)->vals[idx])

/** @cond false */
#define AB_ht_begin(ht)       ((int32_t)0)
#define AB_ht_end(ht)         ((ht)->n_buckets)
/** @endcond */

/** @brief Iterate over valid hashtable indexes
 * @param k The iterator, @c int32_t
 * @param ht The hashtable
 */
#define AB_ht_foreach(k, ht) \
    for ((k) = AB_ht_begin(ht); \
            (k) != AB_ht_end(ht); \
            (k)++) \
        if (!AB_ht_exists(ht, k)) \
            (void)0; \
        else

#endif /* AMBER_UTIL_HASHTABLE_H */
