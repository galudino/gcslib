/**
 *  @file       bstree.c
 *  @brief      Source file for a binary search tree data structure, type int
 *
 *  @author     Gemuele (Gem) Aludino
 *  @date       23 Jul 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

/**
 *  [Algorithm credits]
 *  bstree_find, bstree_min, bstree_max, bstree_insert, bstree_erase,
 * bsnode_predecessor, bsnode_successor)
 *      http://staff.ustc.edu.cn/~csli/graduate/algorithms/book6/chap13.htm
 *
 * [bstree_puts credits]
 *      https://github.com/Bibeknam/algorithmtutorprograms/blob/master/data
 * structures/red-black-trees/RedBlackTree.cpp
 */

#include "bstree.h"

#define BSTREE_BUFSZ 4096

struct bstree {
    bsnode *root;
};

static bsnode *bsnode_allocate(void);
static void bsnode_init(bsnode *n, int val);
static void bsnode_deinit(bsnode *n);

static bsnode *bsnode_new(int val);
static void bsnode_delete(void *arg);

static void bsnode_copytree_recursive(bsnode **n, bsnode *other, bsnode *parent);

static bsnode *bsnode_find(bsnode *n, int val);
static bsnode *bsnode_min(bsnode *n);
static bsnode *bsnode_max(bsnode *n);

static int bsnode_size(bsnode *n);
static int bsnode_height(bsnode *n);
static int bsnode_level_compare(int x, int y);
static int bsnode_leafct(bsnode *n);

static bsnode *bsnode_successor(bsnode *n);
static bsnode *bsnode_predecessor(bsnode *n);

static void bsnode_inorder_recursive(bsnode **n, void (*consumer)(void *));
static void bsnode_preorder_recursive(bsnode **n, void (*consumer)(void *));
static void bsnode_postorder_recursive(bsnode **n, void (*consumer)(void *));
static void bsnode_levelorder_recursive(bsnode **n, void (*consumer)(void *));
static void bsnode_levelorder_helper(bsnode **n, void (*consumer)(void *),
                                     int level);

static void bsnode_fputs(bsnode *root, FILE *dest, char *b, bool last);

static bstree *bstree_allocate(void);
static void bstree_init(bstree *t);
static void bstree_deinit(bstree *t);

static void bstree_swap_nodes(bstree *t, bsnode **u, bsnode **v);

void bsnode_int_puts(void *arg) {
    bsnode **n = (bsnode **)(arg);
    printf("%d\n", (*n)->data);
}

void bsnode_int_puts_nobrk(void *arg) {
    bsnode **n = (bsnode **)(arg);
    printf(" %d ", (*n)->data);
}

bstree *bstree_new(void) {
    bstree *t = bstree_allocate();
    bstree_init(t);
    return t;
}

bstree *bstree_newcopy(bstree *t) {
    assert(t);

    bstree *copy = bstree_new();
    // TODO iterative version
    bsnode_copytree_recursive(&(copy->root), t->root, NULL);

    return copy;
}

void bstree_delete(void *arg) {
    assert(arg);

    bstree **t = (bstree **)(arg);
    bstree_deinit((*t));

    free((*t));
    (*t) = NULL;
}

int *bstree_find(bstree *t, int val) {
    assert(t);

    bsnode *result = bsnode_find(t->root, val);
    return result ? &result->data : NULL;
}

int *bstree_front(bstree *t) {
    assert(t);
    return &t->root->data;
}

int *bstree_min(bstree *t) {
    assert(t);

    bsnode *min = bsnode_min(t->root);
    return min ? &min->data : NULL;
}

int *bstree_max(bstree *t) {
    assert(t);

    bsnode *max = bsnode_max(t->root);
    return max ? &max->data : NULL;
}

bsnode **bstree_data(bstree *t) { return t->root ? &t->root : NULL; }

int bstree_size(bstree *t) {
    assert(t);
    // TODO iterative version
    return bsnode_size(t->root);
}

int bstree_height(bstree *t) {
    assert(t);
    return bsnode_height(t->root);
}

int bstree_leafct(bstree *t) {
    assert(t);
    return bsnode_leafct(t->root);
}

bool bstree_empty(bstree *t) {
    assert(t);
    return t->root == NULL;
}

