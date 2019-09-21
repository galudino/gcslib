/**
 *  @file       vector_char.c
 *  @brief      Source for preprocessed template instantiation of vector(char)
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

#include "vector_char.h"

/**
 *  @file       mergesort.h
 *  @brief      Public header file for access to mergesort-related functions
 */
#include "mergesort.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

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
 *  @struct     vector_char
 *  @brief      Represents a type-safe dynamic array ADT
 *
 *  Note that struct vector_char and struct vector_base_char are
 *  opaque -- their fields cannot be accessed directly,
 *  nor can instances of struct vector_char/struct vector_base_char
 *  be created on the stack. This is done to enforce encapsulation.
 */
struct vector_char {
    struct vector_base_char {
        char *start; /**< address of array base (first element) */
        char *finish; /**< address reserved for next rear element */
        char *end_of_storage; /**< addresses last allocated block of storage */
    } impl;

    struct typetable *ttbl; /*<< data width, cpy, dtor, swap, compare, print */
};

struct typetable ttbl_vector_char = {
    sizeof(struct vector_char),
    tmpl_vector_copy_char,
    tmpl_vector_dtor_char,
    tmpl_vector_swap_char,
    tmpl_vector_compare_char,
    tmpl_vector_print_char
};

struct typetable *_vector_char_ = &ttbl_vector_char;

static vector_char *vallocate_char(void);
static void vinit_char(vector_char *v, size_t capacity);
static void vdeinit_char(vector_char *v);
static void vswapaddr_char(vector_char *v, char *first, char *second);

static iterator vibegin_char(void *arg);
static iterator viend_char(void *arg);

static iterator vinext_char(iterator it);
static iterator vinextn_char(iterator it, int n);

static iterator viprev_char(iterator it);
static iterator viprevn_char(iterator it, int n);

static int vidistance_char(iterator *first, iterator *last);

static iterator *viadvance_char(iterator *it, int n);
static iterator *viincr_char(iterator *it);
static iterator *videcr_char(iterator *it);

static void *vicurr_char(iterator it);
static void *vistart_char(iterator it);
static void *vifinish_char(iterator it);

static bool vihasnext_char(iterator it);
static bool vihasprev_char(iterator it);

static struct typetable *vigetttbl_char(void *arg);

struct iterator_table itbl_vector_char = {
    vibegin_char,
    viend_char,
    vinext_char,
    vinextn_char,
    viprev_char,
    viprevn_char,
    viadvance_char,
    viincr_char,
    videcr_char,
    vicurr_char,
    vistart_char,
    vifinish_char,
    vidistance_char,
    vihasnext_char,
    vihasprev_char,
    vigetttbl_char
};

struct iterator_table *_vector_iterator_char_ = &itbl_vector_char;

/**
 *  @brief  Allocates, constructs, and returns a pointer to vector_char,
 *          size VECTOR_DEFAULT_CAPACITY (16)
 *
 *  @return     pointer to vector_char
 */
vector_char *vnew_char(void) {
    vector_char *v = vallocate_char(); /* allocate */
    vinit_char(v, VECTOR_DEFAULT_CAPACITY); /* construct */
    return v; /* return */
}

/**
 *  @brief  Allocates, constructs, and returns a pointer to vector_char,
 *          size n
 *
 *  @return     pointer to vector_char
 */
vector_char *vnewr_char(size_t n) {
    vector_char *v = vallocate_char(); /* allocate */
    vinit_char(v, n); /* construct */
    return v; /* return */
}

/**
 *  @brief  Calls vnewr and returns a pointer to vector_char,
 *          filled with n copies of val
 *
 *  @param[in]  n       number of elements to copy
 *  @param[in]  val     element to copy
 *
 *  @return     pointer to vector_char
 *
 *  If ttbl has a copy function defined,
 *  n copies of val will be inserted into the returned vector
 *  using the copy function - this copy function is intended for
 *  deep copies. Otherwise, val will be shallow-copied using memcpy.
 */
vector_char *vnewfill_char(size_t n, char val) {
    vector_char *v = NULL;
    char *sentinel = NULL;

    v = vnewr_char(n);
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
 *  @brief  Calls vnewr and returns a pointer to vector_char,
 *          filled with n copies of valaddr
 *
 *  @param[in]  n       number of elements to copy
 *  @param[in]  valaddr address of element to copy
 *
 *  @return     pointer to vector_char
 *
 *  If ttbl has a copy function defined,
 *  n copies of valaddr will be inserted into the returned vector
 *  using the copy function - this copy function is intended for
 *  deep copies. Otherwise, valaddr will be shallow-copied using memcpy.
 */
vector_char *vnewfillptr_char(size_t n, char *valaddr) {
    vector_char *v = NULL;
    char *sentinel = NULL;

    v = vnewr_char(n);
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
 *  @return     pointer to vector_char
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
vector_char *vnewrnge_char(iterator first, iterator last) {
    int delta = 0;

    struct typetable *ttbl_first = NULL;
    vector_char *v = NULL;

    char *sentinel = NULL;
    char *curr = NULL;

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
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)331, "first and last must have matching container types and refer to the same container.");
        return NULL;
    }

    delta = it_distance(&first, &last);
    ttbl_first = it_get_ttbl(first);

    v = vnewr_char(delta);

    sentinel = it_curr(last); /* iteration range is [first, last) */

    if (ttbl_first->copy) {
        while ((curr = it_curr(first)) != sentinel) {
            ttbl_first->copy(v->impl.finish++, curr);

            it_incr(&first);
        }
    } else {
        while ((curr = it_curr(first)) != sentinel) {
            memcpy(v->impl.finish++, curr, v->ttbl->width);

            it_incr(&first);
        };
    }

    return v;
}

/**
 *  @brief  Calls vnewr and returns a pointer to vector_char,
 *          filled with elements from an existing vector_char v.
 *
 *  @param[in]  v   pointer to vector_char, containing the desired source elements
 *
 *  @return     pointer to vector, with copies of v's elements
 *
 *  If v uses a ttbl that has a copy function defined,
 *  elements from v will be copied to the returned vector using the
 *  copy function -- this function is meant for deep copying.
 *  Otherwise, the contents of v will be shallow-copied using memcpy.
 */
vector_char *vnewcopy_char(vector_char *v) {
    vector_char *copy = NULL;

    char *sentinel = NULL;
    char *curr = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)378, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    copy = vnewr_char(vcapacity_char(v));

    sentinel = copy->impl.finish + vsize_char(v);
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
 *  @brief  Calls vallocate_char to allocate a new vector_char, named move,
 *          and transfers ownership of v's fields to vector_char move.
 *
 *  @param[out] v   Address of a pointer to vector_char
 *
 *  @return     pointer to vector_char, initialized with v's fields
 *
 *  vector v is reinitialized with the same ttbl, with size 1.
 *  v may be deleted by the client, or reused.
 */
vector_char *vnewmove_char(vector_char **v) {
    vector_char *move = NULL;

    ;if ((((*v)) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "(*v)"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)412, ("['""(*v)""' was found to be NULL - '""(*v)""' must be nonnull to continue.]")); abort();};;

    move = vallocate_char();

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

    vinit_char((*v), 1);

    return move;
}

/**
 *  @brief  Calls vdeinit_char (vector_char's destructor) and deallocates the pointer v.
 *
 *  @param[out] v   Address of a pointer to vector_char
 *
 *  Every call to any of vector_char's "new" functions should be accompanied
 *  by a call to vdelete_char when a pointer to vector_char is no longer needed.
 *
 *  If the vector_char has a ttbl with a dtor function defined,
 *  the elements within the vector_char will be destroyed using the dtor function.
 *
 *  Note that if the elements from within v are dynamically allocated,
 *  and/or the elements have dynamically allocated fields --
 *  and there is no dtor function defined, memory management
 *  will become the client's responsibility.
 */
void vdelete_char(vector_char **v) {
    ;if ((((*v)) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "(*v)"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)449, ("['""(*v)""' was found to be NULL - '""(*v)""' must be nonnull to continue.]")); abort();};;

    /**
     *  Deinitialization involves releasing all dynamically allocated
     *  memory at each field (if dtor function is defined in ttbl,
     *  and is written to release such memory) --
     *  after that step (or if there is no memory to free within the elements),
     *  (*v)->impl.start has its memory freed, and the remainder of (*v)'s fields
     *  are set NULL.
     */
    vdeinit_char((*v));

    /* finally, the memory (*v) points to will be freed. */
    free((*v));
    (*v) = NULL;
}

