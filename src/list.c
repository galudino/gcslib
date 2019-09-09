/**
 *  @file       list.c
 *  @brief      Source file for a doubly-linked list ADT
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

#include "list.h"
#include "iterator.h"
#include "mergesort.h"
#include "utils.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static list_node *ln_allocate(void);
static void ln_init(list_node *n, struct typetable *ttbl, const void *valaddr);
static void ln_deinit(list_node *n, struct typetable *ttbl);

static list_node *ln_new(struct typetable *ttbl, const void *valaddr);
static void ln_delete(list_node **n, struct typetable *ttbl);

static void ln_swap(list_node *x, list_node *y);
static void ln_transfer(list_node *n, list_node *first, list_node *last);
static void ln_reverse(list_node *n);
static void ln_hook(list_node *n, list_node *position);
static void ln_unhook(list_node *n);
static size_t ln_distance(list_node *pos, list_node *end);

/**
 *  @struct     list
 *  @brief      Represents a doubly-linked list ADT
 *
 *  Note that struct list is opaque --
 *  its fields cannot be accessed directly,
 *  nor can instances of struct list be created on the stack.
 *  This is done to enforce encapsulation.
 */
struct list {
    struct list_base {
        struct list_node node;
    } impl;

    struct typetable *ttbl;
};

static list *l_allocate(void);
static void l_init(list *l, struct typetable *ttbl);
static void l_deinit(list *l);

static list_node *l_node_at(list *l, int index);
static list_node *l_traverse_h(list *l, int index);
static list_node *l_traverse_t(list *l, int index);

struct typetable ttbl_list = {sizeof(list), list_copy,    list_dtor,
                              list_swap,    list_compare, list_print};

struct typetable *_list_ = &ttbl_list;

static iterator li_begin(void *arg);
static iterator li_end(void *arg);

static iterator li_next(iterator it);
static iterator li_next_n(iterator it, int n);

static iterator li_prev(iterator it);
static iterator li_prev_n(iterator it, int n);

static int li_distance(iterator *first, iterator *last);

static iterator *li_advance(iterator *it, int n);
static iterator *li_incr(iterator *it);
static iterator *li_decr(iterator *it);

static void *li_curr(iterator it);
static void *li_start(iterator it);
static void *li_finish(iterator it);

static bool li_has_next(iterator it);
static bool li_has_prev(iterator it);

static struct typetable *li_get_ttbl(void *arg);

struct iterator_table itbl_list = {
    li_begin,    li_end,      li_next,     li_next_n,  li_prev,  li_prev_n,
    li_advance,  li_incr,     li_decr,     li_curr,    li_start, li_finish,
    li_distance, li_has_next, li_has_prev, li_get_ttbl};

struct iterator_table *_list_iterator_ = &itbl_list;

list *l_new(struct typetable *ttbl) { 
    list *l = l_allocate();
    l_init(l, ttbl);
    return l;
}

list *l_newfill(struct typetable *ttbl, size_t n, void *valaddr) {
    list *l = NULL;
    int i = 0;

    assert(valaddr);

    l = l_new(ttbl);

    if (l->ttbl->copy) {
        for (i = 0; i < n; i++) {
            
        }    
    } else {
        for (i = 0; i < n; i++) {

        }
    }

    return l;
}

list *l_newrnge(iterator first, iterator last) { 
    /* TODO */
    return 0;
}

list *l_newcopy(list *l) { 
    /* TODO */
    return 0;
}

list *l_newmove(list **l) { 
    /* TODO */
    return 0;
}

void l_delete(list **l) { 
    assert((*l));

    l_deinit((*l));

    free((*l));
    (*l) = NULL;
}

iterator l_begin(list *l) { 
    return li_begin(l);
}

iterator l_end(list *l) {
    return li_end(l);
}

size_t l_size(list *l) { 
    assert(l);

    if (l->impl.node.next == NULL) {
        return 0;
    } else {
        return ln_distance(l->impl.node.next, &(l->impl.node));
    }
}

size_t l_maxsize(list *l) { 
    /* TODO */
    return 0;
}

void l_resizefill(list *l, size_t n, const void *valaddr) { 
    /* TODO */

}

