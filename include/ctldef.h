#include <stdlib.h>

#ifndef CTLDEF_H
#define CTLDEF_H

#ifdef __cplusplus
    #define CTL_EXTC_OPEN extern "C" {
    #define CTL_EXTC_CLOSE }
#else
    #define CTL_EXTC_OPEN
    #define CTL_EXTC_CLOSE
#endif

/*! Structure used in ctl_hashmap and ctl_map */
typedef struct ctl_keyvalue_t {
    void* key;     /*! Pointer to the key. */
    size_t keylen; /*! Key length. */
    void* value;   /*! The Value.  */
} ctl_keyvalue;

#endif
