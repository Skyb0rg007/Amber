/** @file concurrent-hashtable.h
 * @brief A lock-free robin-hood hashtable based on ConcurrencyKit
 *
 * Differences from libck:
 *  No 'workload delete' or pointer packing.
 */
#ifndef AMBER_UTIL_CONCURRENT_HASHTABLE_H
#define AMBER_UTIL_CONCURRENT_HASHTABLE_H

#error "Not finished!"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <SDL_atomic.h>

enum AB_ht_mode {
    AB_HT_MODE_DIRECT,
    AB_HT_MODE_BYTESTRING
};

struct AB_ht_entry {
    uintptr_t key;
    uintptr_t value;
    SDL_atomic_t key_length;
    SDL_atomic_t hash;
};

#ifndef AB_HT_KEY_EMPTY
#define AB_HT_KEY_EMPTY ((uintptr_t)0)
#endif

#ifndef AB_HT_KEY_TOMBSTONE
#define AB_HT_KEY_TOMBSTONE (~(uintptr_t)0)
#endif

struct AB_ht_map;
struct AB_ht {
    struct {
        void *(*malloc)(size_t size);
        void (*free)(void *ptr, size_t size, bool defer);
        void (*realloc)(void *ptr, size_t old_size, size_t new_size, bool defer);
    } allocator;
    struct AB_ht_map *map;
    uint64_t seed;
    uint64_t (*hash_fn)(const void *entry, size_t entry_size, uint64_t seed);
    enum AB_ht_mode mode;
};

struct AB_ht_iterator {
    struct AB_ht_entry *current;
    uint64_t offset;
};
#define AB_HT_ITERATOR_INITIALIZER { NULL, 0 }

static inline void AB_ht_iterator_init(struct AB_ht_iterator *it)
{ it->current = NULL; it->offset = 0; }

static inline bool AB_ht_entry_empty(struct AB_ht_entry *entry)
{ return entry->key == AB_HT_KEY_EMPTY; }

static inline void AB_ht_entry_key_set_direct(struct AB_ht_entry *entry, uintptr_t key)
{ entry->key = key; }

static inline void AB_ht_entry_key_set(struct AB_ht_entry *entry, const void *key, int key_length)
{ entry->key = (uintptr_t)key; SDL_AtomicSet(&entry->key_length, key_length); }

static inline int AB_ht_entry_key_length(struct AB_ht_entry *entry)
{ return SDL_AtomicGet(&entry->key_length); }

static inline void *AB_ht_entry_value(struct AB_ht_entry *entry)
{ return (void *)entry->value; }

static inline void AB_ht_entry_set(struct AB_ht_entry *entry, uint64_t hash, 
        const void *key, int key_length, const void *value)
{ 
    entry->key = (uintptr_t)key; entry->value = (uintptr_t)value;
    entry->key_length.value = key_length;
    entry->hash.value = hash;
}

#endif /* AMBER_UTIL_HASHTABLE_H */
