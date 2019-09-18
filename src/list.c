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
    struct list_node impl;
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
    size_t i = 0;

    assert(valaddr);

    l = l_new(ttbl);

    for (i = 0; i < n; i++) {
        l_pushb(l, valaddr);
    }

    return l;
}

list *l_newrnge(iterator first, iterator last) { 
    list *l = NULL;

    struct typetable *ttbl_first = NULL;
    void *sentinel = NULL;
    void *curr = NULL;
    
    if (first.itbl != last.itbl) {
        ERROR(__FILE__, "first and last must have matching container types and refer to the same container.");
        return NULL;
    }

    ttbl_first = it_get_ttbl(first);
    l = l_new(ttbl_first);

    sentinel = it_curr(last);

    while ((curr = it_curr(first)) != sentinel) {
        l_pushb(l, curr);
        it_incr(&first);
    }

    return l;
}

list *l_newcopy(list *l) { 
    assert(l);
    return l_newrnge(l_begin(l), l_end(l));
}

list *l_newmove(list **l) { 
    list *move = NULL;

    assert((*l));

    move = l_new((*l)->ttbl);

    move = (*l);
    (*l) = l_new(move->ttbl);

    return move;
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
        return lnb_distance(l->impl.node.next, &(l->impl.node));
    }
}

size_t l_maxsize(list *l) { 
    /* TODO */
    LOG(__FILE__, "TODO");
    return 0;
}

void l_resizefill(list *l, size_t n, const void *valaddr) { 
    size_t size = l_size(l);

    if (n > size) {
        size_t i = 0;
        size_t delta = n - size;
    
        for (i = 0; i < delta; i++) {
            l_pushb(l, valaddr);
        }
    } else {
        int i = -1;

        /**
         *  Code for l_clear(), inlined.
         */

        /* l->impl.node.next is the head node pointer. */
        list_node *curr = *(list_node **)(&l->impl.node.next);

        /* &(l->impl.node) is the sentinel denoting EOL. */
        while (*(list_node_base **)(&curr) != &(l->impl.node)) {
            list_node *temp = curr;
            curr = *(list_node **)(&curr->node.next);

            ln_delete(&temp, l->ttbl);
        }

        /* Reinitialize l->impl.node */
        l->impl.node.next = &(l->impl.node);
        l->impl.node.prev = &(l->impl.node);

        for (i = 0; i < n; i++) {
            l_pushb(l, valaddr);
        }
    }
}

bool l_empty(list *l) { 
    assert(l);
    return l->impl.node.next == &(l->impl.node);
}

void *l_front(list *l) { 
    assert(l);
    /**
     *  Note that the address of an object
     *  (instance of a struct)
     *  is the same address as that
     *  of said object's first field.
     * 
     *  l->impl.node.next is a (list_node_base *),
     *  which is the head pointer of list.
     *  
     *  The object, named node, that houses list_node_base
     *  is of type list_node.
     * 
     *  (&l->impl.node.next) is a (list_node_base **).
     *  We can typecast (list_node_base **) to (list_node **),
     *  like this:
     *  (list_node **)(&l->impl.node.next);
     *  and dereference it like this:
     *  *(list_node **)(&l->impl.node.next);
     *  to yield:
     *  list_node *n = *(list_node **)(&l->impl.node.next);
     * 
     *  Now, we can access n's data field, via n->data.
     * 
     *  At the client level, l_front returns a (TYPENAME *).
     * 
     *  Retrieve a (TYPENAME *) like this:
     *  TYPENAME *valaddr = (TYPENAME *)(l_front(l));
     *  Retrieve a (TYPENAME) like this:
     *  TYPENAME val = (*valaddr);
     *  or
     *  TYPENAME val = *(TYPENAME *)(l_front(l));
     */
    return (*(list_node **)(&l->impl.node.next))->data;
}

