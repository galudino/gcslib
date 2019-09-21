#include "vector_uint32.h"

truct vector_uint32_t {
    struct vector_base_uint32_t {
        uint32_t *start;
        uint32_t *finish;
        uint32_t *end_of_storage;
    } impl;

    struct typetable *ttbl;
};

struct typetable ttbl_vector_uint32_t = {
    sizeof(struct vector_uint32_t),
    tmpl_vector_copy_uint32_t,
    tmpl_vector_dtor_uint32_t,
    tmpl_vector_swap_uint32_t,
    tmpl_vector_compare_uint32_t,
    tmpl_vector_print_uint32_t
};

struct typetable *_vector_uint32_t_ = &ttbl_vector_uint32_t;

static vector_uint32_t *vallocate_uint32_t(void);
static void vinit_uint32_t(vector_uint32_t *v, size_t capacity);
static void vdeinit_uint32_t(vector_uint32_t *v);
static void vswapaddr_uint32_t(vector_uint32_t *v, uint32_t *first, uint32_t *second);

static iterator vibegin_uint32_t(void *arg);
static iterator viend_uint32_t(void *arg);

static iterator vinext_uint32_t(iterator it);
static iterator vinextn_uint32_t(iterator it, int n);

static iterator viprev_uint32_t(iterator it);
static iterator viprevn_uint32_t(iterator it, int n);

static int vidistance_uint32_t(iterator *first, iterator *last);

static iterator *viadvance_uint32_t(iterator *it, int n);
static iterator *viincr_uint32_t(iterator *it);
static iterator *videcr_uint32_t(iterator *it);

static void *vicurr_uint32_t(iterator it);
static void *vistart_uint32_t(iterator it);
static void *vifinish_uint32_t(iterator it);

static bool vihasnext_uint32_t(iterator it);
static bool vihasprev_uint32_t(iterator it);

static struct typetable *vigetttbl_uint32_t(void *arg);

struct iterator_table itbl_vector_uint32_t = {
    vibegin_uint32_t,
    viend_uint32_t,
    vinext_uint32_t,
    vinextn_uint32_t,
    viprev_uint32_t,
    viprevn_uint32_t,
    viadvance_uint32_t,
    viincr_uint32_t,
    videcr_uint32_t,
    vicurr_uint32_t,
    vistart_uint32_t,
    vifinish_uint32_t,
    vidistance_uint32_t,
    vihasnext_uint32_t,
    vihasprev_uint32_t,
    vigetttbl_uint32_t
};

struct iterator_table *_vector_iterator_uint32_t_ = &itbl_vector_uint32_t;







vector_uint32_t *vnew_uint32_t(void) {
    vector_uint32_t *v = vallocate_uint32_t();
    vinit_uint32_t(v, 16);
    return v;
}







vector_uint32_t *vnewr_uint32_t(size_t n) {
    vector_uint32_t *v = vallocate_uint32_t();
    vinit_uint32_t(v, n);
    return v;
}

vector_uint32_t *vnewfill_uint32_t(size_t n, uint32_t val) {
    vector_uint32_t *v = NULL;
    uint32_t *sentinel = NULL;

    v = vnewr_uint32_t(n);
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

    return v;
}

vector_uint32_t *vnewfillptr_uint32_t(size_t n, uint32_t *valaddr) {
    vector_uint32_t *v = NULL;
    uint32_t *sentinel = NULL;

    v = vnewr_uint32_t(n);
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

    return v;
}

vector_uint32_t *vnewrnge_uint32_t(iterator first, iterator last) {
    int delta = 0;

    struct typetable *ttbl_first = NULL;
    vector_uint32_t *v = NULL;

    uint32_t *sentinel = NULL;
    uint32_t *curr = NULL;

    if (first.itbl != last.itbl) {

        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)331, "first and last must have matching container types and refer to the same container.");
        return NULL;
    }

    delta = it_distance(&first, &last);
    ttbl_first = it_get_ttbl(first);

    v = vnewr_uint32_t(delta);

    sentinel = it_curr(last);

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

vector_uint32_t *vnewcopy_uint32_t(vector_uint32_t *v) {
    vector_uint32_t *copy = NULL;

    uint32_t *sentinel = NULL;
    uint32_t *curr = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)378, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    copy = vnewr_uint32_t(vcapacity_uint32_t(v));

    sentinel = copy->impl.finish + vsize_uint32_t(v);
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

vector_uint32_t *vnewmove_uint32_t(vector_uint32_t **v) {
    vector_uint32_t *move = NULL;

    ;if ((((*v)) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "(*v)"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)412, ("['""(*v)""' was found to be NULL - '""(*v)""' must be nonnull to continue.]")); abort();};;

    move = vallocate_uint32_t();







    move->impl.start = (*v)->impl.start;
    move->impl.finish = (*v)->impl.finish;
    move->impl.end_of_storage = (*v)->impl.end_of_storage;
    move->ttbl = (*v)->ttbl;

    vinit_uint32_t((*v), 1);

    return move;
}

void vdelete_uint32_t(vector_uint32_t **v) {
    ;if ((((*v)) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "(*v)"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)449, ("['""(*v)""' was found to be NULL - '""(*v)""' must be nonnull to continue.]")); abort();};;

    vdeinit_uint32_t((*v));


    free((*v));
    (*v) = NULL;
}

iterator vbegin_uint32_t(vector_uint32_t *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)474, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return vibegin_uint32_t(v);
}

iterator vend_uint32_t(vector_uint32_t *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)487, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return viend_uint32_t(v);
}

size_t vsize_uint32_t(vector_uint32_t *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)499, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->impl.finish - v->impl.start;
}

size_t vmaxsize_uint32_t(vector_uint32_t *v) {
    size_t ptr_sz = 0;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)514, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;


    ptr_sz = (sizeof(void *) == 8) ? 8 : 4;
    return ((pow)(2.0, (ptr_sz * 8)) / (v->ttbl->width)) - 1;
}

