#include <Amber/util/concurrent-hashtable-incomplete.h>
#include <Amber/util/common.h>
#include <SDL_atomic.h>
#include <string.h>

static inline unsigned AB_ht_roundup(unsigned x);

#define AB_HT_BUCKET_SHIFT  2u
#define AB_HT_BUCKET_LENGTH (1u << AB_HT_BUCKET_SHIFT)
#define AB_HT_PROBE_DEFAULT 64u
#define AB_HT_CACHELINE 64u

struct AB_ht_map {
    enum AB_ht_mode mode;
    SDL_atomic_t deletions,
                 probe_maximum,
                 probe_limit,
                 size,
                 num_entries,
                 mask,
                 capacity,
                 step;
    SDL_atomic_t *probe_bound;
    struct AB_ht_entry *entries;
};

static struct AB_ht_map *AB_ht_map_create(struct AB_ht *ht, unsigned num_entries)
{
    num_entries = AB_ht_roundup(num_entries);
    num_entries = MIN(num_entries, AB_HT_BUCKET_LENGTH);

    unsigned size =
        (unsigned)sizeof(struct AB_ht_map) + 
        (unsigned)sizeof(struct AB_ht_entry) * num_entries +
        AB_HT_CACHELINE - 1;

    struct AB_ht_map *map = ht->allocator.malloc(size);
    if (map == NULL)
        return NULL;

    map->mode = ht->mode;
    map->size.value = (int)size;
    map->probe_limit.value = (int)MAX(
            num_entries >> (AB_HT_BUCKET_SHIFT + 2),
            AB_HT_PROBE_DEFAULT);
    map->capacity.value = (int)num_entries;
    map->step.value = (int)__builtin_ffsl(num_entries);
    map->mask.value = (int)num_entries;
    map->num_entries.value = 0;
    map->entries = (struct AB_ht_entry *)
        (((uintptr_t)&map[1] + AB_HT_CACHELINE - 1) & ~(AB_HT_CACHELINE - 1));
    map->probe_bound = NULL;

    memset(map->entries, 0x0, sizeof(struct AB_ht_entry) * num_entries);

    SDL_CompilerBarrier();
    return map;
}






static inline unsigned AB_ht_roundup(unsigned x)
{
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return ++x;
}
