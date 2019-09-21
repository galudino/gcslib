/**
 *  @file       vector_char_ptr.h
 *  @brief      Header for preprocessed template instantiation of vector(char_ptr)
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
 *  @typedef    vector_char_ptr
 *  @brief      Alias for (struct vector_char_ptr)
 *
 *  All instances of (struct vector_char_ptr) will be addressed as (vector_char_ptr).
 */
typedef struct vector_char_ptr vector_char_ptr;

/**
 *  @typedef    vector_ptr_char_ptr
 *  @brief      Alias for (struct vector_char_ptr *) or (vector_char_ptr *)
 *
 *  All instances of (struct vector_char_ptr *) will be addressed as (vector_char_ptr *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct vector_char_ptr *vector_ptr_char_ptr;

/**
 *  @typedef    vector_dptr_char_ptr
 *  @brief      Alias for (struct vector_char_ptr **) or (vector_char_ptr **)
 *
 *  All instances of (struct vector_char_ptr **) will be addressed as (vector_char_ptr **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct vector_char_ptr **vector_dptr_char_ptr;

/**< vector_char_ptr: constructors */
vector_char_ptr *vnew_char_ptr(void);
vector_char_ptr *vnewr_char_ptr(size_t n);
vector_char_ptr *vnewfill_char_ptr(size_t n, char_ptr val);
vector_char_ptr *vnewfillptr_char_ptr(size_t n, char_ptr *valaddr);
vector_char_ptr *vnewrnge_char_ptr(iterator first, iterator last);
vector_char_ptr *vnewcopy_char_ptr(vector_char_ptr *v);
vector_char_ptr *vnewmove_char_ptr(vector_char_ptr **v);

/**< vector_char_ptr: destructor */
void vdelete_char_ptr(vector_char_ptr **v);

/**< vector_char_ptr: iterator functions */
iterator vbegin_char_ptr(vector_char_ptr *v);
iterator vend_char_ptr(vector_char_ptr *v);

/**< vector_char_ptr: length functions */
size_t vsize_char_ptr(vector_char_ptr *v);
size_t vmaxsize_char_ptr(vector_char_ptr *v);

/**< vector_char_ptr: resize functions */
void vresize_char_ptr(vector_char_ptr *v, size_t n);
void vresizefill_char_ptr(vector_char_ptr *v, size_t n, char_ptr val);
void vresizefillptr_char_ptr(vector_char_ptr *v, size_t n, char_ptr *valaddr);

/**< vector_char_ptr: capacity based functions */
size_t vcapacity_char_ptr(vector_char_ptr *v);
bool vempty_char_ptr(vector_char_ptr *v);

/**< vector_char_ptr: reserve/shrinking functions */
void vreserve_char_ptr(vector_char_ptr *v, size_t n);
void vshrinktofit_char_ptr(vector_char_ptr *v);

/**< vector_char_ptr: element access functions */
char_ptr *vat_char_ptr(vector_char_ptr *v, size_t n);
char_ptr *vfront_char_ptr(vector_char_ptr *v);
char_ptr *vback_char_ptr(vector_char_ptr *v);
char_ptr **vdata_char_ptr(vector_char_ptr *v);

/**< vector_char_ptr: element access functions with const qualifier */
const char_ptr *vatconst_char_ptr(vector_char_ptr *v, size_t n);
const char_ptr *vfrontconst_char_ptr(vector_char_ptr *v);
const char_ptr *vbackconst_char_ptr(vector_char_ptr *v);
const char_ptr **vdataconst_char_ptr(vector_char_ptr *v);

/**< vector_char_ptr: modifiers - assignement */
void vassignrnge_char_ptr(vector_char_ptr *v, iterator first, iterator last);
void vassignfill_char_ptr(vector_char_ptr *v, size_t n, char_ptr val);
void vassignfillptr_char_ptr(vector_char_ptr *v, size_t n, char_ptr *valaddr);

/**< vector_char_ptr: modifiers - push/pop */
void vpushb_char_ptr(vector_char_ptr *v, char_ptr val);
void vpushbptr_char_ptr(vector_char_ptr *v, char_ptr *valaddr);
void vpopb_char_ptr(vector_char_ptr *v);

