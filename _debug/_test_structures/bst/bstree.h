/**
 *  @file       bstree.h
 *  @brief      Header file for a binary search tree data structure, type int
 *
 *  @author     Gemuele (Gem) Aludino
 *  @date       23 Jul 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#ifndef BSTREE_H
#define BSTREE_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bsnode bsnode;
typedef struct bstree bstree;

enum bstree_traversal_type { INORDER, PREORDER, POSTORDER, LEVELORDER };

struct bsnode {
    int data;

    bsnode *left;
    bsnode *right;
    bsnode *parent;
};

void bsnode_int_puts(void *arg);
void bsnode_int_puts_nobrk(void *arg);

bstree *bstree_new(void);
bstree *bstree_newcopy(bstree *t);
void bstree_delete(void *arg);

int *bstree_find(bstree *t, int val);
int *bstree_front(bstree *t);
int *bstree_min(bstree *t);
int *bstree_max(bstree *t);
bsnode **bstree_data(bstree *t);

int bstree_size(bstree *t);
int bstree_height(bstree *t);
int bstree_leafct(bstree *t);
bool bstree_empty(bstree *t);

void bstree_insert(bstree *t, int val);
void bstree_erase(bstree *t, int val);

void bstree_clear(bstree *t);

void bstree_foreach(bstree *t, void (*consumer)(void *),
                    enum bstree_traversal_type ttype);

void bstree_puts(bstree *t);
void bstree_fputs(bstree *t, FILE *dest);

#endif /* BSTREE_H */
