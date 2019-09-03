/**
 *  @file       vector_tmpl.h
 *  @brief      Template source file for a type-safe dynamic array ADT
 *
 *  @author     Gemuele Aludino
 *  @date       11 Jul 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#ifdef T

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// optional macros for accessing the innards of vector_base
#define AT(VEC, INDEX)      ((char *)(VEC->impl.start) + ((INDEX) * (VEC->ttbl->width)))
#define FRONT(VEC)          VEC->impl.start
#define BACK(VEC)           ((char *)(VEC->impl.finish) - (VEC->ttbl->width))
#define END(VEC)            VEC->impl.end_of_storage

#define vallocate(T) tmpl(vallocate, T)
#define vinit(T) tmpl(vinit, T)
#define vdeinit(T) tmpl(vdeinit, T)
#define vswapaddr(T) tmpl(vswapaddr, T)

#define vector_iterator(T) tmpl(vector_iterator, T)

#define vibegin(T) tmpl(vibegin, T)
#define viend(T) tmpl(viend, T)

#define vinext(T) tmpl(vinext, T)
#define vinextn(T) tmpl(vinextn, T)

#define viprev(T) tmpl(viprev, T)
#define viprevn(T) tmpl(viprevn, T)

#define vidistance(T) tmpl(vidistance, T)

#define viadvance(T) tmpl(viadvance, T)
#define viincr(T)    tmpl(viincr, T)
#define videcr(T) tmpl(videcr, T)

#define vicurr(T) tmpl(vicurr, T)
#define vistart(T) tmpl(vistart, T)
#define vifinish(T) tmpl(vifinish, T)

#define vihasnext(T) tmpl(vihasnext, T)
#define vihasprev(T) tmpl(vihasprev, T)

#define vigetttbl(T) tmpl(vigetttbl, T)

struct vector(T) {
    struct vector_base(T) {
        T *start;           /**< address of array base (first element) */
        T *finish;          /**< address reserved for next rear element */
        T *end_of_storage;  /**< addresses last allocated block of storage */
    } impl;

    struct typetable *ttbl; /*<< data width, cpy, dtor, swap, compare, print */
};

struct typetable table_id(ttbl_vector, T) = {
    sizeof(struct vector(T)),
    tmpl_vector_copy(T),
    tmpl_vector_dtor(T),
    tmpl_vector_swap(T),
    tmpl_vector_compare(T),
    tmpl_vector_print(T)
};

struct typetable *vector_typetable_ptr_id(T) = &table_id(ttbl_vector, T);

static vector(T) *vallocate(T)(void);
static void vinit(T)(vector(T) *v, size_t capacity);
static void vdeinit(T)(vector(T) *v);
static void vswapaddr(T)(vector(T) *v, T *first, T *second);

static iterator vibegin(T)(void *arg);
static iterator viend(T)(void *arg);

static iterator vinext(T)(iterator it);
static iterator vinextn(T)(iterator it, int n);

static iterator viprev(T)(iterator it);
static iterator viprevn(T)(iterator it, int n);

static int vidistance(T)(iterator *first, iterator *last);

static iterator *viadvance(T)(iterator *it, int n);
static iterator *viincr(T)(iterator *it);
static iterator *videcr(T)(iterator *it);

static void *vicurr(T)(iterator it);
static void *vistart(T)(iterator it);
static void *vifinish(T)(iterator it);

static bool vihasnext(T)(iterator it);
static bool vihasprev(T)(iterator it);

static struct typetable *vigetttbl(T)(void *arg);

struct iterator_table table_id(itbl_vector, T) = {
    vibegin(T),
    viend(T),
    vinext(T),
    vinextn(T),
    viprev(T),
    viprevn(T),
    viadvance(T),
    viincr(T),
    videcr(T),
    vicurr(T),
    vistart(T),
    vifinish(T),
    vidistance(T),
    vihasnext(T),
    vihasprev(T),
    vigetttbl(T)
};

