/**
 *  @file       list.c
 *  @brief      Source file for a doubly-linked list ADT
 *
 *  @author     Gemuele Aludino
 *  @date       25 Jun 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#include <assert.h>
#include <math.h>
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

/**
 *  @file       list.h
 *  @brief      Public header file for access to list-related functions
 */
#include "list.h"

/**
 *  @def        LIST_MAXIMUM_STACK_BUFFER_SIZE
 *  @brief      Arbitrary maximum size for a statically allocated (char) buffer
 */
#define LIST_MAXIMUM_STACK_BUFFER_SIZE 16384

/**
 *  @file       mergesort.h
 *  @brief      Routines for mergesort on linear node-based ADTs
 */
#include "mergesort.h"

/**< dlnode: allocation/initialization */
static dlnode *dn_allocate(void);
static dlnode *dn_new(const void *valaddr, size_t width);
static void dn_init(dlnode *n, const void *valaddr, size_t width);
static void dl_deinit(dlnode *n, void (*delete)(void *));
static void dn_delete(void *arg, void (*delete)(void *));

struct list {
    dlnode *head;
    dlnode *tail;

    size_t length;

    struct typetable *ttbl;
};

/**< list: node access */
// static  dlnode *    l_head_dl       (list *l);
// static  dlnode *    l_tail_dl       (list *l);
static dlnode *l_node_dn(list *l, int index);

/**< list: node traversal */
static dlnode *l_traverse_h(list *l, int index);
static dlnode *l_traverse_t(list *l, int index);

/**< list: allocation/initialization */
static list *l_allocate(void);
static void l_init(list *l, struct typetable *ttbl);
static void l_deinit(list *l);

struct typetable ttbl_list = {sizeof(struct list), list_compare, list_copy,
                                   list_print, list_delete};

struct typetable *_list_ = &ttbl_list;

typedef struct list_iterator list_iterator;
typedef struct list_iterator *list_iterator_ptr;
typedef struct list_iterator **list_iterator_dptr;

struct list_iterator {
    list *l;
    int index;
    dlnode *curr;
};

/**< list_iterator: create/initialize/destroy */
static void *li_allocate(void);
static void *li_new(void *arg_l);
static void li_init(void *arg_li, void *arg_l);
static void li_deinit(void *arg_li);
static void li_delete(void *arg_li);

/**< list_iterator: index based operations */
static void *li_advance(void *arg_li, int n);
static int li_distance(void *arg_li_first, void *arg_li_last);

/**< list_iterator: return iterator to begin/end */
static void *li_begin(void *arg_li);
static void *li_end(void *arg_li);

/**< list_iterator: advance/retract iterator position */
static void *li_next(void *arg_li);
static void *li_prev(void *arg_li);

/**< list_iterator: determine if there are more elements to traverse */
static bool li_has_next(void *arg_li);
static bool li_has_prev(void *arg_li);

/**< list_iterator: retrieve (typetable *) */
static struct typetable *li_get_ttbl(void *arg_li);

/**
 *  iterator_table itbl_list
 *  @brief      Instance of struct iterator_table used by iterator
 *              (see iterator.h)
 */
struct iterator_table itbl_list = {
    li_new,  li_delete, li_advance,  li_distance, li_begin,   li_end,
    li_next, li_prev,   li_has_next, li_has_prev, li_get_ttbl};

struct iterator_table *_list_iterator_ = &itbl_list;

list *l_new(struct typetable *ttbl) {
    list *l = l_allocate();
    l_init(l, ttbl);
    return l;
}

list *l_newfill(struct typetable *ttbl, size_t n, void *valaddr) {
    list *l = l_new(ttbl);

    for (size_t i = 0; i < n; i++) {
        l_pushb(l, valaddr);
    }

    return l;
}

list *l_newrnge(iterator *first, iterator *last) {
    assert(first);
    assert(last);

    int start = it_distance(NULL, first);
    int end = it_distance(NULL, last);
    int delta = end - start;

    list *l = l_new(it_get_ttbl(first));

    for (int i = 0; i < delta; i++) {
        void *target = it_next(first);
        l_pushb(l, target);
    }

    return l;
}

list *l_newcopy(list *l) {
    assert(l);
    list *copy = l_new(l->ttbl);

    dlnode *curr = l->head;

    bool has_copy_fn = l->ttbl->copy != NULL;
    void *src = NULL;

    if (has_copy_fn) {
        for (size_t i = 0; i < l->length; i++) {
            src = curr->valaddr;
            void *target = l->ttbl->copy(src);
            l_pushb(copy, &target);
            curr = curr->next;
        }
    } else {
        for (size_t i = 0; i < l->length; i++) {
            void *target = curr->valaddr;
            l_pushb(copy, &target);
            curr = curr->next;
        }
    }

    return copy;
}

list *l_newmove(list **l) {
    assert(l);

    list *move = l_allocate();

    move->head = (*l)->head;
    move->tail = (*l)->tail;
    move->length = (*l)->length;
    move->ttbl = (*l)->ttbl;

    l_init((*l), (*l)->ttbl);

    return move;
}

void l_delete(list **l) {
    assert(l);

    l_deinit((*l));

    free((*l));
    (*l) = NULL;
}

iterator *l_begin(list *l) {
    assert(l);

    iterator *it = it_new(_list_iterator_, l);
    return it_begin(it);
}

