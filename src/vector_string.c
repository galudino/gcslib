/**
 *  @file       vector_str.c
 *  @brief      Source for preprocessed template instantiation of vector(str)
 *
 *  @author     Gemuele Aludino
 *  @date       21 Sep 2019
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

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "vector_string.h"

/**
 *  @file       utils.h
 *  @brief      Required for (struct typetable) and related functions
 */
#include "utils.h"

/**
 *  @file       iterator.h
 *  @brief      Required for iterator (struct iterator) and related functions
 */
#include "iterator.h"

/**
 *  @file       mergesort.h
 *  @brief      Public header file for access to mergesort-related functions
 */
#include "mergesort.h"

/**
 *  @def        VECTOR_TMPL_MAXIMUM_STACK_BUFFER_SIZE
 *  @brief      Arbitrary maximum size for a statically allocated (char) buffer
 */
#define VECTOR_TMPL_MAXIMUM_STACK_BUFFER_SIZE 16384
#define VECTOR_DEFAULT_CAPACITY 16

/* optional macros for accessing the innards of vector_base */
#define AT(VEC, INDEX)  ((VEC->impl.start) + (INDEX))
#define FRONT(VEC)      ((VEC->impl.start))
#define BACK(VEC)       ((VEC->impl.finish) - 1)
#define END(VEC)        (VEC->impl.end_of_storage)

/**
 *  @struct     vector_str
 *  @brief      Represents a type-safe dynamic array ADT
 *
 *  Note that struct vector_str and struct vector_base_str are
 *  opaque -- their fields cannot be accessed directly,
 *  nor can instances of struct vector_str/struct vector_base_str
 *  be created on the stack. This is done to enforce encapsulation.
 */
struct vector_str {
    struct vector_base_str {
        str *start;           /**< address of array base (first element) */
        str *finish;          /**< address reserved for next rear element */
        str *end_of_storage;  /**< addresses last allocated block of storage */
    } impl;

    struct typetable *ttbl; /*<< data width, cpy, dtor, swap, compare, print */
};

struct typetable table_id(ttbl_vector, str) = {
    sizeof(struct vector_str),
    tmpl_vector_copy_str,
    tmpl_vector_dtor_str,
    tmpl_vector_swap_str,
    tmpl_vector_compare_str,
    tmpl_vector_print_str
};

struct typetable *vector_typetable_ptr_id_str = &table_id(ttbl_vector, str);

static vector_str *vallocate_str(void);
static void vinit_str(vector_str *v, size_t capacity);
static void vdeinit_str(vector_str *v);
static void vswapaddr_str(vector_str *v, str *first, str *second);

static iterator vibegin_str(void *arg);
static iterator viend_str(void *arg);

static iterator vinext_str(iterator it);
static iterator vinextn_str(iterator it, int n);

static iterator viprev_str(iterator it);
static iterator viprevn_str(iterator it, int n);

static int vidistance_str(iterator *first, iterator *last);

static iterator *viadvance_str(iterator *it, int n);
static iterator *viincr_str(iterator *it);
static iterator *videcr_str(iterator *it);

static void *vicurr_str(iterator it);
static void *vistart_str(iterator it);
static void *vifinish_str(iterator it);

static bool vihasnext_str(iterator it);
static bool vihasprev_str(iterator it);

static struct typetable *vigetttbl_str(void *arg);

struct iterator_table table_id(itbl_vector, str) = {
    vibegin_str,
    viend_str,
    vinext_str,
    vinextn_str,
    viprev_str,
    viprevn_str,
    viadvance_str,
    viincr_str,
    videcr_str,
    vicurr_str,
    vistart_str,
    vifinish_str,
    vidistance_str,
    vihasnext_str,
    vihasprev_str,
    vigetttbl_str
};

struct iterator_table *vector_iterator_table_ptr_id_str = &table_id(itbl_vector, str);

/**
 *  @brief  Allocates, constructs, and returns a pointer to vector_str,
 *          size VECTOR_DEFAULT_CAPACITY (16)
 *
 *  @return     pointer to vector_str
 */
vector_str *vnew_str(void) {
    vector_str *v = vallocate_str();  /* allocate */
    vinit_str(v, VECTOR_DEFAULT_CAPACITY); /* construct */
    return v;                                   /* return */
}

/**
 *  @brief  Allocates, constructs, and returns a pointer to vector_str,
 *          size n
 *
 *  @return     pointer to vector_str
 */
vector_str *vnewr_str(size_t n) {
    vector_str *v = vallocate_str();/* allocate */
    vinit_str(v, n);                     /* construct */
    return v;                                 /* return */
}

/**
 *  @brief  Calls vnewr and returns a pointer to vector_str,
 *          filled with n copies of val
 *
 *  @param[in]  n       number of elements to copy
 *  @param[in]  val     element to copy
 *
 *  @return     pointer to vector_str
 *
 *  If ttbl has a copy function defined,
 *  n copies of val will be inserted into the returned vector
 *  using the copy function - this copy function is intended for
 *  deep copies. Otherwise, val will be shallow-copied using memcpy.
 */
vector_str *vnewfill_str(size_t n, str val) {
    vector_str *v = NULL;
    str *sentinel = NULL;

    v = vnewr_str(n);
    sentinel = v->impl.start + n;

    if (v->ttbl->copy) {
        /**
         *  If copy function defined in ttbl,
         *  instances of valaddr are deep copied
         */
        while (v->impl.finish != sentinel) {
            v->ttbl->copy(v->impl.finish++, &val);
        }
    } else {
        /**
         *  If no copy function defined in ttbl,
         *  instances of valaddr are shallow copied
         */
        while (v->impl.finish != sentinel) {
            memcpy(v->impl.finish++, &val, v->ttbl->width);
        }
    }

    return v;
}

/**
 *  @brief  Calls vnewr and returns a pointer to vector_str,
 *          filled with n copies of valaddr
 *
 *  @param[in]  n       number of elements to copy
 *  @param[in]  valaddr address of element to copy
 *
 *  @return     pointer to vector_str
 *
 *  If ttbl has a copy function defined,
 *  n copies of valaddr will be inserted into the returned vector
 *  using the copy function - this copy function is intended for
 *  deep copies. Otherwise, valaddr will be shallow-copied using memcpy.
 */
vector_str *vnewfillptr_str(size_t n, str *valaddr) {
    vector_str *v = NULL;
    str *sentinel = NULL;

    v = vnewr_str(n);
    sentinel = v->impl.start + n;

    if (v->ttbl->copy) {
        /**
         *  If copy function defined in ttbl,
         *  instances of valaddr are deep copied
         */
        while (v->impl.finish != sentinel) {
            v->ttbl->copy(v->impl.finish++, valaddr);
        }
    } else {
        /**
         *  If no copy function defined in ttbl,
         *  instances of valaddr are shallow copied
         */
        while (v->impl.finish != sentinel) {
            memcpy(v->impl.finish++, valaddr, v->ttbl->width);
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
 *  @return     pointer to vector_str
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
vector_str *vnewrnge_str(iterator first, iterator last) {
    int delta = 0;

    struct typetable *ttbl_first = NULL;
    vector_str *v = NULL;

    str *sentinel = NULL;
    str *curr = NULL;

    if (first.itbl != last.itbl) {
        /**
         *  Iterators first and last must refer to the same container,
         *  or else the cursor pointer will never meet the sentinel pointer
         *  that will end the copy loop.
         * 
         *  This also means that iterators first and last cannot refer to
         *  different container types (this is determiend by what itbls
         *  they each point to)
         */
        ERROR(__FILE__, "first and last must have matching container types and refer to the same container.");
        return NULL;
    }

    delta = it_distance(&first, &last);
    ttbl_first = it_get_ttbl(first);

    v = vnewr_str(delta);

    sentinel = it_curr(last);         /* iteration range is [first, last) */

    if (ttbl_first->copy) {
        while ((curr = it_curr(first)) != sentinel)  {
            ttbl_first->copy(v->impl.finish++, curr);

            it_incr(&first);
        }
    } else {
        while ((curr = it_curr(first)) != sentinel)  {
            memcpy(v->impl.finish++, curr, v->ttbl->width);

            it_incr(&first);
        };
    }

    return v;
}

/**
 *  @brief  Calls vnewr and returns a pointer to vector_str,
 *          filled with elements from an existing vector_str v.
 *
 *  @param[in]  v   pointer to vector_str, containing the desired source elements
 *
 *  @return     pointer to vector, with copies of v's elements
 *
 *  If v uses a ttbl that has a copy function defined,
 *  elements from v will be copied to the returned vector using the
 *  copy function -- this function is meant for deep copying.
 *  Otherwise, the contents of v will be shallow-copied using memcpy.
 */
vector_str *vnewcopy_str(vector_str *v) {
    vector_str *copy = NULL;

    str *sentinel = NULL;
    str *curr = NULL;

    massert_container(v);

    copy = vnewr_str(vcapacity_str(v));

    sentinel = copy->impl.finish + vsize_str(v);
    curr = v->impl.start;

    if (v->ttbl->copy) {
        while (copy->impl.finish != sentinel) {
            copy->ttbl->copy(copy->impl.finish++, curr++);
        }
    } else {
        while (copy->impl.finish != sentinel) {
            memcpy(copy->impl.finish++, curr++, v->ttbl->width);
        }
    }

    return copy;
}

/**
 *  @brief  Calls vallocate_str to allocate a new vector_str, named move,
 *          and transfers ownership of v's fields to vector_str move.
 *
 *  @param[out] v   Address of a pointer to vector_str
 *
 *  @return     pointer to vector_str, initialized with v's fields
 *
 *  vector v is reinitialized with the same ttbl, with size 1.
 *  v may be deleted by the client, or reused.
 */
vector_str *vnewmove_str(vector_str **v) {
    vector_str *move = NULL;

    massert_ptr((*v));

    move = vallocate_str();

    /**
     *  A new vector, move, is constructed from the contents of an existing vector,
     *  by a change of ownership between impl pointers.
     *  The previous vector, v, is initialized to have a size of 1 --
     *  v may be deleted clientside if preferred.
     */
    move->impl.start = (*v)->impl.start;
    move->impl.finish = (*v)->impl.finish;
    move->impl.end_of_storage = (*v)->impl.end_of_storage;
    move->ttbl = (*v)->ttbl;

    vinit_str((*v), 1);

    return move;
}

/**
 *  @brief  Calls vdeinit_str (vector_str's destructor) and deallocates the pointer v.
 *
 *  @param[out] v   Address of a pointer to vector_str
 *
 *  Every call to any of vector_str's "new" functions should be accompanied
 *  by a call to vdelete_str when a pointer to vector_str is no longer needed.
 *
 *  If the vector_str has a ttbl with a dtor function defined,
 *  the elements within the vector_str will be destroyed using the dtor function.
 *
 *  Note that if the elements from within v are dynamically allocated,
 *  and/or the elements have dynamically allocated fields --
 *  and there is no dtor function defined, memory management
 *  will become the client's responsibility.
 */
void vdelete_str(vector_str **v) {
    massert_ptr((*v));

    /**
     *  Deinitialization involves releasing all dynamically allocated
     *  memory at each field (if dtor function is defined in ttbl,
     *  and is written to release such memory) --
     *  after that step (or if there is no memory to free within the elements),
     *  (*v)->impl.start has its memory freed, and the remainder of (*v)'s fields
     *  are set NULL.
     */
    vdeinit_str((*v));

    /* finally, the memory (*v) points to will be freed. */
    free((*v));
    (*v) = NULL;
}

/**
 *  @brief  Returns an iterator that points to the first element in vector_str
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return     iterator that refers to v
 */
iterator vbegin_str(vector_str *v) {
    massert_container(v);
    return vibegin_str(v);
}

/**
 *  @brief  Returns an iterator that pointers to an element that is one block
 *          past the last element in v
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return     iterator that refers to v
 */
iterator vend_str(vector_str *v) {
    massert_container(v);
    return viend_str(v);
}

/**
 *  @brief  Returns the logical length of v
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return     logical length of v
 */
size_t vsize_str(vector_str *v) {
    massert_container(v);
    return v->impl.finish - v->impl.start;
}

/**
 *  @brief  Returns the theoretical maximum amount of elements that
 *          can be stored by vector_str
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return     theoretical maximum logical length of v
 */
size_t vmaxsize_str(vector_str *v) {
    size_t ptr_sz = 0;

    massert_container(v);

    /* ptr_sz is 8 bytes on 64 bit system, 4 bytes on 32 bit system */
    ptr_sz = (sizeof(void *) == 8) ? 8 : 4;
    return ((pow)(2.0, (ptr_sz * 8)) / (v->ttbl->width)) - 1;
}

/**
 *  @brief  Resizes the vector_str to size n
 *
 *  @param[in]  v   pointer to vector_str
 *  @param[in]  n   desired size for v
 *
 *  If n is less than the current logical length (vsize_str(v)),
 *  v will be truncated (excess elements are destroyed),
 *  otherwise, v's capacity will be extended.
 */
void vresize_str(vector_str *v, size_t n) {
    size_t old_size = 0;
    size_t old_capacity = 0;

    str *target = NULL;
    str *newstart = NULL;

    int back_index = 0;
    int i = 0;

    size_t fin = 0;
    size_t end = 0;
    
    massert_container(v);

    old_size = vsize_str(v);
    old_capacity = vcapacity_str(v);

    if (old_capacity == n) {
        return;
    } else if (n == 0) {
        WARNING(__FILE__, "vresize must receive a nonzero value for n.");
    }

    /**
     *  If n is less than the old size, and items were deep copied --
     *  dynamically allocated memory will be released as per the dtor function
     *  prior to truncation
     *  (if dtor is defined in ttbl, copy should be defined as well)
     */
    if (n < old_size && v->ttbl->dtor) {
        target = v->impl.start + n;

        back_index = n - 1;
        for (i = 0; i < back_index; i++) {
            v->ttbl->dtor(target--);
        }
    }

    newstart = realloc(v->impl.start, n * v->ttbl->width);
    massert_realloc(newstart);

    fin = n > old_size ? old_size : n;
    end = n > old_size ? n : fin;

    v->impl.start = newstart;
    v->impl.finish = v->impl.start + fin;
    v->impl.end_of_storage = v->impl.start + end;
}

/**
 *  @brief  Resizes the vector_str to size n elements.
 *
 *  @param[in]  v           pointer to vector_str
 *  @param[in]  n           desired size for v
 *  @param[in]  val         element to fill new blocks with
 *
 *  If n exceeds the size/capacity of v, val will be copied
 *  to the new blocks for the resize.
 *
 *  If n is less than or equal to the size of v,
 *  the vector_str will be cleared and new memory of size n will be
 *  initialized, with each block consisting of copies of val.
 */
void vresizefill_str(vector_str *v, size_t n, str val) {
    size_t old_capacity = 0;

    str *sentinel = NULL;
    str *newstart = NULL;

    massert_container(v);

    old_capacity = vcapacity_str(v);

    sentinel = v->impl.start + n;

    if (n > old_capacity) {
        /**
         *  If n is greater than the old size, multiple instances of valaddr are
         *  appended to the rear of the vector following a resize.
         */
        vresize_str(v, n);

        /* sentinel must be updated to reflect the resize. */
        sentinel = v->impl.start + n;

        if (v->ttbl->copy) {
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish++, &val);
            }
        } else {
            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish++, &val, v->ttbl->width);
            }
        }
    } else {
        /**
         *  If n is less than or equal to the old size,
         *  and dtor is defined in ttbl,
         *  then dynamically allocated memory will be released
         *  at each element, as per the dtor function.
         */
        if (v->ttbl->dtor) {
            --v->impl.finish;

            while (v->impl.finish != v->impl.start) {
                v->ttbl->dtor(v->impl.finish--);
            }

            v->ttbl->dtor(v->impl.finish);
        }

        /* memory at the base address of the vector will be released... */
        free(v->impl.start);
        v->impl.start = NULL;

        /**
         *  ...and new memory will be allocated of size n to represent
         *  the new base address of the vector.
         *  (alternatively, v->impl.start have also been realloc'ed to size n),
         *  but if n is less than or equal to the old size, the old elements
         *  will be overwritten anyway.
         */
        newstart = calloc(n, v->ttbl->width);
        massert_calloc(newstart);

        /* pointers at impl are re-established */
        v->impl.start = newstart;
        v->impl.finish = v->impl.start;
        v->impl.end_of_storage = v->impl.start + n;

        sentinel = v->impl.end_of_storage;

        if (v->ttbl->copy) {
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish++, &val);
            }
        } else {
            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish++, &val, v->ttbl->width);
            }
        }
    }
}

