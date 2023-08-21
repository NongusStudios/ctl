//! This file is under the MIT License.
//! See bottom of file. https://github.com/NongusStudios/ctl.git

#include "ctldef.h"

#include <stdlib.h>

#ifndef CTLVEC_H
#define CTLVEC_H

#define ctl_vec(type) type* /*! Used as a type qualifier for vectors. */
#define ctl_vec_init(type, ...) (type*)ctl_vec_new((type[]){__VA_ARGS__}, sizeof((type[]){__VA_ARGS__})/sizeof(type), sizeof(type)) /*! e.g clt_vec(int) v = ctl_vec_init(int, 1, 2, 3, 4, 5); */
#define ctl_vec_init_empty(type) (type*)ctl_vec_new(NULL, 0, sizeof(type)) /*! Initialises an empty vector of type. */
#define ctl_vec_init_with_capacity(type, n) (type*)ctl_vec_with_capacity((n), sizeof(type)) /*! Initialises an empty vector of type with capacity of n. */

#define ctl_vec_foreach(type, v, iter) for(type* v = iter; v != ctl_vec_end(iter); v++) /*! Allows for scuffed foreach emulation in C. */

#define ctl_vptr(vec) (void**)&vec /*! Helper macro so you can do this 'ctl_vec_push(ctl_vptr(vec), ...)' instead of 'ctl_vec_push(&(void*)vec, ...)' */

/*! Used to point to either end of a vector. */
typedef enum ctl_vecpos_t {
    CTL_VECPOS_FRONT, /*! Points to the first element of a vector. */
    CTL_VECPOS_BACK,  /*! Points to the last element of a vector.  */
} ctl_vecpos;

/*! Length data of a vector. */
typedef struct ctl_vec_length_data_t {
    size_t len; /*! Length of the vectors elements. */
    size_t cap; /*! Length of the vectors data. */
    size_t nbyte; /*! Size of an element in bytes. */
} ctl_vec_data;

CTL_EXTC_OPEN

/*!
 * Creates a new vector.
 * @param data An array of elements to populate the vector with. Each element must be of type_len (bytes) and contain len elements.
 *        Note: data can be left NULL and the data will be zeroed.
 * @param len How many elements to allocate. 
 * @param type_len Size of each element in bytes.
 * @return A pointer to the start of the vector's data.
*/
void* ctl_vec_new(void* data, size_t len, size_t type_len);

/*!
 * Creates a new vector with a chosen capacity, but a length of 0.
 * @param cap The capacity of the vector in elements.
 * @param type_len Size of each element in bytes.
 * @return A pointer to the start of the vector's data.
*/
void* ctl_vec_with_capacity(size_t cap, size_t type_len);

/*!
 * Frees the resources allocated by ctl_vec_new(...) and ctl_vec_with_capacity(...).
 * Not calling this before a vector goes out of scope will result in a memory leak.
 * @param vec The vector.
*/
void  ctl_vec_free(void* vec);

/*!
 * Sets the capacity of vec to cap.
 * @param vec Pointer to a vector.
 * @param cap Capacity to set vec to
*/
void ctl_vec_reserve(void** vec, size_t cap);

/*!
 * Resizes vec to len
 * @param[in, out] vec Pointer to a vector.
 * @param[in] len Length to resize to.
*/
void ctl_vec_resize( void** vec, size_t len);

/*!
 * Pushes an element to either end of vec.
 * @param[in, out] vec Pointer to a vector.
 * @param[in] pos Enum that sets which end of vec to push (CTL_VECPOS_BACK/CTL_VECPOS_FRONT).
 * @param[in] item Element data to push to vec.
*/
void ctl_vec_push(   void** vec, ctl_vecpos pos, const void* item);

/*!
 * Removes an element from either end of vec.
 * @param[in, out] vec Pointer to a vector.
 * @param[in] pos Enum that sets which end of vec to push (CTL_VECPOS_BACK/CTL_VECPOS_FRONT).
 * @param[out] dst Used to return the value of the removed element.
*/
void ctl_vec_pop(    void** vec, ctl_vecpos pos, void* dst);

/*!
 * Inserts an element at index.
 * a = [0, 1, 2, 3, 4, 5]
 * a.insert(2: index, 11: item)
 * a: [0, 1, 11, 2, 3, 4, 5]
 * @param[in, out] vec Pointer to a vector.
 * @param[in] index The index that item will be inserted at.
 * @param[in] item The data that will be inserted at index.
*/
void ctl_vec_insert( void** vec, size_t index, const void* item);

/*!
 * Erases an element at index.
 * @param[in, out] vec Pointer to a vector.
 * @param[in] index The index to erase.
 * @param[out] dest Used to return the data at index (size=type_len).
*/
void ctl_vec_erase(  void** vec, size_t index, void* dst);

/*!
 * Clears the vector but keeps capacity.
 * @param[in, out] vec Pointer to a vector.
*/
void ctl_vec_clear(  void** vec);

/*!
 * Creates a clone of vec.
 * @param vec The vector to clone.
 * @return The cloned vector.
*/
void* ctl_vec_clone(void* vec);

/*!
 * Gets the end of vec.
 * @return Pointer to the address after the last element.
*/
void* ctl_vec_end(void* vec);

/*!
 * Gets the last element of vec.
 * @return Pointer to last element of vec.
*/
void* ctl_vec_last(void* vec);

/*!
 * Indexes vec by index.
 * @param vec Vector to index.
 * @param index What to index vec by.
 * @return Pointer to the index.
*/
void* ctl_vec_index(void* vec, size_t index);

/*!
 * Gets the length of vec.
 * @param vec Vector that size will be fetched from.
 * @return The length of vec.
*/
size_t ctl_vec_len(void* vec);

/*!
 * Gets the capacity of vec.
 * @param vec Vector that capacity will be fetched from.
 * @return The capacity of vec.
*/
size_t ctl_vec_capacity(void* vec);

CTL_EXTC_CLOSE

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