void vresize_uint32_t(vector_uint32_t *v, size_t n) {
    size_t old_size = 0;
    size_t old_capacity = 0;

    uint32_t *target = NULL;
    uint32_t *newstart = NULL;

    int back_index = 0;
    int i = 0;

    size_t fin = 0;
    size_t end = 0;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)544, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    old_size = vsize_uint32_t(v);
    old_capacity = vcapacity_uint32_t(v);

    if (old_capacity == n) {
        return;
    } else if (n == 0) {
        ulog(stderr, "[WARNING]", "src/vector_tmpl.c", __func__, (long int)552, "vresize must receive a nonzero value for n.");
    }







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

void vresizefill_uint32_t(vector_uint32_t *v, size_t n, uint32_t val) {
    size_t old_capacity = 0;

    uint32_t *sentinel = NULL;
    uint32_t *newstart = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)601, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    old_capacity = vcapacity_uint32_t(v);

    sentinel = v->impl.start + n;

    if (n > old_capacity) {




        vresize_uint32_t(v, n);


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






        if (v->ttbl->dtor) {
            --v->impl.finish;

            while (v->impl.finish != v->impl.start) {
                v->ttbl->dtor(v->impl.finish--);
            }

            v->ttbl->dtor(v->impl.finish);
        }


        free(v->impl.start);
        v->impl.start = NULL;

        newstart = calloc(n, v->ttbl->width);
        ;if (((newstart) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "newstart"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)655, ("[Request for heap storage allocation failed (calloc returned NULL and was assigned to '""newstart""')]")); abort();};;


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

void vresizefillptr_uint32_t(vector_uint32_t *v, size_t n, uint32_t *valaddr) {
    size_t old_capacity = 0;

    uint32_t *sentinel = NULL;
    uint32_t *newstart = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)696, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    old_capacity = vcapacity_uint32_t(v);

    sentinel = v->impl.start + n;

    if (n > old_capacity) {




        vresize_uint32_t(v, n);


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






        if (v->ttbl->dtor) {
            --v->impl.finish;

            while (v->impl.finish != v->impl.start) {
                v->ttbl->dtor(v->impl.finish--);
            }

            v->ttbl->dtor(v->impl.finish);
        }


        free(v->impl.start);
        v->impl.start = NULL;

        newstart = calloc(n, v->ttbl->width);
        ;if (((newstart) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "newstart"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)750, ("[Request for heap storage allocation failed (calloc returned NULL and was assigned to '""newstart""')]")); abort();};;


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

size_t vcapacity_uint32_t(vector_uint32_t *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)779, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->impl.end_of_storage - v->impl.start;
}

bool vempty_uint32_t(vector_uint32_t *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)791, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;




    return v->impl.start == v->impl.finish;
}

void vreserve_uint32_t(vector_uint32_t *v, size_t n) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)800, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;





    if (n > vcapacity_uint32_t(v)) {
        vresize_uint32_t(v, n);
    } else {

        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)810, "n must be greater than vector's buffer capacity.");
        return;
    }
}

void vshrinktofit_uint32_t(vector_uint32_t *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)826, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    if ((v->impl.start != v->impl.finish) && (v->impl.finish != v->impl.end_of_storage)) {
        vresize_uint32_t(v, vsize_uint32_t(v));
    }
}

uint32_t *vat_uint32_t(vector_uint32_t *v, size_t n) {
    size_t size = 0;
    uint32_t *target = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)847, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    size = vsize_uint32_t(v);
    target = NULL;

    if (n >= size) {
        char str[256];
        sprintf(str, "Input %lu is greater than vector's logical length, %lu -- index out of bounds.", n, size);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)855, str);
        return NULL;
    } else if (n == 0) {

        target = v->impl.start;
    } else if (n == (size - 1)) {

        target = v->impl.finish - 1;
    } else {

        target = v->impl.start + n;
    }

    return target ? target : NULL;
}

uint32_t *vfront_uint32_t(vector_uint32_t *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)879, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->impl.start;
}

uint32_t *vback_uint32_t(vector_uint32_t *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)891, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->impl.finish - 1;
}

uint32_t **vdata_uint32_t(vector_uint32_t *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)903, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return &(v->impl.start);
}

const uint32_t *vatconst_uint32_t(vector_uint32_t *v, size_t n) {
    uint32_t *target = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)920, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    if (n >= vsize_uint32_t(v)) {
        char str[256];
        sprintf(str, "Input %lu is greater than vector's logical length, %lu -- index out of bounds.", n, vsize_uint32_t(v));
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)925, str);
        return NULL;
    }

    target = v->impl.start + n;
    return target ? target : NULL;
}

const uint32_t *vfrontconst_uint32_t(vector_uint32_t *v) {
    const uint32_t *result = (const uint32_t *)(v->impl.start);
    return result;
}

const uint32_t *vbackconst_uint32_t(vector_uint32_t *v) {
    const uint32_t *result = (const uint32_t *)(v->impl.finish - 1);
    return result;
}

const uint32_t **vdataconst_uint32_t(vector_uint32_t *v) {
    const uint32_t **result = (const uint32_t **)(&v->impl.start);
    return result;
}

