#include <ctlvec.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>

ctl_vec_data* ctl_get_vec_data(void* vec){
    assert(vec);
    return (ctl_vec_data*)vec - 1;
}
void* ctl_get_vec_start(ctl_vec_data* vdata){
    assert(vdata);
    return (void*)(vdata + 1);
}

size_t ctl_vec_typelen(void* vec){
    return ctl_get_vec_data(vec)->nbyte;
}

void* ctl_vec_new(void* data, size_t len, size_t type_len){
    size_t byte_len = sizeof(ctl_vec_data) + len*type_len;
    ctl_vec_data* vec = malloc(byte_len);
    memset(vec, 0, byte_len);
    
    vec->len = len;
    vec->cap = len;
    vec->nbyte = type_len;

    void* start = ctl_get_vec_start(vec);
    memcpy(start, data, len*type_len * (data != NULL));

    return start;
}
void* ctl_vec_with_capacity(size_t cap, size_t type_len){
    size_t byte_len = sizeof(ctl_vec_data) + cap*type_len;
    ctl_vec_data* vec = malloc(byte_len);
    memset(vec, 0, byte_len);
    
    vec->len = 0;
    vec->cap = cap;
    vec->nbyte = type_len;

    void* start = ctl_get_vec_start(vec);

    return start;
}
void ctl_vec_free(void* vec){
    assert(vec);
    free(ctl_get_vec_data(vec));
}

void ctl_vec_reserve(void** ppvec, size_t cap){
    assert(ppvec);
    assert(*ppvec);

    void* vec = *ppvec;

    ctl_vec_data* data = ctl_get_vec_data(vec);
    size_t old_cap = data->cap;

    data = realloc(data, sizeof(ctl_vec_data)+cap*data->nbyte); 
    data->cap = cap;
    if(data->len > cap) data->len = cap;

    void* start = ctl_get_vec_start(data); 
    
    if(cap > old_cap) memset(ctl_vec_index(start, old_cap), 0, (cap-old_cap)*data->nbyte);

    *ppvec = start;
}
void ctl_vec_resize(void** ppvec, size_t len){
    assert(ppvec);
    assert(*ppvec);

    void* vec = *ppvec;
    
    ctl_vec_data* data = ctl_get_vec_data(vec);
    if(len > data->cap) {
        ctl_vec_reserve(&vec, len);
        data = ctl_get_vec_data(vec);
    }

    data->len = len;

    *ppvec = vec;
}

void ctl_vec_push(void** ppvec, ctl_vecpos pos, const void* item){
    assert(ppvec);
    assert(*ppvec);
    
    void* vec = *ppvec;

    switch(pos){
        case CTL_VECPOS_BACK: {
            ctl_vec_resize(&vec, ctl_vec_len(vec) + 1);
            memmove(ctl_vec_last(vec), item, ctl_vec_typelen(vec) * (item != NULL));
            break;
        }
        case CTL_VECPOS_FRONT: {
            ctl_vec_resize(&vec, ctl_vec_len(vec) + 1);
            memmove(ctl_vec_index(vec, 1), vec, ctl_vec_len(vec)*ctl_vec_typelen(vec));
            memmove(vec, item, ctl_vec_typelen(vec) * (item != NULL));
            break;
        }
    }

    *ppvec = vec;
}
void ctl_vec_pop(void** ppvec, ctl_vecpos pos, void* dst){
    assert(ppvec);
    assert(*ppvec);

    void* vec = *ppvec;

    switch(pos){
        case CTL_VECPOS_BACK: {
            memmove(dst, ctl_vec_index(vec, ctl_vec_len(vec)-1), ctl_vec_typelen(vec) * (dst != NULL));
            ctl_vec_resize(&vec, ctl_vec_len(vec)-1);
            break;
        }
        case CTL_VECPOS_FRONT: {
            void* clone = ctl_vec_clone(vec);
            ctl_vec_resize(&vec, ctl_vec_len(vec)-1);
            memcpy(vec, ctl_vec_index(clone, 1), ctl_vec_len(vec)*ctl_vec_typelen(vec));
            memcpy(dst, clone, ctl_vec_typelen(vec)*(dst != NULL));
            ctl_vec_free(clone);
            break;
        }
    }

    *ppvec = vec;
}
void ctl_vec_insert(void** ppvec, size_t index, const void* item){
    assert(ppvec);
    assert(*ppvec);
    
    void* vec = *ppvec;
    assert(index < ctl_vec_len(vec));

    ctl_vec_resize(&vec, ctl_vec_len(vec)+1);
    memmove(ctl_vec_index(vec, index+1), ctl_vec_index(vec, index), (ctl_vec_len(vec)-index)*ctl_vec_typelen(vec));
    memmove(ctl_vec_index(vec, index), item, ctl_vec_typelen(vec) * (item != NULL));

    *ppvec = vec;
}
void ctl_vec_erase(void** ppvec, size_t index, void* dst){
    assert(ppvec);
    assert(*ppvec);

    void* vec = *ppvec;
    assert(index < ctl_vec_len(vec));
    void* clone = ctl_vec_clone(vec);
    
    ctl_vec_resize(&vec, ctl_vec_len(vec)-1);
    memcpy(ctl_vec_index(vec, index), ctl_vec_index(clone, index+1), (ctl_vec_len(vec)-index)*ctl_vec_typelen(vec));
    memcpy(dst, ctl_vec_index(clone, index), ctl_vec_typelen(vec) * (dst != NULL));
    ctl_vec_free(clone);

    *ppvec = vec;
}

void* ctl_vec_clone(void* vec){
    return ctl_vec_new(vec, ctl_vec_len(vec), ctl_vec_typelen(vec));
}

void* ctl_vec_end(void* vec){
    assert(vec);
    return ctl_vec_index(vec, ctl_vec_len(vec));
}
void* ctl_vec_last(void* vec){
    assert(vec);
    assert(ctl_vec_len(vec) > 0);
    return ctl_vec_index(vec, ctl_vec_len(vec)-1);
}
void* ctl_vec_index(void* vec, size_t index){
    assert(vec);
    return (void*)((char*)vec + index * ctl_vec_typelen(vec));
}

size_t ctl_vec_len(void* vec){
    assert(vec);
    return ctl_get_vec_data(vec)->len;
}
size_t ctl_vec_capacity(void* vec){
    assert(vec);
    return ctl_get_vec_data(vec)->cap;
}
