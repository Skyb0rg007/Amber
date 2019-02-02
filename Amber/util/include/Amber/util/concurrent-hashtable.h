#ifndef AMBER_UTIL_CONCURRENT_HASHTABLE_H
#define AMBER_UTIL_CONCURRENT_HASHTABLE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
// key -> uint32_t
struct Hashtable;
bool Contains(struct Hashtable *ht, uint32_t key);

#endif /* AMBER_UTIL_CONCURRENT_HASHTABLE_H */
