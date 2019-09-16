/**
 *  @file       list.h
 *  @brief      Header file for a doubly-linked list ADT
 *
 *  @author     Gemuele Aludino
 *  @date       07 Sep 2019
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

#ifndef LIST_H
#define LIST_H

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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list_node            list_node;
typedef struct list_node *          list_node_ptr;
typedef struct list_node **         list_node_dptr;

struct list_node {
    struct list_node_base node;
    void *data;
};

typedef struct list     list;
typedef struct list *   list_ptr;
typedef struct list **  list_dptr;

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

/**< list: constructors */
list *l_new(struct typetable *ttbl);
list *l_newfill(struct typetable *ttbl, size_t n, void *valaddr);
list *l_newrnge(iterator first, iterator last);
list *l_newcopy(list *l);
list *l_newmove(list **l);

/**< list: destructor */
void l_delete(list **l);

/**< list: iterator functions */
iterator l_begin(list *l);
iterator l_end(list *l);

/**< list: length functions */
size_t l_size(list *l);
size_t l_maxsize(list *l);

/**< list: resize functions */
void l_resizefill(list *l, size_t n, const void *valaddr);

/**< list: capacity-based functions */
bool l_empty(list *l);

/**< list: element access functions */
void *l_front(list *l);
void *l_back(list *l);

/**< list: element access functions with const qualifier */
const void *l_front_const(list *l);
const void *l_back_const(list *l);

/**< list: modifiers - assignment */
void l_assignrnge(list *l, iterator first, iterator last);
void l_assignfill(list *l, size_t n, const void *valaddr);

/**< list: modifiers - push/pop */
void l_pushf(list *l, const void *valaddr);
void l_popf(list *l);
void l_pushb(list *l, const void *valaddr);
void l_popb(list *l);

/**< list: modifiers - insertion */
iterator l_insert(list *l, iterator pos, const void *valaddr);

iterator l_insertfill(list *l, iterator pos, size_t n, const void *valaddr);

iterator l_insertrnge(list *l, iterator pos, iterator first, iterator last);

/**< list: modifiers - erasure */
iterator l_erase(list *l, iterator pos);
iterator l_erasernge(list *l, iterator pos, iterator last);

/**< list: container swappage */
void l_swap(list **l, list **other);

/**< list: modifiers - clear container */
void l_clear(list *l);

/**< list: custom modifiers - insertion/erasure/replacement by index */
void l_insert_at(list *l, size_t index, const void *valaddr);
void l_erase_at(list *l, size_t index);
void l_replace_at(list *l, size_t index, const void *valaddr);

/**< list: custom modifiers - element swappage */
void l_swap_elem(list *l, size_t n1, size_t n2);

/**< list: splice */
iterator l_splice(list *l, iterator pos, list *other, iterator opos);
iterator l_splicelist(list *l, iterator pos, list *other);
iterator l_splicernge(list *l, iterator pos, list *other, iterator first,
                       iterator last);

/**< list: element removal by search */
void l_remove(list *l, const void *valaddr);
void l_remove_if(list *l, bool (*unary_predicate)(const void *));

/**< list: remove duplicates */
void l_unique(list *l);

/**< list: merge/reverse */
list *l_merge(list *l, list *other);

list *l_merge_custom(list *l, list *other,
                     bool (*binary_predicate)(const void *, const void *));

void l_reverse(list *l);

/**< list: utility functions - search/sort by default comparator */
void l_sort(list *l);
int l_search(list *l, const void *valaddr);

/**< list: custom utility functions - arrtol/ltoarr */
list *l_arrtol(struct typetable *ttbl, void *base, size_t n);
void *l_ltoarr(list *l);

/**< list: custom print functions - output to FILE stream */
void l_puts(list *l);

void l_putsf(list *l, const char *before, const char *after,
             const char *postelem, const char *empty, size_t breaklim);

void l_fputs(list *l, FILE *dest);

void l_fputsf(list *l, FILE *dest, const char *before, const char *after,
              const char *postelem, const char *empty, size_t breaklim);

/**< list: required function prototypes for (struct typetable) */
void *list_copy(void *arg, const void *other);
void list_dtor(void *arg);
void list_swap(void *s1, void *s2);
int list_compare(const void *c1, const void *c2);
void list_print(const void *arg, FILE *dest);

/**< list: change typetable */
void l_set_ttbl(list *l, struct typetable *ttbl);

/**< list: retrieve width/copy/dtor/swap/compare/print/typetable */
size_t l_get_width(list *l);
copy_fn l_get_copy(list *l);
dtor_fn l_get_dtor(list *l);
swap_fn l_get_swap(list *l);
compare_fn l_get_compare(list *l);
print_fn l_get_print(list *l);
struct typetable *l_get_ttbl(list *l);

#endif /* LIST_H */
