/**
 *  @file       vector.c
 *  @brief      Source file for a dynamic array ADT
 *
 *  @author     Gemuele Aludino
 *  @date       24 Aug 2019
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

#include "vector.h"
#include "mergesort.h"
#include "iterator.h"
#include "utils.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_MAXIMUM_STACK_BUFFER_SIZE 16384
#define VECTOR_DEFAULT_CAPACITY          16

// optional macros for accessing the innards of vector_base
#define AT(VEC, INDEX)      ((char *)(VEC->impl.start) + ((INDEX) * (VEC->ttbl->width)))
#define FRONT(VEC)          VEC->impl.start
#define BACK(VEC)           ((char *)(VEC->impl.finish) - (VEC->ttbl->width))
#define END(VEC)            VEC->impl.end_of_storage

/**
 *  @struct     vector
 *  @brief      Represents a dynamic array ADT
 *
 *  Note that struct vector and struct vector_base are
 *  opaque -- their fields cannot be accessed directly,
 *  nor can instances of struct vector/struct vector_base
 *  be created on the stack. This is done to enforce encapsulation.
 */
struct vector {
    /**
     *  @struct     vector_base
     *  @brief      Decouples vector-related fields from typetable
     */
    struct vector_base {
        void *start;          /**< address of array base (first element) */
        void *finish;         /**< address reserved for next rear element */
        void *end_of_storage; /**< addresses last allocated block of storage */
    } impl;

    struct typetable *ttbl; /**< data width, cpy, dtor, swap, compare, print */
};

static vector *v_allocate(void);
static void v_init(vector *v, struct typetable *ttbl, size_t capacity);
static void v_deinit(vector *v);
static void v_swap_addr(vector *v, void *first, void *second);

struct typetable ttbl_vector = {
    sizeof(vector),
    vector_copy,
    vector_dtor,
    vector_swap,
    vector_compare,
    vector_print
};

struct typetable *_vector_ = &ttbl_vector;

static iterator vi_begin(void *arg);
static iterator vi_end(void *arg);

static iterator vi_next(iterator it);
static iterator vi_next_n(iterator it, int n);

static iterator vi_prev(iterator it);
static iterator vi_prev_n(iterator it, int n);

static int vi_distance(iterator *first, iterator *last);

static iterator *vi_advance(iterator *it, int n);
static iterator *vi_incr(iterator *it);
static iterator *vi_decr(iterator *it);

static void *vi_curr(iterator it);
static void *vi_start(iterator it);
static void *vi_finish(iterator it);

static bool vi_has_next(iterator it);
static bool vi_has_prev(iterator it);

static struct typetable *vi_get_ttbl(void *arg);

struct iterator_table itbl_vector = {
    vi_begin,
    vi_end,
    vi_next,
    vi_next_n,
    vi_prev,
    vi_prev_n,
    vi_advance,
    vi_incr,
    vi_decr,
    vi_curr,
    vi_start,
    vi_finish,
    vi_distance,
    vi_has_next,
    vi_has_prev,
    vi_get_ttbl
};

struct iterator_table *_vector_iterator_ = &itbl_vector;

/**
 *  @brief  Allocates, constructs, and returns a pointer to vector,
 *          size VECTOR_DEFAULT_CAPACITY (16)
 *
 *  @param[in]  ttbl    pointer to struct typetable for
 *                      width/copy/dtor/swap/compare/print
 *
 *  @return     pointer to vector
 */
vector *v_new(struct typetable *ttbl) {
    vector *v = v_allocate();                   // allocate
    v_init(v, ttbl, VECTOR_DEFAULT_CAPACITY);   // construct
    return v;                                   // return
}

/**
 *  @brief  Allocates, constructs, and returns a pointer to vector,
 *          size n
 *
 *  @param[in]  ttbl   pointer to struct typetable for
 *                     width/copy/dtor/swap/compare/print
 *
 *  @return     pointer to vector
 */
vector *v_newr(struct typetable *ttbl, size_t n) {
    vector *v = v_allocate();                   // allocate
    v_init(v, ttbl, n);                         // construct
    return v;                                   // return
}

/**
 *  @brief  Calls vnewr and returns a pointer to vector,
 *          filled with n copies of valaddr
 *
 *  @param[in]  ttbl    pointer to struct typetable for
 *                      width/copy/dtor/swap/compare/print
 *  @param[in]  n       number of elements to copy
 *  @param[in]  valaddr address of element to copy
 *
 *  @return     pointer to vector
 *
 *  If ttbl has a copy function defined,
 *  n copies of valaddr will be inserted into the returned vector
 *  using the copy function - this copy function is intended for
 *  deep copies. Otherwise, valaddr will be shallow-copied using memcpy.
 */
vector *v_newfill(struct typetable *ttbl, size_t n, void *valaddr) {
    assert(valaddr);

    vector *v = v_newr(ttbl, n);
    void *sentinel = (char *)(v->impl.start) + (n * v->ttbl->width);

    if (v->ttbl->copy) {
        // if copy function defined in ttbl, instances of valaddr are deep copied
        while (v->impl.finish != sentinel) {
            v->ttbl->copy(v->impl.finish, valaddr);
            v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
        }
    } else {
        // if no copy function defined in ttbl, instance of valaddr are shallow copied
        while (v->impl.finish != sentinel) {
            memcpy(v->impl.finish, valaddr, v->ttbl->width);
            v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
        }
    }

    return v;
}

/**
 *  @brief  Calls vnewr and returns a pointer to vector,
 *          filled with elements from iterators first and last,
 *          with range [first, last)
 *
 *  @param[in]  first   represents the beginning of the range (inclusive)
 *  @param[in]  last    represents the end of the range (exclusive)
 *
 *  @return     pointer to vector
 *
 *  iterators first and last must refer to the same struct iterator_table,
 *  and would ideally refer to the same container -- but there are no mechanisms
 *  to prevent a use from using a pair of iterators that iterate through the same
 *  container type, but refer to different containers (and possibly holding different
 *  data types). It is the user's responsibilty to ensure that first and last
 *      - refer to the same container type
 *      - refer to containers of matching data types
 *      - (most importantly) refer to the same container
 *
 *  If iterator first refers to a container with a ttbl that has a copy
 *  function defined, [first, last) will be copied into the return vector
 *  using the copy function -- this function meant for deep copying.
 *  Otherwise, [first, last) will be shallow-copied using memcpy.
 */
vector *v_newrnge(iterator first, iterator last) {
    if (first.itbl != last.itbl) {
        // iterators first and last must refer to the same container,
        // or else the cursor pointer will never meet the sentinel pointer
        // that will end the copy loop.
        // this also means that iterators first and last cannot refer to
        // different container types (this is determined by what itbls
        // they each point to)
        ERROR(__FILE__, "first and last must matching container types and refer to the same container.");
        return NULL;
    }

    int delta = it_distance(&first, &last);
    struct typetable *ttbl_first = it_get_ttbl(first);

    vector *v = v_newr(ttbl_first, delta);

    void *sentinel = it_curr(last);         // iteration range is [first, last)
    void *curr = NULL;

    if (ttbl_first->copy) {
        while ((curr = it_curr(first)) != sentinel)  {
            ttbl_first->copy(v->impl.finish, curr);

            v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            it_incr(&first);
        }
    } else {
        while ((curr = it_curr(first)) != sentinel)  {
            curr = it_curr(first);
            memcpy(v->impl.finish, curr, v->ttbl->width);

            v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            it_incr(&first);
        };
    }

    return v;
}

/**
 *  @brief  Calls vnewr and returns a pointer to vector,
 *          filled with elements from an existing vector v.
 *
 *  @param[in]  v   pointer to vector, containing the desired source elements
 *
 *  @return     pointer to vector, with copies of v's elements
 *
 *  If v uses a ttbl that has a copy function defined,
 *  elements from v will be copied to the returned vector using the
 *  copy function -- this function is meant for deep copying.
 *  Otherwise, the contents of v will be shallow-copied using memcpy.
 */
vector *v_newcopy(vector *v) {
    assert(v);

    vector *copy = v_newr(v->ttbl, v_capacity(v));

    void *sentinel = (char *)(copy->impl.finish) + (v_size(v) * copy->ttbl->width);
    void *curr = v->impl.start;

    if (v->ttbl->copy) {
        while (copy->impl.finish != sentinel) {
            copy->ttbl->copy(copy->impl.finish, curr);

            copy->impl.finish = (char *)(copy->impl.finish) + (copy->ttbl->width);
            curr = (char *)(curr) + (v->ttbl->width);
        }
    } else {
        while (copy->impl.finish != sentinel) {
            memcpy(copy->impl.finish, curr, v->ttbl->width);

            copy->impl.finish = (char *)(copy->impl.finish) + (copy->ttbl->width);
            curr = (char *)(curr) + (v->ttbl->width);
        }
    }

    return copy;
}

/**
 *  @brief  Calls v_allocate to allocate a new vector, named move,
 *          and transfers ownership of v's fields to vector move.
 *
 *  @param[out] v   Address of a pointer to vector
 *
 *  @return     pointer to vector, initialized with v's fields
 *
 *  vector v is reinitialized with the same ttbl, with size 1.
 *  v may be deleted by the client, or reused.
 */
vector *v_newmove(vector **v) {
    assert(v);

    vector *move = v_allocate();

    // a new vector, move, is constructed from the contents of an existing vector,
    // by a change of ownership between impl pointers.
    // the previous vector, v, is initialized to have a size of 1 --
    // v may be deleted clientside if preferred.
    move->impl.start = (*v)->impl.start;
    move->impl.finish = (*v)->impl.finish;
    move->impl.end_of_storage = (*v)->impl.end_of_storage;
    move->ttbl = (*v)->ttbl;

    v_init((*v), (*v)->ttbl, 1);

    return move;
}

/**
 *  @brief  Calls v_deinit (vector's destructor) and deallocates the pointer v.
 *
 *  @param[out] v   Address of a pointer to vector
 *
 *  Every call to any of vector's "new" functions should be accompanied
 *  by a call to v_delete when a pointer to vector is no longer needed.
 *
 *  If the vector has a ttbl with a dtor function defined,
 *  the elements within the vector will be destroyed using the dtor function.
 *
 *  Note that if the elements from within v are dynamically allocated,
 *  and/or the elements have dynamically allocated fields --
 *  and there is no dtor function defined, memory management
 *  will become the client's responsibility.
 */