void *l_back(list *l) { 
    assert(l);
    /**
     *  Note that the address of an object
     *  (instance of a struct)
     *  is the same address as that
     *  of said object's first field.
     * 
     *  l->impl.node.prev is a (list_node_base *),
     *  which is the tail pointer of list.
     *  
     *  The object, named node, that houses list_node_base
     *  is of type list_node.
     * 
     *  (&l->impl.node.prev) is a (list_node_base **).
     *  We can typecast (list_node_base **) to (list_node **),
     *  like this:
     *  (list_node **)(&l->impl.node.prev);
     *  and dereference it like this:
     *  *(list_node **)(&l->impl.node.prev);
     *  to yield:
     *  list_node *n = *(list_node **)(&l->impl.node.prev);
     * 
     *  Now, we can access n's data field, via n->data.
     * 
     *  At the client level, l_back returns a (TYPENAME *).
     * 
     *  Retrieve a (TYPENAME *) like this:
     *  TYPENAME *valaddr = (TYPENAME *)(l_back(l));
     *  Retrieve a (TYPENAME) like this:
     *  TYPENAME val = (*valaddr);
     *  or
     *  TYPENAME val = *(TYPENAME *)(l_back(l));
     */
    return (*(list_node **)(&l->impl.node.prev))->data;
}

const void *l_front_const(list *l) { 
    assert(l);
    /**
     *  Note that the address of an object
     *  (instance of a struct)
     *  is the same address as that
     *  of said object's first field.
     * 
     *  l->impl.node.next is a (list_node_base *),
     *  which is the head pointer of list.
     *  
     *  The object, named node, that houses list_node_base
     *  is of type list_node.
     * 
     *  (&l->impl.node.next) is a (list_node_base **).
     *  We can typecast (list_node_base **) to (list_node **),
     *  like this:
     *  (list_node **)(&l->impl.node.next);
     *  and dereference it like this:
     *  *(list_node **)(&l->impl.node.next);
     *  to yield:
     *  list_node *n = *(list_node **)(&l->impl.node.next);
     * 
     *  Now, we can access n's data field, via n->data.
     * 
     *  At the client level, l_front returns a (TYPENAME *).
     * 
     *  Retrieve a (TYPENAME *) like this:
     *  TYPENAME *valaddr = (TYPENAME *)(l_front(l));
     *  Retrieve a (TYPENAME) like this:
     *  TYPENAME val = (*valaddr);
     *  or
     *  TYPENAME val = *(TYPENAME *)(l_front(l));
     */
    return (*(list_node **)(&l->impl.node.next))->data;
}

const void *l_back_const(list *l) { 
    assert(l);
    /**
     *  Note that the address of an object
     *  (instance of a struct)
     *  is the same address as that
     *  of said object's first field.
     * 
     *  l->impl.node.prev is a (list_node_base *),
     *  which is the tail pointer of list.
     *  
     *  The object, named node, that houses list_node_base
     *  is of type list_node.
     * 
     *  (&l->impl.node.prev) is a (list_node_base **).
     *  We can typecast (list_node_base **) to (list_node **),
     *  like this:
     *  (list_node **)(&l->impl.node.prev);
     *  and dereference it like this:
     *  *(list_node **)(&l->impl.node.prev);
     *  to yield:
     *  list_node *n = *(list_node **)(&l->impl.node.prev);
     * 
     *  Now, we can access n's data field, via n->data.
     * 
     *  At the client level, l_back returns a (TYPENAME *).
     * 
     *  Retrieve a (TYPENAME *) like this:
     *  TYPENAME *valaddr = (TYPENAME *)(l_back(l));
     *  Retrieve a (TYPENAME) like this:
     *  TYPENAME val = (*valaddr);
     *  or
     *  TYPENAME val = *(TYPENAME *)(l_back(l));
     */
    return (*(list_node **)(&l->impl.node.prev))->data;
}

void l_assignrnge(list *l, iterator first, iterator last) { 
    void *curr = NULL;
    void *sentinel = NULL;

    assert(l);
    l_clear(l);

    sentinel = it_curr(last);
    
    while ((curr = it_curr(first)) != sentinel) {
        list_node *new_node = ln_new(l->ttbl, curr);
        lnb_hook(*(list_node_base **)(&new_node), &(l->impl.node));

        it_incr(&first);
    }
}

