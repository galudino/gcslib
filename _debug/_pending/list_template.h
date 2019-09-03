/**
 *  @file       list_template.h
 *  @brief      Template header file for a type-safe doubly linked-list ADT
 *
 *  @author     Gemuele Aludino
 *  @date       21 Jul 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
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
 *  Since list(T) is an opaque data structure, instances of list cannot be
 *  created on the stack, nor can malloc/calloc be called to allocate memory
 *  for list directly.
 *
 *  Instead, all functionality related to list's usage will be done using
 *  the functions declared in this public header file, list_template.h.
 *
 *  Functions prepended with "l" and/or have a (list(T) *)
 *  as its first argument are designated for use with (list(T) *).
 *
 *  ############################################################################
 *
 *  This header file, and its corresponding source file, list_template.c,
 *  are not usable on their own -- they require the use of the preprocessor
 *  to have code generated for it.
 *
 *  list(T), unlike its counterpart, list (from list.h and list.c)
 *  is a TYPE-SAFE alternative to list, which uses a (void *) buffer as its
 *  internal data structure.
 *
 *  list(T) will perform as would a hand-written list for a given type T.
 *
 *  For example, to create a list of TYPENAME, you will need
 *  two sets of preprocessor statements.
 *
 *  The first set will go in a .h (header) file,
 *  and the second set will go in a .c (source) file.
 *
 *  Your client program will include the described .h (header) file
 *  so that it can use the list data structure and its functions.
 *
 *  You can have multiple sets of preprocessor statements for each type
 *  to be used with list(T) in a single header/source file pair,
 *  or dedicate a single header/source file pairing for one type.

 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *  // In a header file, i.e. list_TYPENAME.h
 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 *  #include "utils.h"              // for a .h file with multiple types,
 *                                  // include this only once.
 *  #ifdef  T
 *  #undef  T
 *  #endif
 *  // if T is user-defined (i.e. a struct), include its header file here.
 *  #define T   TYPENAME
 *  #include "list_template.h"
 *  #undef T
 *
 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *  // Repeat the same 5-6 lines for ANOTHER_TYPENAME, if you would like.
 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *  // In a corresponding source file, i.e. list_TYPENAME.c
 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 *  #include "list_TYPENAME.h"    // for a .h file with multiple types,
 *                                  // include this only once.
 *  // ------------------------
 *  #ifdef  T
 *  #undef  T
 *  #endif
 *  #define T   TYPENAME
 *  #include "list_template.c"
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
 *  #include "list_TYPENAME.h"
 *
 *  int main(int argc, const char *argv[]) {
 *      list_TYPENAME *lst = lnew_TYPENAME();     // heap allocated
 *      // use lst
 *      vdelete_TYPENAME(&vec);                     // freeing memory
 *
 *      return 0;
 *  }
 *
 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *  // Seek the example header/source file pairs:
 *  //      list_char.h, list_int.h, list_str.h
 *  //      list_char.c, list_int.c, list_str.c
 *  // list(T) of char, int, and string variants
 *  // are part of this source code package, so you
 *  // do not have to create them.
 *  //
 *  // Note: string variants means:
 *           (const char *) is typedefed to cstr.
 *           ------------------------------------
 *           list_cstr is used for stack-allocated string literals.
 *
 *           (char *)       is typedefed to str.
 *           -----------------------------------
 *           list_str is used for heap-allocated strings
 *           (created with malloc or strdup), and list_str
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
 *           list_char_ptr can be used for non-const (char *) elements,
 *           or arrays of char (char[]), which will decay to a (char *).
 *
 *  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 *  ############################################################################
 */

/**
 *  The macro
 *      template(ARG, TYPE)
 *  is really a macro that wraps
 *      CAT(X, Y)
 *  and CAT(X, Y) is a token-pasting utility that produces the following:
 *      X_Y
 *
 *  For example:
 *      if T is defined to be int,
 *      and list(T) is defined to be template(list, T)
 *          The result is:
 *              list_int
 */