void v_delete(vector **v) {
    assert((*v));

    // deinitialization involves releasing all dynamically allocated
    // memory at each field (if dtor function is defined in ttbl,
    // and is written to release such memory) --
    // after that step (or if there is no memory to free within the elements),
    // (*v)->impl.start has its memory freed, and the remainder of (*v)'s fields
    // are set NULL.
    v_deinit((*v));

    // finally, the memory (*v) points to will be freed.
    free((*v));
    (*v) = NULL;
}

/**
 *  @brief  Returns an iterator that points to the first element in vector
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return     iterator that refers to v
 */
iterator v_begin(vector *v) {
    return vi_begin(v);
}

/**
 *  @brief  Returns an iterator that pointers to an element that is one block
 *          past the last element in v
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return     iterator that refers to v
 */
iterator v_end(vector *v) {
    return vi_end(v);
}

/**
 *  @brief  Returns the logical length of v
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return     logical length of v
 */
size_t v_size(vector *v) {
    assert(v);
    // effectively v->impl.finish - v->impl.start
    return ptr_distance(v->impl.start, v->impl.finish, v->ttbl->width);
}

/**
 *  @brief  Returns the theoretical maximum amount of elements that
 *          can be stored by vector
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return     theoretical maximum logical length of v
 */
size_t v_maxsize(vector *v) {
    assert(v);

    // ptr_sz is 8 bytes on 64 bit system, 4 bytes on 32 bit system
    size_t ptr_sz = (sizeof(void *) == 8) ? 8 : 4;
    return ((pow)(2.0, (ptr_sz * 8)) / (v->ttbl->width)) - 1;
}

/**
 *  @brief  Resizes the vector to size n
 *
 *  @param[in]  v   pointer to vector
 *  @param[in]  n   desired size for v
 *
 *  If n is less than the current logical length (v_size(v)),
 *  v will be truncated (excess elements are destroyed),
 *  otherwise, v's capacity will be extended.
 */
void v_resize(vector *v, size_t n) {
    assert(v);

    size_t old_size = v_size(v);
    size_t old_capacity = v_capacity(v);

    if (old_capacity == n) {
        return;
    }

    // if n is less than the old size, and items were deep copied --
    // dynamically allocated memory will be released as per the dtor function
    // prior to truncation
    // (if dtor is defined in ttbl, copy should be defined as well)
    if (n < old_size && v->ttbl->dtor) {
        void *target = (char *)(v->impl.start) + (n * v->ttbl->width);

        const int back_index = n - 1;
        for (int i = 0; i < back_index; i++) {
            v->ttbl->dtor(target);
            target = (char *)(target) - (v->ttbl->width);
        }
    }

    void *newstart = NULL;
    newstart = realloc(v->impl.start, n * v->ttbl->width);
    assert(newstart);

    size_t fin = n > old_size ? old_size : n;
    size_t end = n > old_size ? n : fin;

    v->impl.start = newstart;
    v->impl.finish = (char *)(v->impl.start) + (fin * v->ttbl->width);
    v->impl.end_of_storage = (char *)(v->impl.start) + (end * v->ttbl->width);
}

/**
 *  @brief  Resizes the vector to size n elements.
 *
 *  @param[in] v           pointer to vector
 *  @param[in]  n           desired size for v
 *  @param[in]  valaddr     element to fill new blocks with
 *
 *  If n exceeds the size/capacity of v, valaddr will be copied
 *  to the new blocks for the resize.
 *
 *  If n is less than or equal to the size of v,
 *  the vector will be cleared and new memory of size n will be
 *  initialized, with each block consisting of copies of valaddr.
 */
void v_resizefill(vector *v, size_t n, const void *valaddr) {
    assert(v);

    size_t old_size = v_size(v);
    size_t old_capacity = v_capacity(v);

    void *sentinel = (char *)(v->impl.start) + (n * v->ttbl->width);

    if (n > old_capacity) {
        LOG(__FILE__, "greater");
        // if n is greater than the old size, multiple instances of valaddr are
        // appended to the rear of the vector following a resize.
        v_resize(v, n);

        // sentinel must be updated to reflect the resize.
        sentinel = (char *)(v->impl.start) + (n * v->ttbl->width);

        if (v->ttbl->copy) {
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish, valaddr);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            }
        } else {
            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish, valaddr, v->ttbl->width);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            }
        }
    } else {
        // if n is less than or equal to the old size,
        // and dtor is defined in ttbl,
        // then dynamically allocated memory will be released
        // at each element, as per the dtor function.
        if (v->ttbl->dtor) {
            v->impl.finish = (char *)(v->impl.finish) - (v->ttbl->width);

            while (v->impl.finish != v->impl.start) {
                v->ttbl->dtor(v->impl.finish);
                v->impl.finish = (char *)(v->impl.finish) - (v->ttbl->width);
            }

            v->ttbl->dtor(v->impl.finish);
        }

        // memory at the base address of the vector will be released...
        free(v->impl.start);
        v->impl.start = NULL;

        // ...and new memory will be allocated of size n to represent
        // the new base address of the vector.
        // (alternatively, v->impl.start have also been realloc'ed to size n),
        // but if n is less than or equal to the old size, the old elements
        // will be overwritten anyway.
        void *newstart = NULL;
        newstart = calloc(n, v->ttbl->width);
        assert(newstart);

        // pointers at impl are re-established
        v->impl.start = newstart;
        v->impl.finish = v->impl.start;
        v->impl.end_of_storage = (char *)(v->impl.start) + (n * v->ttbl->width);

        sentinel = v->impl.end_of_storage;

        if (v->ttbl->copy) {
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish, valaddr);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            }
        } else {
            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish, valaddr, v->ttbl->width);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            }
        }
    }
}

/**
 *  @brief  Returns the capacity of v
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return         Capacity of v
 */
size_t v_capacity(vector *v) {
    assert(v);
    // effectively v->impl.end_of_storage - v->impl.start
    return ptr_distance(v->impl.start, v->impl.end_of_storage, v->ttbl->width);
}

/**
 *  @brief  Determines if v is an empty vector, or not
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return     true if v->impl.start == v->impl.finish, false otherwise
 */
bool v_empty(vector *v) {
    assert(v);
    // since v->impl.finish is always one address ahead of vector's back element,
    // if v->impl.start == v->impl.finish, the vector is empty.
    return v->impl.start == v->impl.finish;
}

/**
 *  @brief  Reserves n blocks of elements for v
 *
 *  @param[out] v   pointer to vector
 *  @param[in]  n   desired amount of blocks to reserve
 *
 *  This function is effectively a conditional resize --
 *  n must exceed that of v's current capacity.
 */
void v_reserve(vector *v, size_t n) {
    assert(v);

    // reserve is effectively a resize,
    // but verifies that the value of n meets the requirements for a reservation.
    if (n > v_capacity(v)) {
        v_resize(v, n);
    } else {
        // no-op
        ERROR(__FILE__, "n must be greater than vector's buffer capacity.");
        return;
    }
}

/**
 *  @brief  Shrinks vector's buffer to that of its logical length
 *
 *  @param[in] v    pointer to vector
 *
 *  This function is effectively a conditional resize --
 *  but verifies that:
 *      - vector is not empty
 *      - vector's finish pointer is not equal to end_of_storage pointer
 */
void v_shrink_to_fit(vector *v) {
    assert(v);

    if ((v->impl.start != v->impl.finish) && (v->impl.finish != v->impl.end_of_storage)) {
        v_resize(v, v_size(v));
    }
}

/**
 *  @brief  Retrieves the address of an element from vector at index n
 *
 *  @param[in]  v   pointer to vector
 *  @param[in]  n   index of desired element
 *
 *  @return     address of element at n
 *
 *  A bounds check is performed to ensure that n is a valid index.
 *
 *  Note that the address (pointer) of the element type is returned,
 *  be sure to cast the return of v_at to the pointer type of
 *  the element, then dereference the casted v_at to yield
 *  the element itself.
 */
void *v_at(vector *v, size_t n) {
    assert(v);

    size_t size = v_size(v);
    void *target = NULL;

    if (n >= size) {
        ERROR(__FILE__, "n is greater than vector's logical length -- index out of bounds.");
        return NULL;
    } else if (n == 0) {
        // if n is 0, it's the front of the vector
        target = v->impl.start;
    } else if (n == (size - 1)) {
        // if n is (size - 1), the back index, effectively (v->impl.finish - 1)
        target = (char *)(v->impl.finish) - (v->ttbl->width);
    } else {
        // if n is anywhere within [0, size), effectively (v->impl.start + n)
        target = (char *)(v->impl.start) + (n * v->ttbl->width);
    }

    return target ? target : NULL;
}

/**
 *  @brief  Retrieves the address of the front element from vector
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return     address of front element
 *
 *  Note that the address (pointer) of the element type is returned,
 *  be sure to cast the return of v_front to the pointer type of
 *  the element, then dereference the casted v_front to yield
 *  the element itself.
 */
void *v_front(vector *v) {
    assert(v);
    // v_front() returns v->impl.start,
    // so if
    //  TYPE is your intended data type,
    // and
    //  *(TYPE *)(v->impl.start)
    // refers to a (TYPE),
    // and
    //  (TYPE *)(v->impl.start)
    //  refers to a (TYPE *),
    // then v_front(v) can be casted as such:
    //      (TYPE *)(v_front(v))
    // and dereferenced --
    ///     *(TYPE **)(v_front(v))
    // to yield:
    //      TYPE front = *(TYPE *)(v_front(v));
    return v->impl.start;
}

/**
 *  @brief  Retrieves the address of the rear element from vector
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return     address of rear element
 *
 *  Note that the address (pointer) of the element type is returned,
 *  be sure to cast the return of v_back to the pointer type of
 *  the element, then dereference the casted v_back to yield
 *  the element itself.
 */
