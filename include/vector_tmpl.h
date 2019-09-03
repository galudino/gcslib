/**
 *  @file       vector_tmpl.h
 *  @brief      Template header file for a type-safe dynamic array ADT
 *
 *  @author     Gemuele Aludino
 *  @date       11 Jul 2019
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
 
#ifdef T

#include <stdbool.h>
#include <stdio.h>
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
 *  Since vector(T) is an opaque data structure, instances of vector cannot be
 *  created on the stack, nor can malloc/calloc be called to allocate memory
 *  for vector directly.
 *
 *  Instead, all functionality related to vector's usage will be done using
 *  the functions declared in this public header file, vector_tmpl.h.
 *
 *  Functions prepended with "v" and/or have a (vector(T) *)
 *  as its first argument are designated for use with (vector(T) *).
 *
 *  ############################################################################
 *
 *  This header file, and its corresponding source file, vector_tmpl.c,
 *  are not usable on their own -- they require the use of the preprocessor
 *  to have code generated for it.
 *
 *  vector(T), unlike its counterpart, vector (from vector.h and vector.c)
 *  is a TYPE-SAFE alternative to vector, which uses a (void *) buffer as its
 *  internal data structure.
 *
 *  vector(T) will perform as would a hand-written vector for a given type T.
 *
 *  For example, to create a vector of TYPENAME, you will need
 *  two sets of preprocessor statements.
 *
 *  The first set will go in a .h (header) file,
 *  and the second set will go in a .c (source) file.
 *
 *  Your client program will include the described .h (header) file
 *  so that it can use the vector data structure and its functions.
 *
 *  You can have multiple sets of preprocessor statements for each type
 *  to be used with vector(T) in a single header/source file pair,
 *  or dedicate a single header/source file pairing for one type.

 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *  // In a header file, i.e. vector_TYPENAME.h
 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 *  #include "utils.h"              // for a .h file with multiple types,
 *                                  // include this only once.
 *  #ifdef  T
 *  #undef  T
 *  #endif
 *  // if T is user-defined (i.e. a struct), include its header file here.
 *  #define T   TYPENAME
 *  #include "vector_tmpl.h"
 *  #undef T
 *
 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *  // Repeat the same 5-6 lines for ANOTHER_TYPENAME, if you would like.
 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *  // In a corresponding source file, i.e. vector_TYPENAME.c
 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 *  #include "vector_TYPENAME.h"    // for a .h file with multiple types,
 *                                  // include this only once.
 *  // ------------------------
 *  #ifdef  T
 *  #undef  T
 *  #endif
 *  #define T   TYPENAME
 *  #include "vector_tmpl.c"
 *  #undef T
 *
 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *  // Repeat the same 5 lines for ANOTHER_TYPENAME, if you would like.
 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *  // In your client program, i.e. main.c
 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 *  // ...other #include directives...
 *  #include "vector_TYPENAME.h"
 *
 *  int main(int argc, const char *argv[]) {
 *      vector_TYPENAME *vec = vnew_TYPENAME();     // heap allocated
 *      // use vec
 *      vdelete_TYPENAME(&vec);                     // freeing memory
 *
 *      return 0;
 *  }
 *
 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *  // Seek the example header/source file pairs:
 *  //      vector_char.h, vector_int.h, vector_str.h
 *  //      vector_char.c, vector_int.c, vector_str.c
 *  // vector(T) of char, int, and string variants
 *  // are part of this source code package, so you
 *  // do not have to create them.
 *  //
 *  // Note: string variants means:
 *           (const char *) is typedefed to cstr.
 *           ------------------------------------
 *           vector_cstr is used for stack-allocated string literals.
 *
 *           (char *)       is typedefed to str.
 *           -----------------------------------
 *           vector_str is used for heap-allocated strings
 *           (created with malloc or strdup), and vector_str
 *           destroys its memory from the destructor function within the
 *           struct typetable pointer, _str_.
 *
 *           (see utils.h for information on how to create
 *           a struct typetable instance to be used with any container
 *           in this API. a struct typetable is required for
 *           any functions that do deep copying, comparisons/sorts, printing,
 *           and memory cleanup/deletion.)
 *
 *           (char *)       is also typedefed to char_ptr.
 *           ---------------------------------------------
 *           vector_char_ptr can be used for non-const (char *) elements,
 *           or arrays of char (char[]), which will decay to a (char *).
 *
 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 *  ############################################################################
 */

