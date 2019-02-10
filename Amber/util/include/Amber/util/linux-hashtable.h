#ifndef AMBER_UTIL_LINUX_HASHTABLE_H
#define AMBER_UTIL_LINUX_HASHTABLE_H

#include <Amber/util/common.h>
#include <Amber/util/hlist.h>
#include <stddef.h>


/* Fixed-size hashtable with chaining */

/* Initialization */
#define AB_HASHTABLE_DEFINE(name, bits) \
    struct AB_hlist_head name[1 << (bits)] = { 0 }

#define AB_HASHTABLE_DECLARE(name, bits) \
    struct AB_hlist_head name[1 << (bits)]

#define AB_hash_init(ht)

/* Predicates */
static inline int AB_hash_hashed(struct AB_hlist_node *node);
#define AB_hash_empty(ht)

/* Operations */
#define AB_hash_add(ht, node, hash)
static inline void AB_hash_del(struct AB_hlist_node *node);

/* Iteration */
/* ht - hashtable (not a pointer!!)
 * bkt - unsigned int
 * obj - type *
 * type - type
 * member - type.member == struct AB_hlist_node *
 */
#define AB_hash_foreach(ht, bkt, obj, type, member)
/* tmp - hlist_node * */
#define AB_hash_foreach_safe(ht, bkt, tmp, obj, type, member)

#define AB_hash_foreach_possible(ht, obj, type, member, hash)
#define AB_hash_foreach_possible_safe(ht, obj, tmp, type, member, hash)

/***************************
 * Implementation
 **************************/

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
        AB_hlist_foreach_entry_safe(obj, tmp, &ht[bkt], type, member)

#undef AB_hash_foreach_possible
#define AB_hash_foreach_possible(ht, obj, type, member, hash) \
    AB_hlist_foreach_entry(obj, &(ht)[(hash) % AB_HASH_SIZE(ht)], type, member)

#undef AB_hash_foreach_possible_safe
#define AB_hash_foreach_possible_safe(ht, obj, tmp, type, member, hash) \
    AB_hlist_foreach_entry_safe(obj, tmp, &(ht)[(hash) % AB_HASH_SIZE(ht)], type, member)

#endif /* AMBER_UTIL_LINUX_HASHTABLE_H */