void *v_back(vector *v) {
    assert(v);
    // v_back() returns v->impl.finish - (v->ttbl->width),
    // which is effectively (v->impl.finish - 1).
    // so if
    //  TYPE is your intended data type,
    // and
    //  *(TYPE *)(v->impl.finish - v->ttbl->width)
    // refers to a (TYPE),
    // and
    //  (TYPE *)(v->impl.finish - v->ttbl->width)
    //  refers to a (TYPE *),
    // then v_back(v) can be casted as such:
    //      (TYPE *)(v_back(v))
    // and dereferenced --
    ///     *(TYPE **)(v_back(v))
    // to yield:
    //      TYPE back = *(TYPE *)(v_back(v));
    return (char *)(v->impl.finish) - (v->ttbl->width);
}

/**
 *  @brief  Retrieves the address of vector's buffer
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return     address of v->impl.start
 */
void *v_data(vector *v) {
    assert(v);
    // v_data() returns the address of v->impl.start,
    // so if
    //  TYPE is your intended data type,
    // and
    //  *(TYPE *)(v->impl.start)
    //  refers to a (TYPE),
    // and
    //  (TYPE *)(v->impl.start)
    //  refers to a (TYPE *),
    // and
    //  v_data(v)
    //  returns &(v->impl.start),
    // then v_data(v) can be casted as such:
    //     (TYPE **)(v_data(v))
    // and dereferenced --
    //     *(TYPE **)(v_data(v));
    // to yield:
    //      TYPE *array = *(TYPE **)(v_data(v));
    // and array[i] will yield a TYPE at index i.
    return &(v->impl.start);
}

/**
 *  @brief  Retrieves the address of an element from vector at index n
 *
 *  @param[in]  v   pointer to vector
 *  @param[in]  n   index of desired element
 *
 *  @return     address of element at n, const qualified
 *
 *  A bounds check is performed to ensure that n is a valid index.
 *
 *  Note that the address (pointer) of the element type is returned,
 *  be sure to cast the return of v_at to the pointer type of
 *  the element, then dereference the casted v_at to yield
 *  the element itself.
 */
const void *v_at_const(vector *v, size_t n) {
    assert(v);

    if (n >= v_size(v)) {
        ERROR(__FILE__, "n is greater than vector's logical length -- index out of bounds.");
        return NULL;
    }

    void *target = (char *)(v->impl.start) + (n * v->ttbl->width);
    return target ? target : NULL;
}

/**
 *  @brief  Retrieves the address of the front element from vector
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return     address of front element, const qualified
 *
 *  Note that the address (pointer) of the element type is returned,
 *  be sure to cast the return of v_front to the pointer type of
 *  the element, then dereference the casted v_front to yield
 *  the element itself.
 */
const void *v_front_const(vector *v) {
    assert(v);
    return v->impl.start;
}

/**
 *  @brief  Retrieves the address of the rear element from vector
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return     address of rear element, const qualified
 *
 *  Note that the address (pointer) of the element type is returned,
 *  be sure to cast the return of v_back to the pointer type of
 *  the element, then dereference the casted v_back to yield
 *  the element itself.
 */
const void *v_back_const(vector *v) {
    assert(v);
    return (char *)(v->impl.finish) - (v->ttbl->width);
}

/**
 *  @brief  Retrieves the address of vector's buffer
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return     address of v->impl.start, const qualified
 */
const void *v_data_const(vector *v) {
    assert(v);
    return &(v->impl.start);
}

/**
 *  @brief  Assigns a range of elements to vector, starting at its beginning
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  first   represents the beginning of the range (inc)
 *  @param[in]  last    represents the end of the range, (exc)
 *
 *  If the numeric range [first, last) exceeds that of
 *  v_size(v), the old elements will be destroyed,
 *  and a new buffer will be assigned to v, with copies
 *  of elements from the iterator range.
 *
 *  If the range does not exceed that of v_size(v),
 *  only beginning of the vector will be overwritten
 *  with the contents from the range.
 */
void v_assignrnge(vector *v, iterator first, iterator last) {
    int delta = it_distance(&first, &last);
    struct typetable *ttbl_first = it_get_ttbl(first);

    void *sentinel = it_curr(last);
    void *curr = NULL;

    size_t old_size = v_size(v);
    size_t old_capacity = v_capacity(v);

    if (delta >= old_capacity) {
        // if range [first, last) exceeds that of old_capacity,
        // resize the vector to accomodate the new elements
        v_resize(v, delta);
    }

    // move the finish pointer to the beginning of the vector
    v->impl.finish = v->impl.start;

    if (ttbl_first->copy && ttbl_first->dtor) {
        // if items were deep copied, existing memory
        // will be released before copying new data
        while ((curr = it_curr(first)) != sentinel)  {
            ttbl_first->dtor(v->impl.finish);
            ttbl_first->copy(v->impl.finish, curr);

            v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            it_incr(&first);
        }
    } else {
        // if items were shallow copied,
        // blocks will be overwritten immediately
        while ((curr = it_curr(first)) != sentinel)  {
            memcpy(v->impl.finish, curr, v->ttbl->width);

            v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            it_incr(&first);
        };
    }

    // restoring finish pointer to appropriate location
    v->impl.finish = delta < old_size
    ? (char *)(v->impl.start) + (old_size * v->ttbl->width) : v->impl.finish;
}

/**
 *  @brief  Assigns n copies of an element, starting at beginning of vector
 *
 *  @param[in]  v           pointer to vector
 *  @param[in]  n           amount of elements to assign
 *  @param[in]  valaddr     the element to assign
 *
 *  If v has dynamically allocated elements, and/or the elements
 *  have dynamically allocated fields, and there is no dtor function
 *  defined in v's ttbl, the management of v's memory will become
 *  the client's responsibility. This is especially important to note
 *  if n is less than v_size(v).
 *
 *  Likewise, if v does not have a copy function defined,
 *  assignfill will shallow-copy valaddr into v.
 */
void v_assignfill(vector *v, size_t n, const void *valaddr) {
    size_t old_size = v_size(v);
    size_t old_capacity = v_capacity(v);

    if (n >= old_size || n >= old_capacity) {
        // elements from [0, size) are completely overwritten and replaced with
        // valaddr. if elements were deep copied, their memory
        // must be released first.

        if (v->ttbl->dtor) {
            v->impl.finish = (char *)(v->impl.finish) - (v->ttbl->width);

            while (v->impl.finish != v->impl.start) {
                v->ttbl->dtor(v->impl.finish);
                v->impl.finish = (char *)(v->impl.finish) - (v->ttbl->width);
            }

            v->ttbl->dtor(v->impl.start);
        }

        free(v->impl.start);
        v->impl.start = NULL;

        void *newstart = NULL;
        newstart = calloc(n, v->ttbl->width);
        assert(newstart);

        v->impl.start = newstart;
        v->impl.finish = v->impl.start;
        v->impl.end_of_storage = (char *)(v->impl.start) + (n * v->ttbl->width);

        if (v->ttbl->copy) {
            // deep copy
            while (v->impl.finish != v->impl.end_of_storage) {
                v->ttbl->copy(v->impl.finish, valaddr);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            }
        } else {
            // shallow copy
            while (v->impl.finish != v->impl.end_of_storage) {
                memcpy(v->impl.finish, valaddr, v->ttbl->width);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            }
        }
    } else {
        if (v->ttbl->dtor) {
            // elements from [0, n) are completely overwritten and replaced with
            // valaddr. if elements were deep copied, their memory must be released first.
            v->impl.finish = (char *)(v->impl.finish) - ((n + 1) * v->ttbl->width);

            while (v->impl.finish != v->impl.start) {
                v->ttbl->dtor(v->impl.finish);
                v->impl.finish = (char *)(v->impl.finish) - (v->ttbl->width);
            }

            v->ttbl->dtor(v->impl.start);
        } else {
            // if elements are shallow copied,
            // we skip memory deallocation and move the finish pointer
            // to the address of the start pointer
            v->impl.finish = v->impl.start;
        }

        void *sentinel = (char *)(v->impl.start) + (n * v->ttbl->width);

        if (v->ttbl->copy) {
            // deep copy
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish, valaddr);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            }
        } else {
            // shallow copy
            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish, valaddr, v->ttbl->width);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            }
        }

        // restoring the finish pointer to its former address
        v->impl.finish = (char *)(v->impl.start) + (old_size * v->ttbl->width);
    }
}

/**
 *  @brief  Appends an element to the rear of the vector
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  valaddr address of element to be copied
 *
 *  If a copy function is defined in v's ttbl,
 *  valaddr will be deep-copied into v.
 *  Otherwise, valaddr will be shallow-copied into v
 *  using memcpy.
 */
void v_pushb(vector *v, const void *valaddr) {
    assert(v);

    // a doubling strategy is employed when the finish pointer
    // meets the end_of_storage pointer.
    if (v->impl.finish == v->impl.end_of_storage) {
        v_resize(v, v_capacity(v) * 2);
    }

    if (v->ttbl->copy) {
        // if copy fn defined in ttbl, deep copy
        v->ttbl->copy(v->impl.finish, valaddr);
    } else {
        // if no copy defined in ttbl, shallow copy
        memcpy(v->impl.finish, valaddr, v->ttbl->width);
    }

    // advance finish pointer to the next empty block
    v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
}

/**
 *  @brief  Removes element at the rear of the vector
 *
 *  @param[in]  v   pointer to vector
 *
 *  If a dtor function is defined in v's ttbl,
 *  the rear element will be destroyed using the dtor function
 *  from within ttbl.
 *
 *  Otherwise, the popped element will be overwritten by the next
 *  element that is added at the finish pointer.
 *
 *  Memory management of dynamically allocated elements/elements with
 *  dynamically allocated fields become the client's responsibility
 *  if a dtor function is NOT defined within v's ttbl.
 */
void v_popb(vector *v) {
    assert(v);

    if (v->impl.finish == v->impl.start) {
        // v_popb is a no-op if vector is empty
        return;
    }

    // decrement the finish pointer to the address of the "victim" block
    v->impl.finish = (char *)(v->impl.finish) - (v->ttbl->width);

    if (v->ttbl->dtor) {
        // if dtor defined in ttbl.
        // release memory at finish pointer as defined by dtor.
        v->ttbl->dtor(v->impl.finish);
    }

    // if no dtor defined, the next element appendage
    // will simply be overwritten
}