/**
 *  @brief  Resizes the vector_str to size n elements.
 *
 *  @param[in]  v           pointer to vector_str
 *  @param[in]  n           desired size for v
 *  @param[in]  valaddr     address of element to fill new blocks with
 *
 *  If n exceeds the size/capacity of v, valaddr will be copied
 *  to the new blocks for the resize.
 *
 *  If n is less than or equal to the size of v,
 *  the vector_str will be cleared and new memory of size n will be
 *  initialized, with each block consisting of copies of valaddr.
 */
void vresizefillptr_str(vector_str *v, size_t n, str *valaddr) {
    size_t old_capacity = 0;

    str *sentinel = NULL;
    str *newstart = NULL;

    massert_container(v);

    old_capacity = vcapacity_str(v);

    sentinel = v->impl.start + n;

    if (n > old_capacity) {
        /**
         *  If n is greater than the old size, multiple instances of valaddr are
         *  appended to the rear of the vector following a resize.
         */
        vresize_str(v, n);

        /* sentinel must be updated to reflect the resize. */
        sentinel = v->impl.start + n;

        if (v->ttbl->copy) {
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish++, valaddr);
            }
        } else {
            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish++, valaddr, v->ttbl->width);
            }
        }
    } else {
        /**
         *  If n is less than or equal to the old size,
         *  and dtor is defined in ttbl,
         *  then dynamically allocated memory will be released
         *  at each element, as per the dtor function.
         */
        if (v->ttbl->dtor) {
            --v->impl.finish;

            while (v->impl.finish != v->impl.start) {
                v->ttbl->dtor(v->impl.finish--);
            }

            v->ttbl->dtor(v->impl.finish);
        }

        /* memory at the base address of the vector will be released... */
        free(v->impl.start);
        v->impl.start = NULL;

        /**
         *  ...and new memory will be allocated of size n to represent
         *  the new base address of the vector.
         *  (alternatively, v->impl.start have also been realloc'ed to size n),
         *  but if n is less than or equal to the old size, the old elements
         *  will be overwritten anyway.
         */
        newstart = calloc(n, v->ttbl->width);
        massert_calloc(newstart);

        /* pointers at impl are re-established */
        v->impl.start = newstart;
        v->impl.finish = v->impl.start;
        v->impl.end_of_storage = v->impl.start + n;

        sentinel = v->impl.end_of_storage;

        if (v->ttbl->copy) {
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish++, valaddr);
            }
        } else {
            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish++, valaddr, v->ttbl->width);
            }
        }
    }
}

/**
 *  @brief  Returns the capacity of v
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return         capacity of v
 */
size_t vcapacity_str(vector_str *v) {
    massert_container(v);
    return v->impl.end_of_storage - v->impl.start;
}

/**
 *  @brief  Determines if v is an empty vector, or not
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return     true if v->impl.start == v->impl.finish, false otherwise
 */
bool vempty_str(vector_str *v) {
    massert_container(v);
    /**
     *  Since v->impl.finish is always one address ahead of vector's back element,
     *  if v->impl.start == v->impl.finish, the vector is empty.
     */
    return v->impl.start == v->impl.finish;
}

void vreserve_str(vector_str *v, size_t n) {
    massert_container(v);

    /**
     *  Reserve is effectively a resize,
     *  but verifies that the value of n meets the requirements for a reservation.
     */
    if (n > vcapacity_str(v)) {
        vresize_str(v, n);
    } else {
        /* no-op */
        ERROR(__FILE__, "n must be greater than vector's buffer capacity.");
        return;
    }
}

/**
 *  @brief  Shrinks vector_str's buffer to that of its logical length
 *
 *  @param[in] v    pointer to vector_str
 *
 *  This function is effectively a conditional resize --
 *  but verifies that:
 *      - vector is not empty
 *      - vector's finish pointer is not equal to end_of_storage pointer
 */
void vshrinktofit_str(vector_str *v) {
    massert_container(v);

    if ((v->impl.start != v->impl.finish) && (v->impl.finish != v->impl.end_of_storage)) {
        vresize_str(v, vsize_str(v));
    }
}

/**
 *  @brief  Retrieves the address of an element from vector at index n
 *
 *  @param[in]  v   pointer to vector_str
 *  @param[in]  n   index of desired element
 *
 *  @return     address of element at n
 *
 *  A bounds check is performed to ensure that n is a valid index.
 */
str *vat_str(vector_str *v, size_t n) {
    size_t size = 0;
    str *target = NULL;

    massert_container(v);

    size = vsize_str(v);
    target = NULL;

    if (n >= size) {
        char str[256];
        sprintf(str, "Input %lu is greater than vector's logical length, %lu -- index out of bounds.", n, size);
        ERROR(__FILE__, str);
        return NULL;
    } else if (n == 0) {
        /* if n is 0, it's the front of the vector */
        target = v->impl.start;
    } else if (n == (size - 1)) {
        /* if n is (size - 1), the back index, effectively (v->impl.finish - 1) */
        target = v->impl.finish - 1;
    } else {
        /* if n is anywhere within (0, size - 1), effectively (v->impl.start + n) */
        target = v->impl.start + n;
    }

    return target ? target : NULL;
}

/**
 *  @brief  Retrieves the address of the front element from vector_str
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return     address of front element
 */
str *vfront_str(vector_str *v) {
    massert_container(v);
    return v->impl.start;
}

/**
 *  @brief  Retrieves the address of the rear element from vector_str
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return     address of rear element
 */
str *vback_str(vector_str *v) {
    massert_container(v);
    return v->impl.finish - 1;
}

/**
 *  @brief  Retrieves the address of vector_str's buffer
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return     address of v->impl.start
 */
str **vdata_str(vector_str *v) {
    massert_container(v);
    return &(v->impl.start);
}

/**
 *  @brief  Retrieves the address of an element from vector at index n
 *
 *  @param[in]  v   pointer to vector_str
 *  @param[in]  n   index of desired element
 *
 *  @return     address of element at n
 *
 *  A bounds check is performed to ensure that n is a valid index.
 */
const str *vatconst_str(vector_str *v, size_t n) {
    str *target = NULL;

    massert_container(v);

    if (n >= vsize_str(v)) {
        char str[256];
        sprintf(str, "Input %lu is greater than vector's logical length, %lu -- index out of bounds.", n, vsize_str(v));
        ERROR(__FILE__, str);
        return NULL;
    }

    target = v->impl.start + n;
    return target ? target : NULL;
}