/**
 *  @brief  Returns an iterator that points to the first element in vector_char
 *
 *  @param[in]  v   pointer to vector_char
 *
 *  @return     iterator that refers to v
 */
iterator vbegin_char(vector_char *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)474, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return vibegin_char(v);
}

/**
 *  @brief  Returns an iterator that pointers to an element that is one block
 *          past the last element in v
 *
 *  @param[in]  v   pointer to vector_char
 *
 *  @return     iterator that refers to v
 */
iterator vend_char(vector_char *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)487, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return viend_char(v);
}

/**
 *  @brief  Returns the logical length of v
 *
 *  @param[in]  v   pointer to vector_char
 *
 *  @return     logical length of v
 */
size_t vsize_char(vector_char *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)499, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->impl.finish - v->impl.start;
}

/**
 *  @brief  Returns the theoretical maximum amount of elements that
 *          can be stored by vector_char
 *
 *  @param[in]  v   pointer to vector_char
 *
 *  @return     theoretical maximum logical length of v
 */
size_t vmaxsize_char(vector_char *v) {
    size_t ptr_sz = 0;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)514, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    /* ptr_sz is 8 bytes on 64 bit system, 4 bytes on 32 bit system */
    ptr_sz = (sizeof(void *) == 8) ? 8 : 4;
    return ((pow)(2.0, (ptr_sz * 8)) / (v->ttbl->width)) - 1;
}

/**
 *  @brief  Resizes the vector_char to size n
 *
 *  @param[in]  v   pointer to vector_char
 *  @param[in]  n   desired size for v
 *
 *  If n is less than the current logical length (vsize_char(v)),
 *  v will be truncated (excess elements are destroyed),
 *  otherwise, v's capacity will be extended.
 */
void vresize_char(vector_char *v, size_t n) {
    size_t old_size = 0;
    size_t old_capacity = 0;

    char *target = NULL;
    char *newstart = NULL;

    int back_index = 0;
    int i = 0;

    size_t fin = 0;
    size_t end = 0;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)544, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    old_size = vsize_char(v);
    old_capacity = vcapacity_char(v);

    if (old_capacity == n) {
        return;
    } else if (n == 0) {
        ulog(stderr, "[WARNING]", "src/vector_tmpl.c", __func__, (long int)552, "vresize must receive a nonzero value for n.");
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
    ;if (((newstart) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "newstart"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)571, ("[Request for heap storage reallocation failed (realloc returned NULL and was assigned to '""newstart""')]")); abort();};;

    fin = n > old_size ? old_size : n;
    end = n > old_size ? n : fin;

    v->impl.start = newstart;
    v->impl.finish = v->impl.start + fin;
    v->impl.end_of_storage = v->impl.start + end;
}

/**
 *  @brief  Resizes the vector_char to size n elements.
 *
 *  @param[in]  v           pointer to vector_char
 *  @param[in]  n           desired size for v
 *  @param[in]  val         element to fill new blocks with
 *
 *  If n exceeds the size/capacity of v, val will be copied
 *  to the new blocks for the resize.
 *
 *  If n is less than or equal to the size of v,
 *  the vector_char will be cleared and new memory of size n will be
 *  initialized, with each block consisting of copies of val.
 */
void vresizefill_char(vector_char *v, size_t n, char val) {
    size_t old_capacity = 0;

    char *sentinel = NULL;
    char *newstart = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)601, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    old_capacity = vcapacity_char(v);

    sentinel = v->impl.start + n;

    if (n > old_capacity) {
        /**
         *  If n is greater than the old size, multiple instances of valaddr are
         *  appended to the rear of the vector following a resize.
         */
        vresize_char(v, n);

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
        ;if (((newstart) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "newstart"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)655, ("[Request for heap storage allocation failed (calloc returned NULL and was assigned to '""newstart""')]")); abort();};;

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
 *  @brief  Resizes the vector_char to size n elements.
 *
 *  @param[in]  v           pointer to vector_char
 *  @param[in]  n           desired size for v
 *  @param[in]  valaddr     address of element to fill new blocks with
 *
 *  If n exceeds the size/capacity of v, valaddr will be copied
 *  to the new blocks for the resize.
 *
 *  If n is less than or equal to the size of v,
 *  the vector_char will be cleared and new memory of size n will be
 *  initialized, with each block consisting of copies of valaddr.
 */
void vresizefillptr_char(vector_char *v, size_t n, char *valaddr) {
    size_t old_capacity = 0;

    char *sentinel = NULL;
    char *newstart = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)696, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    old_capacity = vcapacity_char(v);

    sentinel = v->impl.start + n;

    if (n > old_capacity) {
        /**
         *  If n is greater than the old size, multiple instances of valaddr are
         *  appended to the rear of the vector following a resize.
         */
        vresize_char(v, n);

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
        ;if (((newstart) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "newstart"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)750, ("[Request for heap storage allocation failed (calloc returned NULL and was assigned to '""newstart""')]")); abort();};;

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
 *  @param[in]  v   pointer to vector_char
 *
 *  @return         capacity of v
 */
size_t vcapacity_char(vector_char *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)779, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->impl.end_of_storage - v->impl.start;
}

/**
 *  @brief  Determines if v is an empty vector, or not
 *
 *  @param[in]  v   pointer to vector_char
 *
 *  @return     true if v->impl.start == v->impl.finish, false otherwise
 */
bool vempty_char(vector_char *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)791, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    /**
     *  Since v->impl.finish is always one address ahead of vector's back element,
     *  if v->impl.start == v->impl.finish, the vector is empty.
     */
    return v->impl.start == v->impl.finish;
}

void vreserve_char(vector_char *v, size_t n) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)800, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    /**
     *  Reserve is effectively a resize,
     *  but verifies that the value of n meets the requirements for a reservation.
     */
    if (n > vcapacity_char(v)) {
        vresize_char(v, n);
    } else {
        /* no-op */
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)810, "n must be greater than vector's buffer capacity.");
        return;
    }
}

/**
 *  @brief  Shrinks vector_char's buffer to that of its logical length
 *
 *  @param[in] v    pointer to vector_char
 *
 *  This function is effectively a conditional resize --
 *  but verifies that:
 *      - vector is not empty
 *      - vector's finish pointer is not equal to end_of_storage pointer
 */
void vshrinktofit_char(vector_char *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)826, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    if ((v->impl.start != v->impl.finish) && (v->impl.finish != v->impl.end_of_storage)) {
        vresize_char(v, vsize_char(v));
    }
}

/**
 *  @brief  Retrieves the address of an element from vector at index n
 *
 *  @param[in]  v   pointer to vector_char
 *  @param[in]  n   index of desired element
 *
 *  @return     address of element at n
 *
 *  A bounds check is performed to ensure that n is a valid index.
 */
char *vat_char(vector_char *v, size_t n) {
    size_t size = 0;
    char *target = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)847, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    size = vsize_char(v);
    target = NULL;

    if (n >= size) {
        char str[256];
        sprintf(str, "Input %lu is greater than vector's logical length, %lu -- index out of bounds.", n, size);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)855, str);
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
 *  @brief  Retrieves the address of the front element from vector_char
 *
 *  @param[in]  v   pointer to vector_char
 *
 *  @return     address of front element
 */
char *vfront_char(vector_char *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)879, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->impl.start;
}

/**
 *  @brief  Retrieves the address of the rear element from vector_char
 *
 *  @param[in]  v   pointer to vector_char
 *
 *  @return     address of rear element
 */
char *vback_char(vector_char *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)891, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->impl.finish - 1;
}

/**
 *  @brief  Retrieves the address of vector_char's buffer
 *
 *  @param[in]  v   pointer to vector_char
 *
 *  @return     address of v->impl.start
 */
char **vdata_char(vector_char *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)903, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return &(v->impl.start);
}

/**
 *  @brief  Retrieves the address of an element from vector at index n
 *
 *  @param[in]  v   pointer to vector_char
 *  @param[in]  n   index of desired element
 *
 *  @return     address of element at n
 *
 *  A bounds check is performed to ensure that n is a valid index.
 */
const char *vatconst_char(vector_char *v, size_t n) {
    char *target = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)920, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    if (n >= vsize_char(v)) {
        char str[256];
        sprintf(str, "Input %lu is greater than vector's logical length, %lu -- index out of bounds.", n, vsize_char(v));
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)925, str);
        return NULL;
    }

    target = v->impl.start + n;
    return target ? target : NULL;
}

/**
 *  @brief  Retrieves the address of the front element from vector_char
 *
 *  @param[in]  v   pointer to vector_char
 *
 *  @return     address of front element, const qualified
 */