/**
 *  @brief  Inserts a value into vector at position specified by pos
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  pos     refers to an element within v
 *  @param[in]  valaddr address of element to insert
 *
 *  @return     iterator referring the position of the new element within v
 *
 *  Undefined behavior if pos does not refer to an element within v.
 *
 *  If a copy function is defined in v's ttbl,
 *  valaddr will be deep-copied into v.
 *  Otherwise, valaddr will be shallow-copied into v
 *  using memcpy.
 */
iterator v_insert(vector *v, iterator pos, const void *valaddr) {
    assert(v);
    assert(valaddr);

    // a doubling strategy is employed when the finish pointer
    // meets the end_of_storage pointer.
    if (v->impl.finish == v->impl.end_of_storage) {
        v_resize(v, v_capacity(v) * 2);
    }

    // to insert within indices (0, v_size(v)),
    // begin by appending valaddr to the rear of the vector.

    if (v->ttbl->copy) {
        // if copy fn defined in ttbl, deep copy
        v->ttbl->copy(v->impl.finish, valaddr);
    } else {
        // if no copy defined in ttbl, shallow copy
        memcpy(v->impl.finish, valaddr, v->ttbl->width);
    }

    // all of the above was code for pushb,
    // but incrementing the finish pointer was omitted.

    // save pos's index - a new iterator will be returned later
    size_t ipos = it_distance(NULL, &pos);

    // use iterator pos to swap elements from [pos, v_size(v))
    // valaddr will reside at the index originally specified by pos
    void *curr = NULL;
    while ((curr = it_curr(pos)) != v->impl.finish) {
        v_swap_addr(v, curr, v->impl.finish);
        it_incr(&pos);
    }

    // restore finish pointer to address of rear element
    v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);

    // returned is an iterator at refers to valaddr's position in v
    return it_next_n(v_begin(v), ipos);
}

/**
 *  @brief  Inserts n copies of valaddr into vector at position specified by pos
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  pos     refers to an element within v
 *  @param[in]  n       quantity of desired copies of valaddr to insert
 *  @param[in]  valaddr the desired element to fill vector with
 *
 *  @return     iterator referring to the first copy of valaddr from within vector
 *
 *  Undefined behavior of pos does not refer to an element positioned within v.
 *
 *  If ttbl has a copy function defined,
 *  n copies of valaddr will be inserted into the returned vector
 *  using the copy function - this copy function is intended for
 *  deep copies. Otherwise, valaddr will be shallow-copied using memcpy.
 */
iterator v_insertfill(vector *v, iterator pos, size_t n, const void *valaddr) {
    assert(v);
    assert(valaddr);

    size_t ipos = it_distance(NULL, &pos);      // pos's index position
    size_t old_size = v_size(v);                // v's former size
    size_t old_capacity = v_capacity(v);        // v's former capacity

    if ((old_size + n) >= old_capacity) {
        // if inserting n elements will equal or exceed that of old_capacity,
        // vector will be resized to accomodate ((old_capacity + n) * 2) elements.
        v_resize(v, (old_capacity + n) * 2);
        pos = it_next_n(v_begin(v), ipos);
    }

    if (n <= 0) {
        // no-op
        return pos;
    } else if (n == 1) {
        // if n is 1, redirect to v_insert and return early
        v_insert(v, pos, valaddr);
        return it_next_n(v_begin(v), ipos);
    }

    if (ipos == old_size - 1 || ipos == old_size) {
        // inlined instructions resembling pushb
        void *sentinel = (char *)(v->impl.finish) + (n * v->ttbl->width);

        if (v->ttbl->copy) {
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish, valaddr);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            }
        } else {
            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish, valaddr, v->ttbl->width);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            }
        }

        // since n == back index, return early
        return it_next_n(v_begin(v), n);
    } else {
        // decrement finish pointer so it refers to the rear element
        v->impl.finish = (char *)(v->impl.finish) - (v->ttbl->width);

        // each element in the range [n, v_size(v)) will be moved n blocks over
        // -- starting with the rear element.

        // sentinel will be set one position behind pos.curr
        // to account for the new element(s) being inserted
        void *sentinel = (char *)(pos.curr) - (v->ttbl->width);

        // new destination address for finish pointer
        void *right_adj = v->impl.finish + (n * v->ttbl->width);

        // need to save this address for later when restoring impl.finish
        void *finish = (char *)(v->impl.finish) + ((n + 1) * v->ttbl->width);

        // elements [n, v_size(v)) are moved over n blocks to the right,
        // starting with v->impl.finish.
        // loop stops when finish pointer reaches sentinel
        while (v->impl.finish != sentinel) {
            v_swap_addr(v, v->impl.finish, right_adj);

            v->impl.finish = (char *)(v->impl.finish) - (v->ttbl->width);
            right_adj = (char *)(right_adj) - (v->ttbl->width);
        }

        // sentinel is changed to point to the (ipos + n)th block
        // (one element past the last fill insertion element)
        sentinel = (char *)(v->impl.start) + ((ipos + n)) * v->ttbl->width;

        // v->impl.finish must be advanced one block forward
        // (finish was decremented until it was at the address
        // of the former sentinel value, which was one position behind
        // pos.curr)
        v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);

        if (v->ttbl->copy) {
            // deep copy
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish, valaddr);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            }
        } else {
            // shallow copy
            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish, valaddr, v->ttbl->width);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            }
        }

        // now restoring v->impl.finish to where it should be
        v->impl.finish = finish;
    }

    return it_next_n(v_begin(v), ipos);
}

/**
 *  @brief  Inserts elements from [first, last) into v, at position pos
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  pos     refers to an element within v
 *  @param[in]  first   represents the beginning of the range (inclusive)
 *  @param[in]  last    represents the end of the range (exclusive)
 *
 *  @return     iterator representing the first element
 *              from the range that was copied into v
 *
 *  Undefined behavior if
 *      - pos does not refer to an element from within v
 *      - first and last do not refer to the same container
 *      - first and last refer to containers of differing element types,
 *        which in turn differ from the element type stored in v
 *
 *  iterators first and last must refer to the same struct iterator_table,
 *  and would ideally refer to the same container -- but there are no mechanisms
 *  to prevent a use from using a pair of iterators that iterate through the same
 *  container type, but refer to different containers (and possibly holding different
 *  data types). It is the user's responsibilty to ensure that first and last
 *      - refer to the same container type
 *      - refer to containers of matching data types
 *      - (most importantly) refer to the same container
 *
 *  If iterator first refers to a container with a ttbl that has a copy
 *  function defined, [first, last) will be copied into the return vector
 *  using the copy function -- this function meant for deep copying.
 *  Otherwise, [first, last) will be shallow-copied using memcpy.
 */
iterator v_insertrnge(vector *v, iterator pos, iterator first, iterator last) {
    assert(v);

    size_t ipos = it_distance(NULL, &pos);      // pos's index position
    size_t old_size = v_size(v);                // v's former size
    size_t old_capacity = v_capacity(v);        // v's former capacity
    size_t delta = it_distance(&first, &last);  // index(last) - index(first)

    if ((old_size + delta) >= old_capacity) {
        // if inserting delta elements will equal or exceed that of
        // old_capacity, vector will be resized to
        // accommodate ((old_capacity + delta) * 2) elements.
        v_resize(v, (old_capacity + delta) * 2);
        pos = it_next_n(v_begin(v), ipos);
    }

    if (delta <= 0) {
        // no-op
        return pos;
    } else if (delta == 1) {
        // if delta is 1, redirect to v_insert and return early
        v_insert(v, pos, it_curr(first));
        return it_next_n(v_begin(v), ipos);
    }

    if (ipos >= old_size - 1) {
        // inlined instructions resembling pushb
        void *sentinel = (char *)(v->impl.finish) + (delta * v->ttbl->width);

        if (v->ttbl->copy) {
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish, it_curr(first));
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
                it_incr(&first);
            }
        } else {
            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish, it_curr(first), v->ttbl->width);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
                it_incr(&first);
            }
        }

        // since n == back index, return early
        return it_next_n(v_begin(v), delta);
    } else {
        // decrement finish pointer so it refers to the rear element
        v->impl.finish = (char *)(v->impl.finish) - (v->ttbl->width);

        // each element in the range [n, v_size(v))
        // will be moved delta blocks over
        // -- starting with the rear element.

        // sentinel will be set one position behind pos.curr
        // to account for the new element(s) being inserted
        void *sentinel = (char *)(pos.curr) - (v->ttbl->width);

        // new destination address for finish pointer
        void *right_adj = v->impl.finish + (delta * v->ttbl->width);

        // need to save this address for later when restoring impl.finish
        void *finish = (char *)(v->impl.finish) + ((delta + 1) * v->ttbl->width);

        // elements [n, v_size(v)) are moved over delta blocks to the right,
        // starting with v->impl.finish.
        // loop stops when finish pointer reaches sentinel
        while (v->impl.finish != sentinel) {
            v_swap_addr(v, v->impl.finish, right_adj);

            v->impl.finish = (char *)(v->impl.finish) - (v->ttbl->width);
            right_adj = (char *)(right_adj) - (v->ttbl->width);
        }

        // sentinel is changed to point to the (ipos + delta)th block
        // (one element past the last fill insertion element)
        sentinel = (char *)(v->impl.start) + ((ipos + delta)) * v->ttbl->width;

        // v->impl.finish must be advanced one block forward
        // (finish was decremented until it was at the address
        // of the former sentinel value, which was one position behind
        // pos.curr)
        v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);

        if (v->ttbl->copy) {
            // deep copy
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish, it_curr(first));
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
                it_incr(&first);
            }
        } else {
            // shallow copy
            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish, it_curr(first), v->ttbl->width);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
                it_incr(&first);
            }
        }

        // now restoring v->impl.finish to where it should be
        v->impl.finish = finish;
    }

    return it_next_n(v_begin(v), ipos);
}

