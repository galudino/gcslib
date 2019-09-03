/**
 *  @file       list_template.c
 *  @brief      Template source file for a type-safe doubly linked-list ADT
 *
 *  @author     Gemuele Aludino
 *  @date       21 Jul 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#ifdef T

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
 *  @file       mergesort_template.h
 *  @brief      Public header file for access to mergesort-related functions
 */
#include "mergesort_template.h"

/**
 *  @def        LIST_TEMPLATE_MAXIMUM_STACK_BUFFER_SIZE
 *  @brief      Arbitrary maximum size for a statically allocated (char) buffer
 */
#define LIST_TEMPLATE_MAXIMUM_STACK_BUFFER_SIZE 16384

#define dlallocate(T) template(dlallocate, T)
#define dlinit(T) template(dlinit, T)
#define dlinitptr(T) template(dlinitptr, T)
#define dldeinit(T) template(dldeinit, T)
#define dlnew(T) template(dlnew, T)
#define dlnewptr(T) template(dlnewptr, T)
#define dldelete(T) template(dldelete, T)

#define lallocate(T) template(lallocate, T)
#define linit(T) template(linit, T)
#define ldeinit(T) template(ldeinit, T)
#define lnodedl(T) template(dlnodedl, T)
#define ltraverseh(T) template(ltraverseh, T)
#define ltraverset(T) template(ltraverset, T)

#define list_iterator(T) template(list_iterator, T)
#define list_iterator_ptr(T) template(list_iterator_ptr, T)
#define list_iterator_dptr(T) template(list_iterator_dptr, T)

#define liallocate(T) template(liallocate, T)
#define liinit(T) template(liinit, T)
#define lideinit(T) template(lideinit, T)
#define linew(T) template(linew, T)
#define lidelete(T) template(lidelete, T)

#define liadvance(T) template(liadvance, T)
#define lidistance(T) template(lidistance, T)

#define libegin(T) template(libegin, T)
#define liend(T) template(liend, T)

#define linext(T) template(linext, T)
#define liprev(T) template(liprev, T)

#define lihasnext(T) template(lihasnext, T)
#define lihasprev(T) template(lihasprev, T)

#define ligetttbl(T) template(ligetttbl, T)

#define list_type_table_ptr_id(T) type_table_ptr_id(T)

/*<< dlnode(T): allocation/initialization */
static dlnode(T) * dlallocate(T)(void);
static void dlinit(T)(dlnode(T) * n, T val);
static void dlinitptr(T)(dlnode(T) * n, T *valaddr);

/*<< dlnode(T): deinitialization */
static void dldeinit(T)(dlnode(T) * n, delete_fn delfn);

/*<< dlnode(T): constructor/destructor */
static dlnode(T) * dlnew(T)(T val);
static dlnode(T) * dlnewptr(T)(T *valaddr);
static void dldelete(T)(void *arg, delete_fn delfn);

/**
 *  @struct     list(T)
 *  @brief      An opaque abstract data type (ADT)
 *              representing a type-safe doubly-linked list
 */
struct list(T) {
    dlnode(T) * head;
    dlnode(T) * tail;

    size_t length;

    struct typetable *ttbl;
};

/*<< list(T): allocation/initialization */
static list(T) * lallocate(T)(void);
static void linit(T)(list(T) * l);

/*<< list(T) deinitialization */
static void ldeinit(T)(list(T) * l);

/*<< list(T): node access */
static dlnode(T) * lnodedl(T)(list(T) * l, int index);
static dlnode(T) * ltraverseh(T)(list(T) * l, int index);
static dlnode(T) * ltraverset(T)(list(T) * l, int index);

/**
 *  @struct     typetable ttbl_list_T
 *  @brief      Instance of struct typetable used for containers
 *              whose elements are of type (list(T))
 *              (used for containers of list, for example)
 */
struct typetable table_id(ttbl_list, T) = {
    sizeof(struct list(T)), tmpl_list_compare(T), tmpl_list_copy(T),
    tmpl_list_print(T), tmpl_list_delete(T)};

/**< Handle for ttbl_list_T, used for container instantiations */
struct typetable *list_typetable_ptr_id(T) = &table_id(ttbl_list, T);

/**
 *  @typedef        list_iterator(T)
 *  @brief          Alias for (struct list_iterator(T))
 *
 *  All instances of (struct list_iterator(T))
 *  will be addressed as (list_iterator(T)).
 */
typedef struct list_iterator(T) list_iterator(T);

/**
 *  @typedef        list_iterator_ptr(T)
 *  @brief          Alias for (struct list_iterator(T) *)
 *                  or (list_iterator(T) *)
 *
 *  All instances of (struct list_iterator(T) *)
 *  will be addressed as (list_iterator(T) *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct list_iterator(T) * list_iterator_ptr(T);

/**
 *  @typedef        list_iterator_dptr(T)
 *  @brief          Alias for (struct list_iterator(T) **)
 *                  or (list_iterator(T) **)
 *
 *  All instances of (struct list_iterator(T) **)
 *  will be addressed as (list_iterator(T) **).
 *
 *  This typedef is to be used only for macros that perform token-pasting,
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct list_iterator(T) * *list_iterator_dptr(T);

/**
 *  @struct        list_iterator(T)
 *  @brief         An implementation-specific iterator for (list(T)).
 *
 *  IMPORTANT:
 *  ----------
 *  This implementation-specific iterator is kept private from the client
 *  and not intended to be used directly. See iterator.h for more information.
 */
struct list_iterator(T) {
    list(T) * l;
    dlnode(T) * curr;
    int index;
};

/**< list_iterator(T): create/initialize/destroy */
static void *liallocate(T)(void);
static void *linew(T)(void *arg_l);
static void liinit(T)(void *arg_li, void *arg_l);
static void lideinit(T)(void *arg_li);
static void lidelete(T)(void *arg_li);

/**< list_iterator(T): index based operations */
static void *liadvance(T)(void *arg_li, int n);
static int lidistance(T)(void *arg_li_first, void *arg_li_last);

/**< list_iterator(T): return iterator to begin/end */
static void *libegin(T)(void *arg_li);
static void *liend(T)(void *arg_li);