const char *vfrontconst_char(vector_char *v) {
    const char *result = (const char *)(v->impl.start);
    return result;
}

/**
 *  @brief  Retrieves the address of the rear element from vector_char
 *
 *  @param[in]  v   pointer to vector_char
 *
 *  @return     address of rear element, const qualified
 */
const char *vbackconst_char(vector_char *v) {
    const char *result = (const char *)(v->impl.finish - 1);
    return result;
}

/**
 *  @brief  Retrieves the address of vector_char's buffer
 *
 *  @param[in]  v   pointer to vector_char
 *
 *  @return     address of v->impl.start, const qualified
 */
const char **vdataconst_char(vector_char *v) {
    const char **result = (const char **)(&v->impl.start);
    return result;
}

/**
 *  @brief  Assigns a range of elements to vector_char, starting at its beginning
 *
 *  @param[in]  v       pointer to vector_char
 *  @param[in]  first   represents the beginning of the range (inc)
 *  @param[in]  last    represents the end of the range, (exc)
 *
 *  Elements in this vector will be destroyed, and replaced with
 *  contents from [first, last).
 *  
 *  If the range of [first, last) exceeds that of vcapacity_char(v),
 *  the capacity will be increased to that of size it_distance(&first, &last).
 */
void vassignrnge_char(vector_char *v, iterator first, iterator last) {
    int delta = it_distance(&first, &last);
    struct typetable *ttbl_first = it_get_ttbl(first);

    char *sentinel = it_curr(last);
    char *curr = NULL;

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
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)999, "first and last must matching container types and refer to the same container.");
        return;
    }

    /**
     *  Clear the vector. 
     */
    vclear_char(v);

    /**
     *  Resize vector if necessary.
     */
    if (delta >= vcapacity_char(v)) {
        vresize_char(v, delta);
    }

    if (ttbl_first->copy) {
        while ((curr = it_curr(first)) != sentinel) {
            ttbl_first->copy(v->impl.finish++, curr);
            it_incr(&first);
        }
    } else {
        while ((curr = it_curr(first)) != sentinel) {
            memcpy(v->impl.finish++, curr, v->ttbl->width);
            it_incr(&first);
        };
    }
}

/**
 *  @brief  Assigns n copies of an element, starting at beginning of vector
 *
 *  @param[in]  v           pointer to vector_char
 *  @param[in]  n           amount of elements to assign
 *  @param[in]  val         the element to assign
 * 
 *  Elements in this vector will be destroyed,
 *  and replaced with n copies of val.
 * 
 *  If n exceeds that of vcapacity_char(v),
 *  the capacity of this vector will be increased to that of size n.
 */
