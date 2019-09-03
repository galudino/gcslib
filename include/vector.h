/**
 *  @file       vector.h
 *  @brief      Header file for a dynamic array ADT
 *
 *  @author     Gemuele Aludino
 *  @date       24 Aug 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */
/**
 *  Copyright © 2019 Gemuele Aludino
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

#ifndef VECTOR_H
#define VECTOR_H

/**
 *  @file       utils.h
 *  @brief      Required for (struct typetable) and related functions
 */
#include "utils.h"

/**
 *  @file       iterator.h
 *  @brief      Required for iterator (struct iterator) and related functions
 */
#include "iterator.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *  @def        VECTOR_DEFAULT_CAPACITY
 *  @brief      Default capacity for vector's internal buffer
 */
#define VECTOR_DEFAULT_CAPACITY 16

/**
 *  @typedef    vector
 *  @brief      Alias for (struct vector)
 *
 *  All instances of (struct vector) will be addressed as (vector).
 */
typedef struct vector vector;

/**
 *  @typedef    vector_ptr
 *  @brief      Alias for (struct vector *) or (vector *)
 *
 *  All instances of (struct vector *) will be addressed as (vector *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct vector *vector_ptr;

/**
 *  @typedef    vector_dptr
 *  @brief      Alias for (struct vector **) or (vector **)
 *
 *  All instances of (struct vector **) will be addressed as (vector **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct vector **vector_dptr;

/**
 *      By default, elements are deep copied into the containers,
 *      iff the typetable provided upon instantiation has a copy function
 *      that performs a deep copy of its argument.
 *
 *      If there is no deep copy function provided in the typetable,
 *      elements are shallow copied.
 *
 *      "fill" functions always create deep copies of elements when used.
 *      (provided a deep copy function is defined in the typetable)
 */

/**< vector: allocate and construct */
vector *v_new(struct typetable *ttbl);
vector *v_newr(struct typetable *ttbl, size_t n);
vector *v_newfill(struct typetable *ttbl, size_t n, void *valaddr);
vector *v_newrnge(iterator first, iterator last);
vector *v_newcopy(vector *v);
vector *v_newmove(vector **v);

/**< vector: destruct and deallocate */
void v_delete(vector **v);

/**< vector: iterator functions */
iterator v_begin(vector *v);
iterator v_end(vector *v);

/**< vector: length functions */
size_t v_size(vector *v);
size_t v_maxsize(vector *v);

/**< vector: resize functions */
void v_resize(vector *v, size_t n);
void v_resizefill(vector *v, size_t n, const void *valaddr);

/**< vector: capacity based functions */
size_t v_capacity(vector *v);
bool v_empty(vector *v);

/**< vector: reserve/shrinking functions */
void v_reserve(vector *v, size_t n);
void v_shrink_to_fit(vector *v);

/**< vector: element access functions */
void *v_at(vector *v, size_t n);
void *v_front(vector *v);
void *v_back(vector *v);
void *v_data(vector *v);

/**< vector: element access functions w/ const qualifier */
const void *v_at_const(vector *v, size_t n);
const void *v_front_const(vector *v);
const void *v_back_const(vector *v);
const void *v_data_const(vector *v);

/**< vector: modifiers - assignment */
void v_assignrnge(vector *v, iterator first, iterator last);
void v_assignfill(vector *v, size_t n, const void *valaddr);

/**< vector: modifiers - push/pop */
void v_pushb(vector *v, const void *valaddr);
void v_popb(vector *v);

/**< vector: modifiers - insertion */
iterator v_insert(vector *v, iterator pos, const void *valaddr);
iterator v_insertfill(vector *v, iterator pos, size_t n, const void *valaddr);
iterator v_insertrnge(vector *v, iterator pos, iterator first, iterator last);
iterator v_insertmove(vector *v, iterator pos, void *valaddr);

/**< vector: modifiers - erasure */
iterator v_erase(vector *v, iterator pos);
iterator v_erasernge(vector *v, iterator pos, iterator last);

/**< vector: modifiers - container swappage */
void v_swap(vector **v, vector **other);

/**< vector: modifiers - clear container */
void v_clear(vector *v);

/**< vector: custom modifiers - insertion/erasure/replacement by index */
void v_insert_at(vector *v, size_t index, const void *valaddr);
void v_erase_at(vector *v, size_t index);
void v_replace_at(vector *v, size_t index, const void *valaddr);

/**< vector: custom modifiers - element swappage */
void v_swap_elem(vector *v, size_t n1, size_t n2);

/**< vector: custom modifiers - element removal by search */
void v_remove(vector *v, const void *valaddr);
void v_remove_if(vector *v, const void *valaddr,
                 bool (*unary_predicate)(const void *));


/**< vector: custom modifiers - merge/reverse */
vector *v_merge(vector *v, vector *other);
void v_reverse(vector *v);

/**< vector: custom utility functions - arrtov */
vector *v_arrtov(struct typetable *ttbl, void *base, size_t length);

/**< vector: custom utility functions - search / sort by default comparator */
int v_search(vector *v, const void *valaddr);
void v_sort(vector *v);

/**< vector: custom print functions - output to FILE stream */
void v_puts(vector *v);

void v_putsf(vector *v, const char *before, const char *after,
             const char *postelem, const char *empty, size_t breaklim);

void v_fputs(vector *v, FILE *dest);

void v_fputsf(vector *v, FILE *dest, const char *before, const char *after,
              const char *postelem, const char *empty, size_t breaklim);

/**< vector: required function prototypes for (struct typetable) */
void *vector_copy(void *arg, const void *other);
void vector_dtor(void *arg);
void vector_swap(void *s1, void *s2);
int vector_compare(const void *c1, const void *c2);
void vector_print(const void *arg, FILE *dest);

/**< vector: change typetable */
void v_set_ttbl(vector *v, struct typetable *ttbl);

/**< vector: retrieve width/copy/dtor/swap/compare/print/typetable */
size_t v_get_width(vector *v);
copy_fn v_get_copy(vector *v);
dtor_fn v_get_dtor(vector *v);
swap_fn v_get_swap(vector *v);
compare_fn v_get_compare(vector *v);
print_fn v_get_print(vector *v);
struct typetable *v_get_ttbl(vector *v);

/**< ptrs to vtables */
extern struct typetable *_vector_;
extern struct iterator_table *_vector_iterator_;

#endif /* VECTOR_H */