void l_assignfill(list *l, size_t n, const void *valaddr) { 
    size_t i = 0;
    assert(l);
    
    l_clear(l);

    for (i = 0; i < n; i++) {
        list_node *new_node = ln_new(l->ttbl, valaddr);
        lnb_hook(*(list_node_base **)(&new_node), &(l->impl.node));
    }
}

void l_pushf(list *l, const void *valaddr) { 
    list_node *new_node = NULL;

    assert(l);

    new_node = ln_new(l->ttbl, valaddr);
    /* l->impl.node.next is the head pointer. */
    lnb_hook(*(list_node_base **)(&new_node), l->impl.node.next);
}

void l_popf(list *l) { 
    list_node *node = NULL;
    assert(l);

    node = *(list_node **)(&l->impl.node.next);
    
    lnb_unhook(*(list_node_base **)(&node));
    ln_delete(&node, l->ttbl);
}

void l_pushb(list *l, const void *valaddr) { 
    list_node *new_node = NULL;

    assert(l);
    
    new_node = ln_new(l->ttbl, valaddr);
    /* &(l->impl.node) is one node past the last elem. */
    lnb_hook(*(list_node_base **)(&new_node), &(l->impl.node)); 
}

void l_popb(list *l) { 
    list_node *node = NULL;
    assert(l);

    node = *(list_node **)(&l->impl.node.prev);

    lnb_unhook(*(list_node_base **)(&node));
    ln_delete(&node, l->ttbl);
}

iterator l_insert(list *l, iterator pos, const void *valaddr) {
    list_node *new_node = NULL;

    assert(l);

    new_node = ln_new(l->ttbl, valaddr);
    lnb_hook(*(list_node_base **)(&new_node), pos.curr);

    return pos;
}

iterator l_insertfill(list *l, iterator pos, size_t n,
                      const void *valaddr) {
    size_t i = 0;

    assert(l);
    
    for (i = 0; i < n; i++) {
        list_node *new_node = ln_new(l->ttbl, valaddr);
        lnb_hook(*(list_node_base **)(&new_node), pos.curr);
    }
    
    return pos;
}

iterator l_insertrnge(list *l, iterator pos, iterator first,
                       iterator last) { 
    void *curr = NULL;
    void *sentinel = NULL;
    
    assert(l);
    sentinel = it_curr(last);

    while ((curr = it_curr(first)) != sentinel) {
        list_node *new_node = ln_new(l->ttbl, curr);
        lnb_hook(*(list_node_base **)(&new_node), pos.curr);

        it_incr(&first);
    }

    return pos;
}

iterator l_erase(list *l, iterator pos) { 
    list_node *n = NULL;
    assert(l);
    
    n = *(list_node **)(&pos.curr);

    lnb_unhook(*(list_node_base **)(&n));
    it_incr(&pos);
    ln_delete(&n, l->ttbl);

    return pos;
}

iterator l_erasernge(list *l, iterator pos, iterator last) { 
    void *curr = NULL;
    void *sentinel = NULL;

    assert(l);
    sentinel = it_curr(last);

    while ((curr = it_curr(pos)) != sentinel) {
        list_node *node = (*(list_node **)(&pos.curr));
        it_incr(&pos);

        lnb_unhook(*(list_node_base **)(&node));
        ln_delete(&node, l->ttbl);
    }

    return pos;
}

void l_swap(list **l, list **other) { 
    list *temp = NULL;

    assert((*l));
    assert((*other));

    temp = (*l);

    (*l) = (*other);
    (*other) = temp;
}

void l_clear(list *l) { 
    /* l->impl.node.next is the head node pointer. */
    list_node *curr = *(list_node **)(&l->impl.node.next);

    /* &(l->impl.node) is the sentinel denoting EOL. */
    while (*(list_node_base **)(&curr) != &(l->impl.node)) {
        list_node *temp = curr;
        curr = *(list_node **)(&curr->node.next);

        ln_delete(&temp, l->ttbl);
    }

    /* Reinitialize l->impl.node */
    l->impl.node.next = &(l->impl.node);
    l->impl.node.prev = &(l->impl.node);
}

