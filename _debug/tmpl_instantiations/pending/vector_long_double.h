/**
 *  @file       vector_long_double.h
 *  @brief      Header for preprocessed template instantiation of vector(long_double)
 *
 *  @author     Gemuele Aludino
 *  @date       21 Sep 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#ifndef VECTOR_LONG_DOUBLE_H */
#define VECTOR_LONG_DOUBLE_H */
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
 *  @typedef    vector_long_double
 *  @brief      Alias for (struct vector_long_double)
 *
 *  All instances of (struct vector_long_double) will be addressed as (vector_long_double).
 */
typedef struct vector_long_double vector_long_double;

/**
 *  @typedef    vector_ptr_long_double
 *  @brief      Alias for (struct vector_long_double *) or (vector_long_double *)
 *
 *  All instances of (struct vector_long_double *) will be addressed as (vector_long_double *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct vector_long_double *vector_ptr_long_double;

/**
 *  @typedef    vector_dptr_long_double
 *  @brief      Alias for (struct vector_long_double **) or (vector_long_double **)
 *
 *  All instances of (struct vector_long_double **) will be addressed as (vector_long_double **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct vector_long_double **vector_dptr_long_double;

/**< vector_long_double: constructors */
vector_long_double *vnew_long_double(void);
vector_long_double *vnewr_long_double(size_t n);
vector_long_double *vnewfill_long_double(size_t n, long_double val);
vector_long_double *vnewfillptr_long_double(size_t n, long_double *valaddr);
vector_long_double *vnewrnge_long_double(iterator first, iterator last);
vector_long_double *vnewcopy_long_double(vector_long_double *v);
vector_long_double *vnewmove_long_double(vector_long_double **v);

/**< vector_long_double: destructor */
void vdelete_long_double(vector_long_double **v);

/**< vector_long_double: iterator functions */
iterator vbegin_long_double(vector_long_double *v);
iterator vend_long_double(vector_long_double *v);

/**< vector_long_double: length functions */
size_t vsize_long_double(vector_long_double *v);
size_t vmaxsize_long_double(vector_long_double *v);

/**< vector_long_double: resize functions */
void vresize_long_double(vector_long_double *v, size_t n);
void vresizefill_long_double(vector_long_double *v, size_t n, long_double val);
void vresizefillptr_long_double(vector_long_double *v, size_t n, long_double *valaddr);

/**< vector_long_double: capacity based functions */
size_t vcapacity_long_double(vector_long_double *v);
bool vempty_long_double(vector_long_double *v);

/**< vector_long_double: reserve/shrinking functions */
void vreserve_long_double(vector_long_double *v, size_t n);
void vshrinktofit_long_double(vector_long_double *v);

/**< vector_long_double: element access functions */
long_double *vat_long_double(vector_long_double *v, size_t n);
long_double *vfront_long_double(vector_long_double *v);
long_double *vback_long_double(vector_long_double *v);
long_double **vdata_long_double(vector_long_double *v);

/**< vector_long_double: element access functions with const qualifier */
const long_double *vatconst_long_double(vector_long_double *v, size_t n);
const long_double *vfrontconst_long_double(vector_long_double *v);
const long_double *vbackconst_long_double(vector_long_double *v);
const long_double **vdataconst_long_double(vector_long_double *v);

/**< vector_long_double: modifiers - assignement */
void vassignrnge_long_double(vector_long_double *v, iterator first, iterator last);
void vassignfill_long_double(vector_long_double *v, size_t n, long_double val);
void vassignfillptr_long_double(vector_long_double *v, size_t n, long_double *valaddr);

/**< vector_long_double: modifiers - push/pop */
void vpushb_long_double(vector_long_double *v, long_double val);
void vpushbptr_long_double(vector_long_double *v, long_double *valaddr);
void vpopb_long_double(vector_long_double *v);