struct iterator_table *vector_iterator_table_ptr_id(T) = &table_id(itbl_vector, T);

vector(T) *vnew(T)(void) {
    vector(T) *v = vallocate(T)();
    vinit(T)(v, VECTOR_DEFAULT_CAPACITY);
    return v;
}

vector(T) *vnewr(T)(size_t n) {
    vector(T) *v = vallocate(T)();
    vinit(T)(v, n);
    return v;
}

vector(T) *vnewfill(T)(size_t n, T val) {
    vector(T) *v = vnewr(T)(n);

    T *src = NULL;
    T *target = v->impl.start;
    T *sentinel = v->impl.start + n;

    if (v->ttbl->copy) {
        while (target != sentinel) {
            //v->ttbl->copy(&src, &val);
            //memcpy(target++, &src, v->ttbl->width);
            v->ttbl->copy(target++, &val);
        }
    } else {
        while (target != sentinel) {
            memcpy(target++, &val, v->ttbl->width);
        }
    }

    v->impl.finish = sentinel;
    return v;
}

vector(T) *vnewfillptr(T)(size_t n, T *valaddr) {
    vector(T) *v = vnewr(T)(n);

    T *src = NULL;
    T *target = v->impl.start;
    T *sentinel = v->impl.start + n;

    if (v->ttbl->copy) {
        while (target != sentinel) {
            v->ttbl->copy(&src, valaddr);
            memcpy(target++, &src, v->ttbl->width);
        }
    } else {
        while (target != sentinel) {
            memcpy(target++, valaddr, v->ttbl->width);
        }
    }

    v->impl.finish = sentinel;
    return v;
}

vector(T) *vnewrnge(T)(iterator first, iterator last) {
    int delta = it_distance(&first, &last);
    struct typetable *ttbl_first = it_get_ttbl(first);

    vector(T) *v = vnewr(T)(delta);

    T *src = NULL;
    T *target = v->impl.start;
    T *sentinel = v->impl.start + delta;
    T *curr = NULL;

    if (ttbl_first->copy) {
        while (target != sentinel) {
            curr = it_curr(first);

            v->ttbl->copy(&src, curr);
            memcpy(target++, &src, v->ttbl->width);
            it_incr(&first);
        }
    } else {
        while (target != sentinel) {
            curr = it_curr(first);

            src = curr;
            memcpy(target++, src, v->ttbl->width);
            it_incr(&first);
        }
    }

    v->impl.finish = sentinel;
    return v;
}

vector(T) *vnewcopy(T)(vector(T) *v) {
    assert(v);

    vector(T) *copy = vnewr(T)(vcapacity(T)(v));

    T *vdata = *vdata(T)(v);

    T *src = NULL;
    T *target = copy->impl.start;
    T *sentinel = copy->impl.start + vsize(T)(v);
    T *curr = v->impl.start;

    if (v->ttbl->copy) {
        while (target != sentinel) {
            v->ttbl->copy(target, curr);
            ++target;
            ++curr;
        }
    } else {
        while (target != sentinel) {
            src = curr;
            (*target) = (*src);

            ++target;
            ++curr;
        }
     }

    copy->impl.finish = sentinel;
    return copy;
}

vector(T) *vnewmove(T)(vector(T) **v) {
    assert(v);

    vector(T) *move = vallocate(T)();

    move->impl.start = (*v)->impl.start;
    move->impl.finish = (*v)->impl.finish;
    move->impl.end_of_storage = (*v)->impl.end_of_storage;
    move->ttbl = (*v)->ttbl;

    vinit(T)((*v), VECTOR_DEFAULT_CAPACITY);

    return move;
}

void vdelete(T)(vector(T) **v) {
    assert((*v));

    vdeinit(T)((*v));

    free((*v));
    (*v) = NULL;
}

iterator vbegin(T)(vector(T) *v) {
    return vibegin(T)(v);
}

