/**
 *  @file       vector_uint16.h
 *  @brief      Header for preprocessed template instantiation of vector(uint16_t)
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

#ifndef VECTOR_UINT16_H
#define VECTOR_UINT16_H

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
 *  @typedef    vector_uint16_t
 *  @brief      Alias for (struct vector_uint16_t)
 *
 *  All instances of (struct vector_uint16_t) will be addressed as (vector_uint16_t).
 */
typedef struct vector_uint16_t vector_uint16_t;

/**
 *  @typedef    vector_ptr_uint16_t
 *  @brief      Alias for (struct vector_uint16_t *) or (vector_uint16_t *)
 *
 *  All instances of (struct vector_uint16_t *) will be addressed as (vector_uint16_t *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct vector_uint16_t *vector_ptr_uint16_t;

/**
 *  @typedef    vector_dptr_uint16_t
 *  @brief      Alias for (struct vector_uint16_t **) or (vector_uint16_t **)
 *
 *  All instances of (struct vector_uint16_t **) will be addressed as (vector_uint16_t **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct vector_uint16_t **vector_dptr_uint16_t;

/**< vector_uint16_t: constructors */
vector_uint16_t *vnew_uint16_t(void);
vector_uint16_t *vnewr_uint16_t(size_t n);
vector_uint16_t *vnewfill_uint16_t(size_t n, uint16_t val);
vector_uint16_t *vnewfillptr_uint16_t(size_t n, uint16_t *valaddr);
vector_uint16_t *vnewrnge_uint16_t(iterator first, iterator last);
vector_uint16_t *vnewcopy_uint16_t(vector_uint16_t *v);
vector_uint16_t *vnewmove_uint16_t(vector_uint16_t **v);

/**< vector_uint16_t: destructor */
void vdelete_uint16_t(vector_uint16_t **v);

/**< vector_uint16_t: iterator functions */
iterator vbegin_uint16_t(vector_uint16_t *v);
iterator vend_uint16_t(vector_uint16_t *v);

/**< vector_uint16_t: length functions */
size_t vsize_uint16_t(vector_uint16_t *v);
size_t vmaxsize_uint16_t(vector_uint16_t *v);

/**< vector_uint16_t: resize functions */
void vresize_uint16_t(vector_uint16_t *v, size_t n);
void vresizefill_uint16_t(vector_uint16_t *v, size_t n, uint16_t val);
void vresizefillptr_uint16_t(vector_uint16_t *v, size_t n, uint16_t *valaddr);

/**< vector_uint16_t: capacity based functions */
size_t vcapacity_uint16_t(vector_uint16_t *v);
bool vempty_uint16_t(vector_uint16_t *v);

/**< vector_uint16_t: reserve/shrinking functions */
void vreserve_uint16_t(vector_uint16_t *v, size_t n);
void vshrinktofit_uint16_t(vector_uint16_t *v);

/**< vector_uint16_t: element access functions */
uint16_t *vat_uint16_t(vector_uint16_t *v, size_t n);
uint16_t *vfront_uint16_t(vector_uint16_t *v);
uint16_t *vback_uint16_t(vector_uint16_t *v);
uint16_t **vdata_uint16_t(vector_uint16_t *v);

/**< vector_uint16_t: element access functions with const qualifier */
const uint16_t *vatconst_uint16_t(vector_uint16_t *v, size_t n);
const uint16_t *vfrontconst_uint16_t(vector_uint16_t *v);
const uint16_t *vbackconst_uint16_t(vector_uint16_t *v);
const uint16_t **vdataconst_uint16_t(vector_uint16_t *v);

/**< vector_uint16_t: modifiers - assignement */
void vassignrnge_uint16_t(vector_uint16_t *v, iterator first, iterator last);
void vassignfill_uint16_t(vector_uint16_t *v, size_t n, uint16_t val);
void vassignfillptr_uint16_t(vector_uint16_t *v, size_t n, uint16_t *valaddr);

/**< vector_uint16_t: modifiers - push/pop */
void vpushb_uint16_t(vector_uint16_t *v, uint16_t val);
void vpushbptr_uint16_t(vector_uint16_t *v, uint16_t *valaddr);
void vpopb_uint16_t(vector_uint16_t *v);

