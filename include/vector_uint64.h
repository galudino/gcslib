/**
 *  @file       vector_uint64.h
 *  @brief      Header for preprocessed template instantiation of vector(uint64_t)
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

#ifndef VECTOR_UINT64_H
#define VECTOR_UINT64_H

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
 *  @typedef    vector_uint64_t
 *  @brief      Alias for (struct vector_uint64_t)
 *
 *  All instances of (struct vector_uint64_t) will be addressed as (vector_uint64_t).
 */
typedef struct vector_uint64_t vector_uint64_t;

/**
 *  @typedef    vector_ptr_uint64_t
 *  @brief      Alias for (struct vector_uint64_t *) or (vector_uint64_t *)
 *
 *  All instances of (struct vector_uint64_t *) will be addressed as (vector_uint64_t *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct vector_uint64_t *vector_ptr_uint64_t;

/**
 *  @typedef    vector_dptr_uint64_t
 *  @brief      Alias for (struct vector_uint64_t **) or (vector_uint64_t **)
 *
 *  All instances of (struct vector_uint64_t **) will be addressed as (vector_uint64_t **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct vector_uint64_t **vector_dptr_uint64_t;

/**< vector_uint64_t: constructors */
vector_uint64_t *vnew_uint64_t(void);
vector_uint64_t *vnewr_uint64_t(size_t n);
vector_uint64_t *vnewfill_uint64_t(size_t n, uint64_t val);
vector_uint64_t *vnewfillptr_uint64_t(size_t n, uint64_t *valaddr);
vector_uint64_t *vnewrnge_uint64_t(iterator first, iterator last);
vector_uint64_t *vnewcopy_uint64_t(vector_uint64_t *v);
vector_uint64_t *vnewmove_uint64_t(vector_uint64_t **v);

/**< vector_uint64_t: destructor */
void vdelete_uint64_t(vector_uint64_t **v);

/**< vector_uint64_t: iterator functions */
iterator vbegin_uint64_t(vector_uint64_t *v);
iterator vend_uint64_t(vector_uint64_t *v);

/**< vector_uint64_t: length functions */
size_t vsize_uint64_t(vector_uint64_t *v);
size_t vmaxsize_uint64_t(vector_uint64_t *v);

/**< vector_uint64_t: resize functions */
void vresize_uint64_t(vector_uint64_t *v, size_t n);
void vresizefill_uint64_t(vector_uint64_t *v, size_t n, uint64_t val);
void vresizefillptr_uint64_t(vector_uint64_t *v, size_t n, uint64_t *valaddr);

/**< vector_uint64_t: capacity based functions */
size_t vcapacity_uint64_t(vector_uint64_t *v);
bool vempty_uint64_t(vector_uint64_t *v);

/**< vector_uint64_t: reserve/shrinking functions */
void vreserve_uint64_t(vector_uint64_t *v, size_t n);
void vshrinktofit_uint64_t(vector_uint64_t *v);

/**< vector_uint64_t: element access functions */
uint64_t *vat_uint64_t(vector_uint64_t *v, size_t n);
uint64_t *vfront_uint64_t(vector_uint64_t *v);
uint64_t *vback_uint64_t(vector_uint64_t *v);
uint64_t **vdata_uint64_t(vector_uint64_t *v);

/**< vector_uint64_t: element access functions with const qualifier */
const uint64_t *vatconst_uint64_t(vector_uint64_t *v, size_t n);
const uint64_t *vfrontconst_uint64_t(vector_uint64_t *v);
const uint64_t *vbackconst_uint64_t(vector_uint64_t *v);
const uint64_t **vdataconst_uint64_t(vector_uint64_t *v);

/**< vector_uint64_t: modifiers - assignement */
void vassignrnge_uint64_t(vector_uint64_t *v, iterator first, iterator last);
void vassignfill_uint64_t(vector_uint64_t *v, size_t n, uint64_t val);
void vassignfillptr_uint64_t(vector_uint64_t *v, size_t n, uint64_t *valaddr);

/**< vector_uint64_t: modifiers - push/pop */
void vpushb_uint64_t(vector_uint64_t *v, uint64_t val);
void vpushbptr_uint64_t(vector_uint64_t *v, uint64_t *valaddr);
void vpopb_uint64_t(vector_uint64_t *v);