/**< list_iterator(T): advance/retract iterator position */
static void *linext(T)(void *arg_li);
static void *liprev(T)(void *arg_li);

/**< list_iterator(T): determine if there are more elements to traverse */
static bool lihasnext(T)(void *arg_li);
static bool lihasprev(T)(void *arg_li);

/**< list_iterator: retrieve (typetable *) */
static struct typetable *ligetttbl(T)(void *arg_li);

/**
 *  iterator_table itbl_list_T
 *  @brief      Instance of struct iterator_table used by iterator
 *              (see iterator.h)
 */
struct iterator_table table_id(itbl_list, T) = {
    linew(T),  lidelete(T), liadvance(T), lidistance(T), libegin(T),  liend(T),
    linext(T), liprev(T),   lihasnext(T), lihasprev(T),  ligetttbl(T)};

struct iterator_table *list_iterator_table_ptr_id(T) = &table_id(itbl_list, T);

list(T) * lnew(T)() {
    list(T) *l = lallocate(T)();
    linit(T)(l);
    return l;
}

list(T) * lnewfill(T)(size_t n, T val) {
    list(T) *l = lnew(T)();

    for (size_t i = 0; i < n; i++) {
        lpushb(T)(l, val);
    }

    return l;
}

list(T) * lnewfillptr(T)(size_t n, T *valaddr) {
    assert(valaddr);

    list(T) *l = lnew(T)();

    for (size_t i = 0; i < n; i++) {
        lpushbptr(T)(l, valaddr);
    }

    return l;
}

list(T) *lnewrnge(T)(iterator *first, iterator *last) {
    assert(first);
    assert(last);

    int start = it_distance(NULL, first);
    int end = it_distance(NULL, last);
    int delta = end - start;

    list(T) *l = lnew(T)();

    for (int i = 0; i < delta; i++) {
        T *target = it_next(first);
        lpushbptr(T)(l, target);
    }

    return l;
}

list(T) * lnewcopy(T)(list(T) * l) {
    assert(l);
    list(T) *copy = lnew(T)();

    dlnode(T) *curr = l->head;

    bool has_copy_fn = l->ttbl->copy != NULL;
    T *src = NULL;

    if (has_copy_fn) {
            BUG(__FILE__, "** FIXME ** Only shallow copies of list(T) are supported at this time.\nDisable the copy function in the struct typetable instance used by this templated container and iterate through this list manually to deep copy each element into this list.\n(Hint: Use an iterator to get at the addresses of each source element.)");
        for (size_t i = 0; i < l->length; i++) {
            src = &curr->data;
            T *target = l->ttbl->copy(&src);
            lpushbptr(T)(copy, target);
            //lpushb(T)(copy, *target);
            curr = curr->next;
        }
    } else {
        for (size_t i = 0; i < l->length; i++) {
            T target = curr->data;
            lpushb(T)(copy, target);
            curr = curr->next;
        }
    }

    return copy;
}

list(T) * lnewmove(T)(list(T) * *l) {
    assert(l);

    list(T) *move = lallocate(T)();

    move->head = (*l)->head;
    move->tail = (*l)->tail;
    move->length = (*l)->length;
    move->ttbl = (*l)->ttbl;

    linit(T)((*l));

    return move;
}

void ldelete(T)(list(T) * *l) {
    assert(l);

    ldeinit(T)((*l));

    free((*l));
    (*l) = NULL;
}

iterator *lbegin(T)(list(T) * l) {
    assert(l);
    iterator *it = it_new(list_iterator_table_ptr_id(T), l);
    return it_begin(it);
}

iterator *lend(T)(list(T) * l) {
    assert(l);
    iterator *it = it_new(list_iterator_table_ptr_id(T), l);
    return it_end(it);
}

size_t lsize(T)(list(T) * l) {
    assert(l);
    return l->length;
}

size_t lmaxsize(T)(list(T) * l) {
    assert(l);

    if (sizeof(void *) == 8) {
        // 64-bit system
        return (pow(2.0, 64.0) / (l->ttbl->width));
    } else {
        // 32-bit system
        return (pow(2.0, 32.0) / (l->ttbl->width));
    }
}

void lresizefill(T)(list(T) * l, size_t n, T val) {
    assert(l);

    for (int i = 0; i < n; i++) {
        lpushb(T)(l, val);
    }
}

void lresizefillptr(T)(list(T) * l, size_t n, T *valaddr) {
    assert(l);

    for (int i = 0; i < n; i++) {
        lpushbptr(T)(l, valaddr);
    }
}

bool lempty(T)(list(T) * l) {
    assert(l);
    return l->length == 0;
}

T *lfront(T)(list(T) * l) {
    assert(l);
    return l->head ? &(l->head->data) : NULL;
}

T *lback(T)(list(T) * l) {
    assert(l);
    return l->tail ? &(l->tail->data) : NULL;
}

const T *lfrontconst(T)(list(T) * l) {
    assert(l);
    return l->head ? &(l->head->data) : NULL;
}

const T *lbackconst(T)(list(T) * l) {
    assert(l);
    return l->tail ? &(l->tail->data) : NULL;
}

void lassignrnge(T)(list(T) * l, iterator *first, iterator *last) {
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
            T *target = it_next(first);

            if (i < old_length) {
                /**
                 *  The range of elements specified by [start, end)
                 *  will overwrite existing elements starting at index 0.
                 */
                lreplaceatptr(T)(l, i, target);
            } else {
                /**
                 *  If the range of elements specified by [start, end)
                 *  exceeds the current logical length of the internal buffer,
                 *  then elements will be appended to the rear of the buffer.
                 */
                lpushbptr(T)(l, target);
            }
        }
    } else {
        WARNING(__FILE__, "lassignrnge(T) returned early -- iterator last must have a greater position index than that of iterator first");
        return;
    }

    /* Restoring the index of first */
    it_advance(first, start - (start + delta));
}

void lassignfill(T)(list(T) * l, size_t n, T val) {
    assert(l);

    for (int i = 0; i < n; i++) {
        lpopf(T)(l);
        lpushf(T)(l, val);
    }
}

