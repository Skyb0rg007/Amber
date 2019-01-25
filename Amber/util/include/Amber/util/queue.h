#ifndef AMBER_UTIL_QUEUE_H
#define AMBER_UTIL_QUEUE_H

#error "not complete!"

#include <SDL_atomic.h>

/**************************
 * Singly-linked-lists
 *************************/
#define AB_SLIST_HEAD(name, type) \
    struct name { type *cslh_first; }

#define AB_SLIST_HEAD_INITIALIZER \
    { NULL }

#define AB_SLIST_ENTRY(type) \
    struct { type *csle_next; }

#define AB_SLIST_EMPTY(head) \
    (SDL_AtomicGetPtr(&(head)->cslh_first) == NULL)

#define AB_SLIST_FIRST(head) \
    (SDL_AtomicGetPtr(&(head)->cslh_first))

#define AB_SLIST_NEXT(elem, field) \
    SDL_AtomicGetPtr(&((elem)->field.csle_next))

#define AB_SLIST_FOREACH(var, head, field)   \
    for ((var) = AB_SLIST_FIRST((head));     \
         (var) != NULL;                      \
         (var) = AB_SLIST_NEXT((var), field))

#define AB_SLIST_FOREACH_SAFE(var, head, field, tvar)      \
    for ((var) = AB_SLIST_FIRST((head));                   \
         (var) && ((tvar) = AB_SLIST_NEXT(var, field), 1); \
         (var) = (tvar))

#define AB_SLIST_FOREACH_PREVPTR(var, varp, head, field) \
    for ((varp) = &(head)->cslh_first;                   \
         ((var) = SDL_AtomicGetPtr(varp)) != NULL;       \
         (varp) = &(var)->field.csle_next)

#define AB_SLIST_INIT(head) \
    SDL_AtomicSetPtr(&(head)->cslh_first, NULL)

#define AB_SLIST_INSERT_AFTER(a, b, field) do { \
    (b)->field.csle_next = (a)->field.csle_next; }

/***********************************
 * Singly-linked tail-queue
 **********************************/
/** @brief define a singly-linked tail queue struct with the name and type
 * @param name The tag for the new struct
 * @param type The tag for the element type (a struct)
 */
#define AB_STAILQ_HEAD(name, type)                           \
    struct name {                                            \
        struct type *cstqh_first; /* first element */        \
        struct type **cstqh_last; /* addr of last element */ \
    }

#define AB_STAILQ_HEAD_INITIALIZER(head) \
    { NULL, &(head).cstqh_first }

/** @brief Defines the embedded entry type for a tail-queue structure
 * @param type The tag of the structure we point to
 */
#define AB_STAILQ_ENTRY(type) \
    struct { struct type *cstqe_next; }

#define AB_STAILQ_CONCAT(head1, head2) do {                           \
    if (SDL_AtomicGetPtr((void **)&(head2)->cstqh_first) != NULL) {   \
        SDL_AtomicStorePtr((head1)->cstq_last, (head2)->cstqh_first); \
        SDL_AtomicStorePtr((head1)->cstq_last, (head2)->cstq_last);   \
        AB_STAILQ_INIT(head2);                                        \
    }                                                                 \
} while (0)

#define AB_STAILQ_EMPTY(head) (SDL_AtomicGetPtr(&(head)->cstqh_first) == NULL)

#define AB_STAILQ_FIRST(head) SDL_AtomicGetPtr(&(head)->cstqh_first)

#define AB_STAILQ_FOREACH(var, head, field)    \
    for ((var) = AB_STAILQ_FIRST((head));      \
         SDL_AtomicGetPtr((var)) != NULL;      \
         (var) = AB_STAILQ_NEXT((var), field))

#define AB_STAILQ_FOREACH_SAFE(var, head, field, tvar)            \
	for ((var) = AB_STAILQ_FIRST((head));                         \
	    SDL_AtomicGetPtr((var)) != NULL &&  \
            ((tvar) = AB_STAILQ_NEXT((var), field), 1); \
	    (var) = (tvar))