void bstree_insert(bstree *t, int val) {
    /*
    TREE-INSERT(T,z)

    1  y <- NIL
    2  x <- root[T]
    3  while x != NIL
    4       do y <- x
    5          if key[z] < key[x]
    6             then x <- left[x]
    7             else x <- right[x]
    8  p[z] <- y
    9  if y = NIL
    10      then root[T] <- z
    11      else if key[z] < key[y]
    12              then left[y] <- z
    13              else right[y] <- z
    */
    assert(t);

    bsnode *x = t->root;
    bsnode *y = NULL;       // make NULL t->SENTINEL

    int compare = -1;

    // while x != t->SENTINEL
    while (x != NULL) {
        // node y will trail behind the cursor node, x, until
        // a suitable insertion location is found. y will be the node to link to
        // when the new node is created -- it will be the new node's parent.
        y = x;

        // the comparison function will assess
        // compare(x->data, val) here.
        compare = x->data - val;

        if (compare == 0) {
            // no duplicates entries will be allowed for this implementation.
            printf("[insert failed - %d already exists.]\n", val);
            return;
        } else if (compare > 0) {
            // proceed to the root of the left subtree
            x = x->left;
        } else if (compare <= 0) {
            // proceed to the root of the right subtree
            x = x->right;
        }
    }

    // creating the new node now, and linking it in to its parent, y.
    bsnode *z = bsnode_new(val);
    z->parent = y;

    // keep if y == NULL
    if (y == NULL) {
        // z is the first node to be added, and has no parent.
        t->root = z;
    } else {
        // the comparison function will assess
        // compare(y->data, val) here.
        compare = y->data - val;

        if (compare > 0) {
            // the new node is a left child
            y->left = z;
        } else if (compare <= 0) {
            // the new node is a right child
            y->right = z;
        }
    }

    // rbt operations begin here:
    /*
    if (z->parent == t->TNULL) {
        // if we have added the first node of the tree, make it black
        z->color = BLACK;
        return;
    }

    if (node->parent->parent != t->TNULL) {
        z->color = RED;
        rbtree_insert_fix(t, z);
    }
    */
}


void bstree_erase(bstree *t, int val) {
    /*
    TREE-DELETE(T, z)

    1  if left[z] = NIL or right[z] = NIL
    2      then y <- z
    3      else y <- TREE-SUCCESSOR(z)
    4  if left[y] != NIL
    5      then x <- left[y]
    6      else x <- right[y]
    7  if x != NIL
    8      then p[x] <- p[y]
    9  if p[y] = NIL
    10      then root[T] <- x
    11      else if y = left[p[y]]
    12              then left[p[y]] <- x
    13              else right[p[y]] <- x
    14  if y !=z
    15      then key[z] <- key[y]
    16            // if y has other fields, copy them, too
    */
    assert(t);

    bsnode *x = NULL;   // NULL is t->SENTINEL for rbt
    bsnode *y = NULL;   // NULL is t->SENTINEL for rbt
    bsnode *z = bsnode_find(t->root, val);

    // if z == t->SENTINEL for rbt
    if (z == NULL) {
        printf("[erase failed - %d not found in tree.]\n", val);
        return;
    } else {
        printf("[node with val %d found. attempting to erase...]\n", val);
    }

    // if z has two children, set y to be z's inorder successor
    // else, set y to z.
    y = z->left && z->right ? bsnode_successor(z) : z;

    // if y has a left child, set x to be y's left child
    // else, set x to y's right child
    x = y->left ? y->left : y->right;

    // don't need to check if x is null for rbt
    if (x != NULL) {
        // if y was assigned to be z's successor (min of z's right subtree)
        // x would have been assigned y->left, or y->right, which is null.

        // if y was assigned to z (the node to delete)
        // x will be assigned y's left or right child (which is z's children)
        // x's grandparent, which is y's parent (really, z's parent)
        // will become x's parent.
        x->parent = y->parent;
    }

    // if y->parent == t->SENTINEL
    if (y->parent == NULL) {
        // y->parent is null if y is the root of the tree.
        t->root = x;
    } else {
        // (the following description applies if x exists)
        // (y was set equal to z)

        // if y's parent exists (meaning y's parent is not null)
        // y's parent, which is x's grandparent,
        // will now link in with its grandchild, x. (y/z's left or right child)

        // (the following description applies if x does not exist)
        // (y was set equal to z's inorder successor)

        // y, being a successor to z, will be a surviving leaf node
        // and thus must be assigned the value of x,
        // which in this case, ended up being null.
        if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
    }

    if (y != z) {
        // if y was never assigned to z,
        // y was instead assigned to z's inorder successor.

        // in this case, y's data will become z's data -
        // (a transfer of ownership from one node to the other)

        z->data = y->data;
    }

    // now that the surviving nodes are linked together,
    // it is now safe to delete y.
    bsnode_delete(&y);

    printf("[node with val %d deleted.]\n", val);

    // if y->color == black then do rbtree_erase_fix(T, x)
}

void bstree_clear(bstree *t) {
    assert(t);
    bsnode_postorder_recursive(&t->root, bsnode_delete);
}

