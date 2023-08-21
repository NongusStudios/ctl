#include <ctlmap.h>

#include <assert.h>
#include <stdbool.h>

bool ctl_map_key_iseq(const void* a, size_t alen, const void* b, size_t blen){
    assert(a && b);
    if(alen != blen) return false;
    
    const char* ac = a;
    const char* bc = b;
    for(size_t i = 0; i < alen; i++){
        if(*ac++ != *bc++) return false;
    }

    return true;
}
