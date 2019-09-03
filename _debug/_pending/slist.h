/**
 *  @file       slist.h
 *  @brief      Header file for a singly-linked list
 *
 *  @author     Gemuele Aludino
 *  @date       26 Jun 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#ifndef SLIST_H
#define SLIST_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct slnode slnode;
typedef struct slnode *slnode_ptr;
typedef struct slnode **slnode_dptr;

struct slnode {
    void *valaddr;
    slnode *next;
};

typedef struct slist slist;
typedef struct slist *slist_ptr;
typedef struct slist **slist_dptr;

/**< slist: constructors */
slist *sl_new(struct typetable *ttbl);
slist *sl_newfill(struct typetable *ttbl, size_t n, void *valaddr);
slist *sl_newrnge(iterator *first, iterator *last);
slist *sl_newcopy(slist *l);
slist *sl_newmove(slist **l);

/**< slist: destructor */
void sl_delete(slist **l);

/**< slist: iterator functions */
iterator *sl_begin(slist *l);
iterator *sl_before_begin(slist *l);
iterator *sl_end(slist *l);

/**< slist: resize functions */
void sl_resizefill(slist *l, size_t n, const void *valaddr);

/**< slist: capacity-based functions */
bool sl_empty(slist *l);
size_t sl_maxsize(slist *l);

/**< slist: element access functions */
void *sl_front(slist *l);

/**< slist: element access functions with const qualifier */
const void *sl_front_const(slist *l);

/**< slist: modifiers - assignment */
void sl_assignrnge(slist *l, iterator *first, iterator *last);
void sl_assignfill(slist *l, size_t n, const void *valaddr);

/**< slist: modifiers - push/pop */
void sl_pushf(slist *l, const void *valaddr);
void sl_popf(slist *l, const void *valaddr);

/**< slist: modifiers - insertion */
iterator *sl_insert_after(slist *l, iterator *pos, const void *valaddr);
iterator *sl_insertfill_after(slist *l, iterator *pos, size_t n, const void *valaddr);
iterator *sl_insertrnge_after(slist *l, iterator *pos, iterator *first, iterator *last);

/**< slist: modifiers - erasure */
iterator *sl_erase_after(slist *l, iterator *pos);
iterator *sl_erasernge_after(slist *l, iterator *pos, iterator *last);

/**< slist: modifiers - container swappage */
void sl_swap(slist **l, slist **other);

/**< slist: modifiers - clear container */
void sl_clear(slist *l);

/**< slist: custom modifiers - insertion/erasure/replacement by index */
void sl_insert_at(slist *l, size_t index, const void *valaddr);
void sl_erase_at(slist *l, size_t index);
void sl_replace_at(slist *l, size_t index, const void *valaddr);

/**< slist: custom modifiers - element swappage */
void sl_swap_elem(slist *l, size_t n1, size_t n2);

/**< slist: splice */
iterator *sl_splice_after(slist *l, iterator *pos, slist *other, iterator *opos);
iterator *sl_splicelist_after(slist *l, iterator *pos, slist *other);
iterator *sl_splicernge_after(slist *l, iterator *pos, slist *other, iterator *first, iterator *last);

/**< slist: element removal by search */
void sl_remove(slist *l, const void *valaddr);
void sl_remove_if(slist *l, const void *valaddr, bool (*unary_predicate)(const void *));

/**< slist: remove duplicates */
void sl_unique(slist *l);

/**< slist: merge/reverse */
slist *sl_merge(slist *l, slist *other);
slist *sl_merge_custom(slist *l, slist *other, bool (*binary_predicate)(const void *, const void *));

/**< slist: utility functions - search/sort by default comparator */
void sl_sort(slist *l);
int sl_search(slist *l, const void *valaddr);

/**< slist: custom utility functions - arrtol/ltoarr */
slist *sl_arrtol(struct typetable *ttbl, void *base, size_t n);
void *sl_sltoarr(slist *l);

/**< slist: custom print functions - output to FILE stream */
void sl_puts(slist *l);

void sl_putsf(slist *l, const char *before, const char *after,
              const char *postelem, const char *empty, size_t breaklim);

void sl_fputs(slist *l, FILE *dest);

void sl_fputsf(slist *l, FILE *dest, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);

/**< slist: required function prototypes for (struct typetable) */
int slist_compare(const void *c1, const void *c2);
void *slist_copy(const void *arg);
void slist_print(const void *arg, FILE *dest);
void slist_delete(void *arg);

/**< ptrs to vtables */
extern struct typetable *_slist_;
extern struct iterator_table *_slist_iterator_;

#endif /* SLIST_H */
