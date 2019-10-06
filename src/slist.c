/**
 *  @file       slist.c
 *  @brief      Source file for a singly-linked list ADT
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

#include "slist.h"
#include "iterator.h"
#include "mergesort.h"
#include "utils.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

static slist_node *sln_allocate(void);
static void sln_init(slist_node *n, struct typetable *ttbl, const void *valaddr);
static void sln_deinit(slist_node *n, struct typetable *ttbl);

static slist_node *sln_new(struct typetable *ttbl, const void *valaddr);
static void sln_delete(slist_node **n, struct typetable *ttbl);

struct slist {
    struct slist_node impl;
    struct typetable *ttbl;
};

static slist_node *sln_node_at(slist *l, int index);

struct typetable ttbl_slist = {
    sizeof(slist),
    slist_copy,
    slist_dtor,
    slist_swap,
    slist_compare,
    slist_print
};

struct typetable *_slist_ = &ttbl_slist;

static iterator sli_begin(void *arg);

static iterator sli_next(iterator it);
static iterator sli_next_n(iterator it, int n);

static int sli_distance(iterator *first, iterator *last);

static iterator *sli_advance(iterator *it, int n);
static iterator *sli_incr(iterator *it);

static void *sli_curr(iterator it);
static void *sli_start(iterator it);

static bool sli_has_next(iterator it);

static struct typetable *sli_get_ttbl(void *arg);

struct iterator_table itbl_slist = {
    sli_begin,
    NULL,
    sli_next,
    sli_next_n,
    NULL,
    NULL,
    sli_advance,
    sli_incr,
    NULL,
    sli_curr,
    sli_start,
    NULL,
    sli_distance,
    sli_has_next,
    NULL,
    sli_get_ttbl
};

struct iterator_table *_slist_iterator_ = &itbl_slist;

slist *sl_new(struct typetable *ttbl) {
    return NULL;
}

slist *sl_newfill(struct typetable *ttbl, size_t n, void *valaddr) {
    return NULL;
}

slist *sl_newrnge(struct typetable *ttbl, iterator first, iterator last) {
    return NULL;
}

slist *sl_newcopy(slist *l) {
    return NULL;
}

slist *sl_newmove(slist **l) {
    return NULL;
}

void sl_delete(slist **l) {
    
}

iterator sl_begin(slist *l) {
    iterator it;
    return it;
}

size_t sl_size(slist *l) {
    return 0;
}

size_t sl_maxsize(slist *l) {
    return 0;
}

void sl_resizefill(slist *l, size_t n, const void *valaddr) {

}

bool sl_empty(slist *l) {
    return false;
}

void *sl_front(slist *l) {
    return NULL;
}

const void *sl_front_const(slist *l) {
    return NULL;
}

void sl_assignrnge(slist *l, iterator first, iterator last) {

}

void sl_assignfill(slist *l, size_t n, const void *valaddr) {

}

void sl_pushf(slist *l, const void *valaddr) {

}

void sl_popf(slist *l, const void *valaddr) {

}

iterator sl_insertaft(slist *l, iterator pos, const void *valaddr) {
    iterator it;

    return it;
}

iterator sl_insertaftfill(slist *l, iterator pos, size_t n, const void *valaddr) {
    iterator it;

    return it;
}

iterator sl_insertaftrnge(slist *l, iterator pos, iterator first, iterator last) {
    iterator it;

    return it;
}

iterator sl_eraseaft(slist *l, iterator pos) {
    iterator it;

    return it;
}

iterator sl_eraseaftrnge(slist *l, iterator pos, iterator last) {
    iterator it;

    return it;
}

void sl_swap(slist **l, slist **other) {

}

void sl_clear(slist *l) {

}

void sl_insertaft_at(slist *l, size_t index, const void *valaddr) {

}

void sl_eraseaft_at(slist *l, size_t index) {

}

void sl_replaceaft_at(slist *l, size_t index, const void *valaddr) {

}

void sl_swap_elem(slist *l, size_t n1, size_t n2) {

}

iterator sl_spliceaft(slist *l, iterator pos, slist *other, iterator opos) {
    iterator it;

    return it;
}

iterator sl_spliceaftlist(slist *l, iterator pos, slist *other) {
    iterator it;

    return it;
}

iterator sl_spliceaftrnge(slist *l, iterator pos, slist *other, iterator first, iterator last) {
    iterator it;

    return it;
}

void sl_remove(slist *l, const void *valaddr) {

}

void sl_remove_if(slist *l, bool (*unary_predicate)(const void *)) {

}

void sl_unique(slist *l) {

}

slist *sl_merge(slist *l, slist *other) {
    return NULL;
}

slist *sl_merge_custom(slist *l, slist *other, bool (*binary_predicate)(const void *, const void *)) {
    return NULL;
}

void sl_reverse(slist *l) {

}

void sl_sort(slist *l) {

}

int sl_search(slist *l, const void *valaddr) {
    return 0;
}

slist *sl_arrtosl(struct typetable *ttbl, void *base, size_t n) {
    return NULL;
}

void *sl_sltoarr(slist *l) {
    return NULL;
}

void sl_puts(slist *l) {

}

void sl_putsf(slist *l, const char *before, const char *after,
             const char *postelem, const char *empty, size_t breaklim) {

}

void sl_fputs(slist *l, FILE *dest) {

}

void sl_fputsf(slist *l, FILE *dest, const char *before, const char *after,
              const char *postelem, const char *empty, size_t breaklim) {

}

void *slist_copy(void *arg, const void *other) {
    return NULL;
}

void slist_dtor(void *arg) {

}

void slist_swap(void *s1, void *s2) {

}

int slist_compare(const void *c1, const void *c2) {
    return 0;
}

void slist_print(const void *arg, FILE *dest) {
    
}

void sl_set_ttbl(slist *l, struct typetable *ttbl) {

}

size_t sl_get_width(slist *l) {
    return l->ttbl->width;
}

copy_fn sl_get_copy(slist *l) {
    return l->ttbl->copy;
}

dtor_fn sl_get_dtor(slist *l) {
    return l->ttbl->dtor;
}

swap_fn sl_get_swap(slist *l) {
    return l->ttbl->swap;
}

compare_fn sl_get_compare(slist *l) {
    return l->ttbl->compare;
}

print_fn sl_get_print(slist *l) {
    return l->ttbl->print;
}

struct typetable *sl_get_ttbl(slist *l) {
    return l->ttbl;
}

static slist_node *sln_allocate(void) {
    return NULL;
}

static void sln_init(slist_node *n, struct typetable *ttbl, const void *valaddr) {

}

static void sln_deinit(slist_node *n, struct typetable *ttbl) {

}

static slist_node *sln_new(struct typetable *ttbl, const void *valaddr) {
    return NULL;
}

static void sln_delete(slist_node **n, struct typetable *ttbl) {
    
}

static slist_node *sln_node_at(slist *l, int index) {
    return NULL;
}

static iterator sli_begin(void *arg) {
    iterator it;

    return it;
}

static iterator sli_next(iterator it) {


    return it;
}

static iterator sli_next_n(iterator it, int n) {


    return it;
}

static int sli_distance(iterator *first, iterator *last) {
    return 0;
}

static iterator *sli_advance(iterator *it, int n) {
    return NULL;
}

static iterator *sli_incr(iterator *it) {
    return NULL;
}

static void *sli_curr(iterator it) {
    return NULL;
}

static void *sli_start(iterator it) {
    return NULL;
}

static bool sli_has_next(iterator it) {
    return false;
}

static struct typetable *sli_get_ttbl(void *arg) {
    return NULL;
}