iterator vend(T)(vector(T) *v) {
    return viend(T)(v);
}

size_t vsize(T)(vector(T) *v) {
    assert(v);
    return v->impl.finish - v->impl.start;
}

size_t vmaxsize(T)(vector(T) *v) {
    assert(v);
    // TODO - need verify if this is correct

    if (sizeof(void *) == 8) {
        // 64-bit system
        return (pow(2.0, 64.0) / (v->ttbl->width));
    } else {
        // 32-bit system
        return (pow(2.0, 32.0) / (v->ttbl->width));
    }
}

void vresize(T)(vector(T) *v, size_t n) {
    assert(v);

    size_t old_size = vsize(T)(v);
    size_t old_capacity = vcapacity(T)(v);

    if (old_capacity == n) {
        return;
    }

    if (n < old_size && v->ttbl->dtor) {
        T *target = v->impl.start + n;

        for (int i = 0; i <= n; i++) {
            v->ttbl->dtor(target++);
        }
    }

    T *newstart = NULL;
    newstart = realloc(v->impl.start, n * v->ttbl->width);
    assert(newstart);

    size_t fin = n > old_size ? (old_size) : n;
    size_t end = n > old_size ? n : fin;

    v->impl.start = newstart;
    v->impl.finish = v->impl.start + fin;
    v->impl.end_of_storage = v->impl.start + end;
}

void vresizefill(T)(vector(T) *v, size_t n, T val) {
    assert(v);

    size_t old_size = vsize(T)(v);
    vresize(T)(v, n);

    if (n > old_size) {
        T *src = NULL;
        T *sentinel = v->impl.start + n;

        if (v->ttbl->copy) {
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(&src, &val);
                memcpy(v->impl.finish++, &src, v->ttbl->width);
            }
        } else {
            while (v->impl.finish != sentinel) {
                vpushb(T)(v, val);
            }
        }
    } else {
        vclear(T)(v);

        T *src = NULL;
        T *sentinel = v->impl.start + n;

        if (v->ttbl->copy) {
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(&src, &val);
                memcpy(v->impl.finish++, &src, v->ttbl->width);
            }
        } else {
            while (v->impl.finish != sentinel) {
                (*src) = val;
                vpushb(T)(v, (*src));
            }
        }
    }
}

void vresizefillptr(T)(vector(T) *v, size_t n, T *valaddr) {
    assert(v);

    size_t old_size = vsize(T)(v);
    vresize(T)(v, n);

    if (n > old_size) {
        T *src = NULL;
        T *sentinel = v->impl.start + n;

        if (v->ttbl->copy) {
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(&src, valaddr);
                memcpy(v->impl.finish++, &src, v->ttbl->width);
            }
        } else {
            while (v->impl.finish != sentinel) {
                vpushbptr(T)(v, valaddr);
            }
        }
    } else {
        vclear(T)(v);

        T *src = NULL;
        T *sentinel = v->impl.start + n;

        if (v->ttbl->copy) {
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(&src, valaddr);
                memcpy(v->impl.finish++, &src, v->ttbl->width);
            }
        } else {
            while (v->impl.finish != sentinel) {
                (*src) = (*valaddr);
                vpushbptr(T)(v, src);
            }
        }
    }
}

size_t vcapacity(T)(vector(T) *v) {
    assert(v);
    return v->impl.end_of_storage - v->impl.start;
}

bool vempty(T)(vector(T) *v) {
    assert(v);
    return v->impl.start == v->impl.finish;
}

void vreserve(T)(vector(T) *v, size_t n) {
    assert(v);

    if (n <= vsize(T)(v) || n <= vcapacity(T)(v)) {
        ERROR(__FILE__, "n must be greater than vector's logical length or greater than vector's buffer capacity.");
        return;
    }

    vresize(T)(v, n);
}

void vshrinktofit(T)(vector(T) *v) {
    assert(v);

    size_t size = vsize(T)(v);

    if (size > 0) {
        vresize(T)(v, size);
    }
}

