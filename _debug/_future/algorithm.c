/**
 *  @file       algorithm.c
 *  @brief      Source file for container utility functions
 *
 *  @author     Gemuele Aludino
 *  @date       23 Jun 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

#include "utils.h"
#include "algorithm.h"
#include "iterator.h"
#include "pair.h"

bool all_of(iterator *first, iterator *last,
            bool (*unary_predicate)(const void *)) {
    //
    return false;
}

bool any_of(iterator *first, iterator *last,
            bool (*unary_predicate)(const void *)) {
    //
    return false;
}

bool none_of(iterator *first, iterator *last,
             bool (*unary_predicate)(const void *)) {
    //
    return false;
}

void for_each(iterator *first, iterator *last, void (*consumer)(const void *)) {

}

iterator *find(iterator *first, iterator *last, const void *valaddr) {

    //return NULL;
    assert(first);
    assert(last);
    assert(valaddr);

    struct typetable *ttbl_first = it_get_ttbl(first);
    //struct typetable *ttbl_last = it_get_ttbl(last);

    //struct iterator_table *itbl_first = it_get_itbl(first);
    //struct iterator_table *itbl_last = it_get_itbl(last);

    int distance = it_distance(first, last);
    bool found = false;

    for (int i = 0; i < distance; i++) {
        void *next = it_next(first);

        if (ttbl_first->compare(next, valaddr) == 0) {
            found = true;
            break;
        }
    }

    if (found) {
        it_advance(first, -1);
    } else {
        it_advance(first, (-1 * distance));
    }

    return first;
}

iterator *find_if(iterator *first, iterator *last,
                  bool (*unary_predicate)(const void *)) {
    //
    return NULL;
}

iterator *find_if_not(iterator *first, iterator *last,
                      bool (*unary_predicate)(const void *)) {
    //
    return NULL;
}

iterator *find_end_equal(iterator *first1, iterator *last1,
                         iterator *first2, iterator *last2) {
    //
    return NULL;
}
iterator *find_end_if(iterator *first1, iterator *last1,
                      iterator *first2, iterator *last2,
                      bool (*unary_predicate)(const void *)) {
    //
    return NULL;
}

iterator *find_first_of_equal(iterator *first1, iterator *last1,
                              iterator *first2, iterator *last2) {
    //
    return NULL;
}
iterator *find_first_of_if(iterator *first1, iterator *last1,
                           iterator *first2, iterator *last2,
                           bool (*unary_predicate)(const void *)) {
    //
    return NULL;
}

iterator *adjacent_find_equal(iterator *first, iterator *last) {
    return NULL;
}

iterator *adjacent_find_if(iterator *first, iterator *last,
                           bool (*binary_predicate)(const void *, const void *)) {
    //
    return NULL;
}

iterator *count(iterator *first, iterator *last, const void *valaddr) {
    //
    return NULL;
}

iterator *count_if(iterator *first, iterator *last,
                   bool (*unary_predicate)(const void *)) {
    //
    return NULL;
}

pair *mismatch_equal(iterator *first1, iterator *last1, iterator *first2) {
    //
    return NULL;
}

pair *mismatch_predicate(iterator *first1, iterator *last1,
                         iterator *first2,
                         bool (*binary_predicate)(const void *, const void *)) {
    //
    return NULL;
}

bool equal_equal(iterator *first1, iterator *last1, iterator *first2) {
    //
    return false;
}

bool equal_predicate(iterator *first1, iterator *last2,
                     iterator *first2,
                     bool (*binary_predicate)(const void *, const void *)) {
    //
    return false;
}


iterator *search_equal(iterator *first1, iterator *last1,
                       iterator *first2, iterator *last2) {
    //
    return NULL;
}

iterator *search_predicate(iterator *first1, iterator *last1,
                           iterator *first2, iterator *last2,
                           bool (*binary_predicate)(const void *, const void *)) {
    //
    return NULL;
}

iterator *search_n_equal(iterator *first, iterator *last,
                         size_t count, const void *valaddr) {
    //
    return NULL;
}

iterator *search_n_predicate(iterator *first, iterator *last,
                             size_t count, const void *valaddr,
                             bool (*binary_predicate)(const void *, const void *)) {
    //
    return NULL;
}

bool is_partitioned(iterator *first, iterator *last,
                    bool (*unary_predicate)(const void *)) {
    //
    return false;
}

iterator *partition(iterator *first, iterator *last,
                    bool (*unary_predicate)(const void *)) {
    //
    return NULL;
}

// omitting stable_partition
// omitting partition_copy

iterator *partition_point(iterator *first, iterator *last,
                          bool (*predicate)(const void *)) {
    //
    return NULL;
}

void sort(iterator *first, iterator *last) {
    //
}
void sort_custom(iterator *first, iterator *last,
                 int (*compare)(const void *, const void *)) {
    //
}

void partial_sort(iterator *first, iterator *middle, iterator *last) {
    //
}

void partial_sort_custom(iterator *first, iterator *middle,
                         iterator *last,
                         int (*compare)(const void *, const void *)) {
    //
}

iterator *partial_sort_copy(iterator *first, iterator *last,
                            iterator *result_first, iterator *result_last) {
    //
    return NULL;
}

iterator *partial_sort_copy_custom(iterator *first, iterator *last,
                                   iterator *result_first,
                                   iterator *result_last,
                                   int (*compare)(const void *, const void *)) {
    //
    return NULL;
}

bool is_sorted(iterator *first, iterator *last) {
    return false;
}

bool is_sorted_custom(iterator *first, iterator *last,
                      int (*compare)(const void *, const void *)) {
    return false;
}

iterator *is_sorted_until(iterator *first, iterator *last) {

    return NULL;
}

iterator *is_sorted_until_custom(iterator *first, iterator *last,
                                 int (*compare)(const void *, const void *)) {

    return NULL;
}

void nth_element(iterator *first, iterator *nth, iterator *last) {
    //
}

void nth_element_custom(iterator *first, iterator *nth, iterator *last,
                        int (*compare)(const void *, const void *)) {
    //
}


iterator *lower_bound(iterator *first, iterator *last, const void *valaddr) {
    //
    return NULL;
}

iterator *lower_bound_custom(iterator *first, iterator *last,
                             const void *valaddr,
                             int (*compare)(const void *, const void *)) {
    //
    return NULL;
}

iterator *upper_bound(iterator *first, iterator *last, const void *valaddr) {
    //
    return NULL;
}

iterator *upper_bound_custom(iterator *first, iterator *last,
                             const void *valaddr,
                             int (*compare)(const void *, const void *)) {
    //
    return NULL;
}

pair *equal_range(iterator *first, iterator *last, const void *valaddr) {
    //
    return NULL;
}

pair *equal_range_custom(iterator *first, iterator *last,
                         const void *valaddr,
                         int (*compare)(const void *, const void *)) {
    //
    return NULL;
}

bool binary_search(iterator *first, iterator *last, const void *valaddr) {
    //
    return false;
}

bool binary_search_custom(iterator *first, iterator *last,
                          const void *valaddr,
                          int (*compare)(const void *, const void *)) {
    //
    return false;
}

iterator *merge(iterator *first1, iterator *last1,
                iterator *first2, iterator *last2,
                iterator *result) {
    //
    return NULL;
}

iterator *merge_custom(iterator *first1, iterator *last1,
                       iterator *first2, iterator *last2,
                       iterator *result,
                       int (*compare)(const void *, const void *)) {
    //
    return NULL;
}

void inplace_merge(iterator *first, iterator *middle, iterator *last) {
    //
}

void inplace_merge_custom(iterator *first, iterator *middle,
                          iterator *last,
                          int (*compare)(const void *, const void *)) {
    //
}

bool includes(iterator *first1, iterator *last1,
              iterator *first2, iterator *last2) {
    //
    return false;
}

bool includes_custom(iterator *first1, iterator *last1,
                     iterator *first2, iterator *last2,
                     int (*compare)(const void *, const void *)) {
    //
    return false;
}

// TODO set_union
// TODO set_intersection
// TODO set_difference
// TODO set_symmetric_difference

void push_heap(iterator *first, iterator *last) {
    //
}
void push_heap_custom(iterator *first, iterator *last,
                      int (*compare)(const void *, const void *)) {
    //
}

void pop_heap(iterator *first, iterator *last) {
    //
}

void pop_heap_custom(iterator *first, iterator *last,
                     int (*compare)(const void *, const void *)) {
    //
}

void make_heap(iterator *first, iterator *last) {
    //
}

void make_heap_custom(iterator *first, iterator *last,
                      int (*compare)(const void *, const void *)) {
    //
}

void sort_heap(iterator *first, iterator *last) {

}

void sort_heap_custom(iterator *first, iterator *last,
                      int (*compare)(const void *, const void *)) {
    //
}

bool is_heap(iterator *first, iterator *last) {
    //
    return false;
}

bool is_heap_custom(iterator *first, iterator *last,
                    int (*compare)(const void *, const void *)) {
    //
    return false;
}

const void *min(const void *a, const void *b) {
    //
    return NULL;
}
const void *min_custom(const void *a, const void *b,
                       int (*compare)(const void *, const void *)) {
    //
    return NULL;
}

const void *max(const void *a, const void *b) {
    //
    return NULL;
}

const void *max_custom(const void *a, const void *b,
                       int (*compare)(const void *, const void *)) {
    //
    return NULL;
}

pair *minmax(const void *a, const void *b) {
    //
    return NULL;
}

pair *minmax_custom(const void *a, const void *b,
                    int (*compare)(const void *, const void *)) {
    //
    return NULL;
}

iterator *min_element(iterator *first, iterator *last) {
    //
    return NULL;
}

iterator *min_element_custom(iterator *first, iterator *last,
                             int (*compare)(const void *, const void *)) {
    //
    return NULL;
}

iterator *max_element(iterator *first, iterator *last) {
    //
    return NULL;
}

iterator *max_element_custom(iterator *first, iterator *last,
                             int (*compare)(const void *, const void *)) {
    //
    return NULL;
}

pair *minmax_element(iterator *first, iterator *last) {
    //
    return NULL;
}

pair *minmax_element_custom(iterator *first, iterator *last,
                            int (*compare)(const void *, const void *)) {
    //
    return NULL;
}

bool lexicographical_compare(iterator *first1, iterator *last1,
                             iterator *first2, iterator *last2) {
    //
    return false;
}

bool lexicographical_compare_custom(iterator *first1, iterator *last1,
                                    iterator *first2, iterator *last2,
                                    int (*compare)(const void *, const void *)) {
    //
    return false;
}