#define dlnode(T) template(dlnode, T)
#define dlnode_ptr(T) template(dlnode_ptr, T)
#define dlnode_dptr(T) template(dlnode_dptr, T)

#define list(T) template(list, T)
#define list_ptr(T) template(list_ptr, T)
#define list_dptr(T) template(list_dptr, T)

#define lnew(T) template(lnew, T)
#define lnewfill(T) template(lnewfill, T)
#define lnewfillptr(T) template(lnewfillptr, T)
#define lnewrnge(T) template(lnewfillrnge, T)
#define lnewcopy(T) template(lnewcopy, T)
#define lnewmove(T) template(lnewmove, T)

#define ldelete(T) template(ldelete, T)

#define lbegin(T) template(lbegin, T)
#define lend(T) template(lend, T)

#define lsize(T) template(lsize, T)
#define lmaxsize(T) template(lmaxsize, T)

#define lresizefill(T) template(lresizefill, T)
#define lresizefillptr(T) template(lresizefillptr, T)

#define lempty(T) template(lempty, T)

#define lfront(T) template(lfront, T)
#define lback(T) template(lback, T)

#define lfrontconst(T) template(lfrontconst, T)
#define lbackconst(T) template(lbackconst, T)

#define lassignrnge(T) template(lassignrnge, T)
#define lassignfill(T) template(lassignfill, T)
#define lassignfillptr(T) template(lassignfillptr, T)

#define lpushf(T) template(lpushf, T)
#define lpushfptr(T) template(lpushfptr, T)
#define lpushb(T) template(lpushb, T)
#define lpushbptr(T) template(lpushbptr, T)

#define lpopf(T) template(lpopf, T)
#define lpopb(T) template(lpopb, T)

#define linsert(T) template(linsert, T)
#define linsertptr(T) template(linsertptr, T)
#define linsertfill(T) template(linsertfill, T)
#define linsertfillptr(T) template(linsertfillptr, T)
#define linsertrnge(T) template(linsertrnge, T)
#define linsertmove(T) template(linsertmove, T)
#define linsertmoveptr(T) template(linsertmoveptr, T)

#define lerase(T) template(lerase, T)
#define lerasernge(T) template(lerasernge, T)

#define lswap(T) template(lswap, T)
#define lclear(T) template(lclear, T)

#define linsertat(T) template(linsertat, T)
#define linsertatptr(T) template(linsertatptr, T)
#define leraseat(T) template(leraseat, T)
#define lreplaceat(T) template(lreplaceat, T)
#define lreplaceatptr(T) template(lreplaceatptr, T)

#define lswapelem(T) template(lswapelem, T)

#define lsplice(T) template(lsplice, T)
#define lsplicelist(T) template(lsplicelist, T)
#define lsplicernge(T) template(lsplicernge, T)

#define lremove(T) template(lremove, T)
#define lremoveptr(T) template(lremoveptr, T)
#define lremoveif(T) template(lremoveif, T)
#define lremoveifptr(T) template(lremoveifptr, T)

#define lunique(T) template(lunique, T)

#define lmerge(T) template(lmerge, T)
#define lmergecustom(T) template(lmergecustom, T)

#define lreverse(T) template(lreverse, T)

#define lsort(T) template(lsort, T)
#define lsearch(T) template(lsearch, T)
#define lsearchptr(T) template(lsearchptr, T)

#define larrtol(T) template(larrtol, T)
#define lltoarr(T) template(lltoarr, T)

#define lputs(T) template(lputs, T)
#define lputsf(T) template(lputsf, T)
#define lfputs(T) template(lfputs, T)
#define lfputsf(T) template(lfputsf, T)

#define tmpl_list_compare(T) template(tmpl_list_compare, T)
#define tmpl_list_copy(T) template(tmpl_list_copy, T)
#define tmpl_list_print(T) template(tmpl_list_print, T)
#define tmpl_list_delete(T) template(tmpl_list_delete, T)

