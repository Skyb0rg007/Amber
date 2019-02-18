/**
 * @file hashtable.h
 * @brief A fixed-size hashtable with chaining
 *
 * This implementation was taken from linux/hashtable.h in the Linux kernel.
 */
#ifndef AMBER_UTIL_LINUX_HASHTABLE_H
#define AMBER_UTIL_LINUX_HASHTABLE_H

#include <Amber/util/common.h>
#include <Amber/util/hlist.h>
#include <stddef.h>
#include <stdint.h>

/* Initialization */
/** @brief Hashtable static initializer */
#define AB_HASHTABLE_INITIALIZER { 0 }

/** @brief Declare an array type representing a hashtable
 * @param name The name of the array
 * @param bits The number of bits in the array
 */
#define AB_HASHTABLE_DECLARE(name, bits) \
    struct AB_hlist_head name[1 << (bits)]

/** @brief Initialize a hashtable */
#define AB_hash_init(ht)

/* Predicates */
/** @brief Determine if a node has been hashed */
static inline int AB_hash_hashed(struct AB_hlist_node *node);
/** @brief Determine if a hashtable is empty */
#define AB_hash_empty(ht)

/* Operations */
/** @brief Add a node to the hashtable
 * @param ht The hashtable
 * @param node Pointer to the struct AB_hlist_node member of the inserted structure
 * @param hash The structure key's hash value
 */
#define AB_hash_add(ht, node, hash)
/** @brief Delete a node from the hashtable */
static inline void AB_hash_del(struct AB_hlist_node *node);

/* Iteration */
/* ht - hashtable (not a pointer!!)
 * bkt - unsigned int
 * obj - type *
 * type - type
 * member - type.member == struct AB_hlist_node *
 */
/** @brief Iterate over a hashtable
 * @param ht The hashtable to iterate over
 * @param bkt unsigned int, the current bucket number
 * @param obj @p type *, the iteration pointer
 * @param type The type of the hashtable's members
 * @param member The name of the hashtable member within the structure
 * @warning This macro evaluates @p obj and @p bkt multiple times
 * @hideinitializer
 */
#define AB_hash_foreach(ht, bkt, obj, type, member)

/** @brief Safely iterate over a hashtable
 * @param ht The hashtable to iterate over
 * @param bkt unsigned int, the current bucket number
 * @param tmp struct AB_hlist_node *, the temporary variable
 * @param obj @p type *, the iteration pointer
 * @param type The type of the hashtable's members
 * @param member The name of the hashtable member within the structure
 * @warning This macro evaluates @p obj, @p bkt, and @p tmp multiple times
 * @hideinitializer
 */
#define AB_hash_foreach_safe(ht, bkt, tmp, obj, type, member)

/** @brief Iterate over the elements with a matching hash value
 * @param ht The hashtable to iterate over
 * @param obj @p type *, the iteration pointer
 * @param type The type of the hashtable's members
 * @param member The name of the hashtable member within the structure
 * @param hash The hash value to look for matching elements
 * @warning This macro evaluates @p obj multiple times
 * @hideinitializer
 */
#define AB_hash_foreach_possible(ht, obj, type, member, hash)

/** @brief Safely iterate over the elements with a matching hash value
 * @param ht The hashtable to iterate over
 * @param obj @p type *, the iteration pointer
 * @param tmp struct AB_hlist_node *, the temporary variable
 * @param type The type of the hashtable's members
 * @param member The name of the hashtable member within the structure
 * @param hash The hash value to look for matching elements
 * @warning This macro evaluates @p obj and @p tmp multiple times
 * @hideinitializer
 */
#define AB_hash_foreach_possible_safe(ht, obj, tmp, type, member, hash)

/* Hashing functions */
/** @brief Hash a null-terminated string
 * @param str The input string
 * @return The string's hash value
 * @note This uses FNVa-1, which is not cryptographically secure
 */
static inline uint32_t AB_fnv_hash_str(const char *str);

/** @brief Hash a 32-bit value */
static inline uint32_t AB_hash32(uint32_t val);
/** @brief Hash a 64-bit value */
static inline uint32_t AB_hash64(uint64_t val);

/***************************
 * Implementation
 **************************/
#if !defined(__DOXYGEN__)

#define AB_HASH_SIZE(ht) AB_ARRAY_SIZE(ht)

#undef AB_hash_init
static inline void AB_hash_init_internal(struct AB_hlist_head *ht, unsigned sz) {
    unsigned i;
    for (i = 0; i < sz; i++)
        AB_hlist_head_init(ht + i);
}
#define AB_hash_init(ht) AB_hash_init_internal(ht, AB_HASH_SIZE(ht))

#undef AB_hash_add
#define AB_hash_add(ht, node, hash) \
    AB_hlist_add_head(node, &(ht)[(hash) % AB_HASH_SIZE(ht)])

static inline int AB_hash_hashed(struct AB_hlist_node *node) {
    return !AB_hlist_unhashed(node);
}

#undef AB_hash_empty
static inline int AB_hash_empty_internal(struct AB_hlist_head *ht, unsigned sz) {
    unsigned i;
    for (i = 0; i < sz; i++)
        if (!AB_hlist_empty(ht + i))
            return 0;
    return 1;
}
#define AB_hash_empty(ht) AB_hash_empty_internal(ht, AB_HASH_SIZE(ht))

static inline void AB_hash_del(struct AB_hlist_node *node) {
    AB_hlist_del(node);
}

#undef AB_hash_foreach
#define AB_hash_foreach(ht, bkt, obj, type, member) \
    for ((bkt) = 0, (obj) = NULL; \
            (obj) == NULL && (bkt) < AB_HASH_SIZE(ht); \
            (bkt)++) \
        AB_hlist_foreach_entry(obj, &(ht)[bkt], type, member)

#undef AB_hash_foreach_safe
#define AB_hash_foreach_safe(ht, bkt, tmp, obj, type, member) \
    for ((bkt) = 0, (obj) = NULL; \
            (obj) == NULL && (bkt) < AB_HASH_SIZE(ht); \
            (bkt)++) \
        AB_hlist_foreach_entry_safe(obj, tmp, &(ht)[bkt], type, member)

#undef AB_hash_foreach_possible
#define AB_hash_foreach_possible(ht, obj, type, member, hash) \
    AB_hlist_foreach_entry(obj, &(ht)[(hash) % AB_HASH_SIZE(ht)], type, member)

#undef AB_hash_foreach_possible_safe
#define AB_hash_foreach_possible_safe(ht, obj, tmp, type, member, hash) \
    AB_hlist_foreach_entry_safe(obj, tmp, &(ht)[(hash) % AB_HASH_SIZE(ht)], type, member)

static inline uint32_t AB_fnv_hash_str(const char *str) {
    const uint32_t FNV_PRIME = 16777619u;
    const uint32_t FNV_OFFSET_BASIS = 2166136261u;

    uint32_t hash = FNV_OFFSET_BASIS;
    while (*str) {
        hash ^= *str++;
        hash *= FNV_PRIME;
    }
    return hash;
}
static inline uint32_t AB_hash32(uint32_t val) {
    const uint32_t GOLDEN_RATIO_32 = 0x61C88647;
    return val * GOLDEN_RATIO_32;
}
static inline uint32_t AB_hash64(uint64_t val) {
    const uint64_t GOLDEN_RATIO_64 = 0x61C8864680B583EB;
    return val * GOLDEN_RATIO_64;
}

#endif /* __DOXYGEN__ */
#endif /* AMBER_UTIL_LINUX_HASHTABLE_H */
