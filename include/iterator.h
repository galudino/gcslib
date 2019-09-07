/**
 *  @file       iterator.h
 *  @brief      Header file for a polymorphic container iterator
 *
 *  @author     Gemuele Aludino
 *  @date       23 Jun 2019
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

#ifndef ITERATOR_H
#define ITERATOR_H

/**
 *  @file       utils.h
 *  @brief      Required for (struct typetable) and related functions
 */
#include "utils.h"

/**
 *  @typedef    iterator
 *  @brief      Alias for (struct iterator)
 */
typedef struct iterator iterator;

/**
 *  @typedef    iterator_table
 *  @brief      Alias for (struct iterator_table)
 */
typedef struct iterator_table iterator_table;

/**
 *  @struct     iterator
 *  @brief      Abstraction for a container
 */
struct iterator {
    struct iterator_table *itbl;         /**< common function interface */
    void *container;                     /**< data structure, for begin, end */
    void *curr;    /**< current position (address of a block, or node, etc.) */
};

/**
 *  @struct     iterator_table
 *  @brief      Determines the functionality of an iterator,
 *              specific to a particular container type
 */
struct iterator_table {
    iterator    (*begin)       (void *);
    iterator    (*end)         (void *);

    iterator    (*next)        (iterator);
    iterator    (*next_n)      (iterator, int);

    iterator    (*prev)        (iterator);
    iterator    (*prev_n)      (iterator, int);

    iterator *  (*advance)     (iterator *, int);
    iterator *  (*incr)        (iterator *);
    iterator *  (*decr)        (iterator *);

    void *      (*curr)        (iterator);
    void *      (*start)       (iterator);
    void *      (*finish)      (iterator);

    int         (*distance)    (iterator *, iterator *);

    bool        (*has_next)    (iterator);
    bool        (*has_prev)    (iterator);

    struct typetable * (*get_ttbl)(void *);
};

/**< iterator: begin/end (returns new iterator) */
iterator it_begin(iterator it);
iterator it_end(iterator it);

/**< iterator: next/next_n (returns new iterator) */
iterator it_next(iterator);
iterator it_next_n(iterator it, int n);

/**< iterator: prev/prev_n  (returns new iterator) */
iterator it_prev(iterator);
iterator it_prev_n(iterator it, int n);

/**< iterator: advance/incr/decr (mutates iterator) */
iterator *it_advance(iterator *it, int n);
iterator *it_incr(iterator *it);
iterator *it_decr(iterator *it);

/**< iterator: curr/start/finish (retrieve elements) */
void *it_curr(iterator it);
void *it_start(iterator it);
void *it_finish(iterator it);

/**< iterator: calculate distance */
int it_distance(iterator *first, iterator *last);

/**< iterator: determine if end of iteration reached */
bool it_has_next(iterator it);
bool it_has_prev(iterator it);

/**< iterator: retrieve typetable */
struct typetable *it_get_ttbl(iterator it);

/* C99 only */
#define foreach(TYPE, ID, IT)                                                  \
for (TYPE *ID = NULL,                                                          \
    (*finish) = it_finish(IT);                                                 \
    ((ID = it_curr(IT)) != finish);                                            \
    it_incr(&IT))                                                              \

#endif /* ITERATOR_H */
