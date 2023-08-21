//! This file is under the MIT License.
//! See bottom of file. https://github.com/NongusStudios/ctl.git

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

/*!
 * MIT License
 * 
 * Copyright (c) 2023 Nongus Studios
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/
