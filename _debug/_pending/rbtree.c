/**
 *  @file       rbtree.c
 *  @brief      Source file for a left-leaning red-black tree data structure
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

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rbtree.h"
#include "vector.h"

typedef void (*delete_rbnode_fn)(rbnode **, delete_fn);

/**< rbnode: allocation/initialization */
static rbnode *rbn_allocate(void);
static void rbn_init(rbnode *n, const void *valaddr, size_t width);
static void rbn_deinit(rbnode *n, delete_fn delfn);

/**< rbnode: new/newcopy/delete */
static rbnode *rbn_new(const void *valaddr, size_t width);
static rbnode *rbn_newcopy(rbnode *n, size_t width, copy_fn cpyfn);
static void rbn_delete(rbnode **n, delete_fn delfn);

/**< rbnode: traversal to copy/destroy tree */
static void rbn_copytree_recursive(rbnode **n, size_t width, rbnode *o,
                                   copy_fn cpyfn);

static void rbn_deltree_recursive(rbnode **n, delete_rbnode_fn delrbnfn,
                                  delete_fn delfn);

/*<< rbnode: determine node color */
static bool rbn_red(rbnode *n);

/*<< rbnode: length functions */
static size_t rbn_size(rbnode *n);
static int rbn_height(rbnode *n);
static int rbn_level_compare(int x, int y);
static size_t rbn_leafct(rbnode *n);

/*<< rbnode: lookup */
static rbnode *rbn_find(rbnode *n, const void *valaddr, compare_fn cmpfn);

/*<< rbnode: node access functions */
static rbnode *rbn_min(rbnode *n);
static rbnode *rbn_max(rbnode *n);
static rbnode *rbn_successor(rbnode *n);
static rbnode *rbn_predecessor(rbnode *n);

/*<< rbnode: node rotation */
static rbnode *rbn_rotate_left(rbnode *n);
static rbnode *rbn_rotate_right(rbnode *n);
static void rbn_color_flip(rbnode *n);

/*<< rbnode: mutators - insert/erase/fixup */
static rbnode *rbn_insert(rbnode *n, const void *valaddr, size_t width,
                          compare_fn cmpfn);
static rbnode *rbn_move_red_left(rbnode *n);
static rbnode *rbn_move_red_right(rbnode *n);
static rbnode *rbn_erase_min(rbnode *n, delete_fn delfn);
static rbnode *rbn_erase_max(rbnode *n, delete_fn delfn);
static rbnode *rbn_erase(rbnode *n, const void *valaddr, delete_fn delfn,
                         compare_fn compfn);
static rbnode *rbn_fixup(rbnode *n);

/*<< rbnode: traversal functions - recursive */
static void rbn_inorder_recursive(rbnode **n, consumer_fn csmfn);
static void rbn_preorder_recursive(rbnode **n, consumer_fn csmfn);
static void rbn_postorder_recursive(rbnode **n, consumer_fn csmfn);
static void rbn_levelorder_recursive(rbnode **n, consumer_fn csmfn);
static void rbn_levelorder_helper(rbnode **n, consumer_fn csmfn, int level);

/*<< rbnode: traversal functions - iterative */
static void rbn_inorder(rbnode **n, consumer_fn csmfn);
static void rbn_preorder(rbnode **n, consumer_fn csmfn);
static void rbn_postorder(rbnode **n, consumer_fn csmfn);
static void rbn_levelorder(rbnode **n, consumer_fn csmfn);

/*<< rbnode: traversal in buffer (used by iterator) - iterative */
static rbnode **rbn_inorder_buffer(rbnode **n);
static rbnode **rbn_preorder_buffer(rbnode **n);
static rbnode **rbn_postorder_buffer(rbnode **n);
static rbnode **rbn_levelorder_buffer(rbnode **n);

static void **rbn_inorder_base(rbnode **n);

/*<< rbnode: output tree to FILE stream */
static void rbn_fputs(rbnode *n, FILE *dest, char *b, bool last,
                      print_fn printfn);

struct rbtree {
    rbnode *root;
    struct typetable *ttbl;
};

struct typetable ttbl_rbtree = {sizeof(struct rbtree), rbtree_compare,
                                     rbtree_copy, rbtree_print, rbtree_delete};

struct typetable *_rbtree_ = &ttbl_rbtree;

/*<< rbtree: allocation/initialization */
static rbtree *rbt_allocate();
static void rbt_init(rbtree *t, struct typetable *ttbl);
static void rbt_deinit(rbtree *t);

/*<< rbtree: get iterator */
static iterator *rbt_get_iterator(rbtree *t, enum node_traversal ttype);

static void *override__vi_next_rbnode_ptr(void *arg_vi);
static void *override__vi_prev_rbnode_ptr(void *arg_vi);

