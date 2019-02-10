#include <Amber/util/common.h>
#include <Amber/util/hashtable.h>
#include <SDL_thread.h>
/* #include <khash.h> */
#include <inttypes.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <cmocka.h>

/* my_ht: int32_t -> unsigned long */
#define hashfn(key) (int32_t)(key)
#define compare(k1, k2) ((k1) == (k2))
#define IS_MAP 1
AB_HT_DECLARE_TYPE(test_ht, int, unsigned long)
AB_HT_DECLARE_PROTOTYPES(test_ht, int, unsigned long)
AB_HT_DECLARE_IMPLEMENTATION(test_ht, extern, int, unsigned long, IS_MAP, hashfn, compare)

static int hashtable_setup(void **data)
{
    *data = malloc(sizeof(test_ht));
    assert_non_null(*data);

    test_ht *ht = *data;
    AB_ht_init(ht);

    return 0;
}

static int hashtable_teardown(void **data)
{
    test_ht *ht = *data;

    AB_ht_destroy(ht);

    return 0;
}

static void insert_delete(void **data)
{
    test_ht *ht = *data;

    for (int i = 0; i < 20; i++) {
        int ret;
        int32_t k = AB_ht_put(test_ht, ht, i, &ret);
        assert_in_range(ret, 1, 100);
        AB_ht_val(ht, k) = i * 2;
    }

    int32_t k;
    AB_ht_foreach(k, ht) {
        int32_t key = AB_ht_key(ht, k);
        unsigned long val = AB_ht_val(ht, k);
        assert_int_equal(val, key * 2);
        fprintf(stderr, "{ %"PRId32": %lu }\n", key, val);
    }

    k = AB_ht_get(test_ht, ht, 1);
    int ret = AB_ht_del(ht, k);
    fprintf(stderr, "ret = %d, k = %"PRId32"\n", ret, k);

    AB_ht_foreach(k, ht) {
        int32_t key = AB_ht_key(ht, k);
        unsigned long val = AB_ht_val(ht, k);
        /* assert_int_not_equal(key, 1); */
        assert_int_equal(val, key * 2);
        fprintf(stderr, "{ %"PRId32": %lu }\n", key, val);
    }
}

int main(void)
{
    const struct CMUnitTest hashtable_tests[] = {
        cmocka_unit_test_setup_teardown(insert_delete,
                hashtable_setup, hashtable_teardown)
    };

    return cmocka_run_group_tests(hashtable_tests, NULL, NULL);
}

int _main(void)
{
    test_ht ht = AB_HT_INITIALIZER;
    int i;
    int32_t k;
    int ret;

    AB_ht_init(&ht);

    for (i = 0; i < 20; i++) {
        unsigned long val = (unsigned long)i * 2;

        k = AB_ht_put(test_ht, &ht, i, &ret);
        fprintf(stderr, "ret == %d\n", ret);
        fprintf(stderr, "i = %d, val = %lu, k = %"PRIi32"\n", i, val, k);
        AB_ht_val(&ht, k) = val;
    }

    for (k = AB_ht_begin(&ht); k != AB_ht_end(&ht); k++) {
        if (!AB_ht_exists(&ht, k))
            continue;
        fprintf(stderr, "{ %d -> %lu }\n",
                AB_ht_key(&ht, k), AB_ht_val(&ht, k));
    }

    ret = AB_ht_del(&ht, 1);
    printf("ret -> %d\n", ret);

    AB_ht_foreach(k, &ht) {
        fprintf(stderr, "{ %d -> %lu }\n",
                AB_ht_key(&ht, k), AB_ht_val(&ht, k));
    }

    AB_ht_destroy(&ht);
    return 0;
}