/**
 *  @brief  Retrieves the address of the front element from vector_str
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return     address of front element, const qualified
 */
const str *vfrontconst_str(vector_str *v) {
    const str *result = (const str *)(v->impl.start);
    return result;
}

/**
 *  @brief  Retrieves the address of the rear element from vector_str
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return     address of rear element, const qualified
 */
const str *vbackconst_str(vector_str *v) {
    const str *result = (const str *)(v->impl.finish - 1);
    return result;
}

/**
 *  @brief  Retrieves the address of vector_str's buffer
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return     address of v->impl.start, const qualified
 */
const str **vdataconst_str(vector_str *v) {
    const str **result = (const str **)(&v->impl.start);
    return result;
}

/**
 *  @brief  Assigns a range of elements to vector_str, starting at its beginning
 *
 *  @param[in]  v       pointer to vector_str
 *  @param[in]  first   represents the beginning of the range (inc)
 *  @param[in]  last    represents the end of the range, (exc)
 *
 *  Elements in this vector will be destroyed, and replaced with
 *  contents from [first, last).
 *  
 *  If the range of [first, last) exceeds that of vcapacity_str(v),
 *  the capacity will be increased to that of size it_distance(&first, &last).
 */
void vassignrnge_str(vector_str *v, iterator first, iterator last) {
    int delta = it_distance(&first, &last);
    struct typetable *ttbl_first = it_get_ttbl(first);

    str *sentinel = it_curr(last);
    str *curr = NULL;

    if (first.itbl != last.itbl) {
        /**
         *  Iterators first and last must refer to the same container,
         *  or else the cursor pointer will never meet the sentinel pointer
         *  that will end the copy loop.
         * 
         *  This also means that iterators first and last cannot refer to
         *  different container types (this is determiend by what itbls
         *  they each point to)
         */
        ERROR(__FILE__, "first and last must matching container types and refer to the same container.");
        return;
    }

    /**
     *  Clear the vector. 
     */
    vclear_str(v);

    /**
     *  Resize vector if necessary.
     */
    if (delta >= vcapacity_str(v)) {
        vresize_str(v, delta);
    }

    if (ttbl_first->copy) {
        while ((curr = it_curr(first)) != sentinel)  {
            ttbl_first->copy(v->impl.finish++, curr);
            it_incr(&first);
        }
    } else {
        while ((curr = it_curr(first)) != sentinel)  {
            memcpy(v->impl.finish++, curr, v->ttbl->width);
            it_incr(&first);
        };
    } 
}

/**
 *  @brief  Assigns n copies of an element, starting at beginning of vector
 *
 *  @param[in]  v           pointer to vector_str
 *  @param[in]  n           amount of elements to assign
 *  @param[in]  val         the element to assign
 * 
 *  Elements in this vector will be destroyed,
 *  and replaced with n copies of val.
 * 
 *  If n exceeds that of vcapacity_str(v),
 *  the capacity of this vector will be increased to that of size n.
 */
void vassignfill_str(vector_str *v, size_t n, str val) {
    str *sentinel = NULL;

    /**
     *  Clear the vector. 
     */
    vclear_str(v);

    /**
     *  Resize vector if necessary.
     */
    if (n > vcapacity_str(v)) {
        vresize_str(v, n);
    }

    sentinel = v->impl.start + n;
    v->impl.finish = v->impl.start;

    if (v->ttbl->copy) {
        while (v->impl.finish != sentinel)  {
            v->ttbl->copy(v->impl.finish++, &val);
        }
    } else {
        while (v->impl.finish != sentinel)  {
            memcpy(v->impl.finish++, &val, v->ttbl->width);
        };
    } 
}

/**
 *  @brief  Assigns n copies of an element, starting at beginning of vector
 *
 *  @param[in]  v           pointer to vector
 *  @param[in]  n           amount of elements to assign
 *  @param[in]  valaddr     the address of the element to assign
 *
 *  Elements in this vector will be destroyed,
 *  and replaced with n copies of valaddr.
 * 
 *  If n exceeds that of vcapacity_str(v),
 *  the capacity of this vector will be increased to that of size n.
 */
void vassignfillptr_str(vector_str *v, size_t n, str *valaddr) {
    str *sentinel = NULL;

    /**
     *  Clear the vector. 
     */
    vclear_str(v);

    /**
     *  Resize vector if necessary.
     */
    if (n > vcapacity_str(v)) {
        vresize_str(v, n);
    }

    sentinel = v->impl.start + n;
    v->impl.finish = v->impl.start;

    if (v->ttbl->copy) {
        while (v->impl.finish != sentinel)  {
            v->ttbl->copy(v->impl.finish++, valaddr);
        }
    } else {
        while (v->impl.finish != sentinel)  {
            memcpy(v->impl.finish++, valaddr, v->ttbl->width);
        };
    } 
}

/**
 *  @brief  Appends an element to the rear of the vector
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  val     element to be copied
 *
 *  If a copy function is defined in v's ttbl,
 *  valaddr will be deep-copied into v.
 *  Otherwise, valaddr will be shallow-copied into v
 *  using memcpy.
 */
void vpushb_str(vector_str *v, str val) {
   massert_container(v);

    /**
     *  A doubling strategy is employed when the finish pointer
     *  meets the end_of_storage pointer.
     */
    if (v->impl.finish == v->impl.end_of_storage) {
        vresize_str(v, vcapacity_str(v) * 2);
    }

    if (v->ttbl->copy) {
        /* if copy fn defined in ttbl, deep copy */
        v->ttbl->copy(v->impl.finish++, &val);
    } else {
        /* if no copy defined in ttbl, shallow copy */
        memcpy(v->impl.finish++, &val, v->ttbl->width);
    }

    /* finish pointer advanced to the next empty block */
}

/**
 *  @brief  Appends an element to the rear of the vector
 *
 *  @param[in]  v       pointer to vector_str
 *  @param[in]  valaddr address of element to be copied
 *
 *  If a copy function is defined in v's ttbl,
 *  valaddr will be deep-copied into v.
 *  Otherwise, valaddr will be shallow-copied into v
 *  using memcpy.
 */
void vpushbptr_str(vector_str *v, str *valaddr) {
   massert_container(v);
   massert_ptr(valaddr);

    /**
     *  A doubling strategy is employed when the finish pointer
     *  meets the end_of_storage pointer.
     */
    if (v->impl.finish == v->impl.end_of_storage) {
        vresize_str(v, vcapacity_str(v) * 2);
    }

    if (v->ttbl->copy) {
        /* if copy fn defined in ttbl, deep copy */
        /*v->ttbl->copy(v->impl.finish++, valaddr);*/
        memcpy(v->impl.finish++, valaddr, v->ttbl->width);
    } else {
        /* if no copy defined in ttbl, shallow copy */
        memcpy(v->impl.finish++, valaddr, v->ttbl->width);
    }

    /* finish pointer advanced to the next empty block */
}

/**
 *  @brief  Removes element at the rear of the vector
 *
 *  @param[in]  v   pointer to vector_str
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
void vpopb_str(vector_str *v) {
    massert_container(v);

    if (v->impl.finish == v->impl.start) {
        /* vpopb is a no-op if vector is empty */
        return;
    }

    /* decrement the finish pointer to the address of the "victim" block */
    --v->impl.finish;

    if (v->ttbl->dtor) {
        /**
         *  If dtor defined in ttbl,
         *  release memory at finish pointer as defined by dtor.
         */
        v->ttbl->dtor(v->impl.finish);
    }

    /**
     *  If no dtor defined, the next element appendage
     *  will simply be overwritten
     */
}

/**
 *  @brief  Inserts a value into vector at position specified by pos
 *
 *  @param[in]  v       pointer to vector_str
 *  @param[in]  pos     refers to an element within v
 *  @param[in]  valaddr element to insert
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
iterator vinsert_str(vector_str *v, iterator pos, str val) {
    str *curr = NULL;
    size_t ipos = 0;

    massert_container(v);

    /**
     *  A doubling strategy is employed when the finish pointer
     *  meets the end_of_storage pointer.
     */
    if (v->impl.finish == v->impl.end_of_storage) {
        vresize_str(v, vcapacity_str(v) * 2);
    }

    /**
     *  To insert within indices (0, vsize_str(v)),
     *  begin by appending val to the rear of the vector.
     */
    if (v->ttbl->copy) {
        /* if copy fn defined in ttbl, deep copy */
        v->ttbl->copy(v->impl.finish, &val);
    } else {
        /* if no copy defined in ttbl, shallow copy */
        memcpy(v->impl.finish, &val, v->ttbl->width);
    }

    /**
     *  All of the above was code for pushb,
     *  but incrementing the finish pointer was omitted.
     */

    /* save pos's index - a new iterator will be returned later */
    ipos = it_distance(NULL, &pos);

    /**
     *  Use iterator pos to swap elements from [pos, vsize_str(v))
     *  val will resize at the index originally specified by pos
     */
    while ((curr = it_curr(pos)) != v->impl.finish) {
        vswapaddr_str(v, curr, v->impl.finish);
        it_incr(&pos);
    }

    /* restore finish pointer to address of rear element */
    ++v->impl.finish;

    /* returned is an iterator at refers to val's position in v */
    return it_next_n(vbegin_str(v), ipos);
}

/**
 *  @brief  Inserts a value into vector at position specified by pos
 *
 *  @param[in]  v       pointer to vector_str
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
iterator vinsertptr_str(vector_str *v, iterator pos, str *valaddr) {
    str *curr = NULL;
    size_t ipos = 0;

    massert_container(v);
    massert_ptr(valaddr);

    /**
     *  A doubling strategy is employed when the finish pointer
     *  meets the end_of_storage pointer.
     */
    if (v->impl.finish == v->impl.end_of_storage) {
        vresize_str(v, vcapacity_str(v) * 2);
    }

    /**
     *  To insert within indices (0, vsize_str(v)),
     *  begin by appending valaddr to the rear of the vector.
     */
    if (v->ttbl->copy) {
        /* if copy fn defined in ttbl, deep copy */
        v->ttbl->copy(v->impl.finish, valaddr);
    } else {
        /* if no copy defined in ttbl, shallow copy */
        memcpy(v->impl.finish, valaddr, v->ttbl->width);
    }

    /**
     *  All of the above was code for pushb,
     *  but incrementing the finish pointer was omitted.
     */

    /* save pos's index - a new iterator will be returned later */
    ipos = it_distance(NULL, &pos);

    /**
     *  Use iterator pos to swap elements from [pos, vsize_str(v))
     *  val will resize at the index originally specified by pos
     */
    while ((curr = it_curr(pos)) != v->impl.finish) {
        vswapaddr_str(v, curr, v->impl.finish);
        it_incr(&pos);
    }

    /* restore finish pointer to address of rear element */
    ++v->impl.finish;

    /* returned is an iterator at refers to val's position in v */
    return it_next_n(vbegin_str(v), ipos);
}

/**
 *  @brief  Inserts n copies of val into vector at position specified by pos
 *
 *  @param[in]  v       pointer to vector_str
 *  @param[in]  pos     refers to an element within v
 *  @param[in]  n       quantity of desired copies of valaddr to insert
 *  @param[in]  val     the desired element to fill vector with
 *
 *  @return     iterator referring to the first copy of val from within vector
 *
 *  Undefined behavior of pos does not refer to an element positioned within v.
 *
 *  If ttbl has a copy function defined,
 *  n copies of val will be inserted into the returned vector
 *  using the copy function - this copy function is intended for
 *  deep copies. Otherwise, val will be shallow-copied using memcpy.
 */
