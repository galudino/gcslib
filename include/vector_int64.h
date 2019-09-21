/**
 *  @file       vector_int64.h
 *  @brief      Header for preprocessed template instantiation of vector(int64_t)
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

#ifndef VECTOR_INT64_H
#define VECTOR_INT64_H

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
 *  @typedef    vector_int64_t
 *  @brief      Alias for (struct vector_int64_t)
 *
 *  All instances of (struct vector_int64_t) will be addressed as (vector_int64_t).
 */
typedef struct vector_int64_t vector_int64_t;

/**
 *  @typedef    vector_ptr_int64_t
 *  @brief      Alias for (struct vector_int64_t *) or (vector_int64_t *)
 *
 *  All instances of (struct vector_int64_t *) will be addressed as (vector_int64_t *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct vector_int64_t *vector_ptr_int64_t;

/**
 *  @typedef    vector_dptr_int64_t
 *  @brief      Alias for (struct vector_int64_t **) or (vector_int64_t **)
 *
 *  All instances of (struct vector_int64_t **) will be addressed as (vector_int64_t **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct vector_int64_t **vector_dptr_int64_t;

/**< vector_int64_t: constructors */
vector_int64_t *vnew_int64_t(void);
vector_int64_t *vnewr_int64_t(size_t n);
vector_int64_t *vnewfill_int64_t(size_t n, int64_t val);
vector_int64_t *vnewfillptr_int64_t(size_t n, int64_t *valaddr);
vector_int64_t *vnewrnge_int64_t(iterator first, iterator last);
vector_int64_t *vnewcopy_int64_t(vector_int64_t *v);
vector_int64_t *vnewmove_int64_t(vector_int64_t **v);

/**< vector_int64_t: destructor */
void vdelete_int64_t(vector_int64_t **v);

/**< vector_int64_t: iterator functions */
iterator vbegin_int64_t(vector_int64_t *v);
iterator vend_int64_t(vector_int64_t *v);

/**< vector_int64_t: length functions */
size_t vsize_int64_t(vector_int64_t *v);
size_t vmaxsize_int64_t(vector_int64_t *v);

/**< vector_int64_t: resize functions */
void vresize_int64_t(vector_int64_t *v, size_t n);
void vresizefill_int64_t(vector_int64_t *v, size_t n, int64_t val);
void vresizefillptr_int64_t(vector_int64_t *v, size_t n, int64_t *valaddr);

/**< vector_int64_t: capacity based functions */
size_t vcapacity_int64_t(vector_int64_t *v);
bool vempty_int64_t(vector_int64_t *v);

/**< vector_int64_t: reserve/shrinking functions */
void vreserve_int64_t(vector_int64_t *v, size_t n);
void vshrinktofit_int64_t(vector_int64_t *v);

/**< vector_int64_t: element access functions */
int64_t *vat_int64_t(vector_int64_t *v, size_t n);
int64_t *vfront_int64_t(vector_int64_t *v);
int64_t *vback_int64_t(vector_int64_t *v);
int64_t **vdata_int64_t(vector_int64_t *v);

/**< vector_int64_t: element access functions with const qualifier */
const int64_t *vatconst_int64_t(vector_int64_t *v, size_t n);
const int64_t *vfrontconst_int64_t(vector_int64_t *v);
const int64_t *vbackconst_int64_t(vector_int64_t *v);
const int64_t **vdataconst_int64_t(vector_int64_t *v);

/**< vector_int64_t: modifiers - assignement */
void vassignrnge_int64_t(vector_int64_t *v, iterator first, iterator last);
void vassignfill_int64_t(vector_int64_t *v, size_t n, int64_t val);
void vassignfillptr_int64_t(vector_int64_t *v, size_t n, int64_t *valaddr);

/**< vector_int64_t: modifiers - push/pop */
void vpushb_int64_t(vector_int64_t *v, int64_t val);
void vpushbptr_int64_t(vector_int64_t *v, int64_t *valaddr);
void vpopb_int64_t(vector_int64_t *v);