/**
 *  @def        VECTOR_TMPL_DEFAULT_CAPACITY
 *  @brief      Default capacity for vector's internal buffer
 */
#define VECTOR_TMPL_DEFAULT_CAPACITY 16

/**
 *  The macro
 *      tmpl(ARG, TYPE)
 *  is really a macro that wraps
 *      CAT(X, Y)
 *  and CAT(X, Y) is a token-pasting utility that produces the following:
 *      X_Y
 *
 *  For example:
 *      if T is defined to be int,
 *      and vector(T) is defined to be tmpl(vector, T)
 *          The result is:
 *              vector_int
 */

#define vector(T) tmpl(vector, T)
#define vector_base(T) tmpl(vector_base, T)
#define vector_ptr(T) tmpl(vector_ptr, T)
#define vector_dptr(T) tmpl(vector_dptr, T)

#define vnew(T) tmpl(vnew, T)
#define vnewr(T) tmpl(vnewr, T)
#define vnewfill(T) tmpl(vnewfill, T)
#define vnewfillptr(T) tmpl(vnewfillptr, T)
#define vnewrnge(T) tmpl(vnewrnge, T)
#define vnewcopy(T) tmpl(vnewcopy, T)
#define vnewmove(T) tmpl(vnewmove, T)

#define vdelete(T) tmpl(vdelete, T)

#define vbegin(T) tmpl(vbegin, T)
#define vend(T) tmpl(vend, T)

#define vsize(T) tmpl(vsize, T)
#define vmaxsize(T) tmpl(vmaxsize, T)

#define vresize(T) tmpl(vresize, T)
#define vresizefill(T) tmpl(vresizefill, T)
#define vresizefillptr(T) tmpl(vresizefillptr, T)

#define vcapacity(T) tmpl(vcapacity, T)
#define vempty(T) tmpl(vempty, T)

#define vreserve(T) tmpl(vreserve, T)
#define vshrinktofit(T) tmpl(vshrinktofit, T)

#define vat(T) tmpl(vat, T)
#define vfront(T) tmpl(vfront, T)
#define vback(T) tmpl(vback, T)
#define vdata(T) tmpl(vdata, T)

#define vatconst(T) tmpl(vatconst, T)
#define vfrontconst(T) tmpl(vfrontconst, T)
#define vbackconst(T) tmpl(vbackconst, T)
#define vdataconst(T) tmpl(vdataconst, T)

#define vassignrnge(T) tmpl(vassignrnge, T)
#define vassignfill(T) tmpl(vassignfill, T)
#define vassignfillptr(T) tmpl(vassignfillptr, T)

#define vpushb(T) tmpl(vpushb, T)
#define vpushbptr(T) tmpl(vpushbptr, T)
#define vpopb(T) tmpl(vpopb, T)

#define vinsert(T) tmpl(vinsert, T)
#define vinsertptr(T) tmpl(vinsertptr, T)
#define vinsertfill(T) tmpl(vinsertfill, T)
#define vinsertfillptr(T) tmpl(vinsertfillptr, T)
#define vinsertrnge(T) tmpl(vinsertrnge, T)
#define vinsertmove(T) tmpl(vinsertmove, T)
#define vinsertmoveptr(T) tmpl(vinsertmoveptr, T)

#define verase(T) tmpl(verase, T)
#define verasernge(T) tmpl(verasernge, T)

#define vswap(T) tmpl(vswap, T)
#define vclear(T) tmpl(vclear, T)

#define vinsertat(T) tmpl(vinsertat, T)
#define vinsertatptr(T) tmpl(vinsertatptr, T)
#define veraseat(T) tmpl(veraseat, T)
#define vreplaceat(T) tmpl(vreplaceat, T)
#define vreplaceatptr(T) tmpl(vreplaceatptr, T)

#define vswapelem(T) tmpl(vswapelem, T)

#define vremove(T) tmpl(vremove, T)
#define vremoveif(T) tmpl(vremoveif, T)

#define vmerge(T) tmpl(vmerge, T)
#define vreverse(T) tmpl(vreverse, T)

#define varrtov(T) tmpl(varrtov, T)

#define vsearch(T) tmpl(vsearch, T)
#define vsort(T) tmpl(vsort, T)

