#include <Amber/util/concurrent-hashtable.h>
#include <Amber/util/vector.h>
#include <stdbool.h>
#include <stdint.h>

struct Hashtable {
    uint64_t size;
};

bool Contains(struct Hashtable *ht, uint32_t key)
{
    uint64_t start_bucket = hash(key) % ht->size;

    AB_vec(uint64_t) timestamps = AB_VEC_INIT;
    uint64_t i = start_bucket, curr_dist = 0;
    for (;; curr_dist++, i++) {
        i %= ht->size;
        AB_vec_push(&timestamps, read_timestamp(i));
        uint32_t current_key = 
    }
}