void vassignfill_char(vector_char *v, size_t n, char val) {
    char *sentinel = NULL;

    /**
     *  Clear the vector. 
     */
    vclear_char(v);

    /**
     *  Resize vector if necessary.
     */
    if (n > vcapacity_char(v)) {
        vresize_char(v, n);
    }

    sentinel = v->impl.start + n;
    v->impl.finish = v->impl.start;

    if (v->ttbl->copy) {
        while (v->impl.finish != sentinel) {
            v->ttbl->copy(v->impl.finish++, &val);
        }
    } else {
        while (v->impl.finish != sentinel) {
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
 *  If n exceeds that of vcapacity_char(v),
 *  the capacity of this vector will be increased to that of size n.
 */
void vassignfillptr_char(vector_char *v, size_t n, char *valaddr) {
    char *sentinel = NULL;

    /**
     *  Clear the vector. 
     */
    vclear_char(v);

    /**
     *  Resize vector if necessary.
     */
    if (n > vcapacity_char(v)) {
        vresize_char(v, n);
    }

    sentinel = v->impl.start + n;
    v->impl.finish = v->impl.start;

    if (v->ttbl->copy) {
        while (v->impl.finish != sentinel) {
            v->ttbl->copy(v->impl.finish++, valaddr);
        }
    } else {
        while (v->impl.finish != sentinel) {
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
void vpushb_char(vector_char *v, char val) {
   ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1124, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    /**
     *  A doubling strategy is employed when the finish pointer
     *  meets the end_of_storage pointer.
     */
    if (v->impl.finish == v->impl.end_of_storage) {
        vresize_char(v, vcapacity_char(v) * 2);
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
 *  @param[in]  v       pointer to vector_char
 *  @param[in]  valaddr address of element to be copied
 *
 *  If a copy function is defined in v's ttbl,
 *  valaddr will be deep-copied into v.
 *  Otherwise, valaddr will be shallow-copied into v
 *  using memcpy.
 */
void vpushbptr_char(vector_char *v, char *valaddr) {
   ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1157, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
   ;if (((valaddr) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "valaddr"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1158, ("['""valaddr""' was found to be NULL - '""valaddr""' must be nonnull to continue.]")); abort();};;

    /**
     *  A doubling strategy is employed when the finish pointer
     *  meets the end_of_storage pointer.
     */
    if (v->impl.finish == v->impl.end_of_storage) {
        vresize_char(v, vcapacity_char(v) * 2);
    }

    if (v->ttbl->copy) {
        /* if copy fn defined in ttbl, deep copy */
        v->ttbl->copy(v->impl.finish++, valaddr);
    } else {
        /* if no copy defined in ttbl, shallow copy */
        memcpy(v->impl.finish++, valaddr, v->ttbl->width);
    }

    /* finish pointer advanced to the next empty block */
}

/**
 *  @brief  Removes element at the rear of the vector
 *
 *  @param[in]  v   pointer to vector_char
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
void vpopb_char(vector_char *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1196, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

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
 *  @param[in]  v       pointer to vector_char
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
iterator vinsert_char(vector_char *v, iterator pos, char val) {
    char *curr = NULL;
    size_t ipos = 0;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1240, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    /**
     *  A doubling strategy is employed when the finish pointer
     *  meets the end_of_storage pointer.
     */
    if (v->impl.finish == v->impl.end_of_storage) {
        vresize_char(v, vcapacity_char(v) * 2);
    }

    /**
     *  To insert within indices (0, vsize_char(v)),
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
     *  Use iterator pos to swap elements from [pos, vsize_char(v))
     *  val will resize at the index originally specified by pos
     */
    while ((curr = it_curr(pos)) != v->impl.finish) {
        vswapaddr_char(v, curr, v->impl.finish);
        it_incr(&pos);
    }

    /* restore finish pointer to address of rear element */
    ++v->impl.finish;

    /* returned is an iterator at refers to val's position in v */
    return it_next_n(vbegin_char(v), ipos);
}

/**
 *  @brief  Inserts a value into vector at position specified by pos
 *
 *  @param[in]  v       pointer to vector_char
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
iterator vinsertptr_char(vector_char *v, iterator pos, char *valaddr) {
    char *curr = NULL;
    size_t ipos = 0;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1306, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    ;if (((valaddr) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "valaddr"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1307, ("['""valaddr""' was found to be NULL - '""valaddr""' must be nonnull to continue.]")); abort();};;

    /**
     *  A doubling strategy is employed when the finish pointer
     *  meets the end_of_storage pointer.
     */
    if (v->impl.finish == v->impl.end_of_storage) {
        vresize_char(v, vcapacity_char(v) * 2);
    }

    /**
     *  To insert within indices (0, vsize_char(v)),
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
     *  Use iterator pos to swap elements from [pos, vsize_char(v))
     *  val will resize at the index originally specified by pos
     */
    while ((curr = it_curr(pos)) != v->impl.finish) {
        vswapaddr_char(v, curr, v->impl.finish);
        it_incr(&pos);
    }

    /* restore finish pointer to address of rear element */
    ++v->impl.finish;

    /* returned is an iterator at refers to val's position in v */
    return it_next_n(vbegin_char(v), ipos);
}

/**
 *  @brief  Inserts n copies of val into vector at position specified by pos
 *
 *  @param[in]  v       pointer to vector_char
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
iterator vinsertfill_char(vector_char *v, iterator pos, size_t n, char val) {
    size_t ipos = 0;
    size_t old_size = 0;
    size_t old_capacity = 0;

    char *sentinel = NULL;
    char *right_adj = NULL;
    char *finish = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1379, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    ipos = it_distance(NULL, &pos); /**< pos's index position */
    old_size = vsize_char(v); /**< v's former size */
    old_capacity = vcapacity_char(v); /**< v's former capacity */

    if ((old_size + n) >= old_capacity) {
        /**
         *  If inserting n elements will equal or exceed that of old_capacity,
         *  vector will be resized to accomodate ((old_capacity + n) * 2) elements.
         */
        vresize_char(v, (old_capacity + n) * 2);
        pos = it_next_n(vbegin_char(v), ipos);
    }

    if (n <= 0) {
        /* no-op */
        return pos;
    } else if (n == 1) {
        /* if n is 1, redirect to vinsert and return early */
        vinsert_char(v, pos, val);
        return it_next_n(vbegin_char(v), ipos);
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
        return it_next_n(vbegin_char(v), n);
    } else {
        /* decrement finish pointer so it refers to the rear element */
        --v->impl.finish;

        /**
         *  Each element in the range [n, vsize_char(v)) will be moved n blocks over
         *  -- starting with the rear element.
         */

        /**
         *  sentinel will be set one position behind pos.curr
         *  to account for the new element(s) being inserted
         */
        sentinel = (char *)(pos.curr) - 1;

        /* new destination address for finish pointer */
        right_adj = v->impl.finish + n;

        /* need to save this address for later when restoring impl.finish */
        finish = v->impl.finish + (n + 1);

        /**
         *  Elements [n, vsize_char(v)) are moved over n blocks to the right,
         *  starting with v->impl.finish.
         *  Loop stops when finish pointer reach sentinel.
         */
        while (v->impl.finish != sentinel) {
            vswapaddr_char(v, v->impl.finish--, right_adj--);
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

    return it_next_n(vbegin_char(v), ipos);
}

/**
 *  @brief  Inserts n copies of valaddr into vector at position specified by pos
 *
 *  @param[in]  v       pointer to vector_char
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
iterator vinsertfillptr_char(vector_char *v, iterator pos, size_t n, char *valaddr) {
    size_t ipos = 0;
    size_t old_size = 0;
    size_t old_capacity = 0;

    char *sentinel = NULL;
    char *right_adj = NULL;
    char *finish = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1508, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    ipos = it_distance(NULL, &pos); /**< pos's index position */
    old_size = vsize_char(v); /**< v's former size */
    old_capacity = vcapacity_char(v); /**< v's former capacity */

    if ((old_size + n) >= old_capacity) {
        /**
         *  If inserting n elements will equal or exceed that of old_capacity,
         *  vector will be resized to accomodate ((old_capacity + n) * 2) elements.
         */
        vresize_char(v, (old_capacity + n) * 2);
        pos = it_next_n(vbegin_char(v), ipos);
    }

    if (n <= 0) {
        /* no-op */
        return pos;
    } else if (n == 1) {
        /* if n is 1, redirect to vinsert and return early */
        vinsertptr_char(v, pos, valaddr);
        return it_next_n(vbegin_char(v), ipos);
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
        return it_next_n(vbegin_char(v), n);
    } else {
        /* decrement finish pointer so it refers to the rear element */
        --v->impl.finish;

        /**
         *  Each element in the range [n, vsize_char(v)) will be moved n blocks over
         *  -- starting with the rear element.
         */

        /**
         *  sentinel will be set one position behind pos.curr
         *  to account for the new element(s) being inserted
         */
        sentinel = (char *)(pos.curr) - 1;

        /* new destination address for finish pointer */
        right_adj = v->impl.finish + n;

        /* need to save this address for later when restoring impl.finish */
        finish = v->impl.finish + (n + 1);

        /**
         *  Elements [n, vsize_char(v)) are moved over n blocks to the right,
         *  starting with v->impl.finish.
         *  Loop stops when finish pointer reach sentinel.
         */
        while (v->impl.finish != sentinel) {
            vswapaddr_char(v, v->impl.finish--, right_adj--);
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

    return it_next_n(vbegin_char(v), ipos);
}

/**
 *  @brief  Inserts elements from [first, last) into v, at position pos
 *
 *  @param[in]  v       pointer to vector_char
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
iterator vinsertrnge_char(vector_char *v, iterator pos,
                        iterator first, iterator last) {
    size_t ipos = 0;
    size_t old_size = 0;
    size_t old_capacity = 0;
    size_t delta = 0;

    char *sentinel = NULL;
    char *right_adj = NULL;
    char *finish = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1653, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    ipos = it_distance(NULL, &pos); /**< pos's index position */
    old_size = vsize_char(v); /**< v's former size */
    old_capacity = vcapacity_char(v); /**< v's former capacity */
    delta = it_distance(&first, &last); /**< index(last) - index(first) */

    if ((old_size + delta) >= old_capacity) {
        /**
         *  If inserting delta elements will equal or exceed that of
         *  old_capacity, vector will be resized to
         *  accommodate ((old_capacity + delta) * 2) elements.
         */
        vresize_char(v, (old_capacity + delta) * 2);
        pos = it_next_n(vbegin_char(v), ipos);
    }

    if (delta <= 0) {
        /* no-op */
        return pos;
    } else if (delta == 1) {
        /* if delta is 1, redirect to vinsert and return early */
        vinsert_char(v, pos, *(char *)it_curr(first));
        return it_next_n(vbegin_char(v), ipos);
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
        return it_next_n(vbegin_char(v), delta);
    } else {
        /* decrement finish pointer so it refers to the rear element */
        --v->impl.finish;

        /**
         *  Each element in the range [n, vsize_char(v))
         *  will be moved delta blocks over
         *  -- starting with the rear element.
         * 
         *  sentinel will be set one position behind pos.curr
         *  to account for the new element(s) being inserted
         */
        sentinel = (char *)(pos.curr) - 1;

        /* new destination address for finish pointer */
        right_adj = v->impl.finish + delta;

        /* need to save this address for later when restoring impl.finish */
        finish = v->impl.finish + (delta + 1);

        /**
         *  Elements [n, vsize_char(v)) are moved over delta blocks to the right,
         *  starting with v->impl.finish.
         *  Loop stops when finish pointer reaches sentinel
         */
        while (v->impl.finish != sentinel) {
            vswapaddr_char(v, v->impl.finish--, right_adj--);
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

    return it_next_n(vbegin_char(v), ipos);
}

/**
 *  @brief  Moves valaddr into v, at position pos
 *
 *  @param[in]  v       pointer to vector_char
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
iterator vinsertmove_char(vector_char *v, iterator pos, char *valaddr) {
    char *dst = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1780, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    ;if (((valaddr) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "valaddr"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1781, ("['""valaddr""' was found to be NULL - '""valaddr""' must be nonnull to continue.]")); abort();};;

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

    return vinsertptr_char(v, pos, dst);
}

/**
 *  @brief  Removes an element from v at position pos
 *
 *  @param[in]  v   pointer to vector_char
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
iterator verase_char(vector_char *v, iterator pos) {
    int ipos = 0;
    size_t back_index = 0;

    char *curr = NULL;
    char *sentinel = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1834, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    ipos = it_distance(NULL, &pos);
    back_index = vsize_char(v) - 1;

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
        vpopb_char(v);

        /**
         *  pos will no longer refer to an existing element,
         *  so an iterator is returned referring to an element
         *  at the erased element's former index
         */
        return it_next_n(vbegin_char(v), ipos);
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
            vswapaddr_char(v, curr, it_curr(it_next(pos)));
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
    return it_next_n(vbegin_char(v), ipos);
}

/**
 *  @brief  Removes elements from v ranging from [pos, last)
 *
 *  @param[in]  v       pointer to vector_char
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
iterator verasernge_char(vector_char *v, iterator pos, iterator last) {
    int ipos = 0;
    int delta = 0;

    size_t back_index = 0;

    char *curr = NULL;
    char *sentinel = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1923, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    ipos = it_distance(NULL, &pos); /**< index of pos */
    delta = it_distance(&pos, &last); /**< diff between pos/last */

    back_index = vsize_char(v) - 1;

    if ((ipos < 0) || (ipos >= back_index + 1)) {
        /**
         *  If ipos is negative or ipos is greater than or equal to vsize_char(v),
         *  no-op
         */
        return pos;
    } else if (ipos == back_index || pos.curr == (v->impl.finish - 1)) {
        /**
         *  If ipos == back_index
         *  or pos refers to rear vector element,
         *  redirect to popb and return early
         */
        vpopb_char(v);

        /**
         *  pos will no longer refer to an existing element,
         *  so an iterator is returned referring to an element
         *  at the erased element's former index
         */
        return it_next_n(vbegin_char(v), ipos);
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
        pos = it_next_n(vbegin_char(v), ipos);

        /* reassigning sentinel to one block past the last elem */
        sentinel = v->impl.finish;

        /**
         *  iterator last will advance until it reaches sentinel
         *  (finish pointer) as iterator pos advances alongside it -- they will swap elements between each other
         *  until last reaches the end of the vector.
         */
        while ((curr = it_curr(last)) != sentinel) {
            vswapaddr_char(v, curr, it_curr(pos));

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
    return it_next_n(vbegin_char(v), ipos);
}

/**
 *  @brief  Swaps fields between v and other
 *
 *  @param[out] v       address of pointer to vector
 *  @param[out] other   address of pointer to vector
 */
void vswap_char(vector_char **v, vector_char * *other) {
    vector_char *temp = NULL;

    ;if ((((*v)) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "(*v)"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2004, ("['""(*v)""' was found to be NULL - '""(*v)""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    ;if ((((*other)) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "(*other)"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2005, ("['""(*other)""' was found to be NULL - '""(*other)""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

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
 *  @param[in]  v   pointer to vector_char
 *
 *  Memory management of dynamically allocated elements
 *  and/or elements with dynamically allocated fields
 *  become the client's responsibility if a dtor function
 *  is NOT defined within v's ttbl.
 */
void vclear_char(vector_char *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2035, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

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
        bzero(v->impl.start, vsize_char(v));
        /* v->impl.finish is already at v->impl.start. */
    } else {
        /**
         *  If elements were shallow copied,
         *  (no dtor function specified in ttbl) --
         *  we just memset and reset the finish pointer.
         */
        bzero(v->impl.start, vsize_char(v));
        v->impl.finish = v->impl.start;
    }
}

/**
 *  @brief  Inserts a value into vector at position specified by index
 *
 *  @param[in]  v       pointer to vector_char
 *  @param[in]  index   numerical index of element from within v
 *  @param[in]  val     element to insert
 *
 *  If a copy function is defined in v's ttbl,
 *  val will be deep-copied into v.
 *  Otherwise, val will be shallow-copied into v
 *  using memcpy.
 */
void vinsertat_char(vector_char *v, size_t index, char val) {
    size_t size = 0;

    char *curr = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2091, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    size = vsize_char(v);

    if (index >= size) {
        char str[256];
        sprintf(str, "index provided [%lu] is out of bounds. size of vector is %lu.", index, size);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2098, str);
        return;
    }

    /**
     *  A doubling strategy is employed when the finish pointer
     *  meets the end_of_storage pointer.
     */
    if (v->impl.finish == v->impl.end_of_storage) {
        vresize_char(v, vcapacity_char(v) * 2);
    }

    /**
     *  To insert within indices (0, vsize_char(v)),
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
     *  Use index to swap elements from [index, vsize_char(v))
     *  val will reside at the index originally specified by
     *  index.
     */
    curr = v->impl.start + index;
    while (curr != v->impl.finish) {
        vswapaddr_char(v, curr++, v->impl.finish);
    }

    /* restore finish pointer to address of rear element */
    ++v->impl.finish;
}

/**
 *  @brief  Inserts a value into vector at position specified by index
 *
 *  @param[in]  v       pointer to vector_char
 *  @param[in]  index   numerical index of element from within v
 *  @param[in]  valaddr address of element to insert
 *
 *  If a copy function is defined in v's ttbl,
 *  valaddr will be deep-copied into v.
 *  Otherwise, valaddr will be shallow-copied into v
 *  using memcpy.
 */
void vinsertatptr_char(vector_char *v, size_t index, char *valaddr) {
    size_t size = 0;

    char *curr = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2157, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    ;if (((valaddr) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "valaddr"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2158, ("['""valaddr""' was found to be NULL - '""valaddr""' must be nonnull to continue.]")); abort();};;

    size = vsize_char(v);

    if (index >= size) {
        char str[256];
        sprintf(str, "index provided [%lu] is out of bounds. size of vector is %lu.", index, size);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2165, str);
        return;
    }

    /**
     *  A doubling strategy is employed when the finish pointer
     *  meets the end_of_storage pointer.
     */
    if (v->impl.finish == v->impl.end_of_storage) {
        vresize_char(v, vcapacity_char(v) * 2);
    }

    /**
     *  To insert within indices (0, vsize_char(v)),
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
     *  Use index to swap elements from [index, vsize_char(v))
     *  val will reside at the index originally specified by
     *  index.
     */
    curr = v->impl.start + index;
    while (curr != v->impl.finish) {
        vswapaddr_char(v, curr++, v->impl.finish);
    }

    /* restore finish pointer to address of rear element */
    ++v->impl.finish;
}

/**
 *  @brief  Removes an element from v at position pos
 *
 *  @param[in]  v       pointer to vector_char
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
void veraseat_char(vector_char *v, size_t index) {
    size_t size = 0;
    size_t back_index = 0;

    char *curr = NULL;
    char *next = NULL;
    char *sentinel = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2229, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    size = vsize_char(v);

    if (index >= size) {
        char str[256];
        sprintf(str, "index provided [%lu] is out of bounds. size of vector is %lu.", index, size);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2236, str);
        return;
    }

    back_index = size - 1;

    curr = v->impl.start + index; /* element at index */
    next = index <= vcapacity_char(v) ? (curr + 1) : NULL; /* elem adj to curr */
    sentinel = v->impl.finish; /* one block after last elem */

    if (index == back_index || curr == (v->impl.finish - 1)) {
        /**
         *  If index == back_index
         *  or pos refers to rear vector element,
         *  redirect to popb and return early
         */
        vpopb_char(v);
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
            vswapaddr_char(v, curr++, next++);
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
void vreplaceat_char(vector_char *v, size_t index, char val) {
    size_t size = 0;

    char *curr = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2301, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    size = vsize_char(v);

    if (index >= size) {
        char str[256];
        sprintf(str, "index provided [%lu] is out of bounds. size of vector is %lu.", index, size);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2308, str);
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
void vreplaceatptr_char(vector_char *v, size_t index, char *valaddr) {
    size_t size = 0;

    char *curr = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2355, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    size = vsize_char(v);

    if (index >= size) {
        char str[256];
        sprintf(str, "index provided [%lu] is out of bounds. size of vector is %lu.", index, size);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2362, str);
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
 *  @param[in]  v   pointer to vector_char
 *  @param[in]  n1  index of first element
 *  @param[in]  n2  index of second element
 */
void vswapelem_char(vector_char *v, size_t n1, size_t n2) {
    size_t size = 0;
    size_t capacity = 0;

    bool n1_bad = 0;
    bool n2_bad = 0;
    bool good_indices = 0;

    char *temp = NULL;
    char *data_1 = NULL;
    char *data_2 = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2401, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    size = vsize_char(v);
    capacity = vcapacity_char(v);

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
            ;if (((temp) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "temp"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2419, ("['""temp""' was found to be NULL - '""temp""' must be nonnull to continue.]")); abort();};;
            memcpy(temp, data_1, v->ttbl->width);

            memcpy(data_1, data_2, v->ttbl->width);
            memcpy(data_2, temp, v->ttbl->width);

            free(temp);
            temp = NULL;
        }
    } else {
        char str[256];
        sprintf(str, "indices n1 [%lu] and/or n2 [%lu] are out of bounds.", n1, n2);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2431, str);
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
void vremove_char(vector_char *v, char val) {
    size_t i = 0;
    size_t size = 0;

    char *curr = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2459, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    if (v->impl.start == v->impl.finish) {
        return;
    }

    i = 0;
    size = vsize_char(v);
    curr = v->impl.start;

    if (v->ttbl->compare(curr, &val) == 0) {
        veraseat_char(v, i);
        --size;
        ++curr;
    }

    for (i = 1; i < size; i++) {
        if (v->ttbl->compare(curr, &val) == 0) {
            --curr;
            veraseat_char(v, i--);
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
void vremoveif_char(vector_char *v, bool (*unary_predicate)(const void *)) {
    size_t i = 0;
    size_t size = 0;

    char *curr = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2501, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    ;if (((unary_predicate) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "unary_predicate"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2502, ("['""unary_predicate""' was found to be NULL - '""unary_predicate""' must be assigned to a function with a matching prototype.]")); abort();};;

    if (v->impl.start == v->impl.finish) {
        return;
    }

    i = 0;
    size = vsize_char(v);
    curr = v->impl.start;

    if (unary_predicate(curr) == 1) {
        veraseat_char(v, i);
        --size;
        ++curr;
    }

    for (i = 1; i < size; i++) {
        if (unary_predicate(curr) == 1) {
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
 *  @param[in]  v       pointer to vector_char
 *  @param[in]  other   pointer to vector_char
 *
 *  @return     pointer to vector_char with other's elements appended
 *
 *  The merging of vector_char v and vector_char other does not mutate other.
 */
vector_char *vmerge_char(vector_char *v, vector_char * other) {
    size_t size_other = 0;
    size_t capacity_v = 0;

    char *sentinel = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2545, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    ;if (((other) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "other"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2546, ("['""other""' was found to be NULL - '""other""' must be nonnull to continue.]")); abort();};;

    size_other = vsize_char(v);
    capacity_v = vcapacity_char(v);

    if (size_other >= capacity_v) {
        /**
         *  If (other's size + v's current size) 
         *  will exceed that of v's capacity,
         *  resize v
         */
        vresize_char(v, capacity_v * 2);
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
void vreverse_char(vector_char *v) {
    char *back = NULL;
    char *restore = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2607, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    back = v->impl.finish - 1;

    /**
     *  saving the address of the finish pointer for later,
     *  as the finish pointer will be set to the beginning of the vector
     */
    restore = v->impl.finish;
    v->impl.finish = v->impl.start;

    while (v->impl.finish != back) {
        /* swap addresses at finish and back */
        vswapaddr_char(v, v->impl.finish++, back--);

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
 *  @brief  Returns a new vector_char with the contents of base
 *
 *  @param[in]  ttbl    typetable matching that of bases's element type
 *  @param[in]  base    base address of an array to copy
 *  @param[in]  length  logical length of base
 *
 *  @return     pointer to vector_char with contents of base
 */
vector_char *varrtov_char(char *base, size_t length) {
    vector_char *v = NULL;

    char *target = NULL;

    ;if (((base) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "base"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2649, ("['""base""' was found to be NULL - '""base""' must be nonnull to continue.]")); abort();};;

    v = vnewr_char(length);

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
vector_char *vptrtov_char(char *base, size_t length, size_t capacity) {
    vector_char *v = NULL;

    ;if (((base) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "base"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2692, ("['""base""' was found to be NULL - '""base""' must be nonnull to continue.]")); abort();};;

    v = vallocate_char();

    /**
     *  An appropriate typetable will be chosen that matches
     *  that of the type T for base.
     */
    v->ttbl = _char_
    ? _char_ : _void_ptr_;

    if (v->ttbl != _void_ptr_) {
        v->ttbl->compare = v->ttbl->compare ? v->ttbl->compare : NULL;
        v->ttbl->copy = v->ttbl->copy ? v->ttbl->copy : NULL;
        v->ttbl->print = v->ttbl->print ? v->ttbl->print : NULL;
        v->ttbl->dtor = v->ttbl->dtor ? v->ttbl->dtor : NULL;
    }

    if (capacity <= 0) {
        ulog(stderr, "[WARNING]", "src/vector_tmpl.c", __func__, (long int)2711, "Provided input capacity was less than or equal to 0. This may result in undefined behavior.");
    }

    v->impl.start = base;
    v->impl.finish = v->impl.start + length;
    v->impl.end_of_storage = v->impl.start + capacity;

    return v;
}

/**
 *  @brief  Performs a linear search to find val using the ttbl->compare function
 *
 *  @param[in]  v       pointer to vector_char
 *  @param[in]  val     a copy of an element to find
 */
int vsearch_char(vector_char *v, char val) {
    int (*comparator)(const void *, const void *) = NULL;

    char *curr = NULL;
    bool found = 0;
    int result = 0;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2734, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    comparator = v->ttbl->compare ? v->ttbl->compare : void_ptr_compare;
    curr = v->impl.start;

    /* standard linear search of val using comparator */
    while (curr != v->impl.finish) {
        if (comparator(curr, &val) == 0) {
            found = 1;
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
 *  @param[in]  v   pointer to vector_char
 */
void vsort_char(vector_char *v) {
    size_t size = 0;
    int (*comparator)(const void *, const void *) = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2763, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    size = vsize_char(v);

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
 *  @brief  Prints a diagnostic of vector_char to stdout
 *
 *  @param[in]  v   pointer to vector_char
 */
void vputs_char(vector_char *v) {
    /* redirect to vfputs with stream stdout */
    vfputs_char(v, stdout);
}

/**
 *  @brief  Prints the contents of vector_char with user-defined formatting
 *
 *  @param[in]  v           pointer to vector_char
 *  @param[in]  before      string that appears before any elements appear
 *  @param[in]  after       string that appears after all the elements have appeared
 *  @param[in]  postelem    string that appears after each element, except the last one
 *  @param[in]  breaklim    amount of elements that print before a line break occurs.
 *                          0 means no line breaks
 */
void vputsf_char(vector_char *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim) {
    /* redirect to vfputsf with stream stdout */
    vfputsf_char(v, stdout, before, after, postelem, empty, breaklim);
}

/**
 *  @brief  Prints a diagnostic of vector_char to file stream dest
 *
 *  @param[in]  v       pointer to vector_char
 *  @param[in]  dest    file stream (e.g stdout, stderr, a file)
 */
void vfputs_char(vector_char *v, FILE *dest) {
    char buffer1[16384];
    char buffer2[16384];

    const char *link = "------------------------------";
    const char *bytes_label = NULL;
    const char *postelem = "";
    const char *empty = "--- Container is empty ---";

    const size_t breaklim = 1;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2828, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    ;if (((dest) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "dest"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2829, ("['""dest""' was found to be NULL - '""dest""' must be nonnull to continue.]")); abort();};;

    sprintf(buffer1, "\n%s\n%s\n%s\n", link, "Elements", link);

    bytes_label = v->ttbl->width == 1 ? "byte" : "bytes";

    sprintf(buffer2, "%s\n%s\t\t%lu\n%s\t%lu\n%s\t%lu %s\n%s\n", link, "Size",
            vsize_char(v), "Capacity", vcapacity_char(v), "Element size", v->ttbl->width,
            bytes_label, link);

    vfputsf_char(v, dest, buffer1, buffer2, postelem, empty, breaklim);
}

/**
 *  @brief  Prints the contents of vector_char with user-defined formatting,
 *          to file stream dest
 *
 *  @param[in]  v           pointer to vector_char
 *  @param[in]  dest        file stream (e.g. stdout, stderr, a file)
 *  @param[in]  before      string that appears before any elements appear
 *  @param[in]  after       string that appears after all the elements have appeared
 *  @param[in]  postelem    string that appears after each element, except the last one
 *  @param[in]  breaklim    amount of elements that print before a line break occurs.
 *                          0 means no line breaks
 */
void vfputsf_char(vector_char *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim) {
    void (*print)(const void *, FILE *dest) = NULL;

    size_t size = 0;
    size_t i = 0;
    size_t curr = 0;

    char *target = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2865, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    ;if (((dest) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "dest"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2866, ("['""dest""' was found to be NULL - '""dest""' must be nonnull to continue.]")); abort();};;

    fprintf(dest, "%s", before ? before : "");

    print = v->ttbl->print ? v->ttbl->print : void_ptr_print;

    size = vsize_char(v);

    if (size == 0) {
        fprintf(dest, "%s\n", empty ? empty : "");
    } else {
        target = v->impl.start;

        for (i = 0, curr = 1; i < size; i++, curr++) {
            print(target, dest);

            /* address - disable for release */
            fprintf(dest, "\t\t(%s%p%s)", "\x1B[0;36m" /**< cyan */, (void *)(target), "\x1B[0;0m" /**< reset to standard color/weight */);

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
 *  @param[in]  arg     address of a vector_char pointer
 *  @param[in]  other   address of a vector_char pointer
 *
 *  @return     a pointer to vector_char
 */
void *tmpl_vector_copy_char(void *arg, const void *other) {
    vector_char **dest = NULL;
    vector_char **source = NULL;

    ;if (((other) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "other"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2913, ("['""other""' was found to be NULL - '""other""' must be nonnull to continue.]")); abort();};;

    dest = (vector_char **)(arg);
    source = (vector_char **)(other);

    (*dest) = vnewcopy_char((*source));

    return (*dest);
}

/**
 *  @brief  Wrapper function for a struct typetable
 *
 *  @param[in]  arg     address of a vector_char pointer
 */
void tmpl_vector_dtor_char(void *arg) {
    vector_char **v = NULL;

    ;if (((arg) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "arg"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2931, ("['""arg""' was found to be NULL - '""arg""' must be nonnull to continue.]")); abort();};;

    v = (vector_char **)(arg);
    vdelete_char(v);
}

/**
 *  @brief  Wrapper function for a struct typetable
 *
 *  @param[in]  s1  address of a vector_char pointer
 *  @param[in]  s2  address of a vector_char pointer
 */
void tmpl_vector_swap_char(void *s1, void *s2) {
    vector_char **v1 = (vector_char **)(s1);
    vector_char **v2 = (vector_char **)(s2);

    if ((*v1)) {
        vswap_char(v1, v2);
    } else {
        (*v1) = (*v2);
        (*v2) = NULL;
    }
}

/**
 *  @brief  Wrapper function for a struct typetable
 *
 *  @param[in]  c1  address of a vector_char pointer
 *  @param[in]  c2  address of a vector_char pointer
 *
 *  @return     -1 if c1 and c2 do not share a comparison function
 *              otherwise, accumulated comparison results between
 *              c1 and c2's elements, within their common length
 *              0 means they are both equivalent, within their common length.
 */
int tmpl_vector_compare_char(const void *c1, const void *c2) {
    vector_char *v1 = NULL;
    vector_char *v2 = NULL;

    vector_char *vec1 = NULL;
    vector_char *vec2 = NULL;

    size_t size1 = 0;
    size_t size2 = 0;
    size_t size = 0;

    char *target1 = NULL;
    char *target2 = NULL;

    int delta = 0;
    int i = 0;

    ;if (((c1) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "c1"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2983, ("['""c1""' was found to be NULL - '""c1""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    ;if (((c2) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "c2"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2984, ("['""c2""' was found to be NULL - '""c2""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    v1 = *(vector_char **)(c1);
    v2 = *(vector_char **)(c2);

    if (v1->ttbl->compare != v2->ttbl->compare) {
        return -1;
    }

    vec1 = vnewcopy_char(v1);
    vec2 = vnewcopy_char(v2);

    vsort_char(vec1);
    vsort_char(vec2);

    size1 = vsize_char(vec1);
    size2 = vsize_char(vec2);

    size = size1 < size2 ? size1 : size2;

    target1 = vec1->impl.start;
    target2 = vec2->impl.start;

    for (i = 0; i < size; i++) {
        delta += vec1->ttbl->compare(&target1, &target2);

        ++target1;
        ++target2;
    }

    vdelete_char(&vec2);
    vdelete_char(&vec1);

    /* if delta == 0, both vectors are equivalent within their common length. */
    return delta;
}

/**
 *  @brief  Wrapper function for a struct typetable
 *
 *  @param[in]  arg     address of a vector_char pointer
 *  @param[in]  dest    file stream (stdout, stderr, a file)
 */
void tmpl_vector_print_char(const void *arg, FILE *dest) {
    vector_char *v = NULL;

    ;if (((arg) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "arg"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3030, ("['""arg""' was found to be NULL - '""arg""' must be nonnull to continue.]")); abort();};;
    ;if (((dest) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "dest"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3031, ("['""dest""' was found to be NULL - '""dest""' must be nonnull to continue.]")); abort();};;

    v = *(vector_char **)(arg);
    vfputs_char(v, dest);
}

/**
 *  @brief  Reassign ttbl to v
 *
 *  @param[in]  v       pointer to vector_char
 *  @param[in]  ttbl    pointer to typetable
 */
void vsetttbl_char(vector_char *v, struct typetable *ttbl) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3044, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    v->ttbl = ttbl ? ttbl : _void_ptr_;
}

/**
 *  @brief  Retrieves width (data size) in v's ttbl
 *
 *  @param[in]  v   pointer to vector_char
 *
 *  @return     size of data type
 */
size_t vgetwidth_char(vector_char *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3056, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->ttbl->width;
}

/**
 *  @brief  Retrieves copy function in v's ttbl
 *
 *  @param[in]  v   pointer to vector_char
 *
 *  @return     copy function used by v
 */
copy_fn vgetcopy_char(vector_char *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3068, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->ttbl->copy;
}

/**
 *  @brief  Retrieves dtor function in v's ttbl
 *
 *  @param[in]  v   pointer to vector_char
 *
 *  @return     dtor function used by v
 */
dtor_fn vgetdtor_char(vector_char *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3080, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->ttbl->dtor;
}

/**
 *  @brief  Retrieves swap function in v's ttbl
 *
 *  @param[in]  v   pointer to vector_char
 *
 *  @return     swap function used by v
 */
swap_fn vgetswap_char(vector_char *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3092, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->ttbl->swap;
}

/**
 *  @brief  Retrieves compare function in v's ttbl
 *
 *  @param[in]  v   pointer to vector_char
 *
 *  @return     compare function used by v
 */
compare_fn vgetcompare_char(vector_char *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3104, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->ttbl->compare;
}

/**
 *  @brief  Retrieves print function in v's ttbl
 *
 *  @param[in]  v   pointer to vector_char
 *
 *  @return     print function used by v
 */
print_fn vgetprint_char(vector_char *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3116, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->ttbl->print;
}

/**
 *  @brief  Retrieves typetable used by v
 *
 *  @param[in]  v   pointer to vector_char
 *
 *  @return     pointer to typetable
 */
struct typetable *vgetttbl_char(vector_char *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3128, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->ttbl;
}

/**
 *  @brief  Calls malloc to allocate memory for a pointer to vector_char
 *
 *  @return     pointer to vector_char
 */
static vector_char *vallocate_char(void) {
    vector_char *v = NULL;
    v = malloc(sizeof *v);
    return v;
}

/**
 *  @brief  "Constructor" function, initializes vector
 *
 *  @param[in]  v           pointer to vector_char
 *  @param[in]  capacity    capacity desired for vector
 */
static void vinit_char(vector_char *v, size_t capacity) {
    char *start = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3152, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    v->ttbl = _char_
    ? _char_ : _void_ptr_;

    if (v->ttbl != _void_ptr_) {
        v->ttbl->compare = v->ttbl->compare ? v->ttbl->compare : NULL;
        v->ttbl->copy = v->ttbl->copy ? v->ttbl->copy : NULL;
        v->ttbl->print = v->ttbl->print ? v->ttbl->print : NULL;
        v->ttbl->dtor = v->ttbl->dtor ? v->ttbl->dtor : NULL;
    }

    if (capacity <= 0) {
        ulog(stderr, "[WARNING]", "src/vector_tmpl.c", __func__, (long int)3165, "Provided input capacity was less than or equal to 0. Will default to capacity of 1.");
        capacity = 1;
    }

    start = calloc(capacity, v->ttbl->width);
    ;if (((start) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "start"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3170, ("[Request for heap storage allocation failed (calloc returned NULL and was assigned to '""start""')]")); abort();};;

    v->impl.start = start;
    v->impl.finish = v->impl.start;
    v->impl.end_of_storage = v->impl.start + capacity;
}

/**
 *  @brief "Destructor" function, deinitializes vector
 *
 *  @param[in]  v   pointer to vector_char
 */
static void vdeinit_char(vector_char *v) {
    if (v == NULL) {
        return;
    }

    vclear_char(v);

    free(v->impl.start);
    v->impl.start = NULL;
    v->impl.finish = NULL;
    v->impl.end_of_storage = NULL;

    v->ttbl = NULL;
}

/**
 *  @brief  Swaps the content at first and second, address from within v
 *
 *  @param[in]  v       pointer to vector_char
 *  @param[out] first   first address to swap content
 *  @param[out] second  second address to swap content
 */
static void vswapaddr_char(vector_char *v, char *first, char *second) {
    char *temp = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3207, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    ;if (((first) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "first"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3208, ("['""first""' was found to be NULL - '""first""' must be nonnull to continue.]")); abort();};;
    ;if (((second) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "second"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3209, ("['""second""' was found to be NULL - '""second""' must be nonnull to continue.]")); abort();};;

    temp = malloc(v->ttbl->width);
    ;if (((temp) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "temp"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3212, ("[Request for heap storage allocation failed (malloc returned NULL and was assigned to '""temp""')]")); abort();};;
    memcpy(temp, first, v->ttbl->width);

    memcpy(first, second, v->ttbl->width);
    memcpy(second, temp, v->ttbl->width);

    free(temp);
    temp = NULL;
}

/**
 *  @brief  Initializes and returns an iterator that refers to arg
 *
 *  @param[in]  arg     pointer to vector_char
 *
 *  @return     iterator that refers to v,
 *              position is at v's first element
 */
static iterator vibegin_char(void *arg) {
    vector_char *v = NULL;
    iterator it;

    v = (vector_char *)(arg);

    it.itbl = _vector_iterator_char_;
    it.container = v;
    it.curr = v->impl.start;

    return it;
}

/**
 *  @brief  Initializes and returns an iterator that refers to arg
 *
 *  @param[in]  arg     pointer to vector_char
 *
 *  @return     iterator that refers to v;
 *              position is at one block past v's last element
 */
static iterator viend_char(void *arg) {
    vector_char *v = NULL;
    iterator it;

    v = (vector_char *)(arg);

    it.itbl = _vector_iterator_char_;
    it.container = v;
    it.curr = v->impl.finish;

    return it;
}

/**
 *  @brief  Initializes and returns an iterator that
 *          is one block past it's current position
 *
 *  @param[in]  it      iterator that refers to a vector_char
 *
 *  @return     a new iterator that is one block past it's current position
 */
static iterator vinext_char(iterator it) {
    vector_char *v = NULL;
    iterator iter;

    v = (vector_char *)(it.container);

    iter.itbl = _vector_iterator_char_;
    iter.container = v;
    iter.curr = it.curr;

    if (iter.curr == v->impl.finish) {
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3283, "Cannot advance - iterator already at end.");
    } else {
        iter.curr = (char *)(v->impl.finish) + (v->ttbl->width);
    }

    return iter;
}

/**
 *  @brief  Initializes and returns an iterator that
 *          is n blocks past it's current position
 *
 *  @param[in]  it      iterator that refers to a vector_char
 *
 *  @return     a new iterator that is n block's past it's current position
 */
static iterator vinextn_char(iterator it, int n) {
    vector_char *v = NULL;
    iterator iter;
    int pos = 0;

    v = (vector_char *)(it.container);

    iter.itbl = _vector_iterator_char_;
    iter.container = v;
    iter.curr = it.curr;

    pos = ptr_distance(v->impl.start, iter.curr, v->ttbl->width);

    if ((vsize_char(v) - pos) <= 0) {
        char str[256];
        sprintf(str, "Cannot advance %d times from position %d.", n, pos);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3315, str);
    } else {
        iter.curr = (char *)(iter.curr) + (n * v->ttbl->width);
    }

    return iter;
}

/**
 *  @brief  Initializes and returns an iterator that
 *          is one block behind it's current position
 *
 *  @param[in]  it      iterator that refers to a vector_char
 *
 *  @return     a new iterator that is one block behind it's current position
 */
static iterator viprev_char(iterator it) {
    vector_char *v = (vector_char *)(it.container);
    iterator iter;

    v = (vector_char *)(it.container);

    iter.itbl = _vector_iterator_char_;
    iter.container = v;
    iter.curr = it.curr;

    if (iter.curr == v->impl.start) {
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3342, "Cannot retract - already at begin.");
    } else {
        iter.curr = (char *)(v->impl.finish) - (v->ttbl->width);
    }

    return iter;
}

/**
 *  @brief  Initializes and returns an iterator that
 *          is n blocks behind it's current position
 *
 *  @param[in]  it      iterator that refers to a vector_char
 *
 *  @return     a new iterator that is n block's behind it's current position
 */
static iterator viprevn_char(iterator it, int n) {
    vector_char *v = (vector_char *)(it.container);
    iterator iter;
    int pos = 0;

    v = (vector_char *)(it.container);

    iter.itbl = _vector_iterator_char_;
    iter.container = v;
    iter.curr = it.curr;

    pos = ptr_distance(v->impl.start, iter.curr, v->ttbl->width);

    if ((vsize_char(v) - pos) <= 0) {
        char str[256];
        sprintf(str, "Cannot retract %d times from position %d.", n, pos);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3374, str);
    } else {
        iter.curr = (char *)(iter.curr) + (n * v->ttbl->width);
    }

    return iter;
}

/**
 *  @brief  Determines the distance between first and last numerically
 *
 *  @param[in]  first   pointer to iterator that refers to a vector_char
 *  @param[in]  last    pointer to iterator that refers to a vector_char
 *
 *  @return     numerical distance between first and last
 *
 *  This function can also be used to determine the numerical position
 *  of a particular iterator, since indices are not stored within iterators.
 *
 *  To find the index position of an iterator, leave one of the parameters
 *  NULL when calling the distance function.
 */
static int vidistance_char(iterator *first, iterator *last) {
    vector_char *v = NULL;

    if (first == NULL && last != NULL) {
        v = (vector_char *)(last->container);
        return (int)((char *)(last->curr) - v->impl.start);
    } else if (last == NULL && first != NULL) {
        v = (vector_char *)(first->container);
        return (int)((char *)(first->curr) - v->impl.start);
    } else if (first == NULL && last == NULL) {
        return 0;
    } else {
        v = (vector_char *)(first->container);
        return (int)((char *)(last->curr) - (char *)(first->curr));
    }
}

/**
 *  @brief  Advances the position of it n blocks
 *
 *  @param[in]  it      pointer to iterator that refers to a vector_char
 *  @param[in]  n       desired amount of blocks to move
 *
 *  @return     pointer to iterator
 */
static iterator *viadvance_char(iterator *it, int n) {
    vector_char *v = NULL;
    int pos = 0;

    if (((it) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "it"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3425, ("['""it""' was found to be NULL - '""it""' must point to an initialized iterator, such that it refers to a non-null pointer-to-container.]")); abort();};;

    pos = ptr_distance(v->impl.start, it->curr, v->ttbl->width);

    if ((vsize_char(v) - pos) < 0) {
        char str[256];
        sprintf(str, "Cannot advance %d times from position %d.", n, pos);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3432, str);
    } else {
        v = (vector_char *)(it->container);
        it->curr = (char *)(it->curr) + (n * v->ttbl->width);
    }

    return it;
}

/**
 *  @brief  Increments the position of it 1 block forward
 *
 *  @param[in]  it     pointer to iterator that refers to a vector_char
 *
 *  @return     pointer to iterator
 */
static iterator *viincr_char(iterator *it) {
    vector_char *v = NULL;

    if (((it) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "it"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3451, ("['""it""' was found to be NULL - '""it""' must point to an initialized iterator, such that it refers to a non-null pointer-to-container.]")); abort();};;

    v = (vector_char *)(it->container);

    if (it->curr == v->impl.finish) {
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3456, "Cannot increment - already at end.");
    } else {
        it->curr = (char *)(it->curr) + (v->ttbl->width);
    }

    return it;
}

/**
 *  @brief  Decrements the position of it 1 block backward
 *
 *  @param[in]  it     pointer to iterator that refers to a vector_char
 *
 *  @return     pointer to iterator
 */
static iterator *videcr_char(iterator *it) {
    vector_char *v = NULL;

    if (((it) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "it"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3474, ("['""it""' was found to be NULL - '""it""' must point to an initialized iterator, such that it refers to a non-null pointer-to-container.]")); abort();};;

    v = (vector_char *)(it->container);

    if (it->curr == v->impl.start) {
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3479, "Cannot decrement - already at begin.");
    } else {
        it->curr = (char *)(it->curr) - (v->ttbl->width);
    }

    return it;
}

/**
 *  @brief  Retrieves the address of the value referred to
 *          by it's current position
 *
 *  @param[in]  it  iterator that refers to a vector_char
 *
 *  @return     address of an element from within a vector
 */
static void *vicurr_char(iterator it) {
    return it.curr;
}

/**
 *  @brief  Retrieves the address of first element from
 *          the vector it is iterating
 *
 *  @param[in]  it  iterator that refers to a vector_char
 *
 *  @return     address of the first element from within a vector
 */
static void *vistart_char(iterator it) {
    vector_char *v = (vector_char *)(it.container);
    return v->impl.start;
}

/**
 *  @brief  Retrieves the address of the block one past
 *          the last element from within the vector it is iterating
 *
 *  @param[in]  it  iterator that refers to a vector_char
 *
 *  @return     address of the element that is one block past
 *              the rear element from within the vector being iterated
 */
static void *vifinish_char(iterator it) {
    vector_char *v = (vector_char *)(it.container);
    return v->impl.finish;
}

/**
 *  @brief  Determines if it has elements to visit in the forward direction
 *
 *  @param[in]  it  iterator that refers to a vector_char
 *
 *  @return     true if elements remain in the forward direction,
 *              false otherwise
 */
static bool vihasnext_char(iterator it) {
    vector_char *v = (vector_char *)(it.container);
    return it.curr != v->impl.finish;
}

/**
 *  @brief  Determines if it has elements to visit in the backward direction
 *
 *  @param[in]  it  iterator that refers to a vector_char
 *
 *  @return     true if elements remain in the backward direction,
 *              false otherwise
 */
static bool vihasprev_char(iterator it) {
    vector_char *v = (vector_char *)(it.container);
    return it.curr != v->impl.start;
}

/**
 *  @brief  Retrieve a container's typetable
 *
 *  @param[in]  arg     pointer to vector_char
 *
 *  @return     pointer to typetable
 */
static struct typetable *vigetttbl_char(void *arg) {
    vector_char *v = (vector_char *)(arg);
    return v->ttbl;
}
