/**
 *  @file       rbtree.h
 *  @brief      Header file for a left-leaning red-black tree data structure
 *
 *  @author     Gemuele (Gem) Aludino
 *  @date       31 Jul 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 *
 *  [Algorithm credits]
 *  rotate_left, rotate_right, color_flip, insert,
 *  move_red_right, move_red_left, erase_min, erase_max, erase, fixup
 *  http://www.cs.princeton.edu/~rs/talks/LLRB/LLRB.pdf
 *  http://www.cs.princeton.edu/~rs/talks/LLRB/RedBlack.pdf
 *  Dr. Robert Sedgewick, Princeton University
 *
 *  (preliminary version, to understand the left-leaning
 *   red-black tree algorithm. does not support utils API,
 *   iterator, or void *. uses int as element type T)
 */

#ifndef RBTREE_H
#define RBTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

typedef int (*compare_fn)(const void *, const void *);
typedef void (*consumer_fn)(void *);

enum rbnode_color { RBNODE_BLACK, RBNODE_RED };

typedef struct rbnode rbnode;

struct rbnode {
    int data;

    rbnode *left;
    rbnode *right;

    enum rbnode_color color;
};

void rbnode_int_puts(void *arg);
void rbnode_int_puts_nobrk(void *arg);

typedef struct rbtree rbtree;

enum rbtree_traversal_type { INORDER, PREORDER, POSTORDER, LEVELORDER };

struct rbtree {
    rbnode *root;
};

rbtree *rbtree_new(void);
rbtree *rbtree_newcopy(const rbtree *t);
void rbtree_delete(void *arg);

int rbtree_size(rbtree *t);
int rbtree_height(rbtree *t);
int rbtree_leafct(rbtree *t);
bool rbtree_empty(rbtree *t);

int *rbtree_front(rbtree *t);
int *rbtree_min(rbtree *t);
int *rbtree_max(rbtree *t);
int *rbtree_predecessor(rbtree *t, int val);
int *rbtree_successor(rbtree *t, int val);

rbnode **rbtree_data(rbtree *t);

int *rbtree_find(rbtree *t, int val);

void rbtree_insert(rbtree *t, int val);
void rbtree_erase_min(rbtree *t);
void rbtree_erase_max(rbtree *t);
void rbtree_erase(rbtree *t, int val);

void rbtree_clear(rbtree *t);

void rbtree_foreach(rbtree *t, consumer_fn consumer, enum rbtree_traversal_type ttype);

void rbtree_puts(rbtree *t);
void rbtree_fputs(rbtree *t, FILE *dest);

#endif /* RBTREE_H */