/*
struct iterator_table itbl_vector_rbnode_ptr = {
    _vector_iterator_->new,
    _vector_iterator_->delete,
    _vector_iterator_->advance,
    _vector_iterator_->distance,
    _vector_iterator_->begin,
    _vector_iterator_->end,
    override__vi_next_rbnode_ptr,
    override__vi_prev_rbnode_ptr,
    _vector_iterator_->has_next,
    _vector_iterator_->has_prev,
    _vector_iterator_->get_ttbl
};
*/

rbtree *rbt_new(struct typetable *ttbl) {
    rbtree *t = rbt_allocate();
    rbt_init(t, ttbl);
    return t;
}

rbtree *rbt_newcopy(rbtree *t) {
    assert(t);

    rbtree *copy = rbt_new(t->ttbl);

    rbn_copytree_recursive(&(copy->root), t->ttbl->width, t->root,
                           t->ttbl->copy);

    return copy;
}

rbtree *rbt_newrnge(iterator *first, iterator *last) {
    assert(first);
    assert(last);

    rbtree *t = rbt_new(it_get_ttbl(first));

    int delta = it_distance(first, last);

    for (int i = 0; i < delta; i++) {
        void *next = it_has_next(first) ? it_next(first) : NULL;

        if (next == NULL) {
            break;
        } else {
            rbt_insert(t, next);
        }
    }

    return t;
}

rbtree *rbt_newmove(rbtree **t) {
    assert((*t));

    rbtree *move = rbt_allocate();

    move->root = (*t)->root;
    move->ttbl = (*t)->ttbl;

    rbt_init((*t), (*t)->ttbl);

    return move;
}

void rbt_delete(rbtree **t) {
    assert((*t));

    rbt_deinit((*t));

    free((*t));
    (*t) = NULL;
}

iterator *rbt_begin(rbtree *t) {
    assert(t);

    iterator *it = rbt_get_iterator(t, INORDER);
    return it_begin(it);
}
iterator *rbt_end(rbtree *t) {
    assert(t);

    iterator *it = rbt_get_iterator(t, INORDER);
    return it_end(it);
}

iterator *rbt_begin_trav(rbtree *t, enum node_traversal ttype) {
    assert(t);

    iterator *it = rbt_get_iterator(t, ttype);
    return it_begin(it);
}

iterator *rbt_end_trav(rbtree *t, enum node_traversal ttype) {
    assert(t);

    iterator *it = rbt_get_iterator(t, ttype);
    return it_end(it);
}

size_t rbt_size(rbtree *t) {
    assert(t);
    return rbn_size(t->root);
}

size_t rbt_maxsize(rbtree *t) {
    assert(t);

    if (sizeof(void *) == 8) {
        // 64-bit system
        return (pow(2.0, 64.0) / (t->ttbl->width));
    } else {
        // 32-bit system
        return (pow(2.0, 32.0) / (t->ttbl->width));
    }
}

int rbt_height(rbtree *t) {
    assert(t);
    return rbn_height(t->root);
}

size_t rbt_leafct(rbtree *t) {
    assert(t);
    return rbn_leafct(t->root);
}

bool rbt_empty(rbtree *t) {
    assert(t);
    return t->root == NULL;
}

void *rbt_front(rbtree *t) {
    assert(t);
    return t->root ? t->root->valaddr : NULL;
}

void *rbt_min(rbtree *t) {
    assert(t);

    void *result = NULL;

    if (t->root) {
        rbnode *min = rbn_min(t->root);
        result = min ? min->valaddr : NULL;
    }

    return result;
}

void *rbt_max(rbtree *t) {
    assert(t);

    void *result = NULL;

    if (t->root) {
        rbnode *max = rbn_max(t->root);
        result = max ? max->valaddr : NULL;
    }

    return result;
}

void *rbt_predecessor(rbtree *t, const void *valaddr) {
    assert(t);

    void *result = NULL;

    if (t->root) {
        rbnode *n = rbn_find(t->root, valaddr, t->ttbl->compare);
        result = n ? rbn_predecessor(n)->valaddr : NULL;
    }

    return result;
}

void *rbt_successor(rbtree *t, const void *valaddr) {
    assert(t);

    void *result = NULL;

    if (t->root) {
        rbnode *n = rbn_find(t->root, valaddr, t->ttbl->compare);
        result = n ? rbn_successor(n)->valaddr : NULL;
    }

    return result;
}

const void *rbt_front_const(rbtree *t) { return rbt_front(t); }

const void *rbt_min_const(rbtree *t) { return rbt_min(t); }

const void *rbt_max_const(rbtree *t) { return rbt_max_const(t); }

const void *rbt_predecessor_const(rbtree *t, const void *valaddr) {
    return rbt_predecessor(t, valaddr);
}

const void *rbt_successor_const(rbtree *t, const void *valaddr) {
    return rbt_successor(t, valaddr);
}

rbnode **rbt_data(rbtree *t) {
    assert(t);
    return t->root ? &t->root : NULL;
}

