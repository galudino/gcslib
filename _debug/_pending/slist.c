/**
 *  @file       slist.c
 *  @brief      Source file for a singly-linked list ADT
 *
 *  @author     Gemuele Aludino
 *  @date       26 Jun 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>

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
 *  @file       slist.h
 *  @brief      Public header file for access to slist-related functions
 */
#include "slist.h"

/**
 *  @def        SLIST_MAXIMUM_STACK_BUFFER_SIZE
 *  @brief      Arbitrary maximum size for a statically allocated (char) buffer
 */
#define SLIST_MAXIMUM_STACK_BUFFER_SIZE    16384

/**
 *  @file       mergesort.h
 *  @brief      Routines for mergesort on linear node-based ADTs
 */
#include "mergesort.h"

/**< slnode: allocation/initialization */
static slnode *sn_allocate(void);
static slnode *sn_new(const void *valaddr, size_t width);
static void sn_init(slnode *n, const void *valaddr, size_t width);
static void sn_deinit(slnode *n, void (*delete)(void *));
static void sn_delete(void *arg, void (*delete)(void *));

struct slist {
    slnode *head;
    struct typetable *ttbl;
};

/**< slist: node access */
static slnode *sl_node_sn(slist *l, int index);

/**< slist: node traversal */
static slnode *sl_traverse_h(slist *l, int index);

/**< slist: allocation/initialization */
static slist *sl_allocate(void);
static void sl_init(slist *l, struct typetable *ttbl);
static void sl_deinit(slist *l);

struct typetable ttbl_slist = {
    sizeof(struct slist),
    slist_compare,
    slist_copy,
    slist_print,
    slist_delete
};

struct typetable *_slist_ = &ttbl_slist;

typedef struct slist_iterator slist_iterator;
typedef struct slist_iterator * slist_iterator_ptr;
typedef struct slist_iterator ** slist_iterator_dptr;

struct slist_iterator {
    slist *l;
    int index;
    slnode *curr;
    slnode *before_begin;
    slnode *end;
};

/**< slist_iterator: create/initialize/destroy */
static void *sli_allocate(void);
static void *sli_new(void *arg_sl);
static void sli_init(void *arg_sli, void *arg_sl);
static void sli_deinit(void *arg_sli);
static void sli_delete(void *arg_sli);

/**< slist_iterator: index based operations */
static void *sli_advance(void *arg_sli, int n);
static int sli_distance(void *arg_sli_first, void *arg_sli_last);

/**< list_iterator: return iterator to begin/end */
static void *sli_begin(void *arg_sli);
static void *sli_end(void *arg_sli);

/**< list_iterator: advance/retract iterator position */
static void *sli_next(void *arg_sli);
static void *sli_prev(void *arg_sli);

/**< list_iterator: determine if there are more elements to traverse */
static bool sli_has_next(void *arg_sli);
static bool sli_has_prev(void *arg_sli);

/**< list_iterator: retrieve (typetable *) */
static struct typetable *sli_get_ttbl(void *arg_sli);

/**
 *  iterator_table itbl_list
 *  @brief      Instance of struct iterator_table used by iterator
 *              (see iterator.h)
 */
struct iterator_table itbl_slist = {
    sli_new,  sli_delete, sli_advance,  sli_distance, sli_begin, sli_end,
    sli_next, sli_prev,   sli_has_next, sli_has_prev, sli_get_ttbl
};

struct iterator_table *_slist_iterator_ = &itbl_slist;

slist *sl_new(struct typetable *ttbl) {
    /* TODO */
    return 0;
}

slist *sl_newfill(struct typetable *ttbl, size_t n, void *valaddr) {
    /* TODO */
    return 0;
}

slist *sl_newrnge(iterator *first, iterator *last) {
    /* TODO */
    return 0;
}

slist *sl_newcopy(slist *l) {
    /* TODO */
    return 0;
}

slist *sl_newmove(slist **l) {
    /* TODO */
    return 0;
}

void sl_delete(slist **l) {
    /* TODO */
    return;
}

iterator *sl_begin(slist *l) {
    /* TODO */
    return 0;
}

iterator *sl_before_begin(slist *l) {
    assert(l);
    iterator *it = it_new(_slist_iterator_, l);
    it_begin(it);
    return it_advance(it, -1);
}

iterator *sl_end(slist *l) {
    /* TODO */
    return 0;
}

void sl_resizefill(slist *l, size_t n, const void *valaddr) {
    /* TODO */
    return;
}

bool sl_empty(slist *l) {
    /* TODO */
    return 0;
}

size_t sl_maxsize(slist *l) {
    /* TODO */
    return 0;
}

