/**
 *	@file		main.c
 * 	@brief		Testbench for bstree.h/bstree.c
 *
 *	@author		Gemuele (Gem) Aludino
 * 	@date		29 Aug 2019
 *	@copyright  Copyright © 2019 Gemuele Aludino
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "bstree.h"

int main(int argc, const char *argv[]) {
    const int sz = 7;
    int intarr[] = { 1, 2, 3, 4, 5, 6, 7 };

    ///*
    bstree *t = bstree_new();
    /*
    bstree_insert(t, 7);
    bstree_insert(t, 3);
    bstree_insert(t, 10);
    bstree_insert(t, 5);
    bstree_insert(t, 8);
    //*/

    //*
    for (int i = 0; i < 20; i++) {
        bstree_insert(t, (rand() % 100) + 1);
    }
    //*/

    bstree_puts(t);

    printf("size of tree: %d\n", bstree_size(t));
    bstree_foreach(t, bsnode_int_puts, INORDER);

    int erase = 10;
    printf("erasing %d...\n", erase);
    bstree_erase(t, erase);

    bstree_puts(t);

    bstree *copy = bstree_newcopy(t);

    bstree_delete(&t);

    printf("printing copy tree:\n");
    bstree_puts(copy);

    bsnode *root = *bstree_data(copy);

    printf("addr root->right->right->right->parent: %p\n", root->right->right->right->parent);

    bstree_erase(copy, 50);
    bstree_puts(copy);

    bstree_delete(&copy);

    return 0;
}
