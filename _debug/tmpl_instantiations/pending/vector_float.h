/**
 *  @file       vector_float.h
 *  @brief      Header for preprocessed template instantiation of vector(float)
 *
 *  @author     Gemuele Aludino
 *  @date       21 Sep 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#ifndef VECTOR_FLOAT_H
#define VECTOR_FLOAT_H
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
 *  @typedef    vector_float
 *  @brief      Alias for (struct vector_float)
 *
 *  All instances of (struct vector_float) will be addressed as (vector_float).
 */
typedef struct vector_float vector_float;

/**
 *  @typedef    vector_ptr_float
 *  @brief      Alias for (struct vector_float *) or (vector_float *)
 *
 *  All instances of (struct vector_float *) will be addressed as (vector_float *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct vector_float *vector_ptr_float;

/**
 *  @typedef    vector_dptr_float
 *  @brief      Alias for (struct vector_float **) or (vector_float **)
 *
 *  All instances of (struct vector_float **) will be addressed as (vector_float **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct vector_float **vector_dptr_float;

/**< vector_float: constructors */
vector_float *vnew_float(void);
vector_float *vnewr_float(size_t n);
vector_float *vnewfill_float(size_t n, float val);
vector_float *vnewfillptr_float(size_t n, float *valaddr);
vector_float *vnewrnge_float(iterator first, iterator last);
vector_float *vnewcopy_float(vector_float *v);
vector_float *vnewmove_float(vector_float **v);

/**< vector_float: destructor */
void vdelete_float(vector_float **v);

/**< vector_float: iterator functions */
iterator vbegin_float(vector_float *v);
iterator vend_float(vector_float *v);

/**< vector_float: length functions */
size_t vsize_float(vector_float *v);
size_t vmaxsize_float(vector_float *v);

/**< vector_float: resize functions */
void vresize_float(vector_float *v, size_t n);
void vresizefill_float(vector_float *v, size_t n, float val);
void vresizefillptr_float(vector_float *v, size_t n, float *valaddr);

/**< vector_float: capacity based functions */
size_t vcapacity_float(vector_float *v);
bool vempty_float(vector_float *v);

/**< vector_float: reserve/shrinking functions */
void vreserve_float(vector_float *v, size_t n);
void vshrinktofit_float(vector_float *v);

/**< vector_float: element access functions */
float *vat_float(vector_float *v, size_t n);
float *vfront_float(vector_float *v);
float *vback_float(vector_float *v);
float **vdata_float(vector_float *v);

/**< vector_float: element access functions with const qualifier */
const float *vatconst_float(vector_float *v, size_t n);
const float *vfrontconst_float(vector_float *v);
const float *vbackconst_float(vector_float *v);
const float **vdataconst_float(vector_float *v);

/**< vector_float: modifiers - assignement */
void vassignrnge_float(vector_float *v, iterator first, iterator last);
void vassignfill_float(vector_float *v, size_t n, float val);
void vassignfillptr_float(vector_float *v, size_t n, float *valaddr);

/**< vector_float: modifiers - push/pop */
void vpushb_float(vector_float *v, float val);
void vpushbptr_float(vector_float *v, float *valaddr);
void vpopb_float(vector_float *v);

/**< vector_float: modifiers - insertion */
iterator vinsert_float(vector_float *v, iterator pos, float val);
iterator vinsertptr_float(vector_float *v, iterator pos, float *val);
iterator vinsertfill_float(vector_float *v, iterator pos, size_t n, float val);
iterator vinsertfillptr_float(vector_float *v, iterator pos, size_t n, float *valaddr);
iterator vinsertrnge_float(vector_float *v, iterator pos, iterator first,
                         iterator last);
iterator vinsertmove_float(vector_float *v, iterator pos, float *valaddr);

/**< vector_float: modifiers - erasure */
iterator verase_float(vector_float *v, iterator pos);
iterator verasernge_float(vector_float *v, iterator pos, iterator last);

/**< vector_float: modifiers - container swappage */
void vswap_float(vector_float **v, vector_float **other);

/**< vector_float: modifiers - clear container */
void vclear_float(vector_float *v);

/**< vector_float: custom modifiers - insertion/erasure/replacement by index */
void vinsertat_float(vector_float *v, size_t index, float val);
void vinsertatptr_float(vector_float *v, size_t index, float *valaddr);
void veraseat_float(vector_float *v, size_t index);
void vreplaceat_float(vector_float *v, size_t index, float val);
void vreplaceatptr_float(vector_float *v, size_t index, float *valaddr);

/**< vector_float: custom modifiers - element swappage */
void vswapelem_float(vector_float *v, size_t n1, size_t n2);

/**< vector_float: custom modifiers - element removal by search */
void vremove_float(vector_float *v, float val);
void vremoveif_float(vector_float *v, bool (*unary_predicate)(const void *));

/**< vector_float: custom modifiers - merge/reverse */
vector_float *vmerge_float(vector_float *v, vector_float * other);
void vreverse_float(vector_float *v);

/**< vector_float: custom utility functions - array to vector/pointer to vector */
vector_float *varrtov_float(float *base, size_t length);
vector_float *vptrtov_float(float *base, size_t length, size_t capacity);

/**< vector_float: custom utility functions - search/sort by default comparator */
int vsearch_float(vector_float *v, float val);
void vsort_float(vector_float *v);

/**< vector_float: custom print functions - output to FILE stream */
void vputs_float(vector_float *v);
void vputsf_float(vector_float *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs_float(vector_float *v, FILE *dest);
void vfputsf_float(vector_float *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);

/**< vector_float: required function prototypes for (struct typetable) */
void *tmpl_vector_copy_float(void *arg, const void *other);
void tmpl_vector_dtor_float(void *arg);
void tmpl_vector_swap_float(void *s1, void *s2);
int tmpl_vector_compare_float(const void *c1, const void *c2);
void tmpl_vector_print_float(const void *arg, FILE *dest);

/**< vector_float: change default typetable */
void vsetttbl_float(vector_float *v, struct typetable *ttbl);

size_t vgetwidth_float(vector_float *v);
copy_fn vgetcopy_float(vector_float *v);
dtor_fn vgetdtor_float(vector_float *v);
swap_fn vgetswap_float(vector_float *v);
compare_fn vgetcompare_float(vector_float *v);
print_fn vgetprint_float(vector_float *v);
struct typetable *vgetttbl_float(vector_float *v);

/**< ptrs to vtables */
struct typetable *_vector_float_;
struct iterator_table *_vector_iterator_float_;

#endif /* VECTOR_FLOAT_H */