/**< vector_char_ptr: modifiers - insertion */
iterator vinsert_char_ptr(vector_char_ptr *v, iterator pos, char_ptr val);
iterator vinsertptr_char_ptr(vector_char_ptr *v, iterator pos, char_ptr *val);
iterator vinsertfill_char_ptr(vector_char_ptr *v, iterator pos, size_t n, char_ptr val);
iterator vinsertfillptr_char_ptr(vector_char_ptr *v, iterator pos, size_t n, char_ptr *valaddr);
iterator vinsertrnge_char_ptr(vector_char_ptr *v, iterator pos, iterator first,
                         iterator last);
iterator vinsertmove_char_ptr(vector_char_ptr *v, iterator pos, char_ptr *valaddr);

/**< vector_char_ptr: modifiers - erasure */
iterator verase_char_ptr(vector_char_ptr *v, iterator pos);
iterator verasernge_char_ptr(vector_char_ptr *v, iterator pos, iterator last);

/**< vector_char_ptr: modifiers - container swappage */
void vswap_char_ptr(vector_char_ptr **v, vector_char_ptr **other);

/**< vector_char_ptr: modifiers - clear container */
void vclear_char_ptr(vector_char_ptr *v);

/**< vector_char_ptr: custom modifiers - insertion/erasure/replacement by index */
void vinsertat_char_ptr(vector_char_ptr *v, size_t index, char_ptr val);
void vinsertatptr_char_ptr(vector_char_ptr *v, size_t index, char_ptr *valaddr);
void veraseat_char_ptr(vector_char_ptr *v, size_t index);
void vreplaceat_char_ptr(vector_char_ptr *v, size_t index, char_ptr val);
void vreplaceatptr_char_ptr(vector_char_ptr *v, size_t index, char_ptr *valaddr);

/**< vector_char_ptr: custom modifiers - element swappage */
void vswapelem_char_ptr(vector_char_ptr *v, size_t n1, size_t n2);

/**< vector_char_ptr: custom modifiers - element removal by search */
void vremove_char_ptr(vector_char_ptr *v, char_ptr val);
void vremoveif_char_ptr(vector_char_ptr *v, bool (*unary_predicate)(const void *));

/**< vector_char_ptr: custom modifiers - merge/reverse */
vector_char_ptr *vmerge_char_ptr(vector_char_ptr *v, vector_char_ptr * other);
void vreverse_char_ptr(vector_char_ptr *v);

/**< vector_char_ptr: custom utility functions - array to vector/pointer to vector */
vector_char_ptr *varrtov_char_ptr(char_ptr *base, size_t length);
vector_char_ptr *vptrtov_char_ptr(char_ptr *base, size_t length, size_t capacity);

/**< vector_char_ptr: custom utility functions - search/sort by default comparator */
int vsearch_char_ptr(vector_char_ptr *v, char_ptr val);
void vsort_char_ptr(vector_char_ptr *v);

/**< vector_char_ptr: custom print functions - output to FILE stream */
void vputs_char_ptr(vector_char_ptr *v);
void vputsf_char_ptr(vector_char_ptr *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs_char_ptr(vector_char_ptr *v, FILE *dest);
void vfputsf_char_ptr(vector_char_ptr *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);

/**< vector_char_ptr: required function prototypes for (struct typetable) */
void *tmpl_vector_copy_char_ptr(void *arg, const void *other);
void tmpl_vector_dtor_char_ptr(void *arg);
void tmpl_vector_swap_char_ptr(void *s1, void *s2);
int tmpl_vector_compare_char_ptr(const void *c1, const void *c2);
void tmpl_vector_print_char_ptr(const void *arg, FILE *dest);

/**< vector_char_ptr: change default typetable */
void vsetttbl_char_ptr(vector_char_ptr *v, struct typetable *ttbl);

size_t vgetwidth_char_ptr(vector_char_ptr *v);
copy_fn vgetcopy_char_ptr(vector_char_ptr *v);
dtor_fn vgetdtor_char_ptr(vector_char_ptr *v);
swap_fn vgetswap_char_ptr(vector_char_ptr *v);
compare_fn vgetcompare_char_ptr(vector_char_ptr *v);
print_fn vgetprint_char_ptr(vector_char_ptr *v);
struct typetable *vgetttbl_char_ptr(vector_char_ptr *v);

/**< ptrs to vtables */
struct typetable *vector_typetable_ptr_id_char_ptr;
struct iterator_table *vector_iterator_table_ptr_id_char_ptr;

#endif /* VECTOR_CHAR_PTR_H */
