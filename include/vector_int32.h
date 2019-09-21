/**
 *  @file       vector_int.h
 *  @brief      Header for preprocessed template instantiation of vector(int)
 *
 *  @author     Gemuele Aludino
 *  @date       21 Sep 2019
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

#ifndef VECTOR_INT_H
#define VECTOR_INT_H

#include <stdlib.h>

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

/**
 *  @def        VECTOR_TMPL_DEFAULT_CAPACITY
 *  @brief      Default capacity for vector's internal buffer
 */
#define VECTOR_TMPL_DEFAULT_CAPACITY 16

/**
 *  @typedef    vector_int
 *  @brief      Alias for (struct vector_int)
 *
 *  All instances of (struct vector_int) will be addressed as (vector_int).
 */
typedef struct vector_int vector_int;

/**
 *  @typedef    vector_ptr_int
 *  @brief      Alias for (struct vector_int *) or (vector_int *)
 *
 *  All instances of (struct vector_int *) will be addressed as (vector_int *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct vector_int *vector_ptr_int;

/**
 *  @typedef    vector_dptr_int
 *  @brief      Alias for (struct vector_int **) or (vector_int **)
 *
 *  All instances of (struct vector_int **) will be addressed as (vector_int **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct vector_int **vector_dptr_int;

/**< vector_int: constructors */
vector_int *vnew_int(void);
vector_int *vnewr_int(size_t n);
vector_int *vnewfill_int(size_t n, int val);
vector_int *vnewfillptr_int(size_t n, int *valaddr);
vector_int *vnewrnge_int(iterator first, iterator last);
vector_int *vnewcopy_int(vector_int *v);
vector_int *vnewmove_int(vector_int **v);

/**< vector_int: destructor */
void vdelete_int(vector_int **v);

/**< vector_int: iterator functions */
iterator vbegin_int(vector_int *v);
iterator vend_int(vector_int *v);

/**< vector_int: length functions */
size_t vsize_int(vector_int *v);
size_t vmaxsize_int(vector_int *v);

/**< vector_int: resize functions */
void vresize_int(vector_int *v, size_t n);
void vresizefill_int(vector_int *v, size_t n, int val);
void vresizefillptr_int(vector_int *v, size_t n, int *valaddr);

/**< vector_int: capacity based functions */
size_t vcapacity_int(vector_int *v);
bool vempty_int(vector_int *v);

/**< vector_int: reserve/shrinking functions */
void vreserve_int(vector_int *v, size_t n);
void vshrinktofit_int(vector_int *v);

/**< vector_int: element access functions */
int *vat_int(vector_int *v, size_t n);
int *vfront_int(vector_int *v);
int *vback_int(vector_int *v);
int **vdata_int(vector_int *v);

/**< vector_int: element access functions with const qualifier */
const int *vatconst_int(vector_int *v, size_t n);
const int *vfrontconst_int(vector_int *v);
const int *vbackconst_int(vector_int *v);
const int **vdataconst_int(vector_int *v);

/**< vector_int: modifiers - assignement */
void vassignrnge_int(vector_int *v, iterator first, iterator last);
void vassignfill_int(vector_int *v, size_t n, int val);
void vassignfillptr_int(vector_int *v, size_t n, int *valaddr);

/**< vector_int: modifiers - push/pop */
void vpushb_int(vector_int *v, int val);
void vpushbptr_int(vector_int *v, int *valaddr);
void vpopb_int(vector_int *v);

/**< vector_int: modifiers - insertion */
iterator vinsert_int(vector_int *v, iterator pos, int val);
iterator vinsertptr_int(vector_int *v, iterator pos, int *val);
iterator vinsertfill_int(vector_int *v, iterator pos, size_t n, int val);
iterator vinsertfillptr_int(vector_int *v, iterator pos, size_t n, int *valaddr);
iterator vinsertrnge_int(vector_int *v, iterator pos, iterator first,
                         iterator last);
iterator vinsertmove_int(vector_int *v, iterator pos, int *valaddr);

/**< vector_int: modifiers - erasure */
iterator verase_int(vector_int *v, iterator pos);
iterator verasernge_int(vector_int *v, iterator pos, iterator last);

/**< vector_int: modifiers - container swappage */
void vswap_int(vector_int **v, vector_int **other);

/**< vector_int: modifiers - clear container */
void vclear_int(vector_int *v);

/**< vector_int: custom modifiers - insertion/erasure/replacement by index */
void vinsertat_int(vector_int *v, size_t index, int val);
void vinsertatptr_int(vector_int *v, size_t index, int *valaddr);
void veraseat_int(vector_int *v, size_t index);
void vreplaceat_int(vector_int *v, size_t index, int val);
void vreplaceatptr_int(vector_int *v, size_t index, int *valaddr);

/**< vector_int: custom modifiers - element swappage */
void vswapelem_int(vector_int *v, size_t n1, size_t n2);

/**< vector_int: custom modifiers - element removal by search */
void vremove_int(vector_int *v, int val);
void vremoveif_int(vector_int *v, bool (*unary_predicate)(const void *));

/**< vector_int: custom modifiers - merge/reverse */
vector_int *vmerge_int(vector_int *v, vector_int * other);
void vreverse_int(vector_int *v);

/**< vector_int: custom utility functions - array to vector/pointer to vector */
vector_int *varrtov_int(int *base, size_t length);
vector_int *vptrtov_int(int *base, size_t length, size_t capacity);

/**< vector_int: custom utility functions - search/sort by default comparator */
int vsearch_int(vector_int *v, int val);
void vsort_int(vector_int *v);

/**< vector_int: custom print functions - output to FILE stream */
void vputs_int(vector_int *v);
void vputsf_int(vector_int *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs_int(vector_int *v, FILE *dest);
void vfputsf_int(vector_int *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);

/**< vector_int: required function prototypes for (struct typetable) */
void *tmpl_vector_copy_int(void *arg, const void *other);
void tmpl_vector_dtor_int(void *arg);
void tmpl_vector_swap_int(void *s1, void *s2);
int tmpl_vector_compare_int(const void *c1, const void *c2);
void tmpl_vector_print_int(const void *arg, FILE *dest);

/**< vector_int: change default typetable */
void vsetttbl_int(vector_int *v, struct typetable *ttbl);

size_t vgetwidth_int(vector_int *v);
copy_fn vgetcopy_int(vector_int *v);
dtor_fn vgetdtor_int(vector_int *v);
swap_fn vgetswap_int(vector_int *v);
compare_fn vgetcompare_int(vector_int *v);
print_fn vgetprint_int(vector_int *v);
struct typetable *vgetttbl_int(vector_int *v);

/**< ptrs to vtables */
struct typetable *vector_typetable_ptr_id_int;
struct iterator_table *vector_iterator_table_ptr_id_int;

#endif /* VECTOR_INT_H */