void *rbt_find(rbtree *t, const void *valaddr) {
    assert(t);

    rbnode *n = rbn_find(t->root, valaddr, t->ttbl->compare);
    return n ? n->valaddr : NULL;
}

void rbt_insert(rbtree *t, const void *valaddr) {
    assert(t);
    assert(valaddr);

    t->root = rbn_insert(t->root, valaddr, t->ttbl->width, t->ttbl->compare);
    t->root->color = BLACK;
}

void rbt_insert_unique(rbtree *t, const void *valaddr) {
    assert(t);
    assert(valaddr);

    if (t->root != NULL) {
        bool duplicate = rbt_find(t, valaddr) != NULL;

        if (duplicate) {
            ERROR(__FILE__, "insert failed - no duplicate elements allowed.");
            return;
        }
    }

    t->root = rbn_insert(t->root, valaddr, t->ttbl->width, t->ttbl->compare);
}

void rbt_erase(rbtree *t, const void *valaddr) {
    assert(t);
    assert(valaddr);

    if (t->root != NULL) {
        rbnode *temp = t->root;
        t->root = rbn_erase(t->root, valaddr, t->ttbl->delete, t->ttbl->compare);

        t->root->color = BLACK;
    }
}

void rbt_erase_min(rbtree *t) {
    assert(t);
    t->root = rbn_erase_min(t->root, t->ttbl->delete);
    t->root->color = BLACK;
}

void rbt_erase_max(rbtree *t) {
    assert(t);
    t->root = rbn_erase_max(t->root, t->ttbl->delete);
    t->root->color = BLACK;
}

void rbt_clear(rbtree *t) {
    assert(t);

    if (t->root) {
        rbn_deltree_recursive(&(t->root), rbn_delete, t->ttbl->delete);
    }
}

void rbt_swap(rbtree **t, rbtree **other) {
    assert((*t));
    assert((*other));

    rbtree *temp = (*t);
    (*t)->root = (*other)->root;
    (*t)->ttbl = (*other)->ttbl;

    (*other)->root = temp->root;
    (*other)->ttbl = temp->ttbl;
}

void rbt_foreach(rbtree *t, consumer_fn csmfn, enum node_traversal ttype) {
    assert(t);

    if (t->root) {
        switch (ttype) {
        case INORDER:
            rbn_inorder(&t->root, csmfn);
            break;

        case PREORDER:
            rbn_preorder(&t->root, csmfn);
            break;

        case POSTORDER:
            rbn_postorder(&t->root, csmfn);
            break;

        case LEVELORDER:
            rbn_levelorder(&t->root, csmfn);
            break;
        }
    }
}

void rbt_foreach_recursive(rbtree *t, consumer_fn csmfn,
                           enum node_traversal ttype) {
    assert(t);

    if (t->root) {
        switch (ttype) {
        case INORDER:
            rbn_inorder_recursive(&t->root, csmfn);
            break;

        case PREORDER:
            rbn_preorder_recursive(&t->root, csmfn);
            break;

        case POSTORDER:
            rbn_postorder_recursive(&t->root, csmfn);
            break;

        case LEVELORDER:
            rbn_levelorder_recursive(&t->root, csmfn);
            break;
        }
    }
}

void rbt_puts(rbtree *t) { rbt_fputs(t, stdout); }

void rbt_fputs(rbtree *t, FILE *dest) {
    assert(t);
    assert(dest);

    if (t->root != NULL) {
        const char *link = "---------------------------";

        char buffer1[128];
        char buffer2[BUFFER_SIZE_4K];

        sprintf(buffer1, "\n%s\n%s\n%s\n", link, "RED-BLACK Tree Elements",
                link);

        const char *bytes_label = t->ttbl->width == 1 ? "byte" : "bytes";

        sprintf(buffer2, "%s\t\t%lu\n%s\t\t%lu %s\n%s\t\t%d\n%s\t%lu\n%s\n",
        "Size         ", rbn_size(t->root),
        "Element size  ", t->ttbl->width, bytes_label,
        "Height       ", rbn_height(t->root),
        "Leaf ct. (non nil)", rbn_leafct(t->root),
        link);

        fprintf(dest, "%s", buffer1);
        fprintf(dest, "\n");
        rbn_fputs(t->root, dest, "", true, t->ttbl->print);
        fprintf(dest, "\n");

        fprintf(dest, "%s\n", link);

        fprintf(dest, "%s\t\t", "Minimum value");
        t->ttbl->print(rbn_min(t->root)->valaddr, dest);
        fprintf(dest, "\n");

        fprintf(dest, "%s\t\t", "Maximum value");
        t->ttbl->print(rbn_max(t->root)->valaddr, dest);
        fprintf(dest, "\n");

        fprintf(dest, "%s\t\t", "Root value   ");
        t->ttbl->print(t->root->valaddr, dest);
        fprintf(dest, "\n\n");

        fprintf(dest, "%s", buffer2);
    } else {
        fprintf(dest, "\n%s\tno elements found\n\n", EMPTY_SET_SYMBOL);
    }
}