/**< vector_int64_t: modifiers - insertion */
iterator vinsert_int64_t(vector_int64_t *v, iterator pos, int64_t val);
iterator vinsertptr_int64_t(vector_int64_t *v, iterator pos, int64_t *val);
iterator vinsertfill_int64_t(vector_int64_t *v, iterator pos, size_t n, int64_t val);
iterator vinsertfillptr_int64_t(vector_int64_t *v, iterator pos, size_t n, int64_t *valaddr);
iterator vinsertrnge_int64_t(vector_int64_t *v, iterator pos, iterator first,
                         iterator last);
iterator vinsertmove_int64_t(vector_int64_t *v, iterator pos, int64_t *valaddr);

/**< vector_int64_t: modifiers - erasure */
iterator verase_int64_t(vector_int64_t *v, iterator pos);
iterator verasernge_int64_t(vector_int64_t *v, iterator pos, iterator last);

/**< vector_int64_t: modifiers - container swappage */
void vswap_int64_t(vector_int64_t **v, vector_int64_t **other);

/**< vector_int64_t: modifiers - clear container */
void vclear_int64_t(vector_int64_t *v);

/**< vector_int64_t: custom modifiers - insertion/erasure/replacement by index */
void vinsertat_int64_t(vector_int64_t *v, size_t index, int64_t val);
void vinsertatptr_int64_t(vector_int64_t *v, size_t index, int64_t *valaddr);
void veraseat_int64_t(vector_int64_t *v, size_t index);
void vreplaceat_int64_t(vector_int64_t *v, size_t index, int64_t val);
void vreplaceatptr_int64_t(vector_int64_t *v, size_t index, int64_t *valaddr);

/**< vector_int64_t: custom modifiers - element swappage */
void vswapelem_int64_t(vector_int64_t *v, size_t n1, size_t n2);

/**< vector_int64_t: custom modifiers - element removal by search */
void vremove_int64_t(vector_int64_t *v, int64_t val);
void vremoveif_int64_t(vector_int64_t *v, bool (*unary_predicate)(const void *));

/**< vector_int64_t: custom modifiers - merge/reverse */
vector_int64_t *vmerge_int64_t(vector_int64_t *v, vector_int64_t * other);
void vreverse_int64_t(vector_int64_t *v);

/**< vector_int64_t: custom utility functions - array to vector/pointer to vector */
vector_int64_t *varrtov_int64_t(int64_t *base, size_t length);
vector_int64_t *vptrtov_int64_t(int64_t *base, size_t length, size_t capacity);

/**< vector_int64_t: custom utility functions - search/sort by default comparator */
int vsearch_int64_t(vector_int64_t *v, int64_t val);
void vsort_int64_t(vector_int64_t *v);

/**< vector_int64_t: custom print functions - output to FILE stream */
void vputs_int64_t(vector_int64_t *v);
void vputsf_int64_t(vector_int64_t *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs_int64_t(vector_int64_t *v, FILE *dest);
void vfputsf_int64_t(vector_int64_t *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);

/**< vector_int64_t: required function prototypes for (struct typetable) */
void *tmpl_vector_copy_int64_t(void *arg, const void *other);
void tmpl_vector_dtor_int64_t(void *arg);
void tmpl_vector_swap_int64_t(void *s1, void *s2);
int tmpl_vector_compare_int64_t(const void *c1, const void *c2);
void tmpl_vector_print_int64_t(const void *arg, FILE *dest);

/**< vector_int64_t: change default typetable */
void vsetttbl_int64_t(vector_int64_t *v, struct typetable *ttbl);

size_t vgetwidth_int64_t(vector_int64_t *v);
copy_fn vgetcopy_int64_t(vector_int64_t *v);
dtor_fn vgetdtor_int64_t(vector_int64_t *v);
swap_fn vgetswap_int64_t(vector_int64_t *v);
compare_fn vgetcompare_int64_t(vector_int64_t *v);
print_fn vgetprint_int64_t(vector_int64_t *v);
struct typetable *vgetttbl_int64_t(vector_int64_t *v);

/**< ptrs to vtables */
struct typetable *vector_typetable_ptr_id_int64_t;
struct iterator_table *vector_iterator_table_ptr_id_int64_t;

#endif /* VECTOR_INT64_H */
