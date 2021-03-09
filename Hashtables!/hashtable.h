#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct hashtable hashtable;

// Creates a new hash table with the given number of
// buckets for values. bucket_count must be > 0.
hashtable *ht_make(size_t bucket_count);

size_t ht_len(const hashtable *ht);

void ht_dump(const hashtable *ht);

void ht_set(hashtable *ht, const char *key, int val);

bool ht_get(const hashtable *ht, const char *key, int *value);

bool ht_remove(hashtable *ht, const char *key);

void ht_free(hashtable *ht);

#endif