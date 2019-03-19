#include <Amber/util/rbtree.h>
#include <stdint.h>

#define RB_RED   0
#define RB_BLACK 1

static AB_INLINE void rb_set_black(struct AB_rb_node *node)
{
    node->parent_color |= RB_BLACK;
}

static AB_INLINE int rb_is_black(struct AB_rb_node *node)
{
    return node->parent_color & 1;
}

static AB_INLINE void rb_set_parent_color(struct AB_rb_node *node,
        struct AB_rb_node *parent, unsigned color)
{
    node->parent_color = (uintptr_t)parent | color;
}

static AB_INLINE void rb_change_child(
        struct AB_rb_node *old, struct AB_rb_node *new,
        struct AB_rb_node *parent, struct AB_rb_root *root)
{
    if (parent != NULL) {
        if (parent->left == old) {
            parent->left = new;
        } else {
            parent->right = new;
        }
    } else {
        root->node = new;
    }
}

static AB_INLINE void rb_rotate_set_parents(
        struct AB_rb_node *old, struct AB_rb_node *new,
        struct AB_rb_root *root, unsigned color)
{
    struct AB_rb_node *parent = AB_rb_parent(old);
    new->parent_color = old->parent_color;
    rb_set_parent_color(old, new, color);
    rb_change_child(old, new, parent, root);
}

static AB_INLINE struct AB_rb_node *rb_red_parent(struct AB_rb_node *node)
{
    return (struct AB_rb_node *)node->parent_color;
}

static AB_INLINE void rb_insert(
        struct AB_rb_node *node, struct AB_rb_root *root,
        struct AB_rb_node *new)
{
    struct AB_rb_node *parent = rb_red_parent(node);
    struct AB_rb_node *gparent;
    struct AB_rb_node *tmp;

    for (;;) {
        if (parent == NULL) {
            rb_set_parent_color(node, NULL, RB_BLACK);
            break;
        }

        if (rb_is_black(parent))
            break;

        gparent = rb_red_parent(parent);
        tmp = gparent->right;

        if (parent != tmp) {

        }
    }
}