#define vputs(T) tmpl(vputs, T)
#define vputsf(T) tmpl(vputsf, T)
#define vfputs(T) tmpl(vfputs, T)
#define vfputsf(T) tmpl(vfputsf, T)

#define tmpl_vector_copy(T) tmpl(tmpl_vector_copy, T)
#define tmpl_vector_dtor(T) tmpl(tmpl_vector_dtor, T)
#define tmpl_vector_swap(T) tmpl(tmpl_vector_swap, T)
#define tmpl_vector_compare(T) tmpl(tmpl_vector_compare, T)
#define tmpl_vector_print(T) tmpl(tmpl_vector_print, T)

#define vsetttbl(T) tmpl(vsetttbl, T)

#define vgetwidth(T) tmpl(vgetwidth, T)
#define vgetcopy(T) tmpl(vgetcopy, T)
#define vgetdtor(T) tmpl(vgetdtor, T)
#define vgetswap(T) tmpl(vgetswap, T)
#define vgetcompare(T) tmpl(vgetcompare, T)
#define vgetprint(T) tmpl(vgetprint, T)
#define vgetttbl(T) tmpl(vgetttbl, T)

#define vector_type_table_ptr_id(T) type_table_ptr_id(T)
#define vector_typetable_ptr_id(T) table_ptr_id(vector, T)
#define vector_iterator_table_ptr_id(T) table_ptr_id(vector_iterator, T)

/**
 *  @typedef    vector(T)
 *  @brief      Alias for (struct vector(T))
 *
 *  All instances of (struct vector(T)) will be addressed as (vector(T)).
 */
typedef struct vector(T) vector(T);

/**
 *  @typedef    vector_ptr(T)
 *  @brief      Alias for (struct vector(T) *) or (vector(T) *)
 *
 *  All instances of (struct vector(T) *) will be addressed as (vector(T) *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct vector(T) *vector_ptr(T);

/**
 *  @typedef    vector_dptr(T)
 *  @brief      Alias for (struct vector(T) **) or (vector(T) **)
 *
 *  All instances of (struct vector(T) **) will be addressed as (vector(T) **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct vector(T) **vector_dptr(T);

/**< vector(T): constructors */
vector(T) *vnew(T)(void);
vector(T) *vnewr(T)(size_t n);
vector(T) *vnewfill(T)(size_t n, T val);
vector(T) *vnewfillptr(T)(size_t n, T *valaddr);
vector(T) *vnewrnge(T)(iterator first, iterator last);
vector(T) *vnewcopy(T)(vector(T) *v);
vector(T) *vnewmove(T)(vector(T) **v);

/**< vector(T): destructor */
void vdelete(T)(vector(T) **v);

/**< vector(T): iterator functions */
iterator vbegin(T)(vector(T) *v);
iterator vend(T)(vector(T) *v);

/**< vector(T): length functions */
size_t vsize(T)(vector(T) *v);
size_t vmaxsize(T)(vector(T) *v);

/**< vector(T): resize functions */
void vresize(T)(vector(T) *v, size_t n);
void vresizefill(T)(vector(T) *v, size_t n, T val);
void vresizefillptr(T)(vector(T) *v, size_t n, T *valaddr);

/**< vector(T): capacity based functions */
size_t vcapacity(T)(vector(T) *v);
bool vempty(T)(vector(T) *v);

/**< vector(T): reserve/shrinking functions */
void vreserve(T)(vector(T) *v, size_t n);
void vshrinktofit(T)(vector(T) *v);

/**< vector(T): element access functions */
T *vat(T)(vector(T) *v, size_t n);
T *vfront(T)(vector(T) *v);
T *vback(T)(vector(T) *v);
T **vdata(T)(vector(T) *v);

/**< vector(T): element access functions with const qualifier */
const T *vatconst(T)(vector(T) *v, size_t n);
const T *vfrontconst(T)(vector(T) *v);
const T *vbackconst(T)(vector(T) *v);
const T **vdataconst(T)(vector(T) *v);

/**< vector(T): modifiers - assignement */
void vassignrnge(T)(vector(T) *v, iterator first, iterator last);
void vassignfill(T)(vector(T) *v, size_t n, T val);
void vassignfillptr(T)(vector(T) *v, size_t n, T *valaddr);

