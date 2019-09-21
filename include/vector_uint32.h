/**
 *  @file       vector_uint32.h
 *  @brief      Header for preprocessed template instantiation of vector(uint32_t)
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

#ifndef VECTOR_UINT32_H
#define VECTOR_UINT32_H

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
 *  @typedef    vector_uint32_t
 *  @brief      Alias for (struct vector_uint32_t)
 *
 *  All instances of (struct vector_uint32_t) will be addressed as (vector_uint32_t).
 */
typedef struct vector_uint32_t vector_uint32_t;

/**
 *  @typedef    vector_ptr_uint32_t
 *  @brief      Alias for (struct vector_uint32_t *) or (vector_uint32_t *)
 *
 *  All instances of (struct vector_uint32_t *) will be addressed as (vector_uint32_t *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct vector_uint32_t *vector_ptr_uint32_t;

/**
 *  @typedef    vector_dptr_uint32_t
 *  @brief      Alias for (struct vector_uint32_t **) or (vector_uint32_t **)
 *
 *  All instances of (struct vector_uint32_t **) will be addressed as (vector_uint32_t **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct vector_uint32_t **vector_dptr_uint32_t;

/**< vector_uint32_t: constructors */
vector_uint32_t *vnew_uint32_t(void);
vector_uint32_t *vnewr_uint32_t(size_t n);
vector_uint32_t *vnewfill_uint32_t(size_t n, uint32_t val);
vector_uint32_t *vnewfillptr_uint32_t(size_t n, uint32_t *valaddr);
vector_uint32_t *vnewrnge_uint32_t(iterator first, iterator last);
vector_uint32_t *vnewcopy_uint32_t(vector_uint32_t *v);
vector_uint32_t *vnewmove_uint32_t(vector_uint32_t **v);

/**< vector_uint32_t: destructor */
void vdelete_uint32_t(vector_uint32_t **v);

/**< vector_uint32_t: iterator functions */
iterator vbegin_uint32_t(vector_uint32_t *v);
iterator vend_uint32_t(vector_uint32_t *v);

/**< vector_uint32_t: length functions */
size_t vsize_uint32_t(vector_uint32_t *v);
size_t vmaxsize_uint32_t(vector_uint32_t *v);

/**< vector_uint32_t: resize functions */
void vresize_uint32_t(vector_uint32_t *v, size_t n);
void vresizefill_uint32_t(vector_uint32_t *v, size_t n, uint32_t val);
void vresizefillptr_uint32_t(vector_uint32_t *v, size_t n, uint32_t *valaddr);

/**< vector_uint32_t: capacity based functions */
size_t vcapacity_uint32_t(vector_uint32_t *v);
bool vempty_uint32_t(vector_uint32_t *v);

/**< vector_uint32_t: reserve/shrinking functions */
void vreserve_uint32_t(vector_uint32_t *v, size_t n);
void vshrinktofit_uint32_t(vector_uint32_t *v);

/**< vector_uint32_t: element access functions */
uint32_t *vat_uint32_t(vector_uint32_t *v, size_t n);
uint32_t *vfront_uint32_t(vector_uint32_t *v);
uint32_t *vback_uint32_t(vector_uint32_t *v);
uint32_t **vdata_uint32_t(vector_uint32_t *v);

/**< vector_uint32_t: element access functions with const qualifier */
const uint32_t *vatconst_uint32_t(vector_uint32_t *v, size_t n);
const uint32_t *vfrontconst_uint32_t(vector_uint32_t *v);
const uint32_t *vbackconst_uint32_t(vector_uint32_t *v);
const uint32_t **vdataconst_uint32_t(vector_uint32_t *v);

/**< vector_uint32_t: modifiers - assignement */
void vassignrnge_uint32_t(vector_uint32_t *v, iterator first, iterator last);
void vassignfill_uint32_t(vector_uint32_t *v, size_t n, uint32_t val);
void vassignfillptr_uint32_t(vector_uint32_t *v, size_t n, uint32_t *valaddr);

/**< vector_uint32_t: modifiers - push/pop */
void vpushb_uint32_t(vector_uint32_t *v, uint32_t val);
void vpushbptr_uint32_t(vector_uint32_t *v, uint32_t *valaddr);
void vpopb_uint32_t(vector_uint32_t *v);