bool l_empty(list *l) { 
    assert(l);
    return l->impl.node.next == &(l->impl.node);
}

void *l_front(list *l) { 
    assert(l);
    return l->impl.node.next->data;
}

void *l_back(list *l) { 
    assert(l);
    return l->impl.node.prev->data;
}

const void *l_front_const(list *l) { 
    assert(l);
    return l->impl.node.next->data;
}

const void *l_back_const(list *l) { 
    assert(l);
    return l->impl.node.prev->data;
}

void l_assignrnge(list *l, iterator first, iterator last) { 
    /* TODO */

}

void l_assignfill(list *l, size_t n, const void *valaddr) { 
    /* TODO */

}

void l_pushf(list *l, const void *valaddr) { 
    list_node *new_node = NULL;

    assert(l);

    new_node = ln_new(l->ttbl, valaddr);
    /* l->impl.node.next is the head pointer. */
    ln_hook(new_node, l->impl.node.next);
}

void l_popf(list *l) { 
    list_node *node = NULL;
    assert(l);

    node = l->impl.node.next;

    ln_unhook(node);
    ln_delete(&node, l->ttbl);
}

void l_pushb(list *l, const void *valaddr) { 
    list_node *new_node = NULL;

    assert(l);
    
    new_node = ln_new(l->ttbl, valaddr);
    /* &(l->impl.node) is one node past the last elem. */
    ln_hook(new_node, &(l->impl.node));
}

void l_popb(list *l) { 
    list_node *node = NULL;
    assert(l);

    node = l->impl.node.prev;

    ln_unhook(node);
    ln_delete(&node, l->ttbl);
}

iterator l_insert(list *l, iterator pos, const void *valaddr) {
    list_node *new_node = NULL;
    size_t ipos = 0;

    assert(l);

    new_node = ln_new(l->ttbl, valaddr);
    ipos = it_distance(NULL, &pos);
    ln_hook(new_node, pos.curr);

    return it_next_n(l_begin(l), ipos);
}

iterator l_insertfill(list *l, iterator pos, size_t n,
                       const void *valaddr) { 
    /* TODO */
    iterator it;

    return it;
}

iterator l_insertrnge(list *l, iterator pos, iterator first,
                       iterator last) { 
    /* TODO */
    iterator it;


    return it;
}

iterator l_erase(list *l, iterator pos) { 
    list_node *node = NULL;
    int ipos = 0;

    assert(l);

    node = pos.curr;
    ipos = it_distance(NULL, &pos);

    ln_unhook(pos.curr);
    ln_delete(&node, l->ttbl);

    return it_next_n(l_begin(l), ipos);
}

iterator l_erasernge(list *l, iterator pos, iterator last) { 
    /* TODO */
    iterator it;

    return it;
}

void l_swap(list **l, list **other) { 
    /* TODO */

}

void l_clear(list *l) { 
    /* l->impl.node.next is the head node pointer. */
    list_node *curr = l->impl.node.next;

    /* &(l->impl.node) is the sentinel denoting EOL. */
    while (curr != &(l->impl.node)) {
        list_node *temp = curr;
        curr = curr->next;

        ln_delete(&temp, l->ttbl);
    }

    /* Reinitialize l->impl.node */
    l->impl.node.next = &(l->impl.node);
    l->impl.node.prev = &(l->impl.node);
}

void l_insert_at(list *l, size_t index, const void *valaddr) { 
    /* TODO */

}

void l_erase_at(list *l, size_t index) { 
    /* TODO */

}

void l_replace_at(list *l, size_t index, const void *valaddr) { 
    /* TODO */

}

void l_swap_elem(list *l, size_t n1, size_t n2) { 
    /* TODO */

}

iterator l_splice(list *l, iterator pos, list *other,
                   iterator opos) { 
    /* TODO */
    iterator it;

    return it;
}

iterator l_splicelist(list *l, iterator pos, list *other) { 
    /* TODO */
    iterator it;

    return it;
}

iterator l_splicernge(list *l, iterator pos, list *other, iterator first,
                       iterator last) { 
    /* TODO */
    iterator it;

    return it;
}

