#include <Amber/util/common.h>
#include <Amber/util/linux-hashtable.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <cmocka.h>

struct person {
    char *name;
    unsigned id;
    struct AB_hlist_node node;
};

static AB_HASHTABLE_DEFINE(my_ht, 8);

static void insert_delete(void **data)
{
    (void)data;

    for (int i = 0; i < 20; i++) {
        struct person *person = test_malloc(sizeof *person);
        AB_ASPRINTF(&person->name, "Person #%d", i);
        person->id = i;
        AB_hlist_node_init(&person->node);

        AB_hash_add(my_ht, &person->node, person->id);
    }

    unsigned bkt;
    struct person *p;
    AB_hash_foreach(my_ht, bkt, p, struct person, node) {
        fprintf(stderr, "%s: %d\n", p->name, p->id);
    }

    struct AB_hlist_node *tmp;
    AB_hash_foreach_safe(my_ht, bkt, tmp, p, struct person, node) {
        AB_hash_del(&p->node);
        free(p->name);
        test_free(p);
    }
}

int main(void)
{
    const struct CMUnitTest hashtable_tests[] = {
        cmocka_unit_test(insert_delete)
    };

    return cmocka_run_group_tests(hashtable_tests, NULL, NULL);
}