T *vat(T)(vector(T) *v, size_t n) {
    assert(v);
    return v->impl.start + n;
}

T *vfront(T)(vector(T) *v) {
    assert(v);
    return v->impl.start;
}

T *vback(T)(vector(T) *v) {
    assert(v);
    return v->impl.finish - 1;
}

T **vdata(T)(vector(T) *v) {
    assert(v);
    return &(v->impl.start);
}

const T *vatconst(T)(vector(T) *v, size_t n) {
    assert(v);
    const T *result = (const T *)(v->impl.start + n);
    return result;
}

const T *vfrontconst(T)(vector(T) *v) {
    assert(v);
    const T *result = (const T *)(v->impl.start);
    return result;
}

const T *vbackconst(T)(vector(T) *v) {
    assert(v);
    const T *result = (const T *)(v->impl.finish - 1);
    return result;
}

const T **vdataconst(T)(vector(T) *v) {
    assert(v);
    const T **result = (const T **)(&v->impl.start);
    return result;
}

void vassignrnge(T)(vector(T) *v, iterator first, iterator last) {
    // TODO

}

void vassignfill(T)(vector(T) *v, size_t n, T val) {
    // TODO

}

void vassignfillptr(T)(vector(T) *v, size_t n, T *valaddr) {
    // TODO

}

void vpushb(T)(vector(T) *v, T val) {
    assert(v);

    if (v->impl.finish == v->impl.end_of_storage) {
        size_t capacity = vcapacity(T)(v);
        vresize(T)(v, capacity * 2);
    }

    (*v->impl.finish++) = val;
}

void vpushbptr(T)(vector(T) *v, T *valaddr) {
    assert(v);

    if (v->impl.finish == v->impl.end_of_storage) {
        size_t capacity = vcapacity(T)(v);
        vresize(T)(v, capacity * 2);
    }

    (*v->impl.finish++) = (*valaddr);
}

void vpopb(T)(vector(T) *v) {
    assert(v);

    if (v->impl.finish == v->impl.start) {
        return;
    }

    T *target = v->impl.finish - 1;

    if (v->ttbl->dtor) {
        v->ttbl->dtor(target);
    }

    v->impl.finish = target;
}

iterator *vinsert(T)(vector(T) *v, iterator pos, T val) {
    // TODO
    return 0;
}

iterator *vinsertptr(T)(vector(T) *v, iterator pos, T *val) {
    // TODO
    return 0;
}

iterator *vinsertfill(T)(vector(T) *v, iterator pos, size_t n, T val) {
    // TODO
    return 0;
}

iterator *vinsertfillptr(T)(vector(T) *v, iterator pos, size_t n, T *valaddr) {
    // TODO
    return 0;
}

iterator *vinsertrnge(T)(vector(T) *v, iterator pos, iterator first,
                         iterator last) {
    // TODO
    return 0;
}

iterator *vinsertmove(T)(vector(T) *v, iterator *pos, T *valaddr) {
    // TODO
    return 0;
}

iterator *verase(T)(vector(T) *v, iterator *pos) {
    // TODO
    return 0;
}

iterator *verasernge(T)(vector(T) *v, iterator *pos, iterator *last) {
    // TODO
    return 0;
}

void vswap(T)(vector(T) **v, vector(T) * *other) {
    assert((*v));
    assert((*other));

    vector(T) *temp = (*v);

    (*v)->impl.start = (*other)->impl.start;
    (*v)->impl.finish = (*other)->impl.finish;
    (*v)->impl.end_of_storage = (*other)->impl.end_of_storage;

    (*v)->ttbl = (*other)->ttbl;

    (*other)->impl.start = temp->impl.start;
    (*other)->impl.finish = temp->impl.finish;
    (*other)->impl.end_of_storage = temp->impl.end_of_storage;

    (*other)->ttbl = temp->ttbl;
}

