#include <Amber/util/common.h>
#include <khash.h>
#include <SDL_thread.h>

/* my_ht: int32_t -> unsigned */
#define hashfn(key) (khint32_t)(key)
#define compare(k1, k2) ((k1) == (k2))
#define IS_MAP 1
KHASH_INIT2(my_ht, static inline, int32_t, unsigned, IS_MAP, hashfn, compare)

khash_t(my_ht) *g_hashtable;

int main(void)
{
    g_hashtable = kh_init(my_ht);
    ASSERT(g_hashtable);

    for (int i = 0; i < 20; i++) {
        int ret;
        khiter_t it = kh_put(my_ht, g_hashtable, i, &ret);
        kh_value(g_hashtable, it) = (unsigned)i;
    }

    int32_t key;
    unsigned value;
    for (khiter_t it = kh_begin(g_hashtable); it != kh_end(g_hashtable); it++) {
        if (!kh_exist(g_hashtable, it))
            continue;
        key = kh_key(g_hashtable, it);
        value = kh_value(g_hashtable, it);
        printf("%"PRIi32": %u\n", key, value);
    }

    kh_destroy(my_ht, g_hashtable);
}