iterator *l_end(list *l) {
    assert(l);

    iterator *it = it_new(_list_iterator_, l);
    return it_end(it);
}

size_t l_size(list *l) {
    assert(l);
    return l->length;
}

size_t l_maxsize(list *l) {
    assert(l);

    if (sizeof(void *) == 8) {
        // 64-bit system
        return (pow(2.0, 64.0) / (l->ttbl->width));
    } else {
        // 32-bit system
        return (pow(2.0, 32.0) / (l->ttbl->width));
    }
}

void l_resizefill(list *l, size_t n, const void *valaddr) {
    assert(l);

    for (int i = 0; i < n; i++) {
        l_pushb(l, valaddr);
    }
}

bool l_empty(list *l) {
    assert(l);
    return l->length == 0;
}

void *l_front(list *l) {
    assert(l);
    return l->head ? l->head->valaddr : NULL;
}

void *l_back(list *l) {
    assert(l);
    return l->tail ? l->tail->valaddr : NULL;
}

const void *l_front_const(list *l) {
    assert(l);
    return l->head ? l->head->valaddr : NULL;
}

const void *l_back_const(list *l) {
    assert(l);
    return l->tail ? l->tail->valaddr : NULL;
}

void l_assignrnge(list *l, iterator *first, iterator *last) {
    assert(l);
    assert(first);
    assert(last);

    // setting one argument NULL for it_distance will retrieve
    // the position index for the non-null argument
    int start = it_distance(NULL, first);
    int end = it_distance(NULL, last);

    int delta = end - start;
    size_t old_length = l->length;

    if (delta > 0) {
        for (int i = 0; i < delta; i++) {
            void *target = it_next(first);

            if (i < old_length) {
                /**
                 *  The range of elements specified by [start, end)
                 *  will overwrite existing elements starting at index 0.
                 */
                l_replace_at(l, i, target);
            } else {
                /**
                 *  If the range of elements specified by [start, end)
                 *  exceeds the current logical length of the internal buffer,
                 *  then elements will be appended to the rear of the buffer.
                 */
                l_pushb(l, target);
            }
        }
    } else {
        WARNING(__FILE__, "l_assignrnge returned early -- iterator last must have a greater position index than that of iterator first");
        return;
    }

    /* Restoring the index of first */
    it_advance(first, start - (start + delta));
}

void l_assignfill(list *l, size_t n, const void *valaddr) {
    assert(l);
    assert(valaddr);

    for (int i = 0; i < n; i++) {
        l_popf(l);
        l_pushf(l, valaddr);
    }
}

void l_pushf(list *l, const void *valaddr) {
    assert(l);
    assert(valaddr);

    dlnode *new_node = dn_new(valaddr, l->ttbl->width);

    if (l->head == NULL) {
        l->tail = new_node;
    } else {
        new_node->next = l->head;
        l->head->prev = new_node;
    }

    l->head = new_node;
    ++l->length;
}

void l_popf(list *l) {
    assert(l);

    if (l->head == NULL) {
        return;
    } else if (l->length == 1) {
        dn_delete(&l->head, l->ttbl->delete);
        --l->length;
        l->head = NULL;
        l->tail = NULL;
        return;
    }

    dlnode *old_node = l->head;

    if (l->head != l->tail) {
        l->head = l->head->next;
    }

    dn_delete(&old_node, l->ttbl->delete);
    --l->length;
}

void l_pushb(list *l, const void *valaddr) {
    assert(l);
    assert(valaddr);

    dlnode *new_node = dn_new(valaddr, l->ttbl->width);

    if (l->tail == NULL) {
        l->head = new_node;
    } else {
        new_node->prev = l->tail;
        l->tail->next = new_node;
    }

    l->tail = new_node;
    ++l->length;
}

void l_popb(list *l) {
    assert(l);

    if (l->head == NULL) {
        return;
    } else if (l->length == 1) {
        dn_delete(&l->head, l->ttbl->delete);
        --l->length;
        l->head = NULL;
        l->tail = NULL;
        return;
    }

    dlnode *old_node = l->tail;

    if (l->tail != l->head) {
        l->tail = l->tail->prev;
        l->tail->next = NULL;
    }

    dn_delete(&old_node, l->ttbl->delete);
    --l->length;
}

iterator *l_insert(list *l, iterator *pos, const void *valaddr) {
    assert(l);
    assert(pos);
    assert(valaddr);
    // why not just splice valaddr in at position pos within l?
    l_pushb(l, valaddr);

    const size_t back_index = l->length - 1;

    int i = it_distance(NULL, pos);

    while (i < back_index) {
        l_swap_elem(l, i++, back_index);
    }

    return pos;
}

iterator *l_insertfill(list *l, iterator *pos, size_t n, const void *valaddr) {
    assert(l);
    assert(pos);
    assert(valaddr);
    // why not just make a new list *tmp, size n, of valaddr,
    // and splice tmp in at position pos within l?
    for (int i = 0; i < n; i++) {
        l_insert(l, pos, valaddr);
    }

    return pos;
}