void l_insert_at(list *l, size_t index, const void *valaddr) { 
    list_node *new_node = NULL;
    list_node *pos = NULL;

    assert(l);

    if (index >= l_size(l)) {
        ERROR(__FILE__, "Index out of bounds.");
        return;
    }

    pos = l_node_at(l, index);          /**< traverse to node at index */
    new_node = ln_new(l->ttbl, valaddr);/**< create new node */

    /**
     *  The former node at position (index) will move to position (index + 1).
     *  new_node is now the node at position (index).
     */
    lnb_hook(*(list_node_base **)(&new_node), (*(list_node_base **)(&pos)));
}

void l_erase_at(list *l, size_t index) { 
    list_node *n = NULL;

    assert(l);

    if (index >= l_size(l)) {
        ERROR(__FILE__, "Index out of bounds.");
        return;
    }

    n = l_node_at(l, index);          /**< traverse to node at index */

    /**
     *  Unlink node at position (index) and release its memory.
     */
    lnb_unhook((*(list_node_base **)(&n)));
    ln_delete(&n, l->ttbl);
}

void l_replace_at(list *l, size_t index, const void *valaddr) { 
    list_node *n = NULL;

    assert(l);

    if (index >= l_size(l)) {
        ERROR(__FILE__, "Index out of bounds.");
        return;
    }

    n = l_node_at(l, index);          /**< traverse to node at index */

    /**
     *  If element lives on the heap,
     *  or has fields that live on the heap,
     *  release their memory.
     */
    if (l->ttbl->dtor) {
        l->ttbl->dtor(n->data);
    }

    /**
     *  If element typetable can deep-copy,
     *  deep copy valaddr into the node.
     *  Else, shallow copy valaddr.
     */
    if (l->ttbl->copy) {
        l->ttbl->copy(n->data, valaddr);
    } else {
        memcpy(n->data, valaddr, l->ttbl->width);
    }
}

void l_swap_elem(list *l, size_t n1, size_t n2) { 
    size_t size = 0;

    bool n1_bad = false;
    bool n2_bad = false;
    bool good_indices = false;

    void *temp = NULL;
    void *data_1 = NULL;
    void *data_2 = NULL;

    assert(l);

    size = l_size(l);

    n1_bad = n1 >= size;
    n2_bad = n2 >= size;

    good_indices = !n1_bad && !n2_bad;

    if (good_indices && size > 0) {
        data_1 = l_node_at(l, n1)->data;
        data_2 = l_node_at(l, n2)->data;

        if (l->ttbl->swap) {
            l->ttbl->swap(data_1, data_2);
        } else {
            temp = malloc(l->ttbl->width);
            assert(temp);
            memcpy(temp, data_1, l->ttbl->width);

            memcpy(data_1, data_2, l->ttbl->width);
            memcpy(data_2, temp, l->ttbl->width);

            free(temp);
            temp = NULL;
        }
    } else {
        char str[256];
        sprintf(str, "Indices n1 [%lu] and/or n2 [%lu] are out of bounds.", n1, n2);
        ERROR(__FILE__, str);
        return;
    }
}

iterator l_splice(list *l, iterator pos, list *other,
                   iterator opos) { 
    iterator j = opos;
    it_incr(&j);

    assert(l);

    if (pos.itbl != _list_iterator_ || opos.itbl != _list_iterator_) {
        ERROR(__FILE__, "iterator pos and iterator opos must refer to unique instances of (struct list *).");
        return pos;
    }

    if (pos.curr == opos.curr || pos.curr == j.curr) {
        ERROR(__FILE__, "(pos.curr) and (opos.curr) must refer to unique instances of (struct list_node_base *).");
        return pos;
    }

    if (l != other) {
        if (l->ttbl != other->ttbl) {
            ERROR(__FILE__, "Cannot splice lists that are using different typetables.");
            return pos;
        }
    }

    lnb_transfer(pos.curr, opos.curr, j.curr);
    return pos;
}

