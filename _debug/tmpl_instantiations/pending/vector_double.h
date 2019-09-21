/**
 *  @file       vector_double.h
 *  @brief      Header for preprocessed template instantiation of vector(double)
 *
 *  @author     Gemuele Aludino
 *  @date       21 Sep 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#ifndef VECTOR_DOUBLE_H
#define VECTOR_DOUBLE_H
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
 *  @typedef    vector_double
 *  @brief      Alias for (struct vector_double)
 *
 *  All instances of (struct vector_double) will be addressed as (vector_double).
 */
typedef struct vector_double vector_double;

/**
 *  @typedef    vector_ptr_double
 *  @brief      Alias for (struct vector_double *) or (vector_double *)
 *
 *  All instances of (struct vector_double *) will be addressed as (vector_double *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct vector_double *vector_ptr_double;

/**
 *  @typedef    vector_dptr_double
 *  @brief      Alias for (struct vector_double **) or (vector_double **)
 *
 *  All instances of (struct vector_double **) will be addressed as (vector_double **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct vector_double **vector_dptr_double;

/**< vector_double: constructors */
vector_double *vnew_double(void);
vector_double *vnewr_double(size_t n);
vector_double *vnewfill_double(size_t n, double val);
vector_double *vnewfillptr_double(size_t n, double *valaddr);
vector_double *vnewrnge_double(iterator first, iterator last);
vector_double *vnewcopy_double(vector_double *v);
vector_double *vnewmove_double(vector_double **v);

/**< vector_double: destructor */
void vdelete_double(vector_double **v);

/**< vector_double: iterator functions */
iterator vbegin_double(vector_double *v);
iterator vend_double(vector_double *v);

/**< vector_double: length functions */
size_t vsize_double(vector_double *v);
size_t vmaxsize_double(vector_double *v);

/**< vector_double: resize functions */
void vresize_double(vector_double *v, size_t n);
void vresizefill_double(vector_double *v, size_t n, double val);
void vresizefillptr_double(vector_double *v, size_t n, double *valaddr);

/**< vector_double: capacity based functions */
size_t vcapacity_double(vector_double *v);
bool vempty_double(vector_double *v);

/**< vector_double: reserve/shrinking functions */
void vreserve_double(vector_double *v, size_t n);
void vshrinktofit_double(vector_double *v);

/**< vector_double: element access functions */
double *vat_double(vector_double *v, size_t n);
double *vfront_double(vector_double *v);
double *vback_double(vector_double *v);
double **vdata_double(vector_double *v);

/**< vector_double: element access functions with const qualifier */
const double *vatconst_double(vector_double *v, size_t n);
const double *vfrontconst_double(vector_double *v);
const double *vbackconst_double(vector_double *v);
const double **vdataconst_double(vector_double *v);

/**< vector_double: modifiers - assignement */
void vassignrnge_double(vector_double *v, iterator first, iterator last);
void vassignfill_double(vector_double *v, size_t n, double val);
void vassignfillptr_double(vector_double *v, size_t n, double *valaddr);

/**< vector_double: modifiers - push/pop */
void vpushb_double(vector_double *v, double val);
void vpushbptr_double(vector_double *v, double *valaddr);
void vpopb_double(vector_double *v);

/**< vector_double: modifiers - insertion */
iterator vinsert_double(vector_double *v, iterator pos, double val);
iterator vinsertptr_double(vector_double *v, iterator pos, double *val);
iterator vinsertfill_double(vector_double *v, iterator pos, size_t n, double val);
iterator vinsertfillptr_double(vector_double *v, iterator pos, size_t n, double *valaddr);
iterator vinsertrnge_double(vector_double *v, iterator pos, iterator first,
                         iterator last);
iterator vinsertmove_double(vector_double *v, iterator pos, double *valaddr);

/**< vector_double: modifiers - erasure */
iterator verase_double(vector_double *v, iterator pos);
iterator verasernge_double(vector_double *v, iterator pos, iterator last);

/**< vector_double: modifiers - container swappage */
void vswap_double(vector_double **v, vector_double **other);

/**< vector_double: modifiers - clear container */
void vclear_double(vector_double *v);

/**< vector_double: custom modifiers - insertion/erasure/replacement by index */
void vinsertat_double(vector_double *v, size_t index, double val);
void vinsertatptr_double(vector_double *v, size_t index, double *valaddr);
void veraseat_double(vector_double *v, size_t index);
void vreplaceat_double(vector_double *v, size_t index, double val);
void vreplaceatptr_double(vector_double *v, size_t index, double *valaddr);

/**< vector_double: custom modifiers - element swappage */
void vswapelem_double(vector_double *v, size_t n1, size_t n2);

/**< vector_double: custom modifiers - element removal by search */
void vremove_double(vector_double *v, double val);
void vremoveif_double(vector_double *v, bool (*unary_predicate)(const void *));

/**< vector_double: custom modifiers - merge/reverse */
vector_double *vmerge_double(vector_double *v, vector_double * other);
void vreverse_double(vector_double *v);

/**< vector_double: custom utility functions - array to vector/pointer to vector */
vector_double *varrtov_double(double *base, size_t length);
vector_double *vptrtov_double(double *base, size_t length, size_t capacity);

/**< vector_double: custom utility functions - search/sort by default comparator */
int vsearch_double(vector_double *v, double val);
void vsort_double(vector_double *v);

/**< vector_double: custom print functions - output to FILE stream */
void vputs_double(vector_double *v);
void vputsf_double(vector_double *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs_double(vector_double *v, FILE *dest);
void vfputsf_double(vector_double *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);

/**< vector_double: required function prototypes for (struct typetable) */
void *tmpl_vector_copy_double(void *arg, const void *other);
void tmpl_vector_dtor_double(void *arg);
void tmpl_vector_swap_double(void *s1, void *s2);
int tmpl_vector_compare_double(const void *c1, const void *c2);
void tmpl_vector_print_double(const void *arg, FILE *dest);

/**< vector_double: change default typetable */
void vsetttbl_double(vector_double *v, struct typetable *ttbl);

size_t vgetwidth_double(vector_double *v);
copy_fn vgetcopy_double(vector_double *v);
dtor_fn vgetdtor_double(vector_double *v);
swap_fn vgetswap_double(vector_double *v);
compare_fn vgetcompare_double(vector_double *v);
print_fn vgetprint_double(vector_double *v);
struct typetable *vgetttbl_double(vector_double *v);

/**< ptrs to vtables */
struct typetable *_vector_double_;
struct iterator_table *_vector_iterator_double_;

#endif /* VECTOR_DOUBLE_H */
