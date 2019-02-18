/** 
 * @file hlist.h
 * @brief Doubly-linked lists with singly-linked heads
 *
 * This specific data structure is mainly used for implementing
 * chained hashtables, see linux-hashtable.h
 *
 */
#ifndef AMBER_UTIL_HLIST_H
#define AMBER_UTIL_HLIST_H

#include <Amber/util/common.h>
#include <stddef.h>

/** @brief The list head */
struct AB_hlist_head;
/** @brief A list item, embed in a struct */
struct AB_hlist_node; 

/* Initialization */
/** @brief Initializer for a struct AB_hlist_head */
#define AB_HLIST_HEAD_INITIALIZER { NULL }
/** @brief Initialize a struct AB_hlist_head */
static AB_INLINE void AB_hlist_head_init(struct AB_hlist_head *head);
/** @brief Initialize a struct AB_hlist_node */
static AB_INLINE void AB_hlist_node_init(struct AB_hlist_node *node);

/** @brief Access an entry from an AB_hlist_node 
 * @param ptr Pointer to the AB_hlist_node
 * @param type The type of the structure that embedded the AB_hlist_node
 * @param member The name of the AB_hlist_node in the structure
 * @return Pointer of type @p type * to the container of the AB_hlist_node
 * @warning This macro evaluates @p ptr multiple times
 * @hideinitializer
 */
#define AB_hlist_entry(ptr, type, member)

/** @brief Safely access an entry from a AB_hlist_node
 * @param ptr Pointer to the AB_hlist_node
 * @param type The type of the structure that embedded the AB_hlist_node
 * @param member The name of the AB_hlist_node in the structure
 * @return Pointer of type @p type * to the container of the AB_hlist_node, 
 *   or @c NULL if @p ptr is @c NULL
 * @warning This macro evaluates @p ptr multiple times
 * @hideinitializer
 */
#define AB_hlist_entry_safe(ptr, type, member)

/* Predicates */
/** @brief Determine if a AB_hlist_node has been inserted into a list */
static AB_INLINE int AB_hlist_unhashed(const struct AB_hlist_node *node);
/** @brief Determine if the AB_hlist_head is an empty list */
static AB_INLINE int AB_hlist_empty(const struct AB_hlist_head *node);
/** @brief Determine if the AB_hlist_head only contains a single item */
static AB_INLINE int
AB_hlist_is_singlular_node(struct AB_hlist_node *node, struct AB_hlist_head *head);

/* Operations */
/** @brief Remove a node from its list */
static AB_INLINE void AB_hlist_del(struct AB_hlist_node *node);
/** @brief Add a node to the head of a list */
static AB_INLINE void AB_hlist_add_head(struct AB_hlist_node *node, struct AB_hlist_head *head);
/** @brief Insert a node in front of another node */
static AB_INLINE void AB_hlist_add_before(struct AB_hlist_node *node, struct AB_hlist_node *next);
/** @brief Insert a node behind another node */
static AB_INLINE void AB_hlist_add_behind(struct AB_hlist_node *node, struct AB_hlist_node *prev);
/** @brief Transfer the entire list from one head to another */
static AB_INLINE void AB_hlist_move_list(struct AB_hlist_head *src, struct AB_hlist_head *dest);

/* Fakes */
/** @brief Create a fake list containing only the one node 
 * @param node The node to turn into a fake
 *
 * Fakes provide the ability to make insertions and deletions without a parent
 * AB_hlist_head.
 */
static AB_INLINE void AB_hlist_add_fake(struct AB_hlist_node *node);
/** @brief Determine if a node is a fake */
static AB_INLINE int AB_hlist_fake(struct AB_hlist_node *node);

/* Iteration */
/** @brief Iterate over an AB_hlist_head
 * @param pos A struct AB_hlist_node *, used as iteration pointer
 * @param head A struct AB_hlist_head *, the list to iterate over
 * @warning This macro evaluates @p pos multiple times
 * @hideinitializer
 */
#define AB_hlist_foreach(pos, head)

/** @brief Safely iterate over an AB_hlist_head
 * @param pos A struct AB_hlist_node *, used as iteration pointer
 * @param tmp A struct AB_hlist_node *, used as a temporary storage
 * @param head A struct AB_hlist_head *, the list to iterate over
 * @note Use this macro if you want to delete the current node
 * @warning This macro evaluates @p pos and @p tmp multiple times
 * @hideinitializer
 */
#define AB_hlist_foreach_safe(pos, tmp, head)

/** @brief Iterate over the entries in an AB_hlist_head
 * @param pos A @p type *, used as iteration pointer
 * @param head A struct AB_hlist_head *, the list to iterate over
 * @param type The type of the entry in the list
 * @param member The name of the AB_hlist_node in the @p type
 * @warning This macro evaluates @p pos multiple times
 * @hideinitializer
 */
#define AB_hlist_foreach_entry(pos, head, type, member)

/** @brief Iterate over an AB_hlist_head starting one after @p pos
 * @param pos A @p type *, used as iteration pointer
 * @param type The type of the entry in the list
 * @param member The name of the AB_hlist_node in the @p type
 * @warning This macro evaluates @p pos multiple times
 * @hideinitializer
 */
#define AB_hlist_foreach_entry_continue(pos, type, member)

/** @brief Iterate over an AB_hlist_head starting from @p pos
 * @param pos A @p type *, used as iteration pointer
 * @param type The type of the entry in the list
 * @param member The name of the AB_hlist_node in the @p type
 * @warning This macro evaluates @p pos multiple times
 * @hideinitializer
 */
#define AB_hlist_foreach_entry_from(pos, type, member) 

/** @brief Safely iterate over the entries in an AB_hlist_head
 * @param pos A @p type *, used as iteration pointer
 * @param tmp A struct AB_hlist_node *, used as a temporary storage
 * @param head A struct AB_hlist_head *, the list to iterate over
 * @param type The type of the entry in the list
 * @param member The name of the AB_hlist_node in the @p type
 * @warning This macro evaluates @p pos and @p tmp multiple times
 * @hideinitializer
 */
#define AB_hlist_foreach_entry_safe(pos, tmp, head, type, member)

/*****************************
 * Implementation
 ****************************/
#if !defined(__DOXYGEN__)

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

#endif /* __DOXYGEN__ */
#endif /* AMBER_UTIL_HLIST_H */
