#include "ctldef.h"

#include <stdlib.h>

#ifndef CTLVEC_H
#define CTLVEC_H

#define ctl_vec(type) type*
#define ctl_vec_init(type, ...) (type*)ctl_vec_new((type[]){__VA_ARGS__}, sizeof((type[]){__VA_ARGS__})/sizeof(type), sizeof(type))
#define ctl_vec_init_empty(type) (type*)ctl_vec_new(NULL, 0, sizeof(type))
#define ctl_vec_init_with_capacity(type, n) (type*)ctl_vec_with_capacity((n), sizeof(type))

#define ctl_vec_foreach(type, v, iter) for(type* v = iter; v != ctl_vec_end(iter); v++)

#define ctl_vptr(vec) (void**)&vec

typedef enum ctl_vecpos_t {
    CTL_VECPOS_FRONT,
    CTL_VECPOS_BACK,
} ctl_vecpos;

typedef struct ctl_vec_length_data_t {
    size_t len;
    size_t cap;
    size_t nbyte;
} ctl_vec_data;

CTL_EXTC_OPEN

void* ctl_vec_new(void* data, size_t len, size_t type_len);
void* ctl_vec_with_capacity(size_t cap, size_t type_len);
void  ctl_vec_free(void* vec);

void ctl_vec_reserve(void** vec, size_t cap);
void ctl_vec_resize( void** vec, size_t len);
void ctl_vec_push(   void** vec, ctl_vecpos pos, const void* item);
void ctl_vec_pop(    void** vec, ctl_vecpos pos, void* dst);
void ctl_vec_insert( void** vec, size_t index, const void* item);
void ctl_vec_erase(  void** vec, size_t index, void* dst);

void* ctl_vec_clone(void* vec);
void* ctl_vec_end(void* vec);
void* ctl_vec_last(void* vec);
void* ctl_vec_index(void* vec, size_t index);
size_t ctl_vec_len(void* vec);
size_t ctl_vec_capacity(void* vec);

CTL_EXTC_CLOSE

#endif