void vassignrnge_uint32_t(vector_uint32_t *v, iterator first, iterator last) {
    int delta = it_distance(&first, &last);
    struct typetable *ttbl_first = it_get_ttbl(first);

    uint32_t *sentinel = it_curr(last);
    uint32_t *curr = NULL;

    if (first.itbl != last.itbl) {

        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)999, "first and last must matching container types and refer to the same container.");
        return;
    }




    vclear_uint32_t(v);




    if (delta >= vcapacity_uint32_t(v)) {
        vresize_uint32_t(v, delta);
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

void vassignfill_uint32_t(vector_uint32_t *v, size_t n, uint32_t val) {
    uint32_t *sentinel = NULL;




    vclear_uint32_t(v);




    if (n > vcapacity_uint32_t(v)) {
        vresize_uint32_t(v, n);
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

void vassignfillptr_uint32_t(vector_uint32_t *v, size_t n, uint32_t *valaddr) {
    uint32_t *sentinel = NULL;




    vclear_uint32_t(v);




    if (n > vcapacity_uint32_t(v)) {
        vresize_uint32_t(v, n);
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

void vpushb_uint32_t(vector_uint32_t *v, uint32_t val) {
   ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1124, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;





    if (v->impl.finish == v->impl.end_of_storage) {
        vresize_uint32_t(v, vcapacity_uint32_t(v) * 2);
    }

    if (v->ttbl->copy) {

        v->ttbl->copy(v->impl.finish++, &val);
    } else {

        memcpy(v->impl.finish++, &val, v->ttbl->width);
    }


}

void vpushbptr_uint32_t(vector_uint32_t *v, uint32_t *valaddr) {
   ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1157, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
   ;if (((valaddr) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "valaddr"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1158, ("['""valaddr""' was found to be NULL - '""valaddr""' must be nonnull to continue.]")); abort();};;





    if (v->impl.finish == v->impl.end_of_storage) {
        vresize_uint32_t(v, vcapacity_uint32_t(v) * 2);
    }

    if (v->ttbl->copy) {

        v->ttbl->copy(v->impl.finish++, valaddr);
    } else {

        memcpy(v->impl.finish++, valaddr, v->ttbl->width);
    }


}

void vpopb_uint32_t(vector_uint32_t *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1196, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    if (v->impl.finish == v->impl.start) {

        return;
    }


    --v->impl.finish;

    if (v->ttbl->dtor) {




        v->ttbl->dtor(v->impl.finish);
    }





}

iterator vinsert_uint32_t(vector_uint32_t *v, iterator pos, uint32_t val) {
    uint32_t *curr = NULL;
    size_t ipos = 0;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1240, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;





    if (v->impl.finish == v->impl.end_of_storage) {
        vresize_uint32_t(v, vcapacity_uint32_t(v) * 2);
    }





    if (v->ttbl->copy) {

        v->ttbl->copy(v->impl.finish, &val);
    } else {

        memcpy(v->impl.finish, &val, v->ttbl->width);
    }







    ipos = it_distance(NULL, &pos);





    while ((curr = it_curr(pos)) != v->impl.finish) {
        vswapaddr_uint32_t(v, curr, v->impl.finish);
        it_incr(&pos);
    }


    ++v->impl.finish;


    return it_next_n(vbegin_uint32_t(v), ipos);
}

iterator vinsertptr_uint32_t(vector_uint32_t *v, iterator pos, uint32_t *valaddr) {
    uint32_t *curr = NULL;
    size_t ipos = 0;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1306, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    ;if (((valaddr) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "valaddr"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1307, ("['""valaddr""' was found to be NULL - '""valaddr""' must be nonnull to continue.]")); abort();};;





    if (v->impl.finish == v->impl.end_of_storage) {
        vresize_uint32_t(v, vcapacity_uint32_t(v) * 2);
    }





    if (v->ttbl->copy) {

        v->ttbl->copy(v->impl.finish, valaddr);
    } else {

        memcpy(v->impl.finish, valaddr, v->ttbl->width);
    }







    ipos = it_distance(NULL, &pos);





    while ((curr = it_curr(pos)) != v->impl.finish) {
        vswapaddr_uint32_t(v, curr, v->impl.finish);
        it_incr(&pos);
    }


    ++v->impl.finish;


    return it_next_n(vbegin_uint32_t(v), ipos);
}

iterator vinsertfill_uint32_t(vector_uint32_t *v, iterator pos, size_t n, uint32_t val) {
    size_t ipos = 0;
    size_t old_size = 0;
    size_t old_capacity = 0;

    uint32_t *sentinel = NULL;
    uint32_t *right_adj = NULL;
    uint32_t *finish = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1379, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    ipos = it_distance(NULL, &pos);
    old_size = vsize_uint32_t(v);
    old_capacity = vcapacity_uint32_t(v);

    if ((old_size + n) >= old_capacity) {




        vresize_uint32_t(v, (old_capacity + n) * 2);
        pos = it_next_n(vbegin_uint32_t(v), ipos);
    }

    if (n <= 0) {

        return pos;
    } else if (n == 1) {

        vinsert_uint32_t(v, pos, val);
        return it_next_n(vbegin_uint32_t(v), ipos);
    }

    if (ipos == old_size - 1 || ipos == old_size) {

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


        return it_next_n(vbegin_uint32_t(v), n);
    } else {

        --v->impl.finish;

        sentinel = (uint32_t *)(pos.curr) - 1;


        right_adj = v->impl.finish + n;


        finish = v->impl.finish + (n + 1);






        while (v->impl.finish != sentinel) {
            vswapaddr_uint32_t(v, v->impl.finish--, right_adj--);
        }





        sentinel = v->impl.start + (ipos + n);







        ++v->impl.finish;

        if (v->ttbl->copy) {

            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish++, &val);
            }
        } else {

            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish++, &val, v->ttbl->width);
            }
        }


        v->impl.finish = finish;
    }

    return it_next_n(vbegin_uint32_t(v), ipos);
}

iterator vinsertfillptr_uint32_t(vector_uint32_t *v, iterator pos, size_t n, uint32_t *valaddr) {
    size_t ipos = 0;
    size_t old_size = 0;
    size_t old_capacity = 0;

    uint32_t *sentinel = NULL;
    uint32_t *right_adj = NULL;
    uint32_t *finish = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1508, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    ipos = it_distance(NULL, &pos);
    old_size = vsize_uint32_t(v);
    old_capacity = vcapacity_uint32_t(v);

    if ((old_size + n) >= old_capacity) {




        vresize_uint32_t(v, (old_capacity + n) * 2);
        pos = it_next_n(vbegin_uint32_t(v), ipos);
    }

    if (n <= 0) {

        return pos;
    } else if (n == 1) {

        vinsertptr_uint32_t(v, pos, valaddr);
        return it_next_n(vbegin_uint32_t(v), ipos);
    }

    if (ipos == old_size - 1 || ipos == old_size) {

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


        return it_next_n(vbegin_uint32_t(v), n);
    } else {

        --v->impl.finish;

        sentinel = (uint32_t *)(pos.curr) - 1;


        right_adj = v->impl.finish + n;


        finish = v->impl.finish + (n + 1);






        while (v->impl.finish != sentinel) {
            vswapaddr_uint32_t(v, v->impl.finish--, right_adj--);
        }





        sentinel = v->impl.start + (ipos + n);







        ++v->impl.finish;

        if (v->ttbl->copy) {

            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish++, valaddr);
            }
        } else {

            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish++, valaddr, v->ttbl->width);
            }
        }


        v->impl.finish = finish;
    }

    return it_next_n(vbegin_uint32_t(v), ipos);
}

