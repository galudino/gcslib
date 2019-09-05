/**
 *  @file       mergesort.c
 *  @brief      Source file for a mergesort algorithm
 *
 *  @author     Gemuele Aludino
 *  @date       21 Jul 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */
/**
 *  Copyright © 2019 Gemuele Aludino
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included
 *  in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
 *  THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "mergesort.h"
#include "vector.h"

/**< slist and list in the works */
/* #include "slist.h" */
/* #include "list.h" */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static int v_mergesort_min(int x, int y);
static void v_mergesort_iterative_merge(void *arr,
                                        int l, int m, int r,
                                        size_t width,
                                        int (*compare)(const void *, const void *));

/* slist and list in the works
//static dlnode *dn_mergesort_recursive_merge(dlnode *a, dlnode *b, int (*compare)(const void *, const void *));
//static void dn_mergesort_recursive_split(dlnode *head, dlnode **tail, dlnode **a, dlnode **b);

//static void sn_mergesort_iterative_swap(void *a, void *b);
//static int sn_mergesort_iterative_length(slnode *curr);
//static void sn_mergesort_iterative_merge(slnode **start1, slnode **end1, slnode **start2, slnode **end2, int (*compare)(const void *, const void *));
*/

static int v_mergesort_min(int x, int y) {
    return (x < y) ? x : y;
}

static void v_mergesort_iterative_merge(void *arr, int l, int m, int r,
                                 size_t width,
                                 int (*compare)(const void *, const void *)) {
    int i = -1;
    int j = -1;
    int k = -1;

    int n1 = m - l + 1;
    int n2 = r - m;

    #if __STD_VERSION__ >= 199901L
    void **L[n1];
    void **R[n2];
    #else
    void *L = NULL;
    void *R = NULL;

    L = calloc(n1, width);
    assert(L);

    R = calloc(n2, width);
    assert(R);
    #endif

    for (i = 0; i < n1; i++) {
        /**
         *  ADDR_AT(pointer, index, width) expands to:
         *  (((char *)(pointer)) + ((index) * (width)))
         */
        void *L_i = ADDR_AT(L, i, width);
        void *arr_l_plus_i = ADDR_AT(arr, l + i, width);

        memcpy(L_i, arr_l_plus_i, width);
    }

    for (j = 0; j < n2; j++) {
        void *R_j = ADDR_AT(R, j, width);
        void *arr_m_plus_j_plus_one = ADDR_AT(arr, m + j + 1, width);

        memcpy(R_j, arr_m_plus_j_plus_one, width);
    }

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
        void *L_i = ADDR_AT(L, i, width);
        void *R_j = ADDR_AT(R, j, width);

        void *arr_k = ADDR_AT(arr, k, width);

        if (compare(L_i, R_j) <= 0) {
            memcpy(arr_k, L_i, width);
            i++;
        } else {
            memcpy(arr_k, R_j, width);
            j++;
        }

        k++;
    }

    while (i < n1) {
        void *arr_k = ADDR_AT(arr, k++, width);
        void *L_i = ADDR_AT(L, i++, width);

        memcpy(arr_k, L_i, width);
    }

    while (j < n2) {
        void *arr_k = ADDR_AT(arr, k++, width);
        void *R_j = ADDR_AT(R, j++, width);

        memcpy(arr_k, R_j, width);
    }
}

void v_mergesort_iterative(void *arr,
                           size_t n, size_t width,
                           int (*compare)(const void *, const void *)) {
    int curr_size = -1;

    int left_start = -1;

    for (curr_size = 1; curr_size <= (n - 1); curr_size *= 2) {

        for (left_start = 0; left_start < (n - 1);
             left_start += (curr_size * 2)) {

            int mid =
            v_mergesort_min(left_start + curr_size - 1, (int)(n) - 1);

            int right_end =
            v_mergesort_min(left_start + curr_size * 2 - 1, (int)(n) - 1);

            v_mergesort_iterative_merge(arr,
                                        left_start, mid, right_end,
                                        width, compare);
        }
    }
}