/**< vector_uint64_t: modifiers - insertion */
iterator vinsert_uint64_t(vector_uint64_t *v, iterator pos, uint64_t val);
iterator vinsertptr_uint64_t(vector_uint64_t *v, iterator pos, uint64_t *val);
iterator vinsertfill_uint64_t(vector_uint64_t *v, iterator pos, size_t n, uint64_t val);
iterator vinsertfillptr_uint64_t(vector_uint64_t *v, iterator pos, size_t n, uint64_t *valaddr);
iterator vinsertrnge_uint64_t(vector_uint64_t *v, iterator pos, iterator first,
                         iterator last);
iterator vinsertmove_uint64_t(vector_uint64_t *v, iterator pos, uint64_t *valaddr);

/**< vector_uint64_t: modifiers - erasure */
iterator verase_uint64_t(vector_uint64_t *v, iterator pos);
iterator verasernge_uint64_t(vector_uint64_t *v, iterator pos, iterator last);

/**< vector_uint64_t: modifiers - container swappage */
void vswap_uint64_t(vector_uint64_t **v, vector_uint64_t **other);

/**< vector_uint64_t: modifiers - clear container */
void vclear_uint64_t(vector_uint64_t *v);

/**< vector_uint64_t: custom modifiers - insertion/erasure/replacement by index */
void vinsertat_uint64_t(vector_uint64_t *v, size_t index, uint64_t val);
void vinsertatptr_uint64_t(vector_uint64_t *v, size_t index, uint64_t *valaddr);
void veraseat_uint64_t(vector_uint64_t *v, size_t index);
void vreplaceat_uint64_t(vector_uint64_t *v, size_t index, uint64_t val);
void vreplaceatptr_uint64_t(vector_uint64_t *v, size_t index, uint64_t *valaddr);

/**< vector_uint64_t: custom modifiers - element swappage */
void vswapelem_uint64_t(vector_uint64_t *v, size_t n1, size_t n2);

/**< vector_uint64_t: custom modifiers - element removal by search */
void vremove_uint64_t(vector_uint64_t *v, uint64_t val);
void vremoveif_uint64_t(vector_uint64_t *v, bool (*unary_predicate)(const void *));

/**< vector_uint64_t: custom modifiers - merge/reverse */
vector_uint64_t *vmerge_uint64_t(vector_uint64_t *v, vector_uint64_t * other);
void vreverse_uint64_t(vector_uint64_t *v);

/**< vector_uint64_t: custom utility functions - array to vector/pointer to vector */
vector_uint64_t *varrtov_uint64_t(uint64_t *base, size_t length);
vector_uint64_t *vptrtov_uint64_t(uint64_t *base, size_t length, size_t capacity);

/**< vector_uint64_t: custom utility functions - search/sort by default comparator */
int vsearch_uint64_t(vector_uint64_t *v, uint64_t val);
void vsort_uint64_t(vector_uint64_t *v);

/**< vector_uint64_t: custom print functions - output to FILE stream */
void vputs_uint64_t(vector_uint64_t *v);
void vputsf_uint64_t(vector_uint64_t *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs_uint64_t(vector_uint64_t *v, FILE *dest);
void vfputsf_uint64_t(vector_uint64_t *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);

/**< vector_uint64_t: required function prototypes for (struct typetable) */
void *tmpl_vector_copy_uint64_t(void *arg, const void *other);
void tmpl_vector_dtor_uint64_t(void *arg);
void tmpl_vector_swap_uint64_t(void *s1, void *s2);
int tmpl_vector_compare_uint64_t(const void *c1, const void *c2);
void tmpl_vector_print_uint64_t(const void *arg, FILE *dest);

/**< vector_uint64_t: change default typetable */
void vsetttbl_uint64_t(vector_uint64_t *v, struct typetable *ttbl);

size_t vgetwidth_uint64_t(vector_uint64_t *v);
copy_fn vgetcopy_uint64_t(vector_uint64_t *v);
dtor_fn vgetdtor_uint64_t(vector_uint64_t *v);
swap_fn vgetswap_uint64_t(vector_uint64_t *v);
compare_fn vgetcompare_uint64_t(vector_uint64_t *v);
print_fn vgetprint_uint64_t(vector_uint64_t *v);
struct typetable *vgetttbl_uint64_t(vector_uint64_t *v);

/**< ptrs to vtables */
struct typetable *vector_typetable_ptr_id_uint64_t;
struct iterator_table *vector_iterator_table_ptr_id_uint64_t;

#endif /* VECTOR_UINT64_H */