void lassignfillptr(T)(list(T) * l, size_t n, T *valaddr) {
    assert(l);
    assert(valaddr);

    for (int i = 0; i < n; i++) {
        lpopf(T)(l);
        lpushfptr(T)(l, valaddr);
    }
}

void lpushf(T)(list(T) * l, T val) {
    assert(l);

    dlnode(T) *new_node = dlnew(T)(val);

    if (l->head == NULL) {
        l->tail = new_node;
    } else {
        new_node->next = l->head;
        l->head->prev = new_node;
    }

    l->head = new_node;
    ++l->length;
}

void lpushfptr(T)(list(T) * l, T *valaddr) {
    assert(l);
    assert(valaddr);

    dlnode(T) *new_node = dlnewptr(T)(valaddr);

    if (l->head == NULL) {
        l->tail = new_node;
    } else {
        new_node->next = l->head;
        l->head->prev = new_node;
    }

    l->head = new_node;
    ++l->length;
}

void lpushb(T)(list(T) * l, T val) {
    assert(l);

    dlnode(T) *new_node = dlnew(T)(val);

    if (l->tail == NULL) {
        l->head = new_node;
    } else {
        new_node->prev = l->tail;
        l->tail->next = new_node;
    }

    l->tail = new_node;
    ++l->length;
}

void lpushbptr(T)(list(T) * l, T *valaddr) {
    assert(l);
    assert(valaddr);

    dlnode(T) *new_node = dlnewptr(T)(valaddr);

    if (l->tail == NULL) {
        l->head = new_node;
    } else {
        new_node->prev = l->tail;
        l->tail->next = new_node;
    }

    l->tail = new_node;
    ++l->length;
}

void lpopf(T)(list(T) * l) {
    assert(l);

    if (l->head == NULL) {
        return;
    } else if (l->length == 1) {
        dldelete(T)(&l->head, l->ttbl->delete);
        --l->length;
        l->head = NULL;
        l->tail = NULL;
        return;
    }

    dlnode(T) *old_node = l->head;

    if (l->head != l->tail) {
        l->head = l->head->next;
    }

    dldelete(T)(&old_node, l->ttbl->delete);
    --l->length;
}

void lpopb(T)(list(T) * l) {
    assert(l);

    if (l->head == NULL) {
        return;
    } else if (l->length == 1) {
        dldelete(T)(&l->head, l->ttbl->delete);
        --l->length;
        l->head = NULL;
        l->tail = NULL;
        return;
    }

    dlnode(T) *old_node = l->tail;

    if (l->tail != l->head) {
        l->tail = l->tail->prev;
        l->tail->next = NULL;
    }

    dldelete(T)(&old_node, l->ttbl->delete);
    --l->length;
}

iterator *linsert(T)(list(T) * l, iterator *pos, T val) {
    assert(l);
    assert(pos);

    lpushb(T)(l, val);

    const size_t back_index = l->length - 1;

    int i = it_distance(NULL, pos);

    while (i < back_index) {
        lswapelem(T)(l, i++, back_index);
    }

    return pos;
}

iterator *linsertptr(T)(list(T) * l, iterator *pos, T *valaddr) {
    assert(l);
    assert(pos);
    assert(valaddr);

    lpushbptr(T)(l, valaddr);

    const size_t back_index = l->length - 1;

    /**
     *  The address of a struct is same address as that of its first field.
     *      The first field in iterator (see iterator.c) is
     *          void *iter;
     *      Therefore, the address of a void *iter is of type (void **).
     *
     *  We will cast pos, which is of type (iterator *), to type (void **).
     *  Then we will dereference the casted pos variable,
     *      which consequently yields a (void *).
     *
     *  Hiding behind void *iter is really a (list_iterator *)
     *  so we will cast our resultant (void *) to a (list_iterator *).
     */
    list_iterator(T) *li = (list_iterator(T) *)(*(void **)(pos));

    /**
     *  Now that we have our variable vi (list_iterator),
     *  we can retrieve the iterator's current position (index).
     */
    int i = li->index;

    while (i < back_index) {
        lswapelem(T)(l, i++, back_index);
    }

    return pos;
}

iterator *linsertfill(T)(list(T) * l, iterator *pos, size_t n, T val) {
    assert(l);
    assert(pos);

    for (int i = 0; i < n; i++) {
        linsert(T)(l, pos, val);
    }

    return pos;
}

iterator *linsertfillptr(T)(list(T) * l, iterator *pos, size_t n, T *valaddr) {
    assert(l);
    assert(pos);
    assert(valaddr);

    for (int i = 0; i < n; i++) {
        linsertptr(T)(l, pos, valaddr);
    }

    return pos;
}

iterator *linsertrnge(T)(list(T) * l, iterator *pos, iterator *first, iterator *last) {
    assert(l);
    assert(pos);
    assert(first);
    assert(last);

    int curr = it_distance(NULL, pos);
    int position = curr;

    int start = it_distance(NULL, first);
    int end = it_distance(NULL, last);
    int delta = end - start;

    if (delta > 0) {
        for (int i = 0; i < delta; i++) {
            T *target = it_next(first);
            linsertatptr(T)(l, curr++, target);
        }
    } else {
        return NULL;
    }

    it_advance(first, start - (start + delta));
    it_advance(pos, position - (position + delta));

    return pos;
}

iterator *linsertmove(T)(list(T) * l, iterator *pos, T val) {
    assert(l);
    assert(pos);

    if (l->ttbl->delete != NULL) {
        T *element = l->ttbl->copy(&val);

        if (element == NULL) {
            return NULL;
        }

        linsertptr(T)(l, pos, element);

        l->ttbl->delete (&val);
    } else {
        /**
         *  If the elements are statically allocated -- we can't "move"
         *  anything -- do a traditional insert.
         */
        linsert(T)(l, pos, val);
    }

    return pos;
}

iterator *linsertmoveptr(T)(list(T) * l, iterator *pos, T *valaddr) {
    assert(l);
    assert(pos);

    if (l->ttbl->delete != NULL) {
        T *element = l->ttbl->copy(valaddr);

        if (element == NULL) {
            return NULL;
        }

        linsertptr(T)(l, pos, element);

        l->ttbl->delete (&valaddr);
    } else {
        /**
         *  If the elements are statically allocated -- we can't "move"
         *  anything -- do a traditional insert.
         */
        linsertptr(T)(l, pos, valaddr);
    }

    return pos;
}