/**
 *  Compares node counts between two rbtrees
 *
 *  @param[in]  c1      the first rbtree to compare
 *  @param[in]  c2      the second rbtree to compare
 *
 *  @return     node count delta between c1 and c2
 */
int rbtree_compare(const void *c1, const void *c2) {
    assert(c1);
    assert(c2);

    rbtree *t1 = *(rbtree **)(c1);
    rbtree *t2 = *(rbtree **)(c2);

    // a more specific user-defined rbtree comparison function
    // can be used to override this function --
    // this is merely provided by default.
    // override this function by assigning _rbtree_->compare
    // to another function.
    return rbt_size(t1) - rbt_size(t2);
}

void *rbtree_copy(const void *arg) {
    assert(arg);
    rbtree *t = *(rbtree **)(arg);
    return rbt_newcopy(t);
}

void rbtree_print(const void *arg, FILE *dest) {
    assert(arg);
    rbtree *t = *(rbtree **)(arg);
    rbt_fputs(t, dest);
}

void rbtree_delete(void *arg) {
    assert(arg);
    rbtree *t = *(rbtree **)(arg);
    rbt_delete(&t);
}

static rbnode *rbn_allocate(void) {
    rbnode *n = NULL;
    n = malloc(sizeof((*n)));
    assert(n);
    return n;
}

static void rbn_init(rbnode *n, const void *valaddr, size_t width) {
    assert(n);
    assert(valaddr);
    assert(width > 0);

    void *data = NULL;
    data = malloc(width);
    assert(width);
    memcpy(data, valaddr, width);

    n->valaddr = data;

    n->left = NULL;
    n->right = NULL;

    n->color = RED;
}

static void rbn_deinit(rbnode *n, delete_fn delfn) {
    assert(n);

    if (delfn) {
        // for dynamically allocated valaddr/
        // or if valaddr has dynamically allocated fields
        delfn(n->valaddr);
    }

    free(n->valaddr);
    n->valaddr = NULL;

    n->left = NULL;
    n->right = NULL;

    n->color = BLACK;
}

static rbnode *rbn_new(const void *valaddr, size_t width) {
    rbnode *n = rbn_allocate();
    rbn_init(n, valaddr, width);
    return n;
}

static rbnode *rbn_newcopy(rbnode *n, size_t width, copy_fn cpyfn) {
    assert(n);

    void *copy_valaddr = NULL;

    if (cpyfn) {
        copy_valaddr = cpyfn(n->valaddr);
    } else {
        copy_valaddr = n->valaddr;
    }

    rbnode *copy_node = rbn_new(copy_valaddr, width);
    copy_node->color = n->color;

    return copy_node;
}

static void rbn_delete(rbnode **n, delete_fn delfn) {
    assert((*n));

    rbn_deinit((*n), delfn);

    free((*n));
    (*n) = NULL;
}

static void rbn_copytree_recursive(rbnode **n, size_t width, rbnode *o,
                                   copy_fn cpyfn) {
    if (o == NULL) {
        (*n) = NULL;
    } else {
        (*n) = rbn_newcopy(o, width, cpyfn);

        rbn_copytree_recursive(&((*n)->left), width, o->left, cpyfn);
        rbn_copytree_recursive(&((*n)->right), width, o->right, cpyfn);
    }
}

static void rbn_deltree_recursive(rbnode **n, delete_rbnode_fn delrbnfn,
                                  delete_fn delfn) {
    if ((*n)) {
        rbn_deltree_recursive(&((*n)->left), delrbnfn, delfn);
        rbn_deltree_recursive(&((*n)->right), delrbnfn, delfn);

        delrbnfn(n, delfn);
    }
}

static bool rbn_red(rbnode *n) { return n ? n->color == RED : false; }

static size_t rbn_size(rbnode *n) {
    return n ? (1 + rbn_size(n->left) + rbn_size(n->right)) : 0;
}

static int rbn_height(rbnode *n) {
    return n ? (1 +
                rbn_level_compare(rbn_height(n->left), rbn_height(n->right)))
             : -1;
}

static int rbn_level_compare(int x, int y) { return x >= y ? x : y; }

static size_t rbn_leafct(rbnode *n) {
    if (n == NULL) {
        return 0;
    } else if (n->left == NULL && n->right == NULL) {
        return 1;
    } else {
        return rbn_leafct(n->left) + rbn_leafct(n->right);
    }
}

static rbnode *rbn_find(rbnode *n, const void *valaddr, compare_fn cmpfn) {
    while (n) {
        int compare = cmpfn(n->valaddr, valaddr);

        if (compare == 0) {
            // the desired node was found.
            break;
        } else {
            // if delta is greater than zero,
            // proceed to left subtree,
            // otherwise, proceed to right subtree
            n = compare > 0 ? n->left : n->right;
        }
    }

    return n;
}

