#include <Amber/util/common.h>
#include <Amber/util/hashtable.h>
#include <SDL_thread.h>
/* #include <khash.h> */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/* my_ht: int32_t -> unsigned */
#define hashfn(key) (int32_t)(key)
#define compare(k1, k2) ((k1) == (k2))
#define IS_MAP 1
AB_HT_DECLARE_TYPE(test_ht, int, unsigned long)
AB_HT_DECLARE_PROTOTYPES(test_ht, int, unsigned long)
AB_HT_DECLARE_IMPLEMENTATION(test_ht, extern, int, unsigned long, IS_MAP, hashfn, compare)

/* typedef struct {
    int32_t n_buckets, size, n_occupied, upper_bound;
    int32_t *flags;
    int *keys;
    unsigned long *vals;
} test_ht;
#define AB_HT_INITIALIZER { 0, 0, 0, 0, NULL, NULL, NULL }

extern int     AB_ht_init_test_ht(test_ht *ht);
extern void    AB_ht_destroy_test_ht(test_ht *ht);
extern void    AB_ht_clear_test_ht(test_ht *ht);
extern int32_t AB_ht_get_test_ht(const test_ht *ht, int key);
extern int     AB_ht_del_test_ht(test_ht *ht, int32_t idx);
extern int32_t AB_ht_put_test_ht(test_ht *ht, int key, int *ret);
extern int     AB_ht_resize_test_ht(test_ht *ht, int32_t new_n_buckets);

int AB_ht_init_test_ht(test_ht *ht) {
    if (ht == NULL)
        return AB_HT_EINVAL;
    memset(ht, 0x0, sizeof(test_ht));
    ht->flags = NULL;
    ht->vals = NULL;
    ht->keys = NULL;
    return AB_HT_NOERROR;
}
void AB_ht_destroy_test_ht(test_ht *ht) {
    if (ht) {
        AB_HT_FREE(ht->keys, 0, NULL);
        AB_HT_FREE(ht->flags, 0, NULL);
        AB_HT_FREE(ht->vals, 0, NULL);
    }
}
void AB_ht_clear_test_ht(test_ht *ht) {
    if (ht && ht->flags) {
        memset(ht->flags, 0xaa, AB_ac_fsize(ht->n_buckets) * sizeof(int32_t));
        ht->size = ht->n_occupied = 0;
    }
}
int32_t AB_ht_get_test_ht(const test_ht *ht, int key) {
    if (ht == NULL)
        return AB_HT_EINVAL;

    if (ht->n_buckets) {
        int32_t mask = ht->n_buckets - 1;
        int32_t k = hashfn(key);
        int32_t i = k & mask;
        int32_t last = i;
        int32_t step = 0;
        while (!AB_ac_isempty(ht->flags, i) &&
                (AB_ac_isdel(ht->flags, i) || compare(ht->keys[i], key))) {
            i = (i + (++step)) & mask;
            if (i == last)
                return ht->n_buckets;
        }
        return AB_ac_iseither(ht->flags, i) ? ht->n_buckets : i;
    }
    return 0;
}
int AB_ht_del_test_ht(test_ht *ht, int32_t idx) {
    if (ht == NULL || idx >= ht->n_buckets)
        return AB_HT_EINVAL;
    if (!AB_ac_iseither(ht->flags, idx)) {
        AB_ac_set_isdel_true(ht->flags, idx);
        --ht->size;
        return AB_HT_NOERROR;
    }
}
int32_t AB_ht_put_test_ht(test_ht *ht, int key, int *ret) {
    int32_t x;
    if (ht->n_occupied >= ht->upper_bound) { [>Test if hash table needs updating<]
        if (ht->n_buckets > (ht->size << 1)) { [>clear the deleted items<]
            if (AB_ht_resize_test_ht(ht, ht->n_buckets - 1) < 0) {
                if (ret)
                    *ret = AB_HT_ENOMEM;
                return ht->n_buckets;
            }
        } else if (AB_ht_resize_test_ht(ht, ht->n_buckets + 1) < 0) { [>expand the hashtable<]
            if (ret)
                *ret = AB_HT_ENOMEM;
            return ht->n_buckets;
        }
    }
    {
        int32_t k, i, site, last;
        int32_t mask = ht->n_buckets - 1, step = 0;
        x = site = ht->n_buckets;
        k = hashfn(key);
        i = k & mask;
        if (AB_ac_isempty(ht->flags, i)) {
            x = i;
        } else {
            last = i;
            while (!AB_ac_isempty(ht->flags, i) &&
                    (AB_ac_isdel(ht->flags, i) || !compare(ht->keys[i], key))) {
                if (AB_ac_isdel(ht->flags, i))
                    site = i;
                i = (i + (++step)) & mask;
                if (i == last) {
                    x = site;
                    break;
                }
            }
            if (x == ht->n_buckets) {
                if (AB_ac_isempty(ht->flags, i) && site != ht->n_buckets)
                    x = site;
                else
                    x = i;
            }
        }
    }
    if (AB_ac_isempty(ht->flags, x)) { [>Element is not present<]
        ht->keys[x] = key;
        AB_ac_set_isboth_false(ht->flags, x);
        ++ht->size;
        ++ht->n_occupied;
        if (ret)
            *ret = AB_HT_ENOTPRESENT;
    } else if (AB_ac_isdel(ht->flags, x)) { [>Element was deleted<]
        ht->keys[x] = key;
        AB_ac_set_isboth_false(ht->flags, x);
        ++ht->size;
        if (ret)
            *ret = AB_HT_EDELETED;
    } else { [>Dont change anything if ht->keys[x] if present and not deleted<]
        if (ret)
            *ret = AB_HT_ENOCHANGE;
    }
    return x;
}
int AB_ht_resize_test_ht(test_ht *ht, int32_t new_n_buckets) {
    int32_t *new_flags = NULL;
    {
        new_n_buckets = MAX(4, AB_ht_roundup32(new_n_buckets));

        if (ht->size >= (int32_t)(new_n_buckets * AB_HT_HASH_UPPER + 0.5)) {
            return AB_HT_ENOCHANGE; [>Requested size is too small<]
        } else {
            new_flags = AB_HT_MALLOC(AB_ac_fsize(new_n_buckets) * sizeof(int32_t), NULL);
            if (!new_flags)
                return AB_HT_ENOMEM;
            memset(new_flags, 0xaa, AB_ac_fsize(new_n_buckets) * sizeof(int32_t));
            if (ht->n_buckets < new_n_buckets) { [>We need to expand<]
                int *new_keys = AB_HT_REALLOC(ht->keys, 0, (size_t)new_n_buckets * sizeof(int), NULL);
                if (new_keys == NULL) {
                    AB_HT_FREE(new_flags, 0, NULL);
                    return AB_HT_ENOMEM;
                }
                if (IS_MAP) {
                    unsigned long *new_vals = AB_HT_REALLOC(ht->vals, 0, (size_t)new_n_buckets * sizeof(unsigned long), NULL);
                    if (new_vals == NULL) {
                        AB_HT_FREE(new_vals, 0, NULL);
                        return AB_HT_ENOMEM;
                    }
                    ht->vals = new_vals;
                }
                ht->keys = new_keys;
            }
        }
    }
    {   [>Rehash<]
        int32_t j;
        for (j = 0; j != ht->n_buckets; j++) {
            if (AB_ac_iseither(ht->flags, j) == 0) {
                int key = ht->keys[j];
                unsigned long val;
                int32_t new_mask = new_n_buckets - 1;
                if (IS_MAP)
                    val = ht->vals[j];
                AB_ac_set_isdel_true(ht->flags, j);
                while (1) {
                    int32_t k, i, step = 0;
                    k = hashfn(key);
                    i = k & new_mask;
                    while (!AB_ac_isempty(new_flags, i))
                        i = (i + (++step)) & new_mask;
                    AB_ac_set_isempty_false(new_flags, i);
                    if (i < ht->n_buckets && AB_ac_iseither(ht->flags, i) == 0) {
                        int tmp = ht->keys[i];
                        ht->keys[i] = key;
                        key = tmp;
                        if (IS_MAP) {
                            unsigned long vtmp = ht->vals[i];
                            ht->vals[i] = val;
                            val = vtmp;
                        }
                        AB_ac_set_isdel_true(ht->flags, i);
                    } else {
                        ht->keys[i] = key;
                        if (IS_MAP)
                            ht->vals[i] = val;
                        break;
                    }
                }
            }
        }
        if (ht->n_buckets > new_n_buckets) { [>shrink the hash table<]
            ht->keys = AB_HT_REALLOC(ht->keys, 0, (size_t)new_n_buckets * sizeof(int), NULL);
            if (IS_MAP)
                ht->vals = AB_HT_REALLOC(ht->vals, 0, (size_t)new_n_buckets * sizeof(unsigned long), NULL);
        }
        AB_HT_FREE(ht->flags, 0, NULL);
        ht->flags = new_flags;
        ht->n_buckets = new_n_buckets;
        ht->n_occupied = ht->size;
        ht->upper_bound = (int32_t)(ht->n_buckets * AB_HT_HASH_UPPER + 0.5);
    }
    return 0;
} */


int main(void)
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
