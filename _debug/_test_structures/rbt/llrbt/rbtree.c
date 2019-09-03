/**
 *  @file       rbtree.c
 *  @brief      Source file for a left-leaning red-black tree data structure
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

#include "rbtree.h"

#define RBTREE_BUFSZ 4096

static rbnode *rbnode_allocate(void);
static void rbnode_init(rbnode *n, int val);
static void rbnode_deinit(rbnode *n);

static rbnode *rbnode_new(int val);
static rbnode *rbnode_newcopy(const rbnode *n);
static void rbnode_delete(void *arg);

static void rbnode_copytree_recursive(rbnode **n, rbnode *o);

static bool rbnode_is_red(rbnode *n);

static rbnode *rbnode_find(rbnode *n, int val);
static rbnode *rbnode_min(rbnode *n);
static rbnode *rbnode_max(rbnode *n);

static int rbnode_size(rbnode *n);
static int rbnode_height(rbnode *n);
static int rbnode_level_compare(int x, int y);
static int rbnode_leafct(rbnode *n);

static rbnode *rbnode_successor(rbnode *n);
static rbnode *rbnode_predecessor(rbnode *n);

static void rbnode_inorder_recursive(rbnode **n, consumer_fn consumer);
static void rbnode_preorder_recursive(rbnode **n, consumer_fn consumer);
static void rbnode_postorder_recursive(rbnode **n, consumer_fn consumer);
static void rbnode_levelorder_recursive(rbnode **n, consumer_fn consumer);
static void rbnode_levelorder_helper(rbnode **n, consumer_fn consumer,
                                     int level);

static rbnode *rbnode_rotate_left(rbnode *n);
static rbnode *rbnode_rotate_right(rbnode *n);
static void rbnode_color_flip(rbnode *n);

static rbnode *rbnode_insert(rbnode *n, int val);
static rbnode *rbnode_move_red_right(rbnode *n);
static rbnode *rbnode_move_red_left(rbnode *n);
static rbnode *rbnode_erase_min(rbnode *n);
static rbnode *rbnode_erase_max(rbnode *n);
static rbnode *rbnode_erase(rbnode *n, int val);
static rbnode *rbnode_fixup(rbnode *n);

static void rbnode_fputs(rbnode *n, FILE *dest, char *b, bool last);

static rbtree *rbtree_allocate(void);
static void rbtree_init(rbtree *t);
static void rbtree_deinit(rbtree *t);

void rbnode_int_puts(void *arg) {
    rbnode **n = (rbnode **)(arg);
    printf("%d\n", (*n)->data);
}

void rbnode_int_puts_nobrk(void *arg) {
    rbnode **n = (rbnode **)(arg);
    printf(" %d ", (*n)->data);
}

rbtree *rbtree_new(void) {
    rbtree *t = rbtree_allocate();
    rbtree_init(t);
    return t;
}

rbtree *rbtree_newcopy(const rbtree *t) {
    assert(t);

    rbtree *copy = rbtree_new();
    // TODO iterative version

    rbnode_copytree_recursive(&(copy->root), t->root);
    return copy;
}

void rbtree_delete(void *arg) {
    assert(arg);

    rbtree **t = (rbtree **)(arg);
    rbtree_deinit((*t));

    free((*t));
    (*t) = NULL;
}

int rbtree_size(rbtree *t) {
    assert(t);
    return rbnode_size(t->root);
}

int rbtree_height(rbtree *t) {
    assert(t);
    return rbnode_height(t->root);
}

int rbtree_leafct(rbtree *t) {
    assert(t);
    return rbnode_leafct(t->root);
}

bool rbtree_empty(rbtree *t) {
    assert(t);
    return t->root != NULL;
}

int *rbtree_front(rbtree *t) { return t->root ? &(t->root->data) : NULL; }

int *rbtree_min(rbtree *t) {
    int *result = NULL;

    if (t->root != NULL) {
        rbnode *min = rbnode_min(t->root);
        result = min ? &min->data : NULL;
    }

    return result;
}

int *rbtree_max(rbtree *t) {
    int *result = NULL;

    if (t->root != NULL) {
        rbnode *max = rbnode_max(t->root);
        result = max ? &max->data : NULL;
    }

    return result;
}

int *rbtree_predecessor(rbtree *t, int val) {
    int *result = NULL;

    if (t->root != NULL) {
        rbnode *n = rbnode_find(t->root, val);
        result = n ? &(rbnode_predecessor(n)->data) : NULL;
    }

    return result;
}

int *rbtree_successor(rbtree *t, int val) {
    int *result = NULL;

    if (t->root != NULL) {
        rbnode *n = rbnode_find(t->root, val);
        result = n ? &(rbnode_successor(n)->data) : NULL;
    }

    return result;
}

rbnode **rbtree_data(rbtree *t) { return t->root ? &t->root : NULL; }

int *rbtree_find(rbtree *t, int val) {
    int *result = NULL;

    if (t->root != NULL) {
        rbnode *n = rbnode_find(t->root, val);
        result = n ? &n->data : NULL;
    }

    return result;
}

void rbtree_insert(rbtree *t, int val) {
    t->root = rbnode_insert(t->root, val);
    t->root->color = RBNODE_BLACK;
}

void rbtree_erase_min(rbtree *t) {
    t->root = rbnode_erase_min(t->root);
    t->root->color = RBNODE_BLACK;
}

void rbtree_erase_max(rbtree *t) {
    t->root = rbnode_erase_max(t->root);
    t->root->color = RBNODE_BLACK;
}

void rbtree_erase(rbtree *t, int val) {
    if (t->root != NULL) {
        t->root = rbnode_erase(t->root, val);
        t->root->color = RBNODE_BLACK;
    }
}

void rbtree_clear(rbtree *t) {
    if (t->root != NULL) {
        rbnode_postorder_recursive(&(t->root), rbnode_delete);
    }
}

void rbtree_foreach(rbtree *t, consumer_fn consumer,
                    enum rbtree_traversal_type ttype) {
    assert(t);

    switch (ttype) {
    case INORDER:
        rbnode_inorder_recursive(&t->root, consumer);
        // TODO iterative version
        break;

    case PREORDER:
        rbnode_preorder_recursive(&t->root, consumer);
        // TODO iterative version
        break;

    case POSTORDER:
        rbnode_postorder_recursive(&t->root, consumer);
        // TODO iterative version
        break;

    case LEVELORDER:
        rbnode_levelorder_recursive(&t->root, consumer);
        // TODO iterative version
        break;
    }
}

void rbtree_puts(rbtree *t) {
    assert(t);
    rbtree_fputs(t, stdout);
}

void rbtree_fputs(rbtree *t, FILE *dest) {
    assert(t);
    assert(dest);

    if (t->root != NULL) {
        const char *link = "---------------------------";

        char buffer1[128];
        char buffer2[RBTREE_BUFSZ];

        sprintf(buffer1, "\n%s\n%s\n%s\n", link, "RED-BLACK Tree Elements",
                link);

        sprintf(
            buffer2,
            "%s\n%s\t\t%d\n%s\t\t%d\n%s\t\t%d\n\n%s\t\t%d\n%s\t\t%d\n%s\t\t%"
            "d\n%s\n",
            link, "Minimum value", *rbtree_min(t), "Maximum value",
            *rbtree_max(t), "Root value   ", *rbtree_front(t), "Size         ",
            rbnode_size(t->root), "Height       ", rbnode_height(t->root),
            "Leaf ct.     ", rbnode_leafct(t->root), link);

        fprintf(dest, "%s", buffer1);
        fprintf(dest, "\n");
        rbnode_fputs(t->root, dest, "", true);
        fprintf(dest, "\n");

        fprintf(dest, "%s", buffer2);
    } else {
        fprintf(dest, "\n{ empty tree }\n\n");
    }
}

static rbnode *rbnode_allocate(void) {
    rbnode *n = NULL;
    n = malloc(sizeof(*n));
    assert(n);
    return n;
}

static void rbnode_init(rbnode *n, int val) {
    assert(n);

    n->data = val;

    n->left = NULL;  // must be initialized to t->TNULL by caller
    n->right = NULL; // must be initialized to t->TNULL by caller

    n->color = RBNODE_RED; // all newly inserted red-black tree nodes are red
}

static void rbnode_deinit(rbnode *n) {
    assert(n);
    // destroy (*n)->data if applicable

    n->left = NULL;
    n->right = NULL;

    n->color = RBNODE_BLACK;
}

static rbnode *rbnode_new(int val) {
    rbnode *n = rbnode_allocate();
    rbnode_init(n, val);
    return n;
}

static rbnode *rbnode_newcopy(const rbnode *n) {
    rbnode *copy = rbnode_new(n->data);
    copy->color = n->color;
    return copy;
}

static void rbnode_delete(void *arg) {
    assert(arg);
    rbnode **n = (rbnode **)(arg);

    rbnode_deinit((*n));

    free((*n));
    (*n) = NULL;
}

static void rbnode_copytree_recursive(rbnode **n, rbnode *o) {
    if (o == NULL) {
        (*n) = NULL;
    } else {
        (*n) = rbnode_newcopy(o);

        rbnode_copytree_recursive(&(*n)->left, o->left);
        rbnode_copytree_recursive(&(*n)->right, o->right);
    }
}

static bool rbnode_is_red(rbnode *n) {
    return n ? n->color == RBNODE_RED : false;
}

static rbnode *rbnode_find(rbnode *n, int val) {
    while (n != NULL) {
        // the comparison function will assess
        // compare(n->data, val) here.
        int compare = n->data - val;

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

static rbnode *rbnode_min(rbnode *n) {
    assert(n);

    while (n->left != NULL) {
        n = n->left;
    }

    return n;
}

static rbnode *rbnode_max(rbnode *n) {
    assert(n);

    while (n->right != NULL) {
        n = n->right;
    }

    return n;
}

static int rbnode_size(rbnode *n) {
    if (n == NULL) {
        return 0;
    } else {
        return 1 + rbnode_size(n->left) + rbnode_size(n->right);
    }
}

static int rbnode_height(rbnode *n) {
    if (n == NULL) {
        return -1;
    } else {
        return 1 + rbnode_level_compare(rbnode_height(n->left),
                                        rbnode_height(n->right));
    }
}

static int rbnode_level_compare(int x, int y) { return x >= y ? x : y; }

static int rbnode_leafct(rbnode *n) {
    if (n == NULL) {
        return 0;
    } else if (n->left == NULL && n->right == NULL) {
        return 1;
    } else {
        return rbnode_leafct(n->left) + rbnode_leafct(n->right);
    }
}

static rbnode *rbnode_successor(rbnode *n) {
    assert(n);
    return rbnode_min(n->right);
}

static rbnode *rbnode_predecessor(rbnode *n) {
    assert(n);
    return rbnode_max(n->left);
}

static void rbnode_inorder_recursive(rbnode **n, consumer_fn consumer) {
    assert(n);
    assert(consumer);

    if ((*n) != NULL) {
        rbnode_inorder_recursive(&(*n)->left, consumer);
        consumer(n);
        rbnode_inorder_recursive(&(*n)->right, consumer);
    }
}

static void rbnode_preorder_recursive(rbnode **n, consumer_fn consumer) {
    assert(n);
    assert(consumer);

    if ((*n) != NULL) {
        consumer(n);
        rbnode_preorder_recursive(&(*n)->left, consumer);
        rbnode_preorder_recursive(&(*n)->right, consumer);
    }
}

static void rbnode_postorder_recursive(rbnode **n, consumer_fn consumer) {
    assert(n);
    assert(consumer);

    if ((*n) != NULL) {
        rbnode_postorder_recursive(&(*n)->left, consumer);
        rbnode_postorder_recursive(&(*n)->right, consumer);
        consumer(n);
    }
}

static void rbnode_levelorder_recursive(rbnode **n, consumer_fn consumer) {
    assert(n);
    assert(consumer);

    int height = rbnode_height((*n));
    for (int i = 1; i < height; i++) {
        rbnode_levelorder_helper(&(*n), consumer, i);
    }
}

static void rbnode_levelorder_helper(rbnode **n, consumer_fn consumer,
                                     int level) {
    if ((*n) == NULL) {
        return;
    } else if (level == 1) {
        consumer(n);
    } else if (level > 1) {
        rbnode_levelorder_helper(&(*n)->left, consumer, level - 1);
        rbnode_levelorder_helper(&(*n)->right, consumer, level - 1);
    }
}

static rbnode *rbnode_rotate_left(rbnode *n) {
    assert(n);

    rbnode *x = NULL;

    x = n->right;
    n->right = x->left;
    x->left = n;

    x->color = n->color;
    n->color = RBNODE_RED;

    return x;
}

static rbnode *rbnode_rotate_right(rbnode *n) {
    assert(n);

    rbnode *x = NULL;

    x = n->left;
    n->left = x->right;
    x->right = n;

    x->color = n->color;
    n->color = RBNODE_RED;

    return x;
}

static void rbnode_color_flip(rbnode *n) {
    if (n != NULL) {
        // if n != NULL, toggle n's colors/n's children's colors
        n->color = !n->color;

        if (n->left != NULL) {
            n->left->color = !(n->left->color);
        }

        if (n->right != NULL) {
            n->right->color = !(n->right->color);
        }
    }
}

static rbnode *rbnode_insert(rbnode *n, int val) {
    if (n == NULL) {
        // BASE CASE: n is a leaf, return new node
        return rbnode_new(val);
    }

    if (rbnode_is_red(n->left) && rbnode_is_red(n->right)) {
        // if n has red children
        rbnode_color_flip(n);
    }

    int compare = n->data - val;

    if (compare == 0) {
        // BASE CASE: will fall through to run remainder
        // of code and return n (no duplicates allowed)
    } else if (compare > 0) {
        // RECURSIVE CASE 1: val < n->data
        // go to left child and return result of recursive call to it
        n->left = rbnode_insert(n->left, val);
    } else {
        // RECURSIVE CASE 2: val > n->data
        // go to right child and return result of recursive call to it
        n->right = rbnode_insert(n->right, val);
    }

    if (rbnode_is_red(n->right)) {
        // if n's right child is red
        // rotate left to fix right-leaning red nodes
        n = rbnode_rotate_left(n);
    }

    if (rbnode_is_red(n->left) && rbnode_is_red((n->left)->left)) {
        // if n's left child is red
        // if n's left grandchild is red
        // rotate right to fix left-leaning double-red nodes
        n = rbnode_rotate_right(n);
    }

    return n;
}

static rbnode *rbnode_move_red_right(rbnode *n) {
    // start by toggling colors
    rbnode_color_flip(n);

    if (rbnode_is_red(n->left->left)) {
        // if n's left grandchild is red
        n = rbnode_rotate_right(n);
        rbnode_color_flip(n);
    }

    return n;
}

static rbnode *rbnode_move_red_left(rbnode *n) {
    // start by toggling colors
    rbnode_color_flip(n);

    if (rbnode_is_red(n->right->left)) {
        // if n's right grandchild is red
        n->right = rbnode_rotate_right(n->right);
        n = rbnode_rotate_left(n);

        rbnode_color_flip(n);
    }

    return n;
}

static rbnode *rbnode_erase_min(rbnode *n) {
    if (n->left == NULL) {
        // BASE CASE: n->left is a leaf, min node found.
        rbnode_delete(&n);
        return NULL;
    }

    if (!rbnode_is_red(n->left) && !rbnode_is_red((n->left)->left)) {
        // n's left child is black,
        // n's left grandchild is black
        n = rbnode_move_red_left(n);
    }

    // RECURSIVE CASE: left leaf node not found yet
    n->left = rbnode_erase_min(n->left);

    return rbnode_fixup(n);
}

static rbnode *rbnode_erase_max(rbnode *n) {
    if (rbnode_is_red(n->left)) {
        // if n's left child is red,
        // rotate right at n
        n = rbnode_rotate_right(n);
    }

    if (n->right == NULL) {
        // BASE CASE: n->right is a leaf, max node found
        rbnode_delete(&n);
        return NULL;
    }

    if (!rbnode_is_red(n->right) && !(rbnode_is_red((n->right)->left))) {
        // n's right child is black,
        // n's right grandchild is black
        n = rbnode_move_red_right(n);
    }

    // RECURSIVE CASE: right leaf node not found yet
    n->right = rbnode_erase_max(n->right);

    return rbnode_fixup(n);
}

static rbnode *rbnode_erase(rbnode *n, int val) {
    int compare = n->data - val;

    if (compare > 0) {
        // val is less than n->data
        if (!rbnode_is_red(n->left) && !rbnode_is_red((n->left)->left)) {
            // if n's left child is black
            // if n's left grandchild is black
            n = rbnode_move_red_left(n);
        }

        // RECURSIVE CASE
        n->left = rbnode_erase(n->left, val);
    } else {
        // val is greater than or equal to n->data
        if (rbnode_is_red(n->left)) {
            // if n's left child is red
            n = rbnode_rotate_right(n);
        }

        if (compare == 0 && n->right == NULL) {
            // RECURSIVE CASE: n is the node to erase,
            // and has no right child
            printf("[erasing val %d]\n", n->data);
            rbnode_delete(&n);
            return NULL;
        }

        if (!rbnode_is_red(n->right) && !rbnode_is_red((n->right)->left)) {
            // if n's right child is black
            // if n's left grandchild from the right child is black
            n = rbnode_move_red_right(n);
        }

        if (compare == 0) {
            // BASE CASE: if n is the node to delete,
            // but has a right child -- will replace n's data with that of its
            // inorder successor,  then delete the inorder successor node
            n->data = rbnode_successor(n)->data;
            n->right = rbnode_erase_min(n->right);
        } else {
            // RECURSIVE CASE: n is not the node to delete
            // proceed to right child, since val is greater than n->data
            n->right = rbnode_erase(n->right, val);
        }
    }

    return rbnode_fixup(n);
}

static rbnode *rbnode_fixup(rbnode *n) {
    if (rbnode_is_red(n->right)) {
        // if n's right child is red
        n = rbnode_rotate_left(n);
    }

    if (rbnode_is_red(n->left) && rbnode_is_red((n->left)->left)) {
        // if n's left child is red
        // if n's left grandchild is red
        n = rbnode_rotate_right(n);
    }

    if (rbnode_is_red(n->left) && rbnode_is_red(n->right)) {
        // if n has red children
        rbnode_color_flip(n);
    }

    return n;
}

static void rbnode_fputs(rbnode *n, FILE *dest, char *b, bool last) {
    /**
     *  Not a good long-term solution,
     *  as this can overflow with large data sets.
     *  Looks "pretty" though.
     */

    char newbuf[RBTREE_BUFSZ];
    strcpy(newbuf, b);

    fprintf(dest, "%s", b);

    if (last) {
        fprintf(dest, "R----");
        strcat(newbuf, "      ");
    } else {
        fprintf(dest, "L----");
        strcat(newbuf, "|     ");
    }

    const char *color = rbnode_is_red(n) ? "RED" : "BLACK";

    if (n != NULL) {
        // with address
        // fprintf(dest, "[%d] (%s) (%p)\n", n->data, color, n);

        fprintf(dest, "[%d] (%s)\n", n->data, color);

        rbnode_fputs(n->left, dest, newbuf, false);
        rbnode_fputs(n->right, dest, newbuf, true);
    } else {
        // with address
        // fprintf(dest, "[#] (nil) (%p)\n", NULL);
        fprintf(dest, "[#] (nil)\n");
    }
}

static rbtree *rbtree_allocate(void) {
    rbtree *t = NULL;
    t = malloc(sizeof((*t)));
    assert(t);
    return t;
}

static void rbtree_init(rbtree *t) {
    assert(t);
    t->root = NULL;
}

static void rbtree_deinit(rbtree *t) {
    assert(t);
    rbtree_clear(t);
}