/**
 *  @brief  Moves valaddr into v, at position pos
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  pos     refers to an element from within vector
 *  @param[out] valaddr address of element to move into v
 *
 *  @return     iterator representing the position from within v
 *              where valaddr was moved
 *
 *  This function is designed for types containing
 *  dynamically allocated fields, and/or the type itself
 *  is a pointer to dynamically allocated memory.
 *
 *  A swap function must be defined in v's typetable in order
 *  to run this function, otherwise a regular insert is performed.
 */
iterator v_insertmove(vector *v, iterator pos, void *valaddr) {
    assert(v);
    assert(valaddr);

    void *dst = NULL;

    if (v->ttbl->swap) {
        // if a swap function is defined,
        // the address of dst will be sent to swap,
        // along with valaddr (which is already the address of some var
        // sent in by the client)
        // the contents of valaddr will be used to initialize dst,
        // and the standard insert function will be called after.

        // if no swap function is defined,
        // a regular insert occurs.

        // this function can prove useful if a client
        // has a dynamically allocated type, like a pointer
        // (or a type with dynamically allocated fields)
        // and wants vector to have full ownership
        // of the memory that the param inserted referred to

        // this can help prevent unwanted deep copying of elements,
        // or shallow copies of elements where there are two pointers
        // referring to the same memory.
        v->ttbl->swap(&dst, valaddr);
    }

    return v_insert(v, pos, &dst);
}

/**
 *  @brief  Removes an element from v at position pos
 *
 *  @param[in]  v   pointer to vector
 *  @param[in]  pos refers to an element from within vector
 *
 *  @return     iterator representing an element that has replaced the
 *              erased element from within v
 *
 *  If a dtor function is defined in v's ttbl,
 *  the element at position pos will be destroyed using the dtor function
 *  from within ttbl.
 *
 *  Memory management of dynamically allocated elements/elements with
 *  dynamically allocated fields become the client's responsibility
 *  if a dtor function is NOT defined within v's ttbl.
 */
iterator v_erase(vector *v, iterator pos) {
    assert(v);

    int ipos = it_distance(NULL, &pos);
    const size_t back_index = v_size(v) - 1;

    if ((ipos < 0) || (ipos >= back_index + 1)) {
        // if ipos is negative or ipos is greater than or equal to v_size(v),
        // no-op
        return pos;
    } else if (ipos == back_index ||
        pos.curr == (char *)(v->impl.finish) - (v->ttbl->width)) {
        // if ipos == back_index
        // or pos refers to rear vector element,
        // redirect to popb and return early
        v_popb(v);

        // pos will no longer refer to an existing element,
        // so an iterator is returned referring to an element
        // at the erased element's former index
        return it_next_n(v_begin(v), ipos);
    } else if (ipos < back_index && ipos >= 0) {
        if (v->ttbl->dtor) {
            // if elements were deep copied, release their memory
            v->ttbl->dtor(pos.curr);
        }

        void *curr = NULL;
        void *sentinel = v->impl.finish;
        // v->impl.finish and it_finish(pos) should refer to the same thing.
        // undefined behavior if pos does not refer to container v!

        while ((curr = it_curr(pos)) != sentinel) {
            // swapping it_curr(pos) and it_curr(it_next(pos))
            // will shift it_curr(it_next(pos)) one element
            // to the left. this process continues until
            // it_curr(pos) equals v->impl.finish.
            v_swap_addr(v, curr, it_curr(it_next(pos)));
            it_incr(&pos);
        }

        // decrementing the finish pointer 1 block to the left
        v->impl.finish = (char *)(v->impl.finish) - (v->ttbl->width);
    }

    // pos will no longer refer to an existing element,
    // so an iterator is returned referring to an element
    // at the erased element's former index
    return it_next_n(v_begin(v), ipos);
}

iterator v_erasernge(vector *v, iterator pos, iterator last) {
    assert(v);

    int ipos = it_distance(NULL, &pos);     // index of pos
    int lpos = it_distance(NULL, &last);    // index of last
    int delta = it_distance(&pos, &last);   // diff between pos/last

    const size_t back_index = v_size(v) - 1;

    if ((ipos < 0) || (ipos >= back_index + 1)) {
        // if ipos is negative or ipos is greater than or equal to v_size(v),
        // no-op
        return pos;
    } else if (ipos == back_index ||
        pos.curr == (char *)(v->impl.finish) - (v->ttbl->width)) {
        // if ipos == back_index
        // or pos refers to rear vector element,
        // redirect to popb and return early
        v_popb(v);

        // pos will no longer refer to an existing element,
        // so an iterator is returned referring to an element
        // at the erased element's former index
        return it_next_n(v_begin(v), ipos);
    } else if (ipos < back_index && ipos >= 0) {
        void *curr = NULL;
        void *sentinel = it_curr(last);

        if (v->ttbl->dtor) {
            // if range of elements were deep copied, release their memory
            while ((curr = it_curr(pos)) != sentinel) {
                v->ttbl->dtor(curr);
                it_incr(&pos);
            }
        }

        // restoring pos to its original index
        pos = it_next_n(v_begin(v), ipos);

        // reassigning sentinel to one block past the last elem
        sentinel = v->impl.finish;

        // iterator last will advance until it reaches sentinel
        // (finish pointer) as iterator pos advances alongside it -- they will swap elements between each other
        // until last reaches the end of the vector.
        while ((curr = it_curr(last)) != sentinel) {
            v_swap_addr(v, curr, it_curr(pos));

            it_incr(&pos);
            it_incr(&last);
        }

        // using the delta (difference between pos and last),
        // the finish pointer is decremented to the first null element (one past the last non-null element)
        v->impl.finish = (char *)(v->impl.finish) - (delta * v->ttbl->width);
    }

    // pos will no longer refer to an existing element,
    // so an iterator is returned referring to an element
    // at the beginning of the erased element's former range
    return it_next_n(v_begin(v), ipos);
}

/**
 *  @brief  Swaps fields between v and other
 *
 *  @param[out] v       address of pointer to vector
 *  @param[out] other   address of pointer to vector
 */
void v_swap(vector **v, vector **other) {
    assert((*v));
    assert((*other));

    vector *temp = (*v);

    // change of ownership between v and other
    (*v)->impl.start = (*other)->impl.start;
    (*v)->impl.finish = (*other)->impl.finish;
    (*v)->impl.end_of_storage = (*other)->impl.end_of_storage;

    // vectors holding two different types can be swapped
    (*v)->ttbl = (*other)->ttbl;

    (*other)->impl.start = temp->impl.start;
    (*other)->impl.finish = temp->impl.finish;
    (*other)->impl.end_of_storage = temp->impl.end_of_storage;

    (*other)->ttbl = temp->ttbl;
}

/**
 *  @brief  Destroys elements from within v
 *
 *  @param[in]  v   pointer to vector
 *
 *  Memory management of dynamically allocated elements
 *  and/or elements with dynamically allocated fields
 *  become the client's responsibility if a dtor function
 *  is NOT defined within v's ttbl.
 */
void v_clear(vector *v) {
    assert(v);

    if (v->impl.finish == v->impl.start) {
        // if vector is empty
        // (start and finish pointers share same address),
        // it's a no-op.
        return;
    }

    if (v->ttbl->dtor) {
        // if elements were deep-copied,
        // their memory must be released as per the
        // client-supplied dtor function in ttbl.

        // decrementing finish pointer to match the address of the last element
        v->impl.finish = (char *)(v->impl.finish) - (v->ttbl->width);

        while (v->impl.finish != v->impl.start) {
            v->ttbl->dtor(v->impl.finish);
            v->impl.finish = (char *)(v->impl.finish) - (v->ttbl->width);
        }

        v->ttbl->dtor(v->impl.finish);
        memset(v->impl.start, 0, v_capacity(v) * v->ttbl->width);
        // v->impl.finish is already at v->impl.start.
    } else {
        // if elements were shallow copied,
        // (no dtor function specified in ttbl) --
        // we just memset and reset the finish pointer.
        memset(v->impl.start, 0, v_capacity(v) * v->ttbl->width);
        v->impl.finish = v->impl.start;
    }
}

/**
 *  @brief  Inserts a value into vector at position specified by index
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  index   numerical index of element from within v
 *  @param[in]  valaddr address of element to insert
 *
 *  If a copy function is defined in v's ttbl,
 *  valaddr will be deep-copied into v.
 *  Otherwise, valaddr will be shallow-copied into v
 *  using memcpy.
 */
void v_insert_at(vector *v, size_t index, const void *valaddr) {
    assert(v);
    assert(valaddr);

    const size_t size = v_size(v);

    if (index >= size) {
        ERROR(__FILE__, "index provided [%lu] is out of bounds. size of vector is %lu.", index, size);
        return;
    }

    // a doubling strategy is employed when the finish pointer
    // meets the end_of_storage pointer.
    if (v->impl.finish == v->impl.end_of_storage) {
        v_resize(v, v_capacity(v) * 2);
    }

    // to insert within indices (0, v_size(v)),
    // begin by appending valaddr to the rear of the vector.

    if (v->ttbl->copy) {
        // if copy fn defined in ttbl, deep copy
        v->ttbl->copy(v->impl.finish, valaddr);
    } else {
        // if no copy defined in ttbl, shallow copy
        memcpy(v->impl.finish, valaddr, v->ttbl->width);
    }

    // all of the above was code for pushb,
    // but incrementing the finish pointer was omitted.

    // use index to swap elements from [index, v_size(v))
    // valaddr will reside at the index originally specified by
    // index
    void *curr = (char *)(v->impl.start) + (index * v->ttbl->width);
    while (curr != v->impl.finish) {
        v_swap_addr(v, curr, v->impl.finish);
        curr = (char *)(curr) + (v->ttbl->width);
    }

    // restore finish pointer to address of rear element
    v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
}

/**
 *  @brief  Removes an element from v at position pos
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  index   numerical index of element from within v
 *
 *  If a dtor function is defined in v's ttbl,
 *  the element at position pos will be destroyed using the dtor function
 *  from within ttbl.
 *
 *  Memory management of dynamically allocated elements/elements with
 *  dynamically allocated fields become the client's responsibility
 *  if a dtor function is NOT defined within v's ttbl.
 */