iterator vinsertrnge_uint32_t(vector_uint32_t *v, iterator pos,
                        iterator first, iterator last) {
    size_t ipos = 0;
    size_t old_size = 0;
    size_t old_capacity = 0;
    size_t delta = 0;

    uint32_t *sentinel = NULL;
    uint32_t *right_adj = NULL;
    uint32_t *finish = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1653, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    ipos = it_distance(NULL, &pos);
    old_size = vsize_uint32_t(v);
    old_capacity = vcapacity_uint32_t(v);
    delta = it_distance(&first, &last);

    if ((old_size + delta) >= old_capacity) {





        vresize_uint32_t(v, (old_capacity + delta) * 2);
        pos = it_next_n(vbegin_uint32_t(v), ipos);
    }

    if (delta <= 0) {

        return pos;
    } else if (delta == 1) {

        vinsert_uint32_t(v, pos, *(uint32_t *)it_curr(first));
        return it_next_n(vbegin_uint32_t(v), ipos);
    }

    if (ipos >= old_size - 1) {

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


        return it_next_n(vbegin_uint32_t(v), delta);
    } else {

        --v->impl.finish;

        sentinel = (uint32_t *)(pos.curr) - 1;


        right_adj = v->impl.finish + delta;


        finish = v->impl.finish + (delta + 1);






        while (v->impl.finish != sentinel) {
            vswapaddr_uint32_t(v, v->impl.finish--, right_adj--);
        }





        sentinel = (v->impl.start) + (ipos + delta);






        ++v->impl.finish;

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


        v->impl.finish = finish;
    }

    return it_next_n(vbegin_uint32_t(v), ipos);
}

iterator vinsertmove_uint32_t(vector_uint32_t *v, iterator pos, uint32_t *valaddr) {
    uint32_t *dst = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1780, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    ;if (((valaddr) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "valaddr"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1781, ("['""valaddr""' was found to be NULL - '""valaddr""' must be nonnull to continue.]")); abort();};;

    if (v->ttbl->swap) {

        v->ttbl->swap(&dst, valaddr);
    }

    return vinsertptr_uint32_t(v, pos, dst);
}

iterator verase_uint32_t(vector_uint32_t *v, iterator pos) {
    int ipos = 0;
    size_t back_index = 0;

    uint32_t *curr = NULL;
    uint32_t *sentinel = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1834, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    ipos = it_distance(NULL, &pos);
    back_index = vsize_uint32_t(v) - 1;

    if ((ipos < 0) || (ipos >= back_index + 1)) {




        return pos;
    } else if (ipos == back_index || pos.curr == (v->impl.finish - 1)) {





        vpopb_uint32_t(v);






        return it_next_n(vbegin_uint32_t(v), ipos);
    } else if (ipos < back_index && ipos >= 0) {
        if (v->ttbl->dtor) {

            v->ttbl->dtor(pos.curr);
        }

        curr = NULL;
        sentinel = v->impl.finish;






        while ((curr = it_curr(pos)) != sentinel) {






            vswapaddr_uint32_t(v, curr, it_curr(it_next(pos)));
            it_incr(&pos);
        }


        --v->impl.finish;
    }






    return it_next_n(vbegin_uint32_t(v), ipos);
}

iterator verasernge_uint32_t(vector_uint32_t *v, iterator pos, iterator last) {
    int ipos = 0;
    int delta = 0;

    size_t back_index = 0;

    uint32_t *curr = NULL;
    uint32_t *sentinel = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)1923, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    ipos = it_distance(NULL, &pos);
    delta = it_distance(&pos, &last);

    back_index = vsize_uint32_t(v) - 1;

    if ((ipos < 0) || (ipos >= back_index + 1)) {




        return pos;
    } else if (ipos == back_index || pos.curr == (v->impl.finish - 1)) {





        vpopb_uint32_t(v);






        return it_next_n(vbegin_uint32_t(v), ipos);
    } else if (ipos < back_index && ipos >= 0) {
        curr = NULL;
        sentinel = it_curr(last);

        if (v->ttbl->dtor) {

            while ((curr = it_curr(pos)) != sentinel) {
                v->ttbl->dtor(curr);
                it_incr(&pos);
            }
        }


        pos = it_next_n(vbegin_uint32_t(v), ipos);


        sentinel = v->impl.finish;






        while ((curr = it_curr(last)) != sentinel) {
            vswapaddr_uint32_t(v, curr, it_curr(pos));

            it_incr(&pos);
            it_incr(&last);
        }





        v->impl.finish = v->impl.finish - delta;
    }






    return it_next_n(vbegin_uint32_t(v), ipos);
}