/**< vector_long_double: modifiers - insertion */
iterator vinsert_long_double(vector_long_double *v, iterator pos, long_double val);
iterator vinsertptr_long_double(vector_long_double *v, iterator pos, long_double *val);
iterator vinsertfill_long_double(vector_long_double *v, iterator pos, size_t n, long_double val);
iterator vinsertfillptr_long_double(vector_long_double *v, iterator pos, size_t n, long_double *valaddr);
iterator vinsertrnge_long_double(vector_long_double *v, iterator pos, iterator first,
                         iterator last);
iterator vinsertmove_long_double(vector_long_double *v, iterator pos, long_double *valaddr);

/**< vector_long_double: modifiers - erasure */
iterator verase_long_double(vector_long_double *v, iterator pos);
iterator verasernge_long_double(vector_long_double *v, iterator pos, iterator last);

/**< vector_long_double: modifiers - container swappage */
void vswap_long_double(vector_long_double **v, vector_long_double **other);

/**< vector_long_double: modifiers - clear container */
void vclear_long_double(vector_long_double *v);

/**< vector_long_double: custom modifiers - insertion/erasure/replacement by index */
void vinsertat_long_double(vector_long_double *v, size_t index, long_double val);
void vinsertatptr_long_double(vector_long_double *v, size_t index, long_double *valaddr);
void veraseat_long_double(vector_long_double *v, size_t index);
void vreplaceat_long_double(vector_long_double *v, size_t index, long_double val);
void vreplaceatptr_long_double(vector_long_double *v, size_t index, long_double *valaddr);

/**< vector_long_double: custom modifiers - element swappage */
void vswapelem_long_double(vector_long_double *v, size_t n1, size_t n2);

/**< vector_long_double: custom modifiers - element removal by search */
void vremove_long_double(vector_long_double *v, long_double val);
void vremoveif_long_double(vector_long_double *v, bool (*unary_predicate)(const void *));

/**< vector_long_double: custom modifiers - merge/reverse */
vector_long_double *vmerge_long_double(vector_long_double *v, vector_long_double * other);
void vreverse_long_double(vector_long_double *v);

/**< vector_long_double: custom utility functions - array to vector/pointer to vector */
vector_long_double *varrtov_long_double(long_double *base, size_t length);
vector_long_double *vptrtov_long_double(long_double *base, size_t length, size_t capacity);

/**< vector_long_double: custom utility functions - search/sort by default comparator */
int vsearch_long_double(vector_long_double *v, long_double val);
void vsort_long_double(vector_long_double *v);

/**< vector_long_double: custom print functions - output to FILE stream */
void vputs_long_double(vector_long_double *v);
void vputsf_long_double(vector_long_double *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs_long_double(vector_long_double *v, FILE *dest);
void vfputsf_long_double(vector_long_double *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);

/**< vector_long_double: required function prototypes for (struct typetable) */
void *tmpl_vector_copy_long_double(void *arg, const void *other);
void tmpl_vector_dtor_long_double(void *arg);
void tmpl_vector_swap_long_double(void *s1, void *s2);
int tmpl_vector_compare_long_double(const void *c1, const void *c2);
void tmpl_vector_print_long_double(const void *arg, FILE *dest);

/**< vector_long_double: change default typetable */
void vsetttbl_long_double(vector_long_double *v, struct typetable *ttbl);

size_t vgetwidth_long_double(vector_long_double *v);
copy_fn vgetcopy_long_double(vector_long_double *v);
dtor_fn vgetdtor_long_double(vector_long_double *v);
swap_fn vgetswap_long_double(vector_long_double *v);
compare_fn vgetcompare_long_double(vector_long_double *v);
print_fn vgetprint_long_double(vector_long_double *v);
struct typetable *vgetttbl_long_double(vector_long_double *v);

/**< ptrs to vtables */
struct typetable *_vector_long_double_;
struct iterator_table *_vector_iterator_long_double_;

#endif /* VECTOR_LONG_DOUBLE_H */