void vclear(T)(vector(T) *v) {
    assert(v);

    if (v->impl.finish == v->impl.start) {
        return;
    }

    if (v->ttbl->dtor) {
        T *target = v->impl.finish - 1;

        while (target != v->impl.start) {
            v->ttbl->dtor(target);
            --target;
        }

        v->ttbl->dtor(target);
        v->impl.finish = v->impl.start;
    } else {
        v->impl.finish = v->impl.start;
    }
}

void vinsertat(T)(vector(T) *v, size_t index, T val) {
    assert(v);

    const size_t back_index = vsize(T)(v);

    vpushb(T)(v, val);

    while (index < back_index) {
        vswapelem(T)(v, index++, back_index);
    }
}

void vinsertatptr(T)(vector(T) *v, size_t index, T *valaddr) {
    assert(v);
    assert(valaddr);

    const size_t back_index = vsize(T)(v);

    vpushbptr(T)(v, valaddr);

    while (index < back_index) {
        vswapelem(T)(v, index++, back_index);
    }
}

void veraseat(T)(vector(T) *v, size_t index) {
    const size_t back_index = vsize(T)(v) - 1;

    if ((index < 0) || (index >= back_index + 1)) {
        return;
    } else if (index == back_index) {
        vpopb(T)(v);
        return;
    } else if (index < back_index && index >= 0) {
        for (int i = index; i < back_index; i++) {
            vswapelem(T)(v, i, i + 1);
        }
    }

    vpopb(T)(v);
}

void vreplaceat(T)(vector(T) *v, size_t index, T val) {
    assert(v);

    vpushb(T)(v, val);

    const size_t back_index = vsize(T)(v) - 1;

    if (index >= back_index) {
        return;
    }

    vswapelem(T)(v, back_index, index);
    vpopb(T)(v);
}

void vreplaceatptr(T)(vector(T) *v, size_t index, T *valaddr) {
    assert(v);
    assert(valaddr);

    vpushbptr(T)(v, valaddr);

    const size_t back_index = vsize(T)(v) - 1;

    if (index >= back_index) {
        return;
    }

    vswapelem(T)(v, back_index, index);
    vpopb(T)(v);
}

void vswapelem(T)(vector(T) *v, size_t n1, size_t n2) {
    assert(v);

    size_t size = vsize(T)(v);
    size_t capacity = vcapacity(T)(v);

    bool n1_bad = n1 >= size || n1 >= capacity;
    bool n2_bad = n2 >= size || n2 >= capacity;

    bool good_indices = !n1_bad && !n2_bad;

    if (good_indices && size > 0) {
        T *data_1 = v->impl.start + n1;
        T *data_2 = v->impl.start + n2;

        T temp = (*data_1);
        (*data_1) = (*data_2);
        (*data_2) = temp;
    } else {
        return;
    }
}

void vremove(T)(vector(T) *v, T val) {
    assert(v);

    if (v->impl.start == v->impl.finish) {
        return;
    }

    for (int i = 0; i < vsize(T)(v); i++) {
        if (v->ttbl->compare(vat(T)(v, i), &val) == 0) {
            veraseat(T)(v, i);
            if (i >= 0) {
                --i;
            }
        }
    }
}

void vremoveif(T)(vector(T) *v, T val, bool (*unary_predicate)(const void *)) {
    assert(v);
    assert(unary_predicate);

    if (v->impl.start == v->impl.finish) {
        return;
    }

    if (unary_predicate(&val) == false) {
        return;
    }

    for (int i = 0; i < vsize(T)(v); i++) {
        if (v->ttbl->compare(vat(T)(v, i), &val) == 0) {
            veraseat(T)(v, i);
            if (i >= 0) {
                --i;
            }
        }
    }
}

vector(T) *vmerge(T)(vector(T) *v, vector(T) * other) {
    // TODO
    return 0;
}

