#include "ctldef.h"

#include <stdbool.h>

#ifndef CTLMAP_H
#define CTLMAP_H

typedef enum ctl_map_color_t {
    CTL_MAPCOLOR_RED,
    CTL_MAPCOLOR_BLACK,
} ctl_map_color;

typedef struct ctl_map_leaf_t {
    ctl_keyvalue  value;
    ctl_map_color color;

    struct ctl_map_leaf_t* left;
    struct ctl_map_leaf_t* right;
} ctl_map_leaf;

typedef struct ctl_map_t {
    ctl_map_leaf root;
    size_t       count;
    size_t       typelen;
} ctl_map;

CTL_EXTC_OPEN

ctl_map ctl_map_new(size_t typelen);
void ctl_map_free(ctl_map* map);

void* ctl_map_insert(ctl_map* map, const void* key, size_t keylen);
void* ctl_map_get(const ctl_map* map, const void* key, size_t keylen);
bool ctl_map_erase(ctl_map* map, const void* key, size_t keylen);

void ctl_map_clear(ctl_map* map);

CTL_EXTC_CLOSE

#endif