static rbnode *rbn_min(rbnode *n) {
    assert(n);

    while (n->left) {
        n = n->left;
    }

    return n;
}

static rbnode *rbn_max(rbnode *n) {
    assert(n);

    while (n->right) {
        n = n->right;
    }

    return n;
}

static rbnode *rbn_successor(rbnode *n) { return rbn_min(n->right); }

static rbnode *rbn_predecessor(rbnode *n) { return rbn_max(n->left); }

static rbnode *rbn_rotate_left(rbnode *n) {
    assert(n);

    rbnode *x = NULL;

    x = n->right;
    n->right = x->left;
    x->left = n;

    x->color = n->color;
    n->color = RED;

    return x;
}

static rbnode *rbn_rotate_right(rbnode *n) {
    assert(n);

    rbnode *x = NULL;

    x = n->left;
    n->left = x->right;
    x->right = n;

    x->color = n->color;
    n->color = RED;

    return x;
}

static void rbn_color_flip(rbnode *n) {
    if (n) {
        // if n != NULL, toggle n's colors/n's children's colors
        n->color = !n->color;

        if (n->left) {
            n->left->color = !(n->left->color);
        }

        if (n->right) {
            n->right->color = !(n->right->color);
        }
    }
}

static rbnode *rbn_insert(rbnode *n, const void *valaddr, size_t width,
                          compare_fn cmpfn) {
    if (n == NULL) {
        // BASE CASE: n is a leaf, return new node
        return rbn_new(valaddr, width);
    }

    if (rbn_red(n->left) && rbn_red(n->right)) {
        rbn_color_flip(n);
    }

    // standard recursive binary search tree insertion

    int compare = cmpfn(n->valaddr, valaddr);

    if (compare > 0) {
        // RECURSIVE CASE 1: val < n->data
        // go to left child and return result of recursive call to it
        n->left = rbn_insert(n->left, valaddr, width, cmpfn);
    } else {
        // RECURSIVE CASE 2: val > n->data
        // go to right child and return result of recursive call to it
        n->right = rbn_insert(n->right, valaddr, width, cmpfn);
    }

    // left-leaning red-black tree functionality - rotations

    if (rbn_red(n->right)) {
        // if n's right child is red
        // rotate left to fix right-leaning red nodes
        n = rbn_rotate_left(n);
    }

    if (rbn_red(n->left) && rbn_red(n->left->left)) {
        // if n's left child is red
        // if n's left grandchild is red
        // rotate right to fix left-leaning double-red nodes
        n = rbn_rotate_right(n);
    }

    return n;
}

static rbnode *rbn_move_red_left(rbnode *n) {
    // start by toggling colors
    rbn_color_flip(n);

    if (rbn_red(n->right->left)) {
        // if n's left grandchild is red (left child of n's right child)
        n->right = rbn_rotate_right(n->right);
        n = rbn_rotate_left(n);

        rbn_color_flip(n);
    }

    return n;
}

static rbnode *rbn_move_red_right(rbnode *n) {
    // start by toggling colors
    rbn_color_flip(n);

    if (rbn_red(n->left->left)) {
        // if n's left grandchild is red (left child of n's left child)
        n = rbn_rotate_right(n);
        rbn_color_flip(n);
    }

    return n;
}

static rbnode *rbn_erase_min(rbnode *n, delete_fn delfn) {
    if (n->left == NULL) {
        // BASE CASE: n->left is a leaf, min node found.
        rbn_delete(&n, delfn);
        return NULL;
    }

    if (!rbn_red(n->left) && !rbn_red(n->left->left)) {
        // n's left child is black
        // n's left grandchild is black
        n = rbn_move_red_left(n);
    }

    // RECURSIVE CASE: left leaf node not found yet
    n->left = rbn_erase_min(n->left, delfn);

    return rbn_fixup(n);
}

static rbnode *rbn_erase_max(rbnode *n, delete_fn delfn) {
    if (rbn_red(n->left->left)) {
        // if n's left child is red,
        // rotate right at n
        n = rbn_rotate_right(n);
    }

    if (n->right == NULL) {
        // BASE CASE: n->right is a leaf, max node found
        rbn_delete(&n, delfn);
        return NULL;
    }

    if (!rbn_red(n->right) && !rbn_red(n->right->left)) {
        // n's right child is black,
        // n's left grandchild is black (left child of n's right child)
        n = rbn_move_red_right(n);
    }

    // RECURSIVE CASE: right leaf node not found yet
    n->right = rbn_erase_max(n->right, delfn);

    return rbn_fixup(n);
}