void vswap_uint32_t(vector_uint32_t **v, vector_uint32_t * *other) {
    vector_uint32_t *temp = NULL;

    ;if ((((*v)) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "(*v)"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2004, ("['""(*v)""' was found to be NULL - '""(*v)""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    ;if ((((*other)) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "(*other)"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2005, ("['""(*other)""' was found to be NULL - '""(*other)""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    temp = (*v);


    (*v)->impl.start = (*other)->impl.start;
    (*v)->impl.finish = (*other)->impl.finish;
    (*v)->impl.end_of_storage = (*other)->impl.end_of_storage;


    (*v)->ttbl = (*other)->ttbl;

    (*other)->impl.start = temp->impl.start;
    (*other)->impl.finish = temp->impl.finish;
    (*other)->impl.end_of_storage = temp->impl.end_of_storage;

    (*other)->ttbl = temp->ttbl;
}

void vclear_uint32_t(vector_uint32_t *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2035, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    if (v->impl.finish == v->impl.start) {





        return;
    }

    if (v->ttbl->dtor) {







        --v->impl.finish;

        while (v->impl.finish != v->impl.start) {
            v->ttbl->dtor(v->impl.finish--);
        }

        v->ttbl->dtor(v->impl.finish);
        bzero(v->impl.start, vsize_uint32_t(v));

    } else {





        bzero(v->impl.start, vsize_uint32_t(v));
        v->impl.finish = v->impl.start;
    }
}

void vinsertat_uint32_t(vector_uint32_t *v, size_t index, uint32_t val) {
    size_t size = 0;

    uint32_t *curr = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2091, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    size = vsize_uint32_t(v);

    if (index >= size) {
        char str[256];
        sprintf(str, "index provided [%lu] is out of bounds. size of vector is %lu.", index, size);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2098, str);
        return;
    }





    if (v->impl.finish == v->impl.end_of_storage) {
        vresize_uint32_t(v, vcapacity_uint32_t(v) * 2);
    }






    if (v->ttbl->copy) {

        v->ttbl->copy(v->impl.finish, &val);
    } else {

        memcpy(v->impl.finish, &val, v->ttbl->width);
    }

    curr = v->impl.start + index;
    while (curr != v->impl.finish) {
        vswapaddr_uint32_t(v, curr++, v->impl.finish);
    }


    ++v->impl.finish;
}

void vinsertatptr_uint32_t(vector_uint32_t *v, size_t index, uint32_t *valaddr) {
    size_t size = 0;

    uint32_t *curr = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2157, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    ;if (((valaddr) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "valaddr"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2158, ("['""valaddr""' was found to be NULL - '""valaddr""' must be nonnull to continue.]")); abort();};;

    size = vsize_uint32_t(v);

    if (index >= size) {
        char str[256];
        sprintf(str, "index provided [%lu] is out of bounds. size of vector is %lu.", index, size);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2165, str);
        return;
    }





    if (v->impl.finish == v->impl.end_of_storage) {
        vresize_uint32_t(v, vcapacity_uint32_t(v) * 2);
    }






    if (v->ttbl->copy) {

        v->ttbl->copy(v->impl.finish, valaddr);
    } else {

        memcpy(v->impl.finish, valaddr, v->ttbl->width);
    }

    curr = v->impl.start + index;
    while (curr != v->impl.finish) {
        vswapaddr_uint32_t(v, curr++, v->impl.finish);
    }


    ++v->impl.finish;
}

void veraseat_uint32_t(vector_uint32_t *v, size_t index) {
    size_t size = 0;
    size_t back_index = 0;

    uint32_t *curr = NULL;
    uint32_t *next = NULL;
    uint32_t *sentinel = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2229, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    size = vsize_uint32_t(v);

    if (index >= size) {
        char str[256];
        sprintf(str, "index provided [%lu] is out of bounds. size of vector is %lu.", index, size);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2236, str);
        return;
    }

    back_index = size - 1;

    curr = v->impl.start + index;
    next = index <= vcapacity_uint32_t(v) ? (curr + 1) : NULL;
    sentinel = v->impl.finish;

    if (index == back_index || curr == (v->impl.finish - 1)) {





        vpopb_uint32_t(v);
        return;
    } else if (index < back_index && index >= 0) {
        if (v->ttbl->dtor) {

            v->ttbl->dtor(curr);
        }

        while (curr != sentinel) {





            vswapaddr_uint32_t(v, curr++, next++);
        }


        --v->impl.finish;
    }
}

void vreplaceat_uint32_t(vector_uint32_t *v, size_t index, uint32_t val) {
    size_t size = 0;

    uint32_t *curr = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2301, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    size = vsize_uint32_t(v);

    if (index >= size) {
        char str[256];
        sprintf(str, "index provided [%lu] is out of bounds. size of vector is %lu.", index, size);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2308, str);
        return;
    }

    curr = v->impl.start + index;

    if (v->ttbl->dtor) {

        v->ttbl->dtor(curr);
    }

    if (v->ttbl->copy) {

        v->ttbl->copy(curr, &val);
    } else {

        memcpy(curr, &val, v->ttbl->width);
    }
}

void vreplaceatptr_uint32_t(vector_uint32_t *v, size_t index, uint32_t *valaddr) {
    size_t size = 0;

    uint32_t *curr = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2355, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    size = vsize_uint32_t(v);

    if (index >= size) {
        char str[256];
        sprintf(str, "index provided [%lu] is out of bounds. size of vector is %lu.", index, size);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2362, str);
        return;
    }

    curr = v->impl.start + index;

    if (v->ttbl->dtor) {

        v->ttbl->dtor(curr);
    }

    if (v->ttbl->copy) {

        v->ttbl->copy(curr, valaddr);
    } else {

        memcpy(curr, valaddr, v->ttbl->width);
    }
}