#define list_typetable_ptr_id(T) table_ptr_id(list, T)
#define list_iterator_table_ptr_id(T) table_ptr_id(list_iterator, T)

/**
 *  @typedef    dlnode(T)
 *  @brief      Alias for (struct dlnode(T))
 *
 *  All instances of (struct dlnode(T)) will be addressed as (dlnode(T)).
 */
typedef struct dlnode(T) dlnode(T);

/**
 *  @typedef    dlnode_ptr(T)
 *  @brief      Alias for (struct dlnode(T) *) or (dlnode(T) *)
 *
 *  All instances of (struct dlnode(T) *) will be addressed as (dlnode(T) *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct dlnode(T) * dlnode_ptr(T);

/**
 *  @typedef    dlnode_dptr(T)
 *  @brief      Alias for (struct dlnode(T) **) or (dlnode(T) **)
 *
 *  All instances of (struct dlnode(T) **) will be addressed as (dlnode(T) **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct dlnode(T) * *dlnode_dptr(T);

/**
 *  @typedef    list(T)
 *  @brief      Alias for (struct list(T))
 *
 *  All instances of (struct list(T)) will be addressed as (list(T)).
 */
typedef struct list(T) list(T);

/**
 *  @typedef    list_ptr(T)
 *  @brief      Alias for (struct list(T) *) or (list(T) *)
 *
 *  All instances of (struct list(T) *) will be addressed as (list(T) *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct list(T) * list_ptr(T);

/**
 *  @typedef    list_dptr(T)
 *  @brief      Alias for (struct list(T) **) or (list(T) **)
 *
 *  All instances of (struct list(T) **) will be addressed as (list(T) **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct list(T) * *list_dptr(T);

struct dlnode(T) {
    T data;

    dlnode(T) * prev;
    dlnode(T) * next;
};

/**< list(T): constructors */
list(T) * lnew(T)(void);
list(T) * lnewfill(T)(size_t n, T val);
list(T) * lnewfillptr(T)(size_t n, T *valaddr);
list(T) * lnewrnge(T)(iterator *first, iterator *last);
list(T) * lnewcopy(T)(list(T) * l);
list(T) * lnewmove(T)(list(T) * *l);

/**< list(T): destructor */
void ldelete(T)(list(T) * *l);

/*<< list(T): iterator functions */
iterator *lbegin(T)(list(T) * l);
iterator *lend(T)(list(T) * l);

/**< list(T): length functions */
size_t lsize(T)(list(T) * l);
size_t lmaxsize(T)(list(T) * l);

/*<< list(T): resize functions */
void lresizefill(T)(list(T) * l, size_t n, T val);
void lresizefillptr(T)(list(T) * l, size_t n, T *valaddr);

/*<< list(T): capacity-based functions */
bool lempty(T)(list(T) * l);

/*<< list(T): element access functions */
T *lfront(T)(list(T) * l);
T *lback(T)(list(T) * l);

/*<< list(T): element access functions with const-qualifier */
const T *lfrontconst(T)(list(T) * l);
const T *lbackconst(T)(list(T) * l);

/*<< list(T): modifiers - assignment */
void lassignrnge(T)(list(T) * l, iterator *first, iterator *last);
void lassignfill(T)(list(T) * l, size_t n, T val);
void lassignfillptr(T)(list(T) * l, size_t n, T *valaddr);

/*<< list(T): push/pop */
void lpushf(T)(list(T) * l, T val);
void lpushfptr(T)(list(T) * l, T *valaddr);
void lpushb(T)(list(T) * l, T val);
void lpushbptr(T)(list(T) * l, T *valaddr);
void lpopf(T)(list(T) * l);
void lpopb(T)(list(T) * l);