void l_remove(list *l, const void *valaddr) { 
    /* TODO */

}

void l_remove_if(list *l, const void *valaddr,
                 bool (*unary_predicate)(const void *)) { 
    /* TODO */

}

void l_unique(list *l) { 
    /* TODO */

}

list *l_merge(list *l, list *other) { 
    /* TODO */
    return 0;
}

list *l_merge_custom(list *l, list *other,
                     bool (*binary_predicate)(const void *,
                                              const void *)) { 
    /* TODO */
    return 0;
}

void l_reverse(list *l) { 
    /* TODO */

}

void l_sort(list *l) { 
    /* TODO */

}

int l_search(list *l, const void *valaddr) { 
    /* TODO */
    return 0;
}

list *l_arrtol(struct typetable *ttbl, void *base, size_t n) { 
    /* TODO */
    return 0;
}

void *l_ltoarr(list *l) { 
    /* TODO */
    return 0;
}

void l_puts(list *l) { 
    l_fputs(l, stdout);
}

void l_putsf(list *l, const char *before, const char *after,
             const char *postelem, const char *empty,
             size_t breaklim) { 
     l_fputsf(l, stdout, before, after, postelem, empty, breaklim);
}

void l_fputs(list *l, FILE *dest) { 
    char buffer1[MAXIMUM_STACK_BUFFER_SIZE];
    char buffer2[MAXIMUM_STACK_BUFFER_SIZE];

    const char *link = "------------------------------";
    const char *bytes_label = NULL;
    const char *postelem = "";
    const char *empty = "--- Container is empty ---";

    const size_t breaklim = 1;

    assert(l);
    assert(dest);

    sprintf(buffer1, "\n%s\n%s\n%s\n", link, "Elements", link);

    bytes_label = l->ttbl->width == 1 ? "byte" : "bytes";

    sprintf(buffer2, "%s\n%s\t\t%lu\n%s\t%lu %s\n%s\n", 
            link, "Size", l_size(l), "Element size", l->ttbl->width, bytes_label, link);

    l_fputsf(l, dest, buffer1, buffer2, postelem, empty, breaklim);
}

void l_fputsf(list *l, FILE *dest, const char *before, const char *after,
              const char *postelem, const char *empty,
              size_t breaklim) { 
    void (*print)(const void *, FILE *dest) = NULL;

    size_t size = 0;
    size_t curr = 1;

    list_node *n = NULL;
    void *target = NULL;

    assert(l);
    assert(dest);

    fprintf(dest, "%s", before ? before : "");

    print = l->ttbl->print ? l->ttbl->print : void_ptr_print;

    if (l->impl.node.next == &(l->impl.node)) {
        fprintf(dest, "%s\n", empty ? empty : "");
    } else {
        n = l->impl.node.next;
        target = n->data;

        while (n != &(l->impl.node)) {
            print(target, dest);

            /* address - disable for release */
            fprintf(dest, "\t\t(%s%p%s)", KCYN, target, KNRM);

            if (n->next == &(l->impl.node)) {
                fprintf(dest, "%s", postelem ? postelem : "");
            }

            if (curr == breaklim) {
                curr = 0;
                fprintf(dest, "\n");
            }

            n = n->next;
            target = n->data;

            ++curr;
        }
    }

    fprintf(dest, "%s", after ? after : "");
}

void *list_copy(void *arg, const void *other) {
    list **dest = NULL;
    list **source = NULL;

    assert(other);

    dest = (list **)(arg);
    source = (list **)(other);

    (*dest) = l_newcopy((*source));

    return (*dest);
}

void list_dtor(void *arg) {
    list **l = NULL;

    assert(arg);

    l = (list **)(arg);
    l_delete(l);
}

void list_swap(void *s1, void *s2) {
    list **l1 = (list **)(s1);
    list **l2 = (list **)(s2);

    if ((*l1)) {
        l_swap(l1, l2);
    } else {
        (*l1) = (*l2);
        (*l2) = NULL;
    }  
}