void v_erase_at(vector *v, size_t index) {
    assert(v);

    const size_t size = v_size(v);

    if (index >= size) {
        ERROR(__FILE__, "index provided [%lu] is out of bounds. size of vector is %lu.", index, size);
        return;
    }

    const size_t back_index = size - 1;

    void *curr = (char *)(v->impl.start) + (index * v->ttbl->width);    // element at index
    void *next = index <= v_capacity(v)
    ? (char *)(curr) + (v->ttbl->width) : NULL;                         // element adj to curr
    void *sentinel = v->impl.finish;                                    // one block after last elem

    if (index == back_index ||
        curr == (char *)(v->impl.finish) - (v->ttbl->width)) {
        // if index == back_index
        // or pos refers to rear vector element,
        // redirect to popb and return early
        v_popb(v);
        return;
    } else if (index < back_index && index >= 0) {
        if (v->ttbl->dtor) {
            // if elements were deep copied, release their memory
            v->ttbl->dtor(curr);
        }

        while (curr != sentinel) {
            // swapping curr and next will shift next
            // one element to the left. this process continues
            // until next equals v->impl.finish.
            v_swap_addr(v, curr, next);

            curr = (char *)(curr) + (v->ttbl->width);
            next = (char *)(next) + (v->ttbl->width);
        }

        // decrementing the finish pointer 1 block to the left
        v->impl.finish = (char *)(v->impl.finish) - (v->ttbl->width);
    }
}

/**
 *  @brief  Replaces element at index with valaddr
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  index   numerical index of element from within v
 *  @param[in]  valaddr address of element to replace at index
 *
 *  Element at index is destroyed, and immediately replaced with valaddr.
 *
 *  If a dtor function is defined in v's ttbl,
 *  the element at position pos will be destroyed using the dtor function
 *  from within ttbl.
 *
 *  Memory management of dynamically allocated elements/elements with
 *  dynamically allocated fields become the client's responsibility
 *  if a dtor function is NOT defined within v's ttbl.
 *
 *  If a copy function is defined in v's ttbl,
 *  valaddr will be deep-copied into v.
 *  Otherwise, valaddr will be shallow-copied into v
 *  using memcpy.
 */
void v_replace_at(vector *v, size_t index, const void *valaddr) {
    assert(v);

    const size_t size = v_size(v);

    if (index >= size) {
        ERROR(__FILE__, "index provided [%lu] is out of bounds. size of vector is %lu.", index, size);
        return;
    }

    const size_t back_index = (size - 1);
    void *curr = (char *)(v->impl.start) + (index * v->ttbl->width);

    if (v->ttbl->dtor) {
        // deep-copied elements are destroyed (if dtor defined)
        v->ttbl->dtor(curr);
    }

    if (v->ttbl->copy) {
        // if copy defined, replacement elem is deep-copied
        v->ttbl->copy(curr, valaddr);
    } else {
        // if no copy defined, replace elem is shallow-copied
        memcpy(curr, valaddr, v->ttbl->width);
    }
}

/**
 *  @brief  Swap two elements from within v
 *
 *  @param[in]  v   pointer to vector
 *  @param[in]  n1  index of first element
 *  @param[in]  n2  index of second element
 */
void v_swap_elem(vector *v, size_t n1, size_t n2) {
    assert(v);

    size_t size = v_size(v);
    size_t capacity = v_capacity(v);

    bool n1_bad = n1 >= size || n1 >= capacity;
    bool n2_bad = n2 >= size || n2 >= capacity;

    bool good_indices = !n1_bad && !n2_bad;

    if (good_indices && size > 0) {
        void *data_1 = (char *)(v->impl.start) + (n1 * v->ttbl->width);
        void *data_2 = (char *)(v->impl.start) + (n2 * v->ttbl->width);

        void *temp = NULL;
        temp = malloc(v->ttbl->width);
        assert(temp);
        memcpy(temp, data_1, v->ttbl->width);

        memcpy(data_1, data_2, v->ttbl->width);
        memcpy(data_2, temp, v->ttbl->width);

        free(temp);
        temp = NULL;
    } else {
        ERROR(__FILE__, "indices n1 [%lu] and/or n2 [%lu] are out of bounds.", n1, n2);
        return;
    }
}

/**
 *  @brief  Removes all occurences of valaddr from within v
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  valaddr address of element to remove (copy of)
 *
 *  This function uses the supplied compare function with v's
 *  ttbl to determine if valaddr matches any of the elements.
 *
 *  If a dtor function is defined in v's ttbl,
 *  the matched element will be destroyed using the dtor function
 *  from within ttbl.
 *
 *  Memory management of dynamically allocated elements/elements with
 *  dynamically allocated fields become the client's responsibility
 *  if a dtor function is NOT defined within v's ttbl.
 */
void v_remove(vector *v, const void *valaddr) {
    assert(v);
    assert(valaddr);

    //
    //  Need to come up with something more efficient than this.
    //  Running v_erase_at in a loop means
    //      When element to erase is found at index i
    //      Erase element at i
    //      Then shift elements from [i + 1, v_size(v))
    //          one block left
    //      Repeat for all elements found.
    //      (that's more shifting than necessary)
    //
    //  An alternative idea would be:
    //      In one pass, delete and/or set NULL
    //      any element that matches that of valaddr
    //      Do not do any shifting of elements yet.
    //
    //      In the next pass,
    //          For each block
    //              Find a NULL block
    //                  When NULL block found,
    //                      Advance from NULL block n times
    //                      until a non-null block is found.
    //                          When non-NULL block found,
    //                          do v_swap_addr with NULL block
    //
    //  Another idea would be:
    //      Keep a bool del_index[v_size(v)] array.
    //      Destroy the elements that are to be removed in one pass.
    //      Mark the indices in the del_index array.
    //
    //      In the next pass,
    //          Go through the vector and specifically visit
    //          the NULL indices, and v_swap_addr that NULL
    //          element with the first non-NULL element found.
    //          Repeat until there are no more NULLs to swap with
    //          Decrement the finish pointer based on how many
    //          elements were removed.

    if (v->impl.start == v->impl.finish) {
        return;
    }

    size_t i = 0;
    size_t size = v_size(v);
    void *curr = v->impl.start;

    if (v->ttbl->compare(curr, valaddr) == 0) {
        v_erase_at(v, i);
        --size;
        curr = (char *)(curr) + (v->ttbl->width);
    }

    for (i = 1; i < size; i++) {
        if (v->ttbl->compare(curr, valaddr) == 0) {
            curr = (char *)(curr) - (v->ttbl->width);
            v_erase_at(v, i--);
            --size;
        }

        curr = (char *)(curr) + (v->ttbl->width);
    }
}

/**
 *  @brief  Removes all occurences of valaddr from within v, by condition
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  valaddr address of element to remove (copy of)
 *
 *  If unary_predicate(valaddr) returns true...
 *
 *  ...this function uses the supplied compare function with v's
 *  ttbl to determine if valaddr matches any of the elements.
 *
 *  If a dtor function is defined in v's ttbl,
 *  the matched element will be destroyed using the dtor function
 *  from within ttbl.
 *
 *  Memory management of dynamically allocated elements/elements with
 *  dynamically allocated fields become the client's responsibility
 *  if a dtor function is NOT defined within v's ttbl.
 */
void v_remove_if(vector *v, const void *valaddr,
                 bool (*unary_predicate)(const void *)) {
    assert(v);
    assert(valaddr);
    assert(unary_predicate);

    if (v->impl.start == v->impl.finish) {
        return;
    }

    // if unary_predicate returns true
    // from evaluating the contents of valaddr,
    // redirect to v_remove
    if (unary_predicate(valaddr) == false) {
        return;
    } else {
        v_remove(v, valaddr);
    }
}

/**
 *  @brief  Append the contents of other to the rear of v
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  other   pointer to vector
 *
 *  @return     pointer to vector with other's elements appended
 *
 *  The merging of vector v and vector other does not mutate other.
 */
vector *v_merge(vector *v, vector *other) {
    assert(v);
    assert(other);

    const size_t size_other = v_size(v);
    const size_t capacity_v = v_capacity(v);

    if (size_other >= capacity_v) {
        // if other's size + v's current size will exceed
        // that of v's capacity, resize v
        v_resize(v, capacity_v * 2);
    }

    void *sentinel = other->impl.finish;
    other->impl.finish = other->impl.start;

    // the merging of v and other does not mutate other.
    if ((v->ttbl->copy && other->ttbl->copy)
        && (v->ttbl->copy == other->ttbl->copy)) {
        // if v has a copy function defined in its ttbl,
        // other has a copy function defined in its ttbl,
        // and v shares the same copy function with other,
        // do a deep copy of other's elements into v.

        // a deep copy from other to v for types with
        // dynamically allocated memory means that clearing
        // v does not affect other whatsoever.
        while (other->impl.finish != sentinel) {
            v->ttbl->copy(v->impl.finish, other->impl.finish);

            v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            other->impl.finish = (char *)(other->impl.finish) + (v->ttbl->width);
        }
    } else {
        // if either v/other lack a copy function,
        // and/or they do not share the same copy function,
        // a shallow copy of other's elements into v will occur.

        // a shallow copy from other to v for types with
        // dynamically allocated memory means that clearing
        // v will clear the memory associated with the elements
        // that were shallow copied from other to v.
        while (other->impl.finish != sentinel) {
            memcpy(v->impl.finish, other->impl.finish, v->ttbl->width);

            v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            other->impl.finish = (char *)(other->impl.finish) + (v->ttbl->width);
        }
    }

    return v;
}

/**
 *  @brief  Reverses the contents of v
 *
 *  @param[in]  v   pointer to vector
 */
void v_reverse(vector *v) {
    assert(v);

    const size_t size = v_size(v);
    void *back = (char *)(v->impl.finish) - (v->ttbl->width);

    // saving the address of the finish pointer for later,
    // as the finish pointer will be set to the beginning of the vector
    void *restore = v->impl.finish;
    v->impl.finish = v->impl.start;

    while (v->impl.finish != back) {
        // swap addresses at finish and back
        v_swap_addr(v, v->impl.finish, back);

        // increment finish, decrement back --
        // until finish's address matches that of back
        v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
        back = (char *)(back) - (v->ttbl->width);
    }

    // restoring the finish pointer - size of vector is the same
    // (the first swap ensures that restore is the desired finish position)
    v->impl.finish = restore;
}