/**< vector_uint32_t: modifiers - insertion */
iterator vinsert_uint32_t(vector_uint32_t *v, iterator pos, uint32_t val);
iterator vinsertptr_uint32_t(vector_uint32_t *v, iterator pos, uint32_t *val);
iterator vinsertfill_uint32_t(vector_uint32_t *v, iterator pos, size_t n, uint32_t val);
iterator vinsertfillptr_uint32_t(vector_uint32_t *v, iterator pos, size_t n, uint32_t *valaddr);
iterator vinsertrnge_uint32_t(vector_uint32_t *v, iterator pos, iterator first,
                         iterator last);
iterator vinsertmove_uint32_t(vector_uint32_t *v, iterator pos, uint32_t *valaddr);

/**< vector_uint32_t: modifiers - erasure */
iterator verase_uint32_t(vector_uint32_t *v, iterator pos);
iterator verasernge_uint32_t(vector_uint32_t *v, iterator pos, iterator last);

/**< vector_uint32_t: modifiers - container swappage */
void vswap_uint32_t(vector_uint32_t **v, vector_uint32_t **other);

/**< vector_uint32_t: modifiers - clear container */
void vclear_uint32_t(vector_uint32_t *v);

/**< vector_uint32_t: custom modifiers - insertion/erasure/replacement by index */
void vinsertat_uint32_t(vector_uint32_t *v, size_t index, uint32_t val);
void vinsertatptr_uint32_t(vector_uint32_t *v, size_t index, uint32_t *valaddr);
void veraseat_uint32_t(vector_uint32_t *v, size_t index);
void vreplaceat_uint32_t(vector_uint32_t *v, size_t index, uint32_t val);
void vreplaceatptr_uint32_t(vector_uint32_t *v, size_t index, uint32_t *valaddr);

/**< vector_uint32_t: custom modifiers - element swappage */
void vswapelem_uint32_t(vector_uint32_t *v, size_t n1, size_t n2);

/**< vector_uint32_t: custom modifiers - element removal by search */
void vremove_uint32_t(vector_uint32_t *v, uint32_t val);
void vremoveif_uint32_t(vector_uint32_t *v, bool (*unary_predicate)(const void *));

/**< vector_uint32_t: custom modifiers - merge/reverse */
vector_uint32_t *vmerge_uint32_t(vector_uint32_t *v, vector_uint32_t * other);
void vreverse_uint32_t(vector_uint32_t *v);

/**< vector_uint32_t: custom utility functions - array to vector/pointer to vector */
vector_uint32_t *varrtov_uint32_t(uint32_t *base, size_t length);
vector_uint32_t *vptrtov_uint32_t(uint32_t *base, size_t length, size_t capacity);

/**< vector_uint32_t: custom utility functions - search/sort by default comparator */
int vsearch_uint32_t(vector_uint32_t *v, uint32_t val);
void vsort_uint32_t(vector_uint32_t *v);

/**< vector_uint32_t: custom print functions - output to FILE stream */
void vputs_uint32_t(vector_uint32_t *v);
void vputsf_uint32_t(vector_uint32_t *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs_uint32_t(vector_uint32_t *v, FILE *dest);
void vfputsf_uint32_t(vector_uint32_t *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);

/**< vector_uint32_t: required function prototypes for (struct typetable) */
void *tmpl_vector_copy_uint32_t(void *arg, const void *other);
void tmpl_vector_dtor_uint32_t(void *arg);
void tmpl_vector_swap_uint32_t(void *s1, void *s2);
int tmpl_vector_compare_uint32_t(const void *c1, const void *c2);
void tmpl_vector_print_uint32_t(const void *arg, FILE *dest);

/**< vector_uint32_t: change default typetable */
void vsetttbl_uint32_t(vector_uint32_t *v, struct typetable *ttbl);

size_t vgetwidth_uint32_t(vector_uint32_t *v);
copy_fn vgetcopy_uint32_t(vector_uint32_t *v);
dtor_fn vgetdtor_uint32_t(vector_uint32_t *v);
swap_fn vgetswap_uint32_t(vector_uint32_t *v);
compare_fn vgetcompare_uint32_t(vector_uint32_t *v);
print_fn vgetprint_uint32_t(vector_uint32_t *v);
struct typetable *vgetttbl_uint32_t(vector_uint32_t *v);

/**< ptrs to vtables */
struct typetable *vector_typetable_ptr_id_uint32_t;
struct iterator_table *vector_iterator_table_ptr_id_uint32_t;

#endif /* VECTOR_UINT32_H */