int list_compare(const void *c1, const void *c2) { 
    list *l1 = NULL;
    list *l2 = NULL;

    list *lst1 = NULL;
    list *lst2 = NULL;

    size_t size1 = 0;
    size_t size2 = 0;
    size_t size = 0;

    list_node *n1 = NULL;
    list_node *n2 = NULL;
    void *target1 = NULL;
    void *target2 = NULL;

    int delta = 0;
    int i = 0;

    assert(c1);
    assert(c2);

    l1 = *(list **)(c1);
    l2 = *(list **)(c2);

    if (l1->ttbl->compare != l2->ttbl->compare) {
        return -1;
    }

    lst1 = l_newcopy(l1);
    lst2 = l_newcopy(l2);

    l_sort(lst1);
    l_sort(lst2);

    size1 = l_size(lst1);
    size2 = l_size(lst2);

    size = size1 < size2 ? size1 : size2;

    n1 = lst1->impl.node.next;
    target1 = n1->data;

    n2 = lst2->impl.node.next;
    target2 = n2->data;

    for (i = 0; i < size; i++) {
        delta += lst1->ttbl->compare(&target1, &target2);

        n1 = n1->next;
        n2 = n2->next;

        target1 = n1->data;
        target2 = n2->data;
    }

    l_delete(&lst2);
    l_delete(&lst1);

    /* if delta == 0, both lists are equivalent within their common length. */
    return delta;
}

void list_print(const void *arg, FILE *dest) {
    list *l = NULL;

    assert(arg);
    assert(dest);

    l = *(list **)(arg);
    l_fputs(l, dest);
}

void l_set_ttbl(list *l, struct typetable *ttbl) {
    assert(l);
    l->ttbl = ttbl ? ttbl : _void_ptr_;
}

size_t l_get_width(list *l) {
    assert(l);
    return l->ttbl->width;
}

copy_fn l_get_copy(list *l) {
    assert(l);
    return l->ttbl->copy;
}

dtor_fn l_get_dtor(list *l) {
    assert(l);
    return l->ttbl->dtor;
}

swap_fn l_get_swap(list *l) {
    assert(l);
    return l->ttbl->swap;
}

compare_fn l_get_compare(list *l) {
    assert(l);
    return l->ttbl->compare;
}

print_fn l_get_print(list *l) {
    assert(l);
    return l->ttbl->print;
}

struct typetable *l_get_ttbl(list *l) {
    assert(l);
    return l->ttbl;
}

static list_node *ln_allocate(void) {
    list_node *n = NULL;
    n = malloc(sizeof *n);
    assert(n);
    return n;
}

static list_node *ln_new(struct typetable *ttbl, const void *valaddr) {
    list_node *n = ln_allocate();
    ln_init(n, ttbl, valaddr);
    return n;
}

static void ln_init(list_node *n, struct typetable *ttbl, const void *valaddr) {
    void *data = NULL;
    struct typetable *table = NULL;

    assert(n);
    assert(valaddr);

    n->next = NULL;
    n->prev = NULL;
    n->data = NULL;

    data = malloc(ttbl->width);
    assert(data);
    n->data = data;

    table = ttbl ? ttbl : _void_ptr_;

    if (table->copy) {
        table->copy(n->data, valaddr);
    } else {
        memcpy(n->data, valaddr, table->width);
    }
}

static void ln_deinit(list_node *n, struct typetable *ttbl) {
    struct typetable *table = NULL;

    assert(n);

    table = ttbl ? ttbl : _void_ptr_;

    if (table->dtor) {
        table->dtor(n->data);
    }

    free(n->data);
    n->data = NULL;

    n->next = NULL;
    n->prev = NULL;
}

static void ln_delete(list_node **n, struct typetable *ttbl) {
    struct typetable *table = NULL;

    assert((*n));

    table = ttbl ? ttbl : _void_ptr_;
    ln_deinit((*n), table);

    free((*n));
    (*n) = NULL;
}