iterator *l_insertrnge(list *l, iterator *pos, iterator *first,
                       iterator *last) {
    assert(l);
    assert(pos);
    assert(first);
    assert(last);
    // why not just make a new list *tmp using iterator first and iterator last,
    // and splice it in at position pos within l?
    int curr = it_distance(NULL, pos);
    int position = curr;

    int start = it_distance(NULL, first);
    int end = it_distance(NULL, last);
    int delta = end - start;

    if (delta > 0) {
        for (int i = 0; i < delta; i++) {
            void *target = it_next(first);
            l_insert_at(l, curr++, target);
        }
    } else {
        return NULL;
    }

    it_advance(first, start - (start + delta));
    it_advance(pos, position - (position + delta));

    return pos;
}

iterator *l_erase(list *l, iterator *pos) {
    assert(l);
    assert(pos);

    int curr = it_distance(NULL, pos);
    const size_t back_index = l->length - 1;

    if (curr < 0) {
        return NULL;
    } else if (curr == l->length - 1) {
        l_popb(l);
        return pos;
    } else if (curr >= l->length) {
        return NULL;
    }

    if (curr < back_index && curr >= 0) {
        for (int i = curr; i < back_index; i++) {
            l_swap_elem(l, i, i + 1);
        }
    }

    l_popb(l);
    return pos;
}

iterator *l_erasernge(list *l, iterator *pos, iterator *last) {
    assert(l);
    assert(pos);
    assert(last);

    // calculate the delta, or distance between pos and last
    int pos_index = it_distance(NULL, pos);
    int last_index = it_distance(NULL, last);
    int delta = last_index - pos_index;

    // retrieve the node behind the beginning of the erasure range
    dlnode *prev_hook = l_node_dn(l, pos_index - 1);
    dlnode *to_delete = prev_hook->next;

    // retrieve the node in front of the last node of the erasure range
    dlnode *next_hook = to_delete;
    for (int i = 0; i < delta; i++) {
        next_hook = next_hook->next;
    }

    // link in prev_hook and next_hook
    prev_hook->next = next_hook;
    next_hook->prev = prev_hook;

    // delete the old nodes
    dlnode *next = NULL;
    while (to_delete != next_hook) {
        next = to_delete->next;

        if (l->ttbl->delete) {
            dn_delete(&to_delete, l->ttbl->delete);
        } else {
            dn_delete(&to_delete, NULL);
        }

        to_delete = next;
    }

    // update the length variable
    l->length = l->length - delta;
    return pos;
}

void l_swap(list **l, list **other) {
    assert(*l);
    assert(*other);

    list *temp = *l;
    *l = *other;
    *other = temp;
}

void l_clear(list *l) {
    while (l->length > 0) {
        l_popb(l);
    }
}

void l_insert_at(list *l, size_t index, const void *valaddr) {
    assert(l);
    assert(valaddr);

    l_pushb(l, valaddr);
    size_t back_index = l->length - 1;

    if (index >= back_index) {
        return;
    }

    for (size_t i = index; i <= back_index; ++i) {
        l_swap_elem(l, i, back_index);
    }
}

void l_erase_at(list *l, size_t index) {
    assert(l);

    if (index < 0) {
        return;
    } else if (index == 0) {
        l_popf(l);
        return;
    } else if (index == l->length - 1) {
        l_popb(l);
        return;
    } else if (index >= l->length) {
        return;
    }

    dlnode *target = l_node_dn(l, (int)(index));

    dlnode *target_prev = target->prev;
    dlnode *target_next = target->next;

    target_prev->next = target_next;
    target_next->prev = target_prev;

    dn_delete(&target, l->ttbl->delete);
    --l->length;
}

void l_replace_at(list *l, size_t index, const void *valaddr) {
    assert(l);
    assert(valaddr);

    l_pushb(l, valaddr);

    if (index >= l->length) {
        return;
    }

    const size_t back_index = l->length - 1;

    l_swap_elem(l, back_index, index);
    l_popb(l);
}

void l_swap_elem(list *l, size_t n1, size_t n2) {
    assert(l);

    bool n1_bad = n1 >= l->length;
    bool n2_bad = n2 >= l->length;

    bool good_indices = !n1_bad && !n2_bad;

    if (good_indices && l->length > 0) {
        dlnode *node_n1 = NULL;
        dlnode *node_n2 = NULL;

        int difference = n2 - n1;

        if (difference >= 1) {
            if (difference == 1) {
                node_n1 = l_node_dn(l, (int)(n1));
                node_n2 = node_n1->next;
            } else {
                // n2 is ahead of n1. get n1 first, then use n1 to get n2
                node_n1 = l_node_dn(l, (int)(n1));

                dlnode *temp = node_n1;
                for (int i = 3; i > 0; i--) {
                    temp = temp->next;
                }

                node_n2 = temp;
            }
        } else if (difference < 1) {
            if (difference == -1) {
                node_n2 = l_node_dn(l, (int)(n2));
                node_n1 = node_n2->next;
            } else {
                // n1 is ahead of n2. get n2 first, then use n2 to get n1
                node_n2 = l_node_dn(l, (int)(n2));

                dlnode *temp = node_n2;
                for (int i = 3; i > 0; i--) {
                    temp = temp->next;
                }

                node_n1 = temp;
            }
        }

        void *temp = node_n1->valaddr;

        node_n1->valaddr = node_n2->valaddr;
        node_n2->valaddr = temp;
    } else {
        return;
    }
}