void vreverse(T)(vector(T) *v) {
    assert(v);

    const size_t size = vsize(T)(v);

    const size_t half_length = (size / 2);
    const size_t back_index = (size - 1);

    for (size_t i = 0; i < half_length; i++) {
        vswapelem(T)(v, i, back_index - i);
    }
}

vector(T) *varrtov(T)(T *base, size_t length) {
    assert(base);

    struct typetable *table = vector_type_table_ptr_id(T) ? vector_type_table_ptr_id(T) : _void_ptr_;

    vector(T) *v = vnewr(T)(length);
    T *target = base;

    for (size_t i = 0; i < length; i++) {
        vpushbptr(T)(v, target);
        ++target;
    }

    return v;
}

int vsearch(T)(vector(T) *v, T val) {
    // TODO
    return 0;
}

void vsort(T)(vector(T) *v) {
    assert(v);

    size_t size = vsize(T)(v);

    if (size < 2) {
        return;
    }

    int (*comparator)(const void *, const void *)
    = v->ttbl->compare ? v->ttbl->compare : void_ptr_compare;

    // cstdlib qsort (best performance)
    //qsort(v->base, v->length, v->ttbl->width, comparator);

    // cstdlib mergesort
    //mergesort(v->impl.start, size, v->ttbl->width, comparator);

    // gcslib mergesort
    v_mergesort_iterative(v->impl.start, vsize(T)(v), v->ttbl->width, comparator);
}

void vputs(T)(vector(T) *v) {
    vfputs(T)(v, stdout);
}

void vputsf(T)(vector(T) *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim) {
    vfputsf(T)(v, stdout, before, after, postelem, empty, breaklim);
}

void vfputs(T)(vector(T) *v, FILE *dest) {
    assert(v);
    assert(dest);

    char buffer1[MAXIMUM_STACK_BUFFER_SIZE];
    char buffer2[MAXIMUM_STACK_BUFFER_SIZE];

    const char *link = "-------------------------";

    sprintf(buffer1, "\n%s\n%s\n%s\n", link, "Elements", link);

    const char *bytes_label = v->ttbl->width == 1 ? "byte" : "bytes";

    sprintf(buffer2, "%s\n%s\t\t%lu\n%s\t%lu\n%s\t%lu %s\n%s\n", link, "Size",
            vsize(T)(v), "Capacity", vcapacity(T)(v), "Element size", v->ttbl->width,
            bytes_label, link);

    const char *postelem = "";
    const char *empty = "--- Container is empty ---";
    const size_t breaklim = 1;

    vfputsf(T)(v, dest, buffer1, buffer2, postelem, empty, breaklim);
}

