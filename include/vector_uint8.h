/**
 *  @file       vector_uint8.h
 *  @brief      Header for preprocessed template instantiation of vector(uint8_t)
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

#ifndef VECTOR_UINT8_H
#define VECTOR_UINT8_H

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
 *  @typedef    vector_uint8_t
 *  @brief      Alias for (struct vector_uint8_t)
 *
 *  All instances of (struct vector_uint8_t) will be addressed as (vector_uint8_t).
 */
typedef struct vector_uint8_t vector_uint8_t;

/**
 *  @typedef    vector_ptr_uint8_t
 *  @brief      Alias for (struct vector_uint8_t *) or (vector_uint8_t *)
 *
 *  All instances of (struct vector_uint8_t *) will be addressed as (vector_uint8_t *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct vector_uint8_t *vector_ptr_uint8_t;

/**
 *  @typedef    vector_dptr_uint8_t
 *  @brief      Alias for (struct vector_uint8_t **) or (vector_uint8_t **)
 *
 *  All instances of (struct vector_uint8_t **) will be addressed as (vector_uint8_t **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct vector_uint8_t **vector_dptr_uint8_t;

/**< vector_uint8_t: constructors */
vector_uint8_t *vnew_uint8_t(void);
vector_uint8_t *vnewr_uint8_t(size_t n);
vector_uint8_t *vnewfill_uint8_t(size_t n, uint8_t val);
vector_uint8_t *vnewfillptr_uint8_t(size_t n, uint8_t *valaddr);
vector_uint8_t *vnewrnge_uint8_t(iterator first, iterator last);
vector_uint8_t *vnewcopy_uint8_t(vector_uint8_t *v);
vector_uint8_t *vnewmove_uint8_t(vector_uint8_t **v);

/**< vector_uint8_t: destructor */
void vdelete_uint8_t(vector_uint8_t **v);

/**< vector_uint8_t: iterator functions */
iterator vbegin_uint8_t(vector_uint8_t *v);
iterator vend_uint8_t(vector_uint8_t *v);

/**< vector_uint8_t: length functions */
size_t vsize_uint8_t(vector_uint8_t *v);
size_t vmaxsize_uint8_t(vector_uint8_t *v);

/**< vector_uint8_t: resize functions */
void vresize_uint8_t(vector_uint8_t *v, size_t n);
void vresizefill_uint8_t(vector_uint8_t *v, size_t n, uint8_t val);
void vresizefillptr_uint8_t(vector_uint8_t *v, size_t n, uint8_t *valaddr);

/**< vector_uint8_t: capacity based functions */
size_t vcapacity_uint8_t(vector_uint8_t *v);
bool vempty_uint8_t(vector_uint8_t *v);

/**< vector_uint8_t: reserve/shrinking functions */
void vreserve_uint8_t(vector_uint8_t *v, size_t n);
void vshrinktofit_uint8_t(vector_uint8_t *v);

/**< vector_uint8_t: element access functions */
uint8_t *vat_uint8_t(vector_uint8_t *v, size_t n);
uint8_t *vfront_uint8_t(vector_uint8_t *v);
uint8_t *vback_uint8_t(vector_uint8_t *v);
uint8_t **vdata_uint8_t(vector_uint8_t *v);

/**< vector_uint8_t: element access functions with const qualifier */
const uint8_t *vatconst_uint8_t(vector_uint8_t *v, size_t n);
const uint8_t *vfrontconst_uint8_t(vector_uint8_t *v);
const uint8_t *vbackconst_uint8_t(vector_uint8_t *v);
const uint8_t **vdataconst_uint8_t(vector_uint8_t *v);

/**< vector_uint8_t: modifiers - assignement */
void vassignrnge_uint8_t(vector_uint8_t *v, iterator first, iterator last);
void vassignfill_uint8_t(vector_uint8_t *v, size_t n, uint8_t val);
void vassignfillptr_uint8_t(vector_uint8_t *v, size_t n, uint8_t *valaddr);

/**< vector_uint8_t: modifiers - push/pop */
void vpushb_uint8_t(vector_uint8_t *v, uint8_t val);
void vpushbptr_uint8_t(vector_uint8_t *v, uint8_t *valaddr);
void vpopb_uint8_t(vector_uint8_t *v);