static void ln_swap(list_node *x, list_node *y) {
    if (x->next != x) {
        if (y->next != y) {
            /* Both x and y are not empty. */
            list_node *tmp_x_next = x->next;
            list_node *tmp_x_prev = x->prev;

            /* swap(x->next, y->next) */
            x->next = y->next;
            y->next = tmp_x_next;

            /* swap(x->prev, y->prev) */
            x->prev = y->prev;
            y->prev = tmp_x_prev;
        } else {
            /* x is not empty, y is empty. */
            y->next = x->next;
            y->prev = x->prev;

            y->next->prev = y;
            y->prev->next = y;

            x->next = x;
            x->prev = x;
        }
    } else if (y->next != y) {
        /* x is empty, y is not empty. */
    }
}

static void ln_transfer(list_node *n, list_node *first, list_node *last) {
    if (n != last) {
        /* Remove [first, last) from its old position. */
        last->prev->next = n;
        first->prev->next = last;
        n->prev->next = first;

        /* Splice [first, last) into its new position. */
        {
            list_node *tmp_n_prev = n->prev;
            n->prev = last->prev;
            last->prev = first->prev;
            first->prev = tmp_n_prev;
        }
    }
}

static void ln_reverse(list_node *n) {
    list_node *tmp = n;
    do {
        /* swap(tmp->next, tmp->prev) */
        list_node *tmp_next = tmp->next;
        tmp->next = tmp->prev;
        tmp->prev = tmp_next;

        tmp = tmp->prev;
    } while (tmp != n);
}

static void ln_hook(list_node *n, list_node *position) {
    n->next = position;
    n->prev = position->prev;

    position->prev->next = n;
    position->prev = n;
}

static void ln_unhook(list_node *n) {
    list_node *next_node = n->next;
    list_node *prev_node = n->prev;

    prev_node->next = next_node;
    next_node->prev = prev_node;
}

static size_t ln_distance(list_node *pos, list_node *end) {
    size_t count = 0;

    while (pos != end) {
        ++count;
        pos = pos->next;
    }

    return count;
}

static list *l_allocate(void) {
    list *l = NULL;
    l = malloc(sizeof *l);
    assert(l);
    return l;
}

static void l_init(list *l, struct typetable *ttbl) {
    assert(l);

    l->impl.node.next = &(l->impl.node);
    l->impl.node.prev = &(l->impl.node);
    l->impl.node.data = NULL;

    l->ttbl = ttbl ? ttbl : _void_ptr_;

    if (l->ttbl != _void_ptr_) {
        l->ttbl->compare = l->ttbl->compare ? l->ttbl->compare : NULL;
        l->ttbl->copy = l->ttbl->copy ? l->ttbl->copy : NULL;
        l->ttbl->print = l->ttbl->print ? l->ttbl->print : NULL;
        l->ttbl->dtor = l->ttbl->dtor ? l->ttbl->dtor : NULL;
    }
}

static void l_deinit(list *l) {
    assert(l);

    l_clear(l);
    l->ttbl = NULL;
}

static list_node *l_node_at(list *l, int index) { 
    return NULL; 
}

static list_node *l_traverse_h(list *l, int index) { 
    return NULL; 
}

static list_node *l_traverse_t(list *l, int index) { 
    return NULL; 
}

static iterator li_begin(void *arg) {
    list *l = NULL;
    iterator it;

    assert(arg);

    l = (list *)(arg);

    it.itbl = _list_iterator_;
    it.container = l;
    it.curr = l->impl.node.next;

    return it;
}

static iterator li_end(void *arg) {
    list *l = NULL;
    iterator it;

    assert(arg);

    l = (list *)(arg);

    it.itbl = _list_iterator_;
    it.container = l;
    it.curr = &(l->impl.node);

    return it;
}

static iterator li_next(iterator it) {
    list *l = NULL;
    iterator iter;

    l = (list *)(it.container);

    iter.itbl = _list_iterator_;
    iter.container = l;
    iter.curr = it.curr;

    if (iter.curr == &(l->impl.node)) {
        ERROR(__FILE__, "Cannot advance - iterator already at end.");
    } else {
        iter.curr = ((list_node *)(it.curr))->next;
    }

    return iter;
}

