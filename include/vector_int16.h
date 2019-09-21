/**
 *  @file       vector_short.h
 *  @brief      Header for preprocessed template instantiation of vector(short)
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

#ifndef VECTOR_SHORT_H
#define VECTOR_SHORT_H

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
 *  @typedef    vector_short
 *  @brief      Alias for (struct vector_short)
 *
 *  All instances of (struct vector_short) will be addressed as (vector_short).
 */
typedef struct vector_short vector_short;

/**
 *  @typedef    vector_ptr_short
 *  @brief      Alias for (struct vector_short *) or (vector_short *)
 *
 *  All instances of (struct vector_short *) will be addressed as (vector_short *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct vector_short *vector_ptr_short;

/**
 *  @typedef    vector_dptr_short
 *  @brief      Alias for (struct vector_short **) or (vector_short **)
 *
 *  All instances of (struct vector_short **) will be addressed as (vector_short **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct vector_short **vector_dptr_short;

/**< vector_short: constructors */
vector_short *vnew_short(void);
vector_short *vnewr_short(size_t n);
vector_short *vnewfill_short(size_t n, short val);
vector_short *vnewfillptr_short(size_t n, short *valaddr);
vector_short *vnewrnge_short(iterator first, iterator last);
vector_short *vnewcopy_short(vector_short *v);
vector_short *vnewmove_short(vector_short **v);

/**< vector_short: destructor */
void vdelete_short(vector_short **v);

/**< vector_short: iterator functions */
iterator vbegin_short(vector_short *v);
iterator vend_short(vector_short *v);

/**< vector_short: length functions */
size_t vsize_short(vector_short *v);
size_t vmaxsize_short(vector_short *v);

/**< vector_short: resize functions */
void vresize_short(vector_short *v, size_t n);
void vresizefill_short(vector_short *v, size_t n, short val);
void vresizefillptr_short(vector_short *v, size_t n, short *valaddr);

/**< vector_short: capacity based functions */
size_t vcapacity_short(vector_short *v);
bool vempty_short(vector_short *v);

/**< vector_short: reserve/shrinking functions */
void vreserve_short(vector_short *v, size_t n);
void vshrinktofit_short(vector_short *v);

/**< vector_short: element access functions */
short *vat_short(vector_short *v, size_t n);
short *vfront_short(vector_short *v);
short *vback_short(vector_short *v);
short **vdata_short(vector_short *v);

/**< vector_short: element access functions with const qualifier */
const short *vatconst_short(vector_short *v, size_t n);
const short *vfrontconst_short(vector_short *v);
const short *vbackconst_short(vector_short *v);
const short **vdataconst_short(vector_short *v);

/**< vector_short: modifiers - assignement */
void vassignrnge_short(vector_short *v, iterator first, iterator last);
void vassignfill_short(vector_short *v, size_t n, short val);
void vassignfillptr_short(vector_short *v, size_t n, short *valaddr);

/**< vector_short: modifiers - push/pop */
void vpushb_short(vector_short *v, short val);
void vpushbptr_short(vector_short *v, short *valaddr);
void vpopb_short(vector_short *v);

/**< vector_short: modifiers - insertion */
iterator vinsert_short(vector_short *v, iterator pos, short val);
iterator vinsertptr_short(vector_short *v, iterator pos, short *val);
iterator vinsertfill_short(vector_short *v, iterator pos, size_t n, short val);
iterator vinsertfillptr_short(vector_short *v, iterator pos, size_t n, short *valaddr);
iterator vinsertrnge_short(vector_short *v, iterator pos, iterator first,
                         iterator last);
iterator vinsertmove_short(vector_short *v, iterator pos, short *valaddr);

/**< vector_short: modifiers - erasure */
iterator verase_short(vector_short *v, iterator pos);
iterator verasernge_short(vector_short *v, iterator pos, iterator last);

/**< vector_short: modifiers - container swappage */
void vswap_short(vector_short **v, vector_short **other);

/**< vector_short: modifiers - clear container */
void vclear_short(vector_short *v);

/**< vector_short: custom modifiers - insertion/erasure/replacement by index */
void vinsertat_short(vector_short *v, size_t index, short val);
void vinsertatptr_short(vector_short *v, size_t index, short *valaddr);
void veraseat_short(vector_short *v, size_t index);
void vreplaceat_short(vector_short *v, size_t index, short val);
void vreplaceatptr_short(vector_short *v, size_t index, short *valaddr);

/**< vector_short: custom modifiers - element swappage */
void vswapelem_short(vector_short *v, size_t n1, size_t n2);

/**< vector_short: custom modifiers - element removal by search */
void vremove_short(vector_short *v, short val);
void vremoveif_short(vector_short *v, bool (*unary_predicate)(const void *));

/**< vector_short: custom modifiers - merge/reverse */
vector_short *vmerge_short(vector_short *v, vector_short * other);
void vreverse_short(vector_short *v);

/**< vector_short: custom utility functions - array to vector/pointer to vector */
vector_short *varrtov_short(short *base, size_t length);
vector_short *vptrtov_short(short *base, size_t length, size_t capacity);

/**< vector_short: custom utility functions - search/sort by default comparator */
int vsearch_short(vector_short *v, short val);
void vsort_short(vector_short *v);

/**< vector_short: custom print functions - output to FILE stream */
void vputs_short(vector_short *v);
void vputsf_short(vector_short *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs_short(vector_short *v, FILE *dest);
void vfputsf_short(vector_short *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);

/**< vector_short: required function prototypes for (struct typetable) */
void *tmpl_vector_copy_short(void *arg, const void *other);
void tmpl_vector_dtor_short(void *arg);
void tmpl_vector_swap_short(void *s1, void *s2);
int tmpl_vector_compare_short(const void *c1, const void *c2);
void tmpl_vector_print_short(const void *arg, FILE *dest);

/**< vector_short: change default typetable */
void vsetttbl_short(vector_short *v, struct typetable *ttbl);

size_t vgetwidth_short(vector_short *v);
copy_fn vgetcopy_short(vector_short *v);
dtor_fn vgetdtor_short(vector_short *v);
swap_fn vgetswap_short(vector_short *v);
compare_fn vgetcompare_short(vector_short *v);
print_fn vgetprint_short(vector_short *v);
struct typetable *vgetttbl_short(vector_short *v);

/**< ptrs to vtables */
struct typetable *vector_typetable_ptr_id_short;
struct iterator_table *vector_iterator_table_ptr_id_short;

#endif /* VECTOR_SHORT_H */