iterator vinsertfill_str(vector_str *v, iterator pos, size_t n, str val) {
    size_t ipos = 0;
    size_t old_size = 0;
    size_t old_capacity = 0;

    str *sentinel = NULL;
    str *right_adj = NULL;
    str *finish = NULL;

    massert_container(v);

    ipos = it_distance(NULL, &pos);      /**< pos's index position */
    old_size = vsize_str(v);                /**< v's former size */
    old_capacity = vcapacity_str(v);        /**< v's former capacity */

    if ((old_size + n) >= old_capacity) {
        /**
         *  If inserting n elements will equal or exceed that of old_capacity,
         *  vector will be resized to accomodate ((old_capacity + n) * 2) elements.
         */
        vresize_str(v, (old_capacity + n) * 2);
        pos = it_next_n(vbegin_str(v), ipos);
    }

    if (n <= 0) {
        /* no-op */
        return pos;
    } else if (n == 1) {
        /* if n is 1, redirect to vinsert and return early */
        vinsert_str(v, pos, val);
        return it_next_n(vbegin_str(v), ipos);
    }

    if (ipos == old_size - 1 || ipos == old_size) {
        /* inlined instructions resembling pushb */
        sentinel = v->impl.finish + n;

        if (v->ttbl->copy) {
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish++, &val);
            }
        } else {
            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish++, &val, v->ttbl->width);
            }
        }

        /* since n == back index, return early */
        return it_next_n(vbegin_str(v), n);
    } else {
        /* decrement finish pointer so it refers to the rear element */
        --v->impl.finish;

        /**
         *  Each element in the range [n, vsize_str(v)) will be moved n blocks over
         *  -- starting with the rear element.
         */

        /**
         *  sentinel will be set one position behind pos.curr
         *  to account for the new element(s) being inserted
         */
        sentinel = (str *)(pos.curr) - 1;

        /* new destination address for finish pointer */
        right_adj = v->impl.finish + n;

        /* need to save this address for later when restoring impl.finish */
        finish = v->impl.finish + (n + 1);

        /**
         *  Elements [n, vsize_str(v)) are moved over n blocks to the right,
         *  starting with v->impl.finish.
         *  Loop stops when finish pointer reach sentinel.
         */
        while (v->impl.finish != sentinel) {
            vswapaddr_str(v, v->impl.finish--, right_adj--);
        }

        /**
         *  sentinel is changed to point to the (ipos + n)th block
         *  (one element past the last fill insertion element)
         */
        sentinel = v->impl.start + (ipos + n);

        /**
         *  v->impl.finish must be advanced one block forward
         *  (finish was decremented until it was at the address
         *  of the former sentinel value, which was one position behind
         *  pos.curr)
         */
        ++v->impl.finish; 

        if (v->ttbl->copy) {
            /* deep copy */
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish++, &val);
            }
        } else {
            /* shallow copy */
            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish++, &val, v->ttbl->width);
            }
        }

        /* now restoring v->impl.finish to where it should be */
        v->impl.finish = finish;
    }

    return it_next_n(vbegin_str(v), ipos);
}

/**
 *  @brief  Inserts n copies of valaddr into vector at position specified by pos
 *
 *  @param[in]  v       pointer to vector_str
 *  @param[in]  pos     refers to an element within v
 *  @param[in]  n       quantity of desired copies of valaddr to insert
 *  @param[in]  valaddr address of the desired element to fill vector with
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
iterator vinsertfillptr_str(vector_str *v, iterator pos, size_t n, str *valaddr) {
    size_t ipos = 0;
    size_t old_size = 0;
    size_t old_capacity = 0;

    str *sentinel = NULL;
    str *right_adj = NULL;
    str *finish = NULL;

    massert_container(v);

    ipos = it_distance(NULL, &pos);      /**< pos's index position */
    old_size = vsize_str(v);                /**< v's former size */
    old_capacity = vcapacity_str(v);        /**< v's former capacity */

    if ((old_size + n) >= old_capacity) {
        /**
         *  If inserting n elements will equal or exceed that of old_capacity,
         *  vector will be resized to accomodate ((old_capacity + n) * 2) elements.
         */
        vresize_str(v, (old_capacity + n) * 2);
        pos = it_next_n(vbegin_str(v), ipos);
    }

    if (n <= 0) {
        /* no-op */
        return pos;
    } else if (n == 1) {
        /* if n is 1, redirect to vinsert and return early */
        vinsertptr_str(v, pos, valaddr);
        return it_next_n(vbegin_str(v), ipos);
    }

    if (ipos == old_size - 1 || ipos == old_size) {
        /* inlined instructions resembling pushb */
        sentinel = v->impl.finish + n;

        if (v->ttbl->copy) {
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish++, valaddr);
            }
        } else {
            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish++, valaddr, v->ttbl->width);
            }
        }

        /* since n == back index, return early */
        return it_next_n(vbegin_str(v), n);
    } else {
        /* decrement finish pointer so it refers to the rear element */
        --v->impl.finish;

        /**
         *  Each element in the range [n, vsize_str(v)) will be moved n blocks over
         *  -- starting with the rear element.
         */

        /**
         *  sentinel will be set one position behind pos.curr
         *  to account for the new element(s) being inserted
         */
        sentinel = (str *)(pos.curr) - 1;

        /* new destination address for finish pointer */
        right_adj = v->impl.finish + n;

        /* need to save this address for later when restoring impl.finish */
        finish = v->impl.finish + (n + 1);

        /**
         *  Elements [n, vsize_str(v)) are moved over n blocks to the right,
         *  starting with v->impl.finish.
         *  Loop stops when finish pointer reach sentinel.
         */
        while (v->impl.finish != sentinel) {
            vswapaddr_str(v, v->impl.finish--, right_adj--);
        }

        /**
         *  sentinel is changed to point to the (ipos + n)th block
         *  (one element past the last fill insertion element)
         */
        sentinel = v->impl.start + (ipos + n);

        /**
         *  v->impl.finish must be advanced one block forward
         *  (finish was decremented until it was at the address
         *  of the former sentinel value, which was one position behind
         *  pos.curr)
         */
        ++v->impl.finish; 

        if (v->ttbl->copy) {
            /* deep copy */
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish++, valaddr);
            }
        } else {
            /* shallow copy */
            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish++, valaddr, v->ttbl->width);
            }
        }

        /* now restoring v->impl.finish to where it should be */
        v->impl.finish = finish;
    }

    return it_next_n(vbegin_str(v), ipos);
}

/**
 *  @brief  Inserts elements from [first, last) into v, at position pos
 *
 *  @param[in]  v       pointer to vector_str
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
iterator vinsertrnge_str(vector_str *v, iterator pos,
                        iterator first, iterator last) {
    size_t ipos = 0;
    size_t old_size = 0;
    size_t old_capacity = 0;
    size_t delta = 0;

    str *sentinel = NULL;
    str *right_adj = NULL;
    str *finish = NULL;

    massert_container(v);

    ipos = it_distance(NULL, &pos);        /**< pos's index position */
    old_size = vsize_str(v);                /**< v's former size */
    old_capacity = vcapacity_str(v);        /**< v's former capacity */
    delta = it_distance(&first, &last);    /**< index(last) - index(first) */

    if ((old_size + delta) >= old_capacity) {
        /**
         *  If inserting delta elements will equal or exceed that of
         *  old_capacity, vector will be resized to
         *  accommodate ((old_capacity + delta) * 2) elements.
         */
        vresize_str(v, (old_capacity + delta) * 2);
        pos = it_next_n(vbegin_str(v), ipos);
    }

    if (delta <= 0) {
        /* no-op */
        return pos;
    } else if (delta == 1) {
        /* if delta is 1, redirect to vinsert and return early */
        vinsert_str(v, pos, *(str *)it_curr(first));
        return it_next_n(vbegin_str(v), ipos);
    }

    if (ipos >= old_size - 1) {
        /* inlined instructions resembling pushb */
        sentinel = v->impl.finish + delta;

        if (v->ttbl->copy) {
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish++, it_curr(first));
                it_incr(&first);
            }
        } else {
            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish++, it_curr(first), v->ttbl->width);
                it_incr(&first);
            }
        }

        /* since n == back index, return early */
        return it_next_n(vbegin_str(v), delta);
    } else {
        /* decrement finish pointer so it refers to the rear element */
        --v->impl.finish;

        /**
         *  Each element in the range [n, vsize_str(v))
         *  will be moved delta blocks over
         *  -- starting with the rear element.
         * 
         *  sentinel will be set one position behind pos.curr
         *  to account for the new element(s) being inserted
         */
        sentinel = (str *)(pos.curr) - 1;

        /* new destination address for finish pointer */
        right_adj = v->impl.finish + delta;

        /* need to save this address for later when restoring impl.finish */
        finish = v->impl.finish + (delta + 1);

        /**
         *  Elements [n, vsize_str(v)) are moved over delta blocks to the right,
         *  starting with v->impl.finish.
         *  Loop stops when finish pointer reaches sentinel
         */
        while (v->impl.finish != sentinel) {
            vswapaddr_str(v, v->impl.finish--, right_adj--);
        }

        /**
         *  sentinel is changed to point to the (ipos + delta)th block
         *  (one element past the last fill insertion element)
         */
        sentinel = (v->impl.start) + (ipos + delta);

        /**
         *  v->impl.finish must be advanced one block forward
         *  (finish was decremented until it was at the address of the former
         *  sentinel value, which one position behind pos.curr)
         */
        ++v->impl.finish;

        if (v->ttbl->copy) {
            /* deep copy */
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish++, it_curr(first));
                it_incr(&first);
            }
        } else {
            /* shallow copy */
            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish++, it_curr(first), v->ttbl->width);
                it_incr(&first);
            }
        }

        /* now restoring v->impl.finish to where it should be */
        v->impl.finish = finish;
    }

    return it_next_n(vbegin_str(v), ipos);
}

/**
 *  @brief  Moves valaddr into v, at position pos
 *
 *  @param[in]  v       pointer to vector_str
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
iterator vinsertmove_str(vector_str *v, iterator pos, str *valaddr) {
    str *dst = NULL;

    massert_container(v);
    massert_ptr(valaddr);

    if (v->ttbl->swap) {
        /**
         *  If a swap function is defined,
         *  the address of dst will be sent to swap,
         *  along with valaddr (which is already the address of some var
         *  sent in by the client)
         * 
         *  The contents of valaddr will be used to initialize dst,
         *  and the standard insert function will be called after.
         * 
         *  If no swap function is defined,
         *  a regular insert occurs.
         *  
         *  This function can prove useful if a client has a dynamically
         *  allocated type, like a pointer (or a type with dynamically allocated fields)
         *  and wants vector to have full ownership of the memory that param inserted
         *  referred to.
         * 
         *  This can help prevent unwanted deep copying of elements, or shallow copies
         *  of elements where there are two pointers referring to the same memory.
         */
        v->ttbl->swap(&dst, valaddr);
    }

    return vinsertptr_str(v, pos, dst);
}