/**< vector_uint16_t: modifiers - insertion */
iterator vinsert_uint16_t(vector_uint16_t *v, iterator pos, uint16_t val);
iterator vinsertptr_uint16_t(vector_uint16_t *v, iterator pos, uint16_t *val);
iterator vinsertfill_uint16_t(vector_uint16_t *v, iterator pos, size_t n, uint16_t val);
iterator vinsertfillptr_uint16_t(vector_uint16_t *v, iterator pos, size_t n, uint16_t *valaddr);
iterator vinsertrnge_uint16_t(vector_uint16_t *v, iterator pos, iterator first,
                         iterator last);
iterator vinsertmove_uint16_t(vector_uint16_t *v, iterator pos, uint16_t *valaddr);

/**< vector_uint16_t: modifiers - erasure */
iterator verase_uint16_t(vector_uint16_t *v, iterator pos);
iterator verasernge_uint16_t(vector_uint16_t *v, iterator pos, iterator last);

/**< vector_uint16_t: modifiers - container swappage */
void vswap_uint16_t(vector_uint16_t **v, vector_uint16_t **other);

/**< vector_uint16_t: modifiers - clear container */
void vclear_uint16_t(vector_uint16_t *v);

/**< vector_uint16_t: custom modifiers - insertion/erasure/replacement by index */
void vinsertat_uint16_t(vector_uint16_t *v, size_t index, uint16_t val);
void vinsertatptr_uint16_t(vector_uint16_t *v, size_t index, uint16_t *valaddr);
void veraseat_uint16_t(vector_uint16_t *v, size_t index);
void vreplaceat_uint16_t(vector_uint16_t *v, size_t index, uint16_t val);
void vreplaceatptr_uint16_t(vector_uint16_t *v, size_t index, uint16_t *valaddr);

/**< vector_uint16_t: custom modifiers - element swappage */
void vswapelem_uint16_t(vector_uint16_t *v, size_t n1, size_t n2);

/**< vector_uint16_t: custom modifiers - element removal by search */
void vremove_uint16_t(vector_uint16_t *v, uint16_t val);
void vremoveif_uint16_t(vector_uint16_t *v, bool (*unary_predicate)(const void *));

/**< vector_uint16_t: custom modifiers - merge/reverse */
vector_uint16_t *vmerge_uint16_t(vector_uint16_t *v, vector_uint16_t * other);
void vreverse_uint16_t(vector_uint16_t *v);

/**< vector_uint16_t: custom utility functions - array to vector/pointer to vector */
vector_uint16_t *varrtov_uint16_t(uint16_t *base, size_t length);
vector_uint16_t *vptrtov_uint16_t(uint16_t *base, size_t length, size_t capacity);

/**< vector_uint16_t: custom utility functions - search/sort by default comparator */
int vsearch_uint16_t(vector_uint16_t *v, uint16_t val);
void vsort_uint16_t(vector_uint16_t *v);

/**< vector_uint16_t: custom print functions - output to FILE stream */
void vputs_uint16_t(vector_uint16_t *v);
void vputsf_uint16_t(vector_uint16_t *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs_uint16_t(vector_uint16_t *v, FILE *dest);
void vfputsf_uint16_t(vector_uint16_t *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);

/**< vector_uint16_t: required function prototypes for (struct typetable) */
void *tmpl_vector_copy_uint16_t(void *arg, const void *other);
void tmpl_vector_dtor_uint16_t(void *arg);
void tmpl_vector_swap_uint16_t(void *s1, void *s2);
int tmpl_vector_compare_uint16_t(const void *c1, const void *c2);
void tmpl_vector_print_uint16_t(const void *arg, FILE *dest);

/**< vector_uint16_t: change default typetable */
void vsetttbl_uint16_t(vector_uint16_t *v, struct typetable *ttbl);

size_t vgetwidth_uint16_t(vector_uint16_t *v);
copy_fn vgetcopy_uint16_t(vector_uint16_t *v);
dtor_fn vgetdtor_uint16_t(vector_uint16_t *v);
swap_fn vgetswap_uint16_t(vector_uint16_t *v);
compare_fn vgetcompare_uint16_t(vector_uint16_t *v);
print_fn vgetprint_uint16_t(vector_uint16_t *v);
struct typetable *vgetttbl_uint16_t(vector_uint16_t *v);

/**< ptrs to vtables */
struct typetable *vector_typetable_ptr_id_uint16_t;
struct iterator_table *vector_iterator_table_ptr_id_uint16_t;

#endif /* VECTOR_UINT16_H */