/*<< list(T): insertion */
iterator *linsert(T)(list(T) * l, iterator *pos, T val);
iterator *linsertptr(T)(list(T) * l, iterator *pos, T *valaddr);
iterator *linsertfill(T)(list(T) * l, iterator *pos, size_t n, T val);
iterator *linsertfillptr(T)(list(T) * l, iterator *pos, size_t n, T *valaddr);
iterator *linsertrnge(T)(list(T) * l, iterator *first, iterator *pos,
                         iterator *last);
iterator *linsertmove(T)(list(T) * l, iterator *pos, T val);
iterator *linsertmoveptr(T)(list(T) * l, iterator *pos, T *valaddr);

/*<< list(T): erasure */
iterator *lerase(T)(list(T) * l, iterator *pos);
iterator *lerasernge(T)(list(T) * l, iterator *pos, iterator *last);

/*<< list(T): container swappage */
void lswap(T)(list(T) * *l, list(T) * *other);

/*<< list(T): modifiers - clear container */
void lclear(T)(list(T) * l);

/*<< list(T): custom modifiers - insertion/erasure/replacement by index */
void linsertat(T)(list(T) * l, size_t index, T val);
void linsertatptr(T)(list(T) * l, size_t index, T *valaddr);
void leraseat(T)(list(T) * l, size_t index);
void lreplaceat(T)(list(T) * l, size_t index, T val);
void lreplaceatptr(T)(list(T) * l, size_t index, T *valaddr);

/*<< list(T): custom modifiers - element swappage */
void lswapelem(T)(list(T) * l, size_t n1, size_t n2);

/*<< list(T): splice */
iterator *lsplice(T)(list(T) * l, iterator *pos, list(T) * other,
                     iterator *opos);
iterator *lsplicelist(T)(list(T) * l, iterator *pos, list(T) * other);
iterator *lsplicernge(T)(list(T) * l, iterator *pos, list(T) * other,
                         iterator *first, iterator *last);

/*<< list(T): element removal by search */
void lremove(T)(list(T) * l, T val);
void lremoveptr(T)(list(T) * l, T *valaddr);
void lremoveif(T)(list(T) * l, T val, bool (*unary_predicate)(const void *));
void lremoveifptr(T)(list(T) * l, T *valaddr,
                     bool (*unary_predicate)(const void *));

/*<< list(T): remove duplicates */
void lunique(T)(list(T) * l);

/*<< list(T): merge/reverse */
list(T) * lmerge(T)(list(T) * l, list(T) * other);
list(T) * lmergecustom(T)(list(T) * l, list(T) * other,
                          bool (*binary_predicate)(const void *, const void *));
void lreverse(T)(list(T) * l);

/*<< list(T): utility functions - search/sort by default comparator */
void lsort(T)(list(T) * l);
int lsearch(T)(list(T) * l, T val);
int lsearchptr(T)(list(T) * l, T *valaddr);

/*<< list(T): custom utility functions - arrtol/ltoarr */
list(T) * larrtol(T)(T *base, size_t n);
T *lltoarr(T)(list(T) * l);

/*<< list(T): custom print functions - output to FILE stream */
void lputs(T)(list(T) * l);
void lputsf(T)(list(T) * l, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void lfputs(T)(list(T) * l, FILE *dest);
void lfputsf(T)(list(T) * l, FILE *dest, const char *before, const char *after,
                const char *postelem, const char *empty, size_t breaklim);

/*<< list(T): required function prototypes for (struct typetable) */
int tmpl_list_compare(T)(const void *c1, const void *c2);
void *tmpl_list_copy(T)(const void *arg);
void tmpl_list_print(T)(const void *arg, FILE *dest);
void tmpl_list_delete(T)(void *arg);

/*<< list(T): ptrs to vtables */
extern struct typetable *list_typetable_ptr_id(T);
extern struct iterator_table *list_iterator_table_ptr_id(T);

#endif /* T */