iterator l_splicelist(list *l, iterator pos, list *other) { 
    assert(l);

    if (pos.itbl != _list_iterator_) {
        ERROR(__FILE__, "iterator pos and iterator opos must refer to unique instances of (struct list *).");
        return pos;
    }

    if (pos.curr == other->impl.node.next) {
        ERROR(__FILE__, "(pos.curr) and (other->impl.node.next) must refer to unique instances of (struct list_node_base *).");
        return pos;
    }

    if (l_empty(other) == false) {
        if (l->ttbl != other->ttbl) {
            ERROR(__FILE__, "Cannot splice lists that are using different typetables.");
        } else {
            list_node_base *beg = l_begin(other).curr;
            list_node_base *end = l_end(other).curr;

            lnb_transfer(pos.curr, beg, end);
        }
    }

    return pos;
}

iterator l_splicernge(list *l, iterator pos, list *other, iterator first,
                       iterator last) { 
    assert(l);

    if (pos.itbl != _list_iterator_ || first.itbl != _list_iterator_ || last.itbl != _list_iterator_) {
        ERROR(__FILE__, "Each iterator (pos, first, and last) must refer to unique instances of (struct list *).");
        return pos;
    }

    if (pos.curr == first.curr || pos.curr == last.curr) {
        ERROR(__FILE__, "(.curr) in iterator pos and iterator opos must refer to unique instances of (struct list_node_base *).");
        return pos;
    }

    if (l_empty(other) == false) {
        if (l->ttbl != other->ttbl) {
            ERROR(__FILE__, "Cannot splice lists that are using different typetables.");
        } else {
            list_node_base *beg = first.curr;
            list_node_base *end = last.curr;

            lnb_transfer(pos.curr, beg, end);
        }
    }

    return pos;
}

void l_remove(list *l, const void *valaddr) { 
    iterator first = { NULL, NULL, NULL };
    iterator last = { NULL, NULL, NULL };

    int (*compare)(const void *, const void *) = NULL;

    assert(l);

    first = l_begin(l);
    last = l_end(l);

    compare = l->ttbl->compare ? l->ttbl->compare : void_ptr_compare;

    while (first.curr != last.curr) {
        iterator next = first;
        it_incr(&next);

        if (compare(it_curr(first), valaddr) == 0) {
            l_erase(l, first);
        }

        first = next;
    }
}

void l_remove_if(list *l, bool (*unary_predicate)(const void *)) { 
    iterator first = { NULL, NULL, NULL };
    iterator last = { NULL, NULL, NULL };

    int (*compare)(const void *, const void *) = NULL;

    assert(l);

    first = l_begin(l);
    last = l_end(l);

    compare = l->ttbl->compare ? l->ttbl->compare : void_ptr_compare;

    while (first.curr != last.curr) {
        iterator next = first;
        it_incr(&next);

        if (unary_predicate(it_curr(first))) {
            l_erase(l, first);
        }

        first = next;
    }
}

void l_unique(list *l) { 
    /*
    iterator first = begin();
    iterator last = end();

    if (first == last) {
        return;
    }

    iterator next = first;

    while (++next != last) {
        if (*first == *next) {
            erase(next);
        } else {
            first = next;
        }

        next = first;
    }
    */

    /* TODO - not working
    iterator first = { NULL, NULL, NULL };
    iterator last = { NULL, NULL, NULL };
    iterator next = { NULL, NULL, NULL };
    void *curr = NULL;
    void *sentinel = NULL;

    int (*compare)(const void *, const void *) = NULL;

    assert(l);

    first = l_begin(l);
    last = l_end(l);
    sentinel = it_curr(last);

    next.itbl = _list_iterator_;
    next.container = l;
    next.curr = first.curr;

    while (next.curr != last.curr) {
        printf("%d\n", *(int *)(it_curr(next)));
        it_incr(&next);
    }
    */
}

list *l_merge(list *l, list *other) { 
    /*
    iterator first1 = l_begin(l);
    iterator last1 = l_end(l);
    iterator first2 = l_begin(other);
    iterator last2 = l_end(other);

    int (*compare)(const void *, const void *) = 
    l->ttbl->compare ? l->ttbl->compare : void_ptr_compare;

    while (first1.curr != last1.curr  && first2.curr != last2.curr) {
        if (compare(it_curr(first2), it_curr(last2)) < 0) {
            iterator next = first2;

            it_incr(&next);
            lnb_transfer(first1.curr, first2.curr, next.curr);
        } else {
            it_incr(&first1);
        }
    }
    */





    return l;
}

