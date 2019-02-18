#include <Amber/util/common.h>
#include <Amber/util/hashtable.h>
#include <SDL_atomic.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <cmocka.h>

struct Hashtable {
    AB_HASHTABLE_DECLARE(ht, 8);
};

struct Person {
    char *name;
    intptr_t id;
    struct AB_hlist_node node;
};

int producer(void *data)
{
    struct Hashtable *ht = data;
}

int consumer(void *data)
{
    struct Hashtable *ht = data;
}

void concurrent_test(void **data)
{
    struct Hashtable *ht = *data;

    for (int i = 0; i < 20; i++) {
        struct Person *p = malloc(sizeof *p);
        AB_ASPRINTF(&p->name, "Person #%d", i);
        p->id = (intptr_t)p;
        AB_hlist_node_init(&p->node);

        AB_hash_add(ht->ht, &p->node, p->id);
    }
}

int concurrent_setup(void **data)
{
    struct Hashtable *ht = malloc(sizeof *ht); 
    AB_hash_init(ht->ht);
    *data = ht;
    return 0;
}

int concurrent_teardown(void **data)
{
    struct Hashtable *ht = *data;

    assert(AB_ARRAY_SIZE(ht->ht) == (1 << 8));

    struct Person *p;
    unsigned bkt;
    struct AB_hlist_node *tmp;
    AB_hash_foreach_safe(ht->ht, bkt, tmp, p, struct Person, node) {
        AB_hash_del(&p->node);
        fprintf(stderr, "Removing: { \"%s\": %lx }\n", p->name, p->id);
        fprintf(stderr, "tmp = %p, p = %p\n", (void *)tmp, (void *)p);
        free(p->name);
        free(p);
    }

   free(ht);
    return 0;
}

int main(void)
{
    const struct CMUnitTest ht_concurrent[] = {
        cmocka_unit_test_setup_teardown(concurrent_test, concurrent_setup, concurrent_teardown)
    };

    return cmocka_run_group_tests(ht_concurrent, NULL, NULL);
}
