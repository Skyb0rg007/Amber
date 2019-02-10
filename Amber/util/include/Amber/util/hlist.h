#ifndef AMBER_UTIL_HLIST_H
#define AMBER_UTIL_HLIST_H

#include <Amber/util/common.h>
#include <stddef.h>

/* Doubly-linked lists with a single-pointer list head */

struct AB_hlist_head; /* The list head */
struct AB_hlist_node; /* A list item, embed in struct */

/* Initialization */
#define AB_HLIST_HEAD_INITIALIZER { NULL }
static AB_INLINE void AB_hlist_head_init(struct AB_hlist_head *head);
static AB_INLINE void AB_hlist_node_init(struct AB_hlist_node *node);

/* Access entry from AB_hlist_node */
#define AB_hlist_entry(ptr, type, member)
/* Returns NULL if ptr is NULL */
#define AB_hlist_entry_safe(ptr, type, member)

/* Predicates */
static AB_INLINE int AB_hlist_unhashed(const struct AB_hlist_node *node);
static AB_INLINE int AB_hlist_empty(const struct AB_hlist_head *node);
static AB_INLINE int
AB_hlist_is_singlular_node(struct AB_hlist_node *node, struct AB_hlist_head *head);

/* Operations */
static AB_INLINE void AB_hlist_del(struct AB_hlist_node *node);
static AB_INLINE void AB_hlist_add_head(struct AB_hlist_node *node, struct AB_hlist_head *head);
static AB_INLINE void AB_hlist_add_before(struct AB_hlist_node *node, struct AB_hlist_node *next);
static AB_INLINE void AB_hlist_add_behind(struct AB_hlist_node *node, struct AB_hlist_node *prev);
static AB_INLINE void AB_hlist_move_list(struct AB_hlist_head *src, struct AB_hlist_head *dest);

/* Fakes */
static AB_INLINE void AB_hlist_add_fake(struct AB_hlist_node *node);
static AB_INLINE int AB_hlist_fake(struct AB_hlist_node *node);

/* Iteration */
/* pos  - hlist_node *
 * head - hlist_head *
 * tmp  - hlist_node *
 */
#define AB_hlist_foreach(pos, head)
/* *_safe - can delete the current position without worry */
#define AB_hlist_foreach_safe(pos, tmp, head)
/* *_entry - iterate over entries
 * pos - type * 
 */
#define AB_hlist_foreach_entry(pos, head, type, member)
/* Continue from where you left off (the next one after pos) */
#define AB_hlist_foreach_entry_continue(pos, type, member)
/* Continue from the current position (pos) */
#define AB_hlist_foreach_entry_from(pos, type, member) 

#define AB_hlist_foreach_entry_safe(pos, tmp, head, type, member)

/*****************************
 * Implementation
 ****************************/

struct AB_hlist_head {
    struct AB_hlist_node *first;
};

struct AB_hlist_node {
    struct AB_hlist_node *next, **pprev;
};

static AB_INLINE void AB_hlist_head_init(struct AB_hlist_head *head) {
    head->first = NULL;
}
static AB_INLINE void AB_hlist_node_init(struct AB_hlist_node *node) {
    node->next  = NULL;
    node->pprev = NULL;
}

#ifndef container_of
# define container_of(ptr, type, member) \
    (type *)((char *)(ptr) - offsetof(type, member))
#endif
#undef AB_hlist_entry
#define AB_hlist_entry(ptr, type, member) container_of(ptr, type, member)

static AB_INLINE int AB_hlist_unhashed(const struct AB_hlist_node *node) {
    return node->pprev == NULL;
}

static AB_INLINE int AB_hlist_empty(const struct AB_hlist_head *node)
{
    return node->first == NULL;
}

static AB_INLINE void AB_hlist_del(struct AB_hlist_node *node) {
    struct AB_hlist_node *next = node->next;
    struct AB_hlist_node **pprev = node->pprev;

    *pprev = next;

    if (next)
        next->pprev = pprev;

    node->next = NULL;
    node->pprev = NULL;
}

static AB_INLINE void AB_hlist_add_head(
        struct AB_hlist_node *node, struct AB_hlist_head *head) {
    struct AB_hlist_node *first = head->first;
    node->next = first;
    if (first)
        first->pprev = &node->next;
    head->first = node;
    node->pprev = &head->first;
}

static AB_INLINE void AB_hlist_add_before(
        struct AB_hlist_node *node, struct AB_hlist_node *next) {
    node->pprev = next->pprev;
    node->next = next;
    next->pprev = &node->next;
    *(node->pprev) = node;
}

static AB_INLINE void AB_hlist_add_behind(
        struct AB_hlist_node *node, struct AB_hlist_node *prev) {
    node->next = prev;
    prev->next = node;
    node->pprev = &prev->next;

    if (node->next)
        node->next->pprev = &node->next;
}

static AB_INLINE void AB_hlist_add_fake(struct AB_hlist_node *node) {
    node->pprev = &node->next;
}

static AB_INLINE int AB_hlist_fake(struct AB_hlist_node *node) {
    return node->pprev == &node->next;
}

static AB_INLINE int AB_hlist_is_singlular_node(
        struct AB_hlist_node *node, struct AB_hlist_head *head) {
    return !node->next && node->pprev == &head->first;
}

static AB_INLINE void AB_hlist_move_list(
        struct AB_hlist_head *src, struct AB_hlist_head *dest) {
    dest->first = src->first;
    if (dest->first)
        dest->first->pprev = &dest->first;
    dest->first = NULL;
}

#undef AB_hlist_foreach
#define AB_hlist_foreach(pos, head) \
    for ((pos) = (head)->first; (pos) != NULL; (pos) = (pos)->next)

#undef AB_hlist_foreach_safe
#define AB_hlist_foreach_safe(pos, tmp, head) \
    for ((pos) = (head)->first; (pos) && ((tmp) = (pos)->next, 1); (pos) = (tmp))

#undef AB_hlist_entry_safe
#define AB_hlist_entry_safe(ptr, type, member) \
    ((ptr) ? AB_hlist_entry(ptr, type, member) : NULL)

#undef AB_hlist_foreach_entry
#define AB_hlist_foreach_entry(pos, head, type, member) \
    for ((pos) = AB_hlist_entry_safe((head)->first, type, member); \
            (pos) != NULL; \
            (pos) = AB_hlist_entry_safe((pos)->member.next, type, member))

#undef AB_hlist_foreach_entry_continue
#define AB_hlist_foreach_entry_continue(pos, type, member) \
    for ((pos) = AB_hlist_entry_safe((pos)->member.next, type, member); \
            (pos) != NULL; \
            (pos) = AB_hlist_entry_safe((pos)->member.next, type, member))

#undef AB_hlist_foreach_entry_from
#define AB_hlist_foreach_entry_from(pos, type, member) \
    for (; (pos) != NULL; \
            (pos) = AB_hlist_entry_safe((pos)->member.next, type, member))

#undef AB_hlist_foreach_entry_safe
#define AB_hlist_foreach_entry_safe(pos, tmp, head, type, member) \
    for ((pos) = AB_hlist_entry_safe((head)->first, type, member); \
            (pos) != NULL && ((tmp) = (pos)->member.next, 1); \
            (pos) = AB_hlist_entry_safe(tmp, type, member))

#endif