list *l_merge_custom(list *l, list *other,
                     bool (*binary_predicate)(const void *,
                                              const void *)) { 
    /* TODO */
    return 0;
}

void l_reverse(list *l) { 
    assert(l);
    lnb_reverse(l->impl.node.next);
}

void l_sort(list *l) { 
    size_t size = 0;
    int (*comparator)(const void *, const void *) = NULL; 

    assert(l);

    size = l_size(l);

    if (size < 2) {
        /* why sort a data structure if size < 2? */
        return;
    }

    comparator = l->ttbl->compare ? l->ttbl->compare : void_ptr_compare;

    /* gcslib mergesort */
    /*
    lnb_mergesort_recursive(&(l->impl.node.next), &(l->impl.node.prev), comparator);
    */


    /*
        // Do nothing if the list has length 0 or 1.
    if (this->impl.node.next != &this->impl.node &&
        this->impl.node.next->next != &this->impl.node) {
        list carry;
        list tmp[64];
        list *fill = &tmp[0];
        list *counter;

        do {
            carry.splice(carry.begin(), *this, begin());

            for (counter = &tmp[0]; counter != fill && !counter->empty();
                 ++counter) {
                counter->merge(carry);
                carry.swap(*counter);
            }

            carry.swap(*counter);

            if (counter == fill) {
                ++fill;
            }
        } while (!empty());

        for (counter = &tmp[1]; counter != fill; ++counter) {
            counter->merge(*(counter - 1));
        }

        swap(*(fill - 1));
    }


    TODO l_merge
    TODO
    */
}

