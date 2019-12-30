/**
 *  @file       slist.h
 *  @brief      Header file for a singly-linked list ADT
 *
 *  @author     Gemuele Aludino
 *  @date       05 Oct 2019
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

#ifndef SLIST_H
#define SLIST_H

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

#include <stdio.h>
#include <stdlib.h>

typedef struct slist_node slist_node;
typedef struct slist_node *slist_node_ptr;
typedef struct slist_node ** slist_node_dptr;

struct slist_node {
    struct slist_node_base head;
    void *data;
};

typedef struct slist slist;
typedef struct slist *slist_ptr;
typedef struct slist **slist_dptr;

/**
 *      By default, elements are deep copied into the containers,
 *      iff the typetable provided upon instantiation has a copy function
 *      that performs a deep copy of its argument.
 *
 *      If there is no deep copy function provided in the typetable,
 *      elements are shallow copied.
 *
 *      "fill" functions always create deep copies of elements when used.
 *      (provided a deep copy function is defined in the typetable)
 */

/**< slist: constructors */
slist *sl_new(struct typetable *ttbl);
slist *sl_newfill(struct typetable *ttbl, size_t n, void *valaddr);
slist *sl_newrnge(struct typetable *ttbl, iterator first, iterator last);
slist *sl_newcopy(slist *l);
slist *sl_newmove(slist **l);

/**< slist: destructors */
void sl_delete(slist **l);

/**< slist: iterator functions */
iterator sl_begin(slist *l);

/**< slist: length functions */
size_t sl_size(slist *l);
size_t sl_maxsize(slist *l);

/**< slist: resize functions */
void sl_resizefill(slist *l, size_t n, const void *valaddr);

/**< slist: capacity-based functions */
bool sl_empty(slist *l);

/**< slist: element access functions */
void *sl_front(slist *l);

/**< slist: element access functions with const qualifier */
const void *sl_front_const(slist *l);

/**< slist: modifiers - assignment */
void sl_assignrnge(slist *l, iterator first, iterator last);
void sl_assignfill(slist *l, size_t n, const void *valaddr);

/**< slist: modifiers - push/pop */
void sl_pushf(slist *l, const void *valaddr);
void sl_popf(slist *l, const void *valaddr);

/**< slist: modifiers - insertion (after position) */
iterator sl_insertaft(slist *l, iterator pos, const void *valaddr);
iterator sl_insertaftfill(slist *l, iterator pos, size_t n, const void *valaddr);
iterator sl_insertaftrnge(slist *l, iterator pos, iterator first, iterator last);

/**< slist: modifers - erasure (after position) */
iterator sl_eraseaft(slist *l, iterator pos);
iterator sl_eraseaftrnge(slist *l, iterator pos, iterator last);

/**< slist: container swappage */
void sl_swap(slist **l, slist **other);

/**< slist: modifiers - clear container */
void sl_clear(slist *l);

/**< slist: custom modifiers - insertion/erasure/replacement by index */
void sl_insertaft_at(slist *l, size_t index, const void *valaddr);
void sl_eraseaft_at(slist *l, size_t index);
void sl_replaceaft_at(slist *l, size_t index, const void *valaddr);

/**< slist: custom modifiers - element swappage */
void sl_swap_elem(slist *l, size_t n1, size_t n2);

/**< slist: splice (after position) */
iterator sl_spliceaft(slist *l, iterator pos, slist *other, iterator opos);
iterator sl_spliceaftlist(slist *l, iterator pos, slist *other);
iterator sl_spliceaftrnge(slist *l, iterator pos, slist *other, iterator first, iterator last);

/**< slist: element removal by search */
void sl_remove(slist *l, const void *valaddr);
void sl_remove_if(slist *l, bool (*unary_predicate)(const void *));

/**< slist: remove duplicates */
void sl_unique(slist *l);

/**< slist: merge/reverse */
slist *sl_merge(slist *l, slist *other);
slist *sl_merge_custom(slist *l, slist *other, bool (*binary_predicate)(const void *, const void *));
void sl_reverse(slist *l);

/**< slist: utility functions: search/sort by default comparator */
void sl_sort(slist *l);
int sl_search(slist *l, const void *valaddr);

/**< slist: custom utility functions - arrtol/ltoarr */
slist *sl_arrtosl(struct typetable *ttbl, void *base, size_t n);
void *sl_sltoarr(slist *l);

/**< slist: custom print functions - output to FILE stream */
void sl_puts(slist *l);
void sl_putsf(slist *l, const char *before, const char *after,
             const char *postelem, const char *empty, size_t breaklim);

void sl_fputs(slist *l, FILE *dest);

void sl_fputsf(slist *l, FILE *dest, const char *before, const char *after,
              const char *postelem, const char *empty, size_t breaklim);

/**< slist: required function prototypes for (struct typetable) */
void *slist_copy(void *arg, const void *other);
void slist_dtor(void *arg);
void slist_swap(void *s1, void *s2);
int slist_compare(const void *c1, const void *c2);
void slist_print(const void *arg, FILE *dest);

/**< slist: change typetable */
void sl_set_ttbl(slist *l, struct typetable *ttbl);

/**< slist: retrieve width/copy/dtor/swap/compare/print/typetable */
size_t sl_get_width(slist *l);
copy_fn sl_get_copy(slist *l);
dtor_fn sl_get_dtor(slist *l);
swap_fn sl_get_swap(slist *l);
compare_fn sl_get_compare(slist *l);
print_fn sl_get_print(slist *l);
struct typetable *sl_get_ttbl(slist *l);

#endif /* SLIST_H */
