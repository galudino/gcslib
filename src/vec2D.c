/**
 *  @file       vec2D.c
 *  @brief      Source file for a two-dimensional vector (mathematics)
 *
 *  @author     Gemuele Aludino
 *  @date       26 Jun 2019
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

#include "vec2D.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VEC2D_ATTRIBUTE_COUNT 4
enum vec2D_attrs { X, Y, MAGNITUDE, THETA };

static vec2D *vec2D_allocate(void);
static void vec2D_update(vec2D *v);
static double calculate_hypotenuse(double a, double b);

vec2D *vec2D_ptr_new(double vx, double vy) {
    vec2D *v = vec2D_allocate();
    vec2D_init(v, vx, vy);
    return v;
}

void vec2D_ptr_delete(void *arg) {
    assert(arg);

    vec2D **v = (vec2D **)(arg);
    vec2D_deinit((*v));

    free(*v);
    (*v) = NULL;
}

void vec2D_init(vec2D *v, double vx, double vy) {
    assert(v);

    // Heap allocation
    double *attrs = NULL;
    attrs = calloc(VEC2D_ATTRIBUTE_COUNT, sizeof((*attrs)));
    assert(attrs);

    v->attrs = attrs;

    v->attrs[X] = vx;
    v->attrs[Y] = vy;

    vec2D_update(v);
}

void vec2D_deinit(vec2D *v) {
    assert(v);

    // Heap allocation
    free(v->attrs);
    v->attrs = NULL;
}

double vec2D_x(vec2D *v) {
    assert(v);
    return v->attrs[X];
}

double vec2D_y(vec2D *v) {
    assert(v);
    return v->attrs[Y];
}

void vec2D_set_x(vec2D *v, double vx) {
    assert(v);

    v->attrs[X] = vx;
    vec2D_update(v);
}

void vec2D_set_y(vec2D *v, double vy) {
    assert(v);

    v->attrs[Y] = vy;
    vec2D_update(v);
}

double vec2D_magnitude(vec2D *v) {
    assert(v);
    return v->attrs[MAGNITUDE];
}

double vec2D_angle_deg(vec2D *v) {
    assert(v);
    return v->attrs[THETA];
}

void vec2D_scale(vec2D *v, double scalar) {
    assert(v);

    v->attrs[X] *= scalar;
    v->attrs[Y] *= scalar;

    vec2D_update(v);
}

void vec2D_invertdir(vec2D *v) {
    assert(v);

    v->attrs[X] *= (-1);
    v->attrs[Y] *= (-1);

    vec2D_update(v);
}

vec2D *vec2D_add(vec2D *v1, vec2D *v2) {
    vec2D *resultant = vec2D_allocate();

    double rx = v1->attrs[X] + v2->attrs[X];
    double ry = v1->attrs[Y] + v2->attrs[Y];

    vec2D_init(resultant, rx, ry);
    return resultant;
}

vec2D *vec2D_subtract(vec2D *v1, vec2D *v2) {
    vec2D_invertdir(v2);
    return vec2D_add(v1, v2); // v1 - v2 == v1 + (-v2)
}

void *vec2D_copy(void *arg, const void *other) {
    assert(other);

    vec2D *dest = (vec2D *)(arg);
    vec2D *source = (vec2D *)(other);

    vec2D_init(dest, source->attrs[X], source->attrs[Y]);

    return dest;
}

void vec2D_dtor(void *arg) {
    assert(arg);
    vec2D *v = (vec2D *)(arg);
    vec2D_deinit(v);
}

void vec2D_swap(void *c1, void *c2) {
    vec2D *dest = (vec2D *)(c1);
    vec2D *source = (vec2D *)(c2);

    if (dest) {
        vec2D temp;
        vec2D_init(&temp, dest->attrs[X], dest->attrs[Y]);

        dest->attrs = source->attrs;
        source->attrs = temp.attrs;
    } else {
        dest->attrs = source->attrs;
        source->attrs = NULL;
    }
}

int vec2D_compare(const void *c1, const void *c2) {
    assert(c1);
    assert(c2);

    vec2D *v1 = (vec2D *)(c1);
    vec2D *v2 = (vec2D *)(c2);

    int compare_magnitude =
    double_compare(&v1->attrs[MAGNITUDE], &v2->attrs[MAGNITUDE]);

    // directions are compared iff magnitudes are equal
    // otherwise, magnitudes determine comparison outcome
    return compare_magnitude ? compare_magnitude :
    double_compare(&v1->attrs[THETA], &v2->attrs[THETA]);
}

void vec2D_print(const void *arg, FILE *dest) {
    assert(arg);
    assert(dest);

    vec2D *v = (vec2D *)(arg);

    fprintf(dest,
            "%s\n  v    = (%0.2fi, %0.2fj)\n||v||  =  %0.2f\n  %s    =  "
            "%0.2f%s\n%s\n",
            "=========================", v->attrs[X], v->attrs[Y],
            v->attrs[MAGNITUDE], THETA_LOW_SYMBOL, v->attrs[THETA],
            DEGREE_SYMBOL, "=========================");
}

void vec2D_print_override_simple(const void *arg, FILE *dest) {
    vec2D *v = (vec2D *)(arg);
    //fprintf(dest, "(%lfi, %lfj)", vec2D_x(v), vec2D_y(v));
    fprintf(dest, "%0.3lf at %0.3lf%s", v->attrs[MAGNITUDE], v->attrs[THETA], DEGREE_SYMBOL);
}

void *vec2D_ptr_copy(void *arg, const void *other) {
    assert(other);

    vec2D **dest = (vec2D **)(arg);
    vec2D **vec = (vec2D **)(other);

    (*dest) = vec2D_ptr_new((*vec)->attrs[X], (*vec)->attrs[Y]);
    return (*dest);
}

void vec2D_ptr_dtor(void *arg) {
    assert(arg);
    vec2D_ptr_delete(arg);
}

void vec2D_ptr_swap(void *c1, void *c2) {
    vec2D **dest = (vec2D **)(c1);
    vec2D **source = (vec2D **)(c2);

    if ((*dest)) {
        vec2D *temp;
        vec2D_ptr_copy(&temp, dest);
        (*dest)->attrs = (*source)->attrs;
        (*source)->attrs = temp->attrs;
    } else {
        (*dest) = (*source);
        (*source) = NULL;
    }
}

int vec2D_ptr_compare(const void *c1, const void *c2) {
    assert(c1);
    assert(c2);

    vec2D *v1 = *(vec2D **)(c1);
    vec2D *v2 = *(vec2D **)(c2);

    int compare_magnitude =
    double_compare(&v1->attrs[MAGNITUDE], &v2->attrs[MAGNITUDE]);

    // directions are compared iff magnitudes are equal
    // otherwise, magnitudes determine comparison outcome
    return compare_magnitude ? compare_magnitude :
    double_compare(&v1->attrs[THETA], &v2->attrs[THETA]);
}

void vec2D_ptr_print(const void *arg, FILE *dest) {
    assert(arg);
    assert(dest);

    vec2D *v = *(vec2D **)(arg);

    fprintf(dest,
            "%s\n  v    = (%0.2fi, %0.2fj)\n||v||  =  %0.2f\n  %s    =  "
            "%0.2f%s\n%s\n",
            "=========================", v->attrs[X], v->attrs[Y],
            v->attrs[MAGNITUDE], THETA_LOW_SYMBOL, v->attrs[THETA],
            DEGREE_SYMBOL, "=========================");
}

void vec2D_ptr_print_override_simple(const void *arg, FILE *dest) {
    vec2D *v = *(vec2D **)(arg);
    //fprintf(dest, "(%lfi, %lfj)", vec2D_x(v), vec2D_y(v));
    fprintf(dest, "%0.3lf at %0.3lf%s", v->attrs[MAGNITUDE], v->attrs[THETA], DEGREE_SYMBOL);
}

static vec2D *vec2D_allocate() {
    vec2D *v = NULL;
    v = malloc(sizeof(struct vec2D));
    assert(v);
    return v;
}

static void vec2D_update(vec2D *v) {
    assert(v);

    v->attrs[MAGNITUDE] = calculate_hypotenuse(v->attrs[X], v->attrs[Y]);

    double radians = atan(v->attrs[Y] / v->attrs[X]);
    double degrees = radians * (180.0 / M_PI);

    v->attrs[THETA] = degrees;
}

static double calculate_hypotenuse(double a, double b) {
    // c = sqrt(a^2 + b^2)
    return sqrt(pow(a, 2.0) + pow(b, 2.0));
}

TYPETABLE_DEFINE_PTR(vec2D);
TYPETABLE_DEFINE_PTR(vec2D_ptr);