static rbnode *rbn_erase(rbnode *n, const void *valaddr, delete_fn delfn,
                         compare_fn cmpfn) {
    int compare = cmpfn(n->valaddr, valaddr);

    if (compare > 0) {
        // val is less than n->data
        if (!rbn_red(n->left) && !rbn_red(n->left->left)) {
            // if n's left child is black
            // if n's left grandchild is black (left child of n's left child)
            n = rbn_move_red_left(n);
        }

        // RECURSIVE CASE
        n->left = rbn_erase(n->left, valaddr, delfn, cmpfn);
    } else {
        // val is greater than or equal to n->data
        if (rbn_red(n->left)) {
            // if n's left child is red
            n = rbn_rotate_right(n);
        }

        if (compare == 0 && n->right == NULL) {
            // RECURSIVE CASE: n is the node to erase,
            // and has no right child
            fprintf(stdout, "]\n");

            rbn_delete(&n, delfn);
            return NULL;
        }

        if (!rbn_red(n->right) && !rbn_red(n->right->left)) {
            // if n's right child is black
            // if n's left grandchild is black (left child of n's right child)
            n = rbn_move_red_right(n);
        }

        if (compare == 0) {
            // BASE CASE: if n is the node to delete,
            // but has a right child -- will replace n's data with that of its
            // inorder successor,  then delete the inorder successor node
            rbnode *successor = rbn_successor(n);

            void *to_delete = n->valaddr;
            n->valaddr = successor->valaddr;
            successor->valaddr = to_delete;

            // data fields were switched between rbnode n and its
            // inorder-successor. (change of pointer ownership)
            n->right = rbn_erase_min(n->right, delfn);
        } else {
            // RECURSIVE CASE: n is not the node to delete
            // proceed to right child, since val is greater than n->data
            n->right = rbn_erase(n->right, valaddr, delfn, cmpfn);
        }
    }

    return rbn_fixup(n);
}

static rbnode *rbn_fixup(rbnode *n) {
    if (rbn_red(n->right)) {
        // if n's right child is red
        // rotate left to fix right-leaning nodes
        n = rbn_rotate_left(n);
    }

    if (rbn_red(n->left) && rbn_red(n->left->left)) {
        // if n's left child is red
        // if n's left grandchild is red (left child of n's left child)
        // rotate right to fix left-leaning double-red nodes
        n = rbn_rotate_right(n);
    }

    if (rbn_red(n->left) && rbn_red(n->right)) {
        rbn_color_flip(n);
    }

    return n;
}

static void rbn_inorder_recursive(rbnode **n, consumer_fn csmfn) {
    if ((*n)) {
        rbn_inorder_recursive(&(*n)->left, csmfn);
        csmfn(n);
        rbn_inorder_recursive(&(*n)->right, csmfn);
    }
}

static void rbn_preorder_recursive(rbnode **n, consumer_fn csmfn) {
    if ((*n)) {
        csmfn(n);
        rbn_preorder_recursive(&(*n)->left, csmfn);
        rbn_preorder_recursive(&(*n)->right, csmfn);
    }
}

static void rbn_postorder_recursive(rbnode **n, consumer_fn csmfn) {
    if ((*n)) {
        rbn_postorder_recursive(&(*n)->left, csmfn);
        rbn_postorder_recursive(&(*n)->right, csmfn);
        csmfn(n);
    }
}

static void rbn_levelorder_recursive(rbnode **n, consumer_fn csmfn) {
    assert(n);
    assert(csmfn);

    int height_plus_one = rbn_height((*n)) + 1;
    for (int i = 0; i <= height_plus_one; i++) {
        rbn_levelorder_helper(&(*n), csmfn, i);
    }
}

static void rbn_levelorder_helper(rbnode **n, consumer_fn csmfn, int level) {
    if ((*n) == NULL) {
        return;
    } else if (level == 1) {
        csmfn(n);
    } else if (level > 1) {
        rbn_levelorder_helper(&(*n)->left, csmfn, level - 1);
        rbn_levelorder_helper(&(*n)->right, csmfn, level - 1);
    }
}

static void rbn_inorder(rbnode **n, consumer_fn csmfn) {
    assert((*n));

    size_t sz = rbn_size((*n));

    rbnode **s = NULL;
    s = calloc(sz, sizeof((*s)));
    assert(s);
    int s_size = 0;

    rbnode *current = (*n);

    while (current || (s_size > 0)) {
        if (current) {
            s[s_size++] = current;
            current = current->left;
        } else {
            rbnode *top = s[--s_size];
            current = top;

            csmfn(&current);
            current = current->right;
        }
    }

    free(s);
    s = NULL;
}

static void rbn_preorder(rbnode **n, consumer_fn csmfn) {
    assert((*n));
    assert(csmfn);

    size_t sz = rbn_size((*n));

    rbnode **s = NULL;
    s = calloc(sz, sizeof((*s)));
    assert(s);
    int s_size = 0;

    rbnode *current = (*n);

    while (current || (s_size > 0)) {
        if (current) {
            s[s_size++] = current;
            csmfn(&current);


            current = current->left;
        } else {
            rbnode *top = s[--s_size];
            current = top;

            current = current->right;
        }
    }

    free(s);
    s = NULL;
}

