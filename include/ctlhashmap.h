#include "ctldef.h"
#include "ctlvec.h"

#include <stdbool.h>

#ifndef CTLHASHMAP_H
#define CTLHASHMAP_H

#define CTL_HASHMAP_MAX_LOAD_FACTOR 1.0

typedef struct ctl_bucket_t {
    void* key;
    size_t keylen;

    void* value;
    struct ctl_bucket_t* next;
} ctl_bucket;

typedef struct ctl_hashmap_t {
    ctl_vec(ctl_bucket) data;
    size_t valuelen;
    size_t bucketcount;
} ctl_hashmap;

CTL_EXTC_OPEN

ctl_hashmap ctl_hashmap_new(size_t valuelen);
void ctl_hashmap_free(ctl_hashmap* map);

void* ctl_hashmap_insert(ctl_hashmap* map, void* key, size_t keylen);
void* ctl_hashmap_get(const ctl_hashmap* map, void* key, size_t keylen);
bool  ctl_hashmap_erase(ctl_hashmap* map, void* key, size_t keylen);

double ctl_hashmap_load_factor(const ctl_hashmap* map);

CTL_EXTC_CLOSE

#endif