/**
 *  @brief  Returns a new vector with the contents of base
 *
 *  @param[in]  ttbl    typetable matching that of bases's element type
 *  @param[in]  base    base address of an array to copy
 *  @param[in]  length  logical length of base
 *
 *  @return     pointer to vector with contents of base
 */
vector *v_arrtov(struct typetable *ttbl, void *base, size_t length) {
    assert(base);

    // an appropriate typetable must be chosen that matches
    // that of the type T for base.
    struct typetable *table = ttbl ? ttbl : _void_ptr_;
    vector *v = v_newr(table, length);

    void *target = base;

    if (v->ttbl->copy) {
        // deep copy
        while (v->impl.finish != v->impl.end_of_storage) {
            v->ttbl->copy(v->impl.finish, target);

            target = (char *)(target) + (v->ttbl->width);
            v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
        }
    } else {
        // shallow copy
        while (v->impl.finish != v->impl.end_of_storage) {
            memcpy(v->impl.finish, target, v->ttbl->width);

            target = (char *)(target) + (v->ttbl->width);
            v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
        }
    }

    return v;
}

/**
 *  @brief  Performs a linear search to find valaddr using the ttbl->compare function
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  valaddr address of a copy of an element to find
 */
int v_search(vector *v, const void *valaddr) {
    assert(v);
    assert(valaddr);

    int (*comparator)(const void *, const void *) =
    v->ttbl->compare ? v->ttbl->compare : void_ptr_compare;

    void *curr = v->impl.start;
    bool found = false;
    int result = 0;

    // standard linear search of valaddr using comparator
    while (curr != v->impl.finish) {
        if (comparator(curr, valaddr) == 0) {
            found = true;
            break;
        }

        curr = (char *)(curr) + (v->ttbl->width);
        ++result;
    }

    // if found, return result, else (-1) to denote not found
    return found ? result : -1;
}

/**
 *  @brief  Sorts the contents of v using ttbl->compare
 *
 *  @param[in]  v   pointer to vector
 */
void v_sort(vector *v) {
    assert(v);

    size_t size = v_size(v);

    if (size < 2) {
        // why sort a data structure if size < 2?
        return;
    }

    int (*comparator)(const void *, const void *)
    = v->ttbl->compare ? v->ttbl->compare : void_ptr_compare;

    // cstdlib qsort (best performance)
    //qsort(v->base, size, v->ttbl->width, comparator);

    // cstdlib mergesort
    //mergesort(v->impl.start, size, v->ttbl->width, comparator);

    // gcslib mergesort
    v_mergesort_iterative(v->impl.start, size, v->ttbl->width, comparator);
}

/**
 *  @brief  Prints a diagnostic of vector to stdout
 *
 *  @param[in]  v   pointer to vector
 */
void v_puts(vector *v) {
    // redirect to v_fputs with stream stdout
    v_fputs(v, stdout);
}

/**
 *  @brief  Prints the contents of vector with user-defined formatting
 *
 *  @param[in]  v           pointer to vector
 *  @param[in]  before      string that appears before any elements appear
 *  @param[in]  after       string that appears after all the elements have appeared
 *  @param[in]  postelem    string that appears after each element, except the last one
 *  @param[in]  breaklim    amount of elements that print before a line break occurs.
 *                          0 means no line breaks
 */
void v_putsf(vector *v,
             const char *before,
             const char *after,
             const char *postelem,
             const char *empty,
             size_t breaklim) {
    // redirect to v_fputsf with stream stdout
    v_fputsf(v, stdout, before, after, postelem, empty, breaklim);
}

/**
 *  @brief  Prints a diagnostic of vector to file stream dest
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  dest    file stream (e.g stdout, stderr, a file)
 */
void v_fputs(vector *v, FILE *dest) {
    assert(v);
    assert(dest);

    char buffer1[MAXIMUM_STACK_BUFFER_SIZE];
    char buffer2[MAXIMUM_STACK_BUFFER_SIZE];

    const char *link = "------------------------------";

    sprintf(buffer1, "\n%s\n%s\n%s\n", link, "Elements", link);

    const char *bytes_label = v->ttbl->width == 1 ? "byte" : "bytes";

    sprintf(buffer2, "%s\n%s\t\t%lu\n%s\t%lu\n%s\t%lu %s\n%s\n", link, "Size",
            v_size(v), "Capacity", v_capacity(v), "Element size", v->ttbl->width,
            bytes_label, link);

    const char *postelem = "";
    const char *empty = "--- Container is empty ---";
    const size_t breaklim = 1;

    v_fputsf(v, dest, buffer1, buffer2, postelem, empty, breaklim);
}

/**
 *  @brief  Prints the contents of vector with user-defined formatting,
 *          to file stream dest
 *
 *  @param[in]  v           pointer to vector
 *  @param[in]  dest        file stream (e.g. stdout, stderr, a file)
 *  @param[in]  before      string that appears before any elements appear
 *  @param[in]  after       string that appears after all the elements have appeared
 *  @param[in]  postelem    string that appears after each element, except the last one
 *  @param[in]  breaklim    amount of elements that print before a line break occurs.
 *                          0 means no line breaks
 */
void v_fputsf(vector *v, FILE *dest,
              const char *before,
              const char *after,
              const char *postelem,
              const char *empty,
              size_t breaklim) {
    assert(v);
    assert(dest);

    fprintf(dest, "%s", before ? before : "");

    void (*print)(const void *, FILE *dest)
    = v->ttbl->print ? v->ttbl->print : void_ptr_print;

    size_t size = v_size(v);

    if (size == 0) {
        fprintf(dest, "%s\n", empty ? empty : "");
    } else {
        void *target = v->impl.start;

        for (size_t i = 0, curr = 1; i < size; i++, curr++) {
            print(target, dest);

            // address - disable for release
            fprintf(dest, "\t\t(%s%p%s)", KCYN, target, KNRM);

            if (i < size - 1) {
                fprintf(dest, "%s", postelem ? postelem : "");
            }

            if (curr == breaklim) {
                curr = 0;
                fprintf(dest, "\n");
            }

            target = target + v->ttbl->width;
        }
    }

    fprintf(dest, "%s", after ? after : "");
}

/**
 *  @brief  Wrapper function for a struct typetable
 *
 *  @param[in]  arg     address of a vector pointer
 *  @param[in]  other   address of a vector pointer
 *
 *  @return     a pointer to vector
 */
void *vector_copy(void *arg, const void *other) {
    assert(other);

    vector **dest = (vector **)(arg);
    vector **source = (vector **)(other);

    (*dest) = v_newcopy((*source));

    return (*dest);
}

/**
 *  @brief  Wrapper function for a struct typetable
 *
 *  @param[in]  arg     address of a vector pointer
 */
void vector_dtor(void *arg) {
    assert(arg);

    vector **v = (vector **)(arg);
    v_delete(v);
}

/**
 *  @brief  Wrapper function for a struct typetable
 *
 *  @param[in]  s1  address of a vector pointer
 *  @param[in]  s2  address of a vector pointer
 */
void vector_swap(void *s1, void *s2) {
    vector **v1 = (vector **)(s1);
    vector **v2 = (vector **)(s2);

    if ((*v1)) {
        v_swap(v1, v2);
    } else {
        (*v1) = (*v2);
        (*v2) = NULL;
    }
}

/**
 *  @brief  Wrapper function for a struct typetable
 *
 *  @param[in]  c1  address of a vector pointer
 *  @param[in]  c2  address of a vector pointer
 *
 *  @return     -1 if c1 and c2 do not share a comparison function
 *              otherwise, accumulated comparison results between
 *              c1 and c2's elements, within their common length
 *              0 means they are both equivalent, within their common length.
 */
int vector_compare(const void *c1, const void *c2) {
    assert(c1);
    assert(c2);

    vector *v1 = *(vector **)(c1);
    vector *v2 = *(vector **)(c2);

    if (v1->ttbl->compare != v2->ttbl->compare) {
        return -1;
    }

    vector *vec1 = v_newcopy(v1);
    vector *vec2 = v_newcopy(v2);

    v_sort(vec1);
    v_sort(vec2);

    size_t size1 = v_size(vec1);
    size_t size2 = v_size(vec2);

    size_t size = size1 < size2 ? size1 : size2;

    void *target1 = vec1->impl.start;
    void *target2 = vec2->impl.start;

    int delta = 0;
    for (int i = 0; i < size; i++) {
        delta += vec1->ttbl->compare(&target1, &target2);

        target1 = (char *)(target1) + (vec1->ttbl->width);
        target2 = (char *)(target2) + (vec2->ttbl->width);
    }

    v_delete(&vec2);
    v_delete(&vec1);

    // if delta == 0, both vectors are equivalent within their common length.
    return delta;
}

/**
 *  @brief  Wrapper function for a struct typetable
 *
 *  @param[in]  arg     address of a vector pointer
 *  @param[in]  dest    file stream (stdout, stderr, a file)
 */
void vector_print(const void *arg, FILE *dest) {
    assert(arg);
    assert(dest);

    vector *v = *(vector **)(arg);
    v_fputs(v, dest);
}

/**
 *  @brief  Retrieves width (data size) in v's ttbl
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return     size of data type
 */
size_t v_get_width(vector *v) {
    assert(v);
    return v->ttbl->width;
}

/**
 *  @brief  Retrieves copy function in v's ttbl
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return     copy function used by v
 */
copy_fn v_get_copy(vector *v) {
    assert(v);
    return v->ttbl->copy;
}

/**
 *  @brief  Retrieves dtor function in v's ttbl
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return     dtor function used by v
 */
dtor_fn v_get_dtor(vector *v) {
    assert(v);
    return v->ttbl->dtor;
}

/**
 *  @brief  Retrieves swap function in v's ttbl
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return     swap function used by v
 */
swap_fn v_get_swap(vector *v) {
    assert(v);
    return v->ttbl->swap;
}