/**
 *  @brief  Removes an element from v at position pos
 *
 *  @param[in]  v   pointer to vector_str
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
iterator verase_str(vector_str *v, iterator pos) {
    int ipos = 0;
    size_t back_index = 0;

    str *curr = NULL;
    str *sentinel = NULL;

    massert_container(v);

    ipos = it_distance(NULL, &pos);
    back_index = vsize_str(v) - 1;

    if ((ipos < 0) || (ipos >= back_index + 1)) {
        /**
         *  If ipos is negative or ipos greater than or equal to v_size(v),
         *  no-op
         */
        return pos;
    } else if (ipos == back_index || pos.curr == (v->impl.finish - 1)) {
        /**
         *  If ipos == back_index
         *  or pos refers to rear vector element,
         *  redirect to popb and return early
         */
        vpopb_str(v);

        /**
         *  pos will no longer refer to an existing element,
         *  so an iterator is returned referring to an element
         *  at the erased element's former index
         */
        return it_next_n(vbegin_str(v), ipos);
    } else if (ipos < back_index && ipos >= 0) {
        if (v->ttbl->dtor) {
            /* If elements were deep copied, release their memory */
            v->ttbl->dtor(pos.curr);
        }

        curr = NULL;
        sentinel = v->impl.finish;

        /**
         *  v->impl.finish and it_finish(pos) should refer to the same thing.
         *  Undefined behavior if pos does not refer to container v!
         */

        while ((curr = it_curr(pos)) != sentinel) {
            /**
             *  Swapping it_curr(pos) and it_curr(it_next(pos))
             *  will shift it_curr(it_next(pos)) one element
             *  to the left. this process continues until
             *  it_curr(pos) equals v->impl.finish.
             */
            vswapaddr_str(v, curr, it_curr(it_next(pos)));
            it_incr(&pos);
        }

        /* decrementing the finish pointer 1 block to the left */
        --v->impl.finish;
    }

    /**
     *  pos will no longer refer to an existing element,
     *  so an iterator is returned referring to an element
     *  at the erased element's former index
     */
    return it_next_n(vbegin_str(v), ipos);
}

/**
 *  @brief  Removes elements from v ranging from [pos, last)
 *
 *  @param[in]  v       pointer to vector_str
 *  @param[in]  pos     refers to element from within vector
 *  @param[in]  last    refers to element from within vector
 *
 *  @return     iterator representing an element that has replaced the
 *              erased element at pos
 *
 *  If a dtor function is defined in v's ttbl,
 *  the element at position pos will be destroyed using the dtor function
 *  from within ttbl.
 *
 *  Memory management of dynamically allocated elements/elements with
 *  dynamically allocated fields become the client's responsibility
 *  if a dtor function is NOT defined within v's ttbl.
 */
iterator verasernge_str(vector_str *v, iterator pos, iterator last) {
    int ipos = 0;
    int delta = 0;

    size_t back_index = 0;

    str *curr = NULL;
    str *sentinel = NULL;

    massert_container(v);

    ipos = it_distance(NULL, &pos);     /**< index of pos */
    delta = it_distance(&pos, &last);   /**< diff between pos/last */

    back_index = vsize_str(v) - 1;

    if ((ipos < 0) || (ipos >= back_index + 1)) {
        /**
         *  If ipos is negative or ipos is greater than or equal to vsize_str(v),
         *  no-op
         */
        return pos;
    } else if (ipos == back_index || pos.curr == (v->impl.finish - 1)) {
        /**
         *  If ipos == back_index
         *  or pos refers to rear vector element,
         *  redirect to popb and return early
         */
        vpopb_str(v);

        /**
         *  pos will no longer refer to an existing element,
         *  so an iterator is returned referring to an element
         *  at the erased element's former index
         */
        return it_next_n(vbegin_str(v), ipos);
    } else if (ipos < back_index && ipos >= 0) {
        curr = NULL;
        sentinel = it_curr(last);

        if (v->ttbl->dtor) {
            /* if range of elements were deep copied, release their memory */
            while ((curr = it_curr(pos)) != sentinel) {
                v->ttbl->dtor(curr);
                it_incr(&pos);
            }
        }

        /* restoring pos to its original index */
        pos = it_next_n(vbegin_str(v), ipos);

        /* reassigning sentinel to one block past the last elem */
        sentinel = v->impl.finish;

        /**
         *  iterator last will advance until it reaches sentinel
         *  (finish pointer) as iterator pos advances alongside it -- they will swap elements between each other
         *  until last reaches the end of the vector.
         */
        while ((curr = it_curr(last)) != sentinel) {
            vswapaddr_str(v, curr, it_curr(pos));

            it_incr(&pos);
            it_incr(&last);
        }

        /**
         *  Using the delta (difference between pos and last),
         *  the finish pointer is decremented to the first null element (one past the last non-null element)
         */
        v->impl.finish = v->impl.finish - delta;
    }

    /**
     *  pos will no longer refer to an existing element,
     *  so an iterator is returned referring to an element
     *  at the beginning of the erased element's former range
     */
    return it_next_n(vbegin_str(v), ipos);
}

/**
 *  @brief  Swaps fields between v and other
 *
 *  @param[out] v       address of pointer to vector
 *  @param[out] other   address of pointer to vector
 */
void vswap_str(vector_str **v, vector_str * *other) {
    vector_str *temp = NULL;

    massert_container((*v));
    massert_container((*other));

    temp = (*v);

    /* change of ownership between v and other */
    (*v)->impl.start = (*other)->impl.start;
    (*v)->impl.finish = (*other)->impl.finish;
    (*v)->impl.end_of_storage = (*other)->impl.end_of_storage;

    /* vectors holding two different types can be swapped */
    (*v)->ttbl = (*other)->ttbl;

    (*other)->impl.start = temp->impl.start;
    (*other)->impl.finish = temp->impl.finish;
    (*other)->impl.end_of_storage = temp->impl.end_of_storage;

    (*other)->ttbl = temp->ttbl;
}

/**
 *  @brief  Destroys elements from within v
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  Memory management of dynamically allocated elements
 *  and/or elements with dynamically allocated fields
 *  become the client's responsibility if a dtor function
 *  is NOT defined within v's ttbl.
 */
void vclear_str(vector_str *v) {
    massert_container(v);

    if (v->impl.finish == v->impl.start) {
        /**
         *  if vector is empty
         *  (start and finish pointers share same address),
         *  it's a no-op.
         */
        return;
    }

    if (v->ttbl->dtor) {
        /* decrementing finish pointer to match the address of the last element */

        /**
         *  If elements were deep-copied,
         *  their memory must be released as per the
         *  client-supplied dtor function in ttbl.
         */
        --v->impl.finish;

        while (v->impl.finish != v->impl.start) {
            v->ttbl->dtor(v->impl.finish--);
        }

        v->ttbl->dtor(v->impl.finish);
        bzero(v->impl.start, vsize_str(v));
        /* v->impl.finish is already at v->impl.start. */
    } else {
        /**
         *  If elements were shallow copied,
         *  (no dtor function specified in ttbl) --
         *  we just memset and reset the finish pointer.
         */
        bzero(v->impl.start, vsize_str(v));
        v->impl.finish = v->impl.start;
    }
}

/**
 *  @brief  Inserts a value into vector at position specified by index
 *
 *  @param[in]  v       pointer to vector_str
 *  @param[in]  index   numerical index of element from within v
 *  @param[in]  val     element to insert
 *
 *  If a copy function is defined in v's ttbl,
 *  val will be deep-copied into v.
 *  Otherwise, val will be shallow-copied into v
 *  using memcpy.
 */
void vinsertat_str(vector_str *v, size_t index, str val) {
    size_t size = 0;

    str *curr = NULL;

    massert_container(v);

    size = vsize_str(v);

    if (index >= size) {
        char str[256];
        sprintf(str, "index provided [%lu] is out of bounds. size of vector is %lu.", index, size);
        ERROR(__FILE__, str);
        return;
    }

    /**
     *  A doubling strategy is employed when the finish pointer
     *  meets the end_of_storage pointer.
     */
    if (v->impl.finish == v->impl.end_of_storage) {
        vresize_str(v, vcapacity_str(v) * 2);
    }

    /**
     *  To insert within indices (0, vsize_str(v)),
     *  begin by appending val to the rear of the vector.
     */

    if (v->ttbl->copy) {
        /* if copy fn defined in ttbl, deep copy */
        v->ttbl->copy(v->impl.finish, &val);
    } else {
        /* if no copy defined in ttbl, shallow copy */
        memcpy(v->impl.finish, &val, v->ttbl->width);
    }

    /**
     *  All of the above was code for pushb,
     *  but incrementing the finish pointer was omitted.
     * 
     *  Use index to swap elements from [index, vsize_str(v))
     *  val will reside at the index originally specified by
     *  index.
     */
    curr = v->impl.start + index;
    while (curr != v->impl.finish) {
        vswapaddr_str(v, curr++, v->impl.finish);
    }

    /* restore finish pointer to address of rear element */
    ++v->impl.finish;
}

/**
 *  @brief  Inserts a value into vector at position specified by index
 *
 *  @param[in]  v       pointer to vector_str
 *  @param[in]  index   numerical index of element from within v
 *  @param[in]  valaddr address of element to insert
 *
 *  If a copy function is defined in v's ttbl,
 *  valaddr will be deep-copied into v.
 *  Otherwise, valaddr will be shallow-copied into v
 *  using memcpy.
 */
void vinsertatptr_str(vector_str *v, size_t index, str *valaddr) {
    size_t size = 0;

    str *curr = NULL;

    massert_container(v);
    massert_ptr(valaddr);

    size = vsize_str(v);

    if (index >= size) {
        char str[256];
        sprintf(str, "index provided [%lu] is out of bounds. size of vector is %lu.", index, size);
        ERROR(__FILE__, str);
        return;
    }

    /**
     *  A doubling strategy is employed when the finish pointer
     *  meets the end_of_storage pointer.
     */
    if (v->impl.finish == v->impl.end_of_storage) {
        vresize_str(v, vcapacity_str(v) * 2);
    }

    /**
     *  To insert within indices (0, vsize_str(v)),
     *  begin by appending valaddr to the rear of the vector.
     */

    if (v->ttbl->copy) {
        /* if copy fn defined in ttbl, deep copy */
        v->ttbl->copy(v->impl.finish, valaddr);
    } else {
        /* if no copy defined in ttbl, shallow copy */
        memcpy(v->impl.finish, valaddr, v->ttbl->width);
    }

    /**
     *  All of the above was code for pushb,
     *  but incrementing the finish pointer was omitted.
     * 
     *  Use index to swap elements from [index, vsize_str(v))
     *  val will reside at the index originally specified by
     *  index.
     */
    curr = v->impl.start + index;
    while (curr != v->impl.finish) {
        vswapaddr_str(v, curr++, v->impl.finish);
    }

    /* restore finish pointer to address of rear element */
    ++v->impl.finish;
}