/**< vector(T): modifiers - push/pop */
void vpushb(T)(vector(T) *v, T val);
void vpushbptr(T)(vector(T) *v, T *valaddr);
void vpopb(T)(vector(T) *v);

/**< vector(T): modifiers - insertion */
iterator *vinsert(T)(vector(T) *v, iterator pos, T val);
iterator *vinsertptr(T)(vector(T) *v, iterator pos, T *val);
iterator *vinsertfill(T)(vector(T) *v, iterator pos, size_t n, T val);
iterator *vinsertfillptr(T)(vector(T) *v, iterator pos, size_t n, T *valaddr);
iterator *vinsertrnge(T)(vector(T) *v, iterator pos, iterator first,
                         iterator last);
iterator *vinsertmove(T)(vector(T) *v, iterator *pos, T *valaddr);

/**< vector(T): modifiers - erasure */
iterator *verase(T)(vector(T) *v, iterator *pos);
iterator *verasernge(T)(vector(T) *v, iterator *pos, iterator *last);

/**< vector(T): modifiers - container swappage */
void vswap(T)(vector(T) * *v, vector(T) * *other);

/**< vector(T): modifiers - clear container */
void vclear(T)(vector(T) *v);

/**< vector(T): custom modifiers - insertion/erasure/replacement by index */
void vinsertat(T)(vector(T) *v, size_t index, T val);
void vinsertatptr(T)(vector(T) *v, size_t index, T *valaddr);
void veraseat(T)(vector(T) *v, size_t index);
void vreplaceat(T)(vector(T) *v, size_t index, T val);
void vreplaceatptr(T)(vector(T) *v, size_t index, T *valaddr);

/**< vector(T): custom modifiers - element swappage */
void vswapelem(T)(vector(T) *v, size_t n1, size_t n2);

/**< vector(T): custom modifiers - element removal by search */
void vremove(T)(vector(T) *v, T val);
void vremoveif(T)(vector(T) *v, T val, bool (*unary_predicate)(const void *));

/**< vector(T): custom modifiers - merge/reverse */
vector(T) *vmerge(T)(vector(T) *v, vector(T) * other);
void vreverse(T)(vector(T) *v);

/**< vector(T): custom utility functions - array to vector */
vector(T) *varrtov(T)(T *base, size_t length);

/**< vector(T): custom utility functions - search/sort by default comparator */
int vsearch(T)(vector(T) *v, T val);
void vsort(T)(vector(T) *v);

/**< vector(T): custom print functions - output to FILE stream */
void vputs(T)(vector(T) *v);
void vputsf(T)(vector(T) *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs(T)(vector(T) *v, FILE *dest);
void vfputsf(T)(vector(T) *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);

/**< vector(T): required function prototypes for (struct typetable) */
void *tmpl_vector_copy(T)(void *arg, const void *other);
void tmpl_vector_dtor(T)(void *arg);
void tmpl_vector_swap(T)(void *s1, void *s2);
int tmpl_vector_compare(T)(const void *c1, const void *c2);
void tmpl_vector_print(T)(const void *arg, FILE *dest);

/**< vector(T): change default typetable */
void vsetttbl(T)(vector(T) *v, struct typetable *ttbl);

size_t vgetwidth(T)(vector(T) *v);
copy_fn vgetcopy(T)(vector(T) *v);
dtor_fn vgetdtor(T)(vector(T) *v);
swap_fn vgetswap(T)(vector(T) *v);
compare_fn vgetcompare(T)(vector(T) *v);
print_fn vgetprint(T)(vector(T) *v);
struct typetable *vgetttbl(T)(vector(T) *v);

/**< ptrs to vtables */
struct typetable *vector_typetable_ptr_id(T);
struct iterator_table *vector_iterator_table_ptr_id(T);

/**
 * TODO:
 *  vassignrnge
 *
 *  vassignfill
 *  vassignfillptr
 *
 *  vassignfill_dcp
 *  vassignfillptr_dcp
 *
 *  vinsert
 *  vinsert_dcp
 *
 *  vinsertptr
 *  vinsertptr_dcp
 *
 *  vinsertfill
 *  vinsertfillptr
 *
 *  vinsertrnge
 *  vinsertrnge_dcp
 *
 *  vinsertmove
 *
 *  verase
 *  verasernge
 *
 *  vmerge
 */

#endif /* T */
