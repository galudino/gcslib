/**
 *  @file       vector_cstring.h
 *  @brief      Header for preprocessed template instantiation of vector(cstr)
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

#ifndef VECTOR_CSTRING_H
#define VECTOR_CSTRING_H

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
 *  @typedef    vector_cstr
 *  @brief      Alias for (struct vector_cstr)
 *
 *  All instances of (struct vector_cstr) will be addressed as (vector_cstr).
 */
typedef struct vector_cstr vector_cstr;

/**
 *  @typedef    vector_ptr_cstr
 *  @brief      Alias for (struct vector_cstr *) or (vector_cstr *)
 *
 *  All instances of (struct vector_cstr *) will be addressed as (vector_cstr *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct vector_cstr *vector_ptr_cstr;

/**
 *  @typedef    vector_dptr_cstr
 *  @brief      Alias for (struct vector_cstr **) or (vector_cstr **)
 *
 *  All instances of (struct vector_cstr **) will be addressed as (vector_cstr **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct vector_cstr **vector_dptr_cstr;

/**< vector_cstr: constructors */
vector_cstr *vnew_cstr(void);
vector_cstr *vnewr_cstr(size_t n);
vector_cstr *vnewfill_cstr(size_t n, cstr val);
vector_cstr *vnewfillptr_cstr(size_t n, cstr *valaddr);
vector_cstr *vnewrnge_cstr(iterator first, iterator last);
vector_cstr *vnewcopy_cstr(vector_cstr *v);
vector_cstr *vnewmove_cstr(vector_cstr **v);

/**< vector_cstr: destructor */
void vdelete_cstr(vector_cstr **v);

/**< vector_cstr: iterator functions */
iterator vbegin_cstr(vector_cstr *v);
iterator vend_cstr(vector_cstr *v);

/**< vector_cstr: length functions */
size_t vsize_cstr(vector_cstr *v);
size_t vmaxsize_cstr(vector_cstr *v);

/**< vector_cstr: resize functions */
void vresize_cstr(vector_cstr *v, size_t n);
void vresizefill_cstr(vector_cstr *v, size_t n, cstr val);
void vresizefillptr_cstr(vector_cstr *v, size_t n, cstr *valaddr);

/**< vector_cstr: capacity based functions */
size_t vcapacity_cstr(vector_cstr *v);
bool vempty_cstr(vector_cstr *v);

/**< vector_cstr: reserve/shrinking functions */
void vreserve_cstr(vector_cstr *v, size_t n);
void vshrinktofit_cstr(vector_cstr *v);

/**< vector_cstr: element access functions */
cstr *vat_cstr(vector_cstr *v, size_t n);
cstr *vfront_cstr(vector_cstr *v);
cstr *vback_cstr(vector_cstr *v);
cstr **vdata_cstr(vector_cstr *v);

/**< vector_cstr: element access functions with const qualifier */
const cstr *vatconst_cstr(vector_cstr *v, size_t n);
const cstr *vfrontconst_cstr(vector_cstr *v);
const cstr *vbackconst_cstr(vector_cstr *v);
const cstr **vdataconst_cstr(vector_cstr *v);

/**< vector_cstr: modifiers - assignement */
void vassignrnge_cstr(vector_cstr *v, iterator first, iterator last);
void vassignfill_cstr(vector_cstr *v, size_t n, cstr val);
void vassignfillptr_cstr(vector_cstr *v, size_t n, cstr *valaddr);

/**< vector_cstr: modifiers - push/pop */
void vpushb_cstr(vector_cstr *v, cstr val);
void vpushbptr_cstr(vector_cstr *v, cstr *valaddr);
void vpopb_cstr(vector_cstr *v);

/**< vector_cstr: modifiers - insertion */
iterator vinsert_cstr(vector_cstr *v, iterator pos, cstr val);
iterator vinsertptr_cstr(vector_cstr *v, iterator pos, cstr *val);
iterator vinsertfill_cstr(vector_cstr *v, iterator pos, size_t n, cstr val);
iterator vinsertfillptr_cstr(vector_cstr *v, iterator pos, size_t n, cstr *valaddr);
iterator vinsertrnge_cstr(vector_cstr *v, iterator pos, iterator first,
                         iterator last);
iterator vinsertmove_cstr(vector_cstr *v, iterator pos, cstr *valaddr);

/**< vector_cstr: modifiers - erasure */
iterator verase_cstr(vector_cstr *v, iterator pos);
iterator verasernge_cstr(vector_cstr *v, iterator pos, iterator last);

/**< vector_cstr: modifiers - container swappage */
void vswap_cstr(vector_cstr **v, vector_cstr **other);

/**< vector_cstr: modifiers - clear container */
void vclear_cstr(vector_cstr *v);

/**< vector_cstr: custom modifiers - insertion/erasure/replacement by index */
void vinsertat_cstr(vector_cstr *v, size_t index, cstr val);
void vinsertatptr_cstr(vector_cstr *v, size_t index, cstr *valaddr);
void veraseat_cstr(vector_cstr *v, size_t index);
void vreplaceat_cstr(vector_cstr *v, size_t index, cstr val);
void vreplaceatptr_cstr(vector_cstr *v, size_t index, cstr *valaddr);

/**< vector_cstr: custom modifiers - element swappage */
void vswapelem_cstr(vector_cstr *v, size_t n1, size_t n2);

/**< vector_cstr: custom modifiers - element removal by search */
void vremove_cstr(vector_cstr *v, cstr val);
void vremoveif_cstr(vector_cstr *v, bool (*unary_predicate)(const void *));

/**< vector_cstr: custom modifiers - merge/reverse */
vector_cstr *vmerge_cstr(vector_cstr *v, vector_cstr * other);
void vreverse_cstr(vector_cstr *v);

/**< vector_cstr: custom utility functions - array to vector/pointer to vector */
vector_cstr *varrtov_cstr(cstr *base, size_t length);
vector_cstr *vptrtov_cstr(cstr *base, size_t length, size_t capacity);

/**< vector_cstr: custom utility functions - search/sort by default comparator */
int vsearch_cstr(vector_cstr *v, cstr val);
void vsort_cstr(vector_cstr *v);

/**< vector_cstr: custom print functions - output to FILE stream */
void vputs_cstr(vector_cstr *v);
void vputsf_cstr(vector_cstr *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs_cstr(vector_cstr *v, FILE *dest);
void vfputsf_cstr(vector_cstr *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);

/**< vector_cstr: required function prototypes for (struct typetable) */
void *tmpl_vector_copy_cstr(void *arg, const void *other);
void tmpl_vector_dtor_cstr(void *arg);
void tmpl_vector_swap_cstr(void *s1, void *s2);
int tmpl_vector_compare_cstr(const void *c1, const void *c2);
void tmpl_vector_print_cstr(const void *arg, FILE *dest);

/**< vector_cstr: change default typetable */
void vsetttbl_cstr(vector_cstr *v, struct typetable *ttbl);

size_t vgetwidth_cstr(vector_cstr *v);
copy_fn vgetcopy_cstr(vector_cstr *v);
dtor_fn vgetdtor_cstr(vector_cstr *v);
swap_fn vgetswap_cstr(vector_cstr *v);
compare_fn vgetcompare_cstr(vector_cstr *v);
print_fn vgetprint_cstr(vector_cstr *v);
struct typetable *vgetttbl_cstr(vector_cstr *v);

/**< ptrs to vtables */
struct typetable *vector_typetable_ptr_id_cstr;
struct iterator_table *vector_iterator_table_ptr_id_cstr;

#endif /* VECTOR_CSTRING_H */
