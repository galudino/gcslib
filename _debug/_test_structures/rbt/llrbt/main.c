/**
 *  @file       main.c
 *  @brief      Source file for testing a left-leaning red-black tree
 *
 *  @author     Gemuele (Gem) Aludino
 *  @date       31 Jul 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "rbtree.h"

int main(int argc, const char * argv[]) {
    //*
    printf("[creating new tree t]\n");
    rbtree *t = rbtree_new();
    int erase = -1;
    //*/

    //*
     for (int i = 0; i < 100; i++) {
         int val = rand() % 50;
         int *exists = rbtree_find(t, val);

         while (exists) {
             printf("[val %d already exists. generating a new value...]\n", val);
             val = rand() % 100;
             exists = rbtree_find(t, val);
         }

         printf("[inserting val %d into tree t]\n", val);
         rbtree_insert(t, val);
     }
     //*/

    /*
    for (int i = 1; i <= 15; i++) {
        rbtree_insert(t, i);
    }

    rbtree_insert(t, 15);
    printf("[printing tree t]\n");
    rbtree_puts(t);

    printf("[erasing min %d]\n", (*rbtree_min(t)));
    rbtree_erase_min(t);

    printf("[erasing max %d]\n", (*rbtree_max(t)));
    rbtree_erase_max(t);

    printf("[printing tree t]\n");
    rbtree_puts(t);

    printf("{");
    if (rbtree_empty(t)) {
        printf(" empty tree ");
    } else {
        rbtree_foreach(t, rbnode_int_puts_nobrk, POSTORDER);
    }
    printf("}\n\n");

    //*
    erase = 2;
    printf("[erase val %d from tree t]\n", erase);
    rbtree_erase(t, erase);

    erase = 4;
    printf("[erasing val %d from tree t]\n", erase);
    rbtree_erase(t, erase);

    printf("[printing tree t after erasing %d]\n", erase);
    rbtree_puts(t);

    erase = 10;
    printf("[erasing val %d from tree t]\n", erase);
    rbtree_erase(t, erase);


    printf("[printing tree t after erasing %d]\n", erase);
    rbtree_puts(t);

    erase = 12;
    printf("[erasing val %d from tree t]\n", erase);
    rbtree_erase(t, erase);


    printf("[printing tree t after erasing %d]\n", erase);
    rbtree_puts(t);
    //*/

    printf("[creating new tree, copy of t named copy]\n");
    rbtree *copy = rbtree_newcopy(t);

    printf("[deleting tree t]\n");
    rbtree_delete(&t);

    printf("[printing tree copy]\n");
    rbtree_puts(copy);

    //printf("[retrieving root of tree copy]\n");
    //rbnode *root = *rbtree_data(copy);

    /*
     printf("[from tree copy's root:\naddr root->right->right->right->parent: (%p): [%d]]\n\n",
     root->right->right->right->parent, root->right->right->right->parent->data);

     //*/

    /*
     printf("root->left: %p\n", root->left);
     printf("root->right->left: %p\n", root->right->left);

     erase = 50;
     printf("[erasing val %d from tree copy]\n", erase);
     rbtree_erase(copy, erase);

     printf("[printing tree copy after erasing %d]\n", erase);
     rbtree_puts(copy);
     */

    printf("{");
    if (rbtree_empty(copy)) {
        printf(" empty tree ");
    } else {
        rbtree_foreach(copy, rbnode_int_puts_nobrk, INORDER);
    }
    printf("}\n\n");

    printf("[deleting tree copy]\n");
    rbtree_delete(&copy);
    //*/
    return 0;
}
