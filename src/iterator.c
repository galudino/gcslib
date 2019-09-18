/**
 *  @file       iterator.c
 *  @brief      Source file for a polymorphic container iterator
 *
 *  @author     Gemuele Aludino
 *  @date       23 Jun 2019
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

/**
 *  @file       iterator.h
 *  @brief      Required for iterator (struct iterator) and related functions
 */
#include "iterator.h"

/**
 *  @file       utils.h
 *  @brief      Required for (struct typetable) and related functions
 */
#include "utils.h"

#include <assert.h>
#include <stdlib.h>

/**
 *  @brief  Returns an iterator to the beginning of a container
 *          using its itbl
 *
 *  @param[in]  it  iterator representing a container
 *
 *  @return     iterator at the beginning of a container
 */
iterator it_begin(iterator it) {
    massert_ptr(it.itbl->begin);
    return it.itbl->begin(it.container);
}

/**
 *  @brief  Returns an iterator to the end of a container
 *          using its itbl
 *
 *  @param[in]  it  iterator representing a container
 *
 *  @return     iterator at the end of a container
 */
iterator it_end(iterator it) {
    massert_ptr(it.itbl->end);
    return it.itbl->end(it.container);
}

/**
 *  @brief  Returns a new iterator that is one block past
 *          it's current position
 *
 *  @param[in]  it  iterator representing a container
 *
 *  @return     iterator that is one block past it's position
 */
iterator it_next(iterator it) {
    return it.itbl->next(it);
}

/**
 *  @brief  Returns a new iterator that is n blocks past
 *          it's current position
 *
 *  @param[in]  it  iterator representing a container
 *
 *  @return     iterator that is n blocks past it's current position
 */
iterator it_next_n(iterator it, int n) {
    return it.itbl->next_n(it, n);
}

/**
 *  @brief  Returns a new iterator that is one block behind
 *          it's current position
 *
 *  @param[in]  it  iterator representing a container
 *
 *  @return     iterator that is one block behind it's current position
 */
iterator it_prev(iterator it) {
    return it.itbl->prev(it);
}

/**
 *  @brief  Returns a new iterator that is n blocks behind
 *          it's current position
 *
 *  @param[in]  it  iterator representing a container
 *
 *  @return     iterator that is n blocks behind it's current position
 */
iterator it_prev_n(iterator it, int n) {
    return it.itbl->prev_n(it, n);
}

/**
 *  @brief  Advances/retracts iterator n blocks
 *
 *  @param[in]  it  pointer to iterator
 *
 *  @return     same pointer to iterator, but advanced n blocks
 */
iterator *it_advance(iterator *it, int n) {
    assert(it);
    return it->itbl->advance(it, n);
}

/**
 *  @brief  Advances it one block (goes forward)
 *
 *  @param[in]  it  pointer to iterator
 *
 *  @return     same pointer to iterator, but advanced one block
 */
iterator *it_incr(iterator *it) {
    massert_iterator(it);
    massert_pfunc(it->itbl->incr);
    return it->itbl->incr(it);
}

/**
 *  @brief  Retracts it one block (goes backward)
 *
 *  @param[in]  it  pointer to iterator
 *
 *  @return     same pointer to iterator, but advanced one block
 */
iterator *it_decr(iterator *it) {
    massert_iterator(it);
    massert_pfunc(it->itbl->decr);
    return it->itbl->decr(it);
}

/**
 *  @brief  Returns address of element at current position
 *
 *  @param[in]  it  iterator that represents a container
 *
 *  @return     address of element at current position
 */
void *it_curr(iterator it) {
    return it.itbl->curr(it);
}

/**
 *  @brief  Returns address of element at beginning of iterator
 *
 *  @param[in]  it  iterator that represents a container
 *
 *  @return     address of element at beginning of container
 */
void *it_start(iterator it) {
    return it.itbl->start(it);
}

/**
 *  @brief  Returns address of element at end of iterator
 *
 *  @param[in]  it  iterator that represents a container
 *
 *  @return     address of element at end of container
 */
void *it_finish(iterator it) {
    return it.itbl->finish(it);
}

/**
 *  @brief  Determines the distance between first and last numerically
 *
 *  @param[in]  first   pointer to iterator
 *  @param[in]  last    pointer to iterator
 *
 *  @return     numerical distance between first and last
 *
 *  This function can also be used to determine the numerical position
 *  of a particular iterator, since indices are not stored within iterators.
 *
 *  To find the index position of an iterator, leave one of the parameters
 *  NULL when calling the distance function.
 */
int it_distance(iterator *first, iterator *last) {
    if (first == NULL && last != NULL) {
        return last->itbl->distance(NULL, last);
    } else if (last == NULL && first != NULL) {
        return first->itbl->distance(first, NULL);
    } else if (first == NULL && last == NULL) {
        return 0;
    } else if (first->itbl != last->itbl) {
        return 0;
    } else {
        if (first->itbl != last->itbl) {
            return 0;
        } else {
            return first->itbl->distance(first, last);
        }
    }
}

/**
 *  @brief  Determines if it has elements to visit in the forward direction
 *
 *  @param[in]  it  iterator that refers to a container
 *
 *  @return     true if elements remain in the forward direction,
 *              false otherwise
 */
bool it_has_next(iterator it) {
    massert_pfunc(it.itbl->has_next);
    return it.itbl->has_next(it);
}

/**
 *  @brief  Determines if it has elements to visit in the backward direction
 *
 *  @param[in]  it  iterator that refers to a container
 *
 *  @return     true if elements remain in the backward direction,
 *              false otherwise
 */
bool it_has_prev(iterator it) {
    massert_pfunc(it.itbl->has_prev);
    return it.itbl->has_prev(it);
}

/**
 *  @brief  Retrieve a container's typetable
 *
 *  @param[in]  it  iterator that refers to a container
 *
 *  @return     pointer to typetable
 */
struct typetable *it_get_ttbl(iterator it) {
    return it.itbl->get_ttbl(it.container);
}