iterator *l_splice(list *l, iterator *pos, list *other, iterator *opos) {
    assert(l);
    assert(pos);
    assert(other);
    assert(opos);

    struct iterator_table *itbl_pos = it_get_itbl(pos);

    if (itbl_pos->new != li_new) {
        ERROR(__FILE__, "pos must be created from a list container instance.");
        return NULL;
    }

    struct iterator_table *itbl_opos = it_get_itbl(pos);

    if (itbl_opos->new != li_new) {
        ERROR(__FILE__, "opos must be created from a list container instance.");
        return NULL;
    }

    list_iterator *lpos = (list_iterator *)(*(void **)(pos));
    list_iterator *lopos = (list_iterator *)(*(void **)(opos));

    if (l == other) {
        WARNING(__FILE__,
                "Undefined behavior if l == other:\tAddress of l: %p\tAddress "
                "of other: %p",
                l, other);
    }

    if (other->length == 0) {
        return pos;
    }

    if (lpos->index == l->length) {
        WARNING(__FILE__,
                "Undefined behavior if pos->iter->index == l->length");
    }

    if (lopos->index == other->length) {
        WARNING(__FILE__,
                "Undefined behavior if opos->iter->index == other->length");
    }

    /**
     *  position is one node behind lpos->curr (node of the intended index)
     */
    dlnode *position = lpos->index > 0 ? lpos->curr->prev : lpos->curr;
    dlnode *temp_next = lpos->l->length > 1 ? position->next : NULL;

    dlnode *head = l->head;
    dlnode *tail = l->tail;
    // dlnode *tail_prev = l->length > 1 ? tail->prev : NULL;

    dlnode *ohead = other->head;
    dlnode *otail = other->tail;
    // dlnode *otail_prev = other->length > 1 ? otail->prev : NULL;

    /**
     *  The node from list other to splice into list l
     */
    dlnode *src = lopos->curr;

    dlnode *temp_src_prev = lopos->index > 0 ? src->prev : NULL;

    dlnode *temp_src_next =
        other->length > 1 && lopos->index < other->length ? src->next : NULL;

    if (lpos->index == 0) {
        /**
         *  If list l's iterator is at index 0,
         *  we append node src to the front of list l
         *  and it becomes its new head.
         */

        src->next = head;
        head->prev = src;
        head = src;

        l->head = head;
    } else if (lpos->index == l->length - 1) {
        /**
         *  If list l's iterator is at index l->length - 1,
         *  we append node src to the back of list l
         *  and it becomes its new tail.
         */

        tail->next = src;
        src->prev = tail;
        tail = src;

        l->tail = tail;
    } else if (l->length == 0) {
        /**
         *  If list l is an empty list,
         *  src becomes its head and tail.
         */
        head = src;
        tail = src;

        l->head = head;
        l->tail = tail;
    } else {
        /**
         *  In any other case, we splice src into the middle of list l.
         *  No changes to the head/tail of list l need to be made.
         */

        position->next = src;
        src->prev = position;

        src->next = temp_next;
        temp_next->prev = src;
    }

    if (lopos->index == 0) {
        /**
         *  If src is the head of list other...
         */
        if (temp_src_next != NULL) {
            temp_src_next->prev = NULL;
            ohead = temp_src_next;

            other->head = ohead;
        }
    } else if (lopos->index == other->length - 1) {
        /**
         *  If src is the tail of list other...
         */
        if (temp_src_prev != NULL) {
            temp_src_prev->next = NULL;
            otail = temp_src_prev;

            other->tail = tail;
        }
    } else {
        temp_src_prev->next = temp_src_next;
        temp_src_next->prev = temp_src_prev;
    }

    /**
     *  Decrementing list other's length value,
     *  Incrementing list l's length value.
     */
    --other->length;
    ++l->length;

    return pos;
}

