/**
 *  @file       rbtree.h
 *  @brief      Header file for a left-leaning red-black tree data structure
 *
 *  @author     Gemuele (Gem) Aludino
 *  @date       1 Aug 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 *
 *  [Algorithm credits]
 *  rotate_left, rotate_right, color_flip,
 *  move_red_right, move_red_left, erase_min, erase_max, erase, fixup
 *  http://www.cs.princeton.edu/~rs/talks/LLRB/LLRB.pdf
 *  http://www.cs.princeton.edu/~rs/talks/LLRB/RedBlack.pdf
 *  Dr. Robert Sedgewick, Princeton University
 */

#ifndef RBTREE_H
#define RBTREE_H

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
 *  @typedef    rbnode
 *  @brief      Alias for (struct rbnode)
 *
 *  All instances of (struct rbnode) will be addressed as (rbnode).
 */
typedef struct rbnode rbnode;

/**
 *  @typedef    rbnode_ptr
 *  @brief      Alias for (struct rbnode *) or (rbnode *)
 *
 *  All instances of (struct rbnode *) will be addressed as (rbnode *).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *   be aliased into a single word)
 */
typedef struct rbnode *rbnode_ptr;

/**
 *  @typedef    rbnode_dptr
 *  @brief      Alias for (struct rbnode **) or (rbnode **)
 *
 *  All instances of (struct rbnode **) will be addressed as (rbnode **).
 *
 *  This typedef is to be used only for macros that perform token-pasting.
 *  (such macros require input types that contain spaces and/or asterisks
 *    be aliased into a single word)
 */
typedef struct rbnode **rbnode_dptr;

struct rbnode {
    void *valaddr;

    rbnode *left;
    rbnode *right;

    enum node_color color;
};

typedef struct rbtree rbtree;

/*<< rbtree: constructors */
rbtree *rbt_new(struct typetable *ttbl);
rbtree *rbt_newcopy(rbtree *t);
rbtree *rbt_newrnge(iterator *first, iterator *last);
rbtree *rbt_newmove(rbtree **t);

/*<< rbtree: destructor */
void rbt_delete(rbtree **t);

/*<< rbtree: iterator functions */


/*<< rbtree: length functions */
size_t rbt_size(rbtree *t);
size_t rbt_maxsize(rbtree *t);
int rbt_height(rbtree *t);
size_t rbt_leafct(rbtree *t);
bool rbt_empty(rbtree *t);

/*<< rbtree: element access functions */
void *rbt_front(rbtree *t);
void *rbt_min(rbtree *t);
void *rbt_max(rbtree *t);
void *rbt_predecessor(rbtree *t, const void *valaddr);
void *rbt_successor(rbtree *t, const void *valaddr);

/*<< rbtree: element access with const qualifier */
const void *rbt_front_const(rbtree *t);
const void *rbt_min_const(rbtree *t);
const void *rbt_max_const(rbtree *t);
const void *rbt_predecessor_const(rbtree *t, const void *valaddr);
const void *rbt_successor_const(rbtree *t, const void *valaddr);

/*<< rbtree: node access */
rbnode **rbt_data(rbtree *t);

/*<< rbtree: lookup */
void *rbt_find(rbtree *t, const void *valaddr);

/*<< rbtree: insert, erase, erase min/max */
void rbt_insert(rbtree *t, const void *valaddr);
void rbt_insert_unique(rbtree *t, const void *valaddr);
void rbt_erase(rbtree *t, const void *valaddr);
void rbt_erase_min(rbtree *t);
void rbt_erase_max(rbtree *t);

/*<< rbtree: remove all elements */
void rbt_clear(rbtree *t);

/*<< rbtree: container swappage */
void rbt_swap(rbtree **t, rbtree **other);

/*<< rbtree: map consumer function to all rbnodes */
void rbt_foreach(rbtree *t, void (*consumer)(void *), enum node_traversal ttype);
void rbt_foreach_recursive(rbtree *t, void (*consumer)(void *),
                           enum node_traversal ttype);

/*<< rbtree: print tree to file stream */
void rbt_puts(rbtree *t);
void rbt_fputs(rbtree *t, FILE *dest);

/*<< rbtree: required function prototypes for (struct typetable) */
int rbtree_compare(const void *c1, const void *c2);
void *rbtree_copy(void *dst, const void *src);
void rbtree_swap(void *c1, void *c2);
void rbtree_print(const void *arg, FILE *dest);
void rbtree_delete(void *arg);

/**< ptrs to vtables */
extern struct typetable *_rbtree_;
extern struct iterator_table *_rbtree_iterator_;

#endif /* RBTREE_H */