/**
 *  @brief  Removes an element from v at position pos
 *
 *  @param[in]  v       pointer to vector_str
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
void veraseat_str(vector_str *v, size_t index) {
    size_t size = 0;
    size_t back_index = 0;

    str *curr = NULL;
    str *next = NULL;
    str *sentinel = NULL;

    massert_container(v);

    size = vsize_str(v);

    if (index >= size) {
        char str[256];
        sprintf(str, "index provided [%lu] is out of bounds. size of vector is %lu.", index, size);
        ERROR(__FILE__, str);
        return;
    }

    back_index = size - 1;

    curr = v->impl.start + index;   /* element at index */
    next = index <= vcapacity_str(v) ? (curr + 1) : NULL;    /* elem adj to curr */
    sentinel = v->impl.finish;                                    /* one block after last elem */

    if (index == back_index || curr == (v->impl.finish - 1)) {
        /**
         *  If index == back_index
         *  or pos refers to rear vector element,
         *  redirect to popb and return early
         */
        vpopb_str(v);
        return;
    } else if (index < back_index && index >= 0) {
        if (v->ttbl->dtor) {
            /* if elements were deep copied, release their memory */
            v->ttbl->dtor(curr);
        }

        while (curr != sentinel) {
            /**
             *  Swapping curr and next will shift next
             *  one element to the left. This process continues
             *  until next equals v->impl.finish.
             */
            vswapaddr_str(v, curr++, next++);
        }

        /* decrementing the finish pointer 1 block to the left */
        --v->impl.finish;
    }
}

/**
 *  @brief  Replaces element at index with valaddr
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  index   numerical index of element from within v
 *  @param[in]  val     element to replace at index
 *
 *  Element at index is destroyed, and immediately replaced with val.
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
 *  val will be deep-copied into v.
 *  Otherwise, val will be shallow-copied into v
 *  using memcpy.
 */
void vreplaceat_str(vector_str *v, size_t index, str val) {
    size_t size = 0;

    str *curr = NULL;

    massert_container(v);

    size = vsize_str(v);

    if (index >= size) {
        char str[256];
        sprintf(str, "index provided [%lu] is out of bounds. size of vector is %lu.", index, size);
        ERROR(__FILE__, str);
        return;
    }

    curr = v->impl.start + index;

    if (v->ttbl->dtor) {
        /* deep-copied elements are destroyed (if dtor defined) */
        v->ttbl->dtor(curr);
    }

    if (v->ttbl->copy) {
        /* if copy defined, replacement element is deep-copied */
        v->ttbl->copy(curr, &val);
    } else {
        /* if no copy defined, replacement element is shallow-copied */
        memcpy(curr, &val, v->ttbl->width);
    }
}

/**
 *  @brief  Replaces element at index with valaddr
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  index   numerical index of element from within v
 *  @param[in]  valaddr address of element to replace at index
 *
 *  Element at index is destroyed, and immediately replaced with val.
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
void vreplaceatptr_str(vector_str *v, size_t index, str *valaddr) {
    size_t size = 0;

    str *curr = NULL;

    massert_container(v);

    size = vsize_str(v);

    if (index >= size) {
        char str[256];
        sprintf(str, "index provided [%lu] is out of bounds. size of vector is %lu.", index, size);
        ERROR(__FILE__, str);
        return;
    }

    curr = v->impl.start + index;

    if (v->ttbl->dtor) {
        /* deep-copied elements are destroyed (if dtor defined) */
        v->ttbl->dtor(curr);
    }

    if (v->ttbl->copy) {
        /* if copy defined, replacement element is deep-copied */
        v->ttbl->copy(curr, valaddr);
    } else {
        /* if no copy defined, replacement element is shallow-copied */
        memcpy(curr, valaddr, v->ttbl->width);
    }
}

/**
 *  @brief  Swap two elements from within v
 *
 *  @param[in]  v   pointer to vector_str
 *  @param[in]  n1  index of first element
 *  @param[in]  n2  index of second element
 */
void vswapelem_str(vector_str *v, size_t n1, size_t n2) {
    size_t size = 0;
    size_t capacity = 0;

    bool n1_bad = false;
    bool n2_bad = false;
    bool good_indices = false;

    str *temp = NULL;
    str *data_1 = NULL;
    str *data_2 = NULL;

    massert_container(v);

    size = vsize_str(v);
    capacity = vcapacity_str(v);

    n1_bad = n1 >= size || n1 >= capacity;
    n2_bad = n2 >= size || n2 >= capacity;

    good_indices = !n1_bad && !n2_bad;

    if (good_indices && size > 0) {
        data_1 = v->impl.start + n1;
        data_2 = v->impl.start + n2;

        if (v->ttbl->swap) {
            v->ttbl->swap(data_1, data_2);
        } else {
            temp = malloc(v->ttbl->width);
            massert_ptr(temp);
            memcpy(temp, data_1, v->ttbl->width);

            memcpy(data_1, data_2, v->ttbl->width);
            memcpy(data_2, temp, v->ttbl->width);

            free(temp);
            temp = NULL;
        }
    } else {
        char str[256];
        sprintf(str, "indices n1 [%lu] and/or n2 [%lu] are out of bounds.", n1, n2);
        ERROR(__FILE__, str);
        return;
    }
}

/**
 *  @brief  Removes all occurences of val from within v
 *
 *  @param[in]  v       pointer to vector
 *  @param[in]  val     element to remove (copy of)
 *
 *  This function uses the supplied compare function with v's
 *  ttbl to determine if val matches any of the elements.
 *
 *  If a dtor function is defined in v's ttbl,
 *  the matched element will be destroyed using the dtor function
 *  from within ttbl.
 *
 *  Memory management of dynamically allocated elements/elements with
 *  dynamically allocated fields become the client's responsibility
 *  if a dtor function is NOT defined within v's ttbl.
 */
void vremove_str(vector_str *v, str val) {
    size_t i = 0;
    size_t size = 0;

    str *curr = NULL;

    massert_container(v);

    if (v->impl.start == v->impl.finish) {
        return;
    }

    i = 0;
    size = vsize_str(v);
    curr = v->impl.start;

    if (v->ttbl->compare(curr, &val) == 0) {
        veraseat_str(v, i);
        --size;
        ++curr;
    }

    for (i = 1; i < size; i++) {
        if (v->ttbl->compare(curr, &val) == 0) {
            --curr;
            veraseat_str(v, i--);
            --size;
        }

        ++curr;
    }
}

/**
 *  @brief  Removes all occurences of elements that meet a condition within v
 * 
 *  @param[in]  v                   pointer to vector
 *  @param[in]  unary_predicate     pointer to function
 * 
 *  For all elements e in v, if unary_predicate(e) == true,
 *  it will be removed.
 */
void vremoveif_str(vector_str *v, bool (*unary_predicate)(const void *)) {
    size_t i = 0;
    size_t size = 0;

    str *curr = NULL;

    massert_container(v);
    massert_pfunc(unary_predicate);

    if (v->impl.start == v->impl.finish) {
        return;
    }

    i = 0;
    size = vsize_str(v);
    curr = v->impl.start;

    if (unary_predicate(curr) == true) {
        veraseat_str(v, i);
        --size;
        ++curr;
    }

    for (i = 1; i < size; i++) {
        if (unary_predicate(curr) == true) {
            --curr;
            v_erase_at(v, i--);
            --size;
        }

        ++curr;
    }
}

/**
 *  @brief  Append the contents of other to the rear of v
 *
 *  @param[in]  v       pointer to vector_str
 *  @param[in]  other   pointer to vector_str
 *
 *  @return     pointer to vector_str with other's elements appended
 *
 *  The merging of vector_str v and vector_str other does not mutate other.
 */
vector_str *vmerge_str(vector_str *v, vector_str * other) {
    size_t size_other = 0;
    size_t capacity_v = 0;

    str *sentinel = NULL;

    massert_container(v);
    massert_ptr(other);

    size_other = vsize_str(v);
    capacity_v = vcapacity_str(v);

    if (size_other >= capacity_v) {
        /**
         *  If (other's size + v's current size) 
         *  will exceed that of v's capacity,
         *  resize v
         */
        vresize_str(v, capacity_v * 2);
    }

    sentinel = other->impl.finish;
    other->impl.finish = other->impl.start;

    /* the merging of v and other does not mutate other. */
    if ((v->ttbl->copy && other->ttbl->copy)
        && (v->ttbl->copy == other->ttbl->copy)) {
        /**
         *  If v has a copy function defined in its ttbl,
         *  other has a copy function defined in its ttbl,
         *  and v shares the same copy function with other,
         *  do a deep copy of other's elements into v.
         * 
         *  A deep copy from other to v for types with
         *  dynamically allocated memory means that clearing
         *  v does not affect other whatsoever.
         */
        while (other->impl.finish != sentinel) {
            v->ttbl->copy(v->impl.finish++, other->impl.finish++);
        }
    } else {
        /**
         *  If either v/other lack a copy function,
         *  and/or they do not share the same copy function,
         *  a shallow copy of other's elements into v will occur.
         *  
         *  A shallow copy from other to v for types with
         *  dynamically allocated memory means that clearing
         *  v will clear the memory associated with the elements
         *  that were shallow copied from other to v.
         */
        while (other->impl.finish != sentinel) {
            memcpy(v->impl.finish++, other->impl.finish++, v->ttbl->width);
        }
    }

    return v;
}

/**
 *  @brief  Reverses the contents of v
 *
 *  @param[in]  v   pointer to vector
 */
void vreverse_str(vector_str *v) {
    str *back = NULL;
    str *restore = NULL;

    massert_container(v);

    back = v->impl.finish - 1;

    /**
     *  saving the address of the finish pointer for later,
     *  as the finish pointer will be set to the beginning of the vector
     */
    restore = v->impl.finish;
    v->impl.finish = v->impl.start;

    while (v->impl.finish != back) {
        /* swap addresses at finish and back */
        vswapaddr_str(v, v->impl.finish++, back--);

        /**
         *  Increment finish, decrement back --
         *  until finish's address matches that of back
         */
    }

    /**
     *  Restoring the finish pointer - size of vector is the same
     *  (the first swap ensures that restore is the desired finish position)
     */
    v->impl.finish = restore;
}

/**
 *  @brief  Returns a new vector_str with the contents of base
 *
 *  @param[in]  ttbl    typetable matching that of bases's element type
 *  @param[in]  base    base address of an array to copy
 *  @param[in]  length  logical length of base
 *
 *  @return     pointer to vector_str with contents of base
 */
vector_str *varrtov_str(str *base, size_t length) {
    vector_str *v = NULL;

    str *target = NULL;

    massert_ptr(base);

    v = vnewr_str(length);

    target = base;

    if (v->ttbl->copy) {
        /* deep copy */
        while (v->impl.finish != v->impl.end_of_storage) {
            v->ttbl->copy(v->impl.finish++, target++);
        }
    } else {
        /* shallow copy */
        while (v->impl.finish != v->impl.end_of_storage) {
            memcpy(v->impl.finish++, target++, v->ttbl->width);
        }
    }

    return v;
}

/**
 *  @brief  Takes an existing pointer, base, and adapts it for use with vector
 * 
 *  @param[in]  base        base address of an array to copy, dynamically allocated
 *  @param[in]  length      logical length of base
 *  @param[in]  capacity    memory capacity of base
 *
 *  @return     pointer to vector with contents of base
 *  
 *  Precondition: base points to a valid address of dynamically allocated memory.
 *  No allocation/copying of any kind is done within this function --
 *  this simply assigns the pointer base to be used by vector --
 *  it acts as a "wrapper" for base. 
 *  
 *  An appropriate typetable must be chosen for this instantiation, as always.
 * 
 *  Since this vector will be using a pre-existing pointer,
 *  be careful and pay special attention to the management of its memory.
 */