iterator *l_splicelist(list *l, iterator *pos, list *other) {
    assert(l);
    assert(pos);
    assert(other);

    struct iterator_table *itbl = it_get_itbl(pos);

    if (itbl->new != li_new) {
        ERROR(__FILE__, "pos must be created from a list container instance.");
        return NULL;
    }

    list_iterator *lpos = (list_iterator *)(*(void **)(pos));

    /**
     *  If list other is empty, there is no work to do here.
     */
    if (other->length == 0) {
        return pos;
    }

    /**
     *  Represents LIST L1 (list l)
     */
    dlnode *position = lpos->index > 0 ? lpos->curr->prev : lpos->curr;
    dlnode *temp_next = lpos->l->length > 1 ? position->next : NULL;

    /**
     *  Represents LIST L2 (list other)
     */
    dlnode *ohead = other->head;
    dlnode *otail = other->tail;

    /**
     *  The following is an example of how l_splicelist will work.
     *
     *  int arr[2][4] = { { 1, 2, 3, 4 }, { 6, 7, 8, 9 } };
     *
     *  list *l1 = l_arrtol(_int_, arr[0], 4);
     *  list *l2 = l_arrtol(_int_, arr[1], 4);
     *
     *  iterator *it1 = it_advance(l_begin(l1), 1);
     *  l_splicelist(l1, it1, l2);
     *
     *  // l1 is { 1, 6, 7, 8, 9, 2, 3, 4, 5 }
     *  // l2 is {   }
     *
     ======== BEFORE: ========

     LIST L1: </1->---<-2->---<-3->---<-4/>
     (l1->head) ^       ^               ^ (l1->tail)
     (lpos->index)
     (lpos->curr) (dlnode *) - iterator's position node

     LIST L2: </6->---<-7->---<-8->---<-9/>
     (ohead) ^                       ^ (otail)

     ======== AFTER: ========

     LIST L1: </1->---<-6->---<-7->---<-8->---<-9->---<-2->---<-3->---<-4/>
     (l1->head)  ^                                       ^               ^
     (lpos->index)    (l1->tail)
     (lpos->curr) (dlnode *)

     LIST L2: (empty list)

     *
     */

    if (lpos->index == l->length) {
        WARNING(__FILE__,
                "Undefined behavior if pos->iter->index == l->length");
    }

    if (lpos->index == 0) {
        /**
         *  list other's tail node gets linked to list l's head node
         *  effectively resulting in a concatentation of lists.
         *
         *  list l's head node is reassigned to list other's head node.
         */
        otail->next = position;
        position->prev = otail;

        l->head = ohead;
    } else if (lpos->index == l->length - 1) {
        /**
         *  list l's tail node gets linked to list l's head node
         *  effectively resulting in a concatenation of lists.
         *
         *  list l's tail node is reassigned to list other's tail node.
         */
        temp_next->next = ohead;
        ohead->prev = temp_next;

        l->tail = otail;
    } else if (l->length == 0) {
        /*
         *  list l is an empty list, but list other is not.
         *  We assign list l's head node to list other's head node,
         *  and list l's tail node to list other's tail node.
         */

        l->head = ohead;
        l->tail = otail;
    } else {
        /**
         *  This case represents a true "splice" in the middle of the list.
         *
         *  list l is split at node lpos->curr and list other's head node
         *  is inserted where lpos->curr used to be.
         *
         *  list l's tail node is safely reassigned by using a temporary node
         *  to linearly traverse list l until a null pointer is found.
         */
        ohead->prev = position;
        otail->next = temp_next;

        position->next = ohead;
        temp_next->prev = otail;
    }

    /**
     *  The following statements are common to all cases.
     */

    /**
     *  Increment list l's length counter value by other's length counter value.
     */
    l->length += other->length;
    lpos->index += other->length;

    /**
     *  list other is now an empty container.
     */
    other->length = 0;
    other->head = NULL;
    other->tail = NULL;

    return pos;
}

iterator *l_splicernge(list *l, iterator *pos, list *other, iterator *first,
                       iterator *last) {
    assert(l);
    assert(pos);
    assert(other);
    assert(first);
    assert(last);

    struct iterator_table *itbl_pos = it_get_itbl(pos);

    if (itbl_pos->new != li_new) {
        ERROR(__FILE__, "pos must be created from a list container instance.");
    }

    struct iterator_table *itbl_first = it_get_itbl(first);

    if (itbl_first->new != li_new) {
        ERROR(__FILE__, "first must be created from a list container instance.");
    }

    struct iterator_table *itbl_last = it_get_itbl(last);

    if (itbl_last->new != li_new) {
        ERROR(__FILE__, "last must be created from a list container instance.");
        return NULL;
    }

    /**
     *  Position within list l
     */
    // list_iterator *lpos = (list_iterator *)(*(void **)(pos));

    /**
     *  Bounds within list other (basis for the sublist to splice)
     */
    list_iterator *lfirst = (list_iterator *)(*(void **)(first));
    list_iterator *llast = (list_iterator *)(*(void **)(last));

    /**
     *  If list other is empty,
     *  or lfirst->l does not point llast->l
     *  there is no work to do here.
     */
    if (other->length == 0 || (lfirst->l != llast->l) || (other != lfirst->l) ||
        (other != llast->l)) {
        return pos;
    }

    if (lfirst->index == 0 && llast->index == other->length) {
        WARNING(__FILE__,
                "Undefined behavior if range is [first, last)\tIndex range: "
                "[%d, %d)\tLength of list other: %d",
                lfirst->index, llast->index, other->length);
    }

    if (lfirst->index == 0 && llast->index == other->length - 1) {
        return l_splicelist(l, pos, other);
    }

    /**
     *  The term "sublist" refers to the range of nodes from list other
     *  determined by list_iterator(s) lfirst and llast that will be spliced
     *  into list l, at index pos->iter->index.
     *
     *  The head and tail of the sublist, respectively, are
     *  lfirst->curr and llast->curr->prev.
     *
     *  Range: [lfirst->curr, llast->curr) -> [lfirst->curr, llast->curr->prev]
     *  aka: [lfirst->index, llast->index) -> [lfirst->index, llast->index - 1]
     */

    /**
     *  Head and tail of sublist (as dictated by iterators first and last)
     */
    dlnode *ohead = lfirst->curr;
    dlnode *otail = llast->curr->prev;

    /**
     *  sublist_length is the difference between the iterator indices plus one.
     */
    int sublist_length = llast->index - lfirst->index;

    if (lfirst->index == 0) {
        /**
         *  Sublist's head is the head node of it's host list - list other
         *  Extra care must be taken to ensure non sublist nodes are linked.
         */

        other->head = otail->next;
        other->head->prev = NULL;
    } else if (llast->index == other->length) {
        /**
         *  Sublist's tail is the head node of it's host list - list other
         *  Extra care must be taken to ensure non sublist nodes are linked.
         */

        otail = llast->curr;

        other->tail = ohead->prev;
        other->tail->next = NULL;
    } else {
        /**
         *  Sublist occurs in the middle of list other.
         */

        /**
         *  temp_first_prev is to the left of ohead
         *  temp_last_next is to the right of otail
         */
        dlnode *temp_first_prev = lfirst->curr->prev;
        dlnode *temp_last_next = llast->l->length > 1 ? llast->curr : NULL;

        /**
         *  The remainder of list other is rejoined at the split point
         *  where the sublist was extracted. The length value of list other
         *  is also readjusted accordingly.
         */
        temp_first_prev->next = temp_last_next;

        if (temp_last_next != NULL) {
            temp_last_next->prev = temp_first_prev;
        }
    }

    /**
     *  The following statements are common to all cases.
     */
    other->length -= sublist_length;

    /**
     *  Isolating the front of the sublist from the remainder of list other
     */
    ohead->prev = NULL;

    /**
     *  Isolating the rear of the sublist from the remainder of list other
     */
    otail->next = NULL;

    /**
     *  Since (struct list) is defined in this compilation unit,
     *  we can create an instance of it on the stack.
     *
     *  All we need is a temporary container for ohead and otail,
     *  the head and tail of our sublist, so that we can reuse the subroutine
     *  l_splicelist to do the splice work.
     *
     *  By the time this function returns, and the list sublist instance
     *  is popped off of the stack, nodes ohead and otail have already
     *  been "spliced" in, and thus we do not lose them.
     */
    list sublist = {ohead, otail, sublist_length, other->ttbl};
    l_splicelist(l, pos, &sublist);

    return pos;
}