void vswapelem_uint32_t(vector_uint32_t *v, size_t n1, size_t n2) {
    size_t size = 0;
    size_t capacity = 0;

    bool n1_bad = 0;
    bool n2_bad = 0;
    bool good_indices = 0;

    uint32_t *temp = NULL;
    uint32_t *data_1 = NULL;
    uint32_t *data_2 = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2401, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    size = vsize_uint32_t(v);
    capacity = vcapacity_uint32_t(v);

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

void vremove_uint32_t(vector_uint32_t *v, uint32_t val) {
    size_t i = 0;
    size_t size = 0;

    uint32_t *curr = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2459, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    if (v->impl.start == v->impl.finish) {
        return;
    }

    i = 0;
    size = vsize_uint32_t(v);
    curr = v->impl.start;

    if (v->ttbl->compare(curr, &val) == 0) {
        veraseat_uint32_t(v, i);
        --size;
        ++curr;
    }

    for (i = 1; i < size; i++) {
        if (v->ttbl->compare(curr, &val) == 0) {
            --curr;
            veraseat_uint32_t(v, i--);
            --size;
        }

        ++curr;
    }
}

void vremoveif_uint32_t(vector_uint32_t *v, bool (*unary_predicate)(const void *)) {
    size_t i = 0;
    size_t size = 0;

    uint32_t *curr = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2501, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    ;if (((unary_predicate) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "unary_predicate"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2502, ("['""unary_predicate""' was found to be NULL - '""unary_predicate""' must be assigned to a function with a matching prototype.]")); abort();};;

    if (v->impl.start == v->impl.finish) {
        return;
    }

    i = 0;
    size = vsize_uint32_t(v);
    curr = v->impl.start;

    if (unary_predicate(curr) == 1) {
        veraseat_uint32_t(v, i);
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

vector_uint32_t *vmerge_uint32_t(vector_uint32_t *v, vector_uint32_t * other) {
    size_t size_other = 0;
    size_t capacity_v = 0;

    uint32_t *sentinel = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2545, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    ;if (((other) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "other"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2546, ("['""other""' was found to be NULL - '""other""' must be nonnull to continue.]")); abort();};;

    size_other = vsize_uint32_t(v);
    capacity_v = vcapacity_uint32_t(v);

    if (size_other >= capacity_v) {





        vresize_uint32_t(v, capacity_v * 2);
    }

    sentinel = other->impl.finish;
    other->impl.finish = other->impl.start;


    if ((v->ttbl->copy && other->ttbl->copy)
        && (v->ttbl->copy == other->ttbl->copy)) {

        while (other->impl.finish != sentinel) {
            v->ttbl->copy(v->impl.finish++, other->impl.finish++);
        }
    } else {

        while (other->impl.finish != sentinel) {
            memcpy(v->impl.finish++, other->impl.finish++, v->ttbl->width);
        }
    }

    return v;
}






void vreverse_uint32_t(vector_uint32_t *v) {
    uint32_t *back = NULL;
    uint32_t *restore = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2607, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    back = v->impl.finish - 1;





    restore = v->impl.finish;
    v->impl.finish = v->impl.start;

    while (v->impl.finish != back) {

        vswapaddr_uint32_t(v, v->impl.finish++, back--);





    }





    v->impl.finish = restore;
}

vector_uint32_t *varrtov_uint32_t(uint32_t *base, size_t length) {
    vector_uint32_t *v = NULL;

    uint32_t *target = NULL;

    ;if (((base) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "base"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2649, ("['""base""' was found to be NULL - '""base""' must be nonnull to continue.]")); abort();};;

    v = vnewr_uint32_t(length);

    target = base;

    if (v->ttbl->copy) {

        while (v->impl.finish != v->impl.end_of_storage) {
            v->ttbl->copy(v->impl.finish++, target++);
        }
    } else {

        while (v->impl.finish != v->impl.end_of_storage) {
            memcpy(v->impl.finish++, target++, v->ttbl->width);
        }
    }

    return v;
}

vector_uint32_t *vptrtov_uint32_t(uint32_t *base, size_t length, size_t capacity) {
    vector_uint32_t *v = NULL;

    ;if (((base) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "base"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2692, ("['""base""' was found to be NULL - '""base""' must be nonnull to continue.]")); abort();};;

    v = vallocate_uint32_t();





    v->ttbl = _uint32_t_
    ? _uint32_t_ : _void_ptr_;

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







int vsearch_uint32_t(vector_uint32_t *v, uint32_t val) {
    int (*comparator)(const void *, const void *) = NULL;

    uint32_t *curr = NULL;
    bool found = 0;
    int result = 0;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2734, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    comparator = v->ttbl->compare ? v->ttbl->compare : void_ptr_compare;
    curr = v->impl.start;


    while (curr != v->impl.finish) {
        if (comparator(curr, &val) == 0) {
            found = 1;
            break;
        }

        ++curr;
        ++result;
    }


    return found ? result : -1;
}






void vsort_uint32_t(vector_uint32_t *v) {
    size_t size = 0;
    int (*comparator)(const void *, const void *) = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2763, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    size = vsize_uint32_t(v);

    if (size < 2) {

        return;
    }

    comparator = v->ttbl->compare
    ? v->ttbl->compare : void_ptr_compare;

    v_mergesort_iterative(v->impl.start, size, v->ttbl->width, comparator);
}






void vputs_uint32_t(vector_uint32_t *v) {

    vfputs_uint32_t(v, stdout);
}

void vputsf_uint32_t(vector_uint32_t *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim) {

    vfputsf_uint32_t(v, stdout, before, after, postelem, empty, breaklim);
}







void vfputs_uint32_t(vector_uint32_t *v, FILE *dest) {
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
            vsize_uint32_t(v), "Capacity", vcapacity_uint32_t(v), "Element size", v->ttbl->width,
            bytes_label, link);

    vfputsf_uint32_t(v, dest, buffer1, buffer2, postelem, empty, breaklim);
}

void vfputsf_uint32_t(vector_uint32_t *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim) {
    void (*print)(const void *, FILE *dest) = NULL;

    size_t size = 0;
    size_t i = 0;
    size_t curr = 0;

    uint32_t *target = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2865, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    ;if (((dest) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "dest"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2866, ("['""dest""' was found to be NULL - '""dest""' must be nonnull to continue.]")); abort();};;

    fprintf(dest, "%s", before ? before : "");

    print = v->ttbl->print ? v->ttbl->print : void_ptr_print;

    size = vsize_uint32_t(v);

    if (size == 0) {
        fprintf(dest, "%s\n", empty ? empty : "");
    } else {
        target = v->impl.start;

        for (i = 0, curr = 1; i < size; i++, curr++) {
            print(target, dest);


            fprintf(dest, "\t\t(%s%p%s)", "\x1B[0;36m", (void *)(target), "\x1B[0;0m");

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

void *tmpl_vector_copy_uint32_t(void *arg, const void *other) {
    vector_uint32_t **dest = NULL;
    vector_uint32_t **source = NULL;

    ;if (((other) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "other"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2913, ("['""other""' was found to be NULL - '""other""' must be nonnull to continue.]")); abort();};;

    dest = (vector_uint32_t **)(arg);
    source = (vector_uint32_t **)(other);

    (*dest) = vnewcopy_uint32_t((*source));

    return (*dest);
}






void tmpl_vector_dtor_uint32_t(void *arg) {
    vector_uint32_t **v = NULL;

    ;if (((arg) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "arg"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2931, ("['""arg""' was found to be NULL - '""arg""' must be nonnull to continue.]")); abort();};;

    v = (vector_uint32_t **)(arg);
    vdelete_uint32_t(v);
}







void tmpl_vector_swap_uint32_t(void *s1, void *s2) {
    vector_uint32_t **v1 = (vector_uint32_t **)(s1);
    vector_uint32_t **v2 = (vector_uint32_t **)(s2);

    if ((*v1)) {
        vswap_uint32_t(v1, v2);
    } else {
        (*v1) = (*v2);
        (*v2) = NULL;
    }
}

int tmpl_vector_compare_uint32_t(const void *c1, const void *c2) {
    vector_uint32_t *v1 = NULL;
    vector_uint32_t *v2 = NULL;

    vector_uint32_t *vec1 = NULL;
    vector_uint32_t *vec2 = NULL;

    size_t size1 = 0;
    size_t size2 = 0;
    size_t size = 0;

    uint32_t *target1 = NULL;
    uint32_t *target2 = NULL;

    int delta = 0;
    int i = 0;

    ;if (((c1) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "c1"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2983, ("['""c1""' was found to be NULL - '""c1""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    ;if (((c2) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "c2"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)2984, ("['""c2""' was found to be NULL - '""c2""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    v1 = *(vector_uint32_t **)(c1);
    v2 = *(vector_uint32_t **)(c2);

    if (v1->ttbl->compare != v2->ttbl->compare) {
        return -1;
    }

    vec1 = vnewcopy_uint32_t(v1);
    vec2 = vnewcopy_uint32_t(v2);

    vsort_uint32_t(vec1);
    vsort_uint32_t(vec2);

    size1 = vsize_uint32_t(vec1);
    size2 = vsize_uint32_t(vec2);

    size = size1 < size2 ? size1 : size2;

    target1 = vec1->impl.start;
    target2 = vec2->impl.start;

    for (i = 0; i < size; i++) {
        delta += vec1->ttbl->compare(&target1, &target2);

        ++target1;
        ++target2;
    }

    vdelete_uint32_t(&vec2);
    vdelete_uint32_t(&vec1);


    return delta;
}







void tmpl_vector_print_uint32_t(const void *arg, FILE *dest) {
    vector_uint32_t *v = NULL;

    ;if (((arg) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "arg"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3030, ("['""arg""' was found to be NULL - '""arg""' must be nonnull to continue.]")); abort();};;
    ;if (((dest) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "dest"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3031, ("['""dest""' was found to be NULL - '""dest""' must be nonnull to continue.]")); abort();};;

    v = *(vector_uint32_t **)(arg);
    vfputs_uint32_t(v, dest);
}







void vsetttbl_uint32_t(vector_uint32_t *v, struct typetable *ttbl) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3044, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    v->ttbl = ttbl ? ttbl : _void_ptr_;
}

size_t vgetwidth_uint32_t(vector_uint32_t *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3056, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->ttbl->width;
}

copy_fn vgetcopy_uint32_t(vector_uint32_t *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3068, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->ttbl->copy;
}

dtor_fn vgetdtor_uint32_t(vector_uint32_t *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3080, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->ttbl->dtor;
}

swap_fn vgetswap_uint32_t(vector_uint32_t *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3092, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->ttbl->swap;
}

compare_fn vgetcompare_uint32_t(vector_uint32_t *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3104, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->ttbl->compare;
}

print_fn vgetprint_uint32_t(vector_uint32_t *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3116, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->ttbl->print;
}

struct typetable *vgetttbl_uint32_t(vector_uint32_t *v) {
    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3128, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;
    return v->ttbl;
}






static vector_uint32_t *vallocate_uint32_t(void) {
    vector_uint32_t *v = NULL;
    v = malloc(sizeof *v);
    return v;
}







static void vinit_uint32_t(vector_uint32_t *v, size_t capacity) {
    uint32_t *start = NULL;

    ;if (((v) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "v"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3152, ("['""v""' was found to be NULL - '""v""' must be assigned to the return value of a container initializer function prior to use.]")); abort();};;

    v->ttbl = _uint32_t_
    ? _uint32_t_ : _void_ptr_;

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






static void vdeinit_uint32_t(vector_uint32_t *v) {
    if (v == NULL) {
        return;
    }

    vclear_uint32_t(v);

    free(v->impl.start);
    v->impl.start = NULL;
    v->impl.finish = NULL;
    v->impl.end_of_storage = NULL;

    v->ttbl = NULL;
}

static void vswapaddr_uint32_t(vector_uint32_t *v, uint32_t *first, uint32_t *second) {
    uint32_t *temp = NULL;

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

static iterator vibegin_uint32_t(void *arg) {
    vector_uint32_t *v = NULL;
    iterator it;

    v = (vector_uint32_t *)(arg);

    it.itbl = _vector_iterator_uint32_t_;
    it.container = v;
    it.curr = v->impl.start;

    return it;
}

static iterator viend_uint32_t(void *arg) {
    vector_uint32_t *v = NULL;
    iterator it;

    v = (vector_uint32_t *)(arg);

    it.itbl = _vector_iterator_uint32_t_;
    it.container = v;
    it.curr = v->impl.finish;

    return it;
}

static iterator vinext_uint32_t(iterator it) {
    vector_uint32_t *v = NULL;
    iterator iter;

    v = (vector_uint32_t *)(it.container);

    iter.itbl = _vector_iterator_uint32_t_;
    iter.container = v;
    iter.curr = it.curr;

    if (iter.curr == v->impl.finish) {
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3283, "Cannot advance - iterator already at end.");
    } else {
        iter.curr = (char *)(v->impl.finish) + (v->ttbl->width);
    }

    return iter;
}

static iterator vinextn_uint32_t(iterator it, int n) {
    vector_uint32_t *v = NULL;
    iterator iter;
    int pos = 0;

    v = (vector_uint32_t *)(it.container);

    iter.itbl = _vector_iterator_uint32_t_;
    iter.container = v;
    iter.curr = it.curr;

    pos = ptr_distance(v->impl.start, iter.curr, v->ttbl->width);

    if ((vsize_uint32_t(v) - pos) <= 0) {
        char str[256];
        sprintf(str, "Cannot advance %d times from position %d.", n, pos);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3315, str);
    } else {
        iter.curr = (char *)(iter.curr) + (n * v->ttbl->width);
    }

    return iter;
}

static iterator viprev_uint32_t(iterator it) {
    vector_uint32_t *v = (vector_uint32_t *)(it.container);
    iterator iter;

    v = (vector_uint32_t *)(it.container);

    iter.itbl = _vector_iterator_uint32_t_;
    iter.container = v;
    iter.curr = it.curr;

    if (iter.curr == v->impl.start) {
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3342, "Cannot retract - already at begin.");
    } else {
        iter.curr = (char *)(v->impl.finish) - (v->ttbl->width);
    }

    return iter;
}

static iterator viprevn_uint32_t(iterator it, int n) {
    vector_uint32_t *v = (vector_uint32_t *)(it.container);
    iterator iter;
    int pos = 0;

    v = (vector_uint32_t *)(it.container);

    iter.itbl = _vector_iterator_uint32_t_;
    iter.container = v;
    iter.curr = it.curr;

    pos = ptr_distance(v->impl.start, iter.curr, v->ttbl->width);

    if ((vsize_uint32_t(v) - pos) <= 0) {
        char str[256];
        sprintf(str, "Cannot retract %d times from position %d.", n, pos);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3374, str);
    } else {
        iter.curr = (char *)(iter.curr) + (n * v->ttbl->width);
    }

    return iter;
}

static int vidistance_uint32_t(iterator *first, iterator *last) {
    vector_uint32_t *v = NULL;

    if (first == NULL && last != NULL) {
        v = (vector_uint32_t *)(last->container);
        return (int)((uint32_t *)(last->curr) - v->impl.start);
    } else if (last == NULL && first != NULL) {
        v = (vector_uint32_t *)(first->container);
        return (int)((uint32_t *)(first->curr) - v->impl.start);
    } else if (first == NULL && last == NULL) {
        return 0;
    } else {
        v = (vector_uint32_t *)(first->container);
        return (int)((uint32_t *)(last->curr) - (uint32_t *)(first->curr));
    }
}

static iterator *viadvance_uint32_t(iterator *it, int n) {
    vector_uint32_t *v = NULL;
    int pos = 0;

    if (((it) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "it"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3425, ("['""it""' was found to be NULL - '""it""' must point to an initialized iterator, such that it refers to a non-null pointer-to-container.]")); abort();};;

    pos = ptr_distance(v->impl.start, it->curr, v->ttbl->width);

    if ((vsize_uint32_t(v) - pos) < 0) {
        char str[256];
        sprintf(str, "Cannot advance %d times from position %d.", n, pos);
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3432, str);
    } else {
        v = (vector_uint32_t *)(it->container);
        it->curr = (char *)(it->curr) + (n * v->ttbl->width);
    }

    return it;
}

static iterator *viincr_uint32_t(iterator *it) {
    vector_uint32_t *v = NULL;

    if (((it) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "it"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3451, ("['""it""' was found to be NULL - '""it""' must point to an initialized iterator, such that it refers to a non-null pointer-to-container.]")); abort();};;

    v = (vector_uint32_t *)(it->container);

    if (it->curr == v->impl.finish) {
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3456, "Cannot increment - already at end.");
    } else {
        it->curr = (char *)(it->curr) + (v->ttbl->width);
    }

    return it;
}

static iterator *videcr_uint32_t(iterator *it) {
    vector_uint32_t *v = NULL;

    if (((it) == (0))) { fprintf(stderr, "Assertion failed: (%s)\n", "it"); ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3474, ("['""it""' was found to be NULL - '""it""' must point to an initialized iterator, such that it refers to a non-null pointer-to-container.]")); abort();};;

    v = (vector_uint32_t *)(it->container);

    if (it->curr == v->impl.start) {
        ulog(stderr, "[ERROR]", "src/vector_tmpl.c", __func__, (long int)3479, "Cannot decrement - already at begin.");
    } else {
        it->curr = (char *)(it->curr) - (v->ttbl->width);
    }

    return it;
}

static void *vicurr_uint32_t(iterator it) {
    return it.curr;
}

static void *vistart_uint32_t(iterator it) {
    vector_uint32_t *v = (vector_uint32_t *)(it.container);
    return v->impl.start;
}

static void *vifinish_uint32_t(iterator it) {
    vector_uint32_t *v = (vector_uint32_t *)(it.container);
    return v->impl.finish;
}

static bool vihasnext_uint32_t(iterator it) {
    vector_uint32_t *v = (vector_uint32_t *)(it.container);
    return it.curr != v->impl.finish;
}

static bool vihasprev_uint32_t(iterator it) {
    vector_uint32_t *v = (vector_uint32_t *)(it.container);
    return it.curr != v->impl.start;
}

static struct typetable *vigetttbl_uint32_t(void *arg) {
    vector_uint32_t *v = (vector_uint32_t *)(arg);
    return v->ttbl;
}
