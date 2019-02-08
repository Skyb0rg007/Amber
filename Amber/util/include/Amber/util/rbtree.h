#ifndef AMBER_UTIL_RBTREE_H
#define AMBER_UTIL_RBTREE_H

#include <stddef.h>
#include <stdint.h>

struct AB_rb_node {
    uintptr_t parent_color; /* packed - assume the struct is aligned */
    struct AB_rb_node *right;
    struct AB_rb_node *left;
};

struct AB_rb_root {
    struct AB_rb_node *node;
};

#define AB_RB_ROOT_INITIALIZER { NULL }

#define AB_rb_parent(r) \
    ((struct AB_rb_node *)((r)->parent_color & ~3))

/* container_of */
#define AB_rb_entry(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

#define AB_RB_ISEMPTY(node) \
    ((node)->parent == (node))

#define AB_RB_CLEAR(node) \
    ((node)->parent = (node))

void AB_rb_insert_color(struct AB_rb_node *node, struct AB_rb_root *root);
void AB_rb_erase(struct AB_rb_node *node, struct AB_rb_root *root);


struct AB_rb_node *AB_rb_next(const struct AB_rb_node  *node);
struct AB_rb_node *AB_rb_prev(const struct AB_rb_node  *node);
struct AB_rb_node *AB_rb_first(const struct AB_rb_root *root);
struct AB_rb_node *AB_rb_last(const struct AB_rb_root  *root);

struct AB_rb_node *AB_rb_first_postorder(const struct AB_rb_root *root);
struct AB_rb_node *AB_rb_next_postorder(const struct AB_rb_root *root);


/* Fast replacement of a single node without remove/rebalance/add/rebalance */
extern void AB_rb_replace_node(struct AB_rb_node *victim,
        struct AB_rb_node *new, struct AB_rb_root *root);

static inline void AB_rb_link_node(struct AB_rb_node *node,
        struct AB_rb_node *parent, struct AB_rb_node **rb_link)
{
	node->parent_color = (uintptr_t)parent;
	node->left = node->right = NULL;

	*rb_link = node;
}

#define AB_RB_ENTRY_SAFE(ptr, type, member) \
    ((ptr) ? AB_rb_entry((ptr), (type), member) : NULL)

/*
 * rbtree_postorder_for_each_entry_safe - iterate in post-order over rb_root of
 * given type allowing the backing memory of @pos to be invalidated
 *
 * @pos:	the 'type *' to use as a loop cursor.
 * @n:		another 'type *' to use as temporary storage
 * @root:	'rb_root *' of the rbtree.
 * @type:   the 'type' of the entry
 * @field:	the name of the rb_node field within 'type'.
 *
 * rbtree_postorder_for_each_entry_safe() provides a similar guarantee as
 * list_for_each_entry_safe() and allows the iteration to continue independent
 * of changes to @pos by the body of the loop.
 *
 * Note, however, that it cannot handle other modifications that re-order the
 * rbtree it is iterating over. This includes calling rb_erase() on @pos, as
 * rb_erase() may rebalance the tree, causing us to miss some nodes.
 */
#define rbtree_postorder_for_each_entry_safe(pos, n, root, type, field) \
	for ((pos) = AB_RB_ENTRY_SAFE(rb_first_postorder(root), type, field); \
	     (pos) && ((n) = AB_RB_ENTRY_SAFE(rb_next_postorder(&pos->field), type, field), 1); \
	     (pos) = (n))

#endif