static void rbn_postorder(rbnode **n, consumer_fn csmfn) {
    assert((*n));
    assert(csmfn);

    size_t sz = rbn_size((*n));

    rbnode **s = NULL;
    s = calloc(sz, sizeof((*s)));
    assert(s);
    int s_size = 0;

    int *is = NULL;
    is = calloc(sz, sizeof((*is)));
    assert(is);
    int is_size = 0;

    rbnode *current = NULL;
    int value = -1;     // int popped from is

    current = (*n);
    value = 0;

    int to_push = 0;    // int pushed to is

    if (current == NULL) {
        // empty tree
    } else {
        s[s_size++] = current;

        to_push = 1;
        is[is_size++] = to_push;

        current = current->left;

        while (s_size > 0) {
            if (current && value == 0) {
                s[s_size++] = current;
                to_push = 1;
                is[is_size++] = to_push;

                current = current->left;
            } else {
                current = s[--s_size];
                value = is[--is_size];

                if (value == 1) {
                    s[s_size++] = current;
                    to_push = 2;
                    is[is_size++] = to_push;

                    current = current->right;
                    value = 0;
                } else {
                    csmfn(&current);
                }
            }
        }
    }

    free(is);
    is = NULL;

    free(s);
    s = NULL;
}

static void rbn_levelorder(rbnode **n, consumer_fn csmfn) {
    assert((*n));
    assert(csmfn);

    size_t sz = rbn_size((*n));

    rbnode **temp_q = NULL;
    temp_q = calloc(sz, sizeof(struct rbnode *));
    assert(temp_q);
    int tq_size = 0;
    int tq_cap = sz;
    int tq_front = 0;
    int tq_rear = sz - 1;

    rbnode *current = (*n);
    // push
    tq_rear = (tq_rear + 1) % tq_cap;
    temp_q[tq_rear] = current;
    ++tq_size;

    while (tq_size > 0) {
        // pop
        rbnode *front = temp_q[tq_front];
        tq_front = (tq_front + 1) % tq_cap;
        --tq_size;

        current = front;

        csmfn(&current);

        if (current->left) {
            // push
            tq_rear = (tq_rear + 1) % tq_cap;
            temp_q[tq_rear] = current->left;
            ++tq_size;
        }

        if (current->right) {
            // push
            tq_rear = (tq_rear + 1) % tq_cap;
            temp_q[tq_rear] = current->right;
            ++tq_size;
        }
    }

    free(temp_q);
    temp_q = NULL;
}

static rbnode **rbn_inorder_buffer(rbnode **n) {
    assert((*n));

    size_t sz = rbn_size((*n));

    rbnode **s = NULL;
    s = calloc(sz, sizeof((*s)));
    assert(s);
    int s_size = 0;

    rbnode **q = NULL;
    q = calloc(sz, sizeof((*q)));
    assert(q);
    int q_size = 0;

    rbnode *current = (*n);

    while (current || (s_size > 0)) {
        if (current) {
            s[s_size++] = current;
            current = current->left;
        } else {
            rbnode *top = s[--s_size];
            current = top;

            q[q_size++] = current;
            current = current->right;
        }
    }

    free(s);
    s = NULL;

    return q;
}

static rbnode **rbn_preorder_buffer(rbnode **n) {
    assert((*n));

    size_t sz = rbn_size((*n));

    rbnode **s = NULL;
    s = calloc(sz, sizeof((*s)));
    assert(s);
    int s_size = 0;

    rbnode **q = NULL;
    q = calloc(sz, sizeof((*q)));
    assert(q);
    int q_size = 0;

    rbnode *current = (*n);

    while (current || (s_size > 0)) {
        if (current) {
            s[s_size++] = current;
            q[q_size++] = current;
            current = current->left;
        } else {
            rbnode *top = s[--s_size];
            current = top;

            current = current->right;
        }
    }

    free(s);
    s = NULL;

    return q;
}

static rbnode **rbn_postorder_buffer(rbnode **n) {
    assert((*n));

    size_t sz = rbn_size((*n));

    rbnode **s = NULL;
    s = calloc(sz, sizeof((*s)));
    assert(s);
    int s_size = 0;

    int *is = NULL;
    is = calloc(sz, sizeof((*is)));
    assert(is);
    int is_size = 0;

    rbnode **q = NULL;
    q = calloc(sz, sizeof((*q)));
    assert(q);
    int q_size = 0;

    rbnode *current = NULL;
    int value = -1;     // int popped from is

    current = (*n);
    value = 0;

    int to_push = 0;    // int pushed to is

    if (current == NULL) {
        // empty tree
    } else {
        s[s_size++] = current;

        to_push = 1;
        is[is_size++] = to_push;

        current = current->left;

        while (s_size > 0) {
            if (current && value == 0) {
                s[s_size++] = current;
                to_push = 1;
                is[is_size++] = to_push;

                current = current->left;
            } else {
                current = s[--s_size];
                value = is[--is_size];

                if (value == 1) {
                    s[s_size++] = current;
                    to_push = 2;
                    is[is_size++] = to_push;

                    current = current->right;
                    value = 0;
                } else {
                    q[q_size++] = current;
                }
            }
        }
    }

    free(is);
    is = NULL;

    free(s);
    s = NULL;

    return q;
}

