/**
 *  @file       vector_char.h
 *  @brief      Header for preprocessed template instantiation of vector(char)
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

#ifndef VECTOR_CHAR_H
#define VECTOR_CHAR_H

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
 *  @typedef    vector_char
 *  @brief      Alias for (struct vector_char)
 *
 *  All instances of (struct vector_char) will be addressed as (vector_char).
 */
typedef struct vector_char vector_char;

/**
 *  @typedef    vector_ptr_char
 *  @brief      Alias for (struct vector_char *) or (vector_char *)
 *
 *  All instances of (struct vector_char *) will be addressed as (vector_char *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct vector_char *vector_ptr_char;

/**
 *  @typedef    vector_dptr_char
 *  @brief      Alias for (struct vector_char **) or (vector_char **)
 *
 *  All instances of (struct vector_char **) will be addressed as (vector_char **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct vector_char **vector_dptr_char;

/**< vector_char: constructors */
vector_char *vnew_char(void);
vector_char *vnewr_char(size_t n);
vector_char *vnewfill_char(size_t n, char val);
vector_char *vnewfillptr_char(size_t n, char *valaddr);
vector_char *vnewrnge_char(iterator first, iterator last);
vector_char *vnewcopy_char(vector_char *v);
vector_char *vnewmove_char(vector_char **v);

/**< vector_char: destructor */
void vdelete_char(vector_char **v);

/**< vector_char: iterator functions */
iterator vbegin_char(vector_char *v);
iterator vend_char(vector_char *v);

/**< vector_char: length functions */
size_t vsize_char(vector_char *v);
size_t vmaxsize_char(vector_char *v);

/**< vector_char: resize functions */
void vresize_char(vector_char *v, size_t n);
void vresizefill_char(vector_char *v, size_t n, char val);
void vresizefillptr_char(vector_char *v, size_t n, char *valaddr);

/**< vector_char: capacity based functions */
size_t vcapacity_char(vector_char *v);
bool vempty_char(vector_char *v);

/**< vector_char: reserve/shrinking functions */
void vreserve_char(vector_char *v, size_t n);
void vshrinktofit_char(vector_char *v);

/**< vector_char: element access functions */
char *vat_char(vector_char *v, size_t n);
char *vfront_char(vector_char *v);
char *vback_char(vector_char *v);
char **vdata_char(vector_char *v);

/**< vector_char: element access functions with const qualifier */
const char *vatconst_char(vector_char *v, size_t n);
const char *vfrontconst_char(vector_char *v);
const char *vbackconst_char(vector_char *v);
const char **vdataconst_char(vector_char *v);

/**< vector_char: modifiers - assignement */
void vassignrnge_char(vector_char *v, iterator first, iterator last);
void vassignfill_char(vector_char *v, size_t n, char val);
void vassignfillptr_char(vector_char *v, size_t n, char *valaddr);

/**< vector_char: modifiers - push/pop */
void vpushb_char(vector_char *v, char val);
void vpushbptr_char(vector_char *v, char *valaddr);
void vpopb_char(vector_char *v);

/**< vector_char: modifiers - insertion */
iterator vinsert_char(vector_char *v, iterator pos, char val);
iterator vinsertptr_char(vector_char *v, iterator pos, char *val);
iterator vinsertfill_char(vector_char *v, iterator pos, size_t n, char val);
iterator vinsertfillptr_char(vector_char *v, iterator pos, size_t n, char *valaddr);
iterator vinsertrnge_char(vector_char *v, iterator pos, iterator first,
                         iterator last);
iterator vinsertmove_char(vector_char *v, iterator pos, char *valaddr);

/**< vector_char: modifiers - erasure */
iterator verase_char(vector_char *v, iterator pos);
iterator verasernge_char(vector_char *v, iterator pos, iterator last);

/**< vector_char: modifiers - container swappage */
void vswap_char(vector_char **v, vector_char **other);

/**< vector_char: modifiers - clear container */
void vclear_char(vector_char *v);

/**< vector_char: custom modifiers - insertion/erasure/replacement by index */
void vinsertat_char(vector_char *v, size_t index, char val);
void vinsertatptr_char(vector_char *v, size_t index, char *valaddr);
void veraseat_char(vector_char *v, size_t index);
void vreplaceat_char(vector_char *v, size_t index, char val);
void vreplaceatptr_char(vector_char *v, size_t index, char *valaddr);

/**< vector_char: custom modifiers - element swappage */
void vswapelem_char(vector_char *v, size_t n1, size_t n2);

/**< vector_char: custom modifiers - element removal by search */
void vremove_char(vector_char *v, char val);
void vremoveif_char(vector_char *v, bool (*unary_predicate)(const void *));

/**< vector_char: custom modifiers - merge/reverse */
vector_char *vmerge_char(vector_char *v, vector_char * other);
void vreverse_char(vector_char *v);

/**< vector_char: custom utility functions - array to vector/pointer to vector */
vector_char *varrtov_char(char *base, size_t length);
vector_char *vptrtov_char(char *base, size_t length, size_t capacity);

/**< vector_char: custom utility functions - search/sort by default comparator */
int vsearch_char(vector_char *v, char val);
void vsort_char(vector_char *v);

/**< vector_char: custom print functions - output to FILE stream */
void vputs_char(vector_char *v);
void vputsf_char(vector_char *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs_char(vector_char *v, FILE *dest);
void vfputsf_char(vector_char *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);

/**< vector_char: required function prototypes for (struct typetable) */
void *tmpl_vector_copy_char(void *arg, const void *other);
void tmpl_vector_dtor_char(void *arg);
void tmpl_vector_swap_char(void *s1, void *s2);
int tmpl_vector_compare_char(const void *c1, const void *c2);
void tmpl_vector_print_char(const void *arg, FILE *dest);

/**< vector_char: change default typetable */
void vsetttbl_char(vector_char *v, struct typetable *ttbl);

size_t vgetwidth_char(vector_char *v);
copy_fn vgetcopy_char(vector_char *v);
dtor_fn vgetdtor_char(vector_char *v);
swap_fn vgetswap_char(vector_char *v);
compare_fn vgetcompare_char(vector_char *v);
print_fn vgetprint_char(vector_char *v);
struct typetable *vgetttbl_char(vector_char *v);

/**< ptrs to vtables */
struct typetable *vector_typetable_ptr_id_char;
struct iterator_table *vector_iterator_table_ptr_id_char;

#endif /* VECTOR_CHAR_H */