int l_search(list *l, const void *valaddr) { 
    iterator first = { NULL, NULL, NULL };
    iterator last = { NULL, NULL, NULL };
    int result = -1;

    int (*compare)(const void *, const void *) = NULL;

    assert(l);

    first = l_begin(l);
    last = l_end(l);

    compare = l->ttbl->compare ? l->ttbl->compare : void_ptr_compare;

    while (first.curr != last.curr) {
        ++result;
        if (compare(it_curr(first), valaddr) == 0) {
            printf("found: %d\n", *(int *)(it_curr(first)));
            break;
        }

        it_incr(&first);
    }

    return result;
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
        n = *(list_node **)(&l->impl.node.next);
        target = n->data;

        while (*(list_node_base **)(&n) != &(l->impl.node)) {
            print(target, dest);

            /* address - disable for release */
            fprintf(dest, "\t\t(%s%p%s)", KCYN, target, KNRM);

            if (*(list_node_base **)(&n->node.next) == &(l->impl.node)) {
                fprintf(dest, "%s", postelem ? postelem : "");
            }

            if (curr == breaklim) {
                curr = 0;
                fprintf(dest, "\n");
            }

            n = *(list_node **)(&n->node.next);
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

    n1 = *(list_node **)(&lst1->impl.node.next);
    target1 = n1->data;

    n2 = *(list_node **)(&lst2->impl.node.next);
    target2 = n2->data;

    for (i = 0; i < size; i++) {
        delta += lst1->ttbl->compare(&target1, &target2);

        n1 = *(list_node **)(&n1->node.next);
        n2 = *(list_node **)(&n2->node.next);

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

    n->node.next = NULL;
    n->node.prev = NULL;
    n->data = NULL;

    table = ttbl ? ttbl : _void_ptr_;

    data = malloc(table->width);
    assert(data);
    n->data = data;

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

    n->node.next = NULL;
    n->node.prev = NULL;
}

static void ln_delete(list_node **n, struct typetable *ttbl) {
    struct typetable *table = NULL;

    assert((*n));

    table = ttbl ? ttbl : _void_ptr_;
    ln_deinit((*n), table);

    free((*n));
    (*n) = NULL;
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
    l->impl.data = NULL;

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
    list_node *n = NULL;
    size_t size = 0;

    assert(l);
    size = l_size(l);

    if (index < size) {
        n = (index < (size / 2)) 
        ? l_traverse_h(l, index) : l_traverse_t(l, index);
    } else {
        char str[256];
        sprintf(str, "Index provided [%d] is out of bounds, size is [%lu]. NULL node returned.", index, size);
        ERROR(__FILE__, str);
    }

    return n;
}

static list_node *l_traverse_h(list *l, int index) { 
    list_node_base *n = NULL;
    int i = -1;

    assert(l);

    for (i = 0, n = l->impl.node.next; 
         i < index; i++, 
         n = n->next) {
        /* get to the node */
    }

    return *(list_node **)(&n);
}

static list_node *l_traverse_t(list *l, int index) { 
    list_node_base *n = NULL;
    int i = -1;
    int delta = -1;

    assert(l);

    delta = (l_size(l) - 1) - index;
    for (i = 0, n = l->impl.node.prev; 
         i < delta; i++, 
         n = n->prev) {
        /* get to the node */
    }

    return *(list_node **)(&n);
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
        iter.curr = ((list_node *)(iter.curr))->node.next;
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

    pos = lnb_distance(l->impl.node.next, iter.curr);

    if ((l_size(l) - pos) <= 0) {
        char str[256];
        sprintf(str, "Cannot advance %d times from position %d.", n, pos);
        ERROR(__FILE__, str);
    } else {
        int i = 0;
        for (i = 0; i < n; i++) {
            iter.curr = ((list_node *)(iter.curr))->node.next;
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
        iter.curr = ((list_node *)(it.curr))->node.prev;
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

    pos = lnb_distance(l->impl.node.next, iter.curr);

    if ((l_size(l) - pos) <= 0) {
        char str[256];
        sprintf(str, "Cannot retract %d times from position %d.", n, pos);
        ERROR(__FILE__, str);
    } else {
        int i = 0;
        for (i = 0; i < n; i++) {
            iter.curr = ((list_node *)(iter.curr))->node.next;
        }
    }

    return iter;
}

static int li_distance(iterator *first, iterator *last) {
    list *l = NULL;

    if (first == NULL && last != NULL) {
        l = (list *)(last->container);
        return (int)(lnb_distance(l->impl.node.next, last->curr));
    } else if (last == NULL && first != NULL) {
        l = (list *)(first->container);
        return (int)(lnb_distance(l->impl.node.next, first->curr));
    } else if (first == NULL && last == NULL) {
        ERROR(__FILE__, "Both iterator first and last are NULL.");
        return 0;
    } else {
        l = (list *)(first->container);
        return (int)(lnb_distance(first->curr, last->curr));
    }
}
    
static iterator *li_advance(iterator *it, int n) {
    list *l = NULL;
    int pos = 0;

    assert(it);

    pos = lnb_distance(l->impl.node.next, it->curr);

    if ((l_size(l) - pos) < 0) {
        char str[256];
        sprintf(str, "Cannot advance %d times from position %d.", n, pos);
        ERROR(__FILE__, str);
    } else {
        int i = 0;

        if (n > 0) {
            for (i = 0; i < n; i++) {
                it->curr = ((list_node *)(it->curr))->node.next;
            }
        } else if (n < 0) {
            for (i = 0; i < n; i++) {
                it->curr = ((list_node *)(it->curr))->node.prev;
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
        it->curr = ((list_node *)(it->curr))->node.next;
    }

    return it;
}

static iterator *li_decr(iterator *it) {
    list *l = NULL;

    assert(it);

    l = (list *)(it->container);

    if (it->curr == &(l->impl.node)) {
        ERROR(__FILE__, "Cannot decrement - already at begin.");
    } else {
        it->curr = ((list_node *)(it->curr))->node.prev;
    }

    return it;
}

static void *li_curr(iterator it) {
    list_node *n = it.curr;
    return n->data;
}

static void *li_start(iterator it) {
    list *l = (list *)(it.container);
    list_node_base *nb = l->impl.node.next;
    list_node *n = *(list_node **)(&nb);
    return n->data;
}

static void *li_finish(iterator it) {
    list *l = (list *)(it.container);
    list_node_base *nb = &(l->impl.node);
    list_node *n = *(list_node **)(&nb);
    return n->data;
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