static rbnode **rbn_levelorder_buffer(rbnode **n)  {
    assert((*n));

    size_t sz = rbn_size((*n));

    rbnode **q = NULL;
    q = calloc(sz, sizeof(struct rbnode *));
    assert(q);
    int q_size = 0;
    int q_cap = sz;
    int q_front = 0;
    int q_rear = sz - 1;

    rbnode **temp_q = NULL;
    temp_q = calloc(sz, sizeof(struct rbnode *));
    assert(temp_q);
    int tq_size = 0;
    int tq_cap = sz;
    int tq_front = 0;
    int tq_rear = sz - 1;

    rbnode *current = (*n);
    // push
    tq_rear = (tq_rear + 1) % tq_cap;
    temp_q[tq_rear] = current;
    ++tq_size;

    while (tq_size > 0) {
        // pop
        rbnode *front = temp_q[tq_front];
        tq_front = (tq_front + 1) % tq_cap;
        --tq_size;

        current = front;

        // push
        q_rear = (q_rear + 1) % q_cap;
        q[q_rear] = current;
        ++q_size;

        if (current->left) {
            // push
            tq_rear = (tq_rear + 1) % tq_cap;
            temp_q[tq_rear] = current->left;
            ++tq_size;
        }

        if (current->right) {
            // push
            tq_rear = (tq_rear + 1) % tq_cap;
            temp_q[tq_rear] = current->right;
            ++tq_size;
        }
    }

    free(temp_q);
    temp_q = NULL;

    return q;
}

static void rbn_fputs(rbnode *n, FILE *dest, char *b, bool last,
                      print_fn printfn) {
    /**
     *  Custom recursive preorder traversal
     */

    char newbuf[BUFFER_SIZE_4K];
    strcpy(newbuf, b);

    fprintf(dest, "%s", b);

    if (last) {
        fprintf(dest, "%sR%s----", KGRY, KNRM);
        strcat(newbuf, "      ");
    } else {
        fprintf(dest, "%sL%s----", KGRY, KNRM);
        strcat(newbuf, "|     ");
    }

    bool is_red = rbn_red(n);

    if (n != NULL) {
        const char *label_color = is_red ? KRED_b : KNRM_b;
        fprintf(dest, "%s[", label_color);
        printfn(n->valaddr, dest);

        fprintf(dest, "]%s ", KNRM);

        // address - disable for release
        fprintf(dest, "(%s%p%s)", KCYN, n, KNRM);

        fprintf(dest, "\n");
        rbn_fputs(n->left, dest, newbuf, false, printfn);
        rbn_fputs(n->right, dest, newbuf, true, printfn);
    } else {
        fprintf(dest, "%s[%s]", KGRY, DIAMOND_SYMBOL);

        // address - disable for release
        fprintf(dest, "%s(%p)", KGRY, NULL);

        fprintf(dest, "%s\n", KNRM);
    }
}

static rbtree *rbt_allocate() {
    rbtree *t = NULL;
    t = malloc(sizeof((*t)));
    assert(t);
    return t;
}

static void rbt_init(rbtree *t, struct typetable *ttbl) {
    assert(t);
    t->root = NULL;
    t->ttbl = ttbl ? ttbl : _void_ptr_;
}

static void rbt_deinit(rbtree *t) {
    assert(t);
    rbt_clear(t);

    t->root = NULL;
}

static iterator *rbt_get_iterator(rbtree *t, enum node_traversal ttype) {
    assert(t);

    if (t->root) {
        rbnode **n = NULL;

        switch (ttype) {
        case INORDER:
            n = rbn_inorder_buffer(&t->root);
            break;

        case PREORDER:
            n = rbn_preorder_buffer(&t->root);
            break;

        case POSTORDER:
            n = rbn_postorder_buffer(&t->root);
            break;

        case LEVELORDER:
            n = rbn_levelorder_buffer(&t->root);
            break;
        }

        size_t sz = rbn_size(t->root);

        vector *v = v_new(t->ttbl);

        for (size_t i = 0; i < sz; i++) {
            void *valaddr = n[i]->valaddr;
            v_pushb(v, valaddr);
        }

        if (n) {
            free(n);
            n = NULL;
        }

        return v_begin(v);
    } else {
        return NULL;
    }
}
