#include "ctlhashmap.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

ctl_hashmap ctl_hashmap_new(size_t valuelen){
    ctl_hashmap map = {
        .valuelen = valuelen,
        .count = 0,
        .buckets = ctl_vec_new(NULL, 1, sizeof(ctl_bucket)),
    };

    return map;
}

void ctl_hashmap_free_value(ctl_keyvalue* value){
    free(value->key);
    free(value->value);
}

void ctl_hashmap_free_bucket(ctl_bucket* bucket){
    ctl_hashmap_free_value(&bucket->value);

    ctl_bucket* current = bucket->next;
    while(current != NULL){
        ctl_hashmap_free_value(&current->value);
        
        ctl_bucket* last = current;
        current = current->next;
        free(last);
    }
}

void ctl_hashmap_free(ctl_hashmap* map){
    assert(map);
    assert(map->buckets);

    ctl_vec_foreach(ctl_bucket, bucket, map->buckets){
        ctl_hashmap_free_bucket(bucket);
    }
    ctl_vec_free(map->buckets);
}

size_t ctl_hash_key(const void* key, size_t len){
    unsigned long hash = 5381;
    
    const char* keyc = key;
    int c = 0;
    for(size_t i = 0; i < len; i++){
        c = keyc[i];
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

bool ctl_hashmap_key_iseq(const void* a, size_t alen, const void* b, size_t blen){
    assert(a && b);
    if(alen != blen) return false;
    
    const char* ac = a;
    const char* bc = b;
    for(size_t i = 0; i < alen; i++){
        if(*ac++ != *bc++) return false;
    }

    return true;
}

void ctl_hashmap_grow_and_rehash(ctl_hashmap* map){
    ctl_vec(ctl_keyvalue) values = ctl_hashmap_as_vec(map);
    ctl_hashmap newmap = ctl_hashmap_new(map->valuelen);
    ctl_vec_resize(ctl_vptr(newmap.buckets), ctl_vec_len(map->buckets)*CTL_HASHMAP_GROWTH_FACTOR);

    ctl_vec_foreach(ctl_keyvalue, value, values){
        void* v = ctl_hashmap_insert(&newmap, value->key, value->keylen);
        memcpy(v, value->value, map->valuelen);
    }

    ctl_vec_free(values);
    ctl_hashmap_free(map);
    *map = newmap;
}

ctl_bucket* ctl_hashmap_get_last_bucket(ctl_bucket* root){
    if(root->next == NULL) return root;

    ctl_bucket* current = root;
    ctl_bucket* last = NULL;
    while(current != NULL){
        last = current;
        current = current->next;
    }

    return last;
}

void ctl_hashmap_init_bucket(ctl_bucket* bucket, const void* key, size_t keylen, size_t valuelen){
    bucket->used = true;
    bucket->next = NULL;
    bucket->value = (ctl_keyvalue){
        .key = malloc(keylen),
        .keylen = keylen,
        .value = calloc(1, valuelen),
    };
    memcpy(bucket->value.key, key, keylen);
}

void* ctl_hashmap_insert(ctl_hashmap* map, const void* key, size_t keylen){
    assert(map);
    assert(map->buckets);
    assert(key && keylen > 0);
    
    void* v = ctl_hashmap_get(map, key, keylen);
    if(v) return v;
    else {
        if(ctl_hashmap_load_factor(map) >= CTL_HASHMAP_MAX_LOAD_FACTOR){
            ctl_hashmap_grow_and_rehash(map);
        }

        size_t hash = ctl_hash_key(key, keylen) % ctl_vec_len(map->buckets);
        ctl_bucket* last = ctl_hashmap_get_last_bucket(&map->buckets[hash]);
        
        if(last->used){
            last->next = malloc(sizeof(ctl_bucket));
            last = last->next;
        }
        ctl_hashmap_init_bucket(last, key, keylen, map->valuelen);
        map->count++;
        return last->value.value;
    }
}
ctl_bucket* ctl_hashmap_search(const ctl_hashmap* map, ctl_bucket** root, const void* key, size_t keylen){
    assert(map);
    assert(map->buckets);

    size_t hash = ctl_hash_key(key, keylen) % ctl_vec_len(map->buckets);
    
    ctl_bucket* current = &map->buckets[hash];
    while(current != NULL){
        if(current->used && ctl_hashmap_key_iseq(current->value.key, current->value.keylen, key, keylen)){
            if(root) *root = &map->buckets[hash];
            return current;
        }
        current = current->next;
    }

    return NULL;
}
void* ctl_hashmap_get(const ctl_hashmap* map, const void* key, size_t keylen){
    ctl_bucket* bucket = ctl_hashmap_search(map, NULL, key, keylen);
    return (bucket) ? bucket->value.value : NULL;
}
bool  ctl_hashmap_erase(ctl_hashmap* map, const void* key, size_t keylen){
    assert(map);
    assert(map->buckets);

    ctl_bucket* root = NULL;
    ctl_bucket* to_erase = ctl_hashmap_search(map, &root, key, keylen);
    if(!to_erase) return false; 
    
    if(root == to_erase) {
        free(to_erase->value.key);
        free(to_erase->value.value);
        if(to_erase->next){
            to_erase->value = to_erase->next->value;
            ctl_bucket* next = to_erase->next;
            to_erase->next = next->next;
            free(next);
        } else {
            to_erase->value = (ctl_keyvalue){.key = NULL, .keylen = 0, .value = NULL};
            to_erase->used  = false;
        }
    } else {
        ctl_bucket* current = root;
        while(current != NULL){
            if(current->next == to_erase) break;
            current = current->next;
        }
        free(to_erase->value.key);
        free(to_erase->value.value);
        current->next = to_erase->next;
        free(to_erase);
    }

    return true;
}
void ctl_hashmap_clear(ctl_hashmap* map){
    assert(map);
    assert(map->buckets);

    size_t valuelen = map->valuelen;
    ctl_hashmap_free(map);
    *map = ctl_hashmap_new(valuelen);
}

void* ctl_hashmap_insert_str(ctl_hashmap* map, const char* key){
    return ctl_hashmap_insert(map, key, strlen(key)+1);
}
ctl_bucket* ctl_hashmap_search_str(const ctl_hashmap* map, ctl_bucket** root, const char* key){
    return ctl_hashmap_search(map, root, key, strlen(key)+1);
}
void* ctl_hashmap_get_str(const ctl_hashmap* map, const char* key){
    return ctl_hashmap_get(map, key, strlen(key)+1);
}
bool ctl_hashmap_erase_str(ctl_hashmap* map, const char* key){
    return ctl_hashmap_erase(map, key, strlen(key)+1);
}

ctl_keyvalue* ctl_hashmap_as_vec(const ctl_hashmap* map){
    assert(map);
    assert(map->buckets);

    if(ctl_vec_len(map->buckets) == 0) return NULL;

    ctl_vec(ctl_keyvalue) values = ctl_vec_with_capacity(map->count, sizeof(ctl_keyvalue));
    
    ctl_vec_foreach(ctl_bucket, bucket, map->buckets){
        ctl_bucket* current = bucket;
        while(current != NULL){
            if(current->used) {
                ctl_vec_push(ctl_vptr(values), CTL_VECPOS_BACK, &current->value);
            }
            current = current->next;
        }
    }

    return values;
}
ctl_hashmap ctl_hashmap_clone(const ctl_hashmap* map){
    assert(map);
    assert(map->buckets);

    ctl_vec(ctl_keyvalue) values = ctl_hashmap_as_vec(map);
    ctl_hashmap clone = ctl_hashmap_new(map->valuelen);

    ctl_vec_foreach(ctl_keyvalue, value, values){
        void* v = ctl_hashmap_insert(&clone, value->key, value->keylen);
        memcpy(v, value->value, map->valuelen);
    }

    return clone;
}

double ctl_hashmap_load_factor(const ctl_hashmap* map){
    assert(map);
    return (double)map->count/(double)ctl_vec_len(map->buckets);
}

