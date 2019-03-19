#include <Amber/util/common.h>
#include <Amber/util/hashtable.h>
#include <Amber/cmocka/AB_cmocka.h>

struct person {
    char *name;
    unsigned id;
    struct AB_hlist_node node;
};

static AB_HASHTABLE_DECLARE(my_ht, 4) = AB_HASHTABLE_INITIALIZER;

static void insert_delete(void **data)
{
    unsigned i;
    (void)data;

    for (i = 0; i < 20; i++) {
        struct person *person = test_malloc(sizeof *person);
        AB_ASPRINTF(&person->name, "Person #%d", i);
        person->id = i;
        AB_hlist_node_init(&person->node);

        uint32_t hash = AB_fnv_hash_str(person->name);

        AB_hash_add(my_ht, &person->node, hash);
    }

    unsigned bkt;
    struct person *p;
    AB_hash_foreach(my_ht, bkt, p, struct person, node) {
        fprintf(stderr, "%s: %d\n", p->name, p->id);
    }

    fprintf(stderr, "Searching for 'Person #4'\n");
    uint32_t hash = AB_fnv_hash_str("Person #4");
    AB_hash_foreach_possible(my_ht, p, struct person, node, hash) {
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