void vfputsf(T)(vector(T) *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim) {
    assert(v);
    assert(dest);

    fprintf(dest, "%s", before ? before : "");

    void (*print)(const void *, FILE *dest)
    = v->ttbl->print ? v->ttbl->print : void_ptr_print;

    size_t size = vsize(T)(v);

    if (size == 0) {
        fprintf(dest, "%s\n", empty ? empty : "");
    } else {
        T *target = v->impl.start;

        for (size_t i = 0, curr = 1; i < size; i++, curr++) {
            v->ttbl->print(target, dest);

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

void *tmpl_vector_copy(T)(void *arg, const void *other) {
    assert(other);

    vector(T) **dest = (vector(T) **)(arg);
    vector(T) **source = (vector(T) **)(other);

    (*dest) = vnewcopy(T)((*source));

    return (*dest);
}

void tmpl_vector_dtor(T)(void *arg) {
    assert(arg);

    vector(T) **v = (vector(T) **)(arg);
    vdelete(T)(v);
}

void tmpl_vector_swap(T)(void *s1, void *s2) {
    vector(T) **v1 = (vector(T) **)(s1);
    vector(T) **v2 = (vector(T) **)(s2);

    if ((*v1)) {
        vswap(T)(v1, v2);
    } else {
        (*v1) = (*v2);
        (*v2) = NULL;
    }
}

int tmpl_vector_compare(T)(const void *c1, const void *c2) {
    assert(c1);
    assert(c2);

    vector(T) *v1 = *(vector(T) **)(c1);
    vector(T) *v2 = *(vector(T) **)(c2);

    if (v1->ttbl->compare != v2->ttbl->compare) {
        return -1;
    }

    vector(T) *vec1 = vnewcopy(T)(v1);
    vector(T) *vec2 = vnewcopy(T)(v2);

    vsort(T)(vec1);
    vsort(T)(vec2);

    size_t size1 = vsize(T)(vec1);
    size_t size2 = vsize(T)(vec2);

    size_t size = size1 < size2 ? size1 : size2;

    T *target1 = vec1->impl.start;
    T *target2 = vec2->impl.start;

    int delta = 0;
    for (int i = 0; i < size; i++) {
        delta += vec1->ttbl->compare(&target1, &target2);

        ++target1;
        ++target2;
    }

    vdelete(T)(&vec2);
    vdelete(T)(&vec1);

    // if delta == 0, both vectors are equivalent within their common length.
    return delta;
}

void vsetttbl(T)(vector(T) *v, struct typetable *ttbl) {
    assert(v);
    v->ttbl = ttbl ? ttbl : _void_ptr_;
}

size_t vgetwidth(T)(vector(T) *v) {
    assert(v);
    return v->ttbl->width;
}

copy_fn vgetcopy(T)(vector(T) *v) {
    assert(v);
    return v->ttbl->copy;
}

dtor_fn vgetdtor(T)(vector(T) *v) {
    assert(v);
    return v->ttbl->dtor;
}

swap_fn vgetswap(T)(vector(T) *v) {
    assert(v);
    return v->ttbl->swap;
}

compare_fn vgetcompare(T)(vector(T) *v) {
    assert(v);
    return v->ttbl->compare;
}

print_fn vgetprint(T)(vector(T) *v) {
    assert(v);
    return v->ttbl->print;
}

struct typetable *vgetttbl(T)(vector(T) *v) {
    assert(v);
    return v->ttbl;
}

void tmpl_vector_print(T)(const void *arg, FILE *dest) {
    assert(arg);
    assert(dest);

    vector(T) *v  = *(vector(T) **)(arg);
    vfputs(T)(v, dest);
}

static vector(T) *vallocate(T)(void) {
    vector(T) *v = NULL;
    v = malloc(sizeof *v);
    return v;
}

static void vinit(T)(vector(T) *v, size_t capacity) {
    assert(v);

    v->ttbl = vector_type_table_ptr_id(T)
    ? vector_type_table_ptr_id(T) : _void_ptr_;

    if (v->ttbl != _void_ptr_) {
        v->ttbl->compare = v->ttbl->compare ? v->ttbl->compare : NULL;
        v->ttbl->copy = v->ttbl->copy ? v->ttbl->copy : NULL;
        v->ttbl->print = v->ttbl->print ? v->ttbl->print : NULL;
        v->ttbl->dtor = v->ttbl->dtor ? v->ttbl->dtor : NULL;
    }

    capacity = (capacity <= 0) ? 1 : capacity;

    T *start = NULL;
    start = calloc(capacity, v->ttbl->width);
    assert(start);

    v->impl.start = start;
    v->impl.finish = v->impl.start;
    v->impl.end_of_storage = v->impl.start + capacity;
}

static void vdeinit(T)(vector(T) *v) {
    if (v == NULL) {
        return;
    }

    vclear(T)(v);

    free(v->impl.start);
    v->impl.start = NULL;
    v->impl.finish = NULL;
    v->impl.end_of_storage = NULL;

    v->ttbl = NULL;
}

static iterator vibegin(T)(void *arg) {
    assert(arg);

    vector(T) *v = (vector(T) *)(arg);

    iterator it;
    it.itbl = vector_iterator_table_ptr_id(T);
    it.container = v;
    it.curr = v->impl.start;

    return it;
}

static iterator viend(T)(void *arg) {
    assert(arg);

    vector(T) *v = (vector(T) *)(arg);

    iterator it;
    it.itbl = vector_iterator_table_ptr_id(T);
    it.container = v;
    it.curr = v->impl.finish;

    return it;
}

static iterator vinext(T)(iterator it) {
    vector(T) *v = (vector(T) *)(it.container);

    iterator iter;
    iter.itbl = vector_iterator_table_ptr_id(T);
    iter.container = v;
    iter.curr = it.curr;

    T **ptr = (T **)(&iter.curr);
    ++(*ptr);

    return iter;
}

static iterator vinextn(T)(iterator it, int n) {
    vector(T) *v = (vector(T) *)(it.container);

    iterator iter;
    iter.itbl = vector_iterator_table_ptr_id(T);
    iter.container = v;
    iter.curr = it.curr;

    T **ptr = (T **)(&iter.curr);
    (*ptr) += n;

    return iter;
}

static iterator viprev(T)(iterator it) {
    vector(T) *v = (vector(T) *)(it.container);

    iterator iter;
    iter.itbl = vector_iterator_table_ptr_id(T);
    iter.container = v;
    iter.curr = it.curr;

    T **ptr = (T **)(&iter.curr);
    --(*ptr);

    return iter;
}

static iterator viprevn(T)(iterator it, int n) {
    vector(T) *v = (vector(T) *)(it.container);

    iterator iter;
    iter.itbl = vector_iterator_table_ptr_id(T);
    iter.container = v;
    iter.curr = it.curr;

    T **ptr = (T **)(&iter.curr);
    (*ptr) -= n;

    return iter;
}

static int vidistance(T)(iterator *first, iterator *last) {
    vector(T) *v = NULL;

    if (first == NULL && last != NULL) {
        v = (vector(T) *)(last->container);
        //return ptr_distance(v->impl.start, last->curr, v->ttbl->width);
        return (int)((T *)(last->curr) - v->impl.start);
    } else if (last == NULL && first != NULL) {
        v = (vector(T) *)(first->container);
        //return ptr_distance(v->impl.start, first->curr, v->ttbl->width);
        return (int)((T *)(first->curr) - v->impl.start);
    } else if (first == NULL && last == NULL) {
        return 0;
    } else {
        v = (vector(T) *)(first->container);
        //return ptr_distance(first->curr, last->curr, v->ttbl->width);
        return (int)((T *)(last->curr) - (T *)(first->curr));
    }
}

static iterator *viadvance(T)(iterator *it, int n) {
    assert(it);

    vector(T) *v = (vector(T) *)(it->container);

    T **ptr = (T **)(&it->curr);
    (*ptr) += n;

    return it;
}

static iterator *viincr(T)(iterator *it) {
    assert(it);

    vector(T) *v = (vector(T) *)(it->container);

    T **ptr = (T **)(&it->curr);
    ++(*ptr);
    return it;
}

static iterator *videcr(T)(iterator *it) {
    assert(it);

    vector(T) *v = (vector(T) *)(it->container);

    T **ptr = (T **)(&it->curr);
    --(*ptr);
    return it;
}

static void *vicurr(T)(iterator it) {
    return it.curr;
}

static void *vistart(T)(iterator it) {
    vector(T) *v = (vector(T) *)(it.container);
    return v->impl.start;
}

static void *vifinish(T)(iterator it) {
    vector(T) *v = (vector(T) *)(it.container);
    return v->impl.finish;
}

static bool vihasnext(T)(iterator it) {
    vector(T) *v = (vector(T) *)(it.container);
    return it.curr != v->impl.finish;
}

static bool vihasprev(T)(iterator it) {
    vector(T) *v = (vector(T) *)(it.container);
    return it.curr != v->impl.start;
}

static struct typetable *vigetttbl(T)(void *arg) {
    vector(T) *v = (vector(T) *)(arg);
    return v->ttbl;
}

#endif /* T */