void *sl_front(slist *l) {
    /* TODO */
    return 0;
}

const void *sl_front_const(slist *l) {
    /* TODO */
    return 0;
}

void sl_assignrnge(slist *l, iterator *first, iterator *last) {
    /* TODO */
    return;
}

void sl_assignfill(slist *l, size_t n, const void *valaddr) {
    /* TODO */
    return;
}

void sl_pushf(slist *l, const void *valaddr) {
    /* TODO */
    return;
}

void sl_popf(slist *l, const void *valaddr ){
    /* TODO */
    return;
}

iterator *sl_insert_after(slist *l, iterator *pos, const void *valaddr) {
    /* TODO */
    return 0;
}

iterator *sl_insertfill_after(slist *l, iterator *pos, size_t n, const void *valaddr) {
    /* TODO */
    return 0;
}

iterator *sl_insertrnge_after(slist *l, iterator *pos, iterator *first, iterator *last) {
    /* TODO */
    return 0;
}

iterator *sl_erase_after(slist *l, iterator *pos) {
    /* TODO */
    return 0;
}

iterator *sl_erasernge_after(slist *l, iterator *pos, iterator *last) {
    /* TODO */
    return 0;
}

void sl_swap(slist **l, slist **other) {
    /* TODO */
    return;
}

void sl_clear(slist *l) {
    /* TODO */
    return;
}

void sl_insert_at(slist *l, size_t index, const void *valaddr) {
    /* TODO */
    return;
}

void sl_erase_at(slist *l, size_t index) {
    /* TODO */
    return;
}

void sl_replace_at(slist *l, size_t index, const void *valaddr) {
    /* TODO */
    return;
}

void sl_swap_elem(slist *l, size_t n1, size_t n2) {
    /* TODO */
    return;
}

iterator *sl_splice_after(slist *l, iterator *pos, slist *other, iterator *opos) {
    /* TODO */
    return 0;
}

iterator *sl_splicelist_after(slist *l, iterator *pos, slist *other) {
    /* TODO */
    return 0;
}

iterator *sl_splicernge_after(slist *l, iterator *pos, slist *other, iterator *first, iterator *last) {
    /* TODO */
    return 0;
}

void sl_remove(slist *l, const void *valaddr) {
    /* TODO */
    return;
}

void sl_remove_if(slist *l, const void *valaddr, bool (*unary_predicate)(const void *)) {
    /* TODO */
    return;
}

void sl_unique(slist *l) {
    /* TODO */
    return;
}

slist *sl_merge(slist *l, slist *other) {
    /* TODO */
    return 0;
}

slist *sl_merge_custom(slist *l, slist *other, bool (*binary_predicate)(const void *, const void *)) {
    /* TODO */
    return 0;
}

void sl_sort(slist *l) {
    /* TODO */
    return;
}

int sl_search(slist *l, const void *valaddr) {
    /* TODO */
    return 0;
}

slist *sl_arrtol(struct typetable *ttbl, void *base, size_t n) {
    /* TODO */
    return 0;
}

void *sl_sltoarr(slist *l ) {
    /* TODO */
    return 0;
}

void sl_puts(slist *l) {
    /* TODO */
    return;
}

void sl_putsf(slist *l, const char *before, const char *after,
              const char *postelem, const char *empty, size_t breaklim) {
    /* TODO */
    return;
}

void sl_fputs(slist *l, FILE *dest) {
    /* TODO */
    return;
}

void sl_fputsf(slist *l, FILE *dest, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim) {
    /* TODO */
    return;
}

int slist_compare(const void *c1, const void *c2) {
    slist *s1 = *(slist **)(c1);
    slist *s2 = *(slist **)(c2);

    return 0;
}

void *slist_copy(const void *arg) {
    assert(arg);
    slist *l = *(slist **)(arg);

    return sl_newcopy(l);
}

void slist_print(const void *arg, FILE *dest) {
    assert(arg);
    assert(dest);

    slist *l = *(slist **)(arg);
    return sl_fputs(l, dest);
}

void slist_delete(void *arg) {
    assert(arg);

    slist **l = (slist **)(arg);
    sl_delete(l);
}

static slnode *sn_allocate(void) {
    slnode *n = NULL;
    n = malloc(sizeof *n);
    assert(n);
    return n;
}

static slnode *sn_new(const void *valaddr, size_t width) {
    slnode *n = sn_allocate();
    sn_init(n, valaddr, width);
    return n;
}

static void sn_init(slnode *n, const void *valaddr, size_t width) {
    assert(n);

    void *data = NULL;

    if (valaddr) {
        data = malloc(width);
        assert(data);
        memcpy(data, valaddr, width);
    }

    n->valaddr = data;
    n->next = NULL;
}

