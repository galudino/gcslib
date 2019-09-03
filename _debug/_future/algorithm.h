/**
 *  @file       algorithm.h
 *  @brief      Header file for container utility functions
 *
 *  @author     Gemuele Aludino
 *  @date       23 Jun 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "iterator.h"
#include "pair.h"
#include "utils.h"

bool all_of(iterator *first, iterator *last,
            bool (*unary_predicate)(const void *));
bool any_of(iterator *first, iterator *last,
            bool (*unary_predicate)(const void *));
bool none_of(iterator *first, iterator *last,
             bool (*unary_predicate)(const void *));

void for_each(iterator *first, iterator *last, void (*consumer)(const void *));

iterator *find(iterator *first, iterator *last, const void *valaddr);
iterator *find_if(iterator *first, iterator *last,
                  bool (*unary_predicate)(const void *));
iterator *find_if_not(iterator *first, iterator *last,
                      bool (*unary_predicate)(const void *));

iterator *find_end_equal(iterator *first1, iterator *last1, iterator *first2,
                         iterator *last2);
iterator *find_end_if(iterator *first1, iterator *last1, iterator *first2,
                      iterator *last2, bool (*unary_predicate)(const void *));

iterator *find_first_of_equal(iterator *first1, iterator *last1,
                              iterator *first2, iterator *last2);
iterator *find_first_of_if(iterator *first1, iterator *last1, iterator *first2,
                           iterator *last2,
                           bool (*unary_predicate)(const void *));

iterator *adjacent_find_equal(iterator *first, iterator *last);
iterator *adjacent_find_if(iterator *first, iterator *last,
                           bool (*binary_predicate)(const void *,
                                                    const void *));

iterator *count(iterator *first, iterator *last, const void *valaddr);
iterator *count_if(iterator *first, iterator *last,
                   bool (*unary_predicate)(const void *));

pair *mismatch_equal(iterator *first1, iterator *last1, iterator *first2);
pair *mismatch_predicate(iterator *first1, iterator *last1, iterator *first2,
                         bool (*binary_predicate)(const void *, const void *));

bool equal_equal(iterator *first1, iterator *last1, iterator *first2);
bool equal_predicate(iterator *first1, iterator *last2, iterator *first2,
                     bool (*binary_predicate)(const void *, const void *));

iterator *search_equal(iterator *first1, iterator *last1, iterator *first2,
                       iterator *last2);
iterator *search_predicate(iterator *first1, iterator *last1, iterator *first2,
                           iterator *last2,
                           bool (*binary_predicate)(const void *,
                                                    const void *));

iterator *search_n_equal(iterator *first, iterator *last, size_t count,
                         const void *valaddr);
iterator *search_n_predicate(iterator *first, iterator *last, size_t count,
                             const void *valaddr,
                             bool (*binary_predicate)(const void *,
                                                      const void *));

bool is_partitioned(iterator *first, iterator *last,
                    bool (*unary_predicate)(const void *));
iterator *partition(iterator *first, iterator *last,
                    bool (*unary_predicate)(const void *));

// omitting stable_partition
// omitting partition_copy

iterator *partition_point(iterator *first, iterator *last,
                          bool (*predicate)(const void *));

void sort(iterator *first, iterator *last);
void sort_custom(iterator *first, iterator *last,
                 int (*compare)(const void *, const void *));

void partial_sort(iterator *first, iterator *middle, iterator *last);
void partial_sort_custom(iterator *first, iterator *middle, iterator *last,
                         int (*compare)(const void *, const void *));

iterator *partial_sort_copy(iterator *first, iterator *last,
                            iterator *result_first, iterator *result_last);

iterator *partial_sort_copy_custom(iterator *first, iterator *last,
                                   iterator *result_first,
                                   iterator *result_last,
                                   int (*compare)(const void *, const void *));

bool is_sorted(iterator *first, iterator *last);
bool is_sorted_custom(iterator *first, iterator *last,
                      int (*compare)(const void *, const void *));

iterator *is_sorted_until(iterator *first, iterator *last);
iterator *is_sorted_until_custom(iterator *first, iterator *last,
                                 int (*compare)(const void *, const void *));

void nth_element(iterator *first, iterator *nth, iterator *last);
void nth_element_custom(iterator *first, iterator *nth, iterator *last,
                        int (*compare)(const void *, const void *));

iterator *lower_bound(iterator *first, iterator *last, const void *valaddr);
iterator *lower_bound_custom(iterator *first, iterator *last,
                             const void *valaddr,
                             int (*compare)(const void *, const void *));
iterator *upper_bound(iterator *first, iterator *last, const void *valaddr);
iterator *upper_bound_custom(iterator *first, iterator *last,
                             const void *valaddr,
                             int (*compare)(const void *, const void *));

pair *equal_range(iterator *first, iterator *last, const void *valaddr);
pair *equal_range_custom(iterator *first, iterator *last, const void *valaddr,
                         int (*compare)(const void *, const void *));

bool binary_search(iterator *first, iterator *last, const void *valaddr);
bool binary_search_custom(iterator *first, iterator *last, const void *valaddr,
                          int (*compare)(const void *, const void *));

iterator *merge(iterator *first1, iterator *last1, iterator *first2,
                iterator *last2, iterator *result);
iterator *merge_custom(iterator *first1, iterator *last1, iterator *first2,
                       iterator *last2, iterator *result,
                       int (*compare)(const void *, const void *));

void inplace_merge(iterator *first, iterator *middle, iterator *last);
void inplace_merge_custom(iterator *first, iterator *middle, iterator *last,
                          int (*compare)(const void *, const void *));

bool includes(iterator *first1, iterator *last1, iterator *first2,
              iterator *last2);
bool includes_custom(iterator *first1, iterator *last1, iterator *first2,
                     iterator *last2,
                     int (*compare)(const void *, const void *));

// TODO set_union
// TODO set_intersection
// TODO set_difference
// TODO set_symmetric_difference

void push_heap(iterator *first, iterator *last);
void push_heap_custom(iterator *first, iterator *last,
                      int (*compare)(const void *, const void *));
void pop_heap(iterator *first, iterator *last);
void pop_heap_custom(iterator *first, iterator *last,
                     int (*compare)(const void *, const void *));

void make_heap(iterator *first, iterator *last);
void make_heap_custom(iterator *first, iterator *last,
                      int (*compare)(const void *, const void *));

void sort_heap(iterator *first, iterator *last);
void sort_heap_custom(iterator *first, iterator *last,
                      int (*compare)(const void *, const void *));

bool is_heap(iterator *first, iterator *last);
bool is_heap_custom(iterator *first, iterator *last,
                    int (*compare)(const void *, const void *));

const void *min(const void *a, const void *b);
const void *min_custom(const void *a, const void *b,
                       int (*compare)(const void *, const void *));

const void *max(const void *a, const void *b);
const void *max_custom(const void *a, const void *b,
                       int (*compare)(const void *, const void *));

pair *minmax(const void *a, const void *b);
pair *minmax_custom(const void *a, const void *b,
                    int (*compare)(const void *, const void *));

iterator *min_element(iterator *first, iterator *last);
iterator *min_element_custom(iterator *first, iterator *last,
                             int (*compare)(const void *, const void *));

iterator *max_element(iterator *first, iterator *last);
iterator *max_element_custom(iterator *first, iterator *last,
                             int (*compare)(const void *, const void *));

pair *minmax_element(iterator *first, iterator *last);
pair *minmax_element_custom(iterator *first, iterator *last,
                            int (*compare)(const void *, const void *));

bool lexicographical_compare(iterator *first1, iterator *last1,
                             iterator *first2, iterator *last2);
bool lexicographical_compare_custom(iterator *first1, iterator *last1,
                                    iterator *first2, iterator *last2,
                                    int (*compare)(const void *, const void *));

#endif /* ALGORITHM_H */
