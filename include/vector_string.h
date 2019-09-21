/**
 *  @file       vector_string.h
 *  @brief      Header for preprocessed template instantiation of vector(str)
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

#ifndef VECTOR_STRING_H
#define VECTOR_STRING_H

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
 *  @typedef    vector_str
 *  @brief      Alias for (struct vector_str)
 *
 *  All instances of (struct vector_str) will be addressed as (vector_str).
 */
typedef struct vector_str vector_str;

/**
 *  @typedef    vector_ptr_str
 *  @brief      Alias for (struct vector_str *) or (vector_str *)
 *
 *  All instances of (struct vector_str *) will be addressed as (vector_str *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct vector_str *vector_ptr_str;

/**
 *  @typedef    vector_dptr_str
 *  @brief      Alias for (struct vector_str **) or (vector_str **)
 *
 *  All instances of (struct vector_str **) will be addressed as (vector_str **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct vector_str **vector_dptr_str;

/**< vector_str: constructors */
vector_str *vnew_str(void);
vector_str *vnewr_str(size_t n);
vector_str *vnewfill_str(size_t n, str val);
vector_str *vnewfillptr_str(size_t n, str *valaddr);
vector_str *vnewrnge_str(iterator first, iterator last);
vector_str *vnewcopy_str(vector_str *v);
vector_str *vnewmove_str(vector_str **v);

/**< vector_str: destructor */
void vdelete_str(vector_str **v);

/**< vector_str: iterator functions */
iterator vbegin_str(vector_str *v);
iterator vend_str(vector_str *v);

/**< vector_str: length functions */
size_t vsize_str(vector_str *v);
size_t vmaxsize_str(vector_str *v);

/**< vector_str: resize functions */
void vresize_str(vector_str *v, size_t n);
void vresizefill_str(vector_str *v, size_t n, str val);
void vresizefillptr_str(vector_str *v, size_t n, str *valaddr);

/**< vector_str: capacity based functions */
size_t vcapacity_str(vector_str *v);
bool vempty_str(vector_str *v);

/**< vector_str: reserve/shrinking functions */
void vreserve_str(vector_str *v, size_t n);
void vshrinktofit_str(vector_str *v);

/**< vector_str: element access functions */
str *vat_str(vector_str *v, size_t n);
str *vfront_str(vector_str *v);
str *vback_str(vector_str *v);
str **vdata_str(vector_str *v);

/**< vector_str: element access functions with const qualifier */
const str *vatconst_str(vector_str *v, size_t n);
const str *vfrontconst_str(vector_str *v);
const str *vbackconst_str(vector_str *v);
const str **vdataconst_str(vector_str *v);

/**< vector_str: modifiers - assignement */
void vassignrnge_str(vector_str *v, iterator first, iterator last);
void vassignfill_str(vector_str *v, size_t n, str val);
void vassignfillptr_str(vector_str *v, size_t n, str *valaddr);

/**< vector_str: modifiers - push/pop */
void vpushb_str(vector_str *v, str val);
void vpushbptr_str(vector_str *v, str *valaddr);
void vpopb_str(vector_str *v);

/**< vector_str: modifiers - insertion */
iterator vinsert_str(vector_str *v, iterator pos, str val);
iterator vinsertptr_str(vector_str *v, iterator pos, str *val);
iterator vinsertfill_str(vector_str *v, iterator pos, size_t n, str val);
iterator vinsertfillptr_str(vector_str *v, iterator pos, size_t n, str *valaddr);
iterator vinsertrnge_str(vector_str *v, iterator pos, iterator first,
                         iterator last);
iterator vinsertmove_str(vector_str *v, iterator pos, str *valaddr);

/**< vector_str: modifiers - erasure */
iterator verase_str(vector_str *v, iterator pos);
iterator verasernge_str(vector_str *v, iterator pos, iterator last);

/**< vector_str: modifiers - container swappage */
void vswap_str(vector_str **v, vector_str **other);

/**< vector_str: modifiers - clear container */
void vclear_str(vector_str *v);

/**< vector_str: custom modifiers - insertion/erasure/replacement by index */
void vinsertat_str(vector_str *v, size_t index, str val);
void vinsertatptr_str(vector_str *v, size_t index, str *valaddr);
void veraseat_str(vector_str *v, size_t index);
void vreplaceat_str(vector_str *v, size_t index, str val);
void vreplaceatptr_str(vector_str *v, size_t index, str *valaddr);

/**< vector_str: custom modifiers - element swappage */
void vswapelem_str(vector_str *v, size_t n1, size_t n2);

/**< vector_str: custom modifiers - element removal by search */
void vremove_str(vector_str *v, str val);
void vremoveif_str(vector_str *v, bool (*unary_predicate)(const void *));

/**< vector_str: custom modifiers - merge/reverse */
vector_str *vmerge_str(vector_str *v, vector_str * other);
void vreverse_str(vector_str *v);

/**< vector_str: custom utility functions - array to vector/pointer to vector */
vector_str *varrtov_str(str *base, size_t length);
vector_str *vptrtov_str(str *base, size_t length, size_t capacity);

/**< vector_str: custom utility functions - search/sort by default comparator */
int vsearch_str(vector_str *v, str val);
void vsort_str(vector_str *v);

/**< vector_str: custom print functions - output to FILE stream */
void vputs_str(vector_str *v);
void vputsf_str(vector_str *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs_str(vector_str *v, FILE *dest);
void vfputsf_str(vector_str *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);

/**< vector_str: required function prototypes for (struct typetable) */
void *tmpl_vector_copy_str(void *arg, const void *other);
void tmpl_vector_dtor_str(void *arg);
void tmpl_vector_swap_str(void *s1, void *s2);
int tmpl_vector_compare_str(const void *c1, const void *c2);
void tmpl_vector_print_str(const void *arg, FILE *dest);

/**< vector_str: change default typetable */
void vsetttbl_str(vector_str *v, struct typetable *ttbl);

size_t vgetwidth_str(vector_str *v);
copy_fn vgetcopy_str(vector_str *v);
dtor_fn vgetdtor_str(vector_str *v);
swap_fn vgetswap_str(vector_str *v);
compare_fn vgetcompare_str(vector_str *v);
print_fn vgetprint_str(vector_str *v);
struct typetable *vgetttbl_str(vector_str *v);

/**< ptrs to vtables */
struct typetable *vector_typetable_ptr_id_str;
struct iterator_table *vector_iterator_table_ptr_id_str;

#endif /* VECTOR_STRING_H */