#define	AB_STAILQ_INIT(head) do {                                 \
	ck_pr_store_ptr(&(head)->cstqh_first, NULL);                  \
	ck_pr_fence_store();                                          \
	(head)->cstqh_last = &(head)->cstqh_first;                    \
} while (0)

#define	AB_STAILQ_INSERT_AFTER(head, tqelm, elm, field) do {      \
	(elm)->field.cstqe_next = (tqelm)->field.cstqe_next;          \
	ck_pr_fence_store();                                          \
	ck_pr_store_ptr(&(tqelm)->field.cstqe_next, elm);             \
	if ((elm)->field.cstqe_next == NULL)                          \
		(head)->cstqh_last = &(elm)->field.cstqe_next;            \
} while (0)

#define	AB_STAILQ_INSERT_HEAD(head, elm, field) do {              \
	(elm)->field.cstqe_next = (head)->cstqh_first;                \
	ck_pr_fence_store();                                          \
	ck_pr_store_ptr(&(head)->cstqh_first, elm);                   \
	if ((elm)->field.cstqe_next == NULL)                          \
		(head)->cstqh_last = &(elm)->field.cstqe_next;            \
} while (0)

#define	AB_STAILQ_INSERT_TAIL(head, elm, field) do {              \
	(elm)->field.cstqe_next = NULL;                               \
	ck_pr_fence_store();                                          \
	ck_pr_store_ptr((head)->cstqh_last, (elm));                   \
	(head)->cstqh_last = &(elm)->field.cstqe_next;                \
} while (0)

#define	AB_STAILQ_NEXT(elm, field)                                \
	(ck_pr_load_ptr(&(elm)->field.cstqe_next))

#define	AB_STAILQ_REMOVE(head, elm, type, field) do {             \
	if ((head)->cstqh_first == (elm)) {                           \
		AB_STAILQ_REMOVE_HEAD((head), field);                     \
	} else {                                                      \
		struct type *curelm = (head)->cstqh_first;                \
		while (curelm->field.cstqe_next != (elm))                 \
			curelm = curelm->field.cstqe_next;                    \
		AB_STAILQ_REMOVE_AFTER(head, curelm, field);              \
	}                                                             \
} while (0)

#define AB_STAILQ_REMOVE_AFTER(head, elm, field) do {             \
	ck_pr_store_ptr(&(elm)->field.cstqe_next,                     \
	    (elm)->field.cstqe_next->field.cstqe_next);               \
	if ((elm)->field.cstqe_next == NULL)                          \
		(head)->cstqh_last = &(elm)->field.cstqe_next;            \
} while (0)

#define	AB_STAILQ_REMOVE_HEAD(head, field) do {                   \
	ck_pr_store_ptr(&(head)->cstqh_first,                         \
	    (head)->cstqh_first->field.cstqe_next);                   \
	if ((head)->cstqh_first == NULL)                              \
		(head)->cstqh_last = &(head)->cstqh_first;                \
} while (0)

#define AB_STAILQ_MOVE(head1, head2, field) do {                  \
	ck_pr_store_ptr(&(head1)->cstqh_first, (head2)->cstqh_first); \
	(head1)->cstqh_last = (head2)->cstqh_last;                    \
	if ((head2)->cstqh_last == &(head2)->cstqh_first)             \
		(head1)->cstqh_last = &(head1)->cstqh_first;              \
} while (0)

/*
 * This operation is not applied atomically.
 */
#define AB_STAILQ_SWAP(head1, head2, type) do {                   \
	struct type *swap_first = AB_STAILQ_FIRST(head1);             \
	struct type **swap_last = (head1)->cstqh_last;                \
	AB_STAILQ_FIRST(head1) = AB_STAILQ_FIRST(head2);              \
	(head1)->cstqh_last = (head2)->cstqh_last;                    \
	AB_STAILQ_FIRST(head2) = swap_first;                          \
	(head2)->cstqh_last = swap_last;                              \
	if (AB_STAILQ_EMPTY(head1))                                   \
		(head1)->cstqh_last = &(head1)->cstqh_first;              \
	if (AB_STAILQ_EMPTY(head2))                                   \
		(head2)->cstqh_last = &(head2)->cstqh_first;              \
} while (0)

#endif /* AMBER_UTIL_QUEUE_H */