/**
 *  @brief  Retrieves compare function in v's ttbl
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return     compare function used by v
 */
compare_fn v_get_compare(vector *v) {
    assert(v);
    return v->ttbl->compare;
}

/**
 *  @brief  Retrieves print function in v's ttbl
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return     print function used by v
 */
print_fn v_get_print(vector *v) {
    assert(v);
    return v->ttbl->print;
}

/**
 *  @brief  Retrieves typetable used by v
 *
 *  @param[in]  v   pointer to vector
 *
 *  @return     pointer to typetable
 */
struct typetable *v_get_ttbl(vector *v) {
    assert(v);
    return v->ttbl;
}

/**
 *  @brief  Calls malloc to allocate memory for a pointer to vector
 *
 *  @return     pointer to vector
 */
static vector *v_allocate(void) {
    vector *v = NULL;
    v = malloc(sizeof *v);
    return v;
}

/**
 *  @brief  "Constructor" function, initializes vector
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  ttbl    pointer to typetable; width/copy/dtor/swap/compare/dtor
 *  @param[in]  capacity    capacity desired for vector
 */
static void v_init(vector *v, struct typetable *ttbl, size_t capacity) {
    assert(v);

    v->ttbl = ttbl ? ttbl : _void_ptr_;

    if (v->ttbl != _void_ptr_) {
        v->ttbl->compare = v->ttbl->compare ? v->ttbl->compare : NULL;
        v->ttbl->copy = v->ttbl->copy ? v->ttbl->copy : NULL;
        v->ttbl->print = v->ttbl->print ? v->ttbl->print : NULL;
        v->ttbl->dtor = v->ttbl->dtor ? v->ttbl->dtor : NULL;
    }

    capacity = (capacity <= 0) ? 1 : capacity;

    void *start = NULL;
    start = calloc(capacity, v->ttbl->width);
    assert(start);

    v->impl.start = start;
    v->impl.finish = v->impl.start;

    v->impl.end_of_storage
    = (char *)(v->impl.start) + (capacity * v->ttbl->width);
}

/**
 *  @brief "Destructor" function, deinitializes vector
 *
 *  @param[in]  v   pointer to vector
 */
static void v_deinit(vector *v) {
    if (v == NULL) {
        return;
    }

    v_clear(v);

    free(v->impl.start);
    v->impl.start = NULL;
    v->impl.finish = NULL;
    v->impl.end_of_storage = NULL;

    v->ttbl = NULL;
}

/**
 *  @brief  Swaps the content at first and second, address from within v
 *
 *  @param[in]  v       pointer to vector
 *  @param[out] first   first address to swap content
 *  @param[out] second  second address to swap content
 */
static void v_swap_addr(vector *v, void *first, void *second) {
    assert(v);
    assert(first);
    assert(second);

    void *temp = NULL;
    temp = malloc(v->ttbl->width);
    assert(temp);
    memcpy(temp, first, v->ttbl->width);

    memcpy(first, second, v->ttbl->width);
    memcpy(second, temp, v->ttbl->width);

    free(temp);
    temp = NULL;
}

/**
 *  @brief  Initializes and returns an iterator that refers to arg
 *
 *  @param[in]  arg     pointer to vector
 *
 *  @return     iterator that refers to v,
 *              position is at v's first element
 */
static iterator vi_begin(void *arg) {
    assert(arg);

    vector *v = (vector *)(arg);

    iterator it;
    it.itbl = _vector_iterator_;
    it.container = v;
    it.curr = v->impl.start;

    return it;
}

/**
 *  @brief  Initializes and returns an iterator that refers to arg
 *
 *  @param[in]  arg     pointer to vector
 *
 *  @return     iterator that refers to v;
 *              position is at one block past v's last element
 */
static iterator vi_end(void *arg) {
    assert(arg);

    vector *v = (vector *)(arg);

    iterator it;
    it.itbl = _vector_iterator_;
    it.container = v;
    it.curr = v->impl.finish;

    return it;
}

/**
 *  @brief  Initializes and returns an iterator that
 *          is one block past it's current position
 *
 *  @param[in]  it      iterator that refers to a vector
 *
 *  @return     a new iterator that is one block past it's current position
 */
static iterator vi_next(iterator it) {
    vector *v = (vector *)(it.container);

    iterator iter;
    iter.itbl = _vector_iterator_;
    iter.container = v;
    iter.curr = it.curr;

    iter.curr = (char *)(iter.curr) + (v->ttbl->width);

    return iter;
}

/**
 *  @brief  Initializes and returns an iterator that
 *          is n blocks past it's current position
 *
 *  @param[in]  it      iterator that refers to a vector
 *
 *  @return     a new iterator that is n block's past it's current position
 */
static iterator vi_next_n(iterator it, int n) {
    vector *v = (vector *)(it.container);

    iterator iter;
    iter.itbl = _vector_iterator_;
    iter.container = v;
    iter.curr = it.curr;

    iter.curr = (char *)(iter.curr) + (n * v->ttbl->width);

    return iter;
}

/**
 *  @brief  Initializes and returns an iterator that
 *          is one block behind it's current position
 *
 *  @param[in]  it      iterator that refers to a vector
 *
 *  @return     a new iterator that is one block behind it's current position
 */
static iterator vi_prev(iterator it) {
    vector *v = (vector *)(it.container);

    iterator iter;
    iter.itbl = _vector_iterator_;
    iter.container = v;
    iter.curr = it.curr;

    iter.curr = (char *)(iter.curr) - (v->ttbl->width);

    return iter;
}

/**
 *  @brief  Initializes and returns an iterator that
 *          is n blocks behind it's current position
 *
 *  @param[in]  it      iterator that refers to a vector
 *
 *  @return     a new iterator that is n block's behind it's current position
 */
static iterator vi_prev_n(iterator it, int n) {
    vector *v = (vector *)(it.container);

    iterator iter;
    iter.itbl = _vector_iterator_;
    iter.container = v;
    iter.curr = it.curr;

    iter.curr = (char *)(iter.curr) - (n * v->ttbl->width);

    return iter;
}

/**
 *  @brief  Determines the distance between first and last numerically
 *
 *  @param[in]  first   pointer to iterator that refers to a vector
 *  @param[in]  last    pointer to iterator that refers to a vector
 *
 *  @return     numerical distance between first and last
 *
 *  This function can also be used to determine the numerical position
 *  of a particular iterator, since indices are not stored within iterators.
 *
 *  To find the index position of an iterator, leave one of the parameters
 *  NULL when calling the distance function.
 */
static int vi_distance(iterator *first, iterator *last) {
    vector *v = NULL;

    if (first == NULL && last != NULL) {
        v = (vector *)(last->container);
        return (int)(ptr_distance(v->impl.start, last->curr, v->ttbl->width));
    } else if (last == NULL && first != NULL) {
        v = (vector *)(first->container);
        return (int)(ptr_distance(v->impl.start, first->curr, v->ttbl->width));
    } else if (first == NULL && last == NULL) {
        return 0;
    } else {
        v = (vector *)(first->container);
        return (int)(ptr_distance(first->curr, last->curr, v->ttbl->width));
    }
}

/**
 *  @brief  Advances the position of it n blocks
 *
 *  @param[in]  it      pointer to iterator that refers to a vector
 *  @param[in]  n       desired amount of blocks to move
 *
 *  @return     pointer to iterator
 */
static iterator *vi_advance(iterator *it, int n) {
    assert(it);

    vector *v = (vector *)(it->container);
    it->curr = (char *)(it->curr) + (n * v->ttbl->width);
    return it;
}


/**
 *  @brief  Increments the position of it 1 block forward
 *
 *  @param[in]  it     pointer to iterator that refers to a vector
 *
 *  @return     pointer to iterator
 */
static iterator *vi_incr(iterator *it) {
    assert(it);

    vector *v = (vector *)(it->container);

    it->curr = (char *)(it->curr) + (v->ttbl->width);
    return it;
}

/**
 *  @brief  Decrements the position of it 1 block backward
 *
 *  @param[in]  it     pointer to iterator that refers to a vector
 *
 *  @return     pointer to iterator
 */
static iterator *vi_decr(iterator *it) {
    assert(it);

    vector *v = (vector *)(it->container);

    it->curr = (char *)(it->curr) - (v->ttbl->width);
    return it;
}

/**
 *  @brief  Retrieves the address of the value referred to
 *          by it's current position
 *
 *  @param[in]  it  iterator that refers to a vector
 *
 *  @return     address of an element from within a vector
 */
static void *vi_curr(iterator it) {
    return it.curr;
}

/**
 *  @brief  Retrieves the address of first element from
 *          the vector it is iterating
 *
 *  @param[in]  it  iterator that refers to a vector
 *
 *  @return     address of the first element from within a vector
 */
static void *vi_start(iterator it) {
    vector *v = (vector *)(it.container);
    return v->impl.start;
}

/**
 *  @brief  Retrieves the address of the block one past
 *          the last element from within the vector it is iterating
 *
 *  @param[in]  it  iterator that refers to a vector
 *
 *  @return     address of the element that is one block past
 *              the rear element from within the vector being iterated
 */
static void *vi_finish(iterator it) {
    vector *v = (vector *)(it.container);
    return v->impl.finish;
}

/**
 *  @brief  Determines if it has elements to visit in the forward direction
 *
 *  @param[in]  it  iterator that refers to a vector
 *
 *  @return     true if elements remain in the forward direction,
 *              false otherwise
 */
static bool vi_has_next(iterator it) {
    vector *v = (vector *)(it.container);
    return it.curr != v->impl.finish;
}

/**
 *  @brief  Determines if it has elements to visit in the backward direction
 *
 *  @param[in]  it  iterator that refers to a vector
 *
 *  @return     true if elements remain in the backward direction,
 *              false otherwise
 */
static bool vi_has_prev(iterator it) {
    vector *v = (vector *)(it.container);
    return it.curr != v->impl.start;
}

/**
 *  @brief  Retrieve a container's typetable
 *
 *  @param[in]  arg     pointer to vector
 *
 *  @return     pointer to typetable
 */
static struct typetable *vi_get_ttbl(void *arg) {
    vector *v = (vector *)(arg);
    return v->ttbl;
}
