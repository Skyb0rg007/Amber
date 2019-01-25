#ifndef AMBER_UTIL_HAZZARD_POINTERS_H
#define AMBER_UTIL_HAZZARD_POINTERS_H

#error "not complete!"

#if 1

struct AB_stack { int i; };
struct AB_stack_entry { int i; };

#endif

struct AB_hazzard;

struct AB_hp {
    struct AB_stack subscribers;
    unsigned num_subscribers;
    unsigned num_free;
    unsigned threshold;
    unsigned degree;
    void (*destroy)(void *);
};

struct AB_hazzard_pointer {
    void *pointer;
    void *data;
    struct AB_stack_entry pending_entry;
};

#endif /* AMBER_UTIL_HAZZARD_POINTERS_H */
