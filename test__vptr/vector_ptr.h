/**
 *  @file       vector_ptr.h
 *  @brief      Header file for a vector ADT (elements of any pointer type)
 *
 *  @author     Gemuele Aludino
 *  @date       04 Jan 2020
 *  @copyright  Copyright © 2020 Gemuele Aludino
 */
/**
 *  Copyright © 2020 Gemuele Aludino
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included
 *  in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
 *  THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef VECTOR_PTR_H
#define VECTOR_PTR_H

#include <stddef.h>
#include <stdbool.h>

/**
 *  @typedef    vptr_t
 *  @brief      Alias for struct vector_ptr
 */
typedef struct vector_ptr vptr_t;

/**
 *  @typedef    vector_ptr__iterator
 *  @brief      Alias for void **
 */
typedef void ** vector_ptr__iterator;

/**
 *  @typedef    vptr_iter_t
 *  @brief      Alias for vector_ptr__iterator
 */
typedef vector_ptr__iterator vptr_iter_t;

/**
 *  @struct     vector_ptr
 *  @brief      Represents a vector ADT (dynamically allocated array)
 *              Elements may be of any type (T *)   (pointer types).
 */
struct vector_ptr {
    struct vector_base_ptr {
        void **start;           /**< base address of internal buffer */
        void **finish;          /**< address of one-past the last element */
        void **end_of_storage;  /**< address of last block within start */
    } impl;
};

/**< vptr_init - explicitly called 'constructor' function */
vptr_t *vptr_init(vptr_t *self, size_t capacity);

/**< vptr_deinit - explicity called 'destructor' function */
vptr_t *vptr_deinit(vptr_t *self);

/**< vptr_front, vptr_back, vptr_at - element access */
void *vptr_front(vptr_t *self);
void *vptr_back(vptr_t *self);
void *vptr_at(vptr_t *self, int n);

/**< vptr_empty, vptr_size, vptr_capacity - size/capacity functions */
bool vptr_empty(vptr_t *self);
size_t vptr_size(vptr_t *self);
size_t vptr_capacity(vptr_t *self);

/**< vptr_resize, vptr_shrink_to_fit - resizing functions */
bool vptr_resize(vptr_t *self, size_t n);
bool vptr_shrink_to_fit(vptr_t *self);

/**< vptr_begin, vptr_end - iterator retrieval */
vptr_iter_t vptr_begin(vptr_t *self);
vptr_iter_t vptr_end(vptr_t *self);

/**< vptr_insert, vptr_erase - insert/erase in the middle */
vptr_iter_t vptr_insert(vptr_t *self, vptr_iter_t it, void *valaddr);
vptr_iter_t vptr_erase(vptr_t *self, vptr_iter_t it, void *tmp);

/**< vptr_pushb, vptr_popb, vptr_pushf, vptr_popf - stack/queue operations */
void vptr_pushb(vptr_t *self, void *valaddr);
void vptr_popb(vptr_t *self, void *tmp);
void vptr_pushf(vptr_t *self, void *valaddr);
void vptr_popf(vptr_t *self, void *tmp);

/**< vptr_search - linear search with a comparator, retrieve index */
int vptr_search(vptr_iter_t base, int (*cmpfn)(const void *, const void *), void *valaddr);

/**< vptr_find - linear search with a comparator, retrieve iterator */
vptr_iter_t vptr_find(vptr_iter_t base, int (*cmpfn)(const void *, const void *), void *valaddr);

/**< vptr_qsort - quicksort with a comparator, retrieve iterator */
vptr_iter_t vptr_qsort(vptr_t *self, int (*cmpfn)(const void *, const void *));

/**
 *  @define     vptr_new
 *  @brief      Macro to allocate and initialize a vptr_t on the heap
 */
#define vptr_new(capacity)  vptr_init(malloc(sizeof(vptr_t)), capacity)

/**
 *  @define     vptr_delete
 *  @brief      Macro to destroy and deallocate a vptr_t from the heap
 */
#define vptr_delete(self)   free(vptr_deinit(self)); self = NULL

#endif /* VECTOR_PTR_H */