void l_remove(list *l, const void *valaddr) {
    assert(l);
    assert(valaddr);
    int found = l_search(l, valaddr);

    while (found != -1) {
        l_erase_at(l, found);
        found = l_search(l, valaddr);
    }
}

void l_remove_if(list *l, const void *valaddr,
                 bool (*unary_predicate)(const void *)) {
    assert(l);
    assert(valaddr);
    assert(unary_predicate);

    if (unary_predicate(valaddr)) {
        l_remove(l, valaddr);
    }
}

void l_unique(list *l) {
    l_sort(l);

    dlnode *curr = l->head;
    int i = 0;
    while (curr != NULL) {
        void *first = curr->valaddr;
        void *second = NULL;

        if (curr->next != NULL) {
            second = curr->next->valaddr;
        }

        if (second != NULL) {
            if (l->ttbl->compare(first, second) == 0) {
                l_erase_at(l, i);
                --i;
            }
        }

        curr = curr->next;
        ++i;
    }
}

list *l_merge(list *l, list *other) {
    assert(l);
    assert(other);

    if (l == other || l->ttbl->compare != other->ttbl->compare) {
        return l;
    }

    dlnode *curr = other->head;
    while (curr != NULL) {
        void *valaddr = curr->valaddr;

        l_pushb(l, valaddr);
        curr = curr->next;
    }

    l_sort(l);
    l_clear(other);

    return l;
}

list *l_merge_custom(list *l, list *other,
                     bool (*binary_predicate)(const void *, const void *)) {
    assert(l);
    assert(other);
    assert(binary_predicate);

    if (l == other) {
        return l;
    }

    dlnode *curr = other->head;
    while (curr != NULL) {
        void *valaddr = curr->valaddr;

        l_pushb(l, valaddr);
        curr = curr->next;
    }

    binary_predicate_fn temp = (binary_predicate_fn)(l->ttbl->compare);
    l->ttbl->compare = (compare_fn)(binary_predicate);

    l_sort(l);
    l->ttbl->compare = (compare_fn)(temp);

    l_clear(other);

    return l;
}

void l_reverse(list *l) {
    assert(l);

    const size_t half_length = l->length / 2;
    const size_t back_index = l->length - 1;

    for (size_t i = 0; i < half_length; i++) {
        l_swap_elem(l, i, back_index - 1);
    }
}

void l_sort(list *l) {
    assert(l);

    if (l->length < 2) {
        return;
    }

    // dl_mergesort_iterative(&l->head, l->ttbl->compare);
    dl_mergesort_recursive(&l->head, &l->tail, l->ttbl->compare);

    /*
     *  Hotfix to update tail pointer.
     *  Need a better way to keep track of this.
     */
    while (l->tail->next != NULL) {
        l->tail = l->tail->next;
    }
}

int l_search(list *l, const void *valaddr) {
    assert(l);
    assert(valaddr);

    int result = -1;

    if (l->ttbl->compare != NULL) {
        dlnode *curr = l->head;

        for (size_t i = 0; i < l->length; i++) {
            bool found = l->ttbl->compare(valaddr, curr->valaddr) == 0;

            if (found) {
                result = (int)(i);
                break;
            }

            curr = curr->next;
        }
    }

    return result;
}

list *l_arrtol(struct typetable *ttbl, void *base, size_t n) {
    assert(ttbl);
    assert(base);

    list *l = l_new(ttbl);

    for (int i = 0; i < n; i++) {
        void *target = ADDR_AT(base, i, ttbl->width);
        l_pushb(l, target);
    }

    return l;
}

