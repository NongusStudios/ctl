#include "ctldef.h"
#include "ctlvec.h"

#include <stdbool.h>

#ifndef CTLHASHMAP_H
#define CTLHASHMAP_H

#define CTL_HASHMAP_MAX_LOAD_FACTOR 1.0
#define CTL_HASHMAP_GROWTH_FACTOR 2

/*! Bucket struct used in ctl_hashmap */
typedef struct ctl_bucket_t {
    ctl_keyvalue         value; /*! Stores the key and the value it holds.                       */
    bool                 used;  /*! Equals true if the bucket is in use.                         */
    struct ctl_bucket_t* next;  /*! Holds further buckets in a linked list to handle collisions. */
} ctl_bucket;

/*!
 * A simple expandable hash table implementation.
*/
typedef struct ctl_hashmap_t {
    ctl_vec(ctl_bucket) buckets; /*! Data in the hash table. */
    size_t valuelen; /*! The length (in bytes) needed to allocate a value. */
    size_t count; /*! The amount of active keys in the hash table. */
} ctl_hashmap;

CTL_EXTC_OPEN

/*!
 * Creates a new hashmap with a capacity of 1 which doubles each time the load factor
 * reaches or exceeds 1.0 (load_factor = count/capacity).
 * @param valuelen The size of each value in bytes.
 * @return The allocated ctl_hashmap
*/
ctl_hashmap ctl_hashmap_new(size_t valuelen);

/*!
 * Frees resources allocated by ctl_hashmap_new(...).
 * @param map The return from ctl_hashmap_new(...).
*/
void ctl_hashmap_free(ctl_hashmap* map);

/*!
 * Inserts a new key into the hash table.
 * If a key is given that is already in the table the value behind that key will be returned
 * with no modifications to map.
 * The type of the key is irrelavent as it will be read as a collection of bytes.
 * @param map The hash map that will be modified.
 * @param key A pointer to a key.
 * @param keylen The size of the key in bytes.
 * @return A pointer to a block of memory (size=valuelen) linked to the given key (NULL if key doesn't exist).
*/
void* ctl_hashmap_insert(ctl_hashmap* map, const void* key, size_t keylen);

/*!
 * Searches through the hash table and returns the bucket that contains the requested key.
 * If the requested key doesn't exist in the table NULL will be returned.
 * @param map The hash map that will be searched.
 * @param root The root of this bucket will be returned through this pointer (If you don't want the root, use NULL).
 * @param key A pointer to a key.
 * @param keylen The size of the key in bytes.
 * @return The bucket that contains key (NULL if key doesn't exist).
*/
ctl_bucket* ctl_hashmap_search(const ctl_hashmap* map, ctl_bucket** root, const void* key, size_t keylen);

/*!
 * Wrapper around ctl_hashmap_search(...) that only returns the value.
 * If the requested key doesn't exist in the table NULL is returned.
 * @param map The hash map that will be searched.
 * @param key A pointer to a key.
 * @param keylen The size if the key in bytes.
 * @return The value linked with key (NULL if key doesn't exist).
*/
void* ctl_hashmap_get(const ctl_hashmap* map, const void* key, size_t keylen);

/*!
 * Erases a key and value from the hash table.
 * @param map The hash map that will be modified.
 * @param key A pointer to a key.
 * @param keylen The size of the key in bytes.
 * @return true if a value was erased.
*/
bool  ctl_hashmap_erase(ctl_hashmap* map, const void* key, size_t keylen);
/*!
 * Clears the hash table.
 * @param map The hash map that will be cleared.
*/
void ctl_hashmap_clear(ctl_hashmap* map);

/*!
 * A wrapper around ctl_hashmap_insert to make using const char* as keys easier.
 * @see ctl_hashmap_insert()
*/
void* ctl_hashmap_insert_str(ctl_hashmap* map, const char* key);

/*!
 * A wrapper around ctl_hashmap_search to make using const char* as keys easier.
 * @see ctl_hashmap_search()
*/
ctl_bucket* ctl_hashmap_search_str(const ctl_hashmap* map, ctl_bucket** root, const char* key);

/*!
 * A wrapper around ctl_hashmap_get to make using const char* as keys easier.
 * @see ctl_hashmap_get()
*/
void* ctl_hashmap_get_str(const ctl_hashmap* map, const char* key);

/*!
 * A wrapper around ctl_hashmap_erase to make using const char* as keys easier.
 * @see ctl_hashmap_erase()
*/
bool ctl_hashmap_erase_str(ctl_hashmap* map, const char* key);

/*!
 * Takes all of the active keys and values and puts them into a ctl_vec<ctl_keyvalue>.
 * ctl_keyvalue is not cloned so modifying it will directly affect the hash map.
 * @param map The hash map that will be used.
 * @return Vector of keys and values.
*/
ctl_keyvalue* ctl_hashmap_as_vec(const ctl_hashmap* map);

/*!
 * Clones the hash map.
 * @param map The hash map to clone.
 * @return The cloned hash map.
*/
ctl_hashmap   ctl_hashmap_clone(const ctl_hashmap* map);

/*!
 * Returns the load factor of the hash map.
 * @param map The hash map that is used.
 * @return The load factor.
*/
double ctl_hashmap_load_factor(const ctl_hashmap* map);

CTL_EXTC_CLOSE

#endif
