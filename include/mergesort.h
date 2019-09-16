/**
 *  @file       mergesort.h
 *  @brief      Header file for a mergesort algorithm
 *
 *  @author     Gemuele Aludino
 *  @date       21 Jul 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino.
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

#ifndef MERGESORT_H
#define MERGESORT_H

#include <stdlib.h>

/**< vector: mergesort (iterative) */
void v_mergesort_iterative(void *arr, size_t n, size_t width,
                           int (*compare)(const void *, const void *));

/**< list_node_base: mergesort (iterative/recursive) */
void lnb_mergesort_iterative(void *arg_head,
                            int (*compare)(const void *, const void *));
void lnb_mergesort_recursive(void *arg_head, void *arg_tail,
                            int (*compare)(const void *, const void *));

/**< slist_node_base: mergesort (iterative/recursive) */
void slnb_mergesort_iterative(void *arg_head,
                            int (*compare)(const void *, const void *));
void slnb_mergesort_recursive(void *arg_head,
                            int (*compare)(const void *, const void *));

#endif /* MERGESORT_H */
