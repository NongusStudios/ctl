#include "ctldef.h"

#ifndef CTLMAP_H
#define CTLMAP_H

typedef enum ctl_map_color_t {
    CTL_MAPCOLOR_RED,
    CTL_MAPCOLOR_BLACK,
} ctl_map_color;

typedef struct ctl_map_leaf_t {
    ctl_keyvalue value;


    struct ctl_map_leaf_t* left;
    struct ctl_map_leaf_t* right;
} ctl_map_leaf_t;

CTL_EXTC_OPEN



CTL_EXTC_CLOSE

#endif