static void sn_deinit(slnode *n, void (*delete)(void *)) {
    assert(n);

    if (delete) {
        delete(n->valaddr);
    }

    free(n->valaddr);
    n->valaddr = NULL;
}

static void sn_delete(void *arg, void (*delete)(void *)) {
    assert(arg);

    slnode **n = (slnode **)(arg);

    if (delete) {
        sn_deinit((*n), delete);
    } else {
        sn_deinit((*n), NULL);
    }

    free((*n));
    (*n) = NULL;
}

static slnode *sl_node_sn(slist *l, int index) {
    assert(l);
    assert(index >= 0);
    return sl_traverse_h(l, index);
}

static slnode *sl_traverse_h(slist *l, int index) {
    slnode *curr = l->head;

    for (int i = 0; i < index; i++) {
        curr = curr->next;
    }

    return curr;
}

static slist *sl_allocate(void) {
    slist *l = NULL;
    l = malloc(sizeof *l);
    assert(l);
    return l;
}

static void sl_init(slist *l, struct typetable *ttbl) {
    assert(l);

    l->head = NULL;

    l->ttbl = ttbl ? ttbl : _void_ptr_;

    /* If any of the function pointers supplied are null,
     they will be replaced with the respective void_ptr functions */
    l->ttbl->compare = l->ttbl->compare ? l->ttbl->compare : NULL;
    l->ttbl->copy = l->ttbl->copy ? l->ttbl->copy : NULL;
    l->ttbl->print = l->ttbl->print ? l->ttbl->print : NULL;
    l->ttbl->delete = l->ttbl->delete ? l->ttbl->delete : NULL;
}

static void sl_deinit(slist *l) {
    assert(l);
    sl_clear(l);

    l->head = NULL;
    l->ttbl = NULL;
}

static void *sli_allocate(void) {
    void *it = NULL;
    it = malloc(sizeof(struct slist_iterator));
    assert(it);
    return it;
}

static void *sli_new(void *arg_sl) {
    void *it = sli_allocate();
    sli_init(it, arg_sl);
    return it;
}

static void sli_init(void *arg_sli, void *arg_sl) {
    assert(arg_sli);
    assert(arg_sl);

    slist_iterator *it = (slist_iterator *)(arg_sli);
    slist *l = (slist *)(arg_sl);

    it->l = l;
    it->curr = it->l->head;

    slnode *before_begin = sn_new(NULL, it->l->ttbl->width);
    before_begin->next = it->l->head;
    it->before_begin = before_begin;

    slnode *end = it->l->head;
    while (end->next != NULL) {
        end = end->next;
    }

    it->end = end;

    it->index = 0;
}

static void sli_deinit(void *arg_sli) {
    assert(arg_sli);

    slist_iterator *it = (slist_iterator *)(arg_sli);

    it->l = NULL;
    it->curr = NULL;
    it->index = -1;
}

static void sli_delete(void *arg_sli) {
    assert(arg_sli);

    slist_iterator **it = (slist_iterator **)(arg_sli);
    sli_deinit((*it));

    free((*it));
    (*it) = NULL;
}

static void *sli_advance(void *arg_sli, int n) {
    assert(arg_sli);

    slist_iterator *it = (slist_iterator *)(arg_sli);
    int size = sli_distance(NULL, it);

    if ((it->index + n < size && n + size >= -1)) {
        it->index += n;
        it->curr = (size - n) >= 0
        ? sl_node_sn(it->l, n) : it->before_begin;
    }


    return NULL;
}

static int sli_distance(void *arg_sli_first, void *arg_sli_last) {
    int last_index = arg_sli_last ? ((slist_iterator *)(arg_sli_last))->index : 0;
    int first_index = arg_sli_first ? ((slist_iterator *)(arg_sli_first))->index : 0;
    return (last_index - first_index);
}

static void *sli_begin(void *arg_sli) {
    assert(arg_sli);

    slist_iterator *it = (slist_iterator *)(arg_sli);

    it->curr = it->l->head;
    it->index = 0;

    return it;
}

static void *sli_end(void *arg_sli) {
    /* TODO */
    return 0;
}

static void *sli_next(void *arg_sli) {
    /* TODO */
    return 0;
}

static void *sli_prev(void *arg_sli) {
    /* TODO */
    return 0;
}

static bool sli_has_next(void *arg_sli) {
    /* TODO */
    return false;
}

static bool sli_has_prev(void *arg_sli) {
    /* TODO */
    return false;
}

static struct typetable *sli_get_ttbl(void *arg_sli) {
    /* TODO */
    return 0;
}