void bstree_foreach(bstree *t, void (*consumer)(void *),
                    enum bstree_traversal_type ttype) {
    assert(t);

    switch (ttype) {
    case INORDER:
        bsnode_inorder_recursive(&t->root, consumer);
        // TODO iterative version
        break;

    case PREORDER:
        bsnode_preorder_recursive(&t->root, consumer);
        // TODO iterative version
        break;

    case POSTORDER:
        bsnode_postorder_recursive(&t->root, consumer);
        // TODO iterative version
        break;

    case LEVELORDER:
        bsnode_levelorder_recursive(&t->root, consumer);
        // TODO iterative version
        break;
    }
}

void bstree_puts(bstree *t) {
    assert(t);
    bstree_fputs(t, stdout);
}

void bstree_fputs(bstree *t, FILE *dest) {
    assert(t);
    assert(dest);

    const char *link = "---------------------------";

    char buffer1[128];
    char buffer2[BSTREE_BUFSZ];

    sprintf(buffer1, "\n%s\n%s\n%s\n", link, "Binary Search Tree Elements", link);

    sprintf(buffer2,
    "%s\n%s\t\t%d\n%s\t\t%d\n%s\t\t%d\n\n%s\t\t%d\n%s\t\t%d\n%s\t\t%d\n%s\n",
    link,
    "Minimum value", *bstree_min(t),
    "Maximum value", *bstree_max(t),
    "Root value   ", *bstree_front(t),
    "Size         ", bsnode_size(t->root),
    "Height       ", bsnode_height(t->root),
    "Leaf ct.     ", bsnode_leafct(t->root),
    link);

    fprintf(dest, "%s", buffer1);

    if (t->root) {
        fprintf(dest, "\n");
        bsnode_fputs(t->root, dest, "", true);
        fprintf(dest, "\n");
    } else {
        fprintf(dest, "\n[ empty tree ]\n\n");
    }
}

static bsnode *bsnode_allocate(void) {
    bsnode *n = NULL;
    n = malloc(sizeof(*n));
    assert(n);
    return n;
}

static void bsnode_init(bsnode *n, int val) {
    assert(n);

    n->data = val;

    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
}

static void bsnode_deinit(bsnode *n) {
    assert(n);
    // destroy (*n)->data if applicable

    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
}

static bsnode *bsnode_new(int val) {
    bsnode *n = bsnode_allocate();
    bsnode_init(n, val);
    return n;
}

static void bsnode_delete(void *arg) {
    assert(arg);

    bsnode **n = (bsnode **)(arg);

    bsnode_deinit((*n));

    free((*n));
    (*n) = NULL;
}

static void bsnode_copytree_recursive(bsnode **n, bsnode *other, bsnode *parent) {
    if (other == NULL) {
        n = NULL;
    } else {
        (*n) = bsnode_new(other->data);
        (*n)->parent = parent ? parent : NULL;
        parent = (*n);

        bsnode_copytree_recursive(&(*n)->left, other->left, parent);
        bsnode_copytree_recursive(&(*n)->right, other->right, parent);
    }
}

static bsnode *bsnode_find(bsnode *n, int val) {
    /*
    TREE_FIND(x, k)

    1  while x != NIL and k != key[x]
    2      do if k < key[x]
    3            then x <- left[x]
    4            else x <- right[x]
    5  return x
    */

    while (n != NULL) {
        // the comparison function will assess
        // compare(n->data, val) here.
        int compare = n->data - val;

        if (compare == 0) {
            // the desired node was found.
            break;
        } else if (compare > 0) {
            // proceed to left subtree
            n = n->left;
        } else if (compare <= 0) {
            // proceed to right subtree
            n = n->right;
        }
    }

    return n;
}

static bsnode *bsnode_min(bsnode *n) {
    /*
    TREE-MINIMUM(x)

    1  while left[x] != NIL
    2      do x <- left[x]
    3  return x
    */

    assert(n);

    while (n->left != NULL) {
        // traverse leftward until a null left child is reached
        n = n->left;
    }

    return n;
}

static bsnode *bsnode_max(bsnode *n) {
    /*
    TREE-MAXIMUM(x)

    1  while right[x] != NIL
    2      do x <- right[x]
    3  return x
    */

    assert(n);

    while (n->right != NULL) {
        // traverse rightward until a null right child is reached
        n = n->right;
    }

    return n;
}

static int bsnode_height(bsnode *n) {
    if (n == NULL) {
        return -1;
    } else {
        return 1 + bsnode_level_compare(bsnode_height(n->left),
                                        bsnode_height(n->right));
    }
}

static int bsnode_level_compare(int x, int y) { return x >= y ? x : y; }