iterator *lerase(T)(list(T) * l, iterator *pos) {
    assert(l);
    assert(pos);

    int curr = it_distance(NULL, pos);
    const size_t back_index = l->length - 1;

    if (curr < 0) {
        return NULL;
    } else if (curr == l->length - 1) {
        lpopb(T)(l);
        return pos;
    } else if (curr >= l->length) {
        return NULL;
    }

    if (curr < back_index && curr >= 0) {
        for (int i = curr; i < back_index; i++) {
            lswapelem(T)(l, i, i + 1);
        }
    }

    lpopb(T)(l);
    return pos;
}

iterator *lerasernge(T)(list(T) *l, iterator *pos, iterator *last) {
    assert(l);
    assert(pos);
    assert(last);

    // calculate the delta, or distance between pos and last
    int pos_index = it_distance(NULL, pos);
    int last_index = it_distance(NULL, last);
    int delta = last_index - pos_index;

    // retrieve the node behind the beginning of the erasure range
    dlnode(T) *prev_hook = lnodedl(T)(l, pos_index - 1);
    dlnode(T) *to_delete = prev_hook->next;

    // retrieve the node in front of the last node of the erasure range
    dlnode(T) *next_hook = to_delete;
    for (int i = 0; i < delta; i++) {
        next_hook = next_hook->next;
    }

    // link in prev_hook and next_hook
    prev_hook->next = next_hook;
    next_hook->prev = prev_hook;

    // delete the old nodes
    dlnode(T) *next = NULL;
    while (to_delete != next_hook) {
        next = to_delete->next;

        if (l->ttbl->delete) {
            dldelete(T)(&to_delete, l->ttbl->delete);
        } else {
            dldelete(T)(&to_delete, NULL);
        }

        to_delete = next;
    }

    // update the length variable
    l->length = l->length - delta;
    return pos;
}

void lswap(T)(list(T) * *l, list(T) * *other) {
    assert(*l);
    assert(*other);

    list(T) *temp = *l;
    *l = *other;
    *other = temp;
}

void lclear(T)(list(T) * l) {
    while (l->length > 0) {
        lpopb(T)(l);
    }
}

void linsertat(T)(list(T) * l, size_t index, T val) {
    assert(l);

    lpushb(T)(l, val);
    size_t back_index = l->length - 1;

    if (index >= back_index) {
        return;
    }

    for (size_t i = index; i <= back_index; ++i) {
        lswapelem(T)(l, i, back_index);
    }
}

void linsertatptr(T)(list(T) * l, size_t index, T *valaddr) {
    assert(l);

    lpushbptr(T)(l, valaddr);
    size_t back_index = l->length - 1;

    if (index >= back_index) {
        return;
    }

    for (size_t i = index; i <= back_index; ++i) {
        lswapelem(T)(l, i, back_index);
    }
}

void leraseat(T)(list(T) * l, size_t index) {
    assert(l);

    if (index < 0) {
        return;
    } else if (index == 0) {
        lpopf(T)(l);
        return;
    } else if (index == l->length - 1) {
        lpopb(T)(l);
        return;
    } else if (index >= l->length) {
        return;
    }

    dlnode(T) *target = lnodedl(T)(l, (int)(index));

    dlnode(T) *target_prev = target->prev;
    dlnode(T) *target_next = target->next;

    target_prev->next = target_next;
    target_next->prev = target_prev;

    dldelete(T)(&target, l->ttbl->delete);
    --l->length;
}

void lreplaceat(T)(list(T) * l, size_t index, T val) {
    assert(l);

    lpushb(T)(l, val);

    if (index >= l->length) {
        return;
    }

    const size_t back_index = l->length - 1;

    lswapelem(T)(l, back_index, index);
    lpopb(T)(l);
}

void lreplaceatptr(T)(list(T) * l, size_t index, T *valaddr) {
    assert(l);
    assert(valaddr);

    lpushbptr(T)(l, valaddr);

    if (index >= l->length) {
        return;
    }

    const size_t back_index = l->length - 1;

    lswapelem(T)(l, back_index, index);
    lpopb(T)(l);
}

void lswapelem(T)(list(T) * l, size_t n1, size_t n2) {
    assert(l);

    bool n1_bad = n1 >= l->length;
    bool n2_bad = n2 >= l->length;

    bool good_indices = !n1_bad && !n2_bad;

    if (good_indices && l->length > 0) {
        dlnode(T) *node_n1 = NULL;
        dlnode(T) *node_n2 = NULL;

        int difference = n2 - n1;

        if (difference >= 1) {
            if (difference == 1) {
                node_n1 = lnodedl(T)(l, (int)(n1));
                node_n2 = node_n1->next;
            } else {
                // n2 is ahead of n1. get n1 first, then use n1 to get n2
                node_n1 = lnodedl(T)(l, (int)(n1));

                dlnode(T) *temp = node_n1;
                for (int i = 3; i > 0; i--) {
                    temp = temp->next;
                }

                node_n2 = temp;
            }
        } else if (difference < 1) {
            if (difference == -1) {
                node_n2 = lnodedl(T)(l, (int)(n2));
                node_n1 = node_n2->next;
            } else {
                // n1 is ahead of n2. get n2 first, then use n2 to get n1
                node_n2 = lnodedl(T)(l, (int)(n2));

                dlnode(T) *temp = node_n2;
                for (int i = 3; i > 0; i--) {
                    temp = temp->next;
                }

                node_n1 = temp;
            }
        }

        T temp = node_n1->data;
        node_n1->data = node_n2->data;
        node_n2->data = temp;
    } else {
        return;
    }
}