/*
static dlnode *dn_mergesort_recursive_merge(dlnode *a, dlnode *b,
                                     int (*compare)(const void *, const void *)) {
    if (a == NULL) {
        return b;
    }

    if (b == NULL) {
        return a;
    }

    if (compare(a->valaddr, b->valaddr) <= 0) {
        a->next = dn_mergesort_recursive_merge(a->next, b, compare);

        a->next->prev = a;
        a->prev = NULL;

        return a;
    } else {
        b->next = dn_mergesort_recursive_merge(a, b->next, compare);

        b->next->prev = b;
        b->prev = NULL;

        return b;
    }
}

static void dn_mergesort_recursive_split(dlnode *head, dlnode **tail, dlnode **a, dlnode **b) {
    dlnode *slow = head;
    dlnode *fast = head->next;

    while (fast != NULL) {
        fast = fast->next;

        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *b = slow->next;
    slow->next = NULL;
}

void dn_mergesort_recursive(dlnode **head, dlnode **tail, int (*compare)(const void *, const void *)) {
    if ((*head) == NULL || (*head)->next == NULL) {
        return;
    }

    dlnode *a = (*head);
    dlnode *b = NULL;

    dn_mergesort_recursive_split((*head), tail, &a, &b);

    dn_mergesort_recursive(&a, NULL, compare);
    dn_mergesort_recursive(&b, NULL, compare);

    *head = dn_mergesort_recursive_merge(a, b, compare);
}

void dn_mergesort_iterative(dlnode **head, int (*compare)(const void *, const void *)) {

}

static void sn_mergesort_iterative_swap(void *a, void *b) {
    slnode **first = (slnode **)(a);
    slnode **second = (slnode **)(b);

    slnode *temp = *first;
    (*first) = (*second);
    (*second) = temp;
}

static int sn_mergesort_iterative_length(slnode *curr) {
    int count = 0;
    while (curr != NULL) {
        curr = curr->next;
        ++count;
    }

    return count;
}

static void sn_mergesort_iterative_merge(slnode **start1, slnode **end1,
                                  slnode **start2, slnode **end2,
                                  int (*compare)(const void *, const void *)) {
    if (compare((*start1)->valaddr, (*start2)->valaddr) > 0) {
        sn_mergesort_iterative_swap(start1, start2);
        sn_mergesort_iterative_swap(end1, end2);
    }

    slnode *astart = *start1;
    slnode *aend = *end1;

    slnode *bstart = *start2;
    //slnode *bend = *end2;

    slnode *bendnext = (*end2)->next;

    while (astart != aend && bstart != bendnext) {
        if (astart->next->valaddr > bstart->valaddr) {
            slnode *temp = bstart->next;

            bstart->next = astart->next;
            astart->next = bstart;

            bstart = temp;
        }

        astart = astart->next;
    }

    if (astart == aend) {
        astart->next = bstart;
    } else {
        (*end2) = (*end1);
    }
}

void sn_mergesort_iterative(void *arg_head, int (*compare)(const void *, const void *)) {
    slnode **head = (slnode **)(arg_head);

    if (*head == NULL) {
        return;
    }

    slnode *start1 = NULL;
    slnode *end1 = NULL;

    slnode *start2 = NULL;
    slnode *end2 = NULL;

    slnode *prevend = NULL;

    int length = sn_mergesort_iterative_length(*head);

    for (int gap = 1; gap < length; gap = gap * 2) {
        start1 = (*head);

        while (start1) {
            bool first_iter = 0;

            if (start1 == *head) {
                first_iter = 1;
            }

            int counter = gap;
            end1 = start1;

            while (--counter && end1->next) {
                end1 = end1->next;
            }

            start2 = end1->next;

            if (!start2) {
                break;
            }

            counter = gap;
            end2 = start2;

            while (--counter && end2->next) {
                end2 = end2->next;
            }

            slnode *temp = end2->next;

            sn_mergesort_iterative_merge(&start1, &end1, &start2, &end2, compare);

            if (first_iter) {
                (*head) = start1;
            } else {
                prevend->next = start1;
            }

            prevend = end2;
            start1 = temp;
        }

        prevend->next = start1;
    }
}

void sn_mergesort_recursive(void *arg_head, int (*compare)(const void *, const void *)) {
    slnode **head = (slnode **)(arg_head);
}
*/