vector_str *vptrtov_str(str *base, size_t length, size_t capacity) {
    vector_str *v = NULL;

    massert_ptr(base);

    v = vallocate_str();

    /**
     *  An appropriate typetable will be chosen that matches
     *  that of the type str for base.
     */
    v->ttbl = _str_ ? _str_ : _void_ptr_;

    if (v->ttbl != _void_ptr_) {
        v->ttbl->compare = v->ttbl->compare ? v->ttbl->compare : NULL;
        v->ttbl->copy = v->ttbl->copy ? v->ttbl->copy : NULL;
        v->ttbl->print = v->ttbl->print ? v->ttbl->print : NULL;
        v->ttbl->dtor = v->ttbl->dtor ? v->ttbl->dtor : NULL;
    }

    if (capacity <= 0) {
        WARNING(__FILE__, "Provided input capacity was less than or equal to 0. This may result in undefined behavior.");
    }

    v->impl.start = base;
    v->impl.finish = v->impl.start + length;
    v->impl.end_of_storage = v->impl.start + capacity;

    return v;
}

/**
 *  @brief  Performs a linear search to find val using the ttbl->compare function
 *
 *  @param[in]  v       pointer to vector_str
 *  @param[in]  val     a copy of an element to find
 */
int vsearch_str(vector_str *v, str val) {
    int (*comparator)(const void *, const void *) = NULL;

    str *curr = NULL;
    bool found = false;
    int result = 0;

    massert_container(v);

    comparator = v->ttbl->compare ? v->ttbl->compare : void_ptr_compare;
    curr = v->impl.start;

    /* standard linear search of val using comparator */
    while (curr != v->impl.finish) {
        if (comparator(curr, &val) == 0) {
            found = true;
            break;
        }

        ++curr;
        ++result;
    }

    /* if found, return result, else (-1) to denote not found */
    return found ? result : -1;
}

/**
 *  @brief  Sorts the contents of v using ttbl->compare
 *
 *  @param[in]  v   pointer to vector_str
 */
void vsort_str(vector_str *v) {
    size_t size = 0;
    int (*comparator)(const void *, const void *) = NULL;

    massert_container(v);

    size = vsize_str(v);

    if (size < 2) {
        /* why sort a data structure if size < 2? */
        return;
    }

    comparator = v->ttbl->compare 
    ? v->ttbl->compare : void_ptr_compare;

    /* cstdlib qsort (best performance) */
    /* qsort(v->impl.start, size, v->ttbl->width, comparator); */

    /* cstdlib mergesort */
    /* mergesort(v->impl.start, size, v->ttbl->width, comparator); */

    /* gcslib mergesort */
    v_mergesort_iterative(v->impl.start, size, v->ttbl->width, comparator);
}

/**
 *  @brief  Prints a diagnostic of vector_str to stdout
 *
 *  @param[in]  v   pointer to vector_str
 */
void vputs_str(vector_str *v) {
    /* redirect to vfputs with stream stdout */
    vfputs_str(v, stdout);
}

/**
 *  @brief  Prints the contents of vector_str with user-defined formatting
 *
 *  @param[in]  v           pointer to vector_str
 *  @param[in]  before      string that appears before any elements appear
 *  @param[in]  after       string that appears after all the elements have appeared
 *  @param[in]  postelem    string that appears after each element, except the last one
 *  @param[in]  breaklim    amount of elements that print before a line break occurs.
 *                          0 means no line breaks
 */
void vputsf_str(vector_str *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim) {
    /* redirect to vfputsf with stream stdout */
    vfputsf_str(v, stdout, before, after, postelem, empty, breaklim);
}

/**
 *  @brief  Prints a diagnostic of vector_str to file stream dest
 *
 *  @param[in]  v       pointer to vector_str
 *  @param[in]  dest    file stream (e.g stdout, stderr, a file)
 */
void vfputs_str(vector_str *v, FILE *dest) {
    char buffer1[MAXIMUM_STACK_BUFFER_SIZE];
    char buffer2[MAXIMUM_STACK_BUFFER_SIZE];

    const char *link = "------------------------------";
    const char *bytes_label = NULL;
    const char *postelem = "";
    const char *empty = "--- Container is empty ---";

    const size_t breaklim = 1;

    massert_container(v);
    massert_ptr(dest);

    sprintf(buffer1, "\n%s\n%s\n%s\n", link, "Elements", link);

    bytes_label = v->ttbl->width == 1 ? "byte" : "bytes";

    sprintf(buffer2, "%s\n%s\t\t%lu\n%s\t%lu\n%s\t%lu %s\n%s\n", link, "Size",
            vsize_str(v), "Capacity", vcapacity_str(v), "Element size", v->ttbl->width,
            bytes_label, link);

    vfputsf_str(v, dest, buffer1, buffer2, postelem, empty, breaklim);
}

/**
 *  @brief  Prints the contents of vector_str with user-defined formatting,
 *          to file stream dest
 *
 *  @param[in]  v           pointer to vector_str
 *  @param[in]  dest        file stream (e.g. stdout, stderr, a file)
 *  @param[in]  before      string that appears before any elements appear
 *  @param[in]  after       string that appears after all the elements have appeared
 *  @param[in]  postelem    string that appears after each element, except the last one
 *  @param[in]  breaklim    amount of elements that print before a line break occurs.
 *                          0 means no line breaks
 */
void vfputsf_str(vector_str *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim) {
    void (*print)(const void *, FILE *dest) = NULL;

    size_t size = 0;
    size_t i = 0;
    size_t curr = 0;

    str *target = NULL;

    massert_container(v);
    massert_ptr(dest);

    fprintf(dest, "%s", before ? before : "");

    print = v->ttbl->print ? v->ttbl->print : void_ptr_print;

    size = vsize_str(v);

    if (size == 0) {
        fprintf(dest, "%s\n", empty ? empty : "");
    } else {
        target = v->impl.start;

        for (i = 0, curr = 1; i < size; i++, curr++) {
            print(target, dest);

            /* address - disable for release */
            fprintf(dest, "\t\t(%s%p%s)", KCYN, (void *)(target), KNRM);

            if (i < size - 1) {
                fprintf(dest, "%s", postelem ? postelem : "");
            }

            if (curr == breaklim) {
                curr = 0;
                fprintf(dest, "\n");
            }

            ++target;
        }
    }

    fprintf(dest, "%s", after ? after : "");
}

/**
 *  @brief  Wrapper function for a struct typetable
 *
 *  @param[in]  arg     address of a vector_str pointer
 *  @param[in]  other   address of a vector_str pointer
 *
 *  @return     a pointer to vector_str
 */
void *tmpl_vector_copy_str(void *arg, const void *other) {
    vector_str **dest = NULL;
    vector_str **source = NULL;

    massert_ptr(other);

    dest = (vector_str **)(arg);
    source = (vector_str **)(other);

    (*dest) = vnewcopy_str((*source));

    return (*dest);
}

/**
 *  @brief  Wrapper function for a struct typetable
 *
 *  @param[in]  arg     address of a vector_str pointer
 */
void tmpl_vector_dtor_str(void *arg) {
    vector_str **v = NULL;
    
    massert_ptr(arg);

    v = (vector_str **)(arg);
    vdelete_str(v);
}

/**
 *  @brief  Wrapper function for a struct typetable
 *
 *  @param[in]  s1  address of a vector_str pointer
 *  @param[in]  s2  address of a vector_str pointer
 */
void tmpl_vector_swap_str(void *s1, void *s2) {
    vector_str **v1 = (vector_str **)(s1);
    vector_str **v2 = (vector_str **)(s2);

    if ((*v1)) {
        vswap_str(v1, v2);
    } else {
        (*v1) = (*v2);
        (*v2) = NULL;
    }
}

/**
 *  @brief  Wrapper function for a struct typetable
 *
 *  @param[in]  c1  address of a vector_str pointer
 *  @param[in]  c2  address of a vector_str pointer
 *
 *  @return     -1 if c1 and c2 do not share a comparison function
 *              otherwise, accumulated comparison results between
 *              c1 and c2's elements, within their common length
 *              0 means they are both equivalent, within their common length.
 */
int tmpl_vector_compare_str(const void *c1, const void *c2) {
    vector_str *v1 = NULL;
    vector_str *v2 = NULL;

    vector_str *vec1 = NULL;
    vector_str *vec2 = NULL;

    size_t size1 = 0;
    size_t size2 = 0;
    size_t size = 0;

    str *target1 = NULL;
    str *target2 = NULL;

    int delta = 0;
    int i = 0;

    massert_container(c1);
    massert_container(c2);

    v1 = *(vector_str **)(c1);
    v2 = *(vector_str **)(c2);

    if (v1->ttbl->compare != v2->ttbl->compare) {
        return -1;
    }

    vec1 = vnewcopy_str(v1);
    vec2 = vnewcopy_str(v2);

    vsort_str(vec1);
    vsort_str(vec2);

    size1 = vsize_str(vec1);
    size2 = vsize_str(vec2);

    size = size1 < size2 ? size1 : size2;

    target1 = vec1->impl.start;
    target2 = vec2->impl.start;

    for (i = 0; i < size; i++) {
        delta += vec1->ttbl->compare(&target1, &target2);

        ++target1;
        ++target2;
    }

    vdelete_str(&vec2);
    vdelete_str(&vec1);

    /* if delta == 0, both vectors are equivalent within their common length. */
    return delta;
}

/**
 *  @brief  Wrapper function for a struct typetable
 *
 *  @param[in]  arg     address of a vector_str pointer
 *  @param[in]  dest    file stream (stdout, stderr, a file)
 */
void tmpl_vector_print_str(const void *arg, FILE *dest) {
    vector_str *v = NULL;

    massert_ptr(arg);
    massert_ptr(dest);

    v = *(vector_str **)(arg);
    vfputs_str(v, dest);
}

/**
 *  @brief  Reassign ttbl to v
 *
 *  @param[in]  v       pointer to vector_str
 *  @param[in]  ttbl    pointer to typetable
 */
void vsetttbl_str(vector_str *v, struct typetable *ttbl) {
    massert_container(v);
    v->ttbl = ttbl ? ttbl : _void_ptr_;
}

/**
 *  @brief  Retrieves width (data size) in v's ttbl
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return     size of data type
 */
size_t vgetwidth_str(vector_str *v) {
    massert_container(v);
    return v->ttbl->width;
}

/**
 *  @brief  Retrieves copy function in v's ttbl
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return     copy function used by v
 */
copy_fn vgetcopy_str(vector_str *v) {
    massert_container(v);
    return v->ttbl->copy;
}

/**
 *  @brief  Retrieves dtor function in v's ttbl
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return     dtor function used by v
 */
dtor_fn vgetdtor_str(vector_str *v) {
    massert_container(v);
    return v->ttbl->dtor;
}

/**
 *  @brief  Retrieves swap function in v's ttbl
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return     swap function used by v
 */
swap_fn vgetswap_str(vector_str *v) {
    massert_container(v);
    return v->ttbl->swap;
}

/**
 *  @brief  Retrieves compare function in v's ttbl
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return     compare function used by v
 */
compare_fn vgetcompare_str(vector_str *v) {
    massert_container(v);
    return v->ttbl->compare;
}

/**
 *  @brief  Retrieves print function in v's ttbl
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return     print function used by v
 */
print_fn vgetprint_str(vector_str *v) {
    massert_container(v);
    return v->ttbl->print;
}

/**
 *  @brief  Retrieves typetable used by v
 *
 *  @param[in]  v   pointer to vector_str
 *
 *  @return     pointer to typetable
 */
struct typetable *vgetttbl_str(vector_str *v) {
    massert_container(v);
    return v->ttbl;
}

/**
 *  @brief  Calls malloc to allocate memory for a pointer to vector_str
 *
 *  @return     pointer to vector_str
 */