void *l_ltoarr(list *l) {
    assert(l);

    void *result = NULL;
    result = calloc(l->length, l->ttbl->width);
    assert(result);

    int i = 0;
    dlnode *curr = l->head;

    while (curr != NULL) {
        void *elem = curr->valaddr;
        void *target = ((char *)(result) + (l->ttbl->width * i++));

        memcpy(target, elem, l->ttbl->width);
        curr = curr->next;
    }

    return result;
}

void l_puts(list *l) { l_fputs(l, stdout); }

void l_putsf(list *l, const char *before, const char *after,
             const char *postelem, const char *empty, size_t breaklim) {
    l_fputsf(l, stdout, before, after, postelem, empty, breaklim);
}

void l_fputs(list *l, FILE *dest) {
    assert(l);
    assert(dest);

    char buffer1[MAXIMUM_STACK_BUFFER_SIZE];
    char buffer2[MAXIMUM_STACK_BUFFER_SIZE];

    const char *link = "-------------------------";

    sprintf(buffer1, "\n%s\n%s\n%s\n", link, "Elements", link);

    const char *bytes_label = l->ttbl->width == 1 ? "byte" : "bytes";

    sprintf(buffer2, "%s\n%s\t\t%lu\n%s\t%lu %s\n%s\n", link, "Size", l->length,
            "Element size", l->ttbl->width, bytes_label, link);

    const char *postelem = "";
    const char *empty = "--- Container is empty ---";
    const size_t breaklim = 1;

    l_fputsf(l, dest, buffer1, buffer2, postelem, empty, breaklim);
}

void l_fputsf(list *l, FILE *dest, const char *before, const char *after,
              const char *postelem, const char *empty, size_t breaklim) {
    assert(l);

    fprintf(dest, "%s", before ? before : "");

    if (l->length == 0) {
        fprintf(dest, "%s\n", empty);
    } else {
        dlnode *n = l->head;

        for (size_t i = 0, curr = 1; i < l->length; i++, curr++) {
            void *target = n->valaddr;
            l->ttbl->print(target, dest);

            if (i < l->length - 1) {
                fprintf(dest, "%s", postelem ? postelem : "");
            }

            if (curr == breaklim) {
                curr = 0;
                fprintf(dest, "\n");
            }

            n = n->next;
        }
    }

    fprintf(dest, "%s", after ? after : "");
}

int list_compare(const void *c1, const void *c2) {
    assert(c1);
    assert(c2);

    list *l1 = *(list **)(c1);
    list *l2 = *(list **)(c2);

    if (l1->ttbl->compare != l2->ttbl->compare) {
        return -1;
    }

    l_sort(l1);
    l_sort(l2);

    size_t size = l1->length < l2->length ? l1->length : l2->length;

    int delta = 0;

    for (int i = 0; i < size; i++) {
        dlnode *n1 = l_node_dn(l1, i);
        dlnode *n2 = l_node_dn(l2, i);

        void *target1 = n1->valaddr;
        void *target2 = n2->valaddr;

        delta += l1->ttbl->compare(&target1, &target2);
    }

    return delta;
}

void *list_copy(const void *arg) {
    assert(arg);

    list *l = *(list **)(arg);
    return l_newcopy(l);
}

void list_print(const void *arg, FILE *dest) {
    assert(arg);
    assert(dest);

    list *l = *(list **)(arg);
    l_fputs(l, dest);
}

void list_delete(void *arg) {
    assert(arg);

    list **l = (list **)(arg);
    l_delete(l);
}

static dlnode *dn_allocate(void) {
    dlnode *n = NULL;
    n = malloc(sizeof(struct dlnode));
    assert(n);
    return n;
}

static dlnode *dn_new(const void *valaddr, size_t width) {
    dlnode *n = dn_allocate();
    dn_init(n, valaddr, width);
    return n;
}

static void dn_init(dlnode *n, const void *valaddr, size_t width) {
    assert(n);
    assert(valaddr);
    assert(width > 0);

    void *data = NULL;
    data = malloc(width);
    assert(data);

    memcpy(data, valaddr, width);
    n->valaddr = data;

    n->prev = NULL;
    n->next = NULL;
}

static void dl_deinit(dlnode *n, void (*delete)(void *)) {
    assert(n);

    if (delete != NULL) {
        delete (n->valaddr);
    }

    n->prev = NULL;
    n->next = NULL;

    free(n->valaddr);
    n->valaddr = NULL;
}

static void dn_delete(void *arg, void (*delete)(void *)) {
    assert(arg);

    dlnode **n = (dlnode **)(arg);
    dl_deinit((*n), delete);

    free((*n));
    (*n) = NULL;
}

/*
 static dlnode *l_head_dl(list *l) {
 assert(l);
 return l->head ? l->head : NULL;
 }

 static dlnode *l_tail_dl(list *l) {
 assert(l);
 return l->tail ? l->tail : NULL;
 }
 */

static list *l_allocate() {
    list *l = NULL;
    l = malloc(sizeof(struct list));
    assert(l);
    return l;
}