static iterator li_next_n(iterator it, int n) {
    list *l = NULL;
    iterator iter;
    int pos = 0;

    l = (list *)(it.container);

    iter.itbl = _list_iterator_;
    iter.container = l;
    iter.curr = it.curr;

    pos = ln_distance(l->impl.node.next, iter.curr);

    if ((l_size(l) - pos) <= 0) {
        char str[256];
        sprintf(str, "Cannot advance %d times from position %d.", n, pos);
        ERROR(__FILE__, str);
    } else {
        int i = 0;
        for (i = 0; i < n; i++) {
            iter.curr = ((list_node *)(iter.curr))->next;
        }
    }

    return iter;
}

static iterator li_prev(iterator it) {
    list *l = NULL;
    iterator iter;

    l = (list *)(it.container);

    iter.itbl = _list_iterator_;
    iter.container = l;
    iter.curr = it.curr;

    if (iter.curr == l->impl.node.next) {
        ERROR(__FILE__, "Cannot retract - iterator already at begin.");
    } else {
        iter.curr = ((list_node *)(it.curr))->prev;
    }

    return iter;
}

static iterator li_prev_n(iterator it, int n) {
    list *l = NULL;
    iterator iter;
    int pos = 0;

    l = (list *)(it.container);

    iter.itbl = _list_iterator_;
    iter.container = l;
    iter.curr = it.curr;

    pos = ln_distance(l->impl.node.next, iter.curr);

    if ((l_size(l) - pos) <= 0) {
        char str[256];
        sprintf(str, "Cannot retract %d times from position %d.", n, pos);
        ERROR(__FILE__, str);
    } else {
        int i = 0;
        for (i = 0; i < n; i++) {
            iter.curr = ((list_node *)(iter.curr))->next;
        }
    }

    return iter;
}

static int li_distance(iterator *first, iterator *last) {
    list *l = NULL;

    if (first == NULL && last != NULL) {
        l = (list *)(last->container);
        return (int)(ln_distance(l->impl.node.next, last->curr));
    } else if (last == NULL && first != NULL) {
        l = (list *)(first->container);
        return (int)(ln_distance(l->impl.node.next, first->curr));
    } else if (first == NULL && last == NULL) {
        ERROR(__FILE__, "Both iterator first and last are NULL.");
        return 0;
    } else {
        l = (list *)(first->container);
        return (int)(ln_distance(first->curr, last->curr));
    }
}

static iterator *li_advance(iterator *it, int n) {
    list *l = NULL;
    int pos = 0;

    assert(it);

    pos = ln_distance(l->impl.node.next, it->curr);

    if ((l_size(l) - pos) < 0) {
        char str[256];
        sprintf(str, "Cannot advance %d times from position %d.", n, pos);
        ERROR(__FILE__, str);
    } else {
        int i = 0;

        if (n > 0) {
            for (i = 0; i < n; i++) {
                it->curr = ((list_node *)(it->curr))->next;
            }
        } else if (n < 0) {
            for (i = 0; i < n; i++) {
                it->curr = ((list_node *)(it->curr))->prev;
            }
        }
    }

    return it;
}

static iterator *li_incr(iterator *it) {
    list *l = NULL;

    assert(it);

    if (it->curr == &(l->impl.node)) {
        ERROR(__FILE__, "Cannot decrement - already at end.");
    } else {
        it->curr = ((list_node *)(it->curr))->next;
    }

    return it;
}

static iterator *li_decr(iterator *it) {
    list *l = NULL;

    assert(it);

    l = (list *)(it->container);

    if (it->curr == l->impl.node.next) {
        ERROR(__FILE__, "Cannot decrement - already at begin.");
    } else {
        it->curr = ((list_node *)(it->curr))->prev;
    }

    return it;
}

static void *li_curr(iterator it) {
    list_node *n = it.curr;
    return n->data;
}

static void *li_start(iterator it) {
    list *l = (list *)(it.container);
    return l->impl.node.next->data;
}

static void *li_finish(iterator it) {
    list *l = (list *)(it.container);
    return l->impl.node.data;
}

static bool li_has_next(iterator it) {
    list *l = (list *)(it.container);
    return it.curr != &(l->impl.node);
}

static bool li_has_prev(iterator it) {
    list *l = (list *)(it.container);
    return it.curr != &(l->impl.node);
}

static struct typetable *li_get_ttbl(void *arg) {
    list *l = (list *)(arg);
    return l->ttbl;
}