static int bsnode_leafct(bsnode *n) {
    if (n == NULL) {
        return 0;
    } else if (n->left == NULL && n->right == NULL) {
        return 1;
    } else {
        return bsnode_leafct(n->left) + bsnode_leafct(n->right);
    }
}

static int bsnode_size(bsnode *n) {
    if (n == NULL) {
        return 0;
    } else {
        return 1 + bsnode_size(n->left) + bsnode_size(n->right);
    }
}

static bsnode *bsnode_predecessor(bsnode *n) {
    /*
    TREE-PREDECESSOR(x)

    1  if left[x] != NIL
    2      then return TREE-MAXIMUM(left[x])
    3  y <- p[x]
    4  while y != NIL and x = left[y]
    5       do x <- y
    6          y <- p[y]
    7  return y
    */

    assert(n);

    if (n->left != NULL) {
        return bsnode_max(n->left);
    }

    bsnode *y = n->parent;

    while (y != NULL && n == y->left) {
        n = y;
        y = y->parent;
    }

    return y;
}

static bsnode *bsnode_successor(bsnode *n) {
    /*
    TREE-SUCCESSOR(x)

    1  if right[x] != NIL
    2      then return TREE-MINIMUM(right[x])
    3  y <- p[x]
    4  while y != NIL and x = right[y]
    5       do x <- y
    6          y <- p[y]
    7  return y
    */

    assert(n);

    if (n->right != NULL) {
        return bsnode_min(n->right);
    }

    bsnode *y = n->parent;

    while (y != NULL && n == y->right) {
        n = y;
        y = y->parent;
    }

    return y;
}

static void bsnode_inorder_recursive(bsnode **n, void (*consumer)(void *)) {
    assert(n);
    assert(consumer);

    if ((*n) != NULL) {
        bsnode_inorder_recursive(&(*n)->left, consumer);
        consumer(n);
        bsnode_inorder_recursive(&(*n)->right, consumer);
    }
}

static void bsnode_preorder_recursive(bsnode **n, void (*consumer)(void *)) {
    assert(n);
    assert(consumer);

    if ((*n) != NULL) {
        consumer(n);
        bsnode_preorder_recursive(&(*n)->left, consumer);
        bsnode_preorder_recursive(&(*n)->right, consumer);
    }
}

static void bsnode_postorder_recursive(bsnode **n, void (*consumer)(void *)) {
    assert(n);
    assert(consumer);

    if ((*n) != NULL) {
        bsnode_postorder_recursive(&(*n)->left, consumer);
        bsnode_postorder_recursive(&(*n)->right, consumer);
        consumer(n);
    }
}

static void bsnode_levelorder_recursive(bsnode **n, void (*consumer)(void *)) {
    assert(n);
    assert(consumer);

    int height = bsnode_height((*n));
    for (int i = 1; i < height; i++) {
        bsnode_levelorder_helper(n, consumer, i);
    }
}

static void bsnode_levelorder_helper(bsnode **n, void (*consumer)(void *),
                                     int level) {
    if (n == NULL) {
        return;
    } else if (level == 1) {
        consumer(n);
    } else if (level > 1) {
        bsnode_levelorder_helper(&(*n)->left, consumer, level - 1);
        bsnode_levelorder_helper(&(*n)->right, consumer, level - 1);
    }
}

static void bsnode_fputs(bsnode *root, FILE *dest, char *b, bool last) {
    /**
     *  Not a good long-term solution,
     *  as this can overflow with large data sets.
     *  Looks "pretty" though.
     */
    if (root != NULL) {
        fprintf(dest, "%s", b);

        char newbuf[BSTREE_BUFSZ];
        strcpy(newbuf, b);

        if (last) {
            fprintf(dest, "R----");
            strcat(newbuf, "      ");
        } else {
            fprintf(dest, "L----");
            strcat(newbuf, "|     ");
        }

        fprintf(dest, "[%d]\n", root->data);

        bsnode_fputs(root->left, dest, newbuf, false);
        bsnode_fputs(root->right, dest, newbuf, true);
    }
}

static bstree *bstree_allocate(void) {
    bstree *t = NULL;
    t = malloc(sizeof((*t)));
    assert(t);
    return t;
}

static void bstree_init(bstree *t) {
    assert(t);
    t->root = NULL;
}

static void bstree_deinit(bstree *t) {
    assert(t);
    bstree_clear(t);
}

static void bstree_swap_nodes(bstree *t, bsnode **u, bsnode **v) {
    if ((*u)->parent == NULL) {
        t->root = (*v);
    } else if ((*u) == (*u)->parent->left) {
        (*u)->parent->left = (*v);
    } else {
        (*u)->parent->right = (*v);
    }

    (*v)->parent = (*u)->parent;
}