static void l_init(list *l, struct typetable *ttbl) {
    assert(l);

    l->head = NULL;
    l->tail = NULL;

    l->length = 0;

    l->ttbl = ttbl ? ttbl : _void_ptr_;

    /* If any of the function pointers supplied are null,
     they will be replaced with the respective void_ptr functions */
    l->ttbl->compare = l->ttbl->compare ? l->ttbl->compare : NULL;
    l->ttbl->copy = l->ttbl->copy ? l->ttbl->copy : NULL;
    l->ttbl->print = l->ttbl->print ? l->ttbl->print : NULL;
    l->ttbl->delete = l->ttbl->delete ? l->ttbl->delete : NULL;
}

static void l_deinit(list *l) {
    if (l != NULL) {
        l_clear(l);
    }

    l->head = NULL;
    l->tail = NULL;

    l->length = 0;

    l->ttbl = NULL;
}

static dlnode *l_node_dn(list *l, int index) {
    assert(l);

    dlnode *n = (index <= l->length / 2) ? l_traverse_h(l, index)
                                          : l_traverse_t(l, index);

    return n;
}

static dlnode *l_traverse_h(list *l, int index) {
    assert(l);
    assert(l->head);

    dlnode *n = l->head;

    const int back_index = (int)(l->length - 1);

    int i = 0;
    while (n != NULL || i != back_index) {
        if (i == index) {
            break;
        }

        n = n->next;
        ++i;
    }

    return n;
}

static dlnode *l_traverse_t(list *l, int index) {
    assert(l);
    assert(l->tail);

    dlnode *n = l->tail;

    int over_front_index = -1;

    int i = (int)(l->length - 1);
    while (n != NULL | i != over_front_index) {
        if (i == index) {
            break;
        }

        n = n->prev;
        --i;
    }

    return n;
}

static void *li_allocate(void) {
    void *it = NULL;
    it = malloc(sizeof(struct list_iterator));
    assert(it);
    return it;
}

static void *li_new(void *arg_l) {
    void *it = li_allocate();
    li_init(it, arg_l);
    return it;
}

static void li_init(void *arg_li, void *arg_l) {
    assert(arg_li);
    assert(arg_l);

    list_iterator *it = (list_iterator *)(arg_li);
    list *l = (list *)(arg_l);

    it->l = l;
    it->curr = it->l->head;
    it->index = 0;
}

static void li_deinit(void *arg_li) {
    assert(arg_li);

    list_iterator *it = (list_iterator *)(arg_li);

    it->l = NULL;
    it->curr = NULL;
    it->index = -1;
}

static void li_delete(void *arg_li) {
    assert(arg_li);

    list_iterator **it = (list_iterator **)(arg_li);
    li_deinit((*it));

    free((*it));
    (*it) = NULL;
}

static void *li_advance(void *arg_li, int n) {
    assert(arg_li);

    list_iterator *it = (list_iterator *)(arg_li);

    if (it->index + n < it->l->length && n + it->index >= 0) {
        it->index += n;
        it->curr = l_node_dn(it->l, it->index);
    }

    return it;
}

static int li_distance(void *arg_li_first, void *arg_li_last) {
    int last_index = arg_li_last ? ((list_iterator *)(arg_li_last))->index : 0;

    int first_index = arg_li_first ? ((list_iterator *)(arg_li_first))->index : 0;

    return (last_index - first_index);
}

static void *li_begin(void *arg_li) {
    assert(arg_li);

    list_iterator *it = (list_iterator *)(arg_li);

    it->curr = it->l->head;
    it->index = 0;

    return it;
}

static void *li_end(void *arg_li) {
    assert(arg_li);

    list_iterator *it = (list_iterator *)(arg_li);

    it->curr = it->l->tail;
    it->index = (int)(it->l->length);

    return it;
}

static void *li_next(void *arg_li) {
    assert(arg_li);

    list_iterator *it = (list_iterator *)(arg_li);

    if (it->index == it->l->length) {
        return NULL;
    } else {
        if (it->curr == NULL) {
            dlnode *node = l_node_dn(it->l, it->index);

            if (node == NULL) {
                return NULL;
            } else {
                it->curr = node;
            }
        }

        void *target = it->curr->valaddr;
        it->curr = it->curr->next;
        it->index++;
        return target;
    }
}

static void *li_prev(void *arg_li) {
    assert(arg_li);

    list_iterator *it = (list_iterator *)(arg_li);

    if (it->index == -1) {
        return NULL;
    } else {
        if (it->curr == NULL) {
            dlnode *node = l_node_dn(it->l, it->index);

            if (node == NULL) {
                it->curr = it->l->tail;

                if (it->curr == NULL) {
                    return NULL;
                }
            } else {
                it->curr = node;
            }
        }

        it->curr = it->index == it->l->length ? it->l->tail : it->curr->prev;

        --it->index;
        void *target = it->curr->valaddr;
        return target;
    }
}

static bool li_has_next(void *arg_li) {
    assert(arg_li);

    list_iterator *it = (list_iterator *)(arg_li);
    return it->index < it->l->length;
}

static bool li_has_prev(void *arg_li) {
    assert(arg_li);

    list_iterator *it = (list_iterator *)(arg_li);
    return it->index > 0 && it->l->length > 0;
}

static struct typetable *li_get_ttbl(void *arg_li) {
    assert(arg_li);

    list_iterator *it = (list_iterator *)(arg_li);
    return it->l->ttbl;
}