/**< vector_uint8_t: modifiers - insertion */
iterator vinsert_uint8_t(vector_uint8_t *v, iterator pos, uint8_t val);
iterator vinsertptr_uint8_t(vector_uint8_t *v, iterator pos, uint8_t *val);
iterator vinsertfill_uint8_t(vector_uint8_t *v, iterator pos, size_t n, uint8_t val);
iterator vinsertfillptr_uint8_t(vector_uint8_t *v, iterator pos, size_t n, uint8_t *valaddr);
iterator vinsertrnge_uint8_t(vector_uint8_t *v, iterator pos, iterator first,
                         iterator last);
iterator vinsertmove_uint8_t(vector_uint8_t *v, iterator pos, uint8_t *valaddr);

/**< vector_uint8_t: modifiers - erasure */
iterator verase_uint8_t(vector_uint8_t *v, iterator pos);
iterator verasernge_uint8_t(vector_uint8_t *v, iterator pos, iterator last);

/**< vector_uint8_t: modifiers - container swappage */
void vswap_uint8_t(vector_uint8_t **v, vector_uint8_t **other);

/**< vector_uint8_t: modifiers - clear container */
void vclear_uint8_t(vector_uint8_t *v);

/**< vector_uint8_t: custom modifiers - insertion/erasure/replacement by index */
void vinsertat_uint8_t(vector_uint8_t *v, size_t index, uint8_t val);
void vinsertatptr_uint8_t(vector_uint8_t *v, size_t index, uint8_t *valaddr);
void veraseat_uint8_t(vector_uint8_t *v, size_t index);
void vreplaceat_uint8_t(vector_uint8_t *v, size_t index, uint8_t val);
void vreplaceatptr_uint8_t(vector_uint8_t *v, size_t index, uint8_t *valaddr);

/**< vector_uint8_t: custom modifiers - element swappage */
void vswapelem_uint8_t(vector_uint8_t *v, size_t n1, size_t n2);

/**< vector_uint8_t: custom modifiers - element removal by search */
void vremove_uint8_t(vector_uint8_t *v, uint8_t val);
void vremoveif_uint8_t(vector_uint8_t *v, bool (*unary_predicate)(const void *));

/**< vector_uint8_t: custom modifiers - merge/reverse */
vector_uint8_t *vmerge_uint8_t(vector_uint8_t *v, vector_uint8_t * other);
void vreverse_uint8_t(vector_uint8_t *v);

/**< vector_uint8_t: custom utility functions - array to vector/pointer to vector */
vector_uint8_t *varrtov_uint8_t(uint8_t *base, size_t length);
vector_uint8_t *vptrtov_uint8_t(uint8_t *base, size_t length, size_t capacity);

/**< vector_uint8_t: custom utility functions - search/sort by default comparator */
int vsearch_uint8_t(vector_uint8_t *v, uint8_t val);
void vsort_uint8_t(vector_uint8_t *v);

/**< vector_uint8_t: custom print functions - output to FILE stream */
void vputs_uint8_t(vector_uint8_t *v);
void vputsf_uint8_t(vector_uint8_t *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs_uint8_t(vector_uint8_t *v, FILE *dest);
void vfputsf_uint8_t(vector_uint8_t *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);

/**< vector_uint8_t: required function prototypes for (struct typetable) */
void *tmpl_vector_copy_uint8_t(void *arg, const void *other);
void tmpl_vector_dtor_uint8_t(void *arg);
void tmpl_vector_swap_uint8_t(void *s1, void *s2);
int tmpl_vector_compare_uint8_t(const void *c1, const void *c2);
void tmpl_vector_print_uint8_t(const void *arg, FILE *dest);

/**< vector_uint8_t: change default typetable */
void vsetttbl_uint8_t(vector_uint8_t *v, struct typetable *ttbl);

size_t vgetwidth_uint8_t(vector_uint8_t *v);
copy_fn vgetcopy_uint8_t(vector_uint8_t *v);
dtor_fn vgetdtor_uint8_t(vector_uint8_t *v);
swap_fn vgetswap_uint8_t(vector_uint8_t *v);
compare_fn vgetcompare_uint8_t(vector_uint8_t *v);
print_fn vgetprint_uint8_t(vector_uint8_t *v);
struct typetable *vgetttbl_uint8_t(vector_uint8_t *v);

/**< ptrs to vtables */
struct typetable *vector_typetable_ptr_id_uint8_t;
struct iterator_table *vector_iterator_table_ptr_id_uint8_t;

#endif /* VECTOR_UINT8_H */