iterator *lsplice(T)(list(T) * l, iterator *pos, list(T) * other,
                     iterator *opos) {
    assert(l);
    assert(pos);
    assert(other);
    assert(opos);

    struct iterator_table *itbl_pos = it_get_itbl(pos);

    if (itbl_pos->new != linew(T)) {
        ERROR(__FILE__, "pos must be created from a list container instance.");
        return NULL;
    }

    list_iterator(T) *lpos = (list_iterator(T) *)(*(void **)(pos));
    list_iterator(T) *lopos = (list_iterator(T) *)(*(void **)(opos));

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
    dlnode(T) *position = lpos->index > 0 ? lpos->curr->prev : lpos->curr;
    dlnode(T) *temp_next = lpos->l->length > 1 ? position->next : NULL;

    dlnode(T) *head = l->head;
    dlnode(T) *tail = l->tail;
    // dlnode *tail_prev = l->length > 1 ? tail->prev : NULL;

    dlnode(T) *ohead = other->head;
    dlnode(T) *otail = other->tail;
    // dlnode *otail_prev = other->length > 1 ? otail->prev : NULL;

    /**
     *  The node from list other to splice into list l
     */
    dlnode(T) *src = lopos->curr;

    dlnode(T) *temp_src_prev = lopos->index > 0 ? src->prev : NULL;

    dlnode(T) *temp_src_next =
        other->length > 1 && lopos->index < other->length ? src->next : NULL;

    if (lpos->index == 0) {
        /**
         *  If list(T) l's iterator is at index 0,
         *  we append node src to the front of list(T) l
         *  and it becomes its new head.
         */

        src->next = head;
        head->prev = src;
        head = src;

        l->head = head;
    } else if (lpos->index == l->length - 1) {
        /**
         *  If list(T) l's iterator is at index l->length - 1,
         *  we append node src to the back of list l
         *  and it becomes its new tail.
         */

        tail->next = src;
        src->prev = tail;
        tail = src;

        l->tail = tail;
    } else if (l->length == 0) {
        /**
         *  If list(T) l is an empty list,
         *  src becomes its head and tail.
         */
        head = src;
        tail = src;

        l->head = head;
        l->tail = tail;
    } else {
        /**
         *  In any other case, we splice src into the middle of list(T) l.
         *  No changes to the head/tail of list(T) l need to be made.
         */

        position->next = src;
        src->prev = position;

        src->next = temp_next;
        temp_next->prev = src;
    }

    if (lopos->index == 0) {
        /**
         *  If src is the head of list(T) other...
         */
        if (temp_src_next != NULL) {
            temp_src_next->prev = NULL;
            ohead = temp_src_next;

            other->head = ohead;
        }
    } else if (lopos->index == other->length - 1) {
        /**
         *  If src is the tail of list(T) other...
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
     *  Decrementing list(T) other's length value,
     *  Incrementing list(T) l's length value.
     */
    --other->length;
    ++l->length;

    return pos;
}

iterator *lsplicelist(T)(list(T) * l, iterator *pos, list(T) * other) {
    assert(l);
    assert(pos);
    assert(other);

    struct iterator_table *itbl = it_get_itbl(pos);

    if (itbl->new != linew(T)) {
        ERROR(__FILE__, "pos must be created from a list container instance.");
        return NULL;
    }


    list_iterator(T) *lpos = (list_iterator(T) *)(*(void **)(pos));

    /**
     *  If list(T) other is empty, there is no work to do here.
     */
    if (other->length == 0) {
        return pos;
    }

    /**
     *  Represents LIST L1 (list(T) l)
     */
    dlnode(T) *position = lpos->index > 0 ? lpos->curr->prev : lpos->curr;
    dlnode(T) *temp_next = lpos->l->length > 1 ? position->next : NULL;

    /**
     *  Represents LIST L2 (list(T) other)
     */
    dlnode(T) *ohead = other->head;
    dlnode(T) *otail = other->tail;

    /**
     *  The following is an example of how lsplicelist(T) will work.
     *
     *  int arr[2][4] = { { 1, 2, 3, 4 }, { 6, 7, 8, 9 } };
     *
     *  list(int) *l1 = larrtol(int)(arr[0], 4);
     *  list(int) *l2 = larrtol(int)(arr[1], 4);
     *
     *  iterator *it1 = it_advance(lbegin(int)(l1), 1);
     *  lsplicelist(int)(l1, it1, l2);
     *
     *  // l1 is { 1, 6, 7, 8, 9, 2, 3, 4, 5 }
     *  // l2 is {   }
     *
     ======== BEFORE: ========

     LIST L1: </1->---<-2->---<-3->---<-4/>
     (l1->head) ^       ^               ^ (l1->tail)
     (lpos->index)
     (lpos->curr) (dlnode(int) *) - iterator's position node

     LIST L2: </6->---<-7->---<-8->---<-9/>
     (ohead) ^                       ^ (otail)

     ======== AFTER: ========

     LIST L1: </1->---<-6->---<-7->---<-8->---<-9->---<-2->---<-3->---<-4/>
     (l1->head)  ^                                       ^               ^
     (lpos->index)    (l1->tail)
     (lpos->curr) (dlnode(int) *)

     LIST L2: (empty list)

     *
     */

    if (lpos->index == l->length) {
        WARNING(__FILE__,
                "Undefined behavior if pos->iter->index == l->length");
    }

    if (lpos->index == 0) {
        /**
         *  list(T) other's tail node gets linked to list(T) l's head node
         *  effectively resulting in a concatentation of lists.
         *
         *  list(T) l's head node is reassigned to list(T) other's head node.
         */
        otail->next = position;
        position->prev = otail;

        l->head = ohead;
    } else if (lpos->index == l->length - 1) {
        /**
         *  list(T) l's tail node gets linked to list(T) l's head node
         *  effectively resulting in a concatenation of lists.
         *
         *  list(T) l's tail node is reassigned to list(T) other's tail node.
         */
        temp_next->next = ohead;
        ohead->prev = temp_next;

        l->tail = otail;
    } else if (l->length == 0) {
        /*
         *  list(T) l is an empty list, but list(T) other is not.
         *  We assign list(T) l's head node to list(T) other's head node,
         *  and lis(T)t l's tail node to list(T) other's tail node.
         */

        l->head = ohead;
        l->tail = otail;
    } else {
        /**
         *  This case represents a true "splice" in the middle of the list.
         *
         *  list(T) l is split at node lpos->curr and list(T) other's head node
         *  is inserted where lpos->curr used to be.
         *
         *  list(T) l's tail node is safely reassigned by using a temporary node
         *  to linearly traverse list(T) l until a null pointer is found.
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
     *  Increment list(T) l's length counter value by other's length counter
     * value.
     */
    l->length += other->length;
    lpos->index += other->length;

    /**
     *  list(T) other is now an empty container.
     */
    other->length = 0;
    other->head = NULL;
    other->tail = NULL;

    return pos;
}

iterator *lsplicernge(T)(list(T) * l, iterator *pos, list(T) * other,
                         iterator *first, iterator *last) {
    assert(l);
    assert(pos);
    assert(other);
    assert(first);
    assert(last);

    struct iterator_table *itbl_pos = it_get_itbl(pos);

    if (itbl_pos->new != linew(T)) {
        ERROR(__FILE__, "pos must be created from a list container instance.");
    }

    struct iterator_table *itbl_first = it_get_itbl(first);

    if (itbl_first->new != linew(T)) {
        ERROR(__FILE__, "first must be created from a list container instance.");
    }

    struct iterator_table *itbl_last = it_get_itbl(last);

    if (itbl_last->new != linew(T)) {
        ERROR(__FILE__, "last must be created from a list container instance.");
        return NULL;
    }

    /**
     *  Position within list(T) l
     */
    // list_iterator(T) *lpos = (list_iterator(T) *)(*(void **)(pos));

    /**
     *  Bounds within list(T) other (basis for the sublist to splice)
     */
    list_iterator(T) *lfirst = (list_iterator(T) *)(*(void **)(first));
    list_iterator(T) *llast = (list_iterator(T) *)(*(void **)(last));

    /**
     *  If list(T) other is empty,
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
                "[%d, %d)\tLength of list(T) other: %d",
                lfirst->index, llast->index, other->length);
    }

    if (lfirst->index == 0 && llast->index == other->length - 1) {
        return lsplicelist(T)(l, pos, other);
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
    dlnode(T) *ohead = lfirst->curr;
    dlnode(T) *otail = llast->curr->prev;

    /**
     *  sublist_length is the difference between the iterator indices plus one.
     */
    int sublist_length = llast->index - lfirst->index;

    if (lfirst->index == 0) {
        /**
         *  Sublist's head is the head node of it's host list - list(T) other
         *  Extra care must be taken to ensure non sublist nodes are linked.
         */

        other->head = otail->next;
        other->head->prev = NULL;
    } else if (llast->index == other->length) {
        /**
         *  Sublist's tail is the head node of it's host list - list(T) other
         *  Extra care must be taken to ensure non sublist nodes are linked.
         */

        otail = llast->curr;

        other->tail = ohead->prev;
        other->tail->next = NULL;
    } else {
        /**
         *  Sublist occurs in the middle of list(T) other.
         */

        /**
         *  temp_first_prev is to the left of ohead
         *  temp_last_next is to the right of otail
         */
        dlnode(T) *temp_first_prev = lfirst->curr->prev;
        dlnode(T) *temp_last_next = llast->l->length > 1 ? llast->curr : NULL;

        /**
         *  The remainder of list(T) other is rejoined at the split point
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
     *  Isolating the front of the sublist from the remainder of list(T) other
     */
    ohead->prev = NULL;

    /**
     *  Isolating the rear of the sublist from the remainder of list(T) other
     */
    otail->next = NULL;

    /**
     *  Since (struct list(T)) is defined in this compilation unit,
     *  we can create an instance of it on the stack.
     *
     *  All we need is a temporary container for ohead and otail,
     *  the head and tail of our sublist, so that we can reuse the subroutine
     *  l_splicelist(T) to do the splice work.
     *
     *  By the time this function returns, and the list(T) sublist instance
     *  is popped off of the stack, nodes ohead and otail have already
     *  been "spliced" in, and thus we do not lose them.
     */
    list(T) sublist = {ohead, otail, sublist_length, other->ttbl};
    lsplicelist(T)(l, pos, &sublist);

    return pos;
}

void lremove(T)(list(T) * l, T val) {
    assert(l);

    int found = lsearch(T)(l, val);

    while (found != -1) {
        leraseat(T)(l, found);
        found = lsearch(T)(l, val);
    }
}

void lremoveptr(T)(list(T) * l, T *valaddr) {
    assert(l);
    assert(valaddr);

    int found = lsearch(T)(l, (*valaddr));

    while (found != -1) {
        leraseat(T)(l, found);
        found = lsearch(T)(l, (*valaddr));
    }
}

void lremoveif(T)(list(T) * l, T val, bool (*unary_predicate)(const void *)) {
    assert(l);
    assert(unary_predicate);

    if (unary_predicate(&val)) {
        lremove(T)(l, val);
    }
}

void lremoveifptr(T)(list(T) * l, T *valaddr,
                     bool (*unary_predicate)(const void *)) {
    assert(l);
    assert(valaddr);
    assert(unary_predicate);

    if (unary_predicate(valaddr)) {
        lremoveptr(T)(l, valaddr);
    }
}

void lunique(T)(list(T) * l) {
    lsort(T)(l);

    dlnode(T) *curr = l->head;
    int i = 0;
    while (curr != NULL) {
        T *first = &(curr->data);
        T *second = NULL;

        if (curr->next != NULL) {
            second = &(curr->next->data);
        }

        if (second != NULL) {
            if (l->ttbl->compare(first, second) == 0) {
                leraseat(T)(l, i);
                --i;
            }
        }

        curr = curr->next;
        ++i;
    }
}

list(T) * lmerge(T)(list(T) * l, list(T) * other) {
    assert(l);
    assert(other);

    if (l == other || l->ttbl->compare != other->ttbl->compare) {
        return l;
    }

    dlnode(T) *curr = other->head;
    while (curr != NULL) {
        T val = curr->data;

        lpushb(T)(l, val);
        curr = curr->next;
    }

    lsort(T)(l);
    lclear(T)(other);

    return l;
}

list(T) *
    lmergecustom(T)(list(T) * l, list(T) * other,
                    bool (*binary_predicate)(const void *, const void *)) {
    assert(l);
    assert(other);
    assert(binary_predicate);

    if (l == other) {
        return l;
    }

    dlnode(T) *curr = other->head;
    while (curr != NULL) {
        T val = curr->data;

        lpushb(T)(l, val);
        curr = curr->next;
    }

    binary_predicate_fn temp = (binary_predicate_fn)(l->ttbl->compare);
    l->ttbl->compare = (compare_fn)(binary_predicate);

    lsort(T)(l);
    l->ttbl->compare = (compare_fn)(temp);

    lclear(T)(other);

    return l;
}

void lreverse(T)(list(T) * l) {
    assert(l);

    const size_t half_length = l->length / 2;
    const size_t back_index = l->length - 1;

    for (size_t i = 0; i < half_length; i++) {
        lswapelem(T)(l, i, back_index - 1);
    }
}

void lsort(T)(list(T) * l) {
    assert(l);

    if (l->length < 2) {
        return;
    }

    // dlmergesortiterative(T)(&l->head, l->ttbl->compare);
    // dlmergesortrecursive(T)(&l->head, &l->tail, l->ttbl->compare);

    /*
     *  Hotfix to update tail pointer.
     *  Need a better way to keep track of this.
     */
    while (l->tail->next != NULL) {
        l->tail = l->tail->next;
    }
}

int lsearch(T)(list(T) * l, T val) {
    assert(l);

    int result = -1;

    if (l->ttbl->compare != NULL) {
        dlnode(T) *curr = l->head;

        for (size_t i = 0; i < l->length; i++) {
            bool found = l->ttbl->compare(&val, &(curr->data)) == 0;

            if (found) {
                result = (int)(i);
                break;
            }

            curr = curr->next;
        }
    }

    return result;
}

int lsearchptr(T)(list(T) * l, T *valaddr) {
    assert(l);
    assert(valaddr);

    int result = -1;

    if (l->ttbl->compare != NULL) {
        dlnode(T) *curr = l->head;

        for (size_t i = 0; i < l->length; i++) {
            bool found = l->ttbl->compare(valaddr, &(curr->data)) == 0;

            if (found) {
                result = (int)(i);
                break;
            }

            curr = curr->next;
        }
    }

    return result;
}

list(T) * larrtol(T)(T *base, size_t n) {
    assert(base);

    list(T) *l = lnew(T)();

    for (int i = 0; i < n; i++) {
        T target = base[i];
        lpushb(T)(l, target);
    }

    return l;
}

T *lltoarr(T)(list(T) * l) {
    assert(l);

    T *result = NULL;
    result = calloc(l->length, l->ttbl->width);
    assert(result);

    int i = 0;
    dlnode(T) *curr = l->head;

    while (curr) {
        result[i++] = curr->data;
        curr = curr->next;
    }

    return result;
}

void lputs(T)(list(T) * l) { lfputs(T)(l, stdout); }

void lputsf(T)(list(T) * l, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim) {
    lfputsf(T)(l, stdout, before, after, postelem, empty, breaklim);
}

void lfputs(T)(list(T) * l, FILE *dest) {
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

    lfputsf(T)(l, dest, buffer1, buffer2, postelem, empty, breaklim);
}

void lfputsf(T)(list(T) * l, FILE *dest, const char *before, const char *after,
                const char *postelem, const char *empty, size_t breaklim) {
    assert(l);

    fprintf(dest, "%s", before ? before : "");

    if (l->length == 0) {
        fprintf(dest, "%s\n", empty);
    } else {
        dlnode(T) *dn = l->head;

        for (size_t i = 0, curr = 1; i < l->length; i++, curr++) {
            T target = dn->data;
            l->ttbl->print(&target, dest);

            if (i < l->length - 1) {
                fprintf(dest, "%s", postelem ? postelem : "");
            }

            if (curr == breaklim) {
                curr = 0;
                fprintf(dest, "\n");
            }

            dn = dn->next;
        }
    }

    fprintf(dest, "%s", after ? after : "");
}

int tmpl_list_compare(T)(const void *c1, const void *c2) {
    assert(c1);
    assert(c2);

    list(T) *l1 = *(list(T) **)(c1);
    list(T) *l2 = *(list(T) **)(c2);

    if (l1->ttbl->compare != l2->ttbl->compare) {
        return -1;
    }

    lsort(T)(l1);
    lsort(T)(l2);

    size_t size = l1->length < l2->length ? l1->length : l2->length;

    int delta = 0;

    for (int i = 0; i < size; i++) {
        dlnode(T) *dn1 = lnodedl(T)(l1, i);
        dlnode(T) *dn2 = lnodedl(T)(l2, i);

        T *target1 = &(dn1->data);
        T *target2 = &(dn2->data);

        delta += l1->ttbl->compare(target1, target2);
    }

    return delta;
}

void *tmpl_list_copy(T)(const void *arg) {
    assert(arg);

    list(T) *l = *(list(T) **)(arg);
    return lnewcopy(T)(l);
}

void tmpl_list_print(T)(const void *arg, FILE *dest) {
    assert(arg);
    assert(dest);

    list(T) *l = *(list(T) **)(arg);
    lfputs(T)(l, dest);
}

void tmpl_list_delete(T)(void *arg) {
    assert(arg);

    list(T) **l = (list(T) **)(arg);
    ldelete(T)(l);
}

static dlnode(T) * dlallocate(T)() {
    dlnode(T) *n = NULL;
    n = malloc(sizeof((*n)));
    assert(n);
    return n;
}

static void dlinit(T)(dlnode(T) * n, T val) {
    assert(n);

    n->data = val;

    n->prev = NULL;
    n->next = NULL;
}

static void dlinitptr(T)(dlnode(T) * n, T *valaddr) {
    assert(n);
    assert(valaddr);

    n->data = (*valaddr);

    n->prev = NULL;
    n->next = NULL;
}

static void dldeinit(T)(dlnode(T) * n, delete_fn delfn) {
    assert(n);

    if (delfn) {
        delfn(&(n->data));
    }

    n->prev = NULL;
    n->next = NULL;
}

static dlnode(T) * dlnew(T)(T val) {
    dlnode(T) *n = dlallocate(T)();
    dlinit(T)(n, val);
    return n;
}

static dlnode(T) * dlnewptr(T)(T *valaddr) {
    dlnode(T) *n = dlallocate(T)();
    dlinitptr(T)(n, valaddr);
    return n;
}

static void dldelete(T)(void *arg, delete_fn delfn) {
    assert(arg);
    dlnode(T) **dn = (dlnode(T) **)(arg);

    dldeinit(T)((*dn), delfn);

    free((*dn));
    (*dn) = NULL;
}

static list(T) * lallocate(T)(void) {
    list(T) *l = NULL;
    l = malloc(sizeof((*l)));
    assert(l);
    return l;
}

static void linit(T)(list(T) * l) {
    assert(l);

    l->head = NULL;
    l->tail = NULL;

    l->length = 0;
    l->ttbl = list_type_table_ptr_id(T) ? list_type_table_ptr_id(T)
                                            : _void_ptr_;

    /* If any of the function pointers supplied are null,
     they will be replaced with the respective void_ptr functions */
    l->ttbl->compare = l->ttbl->compare ? l->ttbl->compare : NULL;
    l->ttbl->copy = l->ttbl->copy ? l->ttbl->copy : NULL;
    l->ttbl->print = l->ttbl->print ? l->ttbl->print : NULL;
    l->ttbl->delete = l->ttbl->delete ? l->ttbl->delete : NULL;
}

static void ldeinit(T)(list(T) * l) {
    if (l != NULL) {
        lclear(T)(l);
    }

    l->head = NULL;
    l->tail = NULL;

    l->length = 0;

    l->ttbl = NULL;
}

static dlnode(T) * lnodedl(T)(list(T) * l, int index) {
    assert(l);

    dlnode(T) *dn = (index <= l->length / 2) ? ltraverseh(T)(l, index)
                                             : ltraverset(T)(l, index);

    return dn;
}

static dlnode(T) * ltraverseh(T)(list(T) * l, int index) {
    assert(l);
    assert(l->head);

    dlnode(T) *dn = l->head;

    const int back_index = (int)(l->length - 1);

    int i = 0;
    while (dn != NULL || i != back_index) {
        if (i == index) {
            break;
        }

        dn = dn->next;
        ++i;
    }

    return dn;
}

static dlnode(T) * ltraverset(T)(list(T) * l, int index) {
    assert(l);
    assert(l->tail);

    dlnode(T) *dn = l->tail;

    int over_front_index = -1;

    int i = (int)(l->length - 1);
    while (dn != NULL | i != over_front_index) {
        if (i == index) {
            break;
        }

        dn = dn->prev;
        --i;
    }

    return dn;
}

static void *liallocate(T)() {
    void *it = NULL;
    it = malloc(sizeof(struct list_iterator(T)));
    assert(it);
    return it;
}

static void *linew(T)(void *arg_l) {
    void *it = liallocate(T)();
    liinit(T)(it, arg_l);
    return it;
}

static void liinit(T)(void *arg_li, void *arg_l) {
    assert(arg_li);
    assert(arg_l);

    list_iterator(T) *it = (list_iterator(T) *)(arg_li);
    list(T) *l = (list(T) *)(arg_l);

    it->l = l;
    it->curr = it->l->head;
    it->index = 0;
}

static void lideinit(T)(void *arg_li) {
    assert(arg_li);

    list_iterator(T) *it = (list_iterator(T) *)(arg_li);

    it->l = NULL;
    it->curr = NULL;
    it->index = -1;
}

static void lidelete(T)(void *arg_li) {
    assert(arg_li);

    list_iterator(T) **it = (list_iterator(T) **)(arg_li);
    lideinit(T)((*it));

    free((*it));
    (*it) = NULL;
}

static void *liadvance(T)(void *arg_li, int n) {
    assert(arg_li);

    list_iterator(T) *it = (list_iterator(T) *)(arg_li);

    if (it->index + n < it->l->length && n + it->index >= 0) {
        it->index += n;
        it->curr = lnodedl(T)(it->l, it->index);
    }

    return it;
}
static int lidistance(T)(void *arg_li_first, void *arg_li_last) {
    int last_index = arg_li_last ? ((list_iterator(T) *)(arg_li_last))->index : 0;

    int first_index = arg_li_first ? ((list_iterator(T) *)(arg_li_first))->index : 0;

    return (last_index - first_index);
}

static void *libegin(T)(void *arg_li) {
    assert(arg_li);

    list_iterator(T) *it = (list_iterator(T) *)(arg_li);

    it->curr = it->l->head;
    it->index = 0;

    return it;
}
static void *liend(T)(void *arg_li) {
    assert(arg_li);

    list_iterator(T) *it = (list_iterator(T) *)(arg_li);

    it->curr = it->l->tail;
    it->index = (int)(it->l->length);

    return it;
}

static void *linext(T)(void *arg_li) {
    assert(arg_li);

    list_iterator(T) *it = (list_iterator(T) *)(arg_li);

    if (it->index == it->l->length) {
        return NULL;
    } else {
        if (it->curr == NULL) {
            dlnode(T) *node = lnodedl(T)(it->l, it->index);

            if (node == NULL) {
                return NULL;
            } else {
                it->curr = node;
            }
        }

        T *target = &(it->curr->data);
        it->curr = it->curr->next;
        it->index++;
        return target;
    }
}
static void *liprev(T)(void *arg_li) {
    assert(arg_li);

    list_iterator(T) *it = (list_iterator(T) *)(arg_li);

    if (it->index == -1) {
        return NULL;
    } else {
        if (it->curr == NULL) {
            dlnode(T) *node = lnodedl(T)(it->l, it->index);

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
        T *target = &(it->curr->data);
        return target;
    }
}

static bool lihasnext(T)(void *arg_li) {
    assert(arg_li);

    list_iterator(T) *it = (list_iterator(T) *)(arg_li);
    return it->index < it->l->length;
}

static bool lihasprev(T)(void *arg_li) {
    assert(arg_li);

    list_iterator(T) *it = (list_iterator(T) *)(arg_li);
    return it->index > 0 && it->l->length > 0;
}

static struct typetable *ligetttbl(T)(void *arg_li) {
    assert(arg_li);

    list_iterator(T) *it = (list_iterator(T) *)(arg_li);
    return it->l->ttbl;
}

#endif /* T */
