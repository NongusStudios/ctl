#include "ctldef.h"
#include "ctlvec.h"

#include <stdbool.h>

#ifndef CTLHASHMAP_H
#define CTLHASHMAP_H

#define CTL_HASHMAP_MAX_LOAD_FACTOR 1.0
#define CTL_HASHMAP_GROWTH_FACTOR 2

typedef struct ctl_hashmap_value_t {
    void* key;
    size_t keylen;
    void* value;
} ctl_hashmap_value;

typedef struct ctl_bucket_t {
    ctl_hashmap_value    value;
    bool                 used;
    struct ctl_bucket_t* next;
} ctl_bucket;

typedef struct ctl_hashmap_t {
    ctl_vec(ctl_bucket) buckets;
    size_t valuelen;
    size_t count;
} ctl_hashmap;

CTL_EXTC_OPEN

ctl_hashmap ctl_hashmap_new(size_t valuelen);
void ctl_hashmap_free(ctl_hashmap* map);

void* ctl_hashmap_insert(ctl_hashmap* map, const void* key, size_t keylen);
ctl_bucket* ctl_hashmap_search(const ctl_hashmap* map, ctl_bucket** root, const void* key, size_t keylen);
void* ctl_hashmap_get(const ctl_hashmap* map, const void* key, size_t keylen);
bool  ctl_hashmap_erase(ctl_hashmap* map, const void* key, size_t keylen);
void ctl_hashmap_clear(ctl_hashmap* map);

void* ctl_hashmap_insert_str(ctl_hashmap* map, const char* key);
ctl_bucket* ctl_hashmap_search_str(const ctl_hashmap* map, ctl_bucket** root, const char* key);
void* ctl_hashmap_get_str(const ctl_hashmap* map, const char* key);
bool ctl_hashmap_erase_str(ctl_hashmap* map, const char* key);

ctl_hashmap_value* ctl_hashmap_as_vec(ctl_hashmap* map);

double ctl_hashmap_load_factor(const ctl_hashmap* map);

CTL_EXTC_CLOSE

#endif
