/**
 *  @file       gcslib.h
 *  @brief      Header file for Gem's C Library (gcslib)
 *
 *  @author     Gemuele Aludino
 *  @date       21 Jun 2019
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

#ifndef GCSLIB_H
#define GCSLIB_H

/**
 *  Any preprocessor directives (includes) that are commented out
 *  are for header files associated with containers/algorithms that
 *  are still in the works.
 */

/**
 *  Required by all headers in gcslib
 */
#include "utils.h"

/**
 *  Dependencies:
 *      utils
 */
#include "iterator.h"
/* #include "pair.h" */
/* #include "pair_int_str.h" */

/**
 *  Dependencies:
 *      utils
 *      algorithm
 */
/* #include "algorithm.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      mergesort
 */
#include "vector.h"

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      vector_tmpl
 *      mergesort_tmpl
 */
/* #include "vector_float.h" */
/* #include "vector_int.h" */
/* #include "vector_str.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      mergesort
 */
/* #include "list.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      mergesort_tmpl
 *      list_tmpl
 */
/* #include "list_float.h" */
/* #include "list_int.h" */
/* #include "list_str.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      mergesort
 */
/* #include "slist.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      mergesort_tmpl
 *      slist_tmpl
 */
/* #include "slist_float.h" */
/* #include "slist_int.h" */
/* #include "slist_str.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      mergesort
 */
/* #include "deque.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      mergesort_tmpl
 *      deque_tmpl
 */
/* #include "deque_float.h" */
/* #include "deque_int.h" */
/* #include "deque_str.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      deque
 */
/* #include "stack.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      deque_tmpl
 */
/* #include "stack_float.h" */
/* #include "stack_int.h" */
/* #include "stack_str.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      deque
 */
/* #include "queue.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      deque_tmpl
 */
/* #include "queue_float.h" */
/* #include "queue_int.h" */
/* #include "queue_str.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      vector
 */
/* #include "rbtree.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      vector
 */
/* #include "rbtree_float.h" */
/* #include "rbtree_int.h" */
/* #include "rbtree_str.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      vector
 *      pair_int_str
 */
/* #include "rbtree_pair_int_str.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      rbtree
 */
/* #include "set.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      rbtree
 */
/* #include "set_float.h" */
/* #include "set_int.h" */
/* #include "set_str.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      pair
 *      rbtree
 */
/* #include "map.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      pair_int_str
 *      rbtree_pair_int_str
 */
/* #include "map_int_str.h" */

/**
 *  Dependencies:
 *      utils
 *      iterator
 *      vector_int
 *      vector_str
 *      list_str
 */
/* #include "csv.h" */

#endif /* GCSLIB_H */