static vector_str *vallocate_str(void) {
    vector_str *v = NULL;
    v = malloc(sizeof *v);
    return v;
}

/**
 *  @brief  "Constructor" function, initializes vector
 *
 *  @param[in]  v           pointer to vector_str
 *  @param[in]  capacity    capacity desired for vector
 */
static void vinit_str(vector_str *v, size_t capacity) {
    str *start = NULL;

    massert_container(v);

    v->ttbl = _str_ ? _str_ : _void_ptr_;

    if (v->ttbl != _void_ptr_) {
        v->ttbl->compare = v->ttbl->compare ? v->ttbl->compare : NULL;
        v->ttbl->copy = v->ttbl->copy ? v->ttbl->copy : NULL;
        v->ttbl->print = v->ttbl->print ? v->ttbl->print : NULL;
        v->ttbl->dtor = v->ttbl->dtor ? v->ttbl->dtor : NULL;
    }

    if (capacity <= 0) {
        WARNING(__FILE__, "Provided input capacity was less than or equal to 0. Will default to capacity of 1.");
        capacity = 1;
    } 

    start = calloc(capacity, v->ttbl->width);
    massert_calloc(start);

    v->impl.start = start;
    v->impl.finish = v->impl.start;
    v->impl.end_of_storage = v->impl.start + capacity;
}

/**
 *  @brief "Destructor" function, deinitializes vector
 *
 *  @param[in]  v   pointer to vector_str
 */
static void vdeinit_str(vector_str *v) {
    if (v == NULL) {
        return;
    }

    vclear_str(v);

    free(v->impl.start);
    v->impl.start = NULL;
    v->impl.finish = NULL;
    v->impl.end_of_storage = NULL;

    v->ttbl = NULL;
}

/**
 *  @brief  Swaps the content at first and second, address from within v
 *
 *  @param[in]  v       pointer to vector_str
 *  @param[out] first   first address to swap content
 *  @param[out] second  second address to swap content
 */
static void vswapaddr_str(vector_str *v, str *first, str *second) {
    str *temp = NULL;

    massert_container(v);
    massert_ptr(first);
    massert_ptr(second);

    temp = malloc(v->ttbl->width);
    massert_malloc(temp);
    memcpy(temp, first, v->ttbl->width);

    memcpy(first, second, v->ttbl->width);
    memcpy(second, temp, v->ttbl->width);

    free(temp);
    temp = NULL;
}

/**
 *  @brief  Initializes and returns an iterator that refers to arg
 *
 *  @param[in]  arg     pointer to vector_str
 *
 *  @return     iterator that refers to v,
 *              position is at v's first element
 */
static iterator vibegin_str(void *arg) {
    vector_str *v = NULL;
    iterator it;

    v = (vector_str *)(arg);

    it.itbl = vector_iterator_table_ptr_id_str;
    it.container = v;
    it.curr = v->impl.start;

    return it;
}

/**
 *  @brief  Initializes and returns an iterator that refers to arg
 *
 *  @param[in]  arg     pointer to vector_str
 *
 *  @return     iterator that refers to v;
 *              position is at one block past v's last element
 */
static iterator viend_str(void *arg) {
    vector_str *v = NULL;
    iterator it;

    v = (vector_str *)(arg);

    it.itbl = vector_iterator_table_ptr_id_str;
    it.container = v;
    it.curr = v->impl.finish;

    return it;
}

/**
 *  @brief  Initializes and returns an iterator that
 *          is one block past it's current position
 *
 *  @param[in]  it      iterator that refers to a vector_str
 *
 *  @return     a new iterator that is one block past it's current position
 */
static iterator vinext_str(iterator it) {
    vector_str *v = NULL;
    iterator iter;

    v = (vector_str *)(it.container);

    iter.itbl = vector_iterator_table_ptr_id_str;
    iter.container = v;
    iter.curr = it.curr;

    if (iter.curr == v->impl.finish) {
        ERROR(__FILE__, "Cannot advance - iterator already at end.");
    } else {
        iter.curr = (char *)(v->impl.finish) + (v->ttbl->width);
    }

    return iter;
}

/**
 *  @brief  Initializes and returns an iterator that
 *          is n blocks past it's current position
 *
 *  @param[in]  it      iterator that refers to a vector_str
 *
 *  @return     a new iterator that is n block's past it's current position
 */
static iterator vinextn_str(iterator it, int n) {
    vector_str *v = NULL;
    iterator iter;
    int pos = 0;

    v = (vector_str *)(it.container);

    iter.itbl = vector_iterator_table_ptr_id_str;
    iter.container = v;
    iter.curr = it.curr;

    pos = ptr_distance(v->impl.start, iter.curr, v->ttbl->width);

    if ((vsize_str(v) - pos) <= 0) {
        char str[256];
        sprintf(str, "Cannot advance %d times from position %d.", n, pos);
        ERROR(__FILE__, str);
    } else {
        iter.curr = (char *)(iter.curr) + (n * v->ttbl->width);
    }

    return iter;
}

/**
 *  @brief  Initializes and returns an iterator that
 *          is one block behind it's current position
 *
 *  @param[in]  it      iterator that refers to a vector_str
 *
 *  @return     a new iterator that is one block behind it's current position
 */
static iterator viprev_str(iterator it) {
    vector_str *v = (vector_str *)(it.container);
    iterator iter;

    v = (vector_str *)(it.container);

    iter.itbl = vector_iterator_table_ptr_id_str;
    iter.container = v;
    iter.curr = it.curr;

    if (iter.curr == v->impl.start) {
        ERROR(__FILE__, "Cannot retract - already at begin.");
    } else {
        iter.curr = (char *)(v->impl.finish) - (v->ttbl->width);
    }

    return iter;
}

/**
 *  @brief  Initializes and returns an iterator that
 *          is n blocks behind it's current position
 *
 *  @param[in]  it      iterator that refers to a vector_str
 *
 *  @return     a new iterator that is n block's behind it's current position
 */
static iterator viprevn_str(iterator it, int n) {
    vector_str *v = (vector_str *)(it.container);
    iterator iter;
    int pos = 0;

    v = (vector_str *)(it.container);

    iter.itbl = vector_iterator_table_ptr_id_str;
    iter.container = v;
    iter.curr = it.curr;

    pos = ptr_distance(v->impl.start, iter.curr, v->ttbl->width);

    if ((vsize_str(v) - pos) <= 0) {
        char str[256];
        sprintf(str, "Cannot retract %d times from position %d.", n, pos);
        ERROR(__FILE__, str);
    } else {
        iter.curr = (char *)(iter.curr) + (n * v->ttbl->width);
    }

    return iter;
}

/**
 *  @brief  Determines the distance between first and last numerically
 *
 *  @param[in]  first   pointer to iterator that refers to a vector_str
 *  @param[in]  last    pointer to iterator that refers to a vector_str
 *
 *  @return     numerical distance between first and last
 *
 *  This function can also be used to determine the numerical position
 *  of a particular iterator, since indices are not stored within iterators.
 *
 *  To find the index position of an iterator, leave one of the parameters
 *  NULL when calling the distance function.
 */
static int vidistance_str(iterator *first, iterator *last) {
    vector_str *v = NULL;

    if (first == NULL && last != NULL) {
        v = (vector_str *)(last->container);
        return (int)((str *)(last->curr) - v->impl.start);
    } else if (last == NULL && first != NULL) {
        v = (vector_str *)(first->container);
        return (int)((str *)(first->curr) - v->impl.start);
    } else if (first == NULL && last == NULL) {
        return 0;
    } else {
        v = (vector_str *)(first->container);
        return (int)((str *)(last->curr) - (str *)(first->curr));
    }
}

/**
 *  @brief  Advances the position of it n blocks
 *
 *  @param[in]  it      pointer to iterator that refers to a vector_str
 *  @param[in]  n       desired amount of blocks to move
 *
 *  @return     pointer to iterator
 */
static iterator *viadvance_str(iterator *it, int n) {
    vector_str *v = NULL;
    int pos = 0;

    massert_iterator(it);

    pos = ptr_distance(v->impl.start, it->curr, v->ttbl->width);

    if ((vsize_str(v) - pos) < 0) {
        char str[256];
        sprintf(str, "Cannot advance %d times from position %d.", n, pos);
        ERROR(__FILE__, str);
    } else {
        v = (vector_str *)(it->container);
        it->curr = (char *)(it->curr) + (n * v->ttbl->width);
    }

    return it;
}

/**
 *  @brief  Increments the position of it 1 block forward
 *
 *  @param[in]  it     pointer to iterator that refers to a vector_str
 *
 *  @return     pointer to iterator
 */
static iterator *viincr_str(iterator *it) {
    vector_str *v = NULL;

    massert_iterator(it);

    v = (vector_str *)(it->container);

    if (it->curr == v->impl.finish) {
        ERROR(__FILE__, "Cannot increment - already at end.");
    } else {
        it->curr = (char *)(it->curr) + (v->ttbl->width);
    }

    return it;
}

/**
 *  @brief  Decrements the position of it 1 block backward
 *
 *  @param[in]  it     pointer to iterator that refers to a vector_str
 *
 *  @return     pointer to iterator
 */
static iterator *videcr_str(iterator *it) {
    vector_str *v = NULL;

    massert_iterator(it);

    v = (vector_str *)(it->container);

    if (it->curr == v->impl.start) {
        ERROR(__FILE__, "Cannot decrement - already at begin.");
    } else {
        it->curr = (char *)(it->curr) - (v->ttbl->width);
    }

    return it;
}

/**
 *  @brief  Retrieves the address of the value referred to
 *          by it's current position
 *
 *  @param[in]  it  iterator that refers to a vector_str
 *
 *  @return     address of an element from within a vector
 */
static void *vicurr_str(iterator it) {
    return it.curr;
}

/**
 *  @brief  Retrieves the address of first element from
 *          the vector it is iterating
 *
 *  @param[in]  it  iterator that refers to a vector_str
 *
 *  @return     address of the first element from within a vector
 */
static void *vistart_str(iterator it) {
    vector_str *v = (vector_str *)(it.container);
    return v->impl.start;
}

/**
 *  @brief  Retrieves the address of the block one past
 *          the last element from within the vector it is iterating
 *
 *  @param[in]  it  iterator that refers to a vector_str
 *
 *  @return     address of the element that is one block past
 *              the rear element from within the vector being iterated
 */
static void *vifinish_str(iterator it) {
    vector_str *v = (vector_str *)(it.container);
    return v->impl.finish;
}

/**
 *  @brief  Determines if it has elements to visit in the forward direction
 *
 *  @param[in]  it  iterator that refers to a vector_str
 *
 *  @return     true if elements remain in the forward direction,
 *              false otherwise
 */
static bool vihasnext_str(iterator it) {
    vector_str *v = (vector_str *)(it.container);
    return it.curr != v->impl.finish;
}

/**
 *  @brief  Determines if it has elements to visit in the backward direction
 *
 *  @param[in]  it  iterator that refers to a vector_str
 *
 *  @return     true if elements remain in the backward direction,
 *              false otherwise
 */
static bool vihasprev_str(iterator it) {
    vector_str *v = (vector_str *)(it.container);
    return it.curr != v->impl.start;
}

/**
 *  @brief  Retrieve a container's typetable
 *
 *  @param[in]  arg     pointer to vector_str
 *
 *  @return     pointer to typetable
 */
static struct typetable *vigetttbl_str(void *arg) {
    vector_str *v = (vector_str *)(arg);
    return v->ttbl;
}
